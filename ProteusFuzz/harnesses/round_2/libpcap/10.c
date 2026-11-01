// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_bpf_program(struct bpf_program *fp) {
    if (fp && fp->bf_insns) {
        pcap_freecode(fp);
        fp->bf_insns = NULL;
        fp->bf_len = 0;
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    pcap_t *handle = NULL;
    int snaplen = 65535;
    int promisc = 0;
    int to_ms = 1000;
    int optimize = 1;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;

    // Initialize bpf_program
    memset(&fp, 0, sizeof(fp));

    // Step 1: pcap_statustostr
    if (Size > 0) {
        int status_code = Data[0];
        const char *status_str = pcap_statustostr(status_code);
        (void)status_str;
    }

    // Step 2: pcap_geterr (on NULL handle)
    char *err = pcap_geterr(NULL);
    (void)err;

    // Step 3: pcap_statustostr again
    if (Size > 1) {
        int status_code2 = Data[1];
        const char *status_str2 = pcap_statustostr(status_code2);
        (void)status_str2;
    }

    // Step 4: pcap_geterr again
    err = pcap_geterr(NULL);
    (void)err;

    // Step 5: pcap_open_live
    // Ensure we have enough data before accessing indices 2, 3, 4, 5, 6
    if (Size >= 7) {
        snaplen = (Data[2] << 8) | Data[3];
        if (snaplen == 0) snaplen = 65535;
        promisc = (Data[4] % 2);
        to_ms = (Data[5] << 8) | Data[6];
        if (to_ms == 0) to_ms = 1000;
    }

    // Try opening with "any"
    handle = pcap_open_live("any", snaplen, promisc, to_ms, errbuf);
    if (handle == NULL) {
        // Try opening with NULL as device name
        handle = pcap_open_live(NULL, snaplen, promisc, to_ms, errbuf);
    }

    if (handle == NULL) {
        // If we still don't have a handle, we can't proceed with the rest of the sequence effectively.
        // We can still call pcap_geterr to see the error.
        err = pcap_geterr(handle);
        (void)err;
        return 0;
    }

    // Step 6: pcap_fileno
    int fd = pcap_fileno(handle);
    (void)fd;

    // Step 7: pcap_compile
    const char *simple_filter = "host 127.0.0.1";
    int compile_result = pcap_compile(handle, &fp, simple_filter, optimize, net);
    
    if (compile_result == -1) {
        // Try with a filter string derived from Data if Size is sufficient
        if (Size > 0 && Size < 256) {
            char *filter_buf = (char *)malloc(Size + 1);
            if (filter_buf) {
                memcpy(filter_buf, Data, Size);
                filter_buf[Size] = '\0';
                // Try to compile with this filter
                compile_result = pcap_compile(handle, &fp, filter_buf, optimize, net);
                free(filter_buf);
            }
        }
    }

    // Step 8: pcap_geterr
    err = pcap_geterr(handle);
    (void)err;

    // Step 9: pcap_setfilter
    if (compile_result == 0) {
        int setfilter_result = pcap_setfilter(handle, &fp);
        (void)setfilter_result;
    }

    // Step 10: pcap_geterr
    err = pcap_geterr(handle);
    (void)err;

    // Cleanup BPF program
    cleanup_bpf_program(&fp);

    // Step 11: pcap_close
    pcap_close(handle);
    handle = NULL;

    return 0;
}