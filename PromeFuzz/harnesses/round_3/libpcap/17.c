// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle;
    int rfmon_status;
    const char *err_str;
    const char *status_str;
    int status_code;
    
    // Use fuzz input to generate device name (if any)
    char *device_name = NULL;
    if (Size > 0) {
        // Create a null-terminated device name from fuzz data
        size_t name_len = Size < PCAP_ERRBUF_SIZE ? Size : PCAP_ERRBUF_SIZE - 1;
        device_name = (char *)malloc(name_len + 1);
        if (device_name) {
            memcpy(device_name, Data, name_len);
            device_name[name_len] = '\0';
        }
    }
    
    // 1. pcap_create
    pcap_handle = pcap_create(device_name, errbuf);
    
    // 2. pcap_can_set_rfmon (only if pcap_create succeeded)
    if (pcap_handle != NULL) {
        rfmon_status = pcap_can_set_rfmon(pcap_handle);
        
        // 3. pcap_geterr (call it even if no error occurred to test the API)
        err_str = pcap_geterr(pcap_handle);
        
        // Clean up the pcap handle
        pcap_close(pcap_handle);
    }
    
    // 4. pcap_statustostr - test with various status codes from fuzz input
    if (Size >= sizeof(int)) {
        // Use the first sizeof(int) bytes as status code
        memcpy(&status_code, Data, sizeof(int));
    } else if (Size > 0) {
        // Use the first byte as status code
        status_code = (int)Data[0];
    } else {
        // Test with some common error codes if no data
        status_code = PCAP_ERROR;
    }
    
    // Also test with some known status codes
    status_str = pcap_statustostr(status_code);
    
    // Test with additional hardcoded status codes to explore more states
    const int test_codes[] = {
        PCAP_ERROR,
        PCAP_ERROR_BREAK,
        PCAP_ERROR_NOT_ACTIVATED,
        PCAP_ERROR_ACTIVATED,
        PCAP_ERROR_NO_SUCH_DEVICE,
        PCAP_ERROR_RFMON_NOTSUP,
        PCAP_ERROR_NOT_RFMON,
        PCAP_ERROR_PERM_DENIED,
        PCAP_ERROR_IFACE_NOT_UP,
        PCAP_ERROR_CANTSET_TSTAMP_TYPE,
        PCAP_ERROR_PROMISC_PERM_DENIED,
        PCAP_ERROR_TSTAMP_PRECISION_NOTSUP,
        PCAP_WARNING,
        PCAP_WARNING_PROMISC_NOTSUP,
        PCAP_WARNING_TSTAMP_TYPE_NOTSUP
    };
    
    for (size_t i = 0; i < sizeof(test_codes)/sizeof(test_codes[0]); i++) {
        status_str = pcap_statustostr(test_codes[i]);
    }
    
    // Clean up
    if (device_name) {
        free(device_name);
    }
    
    return 0;
}