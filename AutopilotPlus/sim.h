#pragma once


#include <iostream>

struct writablesimvar {

	const char* path = "sim/aircraft/autopilot/";
};


namespace sim {
	using namespace std;

	namespace cockpit {
		const string header = "cockpit";

		class autopilot {

		public:
			string path = header;

		} autopilot;
	}

	namespace flightmodel {
		const string header = "flightmodel";

		class controls {
		public:
			string path = header;

		} controls;
	}
}

void mmain() {

	std::cout << sim::cockpit::autopilot.path << std::endl;
	std::cout << sim::flightmodel::controls.path << std::endl;
	
}