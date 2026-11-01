// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
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
#include <netinet/in.h>
#include <sys/socket.h>

static void cleanup_bpf_program(struct bpf_program *fp) {
    if (fp && fp->bf_insns) {
        pcap_freecode(fp);
        fp->bf_insns = NULL;
        fp->bf_len = 0;
    }
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Create a null-terminated string from input for device name and filter
    char device_name[256];
    char filter_str[256];
    char errbuf[PCAP_ERRBUF_SIZE];

    size_t dev_len = (Size < sizeof(device_name) - 1) ? Size : sizeof(device_name) - 1;
    size_t filter_len = (Size < sizeof(filter_str) - 1) ? Size : sizeof(filter_str) - 1;

    memcpy(device_name, Data, dev_len);
    device_name[dev_len] = '\0';
    memcpy(filter_str, Data, filter_len);
    filter_str[filter_len] = '\0';

    // 1. Test pcap_lookupnet
    bpf_u_int32 net, mask;
    int ret = pcap_lookupnet(device_name, &net, &mask, errbuf);
    (void)ret; // Ignore return value to explore states

    // 2. Test pcap_nametonetaddr
    bpf_u_int32 addr = pcap_nametonetaddr(device_name);
    (void)addr;

    // 3. Test pcap_nametoaddr (Deprecated but tested for fuzzing)
    struct in_addr *addr_list = pcap_nametoaddr(device_name);
    (void)addr_list;

    // 4. Test pcap_compile_nopcap
    struct bpf_program fp_nopcap;
    memset(&fp_nopcap, 0, sizeof(fp_nopcap));
    ret = pcap_compile_nopcap(DLT_EN10MB, 65535, &fp_nopcap, filter_str, 1, 0);
    if (ret == 0) {
        cleanup_bpf_program(&fp_nopcap);
    }

    // 5. Test pcap_compile and pcap_setfilter with a live handle
    pcap_t *handle = pcap_create(device_name, errbuf);
    if (handle) {
        // Try to set options to activate
        pcap_set_snaplen(handle, 65535);
        pcap_set_promisc(handle, 1);
        pcap_set_timeout(handle, 1000);
        pcap_set_buffer_size(handle, 262144);

        ret = pcap_activate(handle);
        if (ret == 0) {
            struct bpf_program fp;
            memset(&fp, 0, sizeof(fp));
            ret = pcap_compile(handle, &fp, filter_str, 1, net);
            if (ret == 0) {
                ret = pcap_setfilter(handle, &fp);
                (void)ret;
                cleanup_bpf_program(&fp);
            }
        }
        cleanup_pcap(handle);
    }

    // 6. Test pcap_compile and pcap_setfilter with a dead handle
    handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (handle) {
        struct bpf_program fp_dead;
        memset(&fp_dead, 0, sizeof(fp_dead));
        ret = pcap_compile(handle, &fp_dead, filter_str, 1, 0);
        if (ret == 0) {
            ret = pcap_setfilter(handle, &fp_dead);
            (void)ret;
            cleanup_bpf_program(&fp_dead);
        }
        cleanup_pcap(handle);
    }

    return 0;
}