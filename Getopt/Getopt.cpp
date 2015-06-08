// Getopt.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Getopts.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <vector>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
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
 	return 0;
}

