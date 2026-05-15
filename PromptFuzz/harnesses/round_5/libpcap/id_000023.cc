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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"pcap_bufsize":[[3552,6,3552,19,0,0,4,0]],"pcap_set_protocol_linux":[[6235,6,6235,32,0,0,4,1]],"pcap_set_immediate_mode":[[2680,6,2680,32,0,0,4,1]]},"library_calls":["pcap_findalldevs","pcap_create","pcap_set_immediate_mode","pcap_set_protocol_linux","pcap_bufsize","bpf_filter","pcap_fopen_offline","pcap_dump_fopen","pcap_compile","pcap_next_ex","bpf_filter","pcap_dump","pcap_freecode","pcap_close","pcap_dump_close","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_findalldevs","pcap_create","pcap_set_immediate_mode","pcap_set_protocol_linux","pcap_bufsize","bpf_filter","pcap_fopen_offline","pcap_dump_fopen","pcap_compile","pcap_next_ex","bpf_filter","pcap_dump","pcap_freecode","pcap_close","pcap_dump_close","pcap_close","pcap_freealldevs"],"visited":1}
/**/


// libpcap headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
        return 0;
    }

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    pcap_dumper_t *dumper = nullptr;
    pcap_if_t *alldevs = nullptr;
    struct bpf_program fp;
    const u_char *pkt_data;
    struct pcap_pkthdr *pkt_header;

    // 1. Find all network devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Cleanup and continue with file processing
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create a pcap handle for live capture (using first device if available)
    if (alldevs) {
        pcap_handle = pcap_create(alldevs->name, errbuf);
        if (pcap_handle) {
            // 3. Set immediate mode (no buffering)
            pcap_set_immediate_mode(pcap_handle, 1);
            
            // 4. Set Linux protocol (requires Linux)
            #ifdef __linux__
            pcap_set_protocol_linux(pcap_handle, 0); // ETH_P_ALL
            #endif
            
            // 5. Get buffer size
            int buf_size = pcap_bufsize(pcap_handle);
            (void)buf_size; // Use buf_size to avoid unused variable warning
            
            // Create a simple BPF filter program
            struct bpf_insn insns[] = {
                { 0x06, 0, 0, 0x00040000 } // Return up to 4 bytes
            };
            
            // 6. Test BPF filter on our input data
            // We'll use a small portion of data for testing
            size_t test_size = size > 100 ? 100 : size;
            u_int result = bpf_filter(insns, data, test_size, test_size);
            (void)result; // Use result to avoid unused variable warning
        }
    }

    // Now process the input data as a capture file
    pcap_t *file_handle = pcap_fopen_offline(in_file, errbuf);
    if (file_handle) {
        // Create dumper for output
        dumper = pcap_dump_fopen(file_handle, out_file);
        
        // Simple BPF filter that accepts all packets
        if (pcap_compile(file_handle, &fp, "ip or arp", 1, PCAP_NETMASK_UNKNOWN) == 0) {
            // Process packets
            while (pcap_next_ex(file_handle, &pkt_header, &pkt_data) >= 0) {
                // Apply BPF filter to each packet
                u_int filter_result = bpf_filter(fp.bf_insns, pkt_data, 
                                                pkt_header->len, pkt_header->caplen);
                if (filter_result != 0) {
                    // Write packet to output if it passes filter
                    pcap_dump((u_char *)dumper, pkt_header, pkt_data);
                }
            }
            pcap_freecode(&fp);
        }
        
        // Close file handle
        pcap_close(file_handle);
    }

    // Cleanup
    if (dumper) {
        pcap_dump_close(dumper);
    }
    
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}