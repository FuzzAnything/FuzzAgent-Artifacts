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
//<ID> 242
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 9
//<Quality> {"density":18,"unique_branches":{"pcap_cleanup_linux":[[995,6,995,22,0,0,4,1],[1003,6,1003,37,0,0,4,1]],"setup_socket":[[2547,7,2547,18,0,0,4,0]],"pcap_activate_linux":[[1253,6,1253,13,0,0,4,0]],"iface_get_arptype":[[5837,6,5837,42,0,0,4,0],[5838,7,5838,22,0,0,4,0]],"pcap_activate":[[2771,6,2771,17,0,0,4,1],[2790,7,2790,27,0,0,4,0]],"pcap_statustostr":[[3744,2,3744,32,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_datalink_ext","pcap_create","pcap_activate","pcap_close","pcap_compile_nopcap","pcap_dump_fopen","pcap_dump_open_append","pcap_getnonblock","pcap_setnonblock","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump","pcap_setnonblock","pcap_freecode","pcap_findalldevs","pcap_freealldevs","pcap_dump_close","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_datalink_ext","pcap_create","pcap_activate","pcap_close","pcap_compile_nopcap","pcap_dump_fopen","pcap_dump_open_append","pcap_getnonblock","pcap_setnonblock","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump","pcap_setnonblock","pcap_freecode","pcap_findalldevs","pcap_freealldevs","pcap_dump_close","pcap_dump_close","pcap_close"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = nullptr;
    pcap_dumper_t *dumper = nullptr;
    pcap_dumper_t *dumper_append = nullptr;
    pcap_if_t *alldevs = nullptr;
    struct bpf_program fp;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // 1. Create a pcap handle for offline reading
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Get datalink type
    int dlt = pcap_datalink_ext(pcap);
    
    // 3. Create dummy pcap handle for compilation
    pcap_t *dummy_pcap = pcap_create(fuzz_str_1, errbuf);
    if (dummy_pcap) {
        pcap_activate(dummy_pcap);
        pcap_close(dummy_pcap);
    }
    
    // 4. Compile a BPF filter (using first byte as filter string length hint)
    int filter_len = (data[0] % 64) + 1;
    if (filter_len > (int)size) filter_len = size;
    
    char *filter_str = (char *)malloc(filter_len + 1);
    if (filter_str) {
        memcpy(filter_str, data, filter_len);
        filter_str[filter_len] = '\0';
        
        // Use pcap_compile_nopcap with dummy values
        if (pcap_compile_nopcap(fuzz_int32_t_2, dlt, &fp, filter_str, fuzz_int32_t_3, fuzz_uint32_t_4) == 0) {
            // 5. Apply filter to packets
            struct pcap_pkthdr header;
            const u_char *packet;
            
            // Open output file for dumping
            out_file = fopen("output_file", "wb");
            if (out_file) {
                // 6. Create dumpers
                dumper = pcap_dump_fopen(pcap, out_file);
                dumper_append = pcap_dump_open_append(pcap, "output_file_append");
                
                // 7. Set non-blocking mode
                char nonblock_err[PCAP_ERRBUF_SIZE];
                int nonblock_old;
                pcap_getnonblock(pcap, nonblock_err);
                pcap_setnonblock(pcap, 1, nonblock_err);
                
                // 8. Process packets
                while ((packet = pcap_next(pcap, &header)) != nullptr) {
                    if (pcap_offline_filter(&fp, &header, packet)) {
                        // Dump filtered packets
                        pcap_dump((u_char *)dumper, &header, packet);
                        if (dumper_append) {
                            pcap_dump((u_char *)dumper_append, &header, packet);
                        }
                    }
                }
                
                // 9. Restore non-blocking mode
                pcap_setnonblock(pcap, 0, nonblock_err);
                
                // 10. Free BPF program
                pcap_freecode(&fp);
            }
            free(filter_str);
        } else {
            free(filter_str);
        }
    }
    
    // 11. Find all devices (just to use the API)
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        pcap_freealldevs(alldevs);
    }
    
    // 12. Cleanup
    if (dumper_append) pcap_dump_close(dumper_append);
    if (dumper) pcap_dump_close(dumper);
    if (out_file) assert_file_closed(&out_file);;
    if (pcap) pcap_close(pcap);
    if (in_file) assert_file_closed(&in_file);;
    
    // Clean up temporary files
    remove("output_file");
    remove("output_file_append");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}