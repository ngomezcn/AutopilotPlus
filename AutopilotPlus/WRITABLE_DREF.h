#ifndef WRITABLE_SIMVAR_H
#define WRITABLE_SIMVAR_H

#include "DREF.h"
#include "DREF_OUTPUT.h"

#include "UDPService.h"

struct WRITABLE_DREF : DREF {
private:
	float data_to_set = 0;
	DREF_OUTPUT dref_out;
	int path_size_ = 0;

public:
	WRITABLE_DREF(const char* path, int dref_freq_ = DEFAULT_DREF_FREQ, float initial_value = 0) : DREF(path, dref_freq_) {

		for (int i = 0; path_[i] != '\0'; i++)
			path_size_++;

		dref_out = DREF_OUTPUT(data_to_set, path);
	}

	void set(float value_) {
		data_to_set = value_;
		dref_out.var_ = data_to_set;

		unsigned char* data = reinterpret_cast<unsigned char*>(&dref_out);
		unsigned char header[5] = "DREF";
		header[4] = '0';
		unsigned char msg[sizeof(header) + sizeof(DREF_OUTPUT)];

		memcpy(msg, header, sizeof(header));
		memcpy(msg + sizeof(header), data, sizeof(DREF_OUTPUT));

		udp_service.async_send(msg, sizeof(msg));
	}
};

#endif /* WRITABLE_SIMVAR_H */