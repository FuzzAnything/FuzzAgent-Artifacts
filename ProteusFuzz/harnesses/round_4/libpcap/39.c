// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <namedb.h>

static void cleanup_bpf_program(struct bpf_program *prog) {
    if (prog && prog->bf_insns) {
        pcap_freecode(prog);
        prog->bf_insns = NULL;
        prog->bf_len = 0;
    }
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Extract parameters from fuzz input
    int linktype = Data[0];
    int snaplen = (Size > 1) ? (Data[1] * 256 + (Size > 2 ? Data[2] : 0)) : 65535;
    int optimize = (Size > 3) ? Data[3] % 2 : 0;
    
    // Create a dummy filter string from input data
    // Ensure it is null-terminated and safe
    char filter_str[256];
    size_t filter_len = (Size > 4 && Size < 256) ? Size - 4 : 0;
    if (filter_len > 0) {
        // Ensure we don't read past the buffer
        if (filter_len >= sizeof(filter_str)) {
            filter_len = sizeof(filter_str) - 1;
        }
        memcpy(filter_str, Data + 4, filter_len);
        filter_str[filter_len] = '\0';
    } else {
        strcpy(filter_str, "host 127.0.0.1");
    }

    // 1. Test pcap_open_dead
    pcap_t *handle = pcap_open_dead(linktype, snaplen);
    if (handle == NULL) {
        // Invalid linktype, try with a valid one to proceed
        handle = pcap_open_dead(DLT_EN10MB, snaplen);
        if (handle == NULL) {
            return 0;
        }
    }

    // 2. Test pcap_compile
    struct bpf_program fp;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    memset(&fp, 0, sizeof(fp));

    int ret = pcap_compile(handle, &fp, filter_str, optimize, net);
    if (ret == 0) {
        // 3. Test pcap_setfilter
        pcap_setfilter(handle, &fp);
        cleanup_bpf_program(&fp);
    }

    // 4. Test pcap_lookupnet
    // Use a dummy device name or part of the input
    char device[64];
    size_t dev_len = 0;
    
    if (Size > 8) {
        // Calculate available bytes for device name
        size_t available = Size - 8;
        if (available >= sizeof(device)) {
            dev_len = sizeof(device) - 1;
        } else {
            dev_len = available;
        }
        memcpy(device, Data + 8, dev_len);
        device[dev_len] = '\0';
    } else {
        strcpy(device, "any");
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_lookupnet(device, &net, &mask, errbuf);

    // 5. Test pcap_nametonetaddr
    // Use the filter string or a part of it as a network name
    bpf_u_int32 addr = pcap_nametonetaddr(filter_str);
    (void)addr;

    // 6. Test pcap_compile_nopcap
    struct bpf_program fp_nopcap;
    memset(&fp_nopcap, 0, sizeof(fp_nopcap));
    int ret_nopcap = pcap_compile_nopcap(linktype, snaplen, &fp_nopcap, filter_str, optimize, net);
    if (ret_nopcap == 0) {
        cleanup_bpf_program(&fp_nopcap);
    }

    // Cleanup
    cleanup_pcap(handle);

    return 0;
}