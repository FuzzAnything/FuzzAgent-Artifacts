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
//<ID> 411
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"setup_mmapped":[[2844,6,2844,34,0,0,4,1]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_create","pcap_close","pcap_set_buffer_size","pcap_set_tstamp_type","pcap_activate","pcap_close","pcap_close","pcap_getnonblock","bpf_image","pcap_next","bpf_filter","pcap_sendpacket","pcap_close","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_create","pcap_set_buffer_size","pcap_set_tstamp_type","pcap_activate","pcap_getnonblock","bpf_image","pcap_next","bpf_filter","pcap_sendpacket","pcap_close","pcap_close"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Event: Parse pcap offline capture file, apply BPF filter to packets,
    // activate a live capture interface, set properties, and send filtered packets
    
    if (size < sizeof(struct pcap_file_header)) {
        return 0;
    }

    // 1. Create FILE* from input data for offline reading
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

    // 3. Create a live capture interface (using loopback as example)
    pcap_t *live_pcap = pcap_create("lo", errbuf);
    if (!live_pcap) {
        pcap_close(offline_pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Set buffer size for live capture
    pcap_set_buffer_size(live_pcap, 1024 * 1024); // 1MB buffer

    // 5. Set timestamp type for live capture
    pcap_set_tstamp_type(live_pcap, PCAP_TSTAMP_HOST);

    // 6. Activate the live capture
    int activate_ret = pcap_activate(live_pcap);
    if (activate_ret < 0 && activate_ret != PCAP_WARNING) {
        pcap_close(live_pcap);
        pcap_close(offline_pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 7. Get non-blocking status
    char nonblock_err[PCAP_ERRBUF_SIZE] = {0};
    int nonblock_ret = pcap_getnonblock(live_pcap, nonblock_err);

    // 8. Create a simple BPF filter program (accept all packets)
    struct bpf_insn insns[] = {
        { 0x06, 0, 0, 0x0000ffff }, // ld #0xffff
        { 0x07, 0, 0, 0x00000000 }, // tax
        { 0x06, 0, 0, 0x00000000 }, // ld #0x0
        { 0x16, 0, 0, 0x00000000 }  // ret a
    };
    
    // 9. Get BPF image (string representation)
    char *bpf_str = bpf_image(insns, sizeof(insns)/sizeof(insns[0]));

    // Process packets from offline capture
    struct pcap_pkthdr header;
    const u_char *packet;
    std::vector<std::vector<u_char>> filtered_packets;

    while ((packet = pcap_next(offline_pcap, &header)) != nullptr) {
        // 10. Apply BPF filter to each packet
        u_int filter_result = bpf_filter(
            insns, 
            packet, 
            header.len, 
            header.caplen
        );
        
        if (filter_result > 0) {
            // Store filtered packet for sending
            std::vector<u_char> pkt_copy(packet, packet + header.caplen);
            filtered_packets.push_back(std::move(pkt_copy));
        }
    }

    // 11. Send filtered packets through live interface
    for (const auto& pkt : filtered_packets) {
        if (!pkt.empty()) {
            pcap_sendpacket(live_pcap, pkt.data(), pkt.size());
        }
    }

    // 12. Cleanup resources
    pcap_close(live_pcap);
    pcap_close(offline_pcap);
    assert_file_closed(&in_file);;
    
    // bpf_image returns pointer to static buffer, no need to free
    
    assert_file_closed(&in_file);
	return 0;
}