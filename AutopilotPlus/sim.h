#pragma once

#include <iostream>
#include "DREF_REQ.h"

#define GLOB_DREF_FREQ 100

#define SIMVAR_ID_BASE 0
#define SIMVAR_ID_INCREMENT 1

struct simvar
{
private:
	static int serial_id;
public: 
	int id;
	DREF_REQ rref;
	float value_ = .0f;

	simvar(const char* path) {
		id = serial_id;
		serial_id += SIMVAR_ID_INCREMENT;

		rref = DREF_REQ(GLOB_DREF_FREQ, id, path);
	}

	float get() {
		// TODO
		return value_;
	}

};
int simvar::serial_id = SIMVAR_ID_BASE;

struct writable_simvar : simvar {

public:
	writable_simvar(const char* path) : simvar(path) {}

	void set(float value_) {
		// TODO
	}
};

namespace sim {
	namespace cockpit {
		namespace autopilot {
				writable_simvar autopilot_mode("sim/cockpit/autopilot/autopilot_mode");
		};
	}

	namespace flightmodel {
		namespace position {
			simvar latitude("sim/flightmodel/position/latitude");
			simvar longitude("sim/flightmodel/position/longitude");
		}
	}
}

std::vector<simvar> datarefs_to_load {
	//sim::flightmodel::position::latitude,
	sim::flightmodel::position::longitude
};

