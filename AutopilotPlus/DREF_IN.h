#ifndef DREF_IN_H
#define DREF_IN_H

#include <cstdint>
#include "xplane_types.h"

extern struct DREF_IN
{
	xint dref_freq_; // number of times per second you want x-plane to send this data
	xint dref_sender_index_;	// the index the customer is using to define this dataref
	xchr dref_string_[400]; // dataref string that you want X-Plane to send to you!

	DREF_IN(
		xint dref_freq,
		xint dref_sender_index,
		const xchr dref_string[400]) : dref_freq_(dref_freq), dref_sender_index_(dref_sender_index)
	{
		strncpy_s(dref_string_, dref_string, strlen(dref_string));

		// TODO: Better way to fill dref_string_ empty space with 0x00
		for (size_t i = 0; i < 400; i++)	
			if ((int)dref_string_[i] == -2) 
				dref_string_[i] = 0x00;
	}
};
#endif /* DREF_IN_H */