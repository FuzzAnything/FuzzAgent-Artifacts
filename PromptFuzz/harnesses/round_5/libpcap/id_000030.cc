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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"pcap_lookupnet":[[1605,6,1605,46,0,0,4,1],[1629,6,1629,49,0,0,4,1],[1638,6,1638,17,0,0,4,1]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_set_tstamp_type","pcap_setdirection","pcap_setdirection","pcap_setdirection","pcap_datalink_ext","pcap_lookupnet","bpf_image","bpf_dump","pcap_loop","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_set_tstamp_type","pcap_setdirection","pcap_setdirection","pcap_setdirection","pcap_datalink_ext","pcap_lookupnet","bpf_image","bpf_dump","pcap_loop","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Packet handler callback for pcap_loop
static void  id_000030_packet_handler(u_char *user, const struct pcap_pkthdr *hdr, const u_char *data) {
    // Simple packet counter - just increment a counter
    int *counter = (int *)user;
    (*counter)++;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open the pcap file with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline_with_tstamp_precision(in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set timestamp type (try a few common types)
    int tstamp_types[] = {PCAP_TSTAMP_HOST, PCAP_TSTAMP_HOST_LOWPREC, PCAP_TSTAMP_HOST_HIPREC};
    for (int i = 0; i < 3; i++) {
        if (pcap_set_tstamp_type(pcap, tstamp_types[i]) == 0) {
            break;
        }
    }

    // Set direction (try both directions)
    pcap_setdirection(pcap, PCAP_D_IN);
    pcap_setdirection(pcap, PCAP_D_OUT);
    pcap_setdirection(pcap, PCAP_D_INOUT);

    // Get data link type
    int dlt = pcap_datalink_ext(pcap);
    
    // Lookup network for loopback interface (as an example)
    bpf_u_int32 netp, maskp;
    char loopback[] = "lo";
    if (pcap_lookupnet(loopback, &netp, &maskp, errbuf) == -1) {
        // This is expected to fail in many environments, continue anyway
    }

    // Create a simple BPF program for demonstration
    struct bpf_program bpf_prog;
    struct bpf_insn insns[] = {
        { 0x06, 0, 0, 0x0000ffff },  // ret #0xffff
    };
    bpf_prog.bf_len = 1;
    bpf_prog.bf_insns = insns;

    // Use bpf_image to get string representation of BPF instruction
    char *bpf_str = bpf_image(insns, 0);
    if (bpf_str) {
        // bpf_str is statically allocated, no need to free
    }

    // Dump BPF program (to stderr, but fuzzer will suppress)
    bpf_dump(&bpf_prog, 1);

    // Process packets with pcap_loop
    int packet_count = 0;
    int loop_result = pcap_loop(pcap, 10,  id_000030_packet_handler, (u_char *)&packet_count);
    
    // If pcap_loop returns -1 (error) or -2 (breakloop), that's OK for fuzzing
    // We've already processed some packets if loop_result == 0

    // Cleanup
    pcap_close(pcap);
    // Note: pcap_fopen_offline_with_tstamp_precision closes the FILE*, 
    // so we shouldn't close in_file again
    // assert_file_closed(&in_file);; // This would cause double-free
    
    assert_file_closed(&in_file);
	return 0;
}