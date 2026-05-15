#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <cre2.h>
}
#include <FuzzedDataProvider.h>


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* data, size_t size);


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	FuzzedDataProvider fdp(data, size);
	FDPConsumeIntegral(uint16_t, switch_id, fdp);
	const uint8_t *input = data + sizeof(uint16_t);
	size_t i_size = size - sizeof(uint16_t);
	switch (switch_id) {
		case 0:
			return LLVMFuzzerTestOneInput_0(input, i_size);
			break;
		case 1:
			return LLVMFuzzerTestOneInput_1(input, i_size);
			break;
		default:
			break;
	}
return 0;
}
