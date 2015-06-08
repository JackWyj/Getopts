#pragma once
#include "paramfunction.h"
#include <string>
using namespace std;

class LongFunction :
	public ParamFunction
{
	std::string sopt;
	bool have_param;
	param_type paramty;
public:
	LongFunction(void);
	LongFunction(const std::string & s, bool b): sopt(s), have_param(b), paramty(NO) {}
	LongFunction(const std::string & s, bool b, param_type pt): sopt(s), have_param(b), paramty(pt) {}
	~LongFunction(void);
	bool haveParam();
	param_type carry_param_type();
};

