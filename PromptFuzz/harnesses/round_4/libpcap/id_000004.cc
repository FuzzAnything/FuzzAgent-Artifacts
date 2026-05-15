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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 9
//<Quality> {"density":14,"unique_branches":{"pcap_dump_open_append":[[1060,6,1060,29,0,0,4,0],[1061,7,1061,16,0,0,4,1],[1066,14,1066,21,0,0,4,0],[1066,25,1066,37,0,0,4,1],[1094,6,1094,18,0,0,4,1],[1169,7,1169,57,0,0,4,1]],"pcap_setdirection":[[3969,3,3969,17,0,0,4,0],[3970,3,3970,18,0,0,4,0],[3971,3,3971,20,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_major_version","pcap_minor_version","pcap_set_timeout","pcap_get_required_select_timeout","pcap_setdirection","pcap_set_rfmon","bpf_validate","pcap_dump_open_append","pcap_close","pcap_next","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_major_version","pcap_minor_version","pcap_set_timeout","pcap_get_required_select_timeout","pcap_setdirection","pcap_set_rfmon","bpf_validate","pcap_dump_open_append","pcap_next","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from input data for reading pcap data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open pcap file for reading
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get major and minor version (APIs 1 and 7)
    int major_ver = pcap_major_version(pcap);
    int minor_ver = pcap_minor_version(pcap);
    
    // Set timeout (API 2) - using a value derived from input data
    int timeout = (data[0] % 1000) + 1; // 1-1000 ms
    pcap_set_timeout(pcap, timeout);

    // Get required select timeout (API 3)
    const struct timeval *required_timeout = pcap_get_required_select_timeout(pcap);

    // Set direction (API 6) - using input to decide direction
    pcap_direction_t direction = (data[0] % 2 == 0) ? PCAP_D_IN : PCAP_D_OUT;
    pcap_setdirection(pcap, direction);

    // Set rfmon mode (API 8) - try to set based on input
    int rfmon_val = (data[0] % 3 == 0) ? 1 : 0; // Try to set rfmon 1/3 of the time
    pcap_set_rfmon(pcap, rfmon_val);

    // Create a simple BPF program for validation (API 5)
    struct bpf_insn bpf_prog[] = {
        { 0x06, 0, 0, 0x00000000 }, // ret #0
        { 0x06, 0, 0, 0x0000FFFF }  // ret #65535
    };
    int bpf_len = sizeof(bpf_prog) / sizeof(bpf_prog[0]);
    int bpf_valid = bpf_validate(bpf_prog, bpf_len);

    // Open a pcap dumper for appending (API 4)
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap, "output_file");
    if (!dumper) {
        // Clean up and return if dumper can't be opened
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Read packets from the input pcap file and write to dumper
    struct pcap_pkthdr header;
    const u_char *packet;
    
    while ((packet = pcap_next(pcap, &header)) != NULL) {
        // Write packet to dumper
        pcap_dump((u_char *)dumper, &header, packet);
    }

    // Flush and close the dumper
    pcap_dump_flush(dumper);
    pcap_dump_close(dumper);

    // Clean up
    pcap_close(pcap);
    assert_file_closed(&in_file);;

    // Remove the output file if it was created
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}