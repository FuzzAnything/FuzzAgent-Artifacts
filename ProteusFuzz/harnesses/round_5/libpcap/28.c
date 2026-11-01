// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <namedb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static void cleanup_bpf_program(struct bpf_program *fp) {
    if (fp && fp->bf_insns) {
        pcap_freecode(fp);
        fp->bf_insns = NULL;
        fp->bf_len = 0;
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into segments for different operations
    size_t seg1_len = Size / 5;
    size_t seg2_len = Size / 5;
    size_t seg3_len = Size / 5;
    size_t seg4_len = Size / 5;
    size_t seg5_len = Size - seg1_len - seg2_len - seg3_len - seg4_len;

    // Ensure null termination for strings
    char *dev_name = (char *)malloc(seg1_len + 1);
    char *filter_str = (char *)malloc(seg2_len + 1);
    char *net_name = (char *)malloc(seg3_len + 1);
    char *host_name = (char *)malloc(seg4_len + 1);
    char *errbuf = (char *)malloc(PCAP_ERRBUF_SIZE);

    if (!dev_name || !filter_str || !net_name || !host_name || !errbuf) {
        free(dev_name);
        free(filter_str);
        free(net_name);
        free(host_name);
        free(errbuf);
        return 0;
    }

    memcpy(dev_name, Data, seg1_len);
    dev_name[seg1_len] = '\0';
    memcpy(filter_str, Data + seg1_len, seg2_len);
    filter_str[seg2_len] = '\0';
    memcpy(net_name, Data + seg1_len + seg2_len, seg3_len);
    net_name[seg3_len] = '\0';
    memcpy(host_name, Data + seg1_len + seg2_len + seg3_len, seg4_len);
    host_name[seg4_len] = '\0';
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;

    // 1. Fuzz pcap_lookupnet
    // Note: pcap_lookupnet expects a valid device name. "any" or NULL often returns 0.0.0.0/0.0.0.0
    // We pass the fuzzed string as the device name.
    pcap_lookupnet(dev_name, &net, &mask, errbuf);

    // 2. Fuzz pcap_nametonetaddr
    pcap_nametonetaddr(net_name);

    // 3. Fuzz pcap_nametoaddr (Deprecated but tested for robustness)
    pcap_nametoaddr(host_name);

    // 4. Fuzz pcap_compile_nopcap
    // We need a linktype. Let's try a few common ones based on input bytes or default.
    int linktype = (Size > 0) ? (Data[0] % 100) : 1; // 1 is LINKTYPE_ETHERNET
    if (linktype < 0) linktype = 1;
    
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // pcap_compile_nopcap is deprecated but we test it.
    // It takes linktype, netmask, filter, optimize, netmask.
    int optimize = (Size > 1) ? (Data[1] % 2) : 0;
    pcap_compile_nopcap(0, linktype, &fp, filter_str, optimize, mask);
    cleanup_bpf_program(&fp);

    // 5. Fuzz pcap_compile and pcap_setfilter via pcap_open_dead
    pcap_t *handle = pcap_open_dead(linktype, 65535);
    if (handle) {
        // Try to compile
        if (pcap_compile(handle, &fp, filter_str, optimize, mask) == 0) {
            // Try to set filter
            pcap_setfilter(handle, &fp);
            cleanup_bpf_program(&fp);
        }
        pcap_close(handle);
    }

    // Cleanup
    free(dev_name);
    free(filter_str);
    free(net_name);
    free(host_name);
    free(errbuf);

    return 0;
}