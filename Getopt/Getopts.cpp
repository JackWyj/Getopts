#include "stdafx.h"
#include "Getopts.h"


Getopts::Getopts(void)
{
	const int num = 1;
	const int num_long = 6;
	parse_level = 0;
	char ex_char[num] = {':'};
	char ex_char_long[num_long] = {'i', 'd', 'f', 's', '=', ';'};
	/*tmpch = '#';*/
	pre_chtype = NONE;
	for(int i = 0; i < num; i++){
		set_ex_char_short.insert(ex_char[i]);
	}
	for(int i = 0; i < num_long ; i++){
		set_ex_char_long.insert(ex_char_long[i]);
	}
	show_error = true;
	is_error = false;
}

Getopts::~Getopts(void)
{
}

std::string Getopts::getbuf(){
	return inbuf;
}

void Getopts::setbuf(const std::string& buf){
	inbuf = buf;
}

ch_type Getopts::is_char_legal(char c){
	//如果是数字或者字母
	if(isalpha(c) || isdigit(c)){
		if(set_short_used.find(c) == set_short_used.end() ){
			set_short_used.insert(c);
			return CHAR;
		}else 
			return NONE;
	}
	//如果是其他字符，如 “:”
	if(set_ex_char_short.find(c) != set_ex_char_short.end()){
		return EX;
	}else 
		return NONE;
}

ch_type Getopts::is_char_legal_long(char c){
	if(isalpha(c) || isdigit(c)){
		return CHAR;
	}
	else if(c == ';')
		return SEQ;
	else if(c == '=')
		return EQ;
	else
		return NONE;
}

bool Getopts::set_short_param(std::string  str, bool store, ...){
	if(str.size() == 0) return error_handle();
	

	std::vector<char> v_keys;
	parse_level = 1;
	char c, tmpch = '#';
	int len = str.size();
	for(int i = 0; i < len; i++){
		c = str[i];
		ch_type chtype;
		chtype = is_char_legal(c);
		if(NONE == chtype){
			std::string s = "未能识别的字符<" + string(&c, 1) + ">\n";
			error_msg(s.c_str());
			return error_handle();
		}
		if(chtype == EX){
			if(pre_chtype != CHAR){
				std::string s = "未能识别的字符<" + string(&c, 1) + ">\n";
				error_msg(s.c_str());
				return error_handle();
			}
			if(pre_chtype == CHAR){
				map_short_func[tmpch] = ShortFunction(tmpch, true);
				v_keys.push_back(tmpch);
			}

		}else if(chtype == CHAR){
			if(pre_chtype == CHAR){
				map_short_func[tmpch] = ShortFunction(tmpch, false);
				v_keys.push_back(tmpch);
			}
		}
		tmpch = c;
		pre_chtype = chtype;
	}
	if(pre_chtype == CHAR){
		map_short_func[tmpch] = ShortFunction(tmpch, false);
		v_keys.push_back(tmpch);
	}

	
	return true;
}

bool Getopts::set_long_param(std::string  str, bool store, ...){
	if(str.size() == 0) return error_handle();
	if(store)
		va_start(vargs, store), vstore_long = true;

	parse_level = 2;
	char c, pre_ch = '#';
	std::string key = "";
	int len = str.size();
	ch_type pre_chtype = NONE;
	bool begin = false;
	std::string carryty;
	for(int i = 0; i < len; i++){
		c = str[i];
		ch_type chtype;
		chtype = is_char_legal_long(c);
		if(NONE == chtype){
			std::string errmsg = "未能识别的字符<" + string(&c, 1) + ">\n";
			error_msg(errmsg.c_str());
			return error_handle();
		}
		if(SEQ == chtype){
			if(pre_chtype != CHAR){
				error_msg( "分号之前的字符不是字母类型的");
				return error_handle();
			}    
			if(! key.empty()){
				if(begin){
					param_type paramty = long_carry_type(carryty);
					if(paramty == NO) {
						error_msg( "参数类型错误\n");
						vec_keys.clear();
						return error_handle();
					}else{
						
						if(map_long_func.find(key) != map_long_func.end()){
							std::string s =  "参数 <" + key + "> 重复出现\n";
							error_msg(s.c_str());
							vec_keys.clear();
							return error_handle();
						}
						else
							map_long_func[key] = LongFunction(key, true, paramty);

					}
				}else {
					map_long_func[key] = LongFunction(key, false);
				}
				vec_keys.push_back(key);
				
			}
			carryty = "";
			key = "";
			begin = false;
		}else if(EQ == chtype){
			if(pre_chtype != CHAR){
				error_msg( "等号前面不是字母\n");
				return error_handle();
			}			
		}else if(CHAR == chtype){
			if(begin){
				carryty += c;
			}else{
				if(pre_chtype == EQ){
					begin = true; 
					carryty += c;
				}else 
					key += c;
			}
		}
		pre_chtype = chtype;
		pre_ch = c;
	}

	if(vstore_long){
		size_t len = vec_keys.size();
		std::string key;
		try{
			for(size_t i = 0; i < len; i++){
				key = vec_keys[i];			
				if(map_long_func[key].haveParam()){
					param_type pt = map_long_func[key].carry_param_type();
					switch(pt){
					case INT: {
						int *ii = va_arg(vargs, int*); 
						v_ptr.push_back(ii); break;}
					case DOUBLE: {
						double *d = va_arg(vargs, double*); 
						v_ptr.push_back(d); break;}
					case FLOAT:{ 
						float *f = va_arg(vargs, float*);
						v_ptr.push_back(f); break;}
					case STRING: {
						std::string *s = va_arg(vargs, std::string*); 
						v_ptr.push_back(s); break;}
					case VINT: {
						std::vector<int> *vi = va_arg(vargs, std::vector<int>*); 
						v_ptr.push_back(vi); break;}
					case VFLOAT: {
						std::vector<float> *vf = va_arg(vargs, std::vector<float>*);
						v_ptr.push_back(vf); break;}
					case VDOUBLE: {
						std::vector<double> *vd = va_arg(vargs, std::vector<double>*);
						v_ptr.push_back(vd); break;}
					case VSTRING:{ 
						std::vector<std::string> *vs = va_arg(vargs, std::vector<std::string>*);
						v_ptr.push_back(vs); break;}
					default: error_msg("未能识别的类型\n"); return error_handle();
					}
				}else{
					bool* b = va_arg(vargs, bool*);
					v_ptr.push_back(b);
				}
			}
		}catch(exception &e){
			std::string err = "错误: " + string(e.what()) ;
			error_msg(err.c_str());
			return error_handle();
		}
	}
	return true;
}

