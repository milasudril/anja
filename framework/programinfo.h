#ifdef __WAND__
target[name[programinfo.h] type[include]]
#endif


#ifndef PROGRAMINFO_H
#define PROGRAMINFO_H

class RGBABlock;

struct ProgramInfo
	{
	const char* name;
	const char* version;
	const char* description;
	const char* const* authors;
	unsigned int year;
	const char* const* acknowledgement;
	const char* disclaimer;
	const RGBABlock* logo;
	const char* const* compileinfo;
	};

#endif
