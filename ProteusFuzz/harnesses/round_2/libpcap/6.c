// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program fp;
    pcap_dumper_t *dumper = NULL;
    const char *filter_str = "host 192.168.1.1";
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    int optimize = 0;

    memset(errbuf, 0, sizeof(errbuf));
    memset(&fp, 0, sizeof(fp));

    // Create a pcap handle. "lo" is a common loopback interface.
    // If this fails, we cannot proceed with the specific API chain.
    handle = pcap_create("lo", errbuf);
    if (handle == NULL) {
        return 0;
    }

    // Attempt to activate the handle.
    // pcap_compile requires an activated handle.
    // We ignore the return value to test error paths if activation fails.
    pcap_activate(handle);

    // 1. pcap_compile
    // Parses the filter string. May fail if handle is not activated or filter is invalid.
    int compile_ret = pcap_compile(handle, &fp, filter_str, optimize, net);
    
    // 2. pcap_geterr
    // Retrieves error message after pcap_compile.
    char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 3. pcap_setfilter
    // Applies the compiled filter. May fail if compile failed or handle is invalid.
    int setfilter_ret = pcap_setfilter(handle, &fp);
    
    // 4. pcap_geterr
    // Retrieves error message after pcap_setfilter.
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 5. pcap_dump_open
    // Creates a dumper. Requires activated handle and valid filename.
    dumper = pcap_dump_open(handle, "./dummy_file");
    
    // 6. pcap_geterr
    // Retrieves error message after pcap_dump_open.
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 7. pcap_datalink
    // Retrieves the link-layer type. Returns error if not activated.
    int linktype = pcap_datalink(handle);
    (void)linktype;

    // Cleanup
    if (dumper != NULL) {
        pcap_dump_close(dumper);
    }
    
    // Free the filter program if it was successfully compiled
    if (compile_ret == 0 && fp.bf_insns != NULL) {
        pcap_freecode(&fp);
    }

    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}