in_ch Getopts::getch(){
	static int i = -1;
	i++;
	if(i == inbuf.size()) 
		return in_ch(true);
	else 
		return in_ch(inbuf[i], false);
}

bool Getopts::parse_short(){
	in_ch inc = getch();
	static char pre_ch = '#', cur_ch, pre_key ;
	static bool begin;
	static std::string label;
	if(!inc.end )
		cur_ch = inc.c;
	else {
		if(begin && label.size() != 0)
			map_ch_carry[pre_key].push_back(label);
		return true;
	}

	if(pre_ch == '-' && ( isalpha(cur_ch) || isdigit(cur_ch) )){
		if(set_short_used.find(cur_ch) != set_short_used.end() ){
			pre_key = cur_ch;
			if(true == map_short_func[cur_ch].haveParam()){
				begin = true;
				std::vector<std::string> vec;
				if(map_ch_carry.find(cur_ch) == map_ch_carry.end())
					map_ch_carry[cur_ch] = vec;
			}else{
				
				begin = false;
				do {
					set_short_param_on.insert(cur_ch);
					inc = getch();
					if(inc.end == true){
						return error_handle();
					}
					cur_ch = inc.c;
				}while(set_short_used.find(cur_ch) != set_short_used.end() &&
					!map_short_func[cur_ch].haveParam());
			}
		}
	}
	else if(begin && cur_ch == '\"'){
		
		label = "";
		do {
			inc = getch();
			if(inc.end == true){
				error_msg("缺少右双引号\n");
				return error_handle();
			}
			if(inc.c != '\"')
				label += inc.c;
		}while(inc.c != '\"');
			
	
	}else if(begin){
		in_ch inch;
		bool space = false;
		while(cur_ch == ' '){
			space = true;
			inch = getch();
			if(inch.end)  return error_handle();
			cur_ch = inch.c;
		}
	
		if(space && label.size() != 0){
			map_ch_carry[pre_key].push_back(label);
			label = "";
			begin = false;
		}else
			label += cur_ch;
	}
	
	pre_ch = cur_ch;
	if(parse_short()) 
		return true;
	else 
		return error_handle();
}

void Getopts::parse_param(const std::string & in){
	if(is_error) return;
	inbuf = in;
	if(parse_level == 1)
		parse_short();
	else if(parse_level == 2){
		parse_long();
		store_carry_param_long();
	}
	help();
}

void Getopts::help(){

}

inline param_type Getopts::long_carry_type(const std::string & c){
	if(c == "i") return INT;
	if(c == "f") return FLOAT;
	if(c == "d") return DOUBLE;
	if(c == "s") return STRING;
	if(c == "vi") return VINT;
	if(c == "vf") return VFLOAT;
	if(c == "vd") return VDOUBLE;
	if(c == "vs") return VSTRING;
	else return NO;
}

