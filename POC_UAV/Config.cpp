#include "stdafx.h"
#include "Config.h"

Config* Config::m_config;


Config::Config()
{
	_option.loadIni("netConfig.ini");
	packageSize = _option.PackageSize;
	signalingSize = _option.SignalingSize;
	maxColumn = _option.MaxColum;
	maxRow = _option.MaxRow;
	bandwidth = _option.Bandwidth;
	maxGenerateRate = _option.MaxGenerateRate;
	round = _option.Round;
	testRound = _option.TestRound;
	maxPacNumPerNode = round;
	fullMod = _option.IsFullMod;
	singleDestMod = _option.IsSingleDestMod;
	singleOutputMod = _option.IsSingleOutputMod;
	while (maxPacNumPerNode < round*maxGenerateRate) {
		maxPacNumPerNode = maxPacNumPerNode * 10;
	}	
}


Config::~Config()
{
}

/*
void Config::init() 
{
}
*/
