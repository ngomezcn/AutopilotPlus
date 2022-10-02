#ifndef SIMVAR_H
#define SIMVAR_H

#include "DREF_REQUEST.h"

#define DEFAULT_DREF_FREQ 100

struct DREF
{
private:
	static int serial_id;
public:
	int id;
	DREF_REQUEST rref;
	float value_ = .0f;

	DREF(const char* path, int dref_freq_ = DEFAULT_DREF_FREQ) : id(serial_id++) {
		id = serial_id;
		rref = DREF_REQUEST(dref_freq_, id, path);
	}

	float get() {
		return value_;
	}
};
int DREF::serial_id = 0;

#endif /* SIMVAR_H */