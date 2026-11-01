// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program fp;
    const char *filter_str = "host 127.0.0.1";
    int optimize = 1;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;

    // Step 1: pcap_statustostr
    const char *status_str1 = pcap_statustostr(PCAP_ERROR);
    (void)status_str1;

    // Step 2: pcap_geterr (with NULL handle, might be undefined but safe to call for fuzzing)
    char *err1 = pcap_geterr(NULL);
    (void)err1;

    // Step 3: pcap_statustostr again
    const char *status_str2 = pcap_statustostr(PCAP_ERROR_BREAK);
    (void)status_str2;

    // Step 4: pcap_geterr again
    char *err2 = pcap_geterr(NULL);
    (void)err2;

    // Step 5: pcap_open_live
    // Use "any" or NULL for device name to attempt opening a live capture
    // Snapshot length, promiscuous flag, timeout, and error buffer are set
    handle = pcap_open_live(NULL, 65535, 1, 100, errbuf);
    if (handle == NULL) {
        // If opening fails, we can still test error handling paths
        // But we cannot proceed with pcap_fileno, pcap_compile, etc. on a NULL handle
        // We will return early to avoid undefined behavior
        return 0;
    }

    // Step 6: pcap_fileno
    int fd = pcap_fileno(handle);
    (void)fd;

    // Step 7: pcap_compile
    // We need a valid filter string. Using a simple one.
    // If the handle is not activated, this might fail, but pcap_open_live usually activates.
    int compile_result = pcap_compile(handle, &fp, filter_str, optimize, net);
    if (compile_result == -1) {
        // Compilation failed, we can still test error retrieval
    }

    // Step 8: pcap_geterr
    char *err3 = pcap_geterr(handle);
    (void)err3;

    // Step 9: pcap_setfilter
    // Only attempt if compilation was successful
    if (compile_result != -1) {
        int setfilter_result = pcap_setfilter(handle, &fp);
        (void)setfilter_result;
    }

    // Step 10: pcap_geterr
    char *err4 = pcap_geterr(handle);
    (void)err4;

    // Step 11: pcap_close
    pcap_close(handle);
    handle = NULL;

    return 0;
}