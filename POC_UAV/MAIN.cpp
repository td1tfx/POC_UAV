#pragma once
#include "Network.h"

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int tolNum = 1000;

//POC game
// 	float totalUti = 0;
// 	float aveUtility = 0;
// 	float maxUtility = 0;
// 	for (int i = 0; i < tolNum; i++) {
// 		auto net = new Network;
// 		net->initGraph();
// 		float netUtility = net->runPOCGame(i);
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
	auto net3 = new Network;
	net3->initGraph();
	net3->initTrainNet();
	net3->trainNet();


//POC DP and compare with POC game
// 	tolNum = 1;
// 	float totalUti = 0;
// 	float aveUtility = 0;
// 	float maxUtility = 0;
// 	for (int i = 0; i < tolNum; i++) {
// 		auto net4 = new Network;
// 		net4->initGraph();
// 		net4->initTrainNet();
// 		net4->getAllCHbyDP();
// 		float netUtility = net4->runPOCGame(i,false);
// 		net4->printWrongCount();
// 	}


	system("pause");
	return argc;
}

