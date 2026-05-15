#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <sqlite3.h>
}
#include <FuzzedDataProvider.h>


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* data, size_t size);


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
		case 2:
			return LLVMFuzzerTestOneInput_2(input, i_size);
			break;
		case 3:
			return LLVMFuzzerTestOneInput_3(input, i_size);
			break;
		case 4:
			return LLVMFuzzerTestOneInput_4(input, i_size);
			break;
		case 5:
			return LLVMFuzzerTestOneInput_5(input, i_size);
			break;
		case 6:
			return LLVMFuzzerTestOneInput_8(input, i_size);
			break;
		case 7:
			return LLVMFuzzerTestOneInput_11(input, i_size);
			break;
		case 8:
			return LLVMFuzzerTestOneInput_12(input, i_size);
			break;
		case 9:
			return LLVMFuzzerTestOneInput_17(input, i_size);
			break;
		case 10:
			return LLVMFuzzerTestOneInput_18(input, i_size);
			break;
		case 11:
			return LLVMFuzzerTestOneInput_19(input, i_size);
			break;
		case 12:
			return LLVMFuzzerTestOneInput_26(input, i_size);
			break;
		case 13:
			return LLVMFuzzerTestOneInput_29(input, i_size);
			break;
		case 14:
			return LLVMFuzzerTestOneInput_31(input, i_size);
			break;
		case 15:
			return LLVMFuzzerTestOneInput_36(input, i_size);
			break;
		case 16:
			return LLVMFuzzerTestOneInput_41(input, i_size);
			break;
		case 17:
			return LLVMFuzzerTestOneInput_42(input, i_size);
			break;
		case 18:
			return LLVMFuzzerTestOneInput_44(input, i_size);
			break;
		case 19:
			return LLVMFuzzerTestOneInput_47(input, i_size);
			break;
		default:
			break;
	}
return 0;
}
