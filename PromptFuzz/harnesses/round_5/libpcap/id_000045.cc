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
//<ID> 241
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"pcap_set_datalink":[[3071,6,3071,13,0,0,4,0]]},"library_calls":["pcap_init","pcap_findalldevs","pcap_freealldevs","pcap_fopen_offline","pcap_datalink_ext","pcap_set_datalink","pcap_setnonblock","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_close","pcap_compile_nopcap","pcap_freecode","pcap_next","pcap_close"],"critical_calls":["pcap_init","pcap_findalldevs","pcap_freealldevs","pcap_fopen_offline","pcap_datalink_ext","pcap_set_datalink","pcap_setnonblock","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_compile_nopcap","pcap_freecode","pcap_next","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_UTF_8, errbuf) != 0) {
        // Continue anyway - some errors might be acceptable
    }

    // 2. Find all network devices
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) != -1) {
        // Process the device list if found
        pcap_freealldevs(alldevs);
    }

    // 3. Create a FILE* from input data for reading libpcap output
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Open the pcap file for offline reading
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Get and set data link type
    int dlt = pcap_datalink_ext(pcap_handle);
    // Try to set the same data link type (might fail for offline files, but try anyway)
    pcap_set_datalink(pcap_handle, dlt);

    // 6. Set non-blocking mode
    pcap_setnonblock(pcap_handle, 1, errbuf);

    // 7. List timestamp types
    int *tstamp_types = nullptr;
    int tstamp_type_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    if (tstamp_type_count > 0 && tstamp_types) {
        pcap_free_tstamp_types(tstamp_types);
    }

    // 8. Create output file for BPF compilation results
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 9. Compile a BPF filter using pcap_compile_nopcap
    // Get file descriptor for output
    int out_fd = fuzz_fileno(out_file);
    if (out_fd != -1) {
        struct bpf_program bpf_prog;
        const char *filter_str = "ip";  // Simple filter expression
        
        // Use reasonable default values for snaplen and linktype
        int snaplen = 65535;
        bpf_u_int32 netmask = 0xffffff00;  // 255.255.255.0
        
        // Try to compile the BPF filter
        if (pcap_compile_nopcap(snaplen, dlt, &bpf_prog, filter_str, 1, netmask) == 0) {
            // Write some information about the compiled program
            fwrite(&bpf_prog.bf_len, sizeof(bpf_prog.bf_len), 1, out_file);
            if (bpf_prog.bf_insns && bpf_prog.bf_len > 0) {
                fwrite(bpf_prog.bf_insns, sizeof(struct bpf_insn), bpf_prog.bf_len, out_file);
            }
            pcap_freecode(&bpf_prog);
        }
    }

    // 10. Process packets from the input file
    struct pcap_pkthdr header;
    const u_char *packet_data;
    
    while ((packet_data = pcap_next(pcap_handle, &header)) != nullptr) {
        // Just consume the packets - fuzzing doesn't need to process them
        // The act of reading them exercises the libpcap parsing code
    }

    // 11. Clean up
    assert_file_closed(&out_file);;
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}