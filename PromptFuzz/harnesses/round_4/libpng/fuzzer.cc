#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <png.h>
}
#include <FuzzedDataProvider.h>


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* data, size_t size);


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	FuzzedDataProvider fdp(data, size);
	FDPConsumeIntegral(uint16_t, switch_id, fdp);
	const uint8_t *input = data + sizeof(uint16_t);
	size_t i_size = size - sizeof(uint16_t);
	switch (switch_id) {
		case 0:
			return LLVMFuzzerTestOneInput_11(input, i_size);
			break;
		case 1:
			return LLVMFuzzerTestOneInput_12(input, i_size);
			break;
		case 2:
			return LLVMFuzzerTestOneInput_13(input, i_size);
			break;
		case 3:
			return LLVMFuzzerTestOneInput_14(input, i_size);
			break;
		case 4:
			return LLVMFuzzerTestOneInput_15(input, i_size);
			break;
		case 5:
			return LLVMFuzzerTestOneInput_16(input, i_size);
			break;
		case 6:
			return LLVMFuzzerTestOneInput_17(input, i_size);
			break;
		case 7:
			return LLVMFuzzerTestOneInput_18(input, i_size);
			break;
		case 8:
			return LLVMFuzzerTestOneInput_19(input, i_size);
			break;
		case 9:
			return LLVMFuzzerTestOneInput_20(input, i_size);
			break;
		default:
			break;
	}
return 0;
}
