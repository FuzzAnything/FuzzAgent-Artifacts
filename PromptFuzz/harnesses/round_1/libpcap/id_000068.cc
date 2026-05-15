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
//<ID> 464
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 2
//<Quality> {"density":25,"unique_branches":{"pcap_dump_open_append":[[1110,3,1110,26,0,0,4,0],[1111,8,1111,61,0,0,4,0]]},"library_calls":["pcap_init","pcap_fopen_offline","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_set_timeout","pcap_close","pcap_compile_nopcap","pcap_perror","pcap_close","bpf_validate","pcap_perror","pcap_freecode","pcap_close","pcap_dump_open_append","pcap_perror","pcap_freecode","pcap_close","pcap_next","bpf_filter","pcap_dump","pcap_geterr","pcap_perror","pcap_dump_close","pcap_freecode","pcap_close"],"critical_calls":["pcap_init","pcap_fopen_offline","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_set_timeout","pcap_compile_nopcap","bpf_validate","pcap_dump_open_append","pcap_next","bpf_filter","pcap_dump","pcap_geterr","pcap_perror","pcap_dump_close","pcap_freecode","pcap_close"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified libpcap APIs to achieve the event of reading packets from a pcap file, filtering them with BPF, and writing filtered packets to an output file:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < sizeof(struct pcap_file_header)) {
        return 0;
    }

    // Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(0, errbuf) != 0) {
        return 0;
    }

    // Create FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open pcap file for reading
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get data link type and convert to description
    int dlt = pcap_datalink(pcap);
    const char *dlt_desc = pcap_datalink_val_to_description_or_dlt(dlt);

    // Set timeout (even though offline capture)
    pcap_set_timeout(pcap, 1000);

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for pcap_compile_nopcap
    int fd = fuzz_fileno(in_file);

    // Compile BPF filter (simple filter that matches all packets)
    struct bpf_program bpf_prog;
    const char *filter_str = "ip or not ip";  // Always matches
    if (pcap_compile_nopcap(65535, dlt, &bpf_prog, filter_str, 1, 0) == -1) {
        pcap_perror(pcap, "BPF compilation failed");
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Validate BPF program
    if (!bpf_validate(bpf_prog.bf_insns, bpf_prog.bf_len)) {
        pcap_perror(pcap, "BPF validation failed");
        pcap_freecode(&bpf_prog);
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Open dumper for appending (append mode for multiple possible calls)
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap, "output_file");
    if (!dumper) {
        pcap_perror(pcap, "Failed to open dumper");
        pcap_freecode(&bpf_prog);
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
    }

    // Read and process packets
    struct pcap_pkthdr header;
    const u_char *packet;
    int packet_count = 0;
    
    while ((packet = pcap_next(pcap, &header)) != nullptr && packet_count < 100) {
        // Apply BPF filter
        u_int result = bpf_filter(bpf_prog.bf_insns, packet, header.len, header.caplen);
        if (result != 0) {
            // Write packet to dump file
            pcap_dump(reinterpret_cast<u_char*>(dumper), &header, packet);
        }
        packet_count++;
    }

    // Check for errors during packet reading
    if (packet == nullptr) {
        const char *error_msg = pcap_geterr(pcap);
        if (error_msg && strlen(error_msg) > 0) {
            pcap_perror(pcap, "Error reading packets");
        }
    }

    // Cleanup resources
    pcap_dump_close(dumper);
    pcap_freecode(&bpf_prog);
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}