# Getopts
1. 该工具有短参数和长参数两种使用方式，长参数这块相对来说比较完整，短参数这一块有待完善。
2. 长参数的使用方法为：
        Getopts getopt;
      	std::string in;
      	in = "dl=d;fl=f;in=i;noc;";
      	double d = 100.01;
      	float f = 555.5;
      	int i = 666;
      	bool b = false;
      	getopt.set_long_param(in, true, &d, &f, &i, &b);
      	in = "-i 37 -no dddd eee -dl -fl \"2.1\" -d 77.7 -f 3.5";
      	getopt.parse_param(in);
  A) 先定义一个Getopts的类实例，然后用该实例的方法set_long_param定义解析规则，长参数可以用只能用字母和数字，使用方法为：
        string=mode;（表示有携带参数）
        string;(表示没有携带参数，做开关参数)
  string是你定义的参数，只能用字母或者数字，mode可选的有i（int）,d（double）,f（float）,s（string），vi(vector<int>),             vd(vector<double>), vf(vector<float>), vs(vector<string>)。每次定义一个规则之后，就必须用";"结束该规则。
  在定义了之后，可以用一些变量来存取解析的结果，
  getopt.set_long_param(in, true, &d, &f, &i, &b);
  在设置了规则字符串后，set_long_param的第二个参数要设置为true，表示要用变量存取参数，接着，就按照定义规则的顺序，传递相应变量的   地址。对于一些开关参数，只能传递bool类型的地址。
  B) 调用解析函数getopt.parse_param(in) 去解析字符串，并把结果存在相应的变量
