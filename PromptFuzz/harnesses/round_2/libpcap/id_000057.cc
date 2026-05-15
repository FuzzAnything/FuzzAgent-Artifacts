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
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"pcap_check_header":[[371,2,371,9,0,0,4,0]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_set_tstamp_precision","pcap_get_tstamp_precision","pcap_set_protocol_linux","pcap_tstamp_type_val_to_description","pcap_set_datalink","pcap_breakloop","pcap_datalink","pcap_compile_nopcap","bpf_image","bpf_validate","pcap_freecode","pcap_minor_version","pcap_next","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_set_tstamp_precision","pcap_get_tstamp_precision","pcap_set_protocol_linux","pcap_tstamp_type_val_to_description","pcap_set_datalink","pcap_breakloop","pcap_datalink","pcap_compile_nopcap","bpf_image","bpf_validate","pcap_freecode","pcap_minor_version","pcap_next","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Helper function to generate random strings from fuzz data
const char* get_random_string(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset >= size) return "test";
    
    // Find next null byte or end of data
    size_t start = *offset;
    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    
    // Calculate length and advance past null byte
    size_t len = *offset - start;
    if (*offset < size) (*offset)++;  // Skip null terminator
    
    if (len == 0) return "test";
    
    // Create a temporary string (simplified - in real fuzzing you'd allocate properly)
    static char buffer[256];
    size_t copy_len = len < 255 ? len : 255;
    memcpy(buffer, data + start, copy_len);
    buffer[copy_len] = '\0';
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* data, size_t size) {
    if (size < 24) return 0;  // Need minimum data for meaningful fuzzing
    
    size_t offset = 0;
    
    // 1. Create FILE* from input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) return 0;
    
    // 2. Use pcap_fopen_offline_with_tstamp_precision to read pcap data
    // Get timestamp precision from data
    u_int tstamp_precision = data[0] % 3;  // 0-2 range
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = pcap_fopen_offline_with_tstamp_precision(in_file, tstamp_precision, errbuf);
    
    if (!pcap) {
        fclose(in_file);
        return 0;
    }
    
    // 3. Get snapshot length
    int snaplen = pcap_snapshot(pcap);
    
    // 4. Set timestamp precision (different from opening precision)
    int new_precision = (data[1] % 2) ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
    pcap_set_tstamp_precision(pcap, new_precision);
    
    // 5. Get timestamp precision value
    int current_precision = pcap_get_tstamp_precision(pcap);
    
    // 6. Set protocol for Linux (if available on this platform)
    #ifdef __linux__
    int protocol = data[2] % 256;
    pcap_set_protocol_linux(pcap, protocol);
    #endif
    
    // 7. Get timestamp type description
    int tstamp_type = data[3] % 8;  // Common timestamp types
    const char* tstamp_desc = pcap_tstamp_type_val_to_description(tstamp_type);
    
    // 8. Set data link type
    int dlt = data[4] % 20;  // Common DLT values
    pcap_set_datalink(pcap, dlt);
    
    // 9. Set up breakloop mechanism
    // We'll read some packets first, then break the loop
    pcap_breakloop(pcap);
    
    // 10. Compile BPF filter using pcap_compile_nopcap
    struct bpf_program bpf_prog;
    const char* filter_str = get_random_string(data, size, &offset);
    int optimize = data[5] % 2;
    bpf_u_int32 netmask = (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | data[9];
    
    // Use current snaplen and DLT for compilation
    int current_dlt = pcap_datalink(pcap);
    int compile_result = pcap_compile_nopcap(snaplen, current_dlt, &bpf_prog, 
                                            filter_str, optimize, netmask);
    
    // 11. Get BPF image if compilation succeeded
    if (compile_result == 0 && bpf_prog.bf_insns && bpf_prog.bf_len > 0) {
        char* bpf_str = bpf_image(bpf_prog.bf_insns, bpf_prog.bf_len);
        // bpf_str could be used here
        
        // 12. Validate BPF program
        int valid = bpf_validate(bpf_prog.bf_insns, bpf_prog.bf_len);
        
        // 13. Free BPF program
        pcap_freecode(&bpf_prog);
    }
    
    // 14. Get minor version
    int minor_ver = pcap_minor_version(pcap);
    
    // 15. Read packets from the pcap file
    struct pcap_pkthdr header;
    int packets_read = 0;
    const u_char* packet_data;
    
    while ((packet_data = pcap_next(pcap, &header)) != nullptr && packets_read < 10) {
        packets_read++;
        
        // Do something with packet data (simplified for fuzzing)
        if (header.caplen > 0 && packet_data) {
            // Access packet data to ensure coverage
            volatile uint8_t first_byte = packet_data[0];
        }
    }
    
    // 16. Close pcap handle (will also close the FILE*)
    pcap_close(pcap);
    
    // Note: pcap_close should close in_file, but to be safe in case pcap is NULL
    if (in_file) {
        fclose(in_file);
    }
    
    return 0;
}