#pragma once
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <stdarg.h>
#include "ShortFunction.h"
#include "LongFunction.h"

using namespace std;
enum ch_type{
	CHAR, EX, NONE, SEQ, EQ
};

struct in_ch{
	char c;
	bool end;
	in_ch(){}
	in_ch(char ch, bool b): c(ch), end(b){}
	in_ch(bool b):end(b){}
};

struct key_chosen{
	bool error;
	std::string key;
	key_chosen(){}
	key_chosen(bool b): error(b) {}
	key_chosen(const std::string s): key(s), error(false){}
};
class Getopts
{

	std::set<char> set_ex_char_short, set_ex_char_long;
	std::string inbuf, short_param;
	std::set<char> set_short_used;
	std::map<char, ShortFunction> map_short_func;
	std::map<std::string, LongFunction> map_long_func;
	std::map<char, std::vector<std::string> > map_ch_carry;
	std::map<std::string, std::vector<std::string> > map_long_carry;
	std::set<char> set_short_param_on;
	std::set<std::string> set_long_param_on;
	std::vector<void*> v_ptr;
	/*char tmpch;*/
	ch_type pre_chtype;
	int parse_level; //0-初始化，1-短参数，2-长参数
	va_list vargs;
	std::vector<std::string> vec_keys;
	bool vstore_long, vstore_short;
	bool show_error;
	bool is_error;
public:
	std::map<char, std::string> map_short_opts;
	in_ch getch();
	void help();
	Getopts(void);
	~Getopts(void);
	//输入设置
	std::string getbuf();
	void setbuf(const std::string& buf);
	bool parse_short();
	bool parse_long();
	//解析使用参数
	bool set_short_param(std::string  str, bool store, ...);
	bool set_long_param( std::string  str, bool store,  ...);
	ch_type is_char_legal(char c);
	ch_type is_char_legal_long(char c);
	void parse_param(const std::string & in);
	param_type long_carry_type(const std::string & c);
	key_chosen pick_key(const std::string sk);
	void store_carry_param_long();
	void error_msg(const char* s);
	void set_error_mode(bool b);
	void set_error_status(bool b);
	bool error_handle();
};

