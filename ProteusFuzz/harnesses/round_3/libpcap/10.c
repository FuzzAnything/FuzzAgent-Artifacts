// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Step 1: Invoke pcap_statustostr with a fuzzed status code
    int status_code = (int)Data[0];
    const char *str1 = pcap_statustostr(status_code);
    (void)str1;

    // Step 2: Invoke pcap_geterr with a NULL handle.
    // Note: pcap_geterr(NULL) is undefined behavior in some versions, but we test robustness.
    // If the library handles it gracefully, good. If not, it's a bug.
    // However, to avoid immediate crashes that stop fuzzing, we might skip if we know it's unsafe.
    // But the task requires invoking in order. We will call it.
    char *err1 = pcap_geterr(NULL);
    (void)err1;

    // Step 3: Invoke pcap_statustostr again
    int status_code2 = (int)Data[1];
    const char *str2 = pcap_statustostr(status_code2);
    (void)str2;

    // Step 4: Invoke pcap_geterr again
    char *err2 = pcap_geterr(NULL);
    (void)err2;

    // Step 5: Invoke pcap_open_live
    const char *device = NULL; // Default to "any"
    int snaplen = (Size > 3) ? (int)Data[3] : 65535;
    int promisc = (Size > 4) ? (int)Data[4] : 0;
    int timeout = (Size > 5) ? (int)Data[5] : 1000;
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    pcap_t *handle = pcap_open_live(device, snaplen, promisc, timeout, errbuf);

    // If handle is NULL, we cannot safely call pcap_fileno, pcap_compile, etc.
    // We will skip those steps that require a valid handle to avoid crashes.
    // But we still need to invoke pcap_geterr if possible.
    // However, pcap_geterr on NULL is already done.
    
    // Step 6: Invoke pcap_fileno only if handle is valid
    int fd = -1;
    if (handle != NULL) {
        fd = pcap_fileno(handle);
        (void)fd;
    }

    // Step 7: Invoke pcap_compile only if handle is valid
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    const char *filter_str = "host 127.0.0.1";
    int compile_result = -1;
    if (handle != NULL) {
        compile_result = pcap_compile(handle, &fp, filter_str, 0, net);
    }
    (void)compile_result;

    // Step 8: Invoke pcap_geterr
    char *err3 = NULL;
    if (handle != NULL) {
        err3 = pcap_geterr(handle);
    } else {
        err3 = pcap_geterr(NULL);
    }
    (void)err3;

    // Step 9: Invoke pcap_setfilter only if handle is valid and compile succeeded
    int setfilter_result = -1;
    if (handle != NULL && compile_result == 0) {
        setfilter_result = pcap_setfilter(handle, &fp);
    }
    (void)setfilter_result;

    // Step 10: Invoke pcap_geterr
    char *err4 = NULL;
    if (handle != NULL) {
        err4 = pcap_geterr(handle);
    } else {
        err4 = pcap_geterr(NULL);
    }
    (void)err4;

    // Step 11: Invoke pcap_close
    if (handle != NULL) {
        pcap_close(handle);
    }

    // Cleanup bpf_program if compiled successfully
    if (compile_result == 0 && fp.bf_insns != NULL) {
        pcap_freecode(&fp);
    }

    return 0;
}