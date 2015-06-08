#pragma once
enum param_type{
	INT,
	FLOAT,
	DOUBLE,
	STRING, 
	NO,
	VINT,
	VFLOAT,
	VDOUBLE,
	VSTRING
};
class ParamFunction
{
public:
	ParamFunction(void);
	virtual ~ParamFunction(void);
};

