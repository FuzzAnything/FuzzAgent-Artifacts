#include "FDSan.h"
#include "FuzzedDataProvider.h"
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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"pcap_tstamp_type_val_to_name":[[3498,14,3498,49,0,0,4,1],[3499,7,3499,49,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_setdirection","pcap_datalink_ext","pcap_can_set_rfmon","pcap_set_immediate_mode","pcap_tstamp_type_val_to_name","pcap_set_protocol_linux","pcap_set_buffer_size","pcap_next","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_setdirection","pcap_datalink_ext","pcap_can_set_rfmon","pcap_set_immediate_mode","pcap_tstamp_type_val_to_name","pcap_set_protocol_linux","pcap_set_buffer_size","pcap_next","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from input data for pcap_fopen_offline
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open pcap file for reading with default timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Set capture direction
    // Use the first byte to determine direction
    pcap_direction_t direction;
    switch (data[0] % 3) {
        case 0: direction = PCAP_D_IN; break;
        case 1: direction = PCAP_D_OUT; break;
        case 2: direction = PCAP_D_INOUT; break;
    }
    pcap_setdirection(pcap, direction);

    // 2. Get extended datalink type
    int dlt_ext = pcap_datalink_ext(pcap);

    // 3. Check if can set rfmon (monitor mode)
    int can_rfmon = pcap_can_set_rfmon(pcap);

    // 4. Set immediate mode (try to enable if size > 10)
    int immediate_mode = (size > 10) ? 1 : 0;
    pcap_set_immediate_mode(pcap, immediate_mode);

    // 5. Convert timestamp type value to name
    // Use dlt_ext mod 5 to get a valid-ish timestamp type
    int tstamp_type = (dlt_ext % 5);
    const char *tstamp_name = pcap_tstamp_type_val_to_name(tstamp_type);

    // 6. Set protocol for Linux (if applicable)
    // Use a simple protocol value based on input size
    int protocol = (size % 256);
    pcap_set_protocol_linux(pcap, protocol);

    // 7. Set buffer size based on input size
    // Ensure minimum buffer size of 1024, max of 65536
    int buffer_size = 1024 + (size % (65536 - 1024));
    pcap_set_buffer_size(pcap, buffer_size);

    // Try to read some packets to exercise the pcap handle
    struct pcap_pkthdr header;
    const u_char *packet;
    
    int packet_count = 0;
    while ((packet = pcap_next(pcap, &header)) != nullptr && packet_count < 10) {
        packet_count++;
    }

    // Clean up
    pcap_close(pcap);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}