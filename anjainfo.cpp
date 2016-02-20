#ifdef __WAND__
target[name[anjainfo.o] type[object]]
#endif

#include "anjainfo.h"
#include "framework/includebinary.h"
#include "framework/programinfo.h"

INCLUDE_BINARY(ANJA_LOGO,"__anjalogo.dat");
INCLUDE_BINARY(COMPILER,"__cxxtag.dat");
INCLUDE_BINARY(ANJA_VERSION,"__vertag.dat");

static const char* const AUTHORS[]={"Torbjörn Rathsman",nullptr};
static const char* const ACKNOWLEDGEMENT[]=
	{
	 "Mega-Nerd - for libsndfile"
	,"Paul Davis et.al. - for JACK"
	,"Stefan Buller - for valuable feedback on the user interface"
	,nullptr
	};

static const char* const COMPILEINFO[]=
	{__DATE__," ",__TIME__," by","\n",(const char*)COMPILER_begin,nullptr};

ProgramInfo ANJAINFO
	{
	 "Anja"
	,(const char*)ANJA_VERSION_begin
	,"A sample recorder and player for use with JACK"
	,AUTHORS
	,2016
	,ACKNOWLEDGEMENT
	,"This program comes with ABSOLUTELY NO WARRANTY. This is free software,\n"
	"and you are welcome to redistribute it under certain conditions. For details,\n"
	"see the file GPL.md, which should have been distributed together with the\n"
	"software."
	,(const RGBABlock*)( ANJA_LOGO_begin )
	,COMPILEINFO
	};
