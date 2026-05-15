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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"pcap_setnonblock_linux":[[3539,7,3539,39,0,0,4,0]]},"library_calls":["pcap_findalldevs","pcap_fopen_offline","pcap_freealldevs","pcap_datalink_ext","pcap_create","pcap_activate","pcap_setnonblock","pcap_getnonblock","pcap_compile_nopcap","pcap_dump_fopen","pcap_dump_open_append","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump","pcap_freecode","pcap_dump_close","pcap_dump_close","pcap_close","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_findalldevs","pcap_fopen_offline","pcap_datalink_ext","pcap_create","pcap_activate","pcap_setnonblock","pcap_getnonblock","pcap_compile_nopcap","pcap_dump_fopen","pcap_dump_open_append","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump","pcap_freecode","pcap_dump_close","pcap_dump_close","pcap_close","pcap_close","pcap_freealldevs"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    pcap_dumper_t *dumper1 = nullptr;
    pcap_dumper_t *dumper2 = nullptr;
    pcap_if_t *alldevs = nullptr;
    struct bpf_program fp;
    
    // 1. Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Continue anyway - this is okay for fuzzing
    }
    
    // 2. Open offline capture from memory
    pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        if (alldevs) pcap_freealldevs(alldevs);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get datalink type
    int dlt = pcap_datalink_ext(pcap_handle);
    
    // 4. Create a dummy pcap handle for compilation
    pcap_t *dummy_handle = pcap_create(fuzz_str_1, errbuf);
    if (dummy_handle) {
        // 5. Activate the dummy handle
        if (pcap_activate(dummy_handle) == 0) {
            // 6. Set non-blocking mode on dummy handle
            char nonblock_err[PCAP_ERRBUF_SIZE];
            if (pcap_setnonblock(dummy_handle, 1, nonblock_err) == 0) {
                // 7. Get non-blocking status
                char getnonblock_err[PCAP_ERRBUF_SIZE];
                pcap_getnonblock(dummy_handle, getnonblock_err);
            }
        }
    }
    
    // 8. Compile BPF filter using nopcap
    const char *filter_str = "ip or arp or tcp or udp";
    if (pcap_compile_nopcap(fuzz_int32_t_3, dlt, &fp, fuzz_str_2, fuzz_int32_t_4, fuzz_uint32_t_5) == 0) {
        // 9. Process packets from input file
        struct pcap_pkthdr header;
        const u_char *packet;
        
        // 10. Open first dumper
        dumper1 = pcap_dump_fopen(pcap_handle, out_file);
        
        // 11. Open second dumper in append mode
        dumper2 = pcap_dump_open_append(pcap_handle, "output_file");
        
        while ((packet = pcap_next(pcap_handle, &header)) != nullptr) {
            // 12. Apply offline filter
            if (pcap_offline_filter(&fp, &header, packet)) {
                // 13. Dump filtered packets
                if (dumper1) {
                    pcap_dump((u_char *)dumper1, &header, packet);
                }
                if (dumper2) {
                    pcap_dump((u_char *)dumper2, &header, packet);
                }
            }
        }
        
        // 14. Free compiled filter
        pcap_freecode(&fp);
    }
    
    // 15. Cleanup
    if (dumper1) pcap_dump_close(dumper1);
    if (dumper2) pcap_dump_close(dumper2);
    if (pcap_handle) pcap_close(pcap_handle);
    if (dummy_handle) pcap_close(dummy_handle);
    if (alldevs) pcap_freealldevs(alldevs);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}