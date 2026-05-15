#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <pcap/bluetooth.h>
}
extern "C" {
#include <pcap/can_socketcan.h>
}
extern "C" {
#include <pcap/compiler-tests.h>
}
extern "C" {
#include <pcap/dlt.h>
}
extern "C" {
#include <pcap/funcattrs.h>
}
extern "C" {
#include <pcap/ipnet.h>
}
extern "C" {
#include <pcap/nflog.h>
}
extern "C" {
#include <pcap/pcap-inttypes.h>
}
extern "C" {
#include <pcap/pcap.h>
}
extern "C" {
#include <pcap/sll.h>
}
extern "C" {
#include <pcap/socket.h>
}
extern "C" {
#include <pcap/usb.h>
}
extern "C" {
#include <pcap/vlan.h>
}
#include <FuzzedDataProvider.h>


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* data, size_t size);


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	FuzzedDataProvider fdp(data, size);
	FDPConsumeIntegral(uint16_t, switch_id, fdp);
	const uint8_t *input = data + sizeof(uint16_t);
	size_t i_size = size - sizeof(uint16_t);
	switch (switch_id) {
		case 0:
			return LLVMFuzzerTestOneInput_46(input, i_size);
			break;
		case 1:
			return LLVMFuzzerTestOneInput_47(input, i_size);
			break;
		case 2:
			return LLVMFuzzerTestOneInput_48(input, i_size);
			break;
		case 3:
			return LLVMFuzzerTestOneInput_49(input, i_size);
			break;
		case 4:
			return LLVMFuzzerTestOneInput_50(input, i_size);
			break;
		case 5:
			return LLVMFuzzerTestOneInput_51(input, i_size);
			break;
		case 6:
			return LLVMFuzzerTestOneInput_52(input, i_size);
			break;
		case 7:
			return LLVMFuzzerTestOneInput_53(input, i_size);
			break;
		case 8:
			return LLVMFuzzerTestOneInput_54(input, i_size);
			break;
		case 9:
			return LLVMFuzzerTestOneInput_55(input, i_size);
			break;
		case 10:
			return LLVMFuzzerTestOneInput_56(input, i_size);
			break;
		case 11:
			return LLVMFuzzerTestOneInput_57(input, i_size);
			break;
		case 12:
			return LLVMFuzzerTestOneInput_58(input, i_size);
			break;
		case 13:
			return LLVMFuzzerTestOneInput_59(input, i_size);
			break;
		case 14:
			return LLVMFuzzerTestOneInput_60(input, i_size);
			break;
		case 15:
			return LLVMFuzzerTestOneInput_61(input, i_size);
			break;
		case 16:
			return LLVMFuzzerTestOneInput_62(input, i_size);
			break;
		case 17:
			return LLVMFuzzerTestOneInput_63(input, i_size);
			break;
		case 18:
			return LLVMFuzzerTestOneInput_64(input, i_size);
			break;
		case 19:
			return LLVMFuzzerTestOneInput_65(input, i_size);
			break;
		case 20:
			return LLVMFuzzerTestOneInput_66(input, i_size);
			break;
		case 21:
			return LLVMFuzzerTestOneInput_67(input, i_size);
			break;
		case 22:
			return LLVMFuzzerTestOneInput_68(input, i_size);
			break;
		case 23:
			return LLVMFuzzerTestOneInput_69(input, i_size);
			break;
		default:
			break;
	}
return 0;
}
