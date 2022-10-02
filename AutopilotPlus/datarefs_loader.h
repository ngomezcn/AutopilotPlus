#ifndef DATAREFS_DEFINITION
#define DATAREFS_DEFINITION

#include <map>
#include <vector>

#include "DREF.h"
#include "WRITABLE_DREF.h"
#include "DREFs_definition.h"

using namespace sim::flightmodel::position;
std::vector<DREF*> DREFs_TO_REQUEST {
		&local_x,
		&local_y,
		&local_z,
		&lat_ref,
		&lon_ref,
		&elevation,
		&latitude,
		&longitude
};

#endif /* DATAREFS_DEFINITION */