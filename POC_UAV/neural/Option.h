#pragma once
#include <string>
#include "lib/INIReader.h"

struct Option
{

#define OPTION_PARAMETERS \
OPTION_STRING(DataFile, "p.txt");\
OPTION_STRING(LoadFile, "save.txt");\
OPTION_STRING(SaveFile, "save.txt");\
OPTION_STRING(TestFile, "testFile.txt");\
\
OPTION_INT(UseMINST, 0);\
OPTION_INT(LoadNet, 0);\
\
OPTION_INT(Layer, 3);\
OPTION_INT(NodePerLayer, 7);\
\
OPTION_INT(LearnMode, 0);\
OPTION_INT(WorkMode, 0);\
OPTION_INT(MiniBatch, -1);\
\
OPTION_DOUBLE(TrainTimes, 100);\
OPTION_DOUBLE(OutputInterval, 1);\
\
OPTION_DOUBLE(LearnSpeed, 0.5);\
OPTION_DOUBLE(Regular, 0.01);\
OPTION_DOUBLE(Tol, 1e-3);\
OPTION_DOUBLE(Dtol, 0);


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

