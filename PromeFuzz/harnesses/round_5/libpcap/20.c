// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_init at pcap.c:223:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <unistd.h>
#include <fcntl.h>

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    uint8_t op = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;

    switch (op) {
        case 0: {
            /* Test pcap_set_snaplen */
            pcap_t *pcap = pcap_create("any", errbuf);
            if (!pcap) {
                return 0;
            }
            
            int snaplen = 0;
            if (fuzz_size >= sizeof(int)) {
                memcpy(&snaplen, fuzz_data, sizeof(int));
            } else if (fuzz_size > 0) {
                snaplen = fuzz_data[0];
            }
            
            pcap_set_snaplen(pcap, snaplen);
            cleanup_pcap(pcap);
            break;
        }
        
        case 1: {
            /* Test pcap_lookupnet */
            bpf_u_int32 net = 0, mask = 0;
            char device_name[256] = {0};
            
            if (fuzz_size > 0) {
                size_t name_len = fuzz_size < 255 ? fuzz_size : 255;
                memcpy(device_name, fuzz_data, name_len);
                device_name[name_len] = '\0';
            } else {
                strcpy(device_name, "any");
            }
            
            pcap_lookupnet(device_name, &net, &mask, errbuf);
            break;
        }
        
        case 2: {
            /* Test pcap_init */
            unsigned int options = 0;
            if (fuzz_size >= sizeof(unsigned int)) {
                memcpy(&options, fuzz_data, sizeof(unsigned int));
            } else if (fuzz_size > 0) {
                options = fuzz_data[0];
            }
            
            pcap_init(options, errbuf);
            break;
        }
        
        case 3: {
            /* Test pcap_open */
            char source[512] = {0};
            int snaplen = 65535;
            int flags = 0;
            int read_timeout = 1000;
            
            if (fuzz_size > 0) {
                size_t source_len = fuzz_size < 511 ? fuzz_size : 511;
                memcpy(source, fuzz_data, source_len);
                source[source_len] = '\0';
            } else {
                strcpy(source, "any");
            }
            
            pcap_t *pcap = pcap_open(source, snaplen, flags, read_timeout, NULL, errbuf);
            cleanup_pcap(pcap);
            break;
        }
        
        case 4: {
            /* Test pcap_open_live */
            char device[256] = {0};
            int snaplen = 65535;
            int promisc = 0;
            int timeout = 1000;
            
            if (fuzz_size > 0) {
                size_t device_len = fuzz_size < 255 ? fuzz_size : 255;
                memcpy(device, fuzz_data, device_len);
                device[device_len] = '\0';
            } else {
                strcpy(device, "any");
            }
            
            pcap_t *pcap = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
            cleanup_pcap(pcap);
            break;
        }
        
        case 5: {
            /* Test pcap_set_timeout */
            pcap_t *pcap = pcap_create("any", errbuf);
            if (!pcap) {
                return 0;
            }
            
            int timeout = 0;
            if (fuzz_size >= sizeof(int)) {
                memcpy(&timeout, fuzz_data, sizeof(int));
            } else if (fuzz_size > 0) {
                timeout = fuzz_data[0];
            }
            
            pcap_set_timeout(pcap, timeout);
            cleanup_pcap(pcap);
            break;
        }
    }
    
    return 0;
}