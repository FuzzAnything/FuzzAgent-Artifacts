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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_dump_open_append","pcap_perror","pcap_close","pcap_get_selectable_fd","pcap_datalink","pcap_set_datalink","pcap_perror","pcap_next","pcap_dump","pcap_perror","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_dump_open_append","pcap_get_selectable_fd","pcap_datalink","pcap_set_datalink","pcap_perror","pcap_next","pcap_dump","pcap_perror","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Create FILE* from input data
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Open pcap from the input file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Open output pcap dumper in append mode
    pcap_dumper_t* dumper = pcap_dump_open_append(pcap_handle, "output_file");
    if (!dumper) {
        pcap_perror(pcap_handle, "pcap_dump_open_append failed");
        pcap_close(pcap_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // 4. Get selectable file descriptor (though we don't actively use it in this fuzzing context)
    int selectable_fd = pcap_get_selectable_fd(pcap_handle);
    (void)selectable_fd; // Mark as used to avoid unused variable warning

    // 5. Set datalink type (try to set to the current type as a no-op)
    int current_dlt = pcap_datalink(pcap_handle);
    if (pcap_set_datalink(pcap_handle, current_dlt) < 0) {
        pcap_perror(pcap_handle, "pcap_set_datalink failed");
    }

    // 6. Read and dump packets from input to output
    struct pcap_pkthdr header;
    const u_char* packet_data;
    int packet_count = 0;
    
    while ((packet_data = pcap_next(pcap_handle, &header)) != NULL) {
        pcap_dump((u_char*)dumper, &header, packet_data);
        packet_count++;
        
        // Limit the number of packets to avoid infinite loops with malformed data
        if (packet_count > 1000) {
            break;
        }
    }

    // 7. Check for errors during packet reading
    if (packet_count == 0) {
        pcap_perror(pcap_handle, "No packets read or error occurred");
    }

    // 8. Clean up resources
    pcap_dump_close(dumper);
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}