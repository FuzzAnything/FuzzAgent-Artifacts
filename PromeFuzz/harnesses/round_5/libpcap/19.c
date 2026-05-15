// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void test_pcap_lib_version(void) {
    const char *version = pcap_lib_version();
    (void)version; // Use to avoid unused variable warning
}

static void test_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth;
    
    // Use fuzzer data to create source string (truncate to avoid overflow)
    char source[256];
    size_t source_len = size < sizeof(source) - 1 ? size : sizeof(source) - 1;
    if (source_len > 0) {
        memcpy(source, data, source_len);
        source[source_len] = '\0';
    } else {
        source[0] = '\0';
    }
    
    // Initialize auth structure with fuzzer data
    auth.type = size > 0 ? data[0] % 3 : 0; // 0-2 for auth types
    
    // Create username and password from fuzzer data if available
    char username[128];
    char password[128];
    
    if (size > 1) {
        size_t user_len = (data[1] % (sizeof(username) - 1)) + 1;
        if (user_len > size - 2) user_len = size - 2;
        if (user_len > 0) {
            memcpy(username, data + 2, user_len);
            username[user_len] = '\0';
            auth.username = username;
        } else {
            auth.username = NULL;
        }
    } else {
        auth.username = NULL;
    }
    
    if (size > 3) {
        size_t pass_len = (data[2] % (sizeof(password) - 1)) + 1;
        size_t pass_offset = 3;
        if (pass_offset + pass_len > size) pass_len = size - pass_offset;
        if (pass_len > 0) {
            memcpy(password, data + pass_offset, pass_len);
            password[pass_len] = '\0';
            auth.password = password;
        } else {
            auth.password = NULL;
        }
    } else {
        auth.password = NULL;
    }
    
    int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (result == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static int is_hex_char(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static void test_pcap_ether_aton(const uint8_t *data, size_t size) {
    // Create a string from fuzzer data
    char ether_str[18]; // Enough for "00:00:00:00:00:00" + null
    
    if (size >= 17) {
        // Try to create a valid MAC address format
        int valid = 1;
        for (int i = 0; i < 17; i++) {
            if (i % 3 == 2) {
                // Position for separator
                char sep = data[i] % 3;
                if (sep == 0) ether_str[i] = ':';
                else if (sep == 1) ether_str[i] = '.';
                else ether_str[i] = '-';
            } else {
                // Position for hex digit
                char hex_digit = "0123456789ABCDEF"[data[i] % 16];
                ether_str[i] = hex_digit;
                if (!is_hex_char(hex_digit)) {
                    valid = 0;
                }
            }
        }
        ether_str[17] = '\0';
        
        // Check if last character is a separator (invalid)
        char last_char = ether_str[16];
        if (last_char == ':' || last_char == '.' || last_char == '-') {
            valid = 0;
        }
        
        if (valid) {
            u_char *addr = pcap_ether_aton(ether_str);
            if (addr != NULL) {
                free(addr);
            }
        }
    }
    
    // Also test with raw data as string (null-terminated)
    if (size > 0) {
        char *raw_str = (char *)malloc(size + 1);
        if (raw_str) {
            memcpy(raw_str, data, size);
            raw_str[size] = '\0';
            u_char *addr = pcap_ether_aton(raw_str);
            if (addr != NULL) {
                free(addr);
            }
            free(raw_str);
        }
    }
}

static void test_pcap_lookupdev(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char *dev = pcap_lookupdev(errbuf);
    (void)dev; // Use to avoid unused variable warning
}

static void test_pcap_ether_hostton(const uint8_t *data, size_t size) {
    // Create hostname from fuzzer data
    if (size > 0) {
        char *hostname = (char *)malloc(size + 1);
        if (hostname) {
            memcpy(hostname, data, size);
            hostname[size] = '\0';
            
            u_char *addr = pcap_ether_hostton(hostname);
            if (addr != NULL) {
                free(addr);
            }
            free(hostname);
        }
    }
}

static void test_pcap_next_etherent(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
        
        fp = fopen("./dummy_file", "r");
        if (fp) {
            struct pcap_etherent *ent;
            while ((ent = pcap_next_etherent(fp)) != NULL) {
                // Nothing to do with the entry - just consume it
                (void)ent;
            }
            fclose(fp);
        }
        remove("./dummy_file");
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    // Test all API functions with the fuzzer input
    
    // 1. pcap_lib_version - no input needed
    test_pcap_lib_version();
    
    // 2. pcap_findalldevs_ex
    test_pcap_findalldevs_ex(Data, Size);
    
    // 3. pcap_ether_aton
    test_pcap_ether_aton(Data, Size);
    
    // 4. pcap_lookupdev - no input needed
    test_pcap_lookupdev();
    
    // 5. pcap_ether_hostton
    test_pcap_ether_hostton(Data, Size);
    
    // 6. pcap_next_etherent
    test_pcap_next_etherent(Data, Size);
    
    return 0;
}