// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void safe_cleanup(pcap_if_t *alldevs, pcap_t *handle) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Test 1: pcap_lookupdev (Deprecated but still testable)
    char *dev_name = pcap_lookupdev(errbuf);
    if (dev_name != NULL) {
        // No cleanup needed for string returned by pcap_lookupdev
    }

    // Test 2: pcap_findalldevs
    pcap_if_t *alldevs = NULL;
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        // Successfully found devices
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // Test 3: pcap_findalldevs_ex with various source strings
    // Use input data to construct a source string
    char source[256];
    size_t src_len = (Size < sizeof(source) - 1) ? Size : sizeof(source) - 1;
    memcpy(source, Data, src_len);
    source[src_len] = '\0';

    // Ensure null termination and basic safety
    if (src_len > 0 && source[src_len-1] == '\0') {
        // Already null terminated, fine
    }

    struct pcap_rmtauth auth = {0};
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = NULL;
    auth.password = NULL;

    pcap_if_t *alldevs_ex = NULL;
    int ret_ex = pcap_findalldevs_ex(source, &auth, &alldevs_ex, errbuf);
    if (ret_ex == 0 && alldevs_ex != NULL) {
        pcap_freealldevs(alldevs_ex);
        alldevs_ex = NULL;
    }

    // Test 4: pcap_parsesrcstr
    int type;
    char host[64], port[64], name[64];
    memset(&host, 0, sizeof(host));
    memset(&port, 0, sizeof(port));
    memset(&name, 0, sizeof(name));

    if (pcap_parsesrcstr(source, &type, host, port, name, errbuf) == 0) {
        // Parsed successfully
    }

    // Test 5: pcap_createsrcstr
    char created_source[256];
    memset(created_source, 0, sizeof(created_source));
    
    // Use parts of input for host/port/name if available, else defaults
    const char *h = (Size > 10) ? (const char*)(Data + 10) : "localhost";
    const char *p = (Size > 20) ? (const char*)(Data + 20) : "2000";
    const char *n = (Size > 30) ? (const char*)(Data + 30) : "any";
    
    // Limit lengths to avoid overflow
    char h_buf[64], p_buf[64], n_buf[64];
    memset(h_buf, 0, sizeof(h_buf));
    memset(p_buf, 0, sizeof(p_buf));
    memset(n_buf, 0, sizeof(n_buf));
    
    size_t h_len = strnlen(h, sizeof(h_buf) - 1);
    size_t p_len = strnlen(p, sizeof(p_buf) - 1);
    size_t n_len = strnlen(n, sizeof(n_buf) - 1);
    
    memcpy(h_buf, h, h_len);
    memcpy(p_buf, p, p_len);
    memcpy(n_buf, n, n_len);

    if (pcap_createsrcstr(created_source, 1, h_buf, p_buf, n_buf, errbuf) == 0) {
        // Successfully created source string
    }

    // Test 6: pcap_open
    pcap_t *handle = NULL;
    
    // Try opening with the original source string
    handle = pcap_open(source, 65535, 1, 1000, &auth, errbuf);
    if (handle != NULL) {
        pcap_close(handle);
        handle = NULL;
    }

    // Try opening with the created source string
    if (strlen(created_source) > 0) {
        handle = pcap_open(created_source, 65535, 1, 1000, &auth, errbuf);
        if (handle != NULL) {
            pcap_close(handle);
            handle = NULL;
        }
    }

    // Try opening with "any"
    handle = pcap_open("any", 65535, 1, 1000, &auth, errbuf);
    if (handle != NULL) {
        pcap_close(handle);
        handle = NULL;
    }

    // Try opening with a dummy file path as requested
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        
        handle = pcap_open(dummy_file, 65535, 1, 1000, &auth, errbuf);
        if (handle != NULL) {
            pcap_close(handle);
            handle = NULL;
        }
        
        // Clean up dummy file
        remove(dummy_file);
    }

    return 0;
}