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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 7
//<Quality> {"density":9,"unique_branches":{"pcap_setnonblock_linux":[[3539,7,3539,39,0,0,4,1]],"pcap_protocol":[[871,6,871,19,0,0,4,1]],"pcap_set_tstamp_type":[[2652,7,2652,38,0,0,4,1]],"pcap_activate":[[2776,7,2776,22,0,0,4,0],[2778,8,2778,18,0,0,4,1]],"pcap_activate_linux":[[1276,6,1276,27,0,0,4,1]],"pcap_set_protocol_linux":[[6235,6,6235,32,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_create","pcap_lookupdev","pcap_create","pcap_setnonblock","pcap_set_tstamp_type","pcap_set_protocol_linux","pcap_activate","pcap_sendpacket","pcap_dump_fopen","pcap_dump_open_append","pcap_datalink_name_to_val","pcap_next","pcap_dump","pcap_dump","pcap_dump_close","pcap_dump_close","pcap_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_create","pcap_lookupdev","pcap_create","pcap_setnonblock","pcap_set_tstamp_type","pcap_set_protocol_linux","pcap_activate","pcap_sendpacket","pcap_dump_fopen","pcap_dump_open_append","pcap_datalink_name_to_val","pcap_next","pcap_dump","pcap_dump","pcap_dump_close","pcap_dump_close","pcap_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<15) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0;  // Need enough data for meaningful operations
    }

    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create pcap handle from input file
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Create a live capture handle (for sending packets)
    pcap_t *live_handle = pcap_create(fuzz_str_1, errbuf);
    if (!live_handle) {
        // Try with different interface name
        char *dev = pcap_lookupdev(errbuf);
        if (dev) {
            live_handle = pcap_create(dev, errbuf);
        }
    }

    if (live_handle) {
        // 4. Set non-blocking mode
        char errbuf2[PCAP_ERRBUF_SIZE];
        pcap_setnonblock(live_handle, 1, errbuf2);
        
        // 5. Set timestamp type (using valid type from input)
        int tstamp_type = (int)(data[0] % 4);  // Use modulo to get valid type
        pcap_set_tstamp_type(live_handle, tstamp_type);
        
        // 6. Set protocol for Linux (if applicable)
        #ifdef __linux__
        int protocol = (int)(data[1] % 256);
        pcap_set_protocol_linux(live_handle, protocol);
        #endif
        
        // Activate the live handle
        if (pcap_activate(live_handle) == 0) {
            // 7. Send a packet using data from input
            // Use middle portion of data as packet content
            size_t packet_size = size / 2;
            if (packet_size > 0) {
                const u_char *packet_data = data + (size / 4);
                pcap_sendpacket(live_handle, packet_data, (int)packet_size);
            }
        }
    }

    // 8. Create output files for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // 9. Open dumper using FILE*
        pcap_dumper_t *dumper1 = pcap_dump_fopen(pcap_handle, out_file);
        
        // 10. Open dumper in append mode using filename
        pcap_dumper_t *dumper2 = pcap_dump_open_append(pcap_handle, "output_file_append.pcap");
        
        // 11. Convert data link type name to value (using a sample name)
        const char *dlt_name = "EN10MB";  // Ethernet
        int dlt_val = pcap_datalink_name_to_val(fuzz_str_2);
        
        // Try to read and process packets from input file
        struct pcap_pkthdr header;
        const u_char *packet;
        
        // Read first few packets and dump them
        for (int i = 0; i < 3; i++) {
            packet = pcap_next(pcap_handle, &header);
            if (!packet) break;
            
            // Dump to both dumpers if they exist
            if (dumper1) {
                pcap_dump((u_char *)dumper1, &header, packet);
            }
            if (dumper2) {
                pcap_dump((u_char *)dumper2, &header, packet);
            }
        }
        
        // Clean up dumpers
        if (dumper1) {
            pcap_dump_close(dumper1);
        }
        if (dumper2) {
            pcap_dump_close(dumper2);
        }
        
        assert_file_closed(&out_file);;
        
        // Remove the output files to clean up
        remove("output_file");
        remove("output_file_append.pcap");
    }

    // 12. Clean up all allocated resources
    if (live_handle) {
        pcap_close(live_handle);
    }
    
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}