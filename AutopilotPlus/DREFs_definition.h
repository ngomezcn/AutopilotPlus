#ifndef _DREFS_DEFINITION_H
#define _DREFS_DEFINITION_H

#include "DREF.h"
#include "WRITABLE_DREF.h"

namespace sim {
	namespace flightmodel {
		namespace position {
			WRITABLE_DREF local_x("sim/flightmodel/position/local_x", 0);
			WRITABLE_DREF local_y("sim/flightmodel/position/local_y", 0);
			WRITABLE_DREF local_z("sim/flightmodel/position/local_z", 0);
			DREF lat_ref("sim/flightmodel/position/lat_ref", 0);
			DREF lon_ref("sim/flightmodel/position/lon_ref", 0);
			DREF elevation("sim/flightmodel/position/elevation", 0);
			DREF latitude("sim/flightmodel/position/latitude", 0);
			DREF longitude("sim/flightmodel/position/longitude", 1);
		}
	}
}

#endif /* _DREFS_DEFINITION_H */