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


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* data, size_t size);
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* data, size_t size);


extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
	FuzzedDataProvider fdp(data, size);
	FDPConsumeIntegral(uint16_t, switch_id, fdp);
	const uint8_t *input = data + sizeof(uint16_t);
	size_t i_size = size - sizeof(uint16_t);
	switch (switch_id) {
		case 0:
			return LLVMFuzzerTestOneInput_33(input, i_size);
			break;
		case 1:
			return LLVMFuzzerTestOneInput_34(input, i_size);
			break;
		case 2:
			return LLVMFuzzerTestOneInput_35(input, i_size);
			break;
		case 3:
			return LLVMFuzzerTestOneInput_36(input, i_size);
			break;
		case 4:
			return LLVMFuzzerTestOneInput_37(input, i_size);
			break;
		case 5:
			return LLVMFuzzerTestOneInput_38(input, i_size);
			break;
		case 6:
			return LLVMFuzzerTestOneInput_39(input, i_size);
			break;
		case 7:
			return LLVMFuzzerTestOneInput_40(input, i_size);
			break;
		case 8:
			return LLVMFuzzerTestOneInput_41(input, i_size);
			break;
		case 9:
			return LLVMFuzzerTestOneInput_42(input, i_size);
			break;
		case 10:
			return LLVMFuzzerTestOneInput_43(input, i_size);
			break;
		case 11:
			return LLVMFuzzerTestOneInput_44(input, i_size);
			break;
		case 12:
			return LLVMFuzzerTestOneInput_45(input, i_size);
			break;
		case 13:
			return LLVMFuzzerTestOneInput_46(input, i_size);
			break;
		case 14:
			return LLVMFuzzerTestOneInput_47(input, i_size);
			break;
		case 15:
			return LLVMFuzzerTestOneInput_48(input, i_size);
			break;
		default:
			break;
	}
return 0;
}
