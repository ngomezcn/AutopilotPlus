#ifndef DREF_OUTPUT_H
#define DREF_OUTPUT_H

const xint strDIM = 500;

struct DREF_OUTPUT
{
	xflt var_ = -1;
	xchr dref_path_[strDIM];

	DREF_OUTPUT(){}
	DREF_OUTPUT(
		const xchr dref_path[strDIM]
	) 
	{
		strncpy_s(dref_path_, dref_path, strlen(dref_path));

		// TODO: Find better way to fill dref_string_ empty space with 0x00
		for (int i = 0; i < strDIM; i++)
			if ((int)dref_path_[i] == -2)
				dref_path_[i] = 0x00;
	}
};

#endif // DREF_OUTPUT_H
