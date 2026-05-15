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
//<ID> 469
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"pcap_ng_next_packet":[[1209,8,1209,18,0,0,4,0]]},"library_calls":["pcap_init","pcap_fopen_offline","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_set_timeout","pcap_set_datalink","pcap_dump_open_append","pcap_dump_fopen","pcap_compile_nopcap","bpf_validate","pcap_next","pcap_offline_filter","bpf_filter","pcap_dump","pcap_freecode","pcap_dump_flush","pcap_dump_close","pcap_geterr","pcap_perror","pcap_close"],"critical_calls":["pcap_init","pcap_fopen_offline","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_set_timeout","pcap_set_datalink","pcap_dump_open_append","pcap_dump_fopen","pcap_compile_nopcap","bpf_validate","pcap_next","pcap_offline_filter","bpf_filter","pcap_dump","pcap_freecode","pcap_dump_flush","pcap_dump_close","pcap_geterr","pcap_perror","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	  // Need minimum bytes for pcap header
    
    // Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(0, errbuf) != 0) {
        return 0;
    }
    
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open offline capture from memory buffer
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get datalink type
    int dlt = pcap_datalink(pcap);
    const char *dlt_desc = pcap_datalink_val_to_description_or_dlt(dlt);
    (void)dlt_desc; // Suppress unused warning
    
    // Set timeout (though it's offline)
    if (pcap_set_timeout(pcap, 1000) != 0) {
        // Continue anyway
    }
    
    // Try to set datalink back to what it was (no-op essentially)
    if (pcap_set_datalink(pcap, dlt) != 0) {
        // Continue anyway
    }
    
    // Open output file for dumping filtered packets
    FILE *out_file = fopen("output_file", "wb");
    pcap_dumper_t *dumper = nullptr;
    if (out_file) {
        dumper = pcap_dump_open_append(pcap, "output_file");
        if (!dumper) {
            // Try with file pointer instead
            dumper = pcap_dump_fopen(pcap, out_file);
        }
    }
    
    // Create and compile a simple BPF filter
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // Use a simple filter that should compile
    const char *filter_str = "ip or arp";
    bpf_u_int32 netmask = 0xffffff00; // Typical netmask
    
    // Compile filter for offline capture
    if (pcap_compile_nopcap(65535, dlt, &fp, filter_str, 1, netmask) == 0) {
        // Validate the BPF program
        if (bpf_validate(fp.bf_insns, fp.bf_len) != 0) {
            // Process packets
            struct pcap_pkthdr header;
            const u_char *packet;
            
            while ((packet = pcap_next(pcap, &header)) != nullptr) {
                // Apply filter using pcap_offline_filter
                if (pcap_offline_filter(&fp, &header, packet) != 0) {
                    // Also apply direct bpf_filter (should give same result)
                    u_int captured = bpf_filter(fp.bf_insns, packet, header.len, header.caplen);
                    (void)captured; // Suppress unused warning
                    
                    // Dump filtered packet if we have a dumper
                    if (dumper) {
                        pcap_dump((u_char *)dumper, &header, packet);
                    }
                }
            }
        }
        
        // Free the BPF program
        pcap_freecode(&fp);
    }
    
    // Flush and close dumper if opened
    if (dumper) {
        pcap_dump_flush(dumper);
        pcap_dump_close(dumper);
    }
    
    // Close output file if opened
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Get and log any errors (for debugging, doesn't affect fuzzing)
    char *pcap_err = pcap_geterr(pcap);
    if (pcap_err && *pcap_err) {
        pcap_perror(pcap, "libpcap error");
    }
    
    // Cleanup
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    
    // Remove output file to avoid disk filling
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}