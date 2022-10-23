#ifndef SIMVAR_H
#define SIMVAR_H

#include "DREF_REQUEST.h"

#define DEFAULT_DREF_FREQ 100

struct DREF
{
private:
	static int serial_id;
	int id;

protected:
	const char* path_;

public:
	DREF_REQUEST dref_request;
	float value_ = .0f;

	DREF(const char* path, int dref_freq = DEFAULT_DREF_FREQ) : id(serial_id++) {
		id = serial_id;
		path_ = path;
		dref_request = DREF_REQUEST(dref_freq, id, path);
	}

	float get() {
		return value_;
	}
};

#endif /* SIMVAR_H */