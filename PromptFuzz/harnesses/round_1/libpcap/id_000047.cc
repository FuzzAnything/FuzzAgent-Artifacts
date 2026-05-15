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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"pcap_set_tstamp_type":[[2652,7,2652,38,0,0,4,1]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_next_ex","pcap_findalldevs","pcap_create","pcap_create","pcap_set_tstamp_type","pcap_setnonblock","pcap_close","pcap_freealldevs","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_next_ex","pcap_findalldevs","pcap_create","pcap_set_tstamp_type","pcap_setnonblock","pcap_close","pcap_freealldevs","pcap_close"],"visited":0}
/**/


// Libpcap headers

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
        return 0;
    }

    // 1. Create a FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Open offline capture with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *offline_pcap = pcap_fopen_offline_with_tstamp_precision(
        in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    
    if (!offline_pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Read packets from offline capture
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;
    int packet_count = 0;
    
    // Read up to 10 packets to limit fuzzing time
    while (packet_count < 10 && 
           pcap_next_ex(offline_pcap, &header, &packet_data) == 1) {
        packet_count++;
    }

    // 4. Find all available network devices
    pcap_if_t *all_devices = nullptr;
    if (pcap_findalldevs(&all_devices, errbuf) == -1) {
        // Continue execution even if this fails
    }

    // 5. Create a pcap handle for live capture (using dummy device name)
    pcap_t *live_pcap = nullptr;
    if (all_devices && all_devices->name) {
        // Try to use first available device
        live_pcap = pcap_create(all_devices->name, errbuf);
    } else {
        // Fallback to "any" device
        live_pcap = pcap_create("any", errbuf);
    }

    // 6. Configure the live pcap handle if created
    if (live_pcap) {
        // Set timestamp type (try a few common ones)
        const int tstamp_types[] = {
            PCAP_TSTAMP_HOST,
            PCAP_TSTAMP_HOST_LOWPREC,
            PCAP_TSTAMP_HOST_HIPREC,
            PCAP_TSTAMP_ADAPTER,
            PCAP_TSTAMP_ADAPTER_UNSYNCED
        };
        
        for (int i = 0; i < 5 && i < (int)(size % 5); i++) {
            pcap_set_tstamp_type(live_pcap, tstamp_types[i]);
        }

        // Set non-blocking mode
        int nonblock = (size % 2) ? 1 : 0;
        pcap_setnonblock(live_pcap, nonblock, errbuf);
    }

    // 7. Cleanup resources
    if (live_pcap) {
        pcap_close(live_pcap);
    }
    
    if (all_devices) {
        pcap_freealldevs(all_devices);
    }
    
    // Note: offline_pcap will close the FILE pointer automatically
    pcap_close(offline_pcap);
    // Don't assert_file_closed(&in_file); because pcap_close already closed it

    // 8. Test with output file (optional cleanup)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data if we read packets
        if (packet_count > 0 && size > 0) {
            fwrite(data, 1, size > 1024 ? 1024 : size, out_file);
        }
        assert_file_closed(&out_file);;
        
        // Clean up the output file
        remove("output_file");
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}