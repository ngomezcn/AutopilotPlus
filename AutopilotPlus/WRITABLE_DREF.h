#ifndef WRITABLE_SIMVAR_H
#define WRITABLE_SIMVAR_H

#include "DREF.h"

struct WRITABLE_DREF : DREF {
private:
	float data_to_set = 0;
public:
	WRITABLE_DREF(const char* path, int dref_freq_ = DEFAULT_DREF_FREQ) : DREF(path, dref_freq_) {}

	void set(float value_) {
		// TODO
	}
};

#endif /* WRITABLE_SIMVAR_H */