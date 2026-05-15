// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // 1. pcap_strerror - convert errno to error string
    int errno_val = (Size > 0) ? (int)Data[0] : 0;
    const char *error_str = pcap_strerror(errno_val);
    (void)error_str;  // Use the result to avoid unused variable warning

    // 2. pcap_open_offline - open a capture file
    // Write fuzz data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_offline = pcap_open_offline("./dummy_file", errbuf);
    if (pcap_offline) {
        pcap_close(pcap_offline);
    }

    // 3. pcap_datalink_name_to_val - convert link-layer name to DLT value
    // Create a null-terminated string from fuzz data for the name
    static char dlt_name[256];
    size_t name_len = (Size < sizeof(dlt_name) - 1) ? Size : sizeof(dlt_name) - 1;
    if (name_len > 0) {
        memcpy(dlt_name, Data, name_len);
    }
    dlt_name[name_len] = '\0';
    int dlt_val = pcap_datalink_name_to_val(dlt_name);
    (void)dlt_val;  // Use the result to avoid unused variable warning

    // 4. pcap_open_dead - create a dummy pcap handle
    int linktype = (Size > 1) ? (int)Data[1] : DLT_EN10MB;
    int snaplen = (Size > 2) ? (int)Data[2] * 256 + Data[3] : 65535;
    pcap_t *pcap_dead = pcap_open_dead(linktype, snaplen);
    if (!pcap_dead) {
        return 0;
    }

    // 5. pcap_compile - compile a filter expression
    // Create a null-terminated string from fuzz data for the filter
    static char filter[1024];
    size_t filter_len = (Size < sizeof(filter) - 1) ? Size : sizeof(filter) - 1;
    if (filter_len > 0) {
        memcpy(filter, Data, filter_len);
    }
    filter[filter_len] = '\0';

    struct bpf_program prog;
    memset(&prog, 0, sizeof(prog));
    
    int optimize = (Size > 4) ? (int)Data[4] & 1 : 1;
    bpf_u_int32 netmask = (Size > 8) ? 
        (bpf_u_int32)(Data[5] << 24 | Data[6] << 16 | Data[7] << 8 | Data[8]) : 0xFFFFFF00;
    
    int compile_result = pcap_compile(pcap_dead, &prog, filter, optimize, netmask);
    (void)compile_result;  // Use the result to avoid unused variable warning
    
    // Always free the BPF program structure
    pcap_freecode(&prog);
    
    // Clean up
    pcap_close(pcap_dead);
    
    // Remove the dummy file
    remove("./dummy_file");
    
    return 0;
}