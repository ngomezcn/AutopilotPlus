#ifndef DREF_H
#define DREF_H

#include <cstdint>
#include "xplane_types.h"

extern struct DREF_IN
{
	int dref_freq_; // number of times per second you want x-plane to send this data
	int dref_sender_index_;	// the index the customer is using to define this dataref
	char dref_string_[400] = {'\0'}; // dataref string that you want X-Plane to send to you!

	DREF_IN(
		xint dref_freq,
		xint dref_sender_index,
		char dref_string[400], int size) : dref_freq_(dref_freq), dref_sender_index_(dref_sender_index)
	{
		std::cout << "WUAT" << sizeof(&dref_string) << size << std::endl;
		//memcpy(dref_string_, &dref_string, size);

		//strncpy_s(dref_string_, dref_string, strlen(dref_string));
	}
};


#endif /* DREF_H */