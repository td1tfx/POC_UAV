#pragma once
#include "Network.h"

void doPOC();
void doRecommendation();
void doCloudletRecommendation();
void doUserCheck();
void getTrainData(Network* network, double geRate);
void getTestData(Network* network);
void runTrainedPath(Network* network, double geRate);
void Routing();


int main(int argc, char *argv[]) {

	//doPOC();
	doRecommendation();
	//doCloudletRecommendation();
	//Routing();
	//doUserCheck();
	system("pause");
	return argc;
}

void doRecommendation() {
	srand(time(NULL));
	auto net = new Network;
	net->initCloudletGraph();
	net->getAllTypeShortestPath();
	int maxRound = Config::getInstance()->getRound();
	net->runCloudRounds(10);
	int cuRound = 0;
	while (net->getCuTime() < 50) {
		net->runCloudRounds(100);
		//topology->saveData(false, filename, dest);
		cuRound++;
	}
	net->saveDelay(true,Config::getInstance()->getMaxGenerateRate());
}

void doCloudletRecommendation(){
	srand(time(NULL));
	auto net = new Network;
	net->initCloudletGraph();
	net->getAllTypeShortestPath();
	int maxRound = Config::getInstance()->getRound();
	net->runCloudletRounds(10);
	int cuRound = 0;
	while (net->getCuTime() < 50) {
		net->runCloudletRounds(100);
		//topology->saveData(false, filename, dest);
		cuRound++;
	}
	net->saveDelay(true, Config::getInstance()->getMaxGenerateRate());
}

void doUserCheck(){
	srand(time(NULL));
	auto net = new Network;
	net->initCloudletGraph();
	net->getAllTypeShortestPath();
	net->initVenues();
	int maxRound = Config::getInstance()->getRound();
	net->runUserMovingRounds(100);
	net->getValidNumber();
}


void doPOC(){

	srand(time(NULL));
	int tolNum = 1;

	//POC game
	float totalUti = 0;
	float aveUtility = 0;
	float maxUtility = 0;
	auto net = new Network;
	net->initGraph();
	net->getAllShortestPath();
	net->updatePribyLoad();
	net->runPOCGame(1, false);
	// 	for (int i = 0; i < tolNum; i++) {
	// 		net->runRounds(1);
	// 		net->getAllShortestPath();
	// 		net->updatePribyLoad();
	// 		float netUtility = net->runPOCGame(i,false);
	// 		totalUti += netUtility;
	// 		if (netUtility > maxUtility) {
	// 			maxUtility = netUtility;
	// 		}
	// 	}
	// 	aveUtility = totalUti / tolNum;
	// 	cout << "aveUtility = " << aveUtility << endl;
	// 	cout << "maxUtility = " << maxUtility << endl;


	//NOrmal 
	// 	auto net1 = new Network;
	// 	net1->initGraph();
	// 	net1->runNormal();

	//POC 
	// 	for (int i = 0; i < tolNum; i++) {
	// 		auto net2 = new Network;
	// 		net2->initGraph();
	// 		net2->runPOC(0);
	// 	}

	//train DP
	// 	auto net3 = new Network;
	// 	net3->initGraph();
	// 	net3->initTrainNet();
	// 	net3->trainNet();


	//POC DP and compare with POC game

	// 	tolNum = 100;
	// 	float totalUti = 0;
	// 	float aveUtility = 0;
	// 	float maxUtility = 0;
	// 	int totalWrong = 0;
	// 	int totalCount = 0;
	// 	auto net4 = new Network;
	// 	net4->initGraph();
	// 	net4->initTrainNet();
	// 	net4->getAllShortestPath();
	// 	net4->updatePribyLoad();
	// 	net4->getAllCHbyDP();
	// 	for (int i = 0; i < tolNum; i++) {
	// 		net4->runRounds(1);
	// 		net4->getAllShortestPath();
	// 		net4->updatePribyLoad();
	// 		net4->getAllCHbyDP();
	// 		totalCount += net4->totalPCount;
	// 		totalWrong += net4->wrongPCount;
	// 		net4->printWrongCount();
	// 	}
	// 	float wrongRatio = (float)totalWrong / (float)totalCount / 1.000;
	// 	std::cout << "totalCount = " << totalCount << ";TotalWrongCount = " << totalWrong << "runTimes" << tolNum << endl;
	// 	std::cout << "wrongRatio = " << wrongRatio << endl;
	// 
}

void Routing()
{

	Network* net = new Network;
	net->initGraph();
	net->updatePribyLoad();
	srand(time(NULL));
	double rate = Config::getInstance()->getMaxGenerateRate();
	//Config::getInstance()->resetMaxGenerateRate(rate);
	//net->initTrainNet(1);
	//choose one 
	runTrainedPath(net, rate);
	//getTrainData(net, rate);         
	//getTestData(topology);
	//net->trainNet(1);
	//topology->testNet();
	delete net;
	system("pause");
}

void getTestData(Network* network) {

	int dest = -1; //all data
	network->isTrained = false;
	network->getAllShortestPath();
	network->runRounds(10);
	network->saveRouting(true,1);
	int cuRound = 0;
	int maxRound = Config::getInstance()->getTestRound();
	while (cuRound < maxRound) {
		network->runRounds(10);
		network->getAllShortestPath();
		network->saveRouting(false, 1);
		cuRound++;
	}
	network->saveDelay();

	cout << "test finalTime=" << network->getCuTime() << endl;
}

void getTrainData(Network* network, double geRate) {
	int dest = -1; //all data
	network->isTrained = false;
	network->getAllShortestPath();
	network->runRounds(10);
	network->saveRouting(true);
	int cuRound = 0;
	int maxRound = Config::getInstance()->getRound();
	while (cuRound < maxRound) {
		network->runRounds(10);
		network->getAllShortestPath();
		network->saveRouting(false);
		cuRound++;
	}
	network->saveDelay(false, geRate, 0);

	cout << "train finalTime=" << network->getCuTime() << endl;
}

void runTrainedPath(Network* network, double geRate) {
	//char* filename = "../data/node";
	int dest = -1; //all data
	network->isTrained = true;
	network->getAllShortestPath();
	network->getAllTrainedPath();
	network->saveRoutingWrongCount(true);
	network->runRoundsWithTrain(10);
	//topology->saveData(true, filename, dest);
	int cuRound = 0;
	int maxRound = Config::getInstance()->getRound();
	while (network->getCuTime() < maxRound) {
		network->runRoundsWithTrain(10);
		//topology->getAllShortestPath();
		network->getAllTrainedPath();
		network->saveRoutingWrongCount(false);
		//topology->saveData(false, filename, dest);
		cuRound++;
	}
	network->saveDelay(true, geRate);

	cout << "train finalTime=" << network->getCuTime() << endl;
}


