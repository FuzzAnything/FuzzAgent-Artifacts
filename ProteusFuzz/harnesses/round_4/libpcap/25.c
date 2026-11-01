// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
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

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct bpf_program fp;
    const char *filter_str = "host 127.0.0.1";
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    int optimize = 0;

    // Ensure minimum size to avoid trivial crashes on parsing logic if needed
    if (Size < 1) {
        return 0;
    }

    // 1. pcap_statustostr
    // Use data from input to generate a status code, or default to known constants
    int status_code = Data[0];
    // Map arbitrary byte to known error codes for better coverage
    // Using only standard PCAP_ERROR_* constants available in most libpcap versions
    if (status_code == 0) status_code = PCAP_ERROR;
    else if (status_code == 1) status_code = PCAP_ERROR_BREAK;
    else if (status_code == 2) status_code = PCAP_ERROR_PERM_DENIED;
    else if (status_code == 3) status_code = PCAP_ERROR_IFACE_NOT_UP;
    else if (status_code == 4) status_code = PCAP_ERROR_NO_SUCH_DEVICE;
    else if (status_code == 5) status_code = PCAP_ERROR_NOT_ACTIVATED;
    // Removed PCAP_ERROR_NOT_TETHERED, PCAP_ERROR_NO_SUCH_FILE, PCAP_ERROR_CANTSET 
    // as they may not be defined in all libpcap versions or headers.
    // Fallback to generic PCAP_ERROR for other values to ensure compilation.
    else status_code = PCAP_ERROR;
    
    const char *msg1 = pcap_statustostr(status_code);
    (void)msg1; // Suppress unused warning

    // 2. pcap_geterr (on NULL handle - should be safe or handled by libpcap implementation)
    char *err1 = pcap_geterr(NULL);
    (void)err1;

    // 3. pcap_statustostr again with different code
    int status_code2 = (Data[0] + 10) % 100;
    // Ensure status_code2 is a valid known error code to avoid undefined behavior if libpcap expects specific values
    // Though pcap_statustostr usually handles unknown codes gracefully, we map to PCAP_ERROR for safety.
    if (status_code2 == 0) status_code2 = PCAP_ERROR;
    else if (status_code2 == 1) status_code2 = PCAP_ERROR_BREAK;
    else if (status_code2 == 2) status_code2 = PCAP_ERROR_PERM_DENIED;
    else if (status_code2 == 3) status_code2 = PCAP_ERROR_IFACE_NOT_UP;
    else if (status_code2 == 4) status_code2 = PCAP_ERROR_NO_SUCH_DEVICE;
    else if (status_code2 == 5) status_code2 = PCAP_ERROR_NOT_ACTIVATED;
    else status_code2 = PCAP_ERROR;

    const char *msg2 = pcap_statustostr(status_code2);
    (void)msg2;

    // 4. pcap_geterr again
    char *err2 = pcap_geterr(NULL);
    (void)err2;

    // 5. pcap_open_live
    // Use "any" or a dummy interface. "any" is usually safe for testing.
    int snaplen = 65535;
    int promisc = (Data[0] % 2);
    int to_ms = 1000;
    
    handle = pcap_open_live("any", snaplen, promisc, to_ms, errbuf);
    
    if (handle == NULL) {
        // If we can't open live capture (common in fuzzing envs without root),
        // we can't proceed with the rest of the API chain that requires a valid handle.
        return 0;
    }

    // 6. pcap_fileno
    int fd = pcap_fileno(handle);
    (void)fd;

    // 7. pcap_compile
    memset(&fp, 0, sizeof(fp));
    
    const char *fuzz_filter = filter_str;
    if (Size > 0 && Data[Size-1] == 0) {
        fuzz_filter = (const char *)Data;
    }

    int compile_ret = pcap_compile(handle, &fp, fuzz_filter, optimize, net);
    
    if (compile_ret == -1) {
        // Compilation failed
    }

    // 8. pcap_geterr
    char *err3 = pcap_geterr(handle);
    (void)err3;

    // 9. pcap_setfilter
    if (compile_ret == 0) {
        int setfilter_ret = pcap_setfilter(handle, &fp);
        (void)setfilter_ret;
        
        // 10. pcap_geterr after setfilter
        char *err4 = pcap_geterr(handle);
        (void)err4;
    } else {
        if (fp.bf_insns) {
            cleanup_bpf_program(&fp);
        }
    }

    // Cleanup bpf_program if it was successfully compiled
    if (compile_ret == 0) {
        cleanup_bpf_program(&fp);
    }

    // 11. pcap_close
    if (handle) {
        pcap_close(handle);
    }

    return 0;
}