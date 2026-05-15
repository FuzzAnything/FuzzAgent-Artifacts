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
//<ID> 445
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"pcap_loop":[[2982,7,2982,33,0,0,4,1]]},"library_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_set_promisc","pcap_bufsize","pcap_set_buffer_size","pcap_minor_version","pcap_set_tstamp_type","pcap_loop","pcap_close"],"critical_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline","pcap_set_promisc","pcap_bufsize","pcap_set_buffer_size","pcap_minor_version","pcap_set_tstamp_type","pcap_loop","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Custom callback handler for pcap_loop
static void  id_000022_packet_handler(u_char *user, const struct pcap_pkthdr *hdr, const u_char *data) {
    // Simple handler that does nothing - just to satisfy pcap_loop requirements
    (void)user;
    (void)hdr;
    (void)data;
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        return 0;
    }

    // Try to lookup a network device
    char *device = pcap_lookupdev(errbuf);
    (void)device; // May be NULL in fuzzing context

    // Create FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open pcap capture from the memory file
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Try to set promiscuous mode (may fail for offline capture)
    pcap_set_promisc(pcap, fuzz_int32_t_1);

    // Get current buffer size
    int buf_size = pcap_bufsize(pcap);
    
    // Try to set a new buffer size (if supported)
    if (buf_size > 0) {
        pcap_set_buffer_size(pcap, buf_size * 2);
    }

    // Get minor version
    int minor_ver = pcap_minor_version(pcap);
    (void)minor_ver;

    // Try to set timestamp type
    pcap_set_tstamp_type(pcap, fuzz_int32_t_2);

    // Process packets from the capture file
    pcap_loop(pcap, fuzz_int32_t_3,  id_000022_packet_handler, nullptr);

    // Cleanup
    pcap_close(pcap);
    // Note: pcap_fopen_offline closes the FILE* automatically
    // so we don't need to assert_file_closed(&in_file); here

    assert_file_closed(&in_file);
	return 0;
}