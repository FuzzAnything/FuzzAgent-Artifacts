// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include "pcap/pcap.h"
#include "pcap/namedb.h"

static void fuzz_pcap_lookupdev(void) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char *device = pcap_lookupdev(errbuf);
    (void)device;
}

static void fuzz_pcap_ether_hostton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *hostname = malloc(size + 1);
    if (!hostname) return;
    
    memcpy(hostname, data, size);
    hostname[size] = '\0';
    
    u_char *addr = pcap_ether_hostton(hostname);
    if (addr) free(addr);
    
    free(hostname);
}

static void fuzz_pcap_ether_aton(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *addr_str = malloc(size + 1);
    if (!addr_str) return;
    
    memcpy(addr_str, data, size);
    addr_str[size] = '\0';
    
    u_char *addr = pcap_ether_aton(addr_str);
    if (addr) free(addr);
    
    free(addr_str);
}

static void fuzz_pcap_lookupnet(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *device = malloc(size + 1);
    if (!device) return;
    
    memcpy(device, data, size);
    device[size] = '\0';
    
    bpf_u_int32 net, mask;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    pcap_lookupnet(device, &net, &mask, errbuf);
    
    free(device);
}

static void fuzz_pcap_open(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *source = malloc(size + 1);
    if (!source) return;
    
    memcpy(source, data, size);
    source[size] = '\0';
    
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth = {0};
    
    pcap_t *pcap = pcap_open(source, 65535, PCAP_OPENFLAG_PROMISCUOUS, 1000, &auth, errbuf);
    if (pcap) pcap_close(pcap);
    
    free(source);
}

static void fuzz_pcap_next_etherent(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    fp = fopen("./dummy_file", "rb");
    if (!fp) return;
    
    struct pcap_etherent *ether;
    while ((ether = pcap_next_etherent(fp)) != NULL) {
        (void)ether;
    }
    
    fclose(fp);
}

int LLVMFuzzerTestOneInput_51(const uint8_t *data, size_t size) {
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
    
    if (size == 0) return 0;
    
    uint8_t selector = data[0];
    const uint8_t *remaining_data = data + 1;
    size_t remaining_size = size - 1;
    
    switch (selector % 6) {
        case 0:
            fuzz_pcap_lookupdev();
            break;
        case 1:
            fuzz_pcap_ether_hostton(remaining_data, remaining_size);
            break;
        case 2:
            fuzz_pcap_ether_aton(remaining_data, remaining_size);
            break;
        case 3:
            fuzz_pcap_lookupnet(remaining_data, remaining_size);
            break;
        case 4:
            fuzz_pcap_open(remaining_data, remaining_size);
            break;
        case 5:
            fuzz_pcap_next_etherent(remaining_data, remaining_size);
            break;
    }
    
    return 0;
}