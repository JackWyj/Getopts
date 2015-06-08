#include "stdafx.h"
#include "ShortFunction.h"


ShortFunction::ShortFunction(void)
{
	ch = 0;
	have_param = false;
}

ShortFunction::ShortFunction(char c , bool b){
	ch = c;
	have_param = b;
}

ShortFunction::~ShortFunction(void)
{
}

bool ShortFunction::haveParam(){
	return have_param;
}

char ShortFunction::getch(){
	return ch;
}

ShortFunction::ShortFunction(const ShortFunction &sfn){
	ch = sfn.ch;
	have_param = sfn.have_param;
}

void ShortFunction::operator=(const ShortFunction &sfn){
	ch = sfn.ch;
	have_param = sfn.have_param;
}

param_type ShortFunction::carry_param_type(){
	return paramty;
}