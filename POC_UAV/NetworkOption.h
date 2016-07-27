#pragma once
#include <string>
#include "INIReader.h"

struct NetworkOption
{

#define OPTION_PARAMETERS \
OPTION_STRING(DataFile, "p.txt");\
\
OPTION_INT(PackageSize, 256);\
OPTION_INT(SignalingSize, 32);\
OPTION_INT(MaxColum, 4);\
OPTION_INT(MaxRow, 4);\
OPTION_INT(Bandwidth, 1024);\
OPTION_INT(Round, 1000);\
OPTION_INT(TestRound, 100);\
OPTION_INT(IsFullMod, 1);\
OPTION_INT(IsSingleDestMod, 1);\
OPTION_INT(IsSingleOutputMod, 1);\
\
OPTION_DOUBLE(MaxGenerateRate, 1);\



#define OPTION_STRING(a, b) std::string a = (b)
#define OPTION_INT(a, b) int a = (b)
#define OPTION_DOUBLE(a, b) double a = (b)	
	OPTION_PARAMETERS
#undef OPTION_STRING
#undef OPTION_INT
#undef OPTION_DOUBLE

		void loadIni(const char* filename)
	{
		INIReader ini(filename);
#define OPTION_STRING(a, b) a = ini.Get("option", #a, (b))
#define OPTION_INT(a, b) a = ini.GetInteger("option", #a, (b))
#define OPTION_DOUBLE(a, b) a = ini.GetReal("option", #a, (b))
		OPTION_PARAMETERS
#undef OPTION_STRING
#undef OPTION_INT
#undef OPTION_DOUBLE
	}
#undef OPTION_PARAMETERS
};


