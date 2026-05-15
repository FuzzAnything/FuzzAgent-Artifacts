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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 5
//<Quality> {"density":26,"unique_branches":{"iface_get_ts_types":[[5161,6,5161,40,0,0,4,0],[5167,3,5167,18,0,0,4,1],[5168,3,5168,14,0,0,4,1],[5178,3,5178,14,0,0,4,0],[5188,3,5188,10,0,0,4,1]]},"library_calls":["pcap_init","pcap_lib_version","pcap_findalldevs","pcap_fopen_offline","pcap_freealldevs","pcap_create","pcap_close","pcap_freealldevs","pcap_set_immediate_mode","pcap_getnonblock","pcap_close","pcap_close","pcap_freealldevs","pcap_dump_fopen","pcap_close","pcap_close","pcap_freealldevs","pcap_compile","bpf_dump","pcap_freecode","pcap_next_ex","pcap_dump","pcap_dump_ftell64","pcap_dump_close","pcap_close","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_init","pcap_lib_version","pcap_findalldevs","pcap_fopen_offline","pcap_create","pcap_set_immediate_mode","pcap_getnonblock","pcap_dump_fopen","pcap_compile","bpf_dump","pcap_freecode","pcap_next_ex","pcap_dump","pcap_dump_ftell64","pcap_dump_close","pcap_close","pcap_close","pcap_freealldevs"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Initialize pcap
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        return 0;
    }

    // 2. Get library version
    const char *version = pcap_lib_version();

    // 3. Find all network devices (just to use the API)
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Continue anyway since this might fail in fuzzing context
    }

    // 4. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 5. Open pcap capture from memory file
    pcap_t *pcap_reader = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_reader) {
        assert_file_closed(&in_file);;
        pcap_freealldevs(alldevs);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Create a pcap writer (just to use the API)
    pcap_t *pcap_writer = pcap_create(fuzz_str_1, errbuf);
    if (!pcap_writer) {
        pcap_close(pcap_reader);
        assert_file_closed(&in_file);;
        pcap_freealldevs(alldevs);
        assert_file_closed(&in_file);
	return 0;
    }

    // 7. Set immediate mode
    pcap_set_immediate_mode(pcap_writer, fuzz_int32_t_2);

    // 8. Get non-blocking mode
    char nonblock_err[PCAP_ERRBUF_SIZE];
    pcap_getnonblock(pcap_reader, nonblock_err);

    // 9. Create output file for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_writer);
        pcap_close(pcap_reader);
        assert_file_closed(&in_file);;
        pcap_freealldevs(alldevs);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 10. Open dumper
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap_reader, out_file);
    if (!dumper) {
        assert_file_closed(&out_file);;
        pcap_close(pcap_writer);
        pcap_close(pcap_reader);
        assert_file_closed(&in_file);;
        pcap_freealldevs(alldevs);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 11. Compile BPF filter (using a simple filter that matches everything)
    struct bpf_program fp;
    const char *filter_str = size > 0 && (data[0] & 0x01) ? "ip" : ""; // Simple condition
    bpf_u_int32 netmask = 0xffffff00; // Example netmask
    
    if (pcap_compile(pcap_reader, &fp, filter_str, fuzz_int32_t_3, fuzz_uint32_t_4) == -1) {
        // Continue even if filter compilation fails
    } else {
        // 12. Dump BPF program (optional)
        bpf_dump(&fp, fuzz_int32_t_5);
        pcap_freecode(&fp);
    }

    // 13. Read and process packets
    struct pcap_pkthdr *header;
    const u_char *packet_data;
    int packet_count = 0;
    const int max_packets = 100; // Limit to prevent infinite loops

    while (packet_count < max_packets) {
        int result = pcap_next_ex(pcap_reader, &header, &packet_data);
        
        if (result == 1) {
            // Packet captured successfully
            pcap_dump((u_char *)dumper, header, packet_data);
            packet_count++;
        } else if (result == 0) {
            // Timeout (not applicable for offline capture)
            break;
        } else if (result == -1) {
            // Error occurred
            break;
        } else if (result == -2) {
            // No more packets in savefile
            break;
        }
    }

    // 14. Get dump file position
    int64_t dump_pos = pcap_dump_ftell64(dumper);

    // 15. Clean up resources in reverse order
    pcap_dump_close(dumper);
    assert_file_closed(&out_file);;
    pcap_close(pcap_writer);
    pcap_close(pcap_reader);
    assert_file_closed(&in_file);;
    pcap_freealldevs(alldevs);

    // Remove output file to avoid accumulation
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}