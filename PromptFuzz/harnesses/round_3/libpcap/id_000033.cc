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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_bufsize","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_set_tstamp_type","pcap_set_tstamp_precision","pcap_compile","bpf_dump","pcap_freecode","pcap_next","pcap_findalldevs","pcap_create","pcap_set_snaplen","pcap_set_promisc","pcap_set_timeout","pcap_activate","pcap_inject","pcap_close","pcap_freealldevs","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_bufsize","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_set_tstamp_type","pcap_set_tstamp_precision","pcap_compile","bpf_dump","pcap_freecode","pcap_next","pcap_findalldevs","pcap_create","pcap_set_snaplen","pcap_set_promisc","pcap_set_timeout","pcap_activate","pcap_inject","pcap_close","pcap_freealldevs","pcap_close"],"visited":1}
/**/



extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < sizeof(struct pcap_file_header)) {
        return 0;
    }

    // Step 1: Create a FILE* from input data for reading pcap data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Open pcap file with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = pcap_fopen_offline_with_tstamp_precision(
        in_file, fuzz_uint32_t_1, errbuf);
    
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Get snapshot length
    int snapshot_len = pcap_snapshot(pcap_handle);

    // Step 4: Get buffer size
    int buffer_size = pcap_bufsize(pcap_handle);

    // Step 5: List available datalinks
    int *datalinks = nullptr;
    int datalink_count = pcap_list_datalinks(pcap_handle, &datalinks);
    if (datalinks && datalink_count > 0) {
        // Try to set the first available datalink type
        // This is just for API coverage, may fail
        pcap_set_datalink(pcap_handle, datalinks[0]);
        pcap_free_datalinks(datalinks);
    }

    // Step 6: Set timestamp type (try NTP first, fallback to host)
    int tstamp_type = PCAP_TSTAMP_HOST;
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    if (tstamp_types && tstamp_count > 0) {
        tstamp_type = tstamp_types[0];
        pcap_free_tstamp_types(tstamp_types);
    }
    pcap_set_tstamp_type(pcap_handle, fuzz_int32_t_2);

    // Step 7: Set timestamp precision
    pcap_set_tstamp_precision(pcap_handle, fuzz_int32_t_3);

    // Step 8: Create a simple BPF program for demonstration
    struct bpf_program bpf_filter;
    const char *filter_str = "ip or arp";
    bpf_u_int32 netmask = 0xffffff00; // 255.255.255.0
    
    // Compile BPF filter (may fail if no network specified)
    if (pcap_compile(pcap_handle, &bpf_filter, fuzz_str_4, fuzz_int32_t_5, fuzz_uint32_t_6) == 0) {
        // Dump BPF program
        bpf_dump(&bpf_filter, fuzz_int32_t_7);
        pcap_freecode(&bpf_filter);
    }

    // Step 9: Read and process packets from the pcap file
    struct pcap_pkthdr header;
    const u_char *packet;
    std::vector<u_char> packet_buffer;
    
    while ((packet = pcap_next(pcap_handle, &header)) != nullptr) {
        // Store packet data for potential injection
        if (header.caplen > 0 && header.caplen <= size) {
            packet_buffer.assign(packet, packet + header.caplen);
        }
    }

    // Step 10: Find all network devices (for pcap_inject preparation)
    pcap_if_t *all_devices = nullptr;
    if (pcap_findalldevs(&all_devices, errbuf) == 0 && all_devices) {
        // Step 11: Try to open first device for packet injection
        pcap_t *inject_handle = pcap_create(all_devices->name, errbuf);
        if (inject_handle) {
            // Configure inject handle
            pcap_set_snaplen(inject_handle, snapshot_len);
            pcap_set_promisc(inject_handle, fuzz_int32_t_8);
            pcap_set_timeout(inject_handle, fuzz_int32_t_9);
            
            // Activate the handle (may fail without permissions)
            if (pcap_activate(inject_handle) == 0) {
                // Try to inject a packet if we captured one
                if (!packet_buffer.empty()) {
                    pcap_inject(inject_handle, 
                               packet_buffer.data(), 
                               packet_buffer.size());
                }
            }
            
            pcap_close(inject_handle);
        }
        
        // Free device list
        pcap_freealldevs(all_devices);
    }

    // Step 12: Cleanup
    pcap_close(pcap_handle);
    // Note: pcap_fopen_offline_with_tstamp_precision closes the FILE*
    // So we don't need to assert_file_closed(&in_file); separately

    assert_file_closed(&in_file);
	return 0;
}