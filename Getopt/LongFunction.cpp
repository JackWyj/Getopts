#include "stdafx.h"
#include "LongFunction.h"


LongFunction::LongFunction(void)
{
	have_param = false; paramty = NO;
}


LongFunction::~LongFunction(void)
{
}

bool LongFunction::haveParam(){
	return have_param;
}

param_type LongFunction::carry_param_type(){
	return paramty;
}