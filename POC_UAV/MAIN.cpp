#pragma once
#include "Network.h"

int main(int argc, char *argv[]) {
	srand(time(NULL));
	auto net = new Network;
	net->initGraph();
	net->runPOCGame();
	auto net1 = new Network;
	net1->initGraphByFile();
	net1->runNormal();
	return argc;
}