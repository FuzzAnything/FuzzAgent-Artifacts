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
//<ID> 446
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 3
//<Quality> {"density":10,"unique_branches":{"pcap_loop":[[2982,7,2982,33,0,0,4,0],[2984,8,2984,16,0,0,4,0],[2984,8,2984,16,0,0,4,1]]},"library_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_set_promisc","pcap_bufsize","pcap_set_buffer_size","pcap_minor_version","pcap_set_tstamp_type","pcap_loop","pcap_close"],"critical_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_set_promisc","pcap_bufsize","pcap_set_buffer_size","pcap_minor_version","pcap_set_tstamp_type","pcap_loop","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Simple packet handler callback for pcap_loop
static void  id_000023_packet_handler(u_char *user_data, const struct pcap_pkthdr *header, const u_char *packet) {
    // Do nothing with the packet in fuzzing context
    (void)user_data;
    (void)header;
    (void)packet;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // 1. Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        return 0;
    }

    // 2. Look up a default device (may return NULL if no device found)
    char *device = pcap_lookupdev(errbuf);
    (void)device; // May be NULL in fuzzing environment

    // 3. Create a pcap handle from the input data (simulating reading from file)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Try to set promiscuous mode (may fail for offline capture)
    pcap_set_promisc(pcap_handle, fuzz_int32_t_1); // Set to non-promiscuous

    // 5. Get and set buffer size
    int current_bufsize = pcap_bufsize(pcap_handle);
    (void)current_bufsize; // May not be meaningful for offline capture
    
    // Try to set buffer size (may fail for offline capture)
    pcap_set_buffer_size(pcap_handle, 1024 * 1024); // 1MB buffer

    // 6. Get minor version
    int minor_ver = pcap_minor_version(pcap_handle);
    (void)minor_ver;

    // 7. Try to set timestamp type (may fail for offline capture)
    pcap_set_tstamp_type(pcap_handle, fuzz_int32_t_2);

    // 8. Process packets with pcap_loop
    // Process up to 100 packets or until end of file
    pcap_loop(pcap_handle, fuzz_int32_t_3,  id_000023_packet_handler, nullptr);

    // Cleanup
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}