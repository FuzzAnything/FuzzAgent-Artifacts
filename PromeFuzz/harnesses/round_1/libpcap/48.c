// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "pcap/pcap.h"
#include "pcap/namedb.h"

static void cleanup_pcap_open(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void cleanup_pcap_findalldevs(pcap_if_t *alldevs) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void cleanup_pcap_ether_hostton(u_char *addr) {
    if (addr != NULL) {
        free(addr);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Prepare errbuf for functions that need it
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Use first byte to decide which functions to call and how
    uint8_t selector = Data[0];
    size_t data_offset = 1;

    // Extract a string from the input data (null-terminated)
    // Ensure we don't read beyond Size
    size_t str_len = Size - data_offset;
    if (str_len > 1024) {
        str_len = 1024;
    }
    char *input_str = malloc(str_len + 1);
    if (input_str == NULL) {
        return 0;
    }
    if (str_len > 0) {
        memcpy(input_str, Data + data_offset, str_len);
    }
    input_str[str_len] = '\0';

    // 1. Test pcap_open
    if (selector & 0x01) {
        pcap_t *handle = pcap_open(input_str, 65535, PCAP_OPENFLAG_PROMISCUOUS,
                                   1000, NULL, errbuf);
        cleanup_pcap_open(handle);
    }

    // 2. Test pcap_findalldevs_ex
    if (selector & 0x02) {
        pcap_if_t *alldevs = NULL;
        int result = pcap_findalldevs_ex(input_str, NULL, &alldevs, errbuf);
        if (result == 0) {
            cleanup_pcap_findalldevs(alldevs);
        }
    }

    // 3. Test pcap_next_etherent
    if (selector & 0x04) {
        // Write data to dummy file
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp != NULL) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            // Now read from the file
            fp = fopen("./dummy_file", "rb");
            if (fp != NULL) {
                struct pcap_etherent *ether_entry;
                while ((ether_entry = pcap_next_etherent(fp)) != NULL) {
                    // Just read entries, no processing needed
                }
                fclose(fp);
            }
            remove("./dummy_file");
        }
    }

    // 4. Test pcap_ether_hostton
    if (selector & 0x08) {
        u_char *mac_addr = pcap_ether_hostton(input_str);
        cleanup_pcap_ether_hostton(mac_addr);
    }

    // 5. Test pcap_findalldevs
    if (selector & 0x10) {
        pcap_if_t *alldevs = NULL;
        int result = pcap_findalldevs(&alldevs, errbuf);
        if (result == 0) {
            cleanup_pcap_findalldevs(alldevs);
        }
    }

    // 6. Test pcap_lookupdev (deprecated but still test it)
    if (selector & 0x20) {
        char *dev_name = pcap_lookupdev(errbuf);
        // No cleanup needed as it returns pointer to static buffer
    }

    // Try different combinations of parameters for pcap_open
    if (selector & 0x40) {
        // Test with different snaplen values
        int snaplen_values[] = {68, 96, 128, 256, 512, 1024, 1514, 65535};
        int num_snaplen = sizeof(snaplen_values) / sizeof(snaplen_values[0]);
        int snaplen_idx = (selector >> 4) % num_snaplen;
        
        // Test with different flags
        int flags_values[] = {0, PCAP_OPENFLAG_PROMISCUOUS, 
                              PCAP_OPENFLAG_DATATX_UDP, PCAP_OPENFLAG_NOCAPTURE_RPCAP};
        int num_flags = sizeof(flags_values) / sizeof(flags_values[0]);
        int flags_idx = (selector >> 2) % num_flags;
        
        // Test with different timeouts
        int timeout_values[] = {0, 10, 100, 1000, 10000};
        int num_timeout = sizeof(timeout_values) / sizeof(timeout_values[0]);
        int timeout_idx = (selector >> 6) % num_timeout;
        
        pcap_t *handle = pcap_open(input_str, 
                                  snaplen_values[snaplen_idx],
                                  flags_values[flags_idx],
                                  timeout_values[timeout_idx],
                                  NULL, errbuf);
        cleanup_pcap_open(handle);
    }

    // Test pcap_findalldevs_ex with different source types
    if (selector & 0x80) {
        const char *source_prefixes[] = {
            "rpcap://",
            "file://",
            "rpcap://localhost/",
            ""
        };
        int num_prefixes = sizeof(source_prefixes) / sizeof(source_prefixes[0]);
        int prefix_idx = (selector >> 3) % num_prefixes;
        
        // Construct source string
        char source[2048];
        snprintf(source, sizeof(source), "%s%s", source_prefixes[prefix_idx], input_str);
        
        pcap_if_t *alldevs = NULL;
        int result = pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
        if (result == 0) {
            cleanup_pcap_findalldevs(alldevs);
        }
    }

    free(input_str);
    return 0;
}