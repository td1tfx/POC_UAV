#pragma once
#include "Network.h"

int main(int argc, char *argv[]) {
	int tolNum = 100;
	float totalUti = 0;
	float aveUtility = 0;
	float maxUtility = 0;
	for (int i = 0; i < tolNum; i++) {
		srand(time(NULL));
		auto net = new Network;
		net->initGraph();
		float netUtility = net->runPOCGame();
		totalUti += netUtility;
		if (netUtility > maxUtility) {
			maxUtility = netUtility;
		}
	}
	aveUtility = totalUti / tolNum;
	cout << "aveUtility = " << aveUtility << endl;
	cout << "maxUtility = " << maxUtility << endl;
// 	auto net1 = new Network;
// 	net1->initGraphByFile();
// 	net1->runNormal();
	system("pause");
	return argc;
}