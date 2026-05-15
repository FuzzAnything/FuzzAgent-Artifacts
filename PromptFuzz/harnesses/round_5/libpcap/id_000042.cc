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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 55, nr_unique_branch: 6
//<Quality> {"density":22,"unique_branches":{"pcap_set_timeout":[[2626,6,2626,32,0,0,4,1]],"pcapint_filter_with_aux_data":[[98,6,98,13,0,0,4,0]],"set_poll_timeout":[[1090,13,1090,33,0,0,4,0],[1097,7,1097,40,0,0,4,0],[1097,44,1097,62,0,0,4,0]],"create_ring":[[3358,6,3358,26,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_lookupnet","pcap_list_datalinks","pcap_free_datalinks","pcap_close","pcap_dump_open_append","pcap_close","pcap_compile","pcap_next","pcap_offline_filter","bpf_filter","pcap_dump","pcap_create","pcap_set_snaplen","pcap_set_promisc","pcap_set_timeout","pcap_activate","pcap_inject","pcap_close","pcap_freecode","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_lookupnet","pcap_list_datalinks","pcap_free_datalinks","pcap_dump_open_append","pcap_compile","pcap_next","pcap_offline_filter","bpf_filter","pcap_dump","pcap_create","pcap_set_snaplen","pcap_set_promisc","pcap_set_timeout","pcap_activate","pcap_inject","pcap_close","pcap_freecode","pcap_dump_close","pcap_close"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Open pcap file for reading
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get network address and mask (using dummy device name)
    bpf_u_int32 net, mask;
    const char *device = "eth0";
    if (pcap_lookupnet(device, &net, &mask, errbuf) == -1) {
        // Continue even if this fails
    }

    // 4. Get list of data link types
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
    if (dlt_list) {
        pcap_free_datalinks(dlt_list);
    }

    // 5. Open output file for dumping packets
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 6. Open dumper for appending
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap_handle, "output_file");
    if (!dumper) {
        assert_file_closed(&out_file);;
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // 7. Create a simple BPF filter program
    struct bpf_program filter;
    const char *filter_str = "ip";
    if (pcap_compile(pcap_handle, &filter, filter_str, 1, mask) == -1) {
        // Continue even if compilation fails
    }

    // 8. Process packets
    struct pcap_pkthdr header;
    const u_char *packet;
    int packet_count = 0;
    const int MAX_PACKETS = 10; // Limit to avoid infinite loops

    while (packet_count < MAX_PACKETS) {
        packet = pcap_next(pcap_handle, &header);
        if (!packet) break;

        // 9. Apply offline filter
        int filter_result = pcap_offline_filter(&filter, &header, packet);
        
        // 10. Apply low-level BPF filter
        u_int bpf_result = bpf_filter(filter.bf_insns, packet, header.len, header.caplen);

        // 11. Dump packet if it passes both filters
        if (filter_result && bpf_result > 0) {
            pcap_dump((u_char *)dumper, &header, packet);
            
            // 12. Try to inject packet (simulate on loopback)
            pcap_t *inject_handle = pcap_create("lo", errbuf);
            if (inject_handle) {
                pcap_set_snaplen(inject_handle, 65535);
                pcap_set_promisc(inject_handle, 0);
                pcap_set_timeout(inject_handle, 1000);
                
                if (pcap_activate(inject_handle) == 0) {
                    pcap_inject(inject_handle, packet, header.caplen);
                }
                pcap_close(inject_handle);
            }
        }
        
        packet_count++;
    }

    // 13. Clean up resources
    pcap_freecode(&filter);
    pcap_dump_close(dumper);
    pcap_close(pcap_handle);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // 14. Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}