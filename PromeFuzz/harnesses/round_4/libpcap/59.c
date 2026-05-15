// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <pcap/pcap.h>

static void fuzz_pcap_setsampling(pcap_t *pcap) {
    struct pcap_samp *samp = pcap_setsampling(pcap);
    (void)samp; /* Ignore return value */
}

static void fuzz_pcap_set_buffer_size(pcap_t *pcap, int buffer_size) {
    int ret = pcap_set_buffer_size(pcap, buffer_size);
    (void)ret; /* Ignore return value */
}

static void fuzz_pcap_set_timeout(pcap_t *pcap, int timeout) {
    int ret = pcap_set_timeout(pcap, timeout);
    (void)ret; /* Ignore return value */
}

static void fuzz_pcap_set_snaplen(pcap_t *pcap, int snaplen) {
    int ret = pcap_set_snaplen(pcap, snaplen);
    (void)ret; /* Ignore return value */
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Create dummy device name from fuzz data */
    char device[256];
    size_t device_len = Size < 255 ? Size : 255;
    memcpy(device, Data, device_len);
    device[device_len] = '\0';

    /* Create error buffer */
    char errbuf[PCAP_ERRBUF_SIZE];
    errbuf[0] = '\0';

    /* Create pcap handle using pcap_create (unactivated) */
    pcap_t *pcap = pcap_create(device, errbuf);
    if (pcap != NULL) {
        /* Test functions that work on unactivated handles */
        fuzz_pcap_set_buffer_size(pcap, (int)Size);
        fuzz_pcap_set_timeout(pcap, (int)Size);
        fuzz_pcap_set_snaplen(pcap, (int)Size);
        
        /* Test pcap_setsampling */
        fuzz_pcap_setsampling(pcap);
        
        pcap_close(pcap);
    }

    /* Test pcap_open_live */
    pcap_t *pcap_live = pcap_open_live(device, (int)Size, (int)Size & 1, 
                                       (int)Size % 1000, errbuf);
    if (pcap_live != NULL) {
        /* Test functions on activated handle (should fail for some) */
        fuzz_pcap_set_buffer_size(pcap_live, (int)Size);
        fuzz_pcap_set_timeout(pcap_live, (int)Size);
        fuzz_pcap_set_snaplen(pcap_live, (int)Size);
        fuzz_pcap_setsampling(pcap_live);
        
        pcap_close(pcap_live);
    }

    /* Test pcap_open with remote authentication */
    struct pcap_rmtauth auth;
    auth.type = (int)Size % 3;
    
    /* Create username and password from fuzz data */
    char username[256];
    char password[256];
    size_t auth_len = Size < 128 ? Size : 128;
    memcpy(username, Data, auth_len);
    username[auth_len] = '\0';
    memcpy(password, Data + (Size > auth_len ? auth_len : 0), 
           Size > auth_len ? auth_len : 0);
    password[auth_len] = '\0';
    
    auth.username = username;
    auth.password = password;

    pcap_t *pcap_open_handle = pcap_open(device, (int)Size, (int)Size & 0xFF,
                                         (int)Size % 1000, &auth, errbuf);
    if (pcap_open_handle != NULL) {
        fuzz_pcap_set_buffer_size(pcap_open_handle, (int)Size);
        fuzz_pcap_set_timeout(pcap_open_handle, (int)Size);
        fuzz_pcap_set_snaplen(pcap_open_handle, (int)Size);
        fuzz_pcap_setsampling(pcap_open_handle);
        
        pcap_close(pcap_open_handle);
    }

    /* Test with NULL auth */
    pcap_t *pcap_open_null_handle = pcap_open(device, (int)Size, (int)Size & 0xFF,
                                              (int)Size % 1000, NULL, errbuf);
    if (pcap_open_null_handle != NULL) {
        fuzz_pcap_set_buffer_size(pcap_open_null_handle, (int)Size);
        fuzz_pcap_set_timeout(pcap_open_null_handle, (int)Size);
        fuzz_pcap_set_snaplen(pcap_open_null_handle, (int)Size);
        fuzz_pcap_setsampling(pcap_open_null_handle);
        
        pcap_close(pcap_open_null_handle);
    }

    return 0;
}