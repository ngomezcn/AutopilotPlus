#pragma once

#include <map>
#include <iostream>
#include "DREF_REQ.h"

#define DEFAULT_DREF_FREQ 100

struct simvar
{
private:
	static int serial_id;
public: 
	int id;
	DREF_REQ rref;
	float value_ = .0f;

	simvar(const char* path, int dref_freq_=DEFAULT_DREF_FREQ) : id(serial_id++) {
		id = serial_id;

		rref = DREF_REQ(dref_freq_, id, path);
	}


	float get() {
		return value_;
	}

};
int simvar::serial_id = 0; // 

struct writable_simvar : simvar {

public:
	writable_simvar(const char* path) : simvar(path) {}

	void set(float value_) {
		// TODO
	}
};

namespace sim {
	namespace flightmodel {
		namespace position {
			simvar latitude("sim/flightmodel/position/latitude", 0);
			simvar longitude("sim/flightmodel/position/longitude", 0);
			simvar elevation("sim/flightmodel/position/elevation", 100);
		}
	}
}

std::vector<simvar*> DATAREFS_TO_LOAD {
	&sim::flightmodel::position::longitude,
	&sim::flightmodel::position::latitude,
	&sim::flightmodel::position::elevation
};

std::map<int, simvar*> DATAREFS_MAP;
