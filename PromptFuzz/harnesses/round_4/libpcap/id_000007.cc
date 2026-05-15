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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 119, nr_unique_branch: 86
//<Quality> {"density":14,"unique_branches":{"set_vlan_offset":[[1141,2,1141,17,0,0,4,1],[1149,2,1149,20,0,0,4,0],[1157,2,1157,9,0,0,4,1]],"iface_get_id":[[4840,6,4840,41,0,0,4,1]],"pcap_protocol":[[871,6,871,19,0,0,4,0]],"pcap_activate":[[2768,6,2768,32,0,0,4,1],[2771,6,2771,17,0,0,4,0],[2776,7,2776,22,0,0,4,1]],"setup_socket":[[2452,12,2452,25,0,0,4,0],[2456,6,2456,19,0,0,4,1],[2510,6,2510,20,0,0,4,1],[2652,7,2652,24,0,0,4,1],[2667,7,2667,31,0,0,4,1],[2709,6,2709,20,0,0,4,1],[2732,6,2733,28,0,0,4,1],[2751,6,2751,21,0,0,4,0],[2752,7,2752,42,0,0,4,1],[2762,6,2762,64,0,0,4,1],[2800,6,2801,33,0,0,4,0],[2802,7,2802,48,0,0,4,0]],"iface_bind":[[4862,21,4862,32,0,0,4,0],[4865,6,4865,59,0,0,4,1],[4893,6,4893,63,0,0,4,1],[4899,6,4899,21,0,0,4,1],[4908,13,4908,20,0,0,4,1]],"set_poll_timeout":[[1071,6,1071,27,0,0,4,0],[1085,7,1085,40,0,0,4,0],[1085,44,1085,61,0,0,4,1]],"prepare_tpacket_socket":[[2973,6,2973,28,0,0,4,0],[2975,7,2975,15,0,0,4,0]],"init_tpacket":[[2902,6,2902,71,0,0,4,1],[2938,6,2939,23,0,0,4,1]],"pcap_activate_linux":[[1191,6,1191,44,0,0,4,1],[1209,6,1209,27,0,0,4,0],[1213,6,1213,29,0,0,4,1],[1226,6,1226,19,0,0,4,0],[1227,7,1227,26,0,0,4,1],[1245,6,1245,25,0,0,4,1],[1253,6,1253,13,0,0,4,1],[1262,6,1262,13,0,0,4,1],[1276,6,1276,27,0,0,4,0],[1278,7,1278,39,0,0,4,1],[1294,6,1294,13,0,0,4,1],[1306,6,1306,13,0,0,4,1],[1322,6,1323,50,0,0,4,1],[1338,10,1338,29,0,0,4,1],[1340,2,1340,17,0,0,4,1],[1344,2,1344,17,0,0,4,0]],"has_broken_tpacket_v3":[[1042,6,1042,27,0,0,4,1],[1048,6,1048,18,0,0,4,1],[1050,6,1050,28,0,0,4,1],[1054,6,1054,15,0,0,4,0]],"pcap_cleanup_linux":[[966,6,966,36,0,0,4,1],[995,6,995,22,0,0,4,0],[1003,6,1003,37,0,0,4,0],[1008,6,1008,32,0,0,4,1],[1012,6,1012,29,0,0,4,0],[1017,6,1017,38,0,0,4,0]],"pcapint_cleanup_live_common":[[4247,6,4247,23,0,0,4,0],[4251,6,4251,25,0,0,4,0],[4268,6,4268,16,0,0,4,0]],"destroy_ring":[[3466,6,3466,22,0,0,4,0]],"setup_mmapped":[[2835,6,2835,24,0,0,4,1],[2838,6,2838,43,0,0,4,1],[2844,6,2844,34,0,0,4,0],[2849,6,2849,18,0,0,4,1],[2855,6,2855,16,0,0,4,1]],"create_ring":[[3077,6,3077,21,0,0,4,0],[3085,6,3086,27,0,0,4,1],[3095,2,3095,17,0,0,4,1],[3201,2,3201,17,0,0,4,0],[3219,2,3219,9,0,0,4,1],[3231,9,3231,46,0,0,4,0],[3231,9,3231,46,0,0,4,1],[3257,6,3257,52,0,0,4,1],[3258,6,3258,61,0,0,4,1],[3358,6,3358,26,0,0,4,1],[3361,13,3361,34,0,0,4,0],[3384,6,3385,33,0,0,4,1],[3410,6,3410,24,0,0,4,1],[3413,6,3413,36,0,0,4,1],[3425,6,3425,21,0,0,4,1],[3435,12,3435,29,0,0,4,0],[3435,12,3435,29,0,0,4,1],[3437,13,3437,31,0,0,4,0],[3437,13,3437,31,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_file","pcap_datalink_ext","pcap_setnonblock","pcap_dump_fopen","pcap_compile","bpf_dump","pcap_freecode","pcap_create","pcap_activate","pcap_close","pcap_next","pcap_dump","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_file","pcap_datalink_ext","pcap_setnonblock","pcap_dump_fopen","pcap_compile","bpf_dump","pcap_freecode","pcap_create","pcap_activate","pcap_close","pcap_next","pcap_dump","pcap_dump_close","pcap_close"],"visited":1}
/*Here's a fuzz driver that uses all the specified libpcap APIs to process input data as a pcap capture file:

*/


