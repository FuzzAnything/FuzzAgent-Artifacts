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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 7
//<Quality> {"density":10,"unique_branches":{"netfilter_create":[[749,6,749,59,0,0,4,0],[751,11,751,68,0,0,4,0],[763,6,763,16,0,0,4,0],[763,20,763,31,0,0,4,1],[773,6,773,15,0,0,4,1]],"pcap_create":[[2391,7,2391,16,0,0,4,0],[2401,8,2401,17,0,0,4,1]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_findalldevs","pcap_close","pcap_next_ex","pcap_create","pcap_set_tstamp_type","pcap_setnonblock","pcap_close","pcap_freealldevs","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_findalldevs","pcap_next_ex","pcap_create","pcap_set_tstamp_type","pcap_setnonblock","pcap_close","pcap_freealldevs","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Create a FILE* from input data for reading pcap dump
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Open the pcap dump file with specific timestamp precision
    const u_int precision = PCAP_TSTAMP_PRECISION_MICRO; // Example precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *dump_pcap = pcap_fopen_offline_with_tstamp_precision(in_file, precision, errbuf);
    if (!dump_pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get all available network devices
    pcap_if_t *all_devices = nullptr;
    if (pcap_findalldevs(&all_devices, errbuf) == -1) {
        pcap_close(dump_pcap);
        // Note: in_file is already closed by pcap_close(dump_pcap) for offline captures
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Read packets from the dump file using pcap_next_ex
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;
    int packet_count = 0;
    
    while (packet_count < 100) { // Limit to prevent infinite loops with malformed data
        int result = pcap_next_ex(dump_pcap, &header, &packet_data);
        if (result <= 0) {
            break; // No more packets or error
        }
        packet_count++;
    }

    // 5. Process each found network device
    for (pcap_if_t *device = all_devices; device != nullptr; device = device->next) {
        // Create a pcap handle for this device (without activating)
        pcap_t *live_pcap = pcap_create(device->name, errbuf);
        if (!live_pcap) {
            continue;
        }

        // Set timestamp type (example: PCAP_TSTAMP_HOST)
        pcap_set_tstamp_type(live_pcap, PCAP_TSTAMP_HOST);

        // Set non-blocking mode
        char nonblock_err[PCAP_ERRBUF_SIZE] = {0};
        pcap_setnonblock(live_pcap, 1, nonblock_err);

        // Close the live capture handle (we're not activating it for fuzzing)
        pcap_close(live_pcap);
    }

    // 6. Clean up all resources
    pcap_freealldevs(all_devices);
    pcap_close(dump_pcap);
    // Note: dump_pcap close also closes the underlying FILE* (in_file)
    
    assert_file_closed(&in_file);
	return 0;
}