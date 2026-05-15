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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_snapshot","pcap_datalink","pcap_dump_open","pcap_dump_open_append","pcap_next","pcap_dump","pcap_compile_nopcap","bpf_dump","pcap_freecode","pcap_dump_close","pcap_dump_close","pcap_close"],"critical_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_snapshot","pcap_datalink","pcap_dump_open","pcap_dump_open_append","pcap_next","pcap_dump","pcap_compile_nopcap","bpf_dump","pcap_freecode","pcap_dump_close","pcap_dump_close","pcap_close"],"visited":1}
/**/


// Include libpcap headers

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if input is too small for meaningful processing
    if (size < 24) {
	return 0;
	}
	
    
    // Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_init(0, errbuf) == -1) {
        return 0;
    }
    
    // 1. Look up a network device
    char* device = pcap_lookupdev(errbuf);
    
    // 2. Create input FILE* from fuzzing data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Open pcap capture from the input data
    pcap_t* pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get snapshot length
    int snaplen = pcap_snapshot(pcap);
    
    // 5. Compile a BPF filter (using part of input data as filter string)
    struct bpf_program filter;
    std::string filter_str;
    if (size > 100) {
        // Use up to 100 bytes from input as filter string
        size_t filter_len = std::min(size_t(100), size);
        filter_str.assign(reinterpret_cast<const char*>(data), filter_len);
        // Ensure it's null-terminated and only printable
        for (char& c : filter_str) {
            if (c < 32 || c > 126) c = ' ';
        }
    } else {
        filter_str = "ip";
    }
    
    // Use pcap_compile_nopcap - need to extract linktype from pcap handle
    int linktype = pcap_datalink(pcap);
    
    // Create output file for dumper
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // 6. Open pcap dumper for writing
        pcap_dumper_t* dumper = pcap_dump_open(pcap, "output_file");
        
        if (dumper) {
            // 7. Try to open same file in append mode
            pcap_dumper_t* append_dumper = pcap_dump_open_append(pcap, "output_file");
            
            // Process some packets if available
            struct pcap_pkthdr header;
            const u_char* packet;
            int packet_count = 0;
            
            while (packet_count < 10 && (packet = pcap_next(pcap, &header)) != nullptr) {
                // Write packet to dumper
                pcap_dump((u_char*)dumper, &header, packet);
                packet_count++;
            }
            
            // 8. Try to compile a BPF filter using pcap_compile_nopcap
            if (pcap_compile_nopcap(snaplen, linktype, &filter, 
                                   filter_str.c_str(), 1, 0) == 0) {
                // 9. Dump the compiled BPF program
                bpf_dump(&filter, 1);
                
                // Free the filter
                pcap_freecode(&filter);
            }
            
            // Close dumpers
            if (append_dumper) pcap_dump_close(append_dumper);
            pcap_dump_close(dumper);
        }
        
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // 10. Clean up resources
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}