bool Getopts::parse_long(){
	in_ch inc = getch();
	static char pre_ch = '#', cur_ch;
	static std::string pre_key ;
	static bool begin;
	static std::string label, mk;
	if(!inc.end){
		cur_ch = inc.c;
	}else{
		if(begin && label.size() != 0)
			map_long_carry[pre_key].push_back(label);
		return true;
	}
	
	if(pre_ch == '-'){
		mk = "";
		while(cur_ch != '=' && cur_ch != ' '
			&& (isalpha(cur_ch) || isdigit(cur_ch))){
			mk += cur_ch;
			inc = getch();
			if(inc.end) return error_handle();
			cur_ch = inc.c;
		};
		key_chosen kc = pick_key(mk);
		if(!kc.error){
			pre_key = kc.key;
			begin = false;
			label = "";
			if(map_long_func[kc.key].haveParam()){
				begin = true;
				if(map_long_carry.find(kc.key) == map_long_carry.end()){
					std::vector<std::string> vec;
					map_long_carry[kc.key] = vec;
				}
			}else
				set_long_param_on.insert(kc.key);
		}
	}else if(begin && cur_ch == '\"'){
		
		label = "";
		do {
			inc = getch();
			if(inc.end == true){
				cout << "缺少右双引号\n";
				return error_handle();
			}
			if(inc.c != '\"')
				label += inc.c;
		}while(inc.c != '\"');
			
	
	}else if(begin){
		in_ch inch;
		bool space = false;
		while(cur_ch == ' '){
			space = true;
			inch = getch();
			if(inch.end) return error_handle();
			cur_ch = inch.c;
		}
		
		if(space && label.size() != 0){
			map_long_carry[pre_key].push_back(label);
			label = "";
			begin = false;
		}else
			label += cur_ch; 
	}
	
	pre_ch = cur_ch;
	return parse_long();

}

key_chosen Getopts::pick_key(const std::string sk){
	if(sk.empty()){
		return key_chosen(true);
	}
	std::map<std::string, LongFunction>::iterator iter;
	bool choose = false;
	std::string key, ss;
	key_chosen kc;
	for(iter = map_long_func.begin(); iter != map_long_func.end(); iter++){
		key = iter->first;
		if(key.size() < sk.size()) continue;
		ss = key.substr(0, sk.size());
		if(sk == ss){
			if(choose) return key_chosen(true);
			choose = true, kc.key = key, kc.error = false;
		}
	}
	return kc;
}

void Getopts::store_carry_param_long(){ 
	if(!vstore_long || is_error) return;
	std::string key;
	try{
		for(size_t i = 0; i < vec_keys.size(); i++){
			key = vec_keys[i];		
			if(map_long_func[key].haveParam()){
				if(map_long_carry[key].size() < 1){
					std::string emsg = "键值<" + key + ">没有携带参数\n";
					error_msg(emsg.c_str());
					return;
				}			
				param_type pt = map_long_func[key].carry_param_type();
				switch(pt){
				case INT: {
					int* ii = (int*)v_ptr[i];
					*ii= atoi(map_long_carry[key].front().c_str());
					break;
						  };
				case DOUBLE:; {
					double* d = (double*)v_ptr[i];;
					*d = atof(map_long_carry[key].front().c_str());
					break;
							}
				case FLOAT:;{
					float* f = (float*)v_ptr[i];
					std::string str = map_long_carry[key].front();
					double d = atof(str.c_str());
					float ff = (float)d;
					*f = ff;
					break;
						   }
				case STRING:;{
					std::string *s = (std::string*)v_ptr[i];
					*s = map_long_carry[key].front();
					break;
					}
				case VINT:;{
					std::vector<int> *vi = (std::vector<int>*)v_ptr[i];
					int len = map_long_carry[key].size();
					for(int i = 0; i < len; i++){
						vi->push_back(atoi(map_long_carry[key][i].c_str()));
					}
					break;
						  }
				case VDOUBLE:;{
					std::vector<double> *vi = (std::vector<double>*)v_ptr[i];
					int len = map_long_carry[key].size();
					for(int i = 0; i < len; i++){
						vi->push_back(atof(map_long_carry[key][i].c_str()));
					}
					break;
							 }
				case VSTRING:;{
					std::vector<std::string> *vs = (std::vector<std::string>*)v_ptr[i];
					*vs = map_long_carry[key];
					break;
							 }
				default: error_msg("未能识别的类型\n"); break;
				};

			}else{
				bool *b =(bool*) v_ptr[i];
				*b = true;
			}
		
		}
	}catch(exception &e){
			std::string err =  "错误：" + string(e.what());
			error_msg(err.c_str()); return;
	}
	va_end(vargs);
}

void Getopts::error_msg(const char* s){
	if(!show_error) return;
	printf("error: %s", s);
}

void Getopts::set_error_mode(bool b){
	show_error = b;
}

bool Getopts::error_handle(){
	is_error = true;
	return false;
}

void Getopts::set_error_status(bool b){
	is_error = true;
}