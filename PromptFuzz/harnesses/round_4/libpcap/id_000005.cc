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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"pcap_setdirection":[[3971,3,3971,20,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_major_version","pcap_minor_version","pcap_set_timeout","pcap_get_required_select_timeout","bpf_validate","pcap_setdirection","pcap_set_rfmon","pcap_dump_open_append","pcap_next","pcap_dump","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_major_version","pcap_minor_version","pcap_set_timeout","pcap_get_required_select_timeout","bpf_validate","pcap_setdirection","pcap_set_rfmon","pcap_dump_open_append","pcap_next","pcap_dump","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // 1. Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Open pcap handle for reading the input data
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get major/minor version (APIs 1 & 6)
    int major = pcap_major_version(pcap);
    int minor = pcap_minor_version(pcap);
    (void)major; (void)minor; // Prevent unused variable warnings

    // 4. Set timeout (API 2)
    pcap_set_timeout(pcap, fuzz_int32_t_1); // 1000ms timeout

    // 5. Get required select timeout (API 3)
    const struct timeval *select_timeout = pcap_get_required_select_timeout(pcap);
    (void)select_timeout; // Prevent unused variable warning

    // 6. Create BPF filter from data and validate it (API 5)
    // Use first portion of data as BPF instructions (if enough data available)
    struct bpf_insn bpf_instructions[10];
    size_t bpf_data_size = sizeof(bpf_instructions);
    if (size >= bpf_data_size) {
        memcpy(bpf_instructions, data, bpf_data_size);
        // Set reasonable instruction codes to avoid validation errors
        for (int i = 0; i < 10; i++) {
            bpf_instructions[i].code = 0x06; // RET instruction
            bpf_instructions[i].k = 0;
        }
        bpf_validate(bpf_instructions, 10);
    }

    // 7. Set direction (API 6)
    pcap_setdirection(pcap, PCAP_D_INOUT);

    // 8. Set monitor mode (API 8)
    pcap_set_rfmon(pcap, fuzz_int32_t_2); // Disable monitor mode

    // 9. Open output dumper in append mode (API 4)
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap, "output_file");
    if (dumper) {
        // Read and process packets
        struct pcap_pkthdr header;
        const u_char *packet;
        
        while ((packet = pcap_next(pcap, &header)) != NULL) {
            // Dump packet to output file
            pcap_dump((u_char *)dumper, &header, packet);
        }
        
        // Close dumper
        pcap_dump_close(dumper);
    }

    // 10. Cleanup
    pcap_close(pcap);
    // Note: pcap_fopen_offline closes the FILE* automatically
    // when pcap_close is called, so we don't need assert_file_closed(&in_file);
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}