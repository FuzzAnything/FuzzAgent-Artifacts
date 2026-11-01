// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <namedb.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

static void cleanup_pcap(pcap_t *handle, struct bpf_program *prog) {
    if (prog != NULL && prog->bf_insns != NULL) {
        pcap_freecode(prog);
    }
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
    struct bpf_program prog;
    pcap_t *handle = NULL;
    int linktype = DLT_EN10MB;
    int snaplen = 65535;

    // Extract parameters from fuzz input
    int optimize = (Data[0] % 2);
    int linktype_choice = Data[1] % 3;
    if (linktype_choice == 1) linktype = DLT_RAW;
    if (linktype_choice == 2) linktype = DLT_IPV4;

    // Prepare a filter string from input, ensuring it's null-terminated and safe
    size_t filter_len = Size - 2;
    if (filter_len > 256) filter_len = 256;
    char filter_str[257];
    memcpy(filter_str, Data + 2, filter_len);
    filter_str[filter_len] = '\0';

    // Ensure the filter string doesn't contain null bytes in the middle that might break parsing
    // (pcap_compile handles this, but we ensure valid C string)
    for (size_t i = 0; i < filter_len; i++) {
        if (filter_str[i] == '\0') {
            filter_str[i] = ' ';
        }
    }

    // Test pcap_lookupnet with a dummy device name derived from input
    char device_name[32];
    size_t dev_len = (Size > 32) ? 31 : Size - 2;
    memcpy(device_name, Data + 2, dev_len);
    device_name[dev_len] = '\0';
    // Sanitize device name to avoid system calls that might hang or fail unexpectedly
    for (size_t i = 0; i < dev_len; i++) {
        if (device_name[i] < 32 || device_name[i] > 126) {
            device_name[i] = 'x';
        }
    }

    int lookup_res = pcap_lookupnet(device_name, &net, &mask, errbuf);
    (void)lookup_res; // Use result to avoid unused warning

    // Test pcap_nametonetaddr
    bpf_u_int32 addr = pcap_nametonetaddr(filter_str);
    (void)addr;

    // Test pcap_compile_nopcap (deprecated but still valid for fuzzing)
    memset(&prog, 0, sizeof(prog));
    int nopcap_res = pcap_compile_nopcap(linktype, snaplen, &prog, filter_str, optimize, net);
    if (nopcap_res == 0) {
        pcap_freecode(&prog);
    }

    // Test pcap_compile and pcap_setfilter with a real handle
    // Use "any" or a dummy device to avoid needing actual hardware
    handle = pcap_open_dead(linktype, snaplen);
    if (handle == NULL) {
        return 0;
    }

    // Try to compile the filter
    memset(&prog, 0, sizeof(prog));
    int compile_res = pcap_compile(handle, &prog, filter_str, optimize, net);
    if (compile_res == 0) {
        // Try to set the filter
        int setfilter_res = pcap_setfilter(handle, &prog);
        (void)setfilter_res;
        
        // Cleanup filter
        pcap_freecode(&prog);
    }

    // Cleanup handle
    pcap_close(handle);

    return 0;
}