#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <turbojpeg.h>
}
#include <FuzzedDataProvider.h>


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* data, size_t size);


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	FuzzedDataProvider fdp(data, size);
	FDPConsumeIntegral(uint16_t, switch_id, fdp);
	const uint8_t *input = data + sizeof(uint16_t);
	size_t i_size = size - sizeof(uint16_t);
	switch (switch_id) {
		case 0:
			return LLVMFuzzerTestOneInput_37(input, i_size);
			break;
		case 1:
			return LLVMFuzzerTestOneInput_38(input, i_size);
			break;
		case 2:
			return LLVMFuzzerTestOneInput_39(input, i_size);
			break;
		case 3:
			return LLVMFuzzerTestOneInput_40(input, i_size);
			break;
		case 4:
			return LLVMFuzzerTestOneInput_41(input, i_size);
			break;
		case 5:
			return LLVMFuzzerTestOneInput_42(input, i_size);
			break;
		case 6:
			return LLVMFuzzerTestOneInput_43(input, i_size);
			break;
		default:
			break;
	}
return 0;
}
