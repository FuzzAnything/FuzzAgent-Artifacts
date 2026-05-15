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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["pcap_init","pcap_lib_version","pcap_fopen_offline","pcap_create","pcap_set_immediate_mode","pcap_getnonblock","pcap_close","pcap_close","pcap_dump_fopen","pcap_close","pcap_close","pcap_compile","bpf_dump","pcap_freecode","pcap_findalldevs","pcap_next_ex","pcap_dump","pcap_dump_ftell64","pcap_dump_close","pcap_freealldevs","pcap_close","pcap_close"],"critical_calls":["pcap_init","pcap_lib_version","pcap_fopen_offline","pcap_create","pcap_set_immediate_mode","pcap_getnonblock","pcap_dump_fopen","pcap_compile","bpf_dump","pcap_freecode","pcap_findalldevs","pcap_next_ex","pcap_dump","pcap_dump_ftell64","pcap_dump_close","pcap_freealldevs","pcap_close","pcap_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
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




    if (size == 0) {
        return 0;
    }

    // 1. Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        // Initialization failed, but continue with limited functionality
    }

    // 2. Get libpcap version (for information)
    const char *version = pcap_lib_version();

    // 3. Create a FILE* from input data for reading pcap data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Open offline pcap capture from memory
    pcap_t *pcap_offline = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_offline) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Create a live capture pcap (dummy, for API usage)
    pcap_t *pcap_live = pcap_create(fuzz_str_1, errbuf);
    if (pcap_live) {
        // 6. Set immediate mode
        pcap_set_immediate_mode(pcap_live, fuzz_int32_t_2);
        
        // 7. Get non-blocking mode
        char nonblock_err[PCAP_ERRBUF_SIZE] = {0};
        pcap_getnonblock(pcap_live, nonblock_err);
    }

    // 8. Open output file for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_offline);
        if (pcap_live) pcap_close(pcap_live);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 9. Create dumper for output
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap_offline, out_file);
    if (!dumper) {
        assert_file_closed(&out_file);;
        pcap_close(pcap_offline);
        if (pcap_live) pcap_close(pcap_live);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 10. Compile a BPF filter (simple filter to match some packets)
    struct bpf_program fp;
    const char *filter_str = "ip or arp";
    bpf_u_int32 netmask = 0xffffff00;
    if (pcap_compile(pcap_offline, &fp, fuzz_str_3, fuzz_int32_t_4, fuzz_uint32_t_5) == 0) {
        // 11. Dump BPF program (to stderr for debugging)
        bpf_dump(&fp, fuzz_int32_t_6);
        pcap_freecode(&fp);
    }

    // 12. Find all network devices (for API usage)
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Device listing failed, but continue
    }

    // 13. Process packets from offline capture
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;
    int packet_count = 0;
    
    while (packet_count < 10 && pcap_next_ex(pcap_offline, &header, &packet_data) == 1) {
        // 14. Dump packet to output file
        pcap_dump((u_char *)dumper, header, packet_data);
        packet_count++;
        
        // 15. Get current position in dump file
        int64_t dump_pos = pcap_dump_ftell64(dumper);
    }

    // 16. Close dumper
    pcap_dump_close(dumper);

    // 17. Free allocated resources
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
    
    if (pcap_live) {
        pcap_close(pcap_live);
    }
    
    pcap_close(pcap_offline);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}