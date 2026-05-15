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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 6.5, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_fileno","pcap_set_protocol_linux","pcap_dump_open","pcap_close","pcap_dump_file","pcap_4_byte_aligned_uint64_val","pcap_next","pcap_dump","pcap_dump_flush","pcap_statustostr","pcap_dump_close","pcap_close"],"critical_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_fileno","pcap_set_protocol_linux","pcap_dump_open","pcap_dump_file","pcap_4_byte_aligned_uint64_val","pcap_next","pcap_dump","pcap_dump_flush","pcap_statustostr","pcap_dump_close","pcap_close"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int input_fd = fuzz_fileno(in_file);
    int output_fd = fuzz_fileno(out_file);
    (void)input_fd; // Mark as used
    (void)output_fd; // Mark as used

    // Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
    }

    // Look up default network device
    char *device = pcap_lookupdev(errbuf);
    (void)device; // Mark as used

    // Open pcap handle from input file
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
    }

    // Get file descriptor for pcap handle
    int pcap_fd = pcap_fileno(pcap);
    (void)pcap_fd; // Mark as used

    // Set protocol for Linux (try with value from first byte of data)
    int protocol = data[0] % 256;
    int status = pcap_set_protocol_linux(pcap, protocol);
    (void)status; // Mark as used

    // Open dumper for output
    pcap_dumper_t *dumper = pcap_dump_open(pcap, "output_file");
    if (!dumper) {
        pcap_close(pcap);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
    }

    // Get FILE* from dumper
    FILE *dump_file = pcap_dump_file(dumper);
    (void)dump_file; // Mark as used

    // Create aligned 64-bit value from input data
    pcap_4_byte_aligned_uint64 aligned_val;
    size_t copy_size = (size < sizeof(aligned_val.halves)) ? size : sizeof(aligned_val.halves);
    memset(&aligned_val, 0, sizeof(aligned_val));
    memcpy(&aligned_val.halves, data, copy_size);
    
    // Convert aligned value to regular uint64
    uint64_t converted_val = pcap_4_byte_aligned_uint64_val(aligned_val);
    (void)converted_val; // Mark as used

    // Read and process packets from input
    struct pcap_pkthdr header;
    const u_char *packet;
    
    while ((packet = pcap_next(pcap, &header)) != nullptr) {
        // Dump packet to output
        pcap_dump((u_char *)dumper, &header, packet);
    }

    // Flush dumper
    pcap_dump_flush(dumper);

    // Get status string for the status from pcap_set_protocol_linux
    const char *status_str = pcap_statustostr(0); // Using 0 as generic status
    (void)status_str; // Mark as used

    // Cleanup
    pcap_dump_close(dumper);
    pcap_close(pcap);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
}