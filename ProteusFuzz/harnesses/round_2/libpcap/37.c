// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_compile_nopcap at gencode.c:1271:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <namedb.h>
#include <arpa/inet.h>

static void safe_pcap_close(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void safe_bpf_freep(struct bpf_program *prog) {
    if (prog != NULL && prog->bf_insns != NULL) {
        pcap_freecode(prog);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from input for device/filter names
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. pcap_lookupnet
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    // Try the input as a device name
    pcap_lookupnet(input_str, &net, &mask, errbuf);
    // Try common pseudo-devices to explore different states
    pcap_lookupnet("any", &net, &mask, errbuf);
    pcap_lookupnet("lo", &net, &mask, errbuf);

    // 2. pcap_nametoaddr (Deprecated, but tested for fuzzing)
    struct hostent *he;
    he = pcap_nametoaddr(input_str);
    (void)he; // Suppress unused warning

    // 3. pcap_compile
    // We need a pcap_t handle. Use pcap_open_dead for a safe, non-live handle.
    pcap_t *handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (handle != NULL) {
        // Compile with optimization flag 0 or 1 based on input
        int optimize = (Size > 0 && Data[0] % 2) ? 1 : 0;
        struct bpf_program fp;
        memset(&fp, 0, sizeof(fp));
        
        int ret = pcap_compile(handle, &fp, input_str, optimize, net);
        if (ret == 0) {
            // If successful, try to set the filter
            ret = pcap_setfilter(handle, &fp);
            (void)ret;
            safe_bpf_freep(&fp);
        }
        
        // Try compiling with a different netmask (0)
        memset(&fp, 0, sizeof(fp));
        pcap_compile(handle, &fp, input_str, optimize, 0);
        safe_bpf_freep(&fp);

        safe_pcap_close(handle);
    }

    // 4. pcap_nametonetaddr
    bpf_u_int32 netaddr = pcap_nametonetaddr(input_str);
    (void)netaddr;

    // 5. pcap_compile_nopcap (Deprecated)
    struct bpf_program fp_nopcap;
    memset(&fp_nopcap, 0, sizeof(fp_nopcap));
    // Use DLT_EN10MB and a snapshot length derived from input
    int snaplen = (Size > 0) ? (Data[0] % 1000) + 1 : 1000;
    int linktype = DLT_EN10MB;
    // Re-define optimize here as it is out of scope from the previous block
    int optimize_nopcap = (Size > 0 && Data[0] % 2) ? 1 : 0;
    int ret_nopcap = pcap_compile_nopcap(linktype, snaplen, &fp_nopcap, input_str, optimize_nopcap, net);
    if (ret_nopcap == 0) {
        safe_bpf_freep(&fp_nopcap);
    }

    // 6. Additional exploration: Create a handle and try setfilter with invalid program
    pcap_t *handle2 = pcap_open_dead(DLT_RAW, 65535);
    if (handle2 != NULL) {
        struct bpf_program invalid_prog;
        invalid_prog.bf_len = 0;
        invalid_prog.bf_insns = NULL;
        pcap_setfilter(handle2, &invalid_prog);
        safe_pcap_close(handle2);
    }

    free(input_str);
    return 0;
}