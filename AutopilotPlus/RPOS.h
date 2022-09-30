#pragma once
struct RPOS {
	double dat_lon;		// longitude of the aircraft in X-Plane of course, in degrees
	double dat_lat;		// latitude
	double dat_ele;		// elevation above sea level in meters
	float y_agl_mtr;	// elevation above the terrain in meters
	float veh_the_loc;	// pitch, degrees
	float veh_psi_loc;	// true heading, in degrees
	float veh_phi_loc;	// roll, in degrees
	float vx_wrl;		// speed in the x, EAST, direction, in meters per second
	float vy_wrl;		// speed in the y, UP, direction, in meters per second
	float vz_wrl;		// speed in the z, SOUTH, direction, in meters per second
	float Prad;			// roll rate in radians per second
	float Qrad;			// pitch rate in radians per second
	float Rrad;			// yaw rate in radians per second

	RPOS(
		double dat_lon,
		double dat_lat,
		double dat_ele,
		float y_agl_mtr,
		float veh_the_loc,
		float veh_psi_loc,
		float veh_phi_loc,
		float vx_wrl,
		float vy_wrl,
		float vz_wrl,
		float Prad,
		float Qrad,
		float Rrad
	) : dat_lon(dat_lon),
		dat_lat(dat_lat),
		dat_ele(dat_ele),
		y_agl_mtr(y_agl_mtr),
		veh_the_loc(veh_the_loc),
		veh_psi_loc(veh_psi_loc),
		veh_phi_loc(veh_phi_loc),
		vx_wrl(vx_wrl),
		vy_wrl(vy_wrl),
		vz_wrl(vz_wrl),
		Prad(Prad),
		Qrad(Qrad),
		Rrad(Rrad) {}
};

static RPOS rpos(0, 0, 0, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f);