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
//<ID> 422
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 6
//<Quality> {"density":16,"unique_branches":{"gen_load_prism_llprefixlen":[[3451,6,3451,35,0,0,4,1]],"gen_proto":[[6867,3,6867,16,0,0,4,0],[6877,3,6877,10,0,0,4,1],[6894,3,6894,16,0,0,4,0]],"pcap_loop":[[2966,7,2966,23,0,0,4,1],[2978,13,2978,19,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_create","pcap_lookupnet","pcap_set_buffer_size","pcap_setdirection","pcap_statustostr","pcap_compile","pcap_compile","bpf_dump","pcap_datalink_ext","pcap_set_datalink","pcap_set_datalink","pcap_set_tstamp_precision","pcap_set_tstamp_precision","pcap_loop","pcap_freecode","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_create","pcap_lookupnet","pcap_set_buffer_size","pcap_setdirection","pcap_statustostr","pcap_compile","pcap_compile","bpf_dump","pcap_datalink_ext","pcap_set_datalink","pcap_set_datalink","pcap_set_tstamp_precision","pcap_set_tstamp_precision","pcap_loop","pcap_freecode","pcap_close"],"visited":0}
/**/


// Include necessary headers for libpcap

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0; // Need some data to work with
    }

    // Create a FILE* from input data for offline reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open a file for writing output (though we won't actually write much)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    struct bpf_program fp;
    bpf_u_int32 netp = 0, maskp = 0;
    const char *status_str = nullptr;
    int ret = 0;

    // 1. Create a pcap handle for offline reading
    pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        // Fallback: try to create a live capture handle if offline fails
        pcap_handle = pcap_create("any", errbuf);
        if (!pcap_handle) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
        }
    }

    // 2. Lookup network for a dummy interface (using "lo" as example)
    if (pcap_lookupnet("lo", &netp, &maskp, errbuf) == -1) {
        netp = 0;
        maskp = 0;
    }

    // 3. Set buffer size
    if (pcap_set_buffer_size(pcap_handle, 1024) != 0) {
        // Continue anyway
    }

    // 4. Set direction (only relevant for live capture)
    if (pcap_setdirection(pcap_handle, PCAP_D_IN) != 0) {
        // Continue anyway
    }

    // 5. Convert status to string (using a sample status)
    status_str = pcap_statustostr(0);
    (void)status_str; // Prevent unused warning

    // 6. Compile a BPF filter using data from input
    // Use first few bytes as filter string (safe copy)
    char filter_str[256];
    size_t filter_len = size < 255 ? size : 255;
    memcpy(filter_str, data, filter_len);
    filter_str[filter_len] = '\0';
    
    // Ensure filter string is printable ASCII
    for (size_t i = 0; i < filter_len; i++) {
        if (filter_str[i] < 32 || filter_str[i] > 126) {
            filter_str[i] = ' ';
        }
    }

    if (pcap_compile(pcap_handle, &fp, filter_str, 0, netp) == -1) {
        // Filter compilation failed, use a default
        pcap_compile(pcap_handle, &fp, "ip", 0, netp);
    }

    // 7. Dump the compiled BPF program
    bpf_dump(&fp, 1);

    // 8. Set datalink type (try to set to current type)
    int dlt = pcap_datalink_ext(pcap_handle);
    if (pcap_set_datalink(pcap_handle, dlt) != 0) {
        // Try a common type
        pcap_set_datalink(pcap_handle, DLT_EN10MB);
    }

    // 9. Set timestamp precision
    if (pcap_set_tstamp_precision(pcap_handle, PCAP_TSTAMP_PRECISION_MICRO) != 0) {
        // Try nanosecond precision
        pcap_set_tstamp_precision(pcap_handle, PCAP_TSTAMP_PRECISION_NANO);
    }

    // 10. Process packets with pcap_loop
    // Use a simple packet handler that does nothing
    struct PacketCounter {
        int count;
    } counter = {0};
    
    pcap_handler dummy_handler = [](u_char *user, const struct pcap_pkthdr *h, const u_char *data) {
        PacketCounter *pc = (PacketCounter *)user;
        pc->count++;
    };
    
    // Process at most 10 packets to avoid infinite loops
    ret = pcap_loop(pcap_handle, 10, dummy_handler, (u_char *)&counter);
    
    // 11. Free the compiled filter
    pcap_freecode(&fp);

    // 12. Close pcap handle
    pcap_close(pcap_handle);

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file if created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}