// Include libpcap headers

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<23) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = nullptr;
    pcap_dumper_t *dumper = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    struct bpf_program filter_program;
    
    // 1. Create a FILE* from input data for reading pcap data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Open the pcap file from the input data
    pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get the underlying FILE* from the pcap_t (using pcap_file)
    FILE *pcap_file_ptr = pcap_file(pcap);
    // This is mostly for demonstration since we already have in_file
    
    // 4. Get the data link type of the pcap file
    int dlt = pcap_datalink_ext(pcap);
    
    // 5. Set the pcap handle to non-blocking mode
    int nonblock_result = pcap_setnonblock(pcap, 1, errbuf);
    
    // 6. Create an output file for dumping processed packets
    out_file = fopen("output_file", "wb");
    if (out_file) {
        // 7. Create a dumper to write packets to the output file
        dumper = pcap_dump_fopen(pcap, out_file);
    }
    
    // 8. Compile a simple BPF filter (match all packets)
    const char *filter_str = "ip";
    if (pcap_compile(pcap, &filter_program, fuzz_str_1, fuzz_int32_t_2, fuzz_uint32_t_3) == 0) {
        // 9. Dump the BPF filter program using bpf_dump
        bpf_dump(&filter_program, fuzz_int32_t_4);
        
        // Set the filter (optional, not in the required API list)
        // pcap_setfilter(pcap, &filter_program);
        
        // Free the compiled filter
        pcap_freecode(&filter_program);
    }
    
    // 10. Create and activate another pcap handle for demonstration
    // This shows usage of pcap_activate
    pcap_t *live_pcap = pcap_create(fuzz_str_5, errbuf);
    if (live_pcap) {
        // Try to activate the pcap handle
        int activate_result = pcap_activate(live_pcap);
        // We don't care about the result since this is just for fuzzing
        
        // Close the live pcap handle
        pcap_close(live_pcap);
    }
    
    // 11. Process packets from the input pcap file
    struct pcap_pkthdr header;
    const u_char *packet_data;
    int packet_count = 0;
    
    while ((packet_data = pcap_next(pcap, &header)) != nullptr && packet_count < 100) {
        packet_count++;
        
        // Write packet to dumper if available
        if (dumper) {
            pcap_dump((u_char *)dumper, &header, packet_data);
        }
    }
    
    // 12. Cleanup - release all allocated resources
    if (dumper) {
        pcap_dump_close(dumper);
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    if (pcap) {
        pcap_close(pcap);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}