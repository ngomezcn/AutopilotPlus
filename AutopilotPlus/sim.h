#pragma once

#include <iostream>

struct writable_simvar {

	const char* path_ = "";
	float dref_value_ = .0f;

public:
	
	float get() {
		return dref_value_;
	}

	void set(float dref_value) {
		dref_value_ = dref_value;
	}

	writable_simvar(const char* path) {
		path_ = path;
	}
};

namespace sim {
	namespace cockpit {
		namespace autopilot {
				writable_simvar autopilot_mode("sim/cockpit/autopilot/autopilot_mode");
		};
	}
}

