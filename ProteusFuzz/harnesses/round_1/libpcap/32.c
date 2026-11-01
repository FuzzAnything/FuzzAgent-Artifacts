// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
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

static void safe_freealldevs(pcap_if_t *alldevs) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void safe_close_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure null termination for string operations
    char *buf = (char *)malloc(Size + 1);
    if (buf == NULL) {
        return 0;
    }
    memcpy(buf, Data, Size);
    buf[Size] = '\0';

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    struct pcap_rmtauth auth = {0};
    char source[1024];
    char host[256], port[64], name[256];
    int type;

    // 1. Test pcap_lookupdev (Deprecated but tested for robustness)
    memset(errbuf, 0, sizeof(errbuf));
    char *dev = pcap_lookupdev(errbuf);
    // dev is static or malloc'd depending on implementation, usually static in older versions.
    // We don't free it as per typical libpcap behavior for this specific deprecated function.
    (void)dev;

    // 2. Test pcap_findalldevs
    memset(errbuf, 0, sizeof(errbuf));
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        // Success, do not process list to avoid heavy iteration in fuzzer, just free
        safe_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. Test pcap_findalldevs_ex with various source strings derived from input
    memset(errbuf, 0, sizeof(errbuf));
    // Try using the input as a source string, truncated to safe length
    size_t src_len = (Size < 512) ? Size : 512;
    char src[513];
    memcpy(src, buf, src_len);
    src[src_len] = '\0';

    // Try with NULL auth
    if (pcap_findalldevs_ex(src, NULL, &alldevs, errbuf) == 0) {
        safe_freealldevs(alldevs);
        alldevs = NULL;
    }

    // Try with a dummy auth structure
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = NULL;
    auth.password = NULL;
    if (pcap_findalldevs_ex(src, &auth, &alldevs, errbuf) == 0) {
        safe_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 4. Test pcap_createsrcstr
    memset(source, 0, sizeof(source));
    memset(errbuf, 0, sizeof(errbuf));
    // Use input as host, port, name components with bounds checking
    int host_len = (Size > 10) ? 10 : Size;
    int port_len = (Size > 5) ? 5 : Size;
    int name_len = (Size > 15) ? 15 : Size;
    
    char h_buf[11], p_buf[6], n_buf[16];
    memset(h_buf, 0, sizeof(h_buf));
    memset(p_buf, 0, sizeof(p_buf));
    memset(n_buf, 0, sizeof(n_buf));
    
    if (Size > 0) {
        memcpy(h_buf, buf, host_len);
        memcpy(p_buf, buf, port_len);
        memcpy(n_buf, buf, name_len);
    }

    int type_val = (Size > 0) ? (int)Data[0] % 5 : 0; // Limit type to small range
    
    int ret_createsrc = pcap_createsrcstr(source, type_val, h_buf, p_buf, n_buf, errbuf);
    (void)ret_createsrc;

    // 5. Test pcap_parsesrcstr
    memset(errbuf, 0, sizeof(errbuf));
    memset(host, 0, sizeof(host));
    memset(port, 0, sizeof(port));
    memset(name, 0, sizeof(name));
    
    // Use the generated source string or the input directly
    const char *parse_src = (strlen(source) > 0) ? source : src;
    int ret_parse = pcap_parsesrcstr(parse_src, &type, host, port, name, errbuf);
    (void)ret_parse;

    // 6. Test pcap_open
    memset(errbuf, 0, sizeof(errbuf));
    int snaplen = (Size > 1) ? (int)Data[1] * 100 : 65535;
    if (snaplen < 0) snaplen = 65535;
    
    int flags = (Size > 2) ? (int)Data[2] : 0;
    int timeout = (Size > 3) ? (int)Data[3] * 10 : 1000;
    if (timeout < 0) timeout = 1000;

    // Try opening with the parsed source or the input string
    const char *open_src = (strlen(source) > 0) ? source : src;
    
    handle = pcap_open(open_src, snaplen, flags, timeout, &auth, errbuf);
    if (handle != NULL) {
        // Immediately close to avoid blocking or resource leaks
        safe_close_pcap(handle);
        handle = NULL;
    }

    // Try opening with NULL source (defaults to "any")
    handle = pcap_open(NULL, snaplen, flags, timeout, NULL, errbuf);
    if (handle != NULL) {
        safe_close_pcap(handle);
        handle = NULL;
    }

    free(buf);
    return 0;
}