// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static void static_free_bpf_program(struct bpf_program *fp) {
    if (fp && fp->bf_insns) {
        free(fp->bf_insns);
        fp->bf_insns = NULL;
        fp->bf_len = 0;
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the input data for use as network names or filter strings
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test pcap_nametonetaddr
    // Converts a network name to its numeric address.
    bpf_u_int32 net_addr = pcap_nametonetaddr(input_str);
    (void)net_addr; // Suppress unused variable warning

    // 2. Test pcap_lookupnet
    // Retrieves network address and netmask for a device.
    // We use a common device name "any" or the input string as device name.
    bpf_u_int32 addr, mask;
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device_name = (Size > 10) ? input_str : "any";
    int lookup_res = pcap_lookupnet(device_name, &addr, &mask, errbuf);
    (void)lookup_res;

    // 3. Test pcap_nametoaddr (Deprecated but tested for robustness)
    // Resolves a hostname to an array of IP addresses.
    struct hostent *hostent = pcap_nametoaddr(input_str);
    (void)hostent;

    // 4. Test pcap_compile_nopcap
    // Compiles a BPF filter without a live capture handle.
    // We try different link types (DLT_EN10MB, DLT_RAW, etc.) based on input bytes.
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    int linktype = DLT_EN10MB;
    if (Size > 0) {
        linktype = (int)Data[0] % 100 + DLT_EN10MB; // Keep within reasonable range
    }
    
    int compile_nopcap_res = pcap_compile_nopcap(
        linktype,
        65535, // snaplen
        &fp,
        input_str, // filter string
        0, // optimize
        0  // netmask (0 usually means use default or ignore)
    );
    
    if (compile_nopcap_res == 0) {
        // If compilation succeeded, we should free the program
        static_free_bpf_program(&fp);
    }

    // 5. Test pcap_compile and pcap_setfilter with a real handle
    pcap_t *handle = pcap_open_dead(linktype, 65535);
    if (handle) {
        struct bpf_program fp2;
        memset(&fp2, 0, sizeof(fp2));
        
        // Attempt to compile
        int compile_res = pcap_compile(handle, &fp2, input_str, 0, 0);
        
        if (compile_res == 0) {
            // Attempt to set the filter
            pcap_setfilter(handle, &fp2);
            // Cleanup filter
            static_free_bpf_program(&fp2);
        }
        
        pcap_close(handle);
    }

    free(input_str);
    return 0;
}