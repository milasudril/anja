#ifdef __WAND__
target[name[floatconv.h] type[include]]
dependency[floatconv.o]
#endif

#ifndef FLOATCONV_H
#define FLOATCONV_H

bool convert(const char* string,double& result);

inline double convert(const char* string)
	{
	double ret;
	if(!convert(string,ret))
		{throw "Invalid number format";}
	return ret;
	}

#endif
