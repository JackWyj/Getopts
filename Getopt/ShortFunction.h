#pragma once
#include "paramfunction.h"

class ShortFunction :
	public ParamFunction
{
	char ch;
	bool have_param;
	param_type paramty;
public:
	ShortFunction(void);
	ShortFunction(char c, bool b);
	~ShortFunction(void);
	char getch();
	ShortFunction(const ShortFunction &sfn);
	void operator=(const ShortFunction &sfn);
	bool haveParam();
	param_type carry_param_type();
};

