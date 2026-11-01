// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_nametonetaddr at nametoaddr.c:220:1 in namedb.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_nametoaddr at nametoaddr.c:138:1 in namedb.h
// pcap_nametoaddrinfo at nametoaddr.c:178:1 in namedb.h
// pcap_nametoport at nametoaddr.c:304:1 in namedb.h
// pcap_ether_hostton at nametoaddr.c:887:1 in namedb.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <pcap.h>
#include <namedb.h>

/* Helper to create a null-terminated string from fuzz input */
static char *create_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    /* Ensure we don't allocate too much */
    if (size > 1024) {
        size = 1024;
    }
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Create a safe string from input for name resolution */
    char *name_str = create_string(Data, Size);
    if (!name_str) return 0;

    /* 1. Test pcap_nametonetaddr */
    bpf_u_int32 net_addr;
    net_addr = pcap_nametonetaddr(name_str);
    (void)net_addr; /* Suppress unused warning */

    /* 2. Test pcap_lookupnet */
    bpf_u_int32 addr, mask;
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    /* Use the input string as a device name, though it likely won't be valid */
    int ret = pcap_lookupnet(name_str, &addr, &mask, errbuf);
    (void)ret;
    (void)addr;
    (void)mask;

    /* 3. Test pcap_nametoaddr (deprecated but tested for robustness) */
    struct hostent *he;
    he = pcap_nametoaddr(name_str);
    (void)he;

    /* 4. Test pcap_nametoaddrinfo */
    struct addrinfo *ai;
    ai = pcap_nametoaddrinfo(name_str);
    if (ai) {
        freeaddrinfo(ai);
    }

    /* 5. Test pcap_nametoport */
    int port, proto;
    ret = pcap_nametoport(name_str, &port, &proto);
    (void)ret;
    (void)port;
    (void)proto;

    /* 6. Test pcap_ether_hostton */
    u_char *ether_addr;
    ether_addr = pcap_ether_hostton(name_str);
    if (ether_addr) {
        free(ether_addr);
    }

    free(name_str);
    return 0;
}