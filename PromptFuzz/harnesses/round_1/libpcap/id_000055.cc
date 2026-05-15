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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_datalink_ext","pcap_lookupdev","pcap_create","pcap_set_rfmon","pcap_major_version","pcap_minor_version","pcap_setdirection","pcap_set_datalink","pcap_activate","pcap_dump_fopen","pcap_freecode","pcap_next","pcap_dump","pcap_dump_close","pcap_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_datalink_ext","pcap_lookupdev","pcap_create","pcap_set_rfmon","pcap_major_version","pcap_minor_version","pcap_setdirection","pcap_set_datalink","pcap_activate","pcap_dump_fopen","pcap_freecode","pcap_next","pcap_dump","pcap_dump_close","pcap_close","pcap_close"],"visited":0}
/**/


// Libpcap headers

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Step 1: Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Open offline pcap file from memory
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *offline_pcap = pcap_fopen_offline(in_file, errbuf);
    if (!offline_pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Get data link type from offline capture
    int dlt = pcap_datalink_ext(offline_pcap);
    
    // Step 4: Find a network device for live capture
    char *device = pcap_lookupdev(errbuf);
    pcap_t *live_pcap = nullptr;
    
    if (device) {
        // Step 5: Create live capture handle
        live_pcap = pcap_create(device, errbuf);
        
        if (live_pcap) {
            // Step 6: Try to set monitor mode (rfmon)
            pcap_set_rfmon(live_pcap, 1); // Try to enable, but ignore failure
            
            // Step 7: Get version information (for completeness)
            int major = pcap_major_version(live_pcap);
            int minor = pcap_minor_version(live_pcap);
            (void)major; // Unused in fuzzing context
            (void)minor; // Unused in fuzzing context
            
            // Step 8: Set capture direction
            pcap_setdirection(live_pcap, PCAP_D_INOUT);
            
            // Step 9: Set data link type from offline capture
            pcap_set_datalink(live_pcap, dlt);
            
            // Step 10: Activate the live capture
            pcap_activate(live_pcap);
            
            // Step 11: Create output file for dumping
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                // Step 12: Create dumper for live capture
                pcap_dumper_t *dumper = pcap_dump_fopen(live_pcap, out_file);
                if (dumper) {
                    // Step 13: Create dummy bpf_program and free it
                    struct bpf_program filter;
                    filter.bf_len = 0;
                    filter.bf_insns = nullptr;
                    pcap_freecode(&filter);
                    
                    // Step 14: Process packets from offline capture
                    struct pcap_pkthdr header;
                    const u_char *packet;
                    
                    while ((packet = pcap_next(offline_pcap, &header)) != nullptr) {
                        // Dump packet to output file
                        pcap_dump((u_char *)dumper, &header, packet);
                    }
                    
                    pcap_dump_close(dumper);
                }
                assert_file_closed(&out_file);;
            }
        }
    }

    // Step 15: Cleanup resources
    if (live_pcap) {
        pcap_close(live_pcap);
    }
    
    pcap_close(offline_pcap);
    assert_file_closed(&in_file);;
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}