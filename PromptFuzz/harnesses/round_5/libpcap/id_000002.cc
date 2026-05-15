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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_is_swapped","pcap_list_tstamp_types","pcap_set_tstamp_type","pcap_set_snaplen","pcap_next_ex","bpf_validate","pcap_4_byte_aligned_int64_val","pcap_create","pcap_activate","pcap_sendpacket","pcap_close","pcap_free_tstamp_types","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_is_swapped","pcap_list_tstamp_types","pcap_set_tstamp_type","pcap_set_snaplen","pcap_next_ex","bpf_validate","pcap_4_byte_aligned_int64_val","pcap_create","pcap_activate","pcap_sendpacket","pcap_close","pcap_free_tstamp_types","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 40) {
	return 0;
	}
	 // Minimum size for meaningful processing

    // 1. Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Open pcap file from memory
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Check if pcap file is swapped
    int swapped = pcap_is_swapped(pcap_handle);

    // 4. Get timestamp types
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    
    // 5. Set timestamp type if available
    if (tstamp_count > 0 && tstamp_types) {
        // Use first available timestamp type
        pcap_set_tstamp_type(pcap_handle, tstamp_types[0]);
    }

    // 6. Set snapshot length (using a value derived from input size)
    int snaplen = size % 65536; // Ensure reasonable size
    if (snaplen < 68) snaplen = 68; // Minimum Ethernet frame
    pcap_set_snaplen(pcap_handle, snaplen);

    // 7. Prepare for packet reading
    struct pcap_pkthdr *header;
    const u_char *packet_data;
    int packet_count = 0;

    // 8. Read first packet
    if (pcap_next_ex(pcap_handle, &header, &packet_data) == 1) {
        packet_count++;
        
        // 9. Validate BPF filter using packet data
        // Create minimal BPF filter that matches all packets
        struct bpf_insn insns[] = {
            { 0x06, 0, 0, 0xFFFFFFFF } // RET #0xFFFFFFFF
        };
        int bpf_valid = bpf_validate(insns, 1);

        // 10. Use 4-byte aligned int64 value from timestamp
        if (header->ts.tv_sec > 0 || header->ts.tv_usec > 0) {
            pcap_4_byte_aligned_int64 timestamp_val;
            timestamp_val.halves[0] = header->ts.tv_sec;
            timestamp_val.halves[1] = header->ts.tv_usec;
            int64_t converted = pcap_4_byte_aligned_int64_val(timestamp_val);
            (void)converted; // Use to avoid unused variable warning
        }

        // 11. Create live capture handle for packet sending
        pcap_t *live_handle = pcap_create("any", errbuf);
        if (live_handle) {
            pcap_activate(live_handle);
            
            // 12. Send packet (if we have one)
            if (header->caplen > 0) {
                pcap_sendpacket(live_handle, packet_data, header->caplen);
            }
            
            pcap_close(live_handle);
        }
    }

    // 13. Cleanup
    if (tstamp_types) {
        pcap_free_tstamp_types(tstamp_types);
    }
    
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}