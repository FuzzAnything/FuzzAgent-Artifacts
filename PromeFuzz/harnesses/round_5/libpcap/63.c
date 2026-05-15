// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
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
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

static void fuzz_pcap_nametoaddrinfo(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *hostname = (char *)malloc(size + 1);
    if (!hostname) return;
    
    memcpy(hostname, data, size);
    hostname[size] = '\0';
    
    struct addrinfo *result = pcap_nametoaddrinfo(hostname);
    if (result) {
        freeaddrinfo(result);
    }
    
    free(hostname);
}

static void fuzz_pcap_parsesrcstr(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *source = (char *)malloc(size + 1);
    if (!source) return;
    
    memcpy(source, data, size);
    source[size] = '\0';
    
    int type;
    char host[1024], port[1024], name[1024], errbuf[PCAP_ERRBUF_SIZE];
    
    pcap_parsesrcstr(source, &type, host, port, name, errbuf);
    
    free(source);
}

static void fuzz_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *source = (char *)malloc(size + 1);
    if (!source) return;
    
    memcpy(source, data, size);
    source[size] = '\0';
    
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    int result = pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
    if (result == 0 && alldevs) {
        pcap_freealldevs(alldevs);
    }
    
    free(source);
}

static void fuzz_pcap_createsrcstr(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    size_t part_size = size / 3;
    char *host = (char *)malloc(part_size + 1);
    char *port = (char *)malloc(part_size + 1);
    char *name = (char *)malloc(part_size + 1);
    
    if (!host || !port || !name) {
        free(host);
        free(port);
        free(name);
        return;
    }
    
    memcpy(host, data, part_size);
    host[part_size] = '\0';
    
    memcpy(port, data + part_size, part_size);
    port[part_size] = '\0';
    
    memcpy(name, data + 2 * part_size, part_size);
    name[part_size] = '\0';
    
    char source[PCAP_BUF_SIZE];
    char errbuf[PCAP_ERRBUF_SIZE];
    
    int types[] = {PCAP_SRC_IFLOCAL, PCAP_SRC_IFREMOTE, PCAP_SRC_FILE};
    for (int i = 0; i < 3; i++) {
        pcap_createsrcstr(source, types[i], host, port, name, errbuf);
    }
    
    free(host);
    free(port);
    free(name);
}

static void fuzz_pcap_open(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    write_dummy_file(data, size);
    
    char *source = (char *)malloc(size + 1);
    if (!source) {
        remove_dummy_file();
        return;
    }
    
    memcpy(source, data, size);
    source[size] = '\0';
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open(source, 65535, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
    
    free(source);
    remove_dummy_file();
}

static void fuzz_pcap_findalldevs(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    int result = pcap_findalldevs(&alldevs, errbuf);
    if (result == 0 && alldevs) {
        pcap_freealldevs(alldevs);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (selector % 6) {
        case 0:
            fuzz_pcap_nametoaddrinfo(remaining_data, remaining_size);
            break;
        case 1:
            fuzz_pcap_parsesrcstr(remaining_data, remaining_size);
            break;
        case 2:
            fuzz_pcap_findalldevs_ex(remaining_data, remaining_size);
            break;
        case 3:
            fuzz_pcap_createsrcstr(remaining_data, remaining_size);
            break;
        case 4:
            fuzz_pcap_open(remaining_data, remaining_size);
            break;
        case 5:
            fuzz_pcap_findalldevs(remaining_data, remaining_size);
            break;
    }
    
    return 0;
}