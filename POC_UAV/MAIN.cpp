#pragma once
#include "Network.h"

int main(int argc, char *argv[]) {
	//srand(time(NULL));
	auto net = new Network;
	net->initGraph();
	net->run();
	ThresholdIF = 100;
	auto net1 = new Network;
	net1->initGraph();
	net1->run();
	return argc;
}