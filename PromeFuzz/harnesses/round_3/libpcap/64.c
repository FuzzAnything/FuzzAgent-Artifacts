// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_pcap_handle(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

static void cleanup_ether_addr(u_char *addr) {
    if (addr) {
        free(addr);
    }
}

static void cleanup_etherent(struct pcap_etherent *ent) {
    if (ent) {
        free(ent);
    }
}

static void cleanup_file(FILE *fp) {
    if (fp) {
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test pcap_lookupdev */
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        char *dev = pcap_lookupdev(errbuf);
        (void)dev; /* Use result to avoid unused variable warning */
    }

    /* Test pcap_ether_hostton */
    {
        /* Create null-terminated string from fuzzer input */
        size_t str_len = Size > 1024 ? 1024 : Size;
        char *hostname = (char *)malloc(str_len + 1);
        if (hostname) {
            memcpy(hostname, Data, str_len);
            hostname[str_len] = '\0';
            
            u_char *ether_addr = pcap_ether_hostton(hostname);
            cleanup_ether_addr(ether_addr);
            free(hostname);
        }
    }

    /* Test pcap_ether_aton */
    {
        /* Create null-terminated string from fuzzer input */
        size_t str_len = Size > 1024 ? 1024 : Size;
        char *ether_str = (char *)malloc(str_len + 1);
        if (ether_str) {
            memcpy(ether_str, Data, str_len);
            ether_str[str_len] = '\0';
            
            u_char *ether_addr = pcap_ether_aton(ether_str);
            cleanup_ether_addr(ether_addr);
            free(ether_str);
        }
    }

    /* Test pcap_lookupnet */
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        bpf_u_int32 net = 0, mask = 0;
        
        /* Create device name from fuzzer input */
        size_t dev_len = Size > 64 ? 64 : Size;
        char *device = (char *)malloc(dev_len + 1);
        if (device) {
            memcpy(device, Data, dev_len);
            device[dev_len] = '\0';
            
            int result = pcap_lookupnet(device, &net, &mask, errbuf);
            (void)result; /* Use result to avoid unused variable warning */
            free(device);
        }
    }

    /* Test pcap_open */
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        
        /* Create source string from fuzzer input */
        size_t src_len = Size > 256 ? 256 : Size;
        char *source = (char *)malloc(src_len + 1);
        if (source) {
            memcpy(source, Data, src_len);
            source[src_len] = '\0';
            
            /* Use fuzzer data to generate parameters */
            int snaplen = (Size > 0) ? (Data[0] * 256 + (Size > 1 ? Data[1] : 0)) : 65535;
            int flags = (Size > 2) ? Data[2] : 0;
            int read_timeout = (Size > 3) ? Data[3] * 1000 : 1000;
            
            pcap_t *handle = pcap_open(source, snaplen, flags, read_timeout, NULL, errbuf);
            cleanup_pcap_handle(handle);
            free(source);
        }
    }

    /* Test pcap_next_etherent */
    {
        /* Write fuzzer data to dummy file */
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            fp = fopen("./dummy_file", "r");
            if (fp) {
                struct pcap_etherent *ent;
                while ((ent = pcap_next_etherent(fp)) != NULL) {
                    cleanup_etherent(ent);
                }
                cleanup_file(fp);
            }
            remove("./dummy_file");
        }
    }

    return 0;
}