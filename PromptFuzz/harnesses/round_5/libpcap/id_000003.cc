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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 6.5, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_is_swapped","pcap_set_snaplen","pcap_list_tstamp_types","pcap_set_tstamp_type","pcap_free_tstamp_types","bpf_validate","pcap_next","pcap_4_byte_aligned_int64_val","pcap_create","pcap_set_snaplen","pcap_activate","pcap_sendpacket","pcap_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_is_swapped","pcap_set_snaplen","pcap_list_tstamp_types","pcap_set_tstamp_type","pcap_free_tstamp_types","bpf_validate","pcap_next","pcap_4_byte_aligned_int64_val","pcap_create","pcap_set_snaplen","pcap_activate","pcap_sendpacket","pcap_close","pcap_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	 // Minimum for pcap file header

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open pcap file from memory
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Check if pcap file is swapped (endianness)
    int swapped = pcap_is_swapped(pcap_handle);
    
    // 2. Set snapshot length
    // Use part of input data to determine snaplen (ensuring reasonable value)
    uint32_t snaplen_val = 0;
    if (size >= 4) {
        memcpy(&snaplen_val, data, sizeof(snaplen_val));
        snaplen_val = snaplen_val % 65536 + 68; // Reasonable range: 68-65603
    } else {
        snaplen_val = 65535;
    }
    pcap_set_snaplen(pcap_handle, snaplen_val);
    
    // 3. List and set timestamp type
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    
    if (tstamp_count > 0) {
        // Use data to select a timestamp type
        uint8_t type_selector = size > 0 ? data[0] % tstamp_count : 0;
        pcap_set_tstamp_type(pcap_handle, tstamp_types[type_selector]);
        pcap_free_tstamp_types(tstamp_types);
    }
    
    // 4. Validate BPF program created from input data
    if (size >= sizeof(struct bpf_insn)) {
        // Create a minimal BPF program from input data
        struct bpf_insn bpf_program[2];
        size_t bpf_data_size = size < sizeof(bpf_program) ? size : sizeof(bpf_program);
        memcpy(bpf_program, data, bpf_data_size);
        
        // Set reasonable defaults if data is insufficient
        if (size < sizeof(struct bpf_insn)) {
            bpf_program[0].code = 0x06; // RET
            bpf_program[0].k = 0xFFFFFFFF;
        }
        
        // Validate BPF program
        int bpf_len = bpf_data_size / sizeof(struct bpf_insn);
        bpf_validate(bpf_program, bpf_len);
    }
    
    // 5. Process packets and send them
    struct pcap_pkthdr header;
    const u_char *packet_data;
    
    // Read and send first packet if available
    packet_data = pcap_next(pcap_handle, &header);
    if (packet_data && header.caplen > 0) {
        // Use pcap_4_byte_aligned_int64_val on timestamp
        pcap_4_byte_aligned_int64 timestamp;
        timestamp.halves[0] = header.ts.tv_sec;
        timestamp.halves[1] = header.ts.tv_usec;
        int64_t ts_val = pcap_4_byte_aligned_int64_val(timestamp);
        
        // Send the packet (simulating loopback)
        // Create a dummy pcap handle for sending
        pcap_t *send_handle = pcap_create("lo", errbuf); // Loopback interface
        if (send_handle) {
            pcap_set_snaplen(send_handle, snaplen_val);
            if (pcap_activate(send_handle) == 0) {
                pcap_sendpacket(send_handle, packet_data, header.caplen);
            }
            pcap_close(send_handle);
        }
    }
    
    // Cleanup
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}