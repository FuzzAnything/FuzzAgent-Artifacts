// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    int rfmon_result;
    const char *err_str;
    char *pcap_err;
    int status_code;
    
    // Use first byte to determine device name strategy
    uint8_t strategy = Size > 0 ? Data[0] % 3 : 0;
    const char *device = NULL;
    
    if (strategy == 0) {
        // NULL device (any device)
        device = NULL;
    } else if (strategy == 1 && Size > 1) {
        // Use fuzzer data as device name (truncate at null byte or size)
        size_t max_len = Size - 1 < 255 ? Size - 1 : 255;
        char *dev_name = (char *)malloc(max_len + 1);
        if (dev_name) {
            memcpy(dev_name, Data + 1, max_len);
            dev_name[max_len] = '\0';
            device = dev_name;
            // Create pcap handle
            pcap_handle = pcap_create(device, errbuf);
            free(dev_name);
        }
    } else {
        // Fixed dummy device name
        device = "eth0";
    }
    
    // If pcap_handle not created yet (cases 0 and 2), create it now
    if (!pcap_handle) {
        pcap_handle = pcap_create(device, errbuf);
    }
    
    // Call pcap_can_set_rfmon if handle was created
    if (pcap_handle) {
        rfmon_result = pcap_can_set_rfmon(pcap_handle);
        
        // Call pcap_geterr regardless of rfmon_result
        pcap_err = pcap_geterr(pcap_handle);
        (void)pcap_err; // Use result to avoid unused variable warning
    }
    
    // Call pcap_statustostr with various status codes derived from fuzzer input
    if (Size > 0) {
        // Use fuzzer data to generate status codes
        for (size_t i = 0; i < Size && i < 8; i++) {
            status_code = (int)Data[i];
            // Map to common PCAP error codes
            switch (status_code % 8) {
                case 0: status_code = PCAP_ERROR; break;
                case 1: status_code = PCAP_ERROR_BREAK; break;
                case 2: status_code = PCAP_ERROR_NOT_ACTIVATED; break;
                case 3: status_code = PCAP_ERROR_ACTIVATED; break;
                case 4: status_code = PCAP_ERROR_NO_SUCH_DEVICE; break;
                case 5: status_code = PCAP_ERROR_RFMON_NOTSUP; break;
                case 6: status_code = PCAP_ERROR_PERM_DENIED; break;
                case 7: status_code = PCAP_ERROR_PROMISC_PERM_DENIED; break;
            }
            err_str = pcap_statustostr(status_code);
            (void)err_str; // Use result to avoid unused variable warning
        }
    }
    
    // Clean up
    cleanup_pcap(pcap_handle);
    
    return 0;
}