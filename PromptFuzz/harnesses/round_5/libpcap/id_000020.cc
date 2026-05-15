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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["pcap_lookupdev","pcap_lookupnet","pcap_fopen_offline","pcap_setnonblock","pcap_get_tstamp_precision","pcap_datalink_name_to_val","pcap_close","pcap_dump_fopen","pcap_close","pcap_compile","bpf_image","pcap_freecode","pcap_next","pcap_dump","pcap_dump_ftell","pcap_dump_close","pcap_close"],"critical_calls":["pcap_lookupdev","pcap_lookupnet","pcap_fopen_offline","pcap_setnonblock","pcap_get_tstamp_precision","pcap_datalink_name_to_val","pcap_dump_fopen","pcap_compile","bpf_image","pcap_freecode","pcap_next","pcap_dump","pcap_dump_ftell","pcap_dump_close","pcap_close"],"visited":1}
/**/


// Include libpcap headers
extern "C" {
    #include <pcap/pcap.h>
    #include <pcap/bpf.h>
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from input data for libpcap to read
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Look up a network device
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    char *device = pcap_lookupdev(errbuf);
    
    // 2. Look up network address for the device (if device was found)
    bpf_u_int32 netp = 0, maskp = 0;
    if (device) {
        pcap_lookupnet(device, &netp, &maskp, errbuf);
    }

    // 3. Open pcap handle from input file
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Set non-blocking mode
    pcap_setnonblock(pcap_handle, 1, errbuf);

    // 5. Get timestamp precision
    int tstamp_precision = pcap_get_tstamp_precision(pcap_handle);

    // 6. Convert data link name to value (using a common DLT name)
    int dlt_val = pcap_datalink_name_to_val("EN10MB");

    // 7. Create output file for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 8. Create pcap dumper
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap_handle, out_file);
    if (!dumper) {
        assert_file_closed(&out_file);;
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 9. Create a simple BPF program
    struct bpf_program bpf_prog;
    const char *filter_str = "ip";
    bpf_u_int32 net = 0;
    
    // Try to compile filter (may fail on invalid input)
    if (pcap_compile(pcap_handle, &bpf_prog, filter_str, 0, net) == 0) {
        // Get BPF program image
        if (bpf_prog.bf_len > 0) {
            char *bpf_str = bpf_image(bpf_prog.bf_insns, bpf_prog.bf_len);
            if (bpf_str) {
                // bpf_str is statically allocated, don't free
            }
        }
        pcap_freecode(&bpf_prog);
    }

    // 10. Read and process packets
    struct pcap_pkthdr header;
    const u_char *packet;
    
    while ((packet = pcap_next(pcap_handle, &header)) != nullptr) {
        // Dump packet to output file
        pcap_dump((u_char *)dumper, &header, packet);
        
        // Get current position in dump file
        long dump_pos = pcap_dump_ftell(dumper);
        (void)dump_pos; // Use the value to avoid unused variable warning
    }

    // 11. Clean up resources
    pcap_dump_close(dumper);
    assert_file_closed(&out_file);;
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;

    // 12. Clean up any allocated device list (not allocated in this case)
    // pcap_freealldevs would be used if we called pcap_findalldevs

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}