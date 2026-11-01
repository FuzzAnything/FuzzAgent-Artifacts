// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    int ret;
    pcap_t *handle = NULL;
    char *fuzz_device = NULL;

    // 1. pcap_findalldevs
    ret = pcap_findalldevs(&alldevs, errbuf);
    
    // 2. pcap_freealldevs
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. Prepare device name
    const char *device_name = "any";
    
    if (Size > 0) {
        size_t len = Size;
        // Find first null byte or limit to Size
        for (size_t i = 0; i < Size; i++) {
            if (Data[i] == 0) {
                len = i;
                break;
            }
        }
        
        if (len > 0) {
            fuzz_device = (char *)malloc(len + 1);
            if (fuzz_device) {
                memcpy(fuzz_device, Data, len);
                fuzz_device[len] = '\0';
                
                // Check if it looks like a device name (printable)
                int is_valid = 1;
                for (size_t i = 0; i < len; i++) {
                    if (fuzz_device[i] < 32 || fuzz_device[i] > 126) {
                        is_valid = 0;
                        break;
                    }
                }
                
                if (is_valid) {
                    device_name = fuzz_device;
                } else {
                    free(fuzz_device);
                    fuzz_device = NULL;
                    device_name = "any";
                }
            } else {
                device_name = "any";
            }
        } else {
            device_name = "any";
        }
    }

    // 4. pcap_create
    handle = pcap_create(device_name, errbuf);

    // Free the fuzz_device if we allocated it, as pcap_create likely strdup'd it or we are done with our copy
    if (fuzz_device != NULL) {
        free(fuzz_device);
        fuzz_device = NULL;
    }

    if (handle == NULL) {
        return 0;
    }

    // 5. pcap_set_snaplen
    int snaplen = 64;
    if (Size >= 2) {
        snaplen = (Data[0] << 8) | Data[1];
        if (snaplen <= 0) snaplen = 64;
        if (snaplen > 65535) snaplen = 65535;
    }
    ret = pcap_set_snaplen(handle, snaplen);
    (void)ret;

    // 6. pcap_statustostr
    const char *status_str = pcap_statustostr(ret);
    (void)status_str;

    // 7. pcap_set_promisc
    int promisc = (Size > 2) ? (Data[2] % 2) : 0;
    ret = pcap_set_promisc(handle, promisc);
    
    // 8. pcap_statustostr again
    status_str = pcap_statustostr(ret);
    (void)status_str;

    // 9. pcap_set_rfmon
    int rfmon = (Size > 3) ? (Data[3] % 2) : 0;
    ret = pcap_set_rfmon(handle, rfmon);
    (void)ret;

    // Cleanup: Always close the handle if it was successfully created
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}