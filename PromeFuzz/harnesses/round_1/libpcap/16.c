// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    
    // Create device name from input data
    char device_name[256];
    size_t name_len = Size < 255 ? Size : 255;
    memcpy(device_name, Data, name_len);
    device_name[name_len] = '\0';
    
    // Vary the device parameter: sometimes NULL, sometimes from input
    const char *device = (Data[0] & 0x1) ? device_name : NULL;
    
    // Call pcap_create
    pcap_handle = pcap_create(device, errbuf);
    
    if (pcap_handle != NULL) {
        // Call pcap_can_set_rfmon
        int rfmon_result = pcap_can_set_rfmon(pcap_handle);
        
        // Call pcap_geterr regardless of result to test error buffer access
        char *error_msg = pcap_geterr(pcap_handle);
        (void)error_msg; // Use to avoid unused variable warning
        
        // Call pcap_statustostr with various error codes
        // Use input data to select error codes
        int error_codes[] = {
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
            PCAP_ERROR_TSTAMP_PRECISION_NOTSUP
        };
        
        size_t num_codes = sizeof(error_codes) / sizeof(error_codes[0]);
        for (size_t i = 0; i < num_codes && i < Size; i++) {
            int code_index = Data[i % Size] % num_codes;
            const char *status_str = pcap_statustostr(error_codes[code_index]);
            (void)status_str; // Use to avoid unused variable warning
        }
        
        // Also test with the rfmon_result if it's negative
        if (rfmon_result < 0) {
            const char *rfmon_status = pcap_statustostr(rfmon_result);
            (void)rfmon_status;
        }
        
        // Clean up
        pcap_close(pcap_handle);
    } else {
        // Test pcap_statustostr with some error codes even when create failed
        const char *status_str = pcap_statustostr(PCAP_ERROR);
        (void)status_str;
    }
    
    // Test with different device names and error buffer states
    // Create another handle with different parameters
    if (Size > 10) {
        char device_name2[256];
        size_t name_len2 = (Size / 2) < 255 ? (Size / 2) : 255;
        memcpy(device_name2, Data + (Size / 2), name_len2);
        device_name2[name_len2] = '\0';
        
        pcap_t *pcap_handle2 = pcap_create(device_name2, errbuf);
        if (pcap_handle2 != NULL) {
            int rfmon_result2 = pcap_can_set_rfmon(pcap_handle2);
            (void)rfmon_result2;
            
            char *error_msg2 = pcap_geterr(pcap_handle2);
            (void)error_msg2;
            
            pcap_close(pcap_handle2);
        }
    }
    
    // Create a dummy file for potential file operations
    write_dummy_file(Data, Size > 1024 ? 1024 : Size);
    
    return 0;
}