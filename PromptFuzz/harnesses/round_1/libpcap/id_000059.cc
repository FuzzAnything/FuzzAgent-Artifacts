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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"or_pullup":[[1933,6,1933,12,0,0,4,1]]},"library_calls":["pcap_init","pcap_fopen_offline","pcap_getnonblock","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_list_datalinks","pcap_free_datalinks","pcap_snapshot","pcap_datalink","pcap_compile_nopcap","pcap_freecode","pcap_dump_fopen","pcap_next_ex","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"critical_calls":["pcap_init","pcap_fopen_offline","pcap_getnonblock","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_list_datalinks","pcap_free_datalinks","pcap_snapshot","pcap_datalink","pcap_compile_nopcap","pcap_freecode","pcap_dump_fopen","pcap_next_ex","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"visited":1}
/**/


// Include necessary libpcap headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < sizeof(pcap_pkthdr) + 1) {
        return 0;
    }

    // Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(0, errbuf) != 0) {
        // Non-fatal - continue execution
    }

    // Create input FILE* from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open pcap capture from memory file with default timestamp precision
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get non-blocking status
    char nonblock_err[PCAP_ERRBUF_SIZE] = {0};
    pcap_getnonblock(pcap, nonblock_err);

    // List timestamp types
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap, &tstamp_types);
    if (tstamp_types) {
        pcap_free_tstamp_types(tstamp_types);
    }

    // List data link types
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
    if (dlt_list) {
        pcap_free_datalinks(dlt_list);
    }

    // Compile BPF filter without pcap handle
    struct bpf_program bpf_prog;
    const char *filter_str = "ip or arp";  // Simple filter expression
    int snaplen = pcap_snapshot(pcap);
    int linktype = pcap_datalink(pcap);
    
    // Use data to influence filter compilation - take first 100 bytes as string if available
    char dynamic_filter[101] = {0};
    if (size > 100) {
        size_t filter_len = (size > 100) ? 100 : size;
        memcpy(dynamic_filter, data, filter_len);
        dynamic_filter[filter_len] = '\0';
        
        // Replace non-printable characters with spaces to make it a valid filter string
        for (size_t i = 0; i < filter_len; i++) {
            if (dynamic_filter[i] < 32 || dynamic_filter[i] > 126) {
                dynamic_filter[i] = ' ';
            }
        }
        filter_str = dynamic_filter;
    }
    
    if (pcap_compile_nopcap(snaplen, linktype, &bpf_prog, 
                           filter_str, 1, PCAP_NETMASK_UNKNOWN) == 0) {
        pcap_freecode(&bpf_prog);
    }

    // Open output file for dumping packets
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        pcap_dumper_t *dumper = pcap_dump_fopen(pcap, out_file);
        if (dumper) {
            // Read and dump packets
            struct pcap_pkthdr *header = nullptr;
            const u_char *packet_data = nullptr;
            
            while (pcap_next_ex(pcap, &header, &packet_data) == 1) {
                // Ensure we have valid packet data
                if (header && packet_data && 
                    header->caplen > 0 && 
                    header->caplen <= header->len) {
                    
                    // Create a copy of packet data for dumping
                    u_char *dump_data = (u_char *)malloc(header->caplen);
                    if (dump_data) {
                        memcpy(dump_data, packet_data, header->caplen);
                        
                        // Use the custom dump function (note: first param is user data)
                        pcap_dump((u_char *)dumper, header, dump_data);
                        
                        free(dump_data);
                    }
                }
            }
            
            // Flush the dumper
            pcap_dump_flush(dumper);
            
            // Clean up dumper
            pcap_dump_close(dumper);
        }
        assert_file_closed(&out_file);;
        remove("output_file");  // Clean up temporary file
    }

    // Cleanup
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}