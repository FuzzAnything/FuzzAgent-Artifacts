// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

/*
 * Fuzz driver for libpcap API functions.
 * Target APIs: pcap_perror, pcap_init, pcap_geterr, pcap_statustostr,
 * pcap_strerror, pcap_setnonblock.
 */

static void write_dummy_file(const uint8_t *data, size_t size) {
    if (size == 0) return;
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE + 1];
    pcap_t *handle = NULL;
    int ret;
    const char *str;
    char *err_str;

    /* Initialize error buffer */
    memset(errbuf, 0, sizeof(errbuf));

    /* 1. Test pcap_init with various encodings derived from input */
    if (Size > 0) {
        unsigned int enc_mode = Data[0] % 2; /* 0 or 1 */
        ret = pcap_init(enc_mode, errbuf);
        (void)ret;
    }
    /* Call pcap_init again to test conflicting mode handling */
    if (Size > 1) {
        unsigned int enc_mode2 = (Data[1] % 2) ^ 1; /* Flip the previous mode */
        ret = pcap_init(enc_mode2, errbuf);
        (void)ret;
    }

    /* 2. Try to open a pcap handle to test functions requiring a handle */
    if (Size > 0) {
        write_dummy_file(Data, Size);
        handle = pcap_open_offline("./dummy_file", errbuf);
    }

    /* 3. Test pcap_geterr */
    if (handle) {
        err_str = pcap_geterr(handle);
        (void)err_str;
    }

    /* 4. Test pcap_perror */
    if (handle) {
        pcap_perror(handle, "FuzzTest");
    }

    /* 5. Test pcap_statustostr with various status codes derived from input */
    if (Size > 3) {
        int status_code = (int)(Data[2] + Data[3]);
        str = pcap_statustostr(status_code);
        (void)str;
    }
    /* Test with negative and large values */
    str = pcap_statustostr(-1);
    (void)str;
    str = pcap_statustostr(9999);
    (void)str;

    /* 6. Test pcap_strerror with various error numbers */
    if (Size > 5) {
        int err_num = (int)(Data[4] + Data[5]);
        str = pcap_strerror(err_num);
        (void)str;
    }
    /* Test with standard error numbers */
    str = pcap_strerror(0);
    (void)str;
    str = pcap_strerror(1);
    (void)str;

    /* 7. Test pcap_setnonblock */
    if (handle) {
        if (Size > 6) {
            int nonblock_mode = (Data[6] % 2); /* 0 or 1 */
            ret = pcap_setnonblock(handle, nonblock_mode, errbuf);
            (void)ret;
        }
        /* Try both modes explicitly */
        ret = pcap_setnonblock(handle, 1, errbuf);
        (void)ret;
        ret = pcap_setnonblock(handle, 0, errbuf);
        (void)ret;
    }

    /* Cleanup */
    if (handle) {
        pcap_close(handle);
    }

    /* Clean up dummy file */
    remove("./dummy_file");

    return 0;
}