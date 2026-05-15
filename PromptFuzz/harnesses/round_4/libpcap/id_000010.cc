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
//<ID> 384
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["pcap_init","pcap_lib_version","pcap_findalldevs","pcap_freealldevs","pcap_fopen_offline","pcap_freealldevs","pcap_create","pcap_set_immediate_mode","pcap_getnonblock","pcap_close","pcap_close","pcap_freealldevs","pcap_dump_fopen","pcap_close","pcap_freealldevs","pcap_compile","bpf_dump","pcap_freecode","pcap_next_ex","pcap_dump","pcap_dump_ftell64","pcap_dump_close","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_init","pcap_lib_version","pcap_findalldevs","pcap_fopen_offline","pcap_create","pcap_set_immediate_mode","pcap_getnonblock","pcap_close","pcap_dump_fopen","pcap_compile","bpf_dump","pcap_freecode","pcap_next_ex","pcap_dump","pcap_dump_ftell64","pcap_dump_close","pcap_close","pcap_freealldevs"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // 1. Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(0, errbuf) != 0) {
        // Initialization failed, but continue anyway
    }
    
    // 2. Get libpcap version (optional, just to use the API)
    const char* version = pcap_lib_version();
    (void)version; // Prevent unused variable warning
    
    // 3. Find all network devices (just to use the API)
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Couldn't find devices, but continue anyway
    }
    
    // 4. Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        pcap_freealldevs(alldevs);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Open pcap capture from file
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        pcap_freealldevs(alldevs);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create another pcap handle for demonstration (using pcap_create)
    pcap_t *dummy_pcap = pcap_create(fuzz_str_1, errbuf); // Try loopback device
    if (dummy_pcap) {
        // 7. Set immediate mode
        pcap_set_immediate_mode(dummy_pcap, fuzz_int32_t_2);
        
        // 8. Get non-blocking mode
        char nonblock_err[PCAP_ERRBUF_SIZE] = {0};
        pcap_getnonblock(dummy_pcap, nonblock_err);
        
        // Close dummy handle
        pcap_close(dummy_pcap);
    }
    
    // 9. Create output FILE* for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        pcap_freealldevs(alldevs);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 10. Open dumper for writing packets
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap_handle, out_file);
    if (!dumper) {
        assert_file_closed(&out_file);;
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        pcap_freealldevs(alldevs);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 11. Compile a BPF filter (try simple "ip" filter)
    struct bpf_program filter;
    const char *filter_str = "ip";
    bpf_u_int32 netmask = 0xffffff00; // Default netmask
    if (pcap_compile(pcap_handle, &filter, fuzz_str_3, fuzz_int32_t_4, fuzz_uint32_t_5) == 0) {
        // 12. Dump the compiled filter (for debugging)
        bpf_dump(&filter, fuzz_int32_t_6);
        
        // Free the compiled filter
        pcap_freecode(&filter);
    }
    
    // 13. Read and process packets from input
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;
    int packet_count = 0;
    
    while (pcap_next_ex(pcap_handle, &header, &packet_data) == 1) {
        // 14. Dump packet to output file
        pcap_dump((u_char *)dumper, header, packet_data);
        packet_count++;
        
        // Break after processing some packets to avoid infinite loop
        if (packet_count > 100) {
            break;
        }
    }
    
    // 15. Get current position in dump file
    int64_t dump_pos = pcap_dump_ftell64(dumper);
    (void)dump_pos; // Prevent unused variable warning
    
    // 16. Clean up resources
    
    // Close dumper
    pcap_dump_close(dumper);
    
    // Close pcap handle
    pcap_close(pcap_handle);
    
    // Close file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 17. Free device list
    pcap_freealldevs(alldevs);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}