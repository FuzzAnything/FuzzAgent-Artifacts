// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcap/pcap.h"
#include "pcap/namedb.h"

static void cleanup_pcap_if_t(pcap_if_t *alldevs) {
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
}

static void cleanup_ether_addr(u_char *addr) {
    if (addr) {
        free(addr);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Split the input data into multiple parts for different functions
    size_t part_size = Size / 6;
    if (part_size == 0) part_size = 1;

    // Part 1: pcap_findalldevs_ex
    {
        char source[256] = {0};
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        pcap_if_t *alldevs = NULL;
        struct pcap_rmtauth auth = {0};
        
        // Use first part as source string (null-terminated)
        size_t copy_size = (part_size < sizeof(source)-1) ? part_size : sizeof(source)-1;
        memcpy(source, Data, copy_size);
        source[copy_size] = '\0';
        
        // Try different auth types
        for (int auth_type = 0; auth_type <= 2; auth_type++) {
            auth.type = auth_type;
            if (auth_type > 0) {
                // For non-null auth, set some dummy credentials
                auth.username = "testuser";
                auth.password = "testpass";
            }
            
            int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
            if (result == 0 && alldevs) {
                cleanup_pcap_if_t(alldevs);
                alldevs = NULL;
            }
        }
    }

    // Part 2: pcap_next_etherent
    {
        // Create a dummy file with the fuzzer input
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            // Write the second part of data to file
            size_t offset = part_size;
            size_t write_size = (offset + part_size <= Size) ? part_size : Size - offset;
            if (write_size > 0) {
                fwrite(Data + offset, 1, write_size, fp);
            }
            fclose(fp);
            
            // Now read from the file
            fp = fopen("./dummy_file", "rb");
            if (fp) {
                struct pcap_etherent *etherent;
                while ((etherent = pcap_next_etherent(fp)) != NULL) {
                    // Just read entries until EOF
                }
                fclose(fp);
            }
        }
    }

    // Part 3: pcap_ether_hostton
    {
        char hostname[256] = {0};
        size_t offset = part_size * 2;
        if (offset < Size) {
            size_t copy_size = (offset + part_size <= Size) ? part_size : Size - offset;
            if (copy_size > sizeof(hostname)-1) copy_size = sizeof(hostname)-1;
            memcpy(hostname, Data + offset, copy_size);
            hostname[copy_size] = '\0';
            
            u_char *ether_addr = pcap_ether_hostton(hostname);
            cleanup_ether_addr(ether_addr);
        }
    }

    // Part 4: pcap_findalldevs
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        pcap_if_t *alldevs = NULL;
        
        int result = pcap_findalldevs(&alldevs, errbuf);
        if (result == 0 && alldevs) {
            cleanup_pcap_if_t(alldevs);
            alldevs = NULL;
        }
    }

    // Part 5: pcap_lookupdev
    {
        char errbuf[PCAP_ERRBUF_SIZE] = {0};
        char *dev = pcap_lookupdev(errbuf);
        // No cleanup needed - returns pointer to static buffer
    }

    // Part 6: pcap_ether_aton
    {
        char ether_str[256] = {0};
        size_t offset = part_size * 5;
        if (offset < Size) {
            size_t copy_size = Size - offset;
            if (copy_size > sizeof(ether_str)-1) copy_size = sizeof(ether_str)-1;
            memcpy(ether_str, Data + offset, copy_size);
            ether_str[copy_size] = '\0';
            
            u_char *ether_addr = pcap_ether_aton(ether_str);
            cleanup_ether_addr(ether_addr);
        }
    }

    return 0;
}