// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <errno.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int ret;
    int nonblock_mode;
    int init_mode;
    int error_code;
    int status_code;

    if (Size == 0) {
        return 0;
    }

    // Fuzz pcap_init
    // Use first byte to determine init mode
    init_mode = Data[0] & 0x01;
    memset(errbuf, 0, sizeof(errbuf));
    ret = pcap_init(init_mode, errbuf);
    (void)ret;

    // Fuzz pcap_strerror
    // Use second byte as error code if available
    if (Size >= 2) {
        error_code = Data[1];
        const char *str_err = pcap_strerror(error_code);
        (void)str_err;
    } else {
        // Fallback if size is too small
        const char *str_err = pcap_strerror(0);
        (void)str_err;
    }

    // Fuzz pcap_statustostr
    // Use third byte as status code if available
    if (Size >= 3) {
        status_code = Data[2];
        const char *str_status = pcap_statustostr(status_code);
        (void)str_status;
    } else {
        // Fallback
        const char *str_status = pcap_statustostr(0);
        (void)str_status;
    }

    // Prepare for pcap_open_live or pcap_open_offline
    // Try to open a file if size is large enough to simulate a pcap file
    // Or try to open a live interface (which might fail, but that's okay for fuzzing)
    if (Size >= 4) {
        // Try to open offline from dummy file
        write_dummy_file(Data, Size);
        handle = pcap_open_offline("./dummy_file", errbuf);
        
        if (handle == NULL) {
            // Try to open live with "any" interface (common fallback)
            handle = pcap_open_live("any", 65535, 1, 1000, errbuf);
        }
    } else {
        // Try to open live directly
        handle = pcap_open_live("any", 65535, 1, 1000, errbuf);
    }

    if (handle != NULL) {
        // Fuzz pcap_setnonblock
        // Use fourth byte to determine nonblock mode (0 or 1)
        if (Size >= 4) {
            nonblock_mode = (Data[3] % 2);
            ret = pcap_setnonblock(handle, nonblock_mode, errbuf);
            (void)ret;
        } else {
            // Fallback
            ret = pcap_setnonblock(handle, 0, errbuf);
            (void)ret;
        }

        // Fuzz pcap_geterr
        char *err_msg = pcap_geterr(handle);
        (void)err_msg;

        // Fuzz pcap_perror
        // Use a prefix based on data if available
        char safe_prefix[16] = "Fuzz";
        if (Size >= 5) {
            size_t len = (Size - 5 < 15) ? Size - 5 : 15;
            memcpy(safe_prefix, Data + 4, len);
            safe_prefix[len] = '\0';
        }
        pcap_perror(handle, safe_prefix);

        // Cleanup
        pcap_close(handle);
        handle = NULL;
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}