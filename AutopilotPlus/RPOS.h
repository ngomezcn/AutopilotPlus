#ifndef RPOS_H
#define RPOS_H

extern struct RPOS {
	double dat_lon	  = .0;		// longitude, in degrees
	double dat_lat    = .0;		// latitude
	double dat_ele    = .0;		// elevation above sea level in meters
	float y_agl_mtr   = .0f;	// elevation above the terrain in meters
	float veh_the_loc = .0f;	// pitch, degrees
	float veh_psi_loc = .0f;	// true heading, in degrees
	float veh_phi_loc = .0f;	// roll, in degrees
	float vx_wrl      = .0f;	// speed in the x, EAST, direction, in meters per second
	float vy_wrl      = .0f;	// speed in the y, UP, direction, in meters per second
	float vz_wrl      = .0f;	// speed in the z, SOUTH, direction, in meters per second
	float Prad        = .0f;	// roll rate in radians per second
	float Qrad        = .0f;	// pitch rate in radians per second
	float Rrad        = .0f;	// yaw rate in radians per second
};

RPOS rpos;

#endif /* RPOS_H */