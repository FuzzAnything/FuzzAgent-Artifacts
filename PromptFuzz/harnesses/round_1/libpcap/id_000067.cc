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
//<ID> 441
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 4
//<Quality> {"density":14,"unique_branches":{"gen_proto":[[6807,2,6807,13,0,0,4,0]],"gen_ncode":[[7487,2,7487,14,0,0,4,0]],"pcap_lex":[[3752,1,3752,8,0,0,4,0]],"pcap_parse":[[2163,3,2163,10,0,0,4,0]]},"library_calls":["pcap_init","pcap_fopen_offline","pcap_is_swapped","pcap_set_buffer_size","pcap_setdirection","pcap_next_ex","pcap_datalink","pcap_snapshot","pcap_compile_nopcap","pcap_freecode","pcap_create","pcap_inject","pcap_close","pcap_close"],"critical_calls":["pcap_init","pcap_fopen_offline","pcap_is_swapped","pcap_set_buffer_size","pcap_setdirection","pcap_next_ex","pcap_datalink","pcap_snapshot","pcap_compile_nopcap","pcap_freecode","pcap_create","pcap_inject","pcap_close","pcap_close"],"visited":0}
/**/


// Include necessary headers for libpcap

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Event: Simulating packet capture from memory, processing with filters,
    //        checking properties, and injecting packets back
    if (size < sizeof(struct pcap_pkthdr)) {
        return 0;
    }

    // 1. Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        // Initialization failed, but continue with other APIs
    }

    // 2. Open input file stream from memory
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Create pcap reader from file
    pcap_t* pcap_reader = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_reader) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Check if capture is byte-swapped
    int swapped = pcap_is_swapped(pcap_reader);
    (void)swapped; // Use variable to avoid unused warning

    // 5. Set buffer size for reader
    pcap_set_buffer_size(pcap_reader, 1024 * 1024); // 1MB buffer

    // 6. Set capture direction (inbound only)
    pcap_setdirection(pcap_reader, PCAP_D_IN);

    // 7. Prepare for packet reading
    struct pcap_pkthdr* header = nullptr;
    const u_char* packet_data = nullptr;
    
    // Read a few packets (limit to prevent infinite loops)
    int packet_count = 0;
    while (packet_count < 10 && pcap_next_ex(pcap_reader, &header, &packet_data) == 1) {
        packet_count++;
        
        // 8. Compile a simple filter program (using Ethernet type 0x0800 for IP)
        struct bpf_program filter;
        const char* filter_str = "ether proto 0x0800";
        
        // Get datalink type and snapshot length from pcap reader
        int dlt = pcap_datalink(pcap_reader);
        int snaplen = pcap_snapshot(pcap_reader);
        
        // Compile filter without pcap context
        if (pcap_compile_nopcap(snaplen, dlt, &filter, filter_str, 0, 0) == 0) {
            // Filter compiled successfully
            pcap_freecode(&filter);
        }
        
        // 9. Create a dummy pcap handle for injection
        pcap_t* pcap_injector = pcap_create("lo", errbuf);
        if (pcap_injector) {
            // Try to inject the packet (won't actually send on loopback without activation)
            pcap_inject(pcap_injector, packet_data, header->caplen);
            pcap_close(pcap_injector);
        }
    }

    // 10. Cleanup
    pcap_close(pcap_reader);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}