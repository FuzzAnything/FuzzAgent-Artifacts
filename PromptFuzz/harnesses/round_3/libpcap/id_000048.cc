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
//<ID> 334
//<Prompt> []
/*<Combination>: [
*/
//<score> 112, nr_unique_branch: 61
//<Quality> {"density":16,"unique_branches":{"iface_get_arptype":[[5837,6,5837,42,0,0,4,1]],"setup_socket":[[2547,7,2547,18,0,0,4,1],[2552,7,2552,17,0,0,4,1],[2556,7,2556,29,0,0,4,1],[2557,7,2557,40,0,0,4,1],[2558,7,2558,41,0,0,4,1],[2559,7,2559,41,0,0,4,1],[2560,7,2560,38,0,0,4,1],[2561,8,2561,38,0,0,4,0],[2562,9,2562,40,0,0,4,1],[2563,9,2563,40,0,0,4,1],[2638,7,2638,29,0,0,4,1],[2643,7,2644,31,0,0,4,1],[2709,6,2709,20,0,0,4,0],[2709,24,2709,43,0,0,4,1],[2751,6,2751,21,0,0,4,1]],"iface_bind":[[4862,21,4862,32,0,0,4,1]],"map_arphrd_to_dlt":[[2052,2,2052,19,0,0,4,1],[2125,2,2125,22,0,0,4,1],[2126,2,2126,22,0,0,4,0],[2131,2,2131,20,0,0,4,1],[2135,2,2135,18,0,0,4,1],[2139,2,2139,20,0,0,4,1],[2143,2,2143,19,0,0,4,1],[2147,2,2147,17,0,0,4,1],[2151,2,2151,24,0,0,4,1],[2152,2,2152,21,0,0,4,1],[2157,2,2157,20,0,0,4,1],[2161,2,2161,18,0,0,4,1],[2166,2,2166,17,0,0,4,1],[2209,2,2209,23,0,0,4,1],[2213,2,2213,29,0,0,4,1],[2217,2,2217,32,0,0,4,1],[2221,2,2221,17,0,0,4,1],[2264,2,2264,19,0,0,4,1],[2270,2,2270,20,0,0,4,1],[2271,2,2271,17,0,0,4,1],[2272,2,2272,19,0,0,4,1],[2273,2,2273,19,0,0,4,1],[2274,2,2274,20,0,0,4,1],[2275,2,2275,19,0,0,4,1],[2276,2,2276,18,0,0,4,1],[2277,2,2277,21,0,0,4,1],[2278,2,2278,18,0,0,4,1],[2286,2,2286,18,0,0,4,1],[2290,2,2290,22,0,0,4,1],[2294,2,2294,9,0,0,4,1],[2313,2,2313,18,0,0,4,1],[2314,2,2314,18,0,0,4,1],[2315,2,2315,18,0,0,4,1],[2316,2,2316,22,0,0,4,1],[2376,2,2376,18,0,0,4,1],[2386,2,2386,18,0,0,4,1],[2391,2,2391,18,0,0,4,1],[2399,2,2399,24,0,0,4,1],[2403,2,2403,21,0,0,4,1],[2415,2,2415,22,0,0,4,1],[2419,2,2419,9,0,0,4,1]],"set_vlan_offset":[[1141,2,1141,17,0,0,4,0],[1149,2,1149,20,0,0,4,1]],"create_ring":[[3077,6,3077,21,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_datalink_ext","pcap_is_swapped","pcap_findalldevs","pcap_create","pcap_set_tstamp_type","pcap_activate","pcap_getnonblock","pcap_setdirection","pcap_dump_fopen","pcap_dump_open_append","pcap_next","pcap_dump","pcap_freecode","pcap_dump_close","pcap_dump_close","pcap_close","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_fopen_offline","pcap_datalink_ext","pcap_is_swapped","pcap_findalldevs","pcap_create","pcap_set_tstamp_type","pcap_activate","pcap_getnonblock","pcap_setdirection","pcap_dump_fopen","pcap_dump_open_append","pcap_next","pcap_dump","pcap_freecode","pcap_dump_close","pcap_dump_close","pcap_close","pcap_close","pcap_freealldevs"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	 // Minimum pcap header size

    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Open pcap file for offline reading
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_offline = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_offline) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get datalink type
    int datalink = pcap_datalink_ext(pcap_offline);

    // 4. Check if pcap is swapped
    int swapped = pcap_is_swapped(pcap_offline);

    // 5. Find all network devices (just to use the API)
    pcap_if_t *alldevs;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Continue anyway - this is fuzzing
    }

    // 6. Create a live capture handle
    pcap_t *pcap_live = pcap_create(fuzz_str_1, errbuf); // Use loopback
    if (pcap_live) {
        // 7. Set timestamp type
        pcap_set_tstamp_type(pcap_live, fuzz_int32_t_2);
        
        // 8. Activate the live capture
        pcap_activate(pcap_live);
        
        // 9. Get non-blocking mode
        char nonblock_err[PCAP_ERRBUF_SIZE];
        pcap_getnonblock(pcap_live, nonblock_err);
        
        // 10. Set direction
        pcap_setdirection(pcap_live, PCAP_D_INOUT);
    }

    // 11. Create output file for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // 12. Open dumpers
        pcap_dumper_t *dumper1 = NULL;
        pcap_dumper_t *dumper2 = NULL;
        
        if (pcap_live) {
            dumper1 = pcap_dump_fopen(pcap_live, out_file);
        }
        
        // 13. Open second dumper in append mode
        dumper2 = pcap_dump_open_append(pcap_offline, "output_file");

        // 14. Read packets from offline pcap and dump them
        struct pcap_pkthdr header;
        const u_char *packet;
        
        while ((packet = pcap_next(pcap_offline, &header)) != NULL) {
            if (dumper1) {
                pcap_dump((u_char *)dumper1, &header, packet);
            }
        }

        // 15. Free BPF program (even though we didn't compile one)
        struct bpf_program prog;
        memset(&prog, 0, sizeof(prog));
        pcap_freecode(&prog);

        // 16. Close dumpers
        if (dumper1) pcap_dump_close(dumper1);
        if (dumper2) pcap_dump_close(dumper2);
        
        assert_file_closed(&out_file);;
    }

    // 17. Clean up
    if (pcap_live) pcap_close(pcap_live);
    pcap_close(pcap_offline);
    assert_file_closed(&in_file);;
    if (alldevs) pcap_freealldevs(alldevs);
    
    // Clean up output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}