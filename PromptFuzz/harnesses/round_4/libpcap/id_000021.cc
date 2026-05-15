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
//<ID> 444
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_set_promisc","pcap_bufsize","pcap_set_buffer_size","pcap_minor_version","pcap_set_tstamp_type","pcap_loop","pcap_close"],"critical_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_set_promisc","pcap_bufsize","pcap_set_buffer_size","pcap_minor_version","pcap_set_tstamp_type","pcap_loop","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Packet handler callback function
static void  id_000021_packet_handler(u_char *user, const struct pcap_pkthdr *hdr, const u_char *data) {
    // Simple packet counter - just increment a counter
    int *counter = (int *)user;
    (*counter)++;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        return 0;
    }

    // Look up a network device
    char *device = pcap_lookupdev(errbuf);
    if (!device) {
        return 0;
    }

    // Create a FILE* from input data for reading pcap data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open pcap file for offline reading
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Set promiscuous mode (though for offline capture this doesn't matter)
    if (pcap_set_promisc(pcap_handle, fuzz_int32_t_1) != 0) {
        // Continue even if this fails for offline capture
    }

    // Get current buffer size
    int current_bufsize = pcap_bufsize(pcap_handle);
    
    // Set buffer size (try to set it to a reasonable value)
    // Use a value based on the input size, but cap it
    int new_bufsize = (size < 65536) ? 65536 : (size > 1048576) ? 1048576 : size;
    if (pcap_set_buffer_size(pcap_handle, new_bufsize) != 0) {
        // Continue even if this fails
    }

    // Get minor version
    int minor_ver = pcap_minor_version(pcap_handle);

    // Set timestamp type (try to set to PCAP_TSTAMP_HOST)
    if (pcap_set_tstamp_type(pcap_handle, fuzz_int32_t_2) != 0) {
        // Continue even if this fails
    }

    // Process packets from the pcap file
    int packet_count = 0;
    int loop_result = pcap_loop(pcap_handle, fuzz_int32_t_3,  id_000021_packet_handler, (u_char *)&packet_count);
    
    // The loop will process until EOF or error
    // Negative return values indicate errors

    // Clean up resources
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;

    // Also try to use the device name we looked up earlier
    // We can't actually use it for capture in this fuzzer context,
    // but we can at least verify it's a valid string
    if (strlen(device) > 0) {
        // Device name is valid
    }

    assert_file_closed(&in_file);
	return 0;
}