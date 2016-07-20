#pragma once
#include "Network.h"

int main(int argc, char *argv[]) {
	srand(time(NULL));
	auto net = new Network;
	net->initGraph();
	net->run();
	return argc;
}