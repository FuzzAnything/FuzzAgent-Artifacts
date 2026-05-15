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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline_with_tstamp_precision","pcap_fopen_offline","pcap_set_buffer_size","pcap_set_tstamp_type","pcap_activate","pcap_datalink","pcap_compile","bpf_dump","pcap_freecode","pcap_next","pcap_close"],"critical_calls":["pcap_init","pcap_lookupdev","pcap_fopen_offline_with_tstamp_precision","pcap_fopen_offline","pcap_set_buffer_size","pcap_set_tstamp_type","pcap_activate","pcap_datalink","pcap_compile","bpf_dump","pcap_freecode","pcap_next","pcap_close"],"visited":0}
/**/


// Include necessary libpcap headers

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize variables
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = nullptr;
    struct bpf_program bpf_filter;
    const char *filter_expr = nullptr;
    int dlt = 0;
    
    // 1. Initialize libpcap
    if (pcap_init(0, errbuf) != 0) {
        // pcap_init failed, but continue with other operations
        // Initialize error buffer to empty string for subsequent calls
        errbuf[0] = '\0';
    }
    
    // 2. Try to find a network device (using pcap_lookupdev)
    char *device = pcap_lookupdev(errbuf);
    // Note: device may be null if no device found, but we continue
    
    // 3. Create a pcap handle from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        // Failed to create memory file, return early
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Open pcap file from memory with timestamp precision
    pcap_handle = pcap_fopen_offline_with_tstamp_precision(in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    if (pcap_handle == nullptr) {
        // Try without timestamp precision
        rewind(in_file);
        pcap_handle = pcap_fopen_offline(in_file, errbuf);
        if (pcap_handle == nullptr) {
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0; // Cannot proceed without valid pcap handle
        }
    }
    
    // 5. Set buffer size (if size is at least sizeof(int))
    if (size >= sizeof(int)) {
        int buffer_size;
        memcpy(&buffer_size, data, sizeof(int));
        // Ensure buffer_size is reasonable
        if (buffer_size > 0 && buffer_size < 1000000) {
            pcap_set_buffer_size(pcap_handle, buffer_size);
        }
    }
    
    // 6. Set timestamp type (if size is at least sizeof(int) * 2)
    if (size >= sizeof(int) * 2) {
        int tstamp_type;
        memcpy(&tstamp_type, data + sizeof(int), sizeof(int));
        // Try to set timestamp type (may fail for invalid types)
        pcap_set_tstamp_type(pcap_handle, tstamp_type);
    }
    
    // 7. Activate the pcap handle
    if (pcap_activate(pcap_handle) != 0) {
        // Activation failed for offline handle, but continue
    }
    
    // 8. Get data link type
    dlt = pcap_datalink(pcap_handle);
    
    // 9. Compile a BPF filter using input data as filter expression
    // Use a portion of input data as filter string if possible
    if (size > 0) {
        // Allocate buffer for filter expression (null-terminated)
        size_t filter_len = size < 256 ? size : 256;
        char *filter_buf = (char *)malloc(filter_len + 1);
        if (filter_buf != nullptr) {
            memcpy(filter_buf, data, filter_len);
            filter_buf[filter_len] = '\0';
            filter_expr = filter_buf;
            
            // Try to compile the filter
            bpf_u_int32 netmask = 0;
            if (pcap_compile(pcap_handle, &bpf_filter, filter_expr, 1, netmask) == 0) {
                // Filter compiled successfully
                
                // 10. Dump the BPF program (if compiled successfully)
                bpf_dump(&bpf_filter, 1); // Verbose output
                
                // Free the compiled filter
                pcap_freecode(&bpf_filter);
            }
            
            free(filter_buf);
        }
    }
    
    // 11. Try to process some packets from the input
    struct pcap_pkthdr header;
    const u_char *packet_data;
    int packet_count = 0;
    
    // Read up to 10 packets from the input
    while (packet_count < 10) {
        packet_data = pcap_next(pcap_handle, &header);
        if (packet_data == nullptr) {
            break; // No more packets
        }
        packet_count++;
    }
    
    // 12. Clean up resources
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
    } else {
        assert_file_closed(&in_file);; // Close file if pcap_fopen_offline failed
    }
    
    assert_file_closed(&in_file);
	return 0;
}