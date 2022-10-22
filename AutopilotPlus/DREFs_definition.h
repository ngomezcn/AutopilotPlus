#ifndef DREFS_DEFINITION_H
#define DREFS_DEFINITION_H

#include "WRITABLE_DREF.h"
#include "DREF_INPUT.h"

namespace xplane
{
	enum DREF_PERIOD
	{
		PERIOD_NEVER = 0,
		PERIOD_SLOW = 5,
		PERIOD_MEDIUM = 20,
		PERIOD_HIGH = 100,
	};

	/********************************************//**
	 *  ... text
	 ***********************************************/
	namespace sim {
		namespace flightmodel {
			namespace position {
				inline WRITABLE_DREF local_x("sim/flightmodel/position/local_x", DREF_PERIOD::PERIOD_NEVER);
				inline WRITABLE_DREF local_y("sim/flightmodel/position/local_y", DREF_PERIOD::PERIOD_NEVER);
				inline WRITABLE_DREF local_z("sim/flightmodel/position/local_z", DREF_PERIOD::PERIOD_NEVER);
				inline DREF lat_ref("sim/flightmodel/position/lat_ref", DREF_PERIOD::PERIOD_NEVER);
				inline DREF lon_ref("sim/flightmodel/position/lon_ref", DREF_PERIOD::PERIOD_NEVER);
				inline DREF elevation("sim/flightmodel/position/elevation", DREF_PERIOD::PERIOD_MEDIUM);
				inline DREF latitude("sim/flightmodel/position/latitude", DREF_PERIOD::PERIOD_MEDIUM);
				inline DREF longitude("sim/flightmodel/position/longitude", DREF_PERIOD::PERIOD_MEDIUM);
			}
		}
	}
}


#endif /* DREFS_DEFINITION_H */