#pragma once

#include <iostream>

struct writablesimvar {

	const char* path = "sim/aircraft/autopilot/";
};

namespace sim {
	const std::string dref_root;

	namespace cockpit {
		const std::string header = "cockpit";

		class autopilot {

		public:
			std::string path = dref_root + header;

		} autopilot;
	}

	namespace flightmodel {
		const  std::string header = "flightmodel";

		class controls {
		public:
			std::string path = dref_root + header;

		} controls;
	}
}

