#pragma once
#include "Network.h"

void doPOC();
void doRecommendation();

int main(int argc, char *argv[]) {

	//doPOC();
	doRecommendation();
	
	system("pause");
	return argc;
}

void doRecommendation() {
	srand(time(NULL));
	auto net = new Network;
	net->initCloudletGraph();
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

