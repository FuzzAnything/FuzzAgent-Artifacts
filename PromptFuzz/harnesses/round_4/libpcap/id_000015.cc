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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 180, nr_unique_branch: 14
//<Quality> {"density":20,"unique_branches":{"gen_linktype":[[4440,3,4440,24,0,0,4,0]],"pcapint_install_bpf_program":[[2928,6,2928,56,0,0,4,0]],"pcap_create":[[2318,6,2318,20,0,0,4,0]],"pcap_lex":[[3532,1,3532,7,0,0,4,0],[3592,1,3592,8,0,0,4,0],[3597,1,3597,8,0,0,4,0]],"pcap_sendpacket":[[4291,6,4291,38,0,0,4,1]],"pcap_parse":[[2382,3,2382,10,0,0,4,0],[2484,3,2484,10,0,0,4,0],[2490,3,2490,10,0,0,4,0]],"gen_proto_abbrev_internal":[[6021,2,6021,15,0,0,4,0],[6028,2,6028,13,0,0,4,0],[6036,2,6036,13,0,0,4,0]],"pcapint_validate_filter":[[412,6,412,13,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_get_tstamp_precision","pcap_close","pcap_dump_fopen","pcap_close","pcap_datalink","pcap_snapshot","pcap_compile_nopcap","pcap_compile_nopcap","pcap_compile_nopcap","pcap_setfilter","pcap_next","pcap_dump","pcap_dump_ftell","pcap_create","pcap_sendpacket","pcap_close","pcap_freecode","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_get_tstamp_precision","pcap_dump_fopen","pcap_datalink","pcap_snapshot","pcap_compile_nopcap","pcap_compile_nopcap","pcap_compile_nopcap","pcap_setfilter","pcap_next","pcap_dump","pcap_dump_ftell","pcap_create","pcap_sendpacket","pcap_close","pcap_freecode","pcap_dump_close","pcap_close"],"visited":0}
/*Looking at the requirements, I need to create a fuzz driver that:
1. Uses the specified libpcap APIs together
2. Processes input data as a pcap capture file
3. Applies a BPF filter to the capture
4. Sends packets from the filtered capture
5. Uses all the specified APIs at least once

Here's my implementation:

*/


extern "C" {
}

// Custom implementation of fmemopen for platforms that might not have it
#ifndef _GNU_SOURCE
extern "C" {
    FILE *fmemopen(void *buf, size_t size, const char *mode);
}
#endif

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<108) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	 // Minimum size for a valid pcap file header
    
    char errbuf[PCAP_ERRBUF_SIZE];
    errbuf[0] = '\0';
    
    // Create FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open pcap capture from file
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get timestamp precision of the capture
    int timestamp_precision = pcap_get_tstamp_precision(pcap_handle);
    
    // Create output file for dump
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a dumper for output
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap_handle, out_file);
    if (!dumper) {
        assert_file_closed(&out_file);;
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get datalink type and snapshot length for filter compilation
    int datalink = pcap_datalink(pcap_handle);
    int snaplen = pcap_snapshot(pcap_handle);
    
    // Compile BPF filter (use "ip" as a simple filter)
    struct bpf_program filter_program;
    const char *filter_exp = "ip or ip6 or arp or rarp or tcp or udp or icmp or icmp6";
    
    // Compile filter using nopcap - we need network mask for some filters
    bpf_u_int32 netmask = 0xffffff00; // Default netmask for compile
    if (pcap_compile_nopcap(snaplen, datalink, &filter_program, 
                           fuzz_str_1, fuzz_int32_t_2, fuzz_uint32_t_3) == -1) {
        // If filter fails, use a simpler filter
        const char *simple_filter = "tcp or udp or icmp";
        if (pcap_compile_nopcap(snaplen, datalink, &filter_program,
                               fuzz_str_4, fuzz_int32_t_5, fuzz_uint32_t_6) == -1) {
            // If all filters fail, use a catch-all filter
            const char *catch_all = "ip";
            pcap_compile_nopcap(snaplen, datalink, &filter_program,
                               fuzz_str_7, fuzz_int32_t_8, fuzz_uint32_t_9);
        }
    }
    
    // Set filter on the pcap handle
    if (pcap_setfilter(pcap_handle, &filter_program) == -1) {
        // Filter setting failed, but continue processing
    }
    
    // Process packets from the capture
    struct pcap_pkthdr header;
    const u_char *packet_data;
    int packets_processed = 0;
    
    while ((packet_data = pcap_next(pcap_handle, &header)) != nullptr) {
        // Dump the packet to output file
        pcap_dump((u_char *)dumper, &header, packet_data);
        
        // Get current position in dump file
        long dump_pos = pcap_dump_ftell(dumper);
        
        // Send the packet (simulate sending - in real scenario this would need a live interface)
        // We'll create a dummy pcap handle for sending attempt
        pcap_t *send_handle = pcap_create(NULL, errbuf);
        if (send_handle) {
            // Try to send the packet (will likely fail without interface, but we call the API)
            pcap_sendpacket(send_handle, packet_data, header.caplen);
            pcap_close(send_handle);
        }
        
        packets_processed++;
        if (packets_processed > 100) break; // Limit processing for fuzzing
    }
    
    // Clean up
    pcap_freecode(&filter_program);
    pcap_dump_close(dumper);
    assert_file_closed(&out_file);;
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;
    
    // Clean up the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}