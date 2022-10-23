#ifndef DREFS_DEFINITION_H
#define DREFS_DEFINITION_H

#include "WRITABLE_DREF.h"
#include "DREF_INPUT.h"



namespace xplane
{
	enum HEADER
	{
		RREF = 0x52524546, // RREF
	};

	enum DREF_PERIOD
	{
		PERIOD_NONE = 0,
		PERIOD_SLOW = 5,
		PERIOD_MEDIUM = 20,
		PERIOD_HIGH = 100,
	};

	/***********************************************
	 *   https://developer.x-plane.com/datarefs/   *
	 ***********************************************/
	namespace sim {
		namespace flightmodel {
			namespace position {
				inline WRITABLE_DREF local_x("sim/flightmodel/position/local_x", DREF_PERIOD::PERIOD_HIGH);
				inline WRITABLE_DREF local_y("sim/flightmodel/position/local_y", DREF_PERIOD::PERIOD_HIGH);
				inline WRITABLE_DREF local_z("sim/flightmodel/position/local_z", DREF_PERIOD::PERIOD_HIGH);
				inline DREF lat_ref("sim/flightmodel/position/lat_ref", DREF_PERIOD::PERIOD_HIGH);
				inline DREF lon_ref("sim/flightmodel/position/lon_ref", DREF_PERIOD::PERIOD_HIGH);
				inline DREF elevation("sim/flightmodel/position/elevation", DREF_PERIOD::PERIOD_HIGH);
				inline DREF latitude("sim/flightmodel/position/latitude", DREF_PERIOD::PERIOD_HIGH);
				inline DREF longitude("sim/flightmodel/position/longitude", DREF_PERIOD::PERIOD_HIGH);
			}
		}
	}
}

#endif /* DREFS_DEFINITION_H */