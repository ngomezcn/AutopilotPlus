#ifndef DATAREFS_DEFINITION
#define DATAREFS_DEFINITION

#include <map>
#include <vector>

#include "DREF.h"
#include "WRITABLE_DREF.h"
#include "DREFs_definition.h"

using namespace sim::flightmodel::position;

WRITABLE_DREF llocal_x("sim/flightmodel/position/local_x", 10);
int c = 0;
DREF* a = &llocal_x;
int cc = 0;
std::vector<DREF*> DREFs_TO_REQUEST {
		&local_y,
		&local_z,
		&lat_ref,
		&lon_ref,
		&elevation,
		&latitude,
		&longitude
};

int b = 0;

#endif /* DATAREFS_DEFINITION */