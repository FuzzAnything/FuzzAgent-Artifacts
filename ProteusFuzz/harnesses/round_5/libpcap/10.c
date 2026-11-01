// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_fileno at pcap.c:3522:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    pcap_t *handle = NULL;
    int fd;
    int ret;

    // 1. pcap_statustostr
    int status_code = 0;
    if (Size > 0) {
        status_code = Data[0] % 100;
    }
    const char *status_str = pcap_statustostr(status_code);
    (void)status_str;

    // 2. pcap_geterr
    // Skip calling with NULL to prevent potential crashes, as no handle exists yet.

    // 3. pcap_statustostr again
    int status_code2 = 0;
    if (Size > 1) {
        status_code2 = Data[1] % 100;
    }
    const char *status_str2 = pcap_statustostr(status_code2);
    (void)status_str2;

    // 4. pcap_geterr again
    // Skip calling with NULL.

    // 5. pcap_open_live
    int snaplen = 65535;
    int promisc = 1;
    int to_ms = 1000;
    
    if (Size > 2) {
        snaplen = 65535 - (Data[2] % 1000);
        if (Size > 3) {
            promisc = Data[3] % 2;
        }
        if (Size > 4) {
            to_ms = 100 + (Data[4] % 1000);
        }
    }

    handle = pcap_open_live(NULL, snaplen, promisc, to_ms, errbuf);
    
    if (handle != NULL) {
        // 6. pcap_fileno
        fd = pcap_fileno(handle);
        (void)fd;

        // 7. pcap_compile
        const char *filter_str = "host 127.0.0.1";
        if (Size > 5) {
            char safe_filter[256];
            size_t len = Size > 250 ? 250 : Size;
            memcpy(safe_filter, Data, len);
            safe_filter[len] = '\0';
            filter_str = safe_filter;
        }

        bpf_u_int32 net = 0;
        bpf_u_int32 mask = 0;
        if (Size > 10) {
            net = Data[6];
            mask = Data[7] << 24 | Data[8] << 16 | Data[9] << 8 | Data[10];
        }

        ret = pcap_compile(handle, &fp, filter_str, 0, net);
        if (ret == -1) {
            // Compilation failed
        }

        // 8. pcap_geterr
        char *err_msg3 = pcap_geterr(handle);
        (void)err_msg3;

        // 9. pcap_setfilter
        ret = pcap_setfilter(handle, &fp);
        (void)ret;

        // 10. pcap_geterr
        char *err_msg4 = pcap_geterr(handle);
        (void)err_msg4;

        // 11. pcap_close
        pcap_close(handle);
    } else {
        // Handle is NULL, skip functions requiring a valid handle.
    }

    // Write dummy file
    write_dummy_file(Data, Size);

    return 0;
}