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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 24
//<Quality> {"density":7,"unique_branches":{"pcapint_validate_filter":[[428,4,428,16,0,0,4,0],[436,9,436,29,0,0,4,0],[439,4,439,16,0,0,4,0],[441,4,441,11,0,0,4,0],[446,3,446,15,0,0,4,0],[447,8,447,28,0,0,4,0],[453,4,453,16,0,0,4,0],[454,4,454,16,0,0,4,0],[457,4,457,16,0,0,4,0],[459,4,459,16,0,0,4,0],[460,4,460,16,0,0,4,0],[462,4,462,16,0,0,4,0],[463,4,463,16,0,0,4,0],[468,9,468,34,0,0,4,0],[468,9,468,34,0,0,4,1],[468,38,468,47,0,0,4,0],[468,38,468,47,0,0,4,1],[471,4,471,11,0,0,4,0],[505,9,505,34,0,0,4,0],[509,4,509,16,0,0,4,0],[510,4,510,16,0,0,4,0],[512,9,512,35,0,0,4,0],[512,39,512,65,0,0,4,0],[515,4,515,11,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_getnonblock","bpf_validate","pcap_dump_open","pcap_setdirection","pcap_next","pcap_dump","pcap_sendpacket","pcap_dump_flush","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_getnonblock","bpf_validate","pcap_dump_open","pcap_setdirection","pcap_next","pcap_dump","pcap_sendpacket","pcap_dump_flush","pcap_dump_close","pcap_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpcap APIs to process packet capture data:

*/



extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if we don't have enough data for basic operations
    if (size < sizeof(struct bpf_insn) + 16) {
        return 0;
    }

    pcap_t *pcap_handle = nullptr;
    pcap_dumper_t *dumper = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    int ret = 0;
    struct bpf_insn *bpf_program = nullptr;
    int bpf_len = 0;
    char nonblock_error[PCAP_ERRBUF_SIZE] = {0};
    
    // 1. Create input FILE* from the fuzzer data using fmemopen
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        goto cleanup;
    }
    
    // 2. Open the pcap file for reading from memory
    pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        goto cleanup;
    }
    
    // 3. Get non-blocking status (first required API)
    if (pcap_getnonblock(pcap_handle, nonblock_error) < 0) {
        // Non-critical if this fails, continue anyway
    }
    
    // 4. Create BPF program from part of the input data for validation
    // Use first part of data as BPF instructions
    bpf_len = (size - sizeof(struct bpf_insn)) / sizeof(struct bpf_insn);
    if (bpf_len > 0) {
        bpf_program = (struct bpf_insn *)malloc(bpf_len * sizeof(struct bpf_insn));
        if (bpf_program) {
            // Use data after initial header for BPF program
            size_t bpf_data_size = bpf_len * sizeof(struct bpf_insn);
            if (bpf_data_size <= size) {
                memcpy(bpf_program, data + sizeof(struct bpf_insn), bpf_data_size);
                
                // 5. Validate BPF program (second required API)
                int bpf_valid = bpf_validate(bpf_program, bpf_len);
                // Result can be ignored for fuzzing purposes
            }
        }
    }
    
    // 6. Open output file for dumping packets
    out_file = fopen("output_file", "wb");
    if (out_file && pcap_handle) {
        // 7. Open dumper for writing packets (third required API)
        dumper = pcap_dump_open(pcap_handle, "output_file");
        
        // 8. Set capture direction (fourth required API)
        // Use both directions to ensure we capture all packets
        ret = pcap_setdirection(pcap_handle, PCAP_D_INOUT);
        // Result can be ignored for fuzzing purposes
    }
    
    // 9. Process packets from input
    if (pcap_handle) {
        struct pcap_pkthdr header;
        const u_char *packet;
        
        // Read and process packets until end of file
        while ((packet = pcap_next(pcap_handle, &header)) != nullptr) {
            // Write packet to dumper if available
            if (dumper) {
                pcap_dump((u_char *)dumper, &header, packet);
            }
            
            // 10. Send packet back out (fifth required API)
            // Note: This would normally send on a live interface,
            // but we're using it for fuzzing the API
            if (header.len > 0 && header.len <= size) {
                pcap_sendpacket(pcap_handle, packet, header.len);
            }
        }
    }
    
    // Flush output if we have a dumper
    if (dumper) {
        pcap_dump_flush(dumper);
    }

cleanup:
    // Clean up all allocated resources
    if (dumper) {
        pcap_dump_close(dumper);
    }
    
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
        // Clean up the output file we created
        remove("output_file");
    }
    
    if (bpf_program) {
        free(bpf_program);
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}