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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_dump_open_append","pcap_perror","pcap_close","pcap_datalink","pcap_set_datalink","pcap_perror","pcap_get_selectable_fd","pcap_next","pcap_dump","pcap_geterr","pcap_geterr","pcap_perror","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_dump_open_append","pcap_datalink","pcap_set_datalink","pcap_perror","pcap_get_selectable_fd","pcap_next","pcap_dump","pcap_geterr","pcap_geterr","pcap_perror","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from input data
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

    // Open dumper in append mode
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap, "output_file");
    if (!dumper) {
        pcap_perror(pcap, "pcap_dump_open_append failed");
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Get current datalink type and attempt to set it (for API usage)
    int dlt = pcap_datalink(pcap);
    if (pcap_set_datalink(pcap, dlt) < 0) {
        pcap_perror(pcap, "pcap_set_datalink failed");
    }

    // Get selectable file descriptor (for API usage)
    int fd = pcap_get_selectable_fd(pcap);
    (void)fd; // May be -1 for offline captures

    // Read and dump packets
    struct pcap_pkthdr header;
    const uint8_t *packet;
    
    while ((packet = pcap_next(pcap, &header)) != nullptr) {
        pcap_dump((u_char *)dumper, &header, packet);
    }

    // Check for errors after reading
    if (pcap_geterr(pcap) && strlen(pcap_geterr(pcap)) > 0) {
        pcap_perror(pcap, "Error reading packets");
    }

    // Cleanup
    pcap_dump_close(dumper);
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    
    // Optional: Remove output file to avoid accumulation
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}