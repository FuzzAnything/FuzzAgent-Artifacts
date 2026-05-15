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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_major_version","pcap_get_tstamp_precision","pcap_set_tstamp_precision","pcap_set_tstamp_precision","pcap_list_tstamp_types","pcap_tstamp_type_val_to_description","pcap_free_tstamp_types","pcap_datalink","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_compile_nopcap","pcap_setfilter","pcap_freecode","pcap_next_ex","pcap_close","pcap_fopen_offline","pcap_loop","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_major_version","pcap_get_tstamp_precision","pcap_set_tstamp_precision","pcap_list_tstamp_types","pcap_tstamp_type_val_to_description","pcap_free_tstamp_types","pcap_datalink","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_compile_nopcap","pcap_setfilter","pcap_freecode","pcap_next_ex","pcap_close","pcap_fopen_offline","pcap_loop","pcap_close"],"visited":0}
/**/


// Include necessary libpcap headers
extern "C" {
    #include <pcap/pcap.h>
}

// Callback handler for pcap_loop
static void  id_000034_packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Simple handler that does nothing - just to satisfy pcap_loop requirement
    (void)user;
    (void)h;
    (void)bytes;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Open the pcap file for reading
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get major version (API requirement)
    int major_version = pcap_major_version(pcap);
    (void)major_version; // Use to avoid unused variable warning

    // 2. Try to set timestamp precision (API requirement)
    // We'll try both microsecond and nanosecond precision
    int tstamp_precision = pcap_get_tstamp_precision(pcap);
    if (tstamp_precision == PCAP_TSTAMP_PRECISION_MICRO) {
        pcap_set_tstamp_precision(pcap, PCAP_TSTAMP_PRECISION_NANO);
    } else {
        pcap_set_tstamp_precision(pcap, PCAP_TSTAMP_PRECISION_MICRO);
    }

    // 3. Get timestamp type description (API requirement)
    // First get available timestamp types
    int *tstamp_types;
    int num_types = pcap_list_tstamp_types(pcap, &tstamp_types);
    if (num_types > 0) {
        for (int i = 0; i < num_types; i++) {
            const char *desc = pcap_tstamp_type_val_to_description(tstamp_types[i]);
            (void)desc; // Use to avoid unused variable warning
        }
        pcap_free_tstamp_types(tstamp_types);
    }

    // 4. Get datalink type (API requirement)
    int dlt = pcap_datalink(pcap);
    
    // 5. Try to set datalink type (API requirement)
    // Get available datalinks first
    int *datalinks;
    int num_datalinks = pcap_list_datalinks(pcap, &datalinks);
    if (num_datalinks > 0) {
        // Try to set to the first available datalink type if it's different
        if (num_datalinks > 1 && datalinks[0] != dlt) {
            pcap_set_datalink(pcap, datalinks[0]);
        }
        pcap_free_datalinks(datalinks);
    }

    // 6. Compile a BPF filter using pcap_compile_nopcap (API requirement)
    // We need to extract some bytes from the input to use as filter string
    std::string filter_str;
    if (size > 100) {
        // Use first 100 bytes as potential filter string
        filter_str.assign(reinterpret_cast<const char*>(data), 
                         std::min(size, (size_t)100));
        // Ensure it's null-terminated
        filter_str[99] = '\0';
    } else {
        filter_str = "ip";
    }

    struct bpf_program fp;
    // Use the datalink type we got earlier and a reasonable snaplen
    if (pcap_compile_nopcap(65535, dlt, &fp, filter_str.c_str(), 1, 0) == 0) {
        // Apply the filter if compilation succeeded
        pcap_setfilter(pcap, &fp);
        pcap_freecode(&fp);
    }

    // 7. Use pcap_next_ex to read packets (API requirement)
    struct pcap_pkthdr *header;
    const u_char *packet_data;
    int packet_count = 0;
    
    while (packet_count < 10) { // Limit to 10 packets to avoid infinite loops
        int result = pcap_next_ex(pcap, &header, &packet_data);
        if (result <= 0) {
            break;
        }
        packet_count++;
    }

    // 8. Use pcap_loop to process remaining packets (API requirement)
    // Reset file position by reopening
    fseek(in_file, 0, SEEK_SET);
    pcap_close(pcap);
    pcap = pcap_fopen_offline(in_file, errbuf);
    
    if (pcap) {
        // Process up to 5 packets with pcap_loop
        pcap_loop(pcap, 5,  id_000034_packet_handler, nullptr);
    }

    // Cleanup
    if (pcap) {
        pcap_close(pcap);
    }
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}