// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void fuzz_pcap_create(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device = NULL;
    
    if (size > 0) {
        device = (const char *)data;
        if (strlen(device) >= size) {
            return;
        }
    }
    
    pcap_t *pcap = pcap_create(device, errbuf);
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

static void fuzz_pcap_findalldevs(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    
    int result = pcap_findalldevs(&alldevs, errbuf);
    if (result == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void fuzz_pcap_parsesrcstr(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    int type;
    char host[256];
    char port[256];
    char name[256];
    
    if (size == 0) return;
    
    const char *source = (const char *)data;
    size_t source_len = strnlen(source, size);
    if (source_len >= size) return;
    
    int result = pcap_parsesrcstr(source, &type, host, port, name, errbuf);
}

static void fuzz_pcap_createsrcstr(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[1024];
    
    int type = PCAP_SRC_IFLOCAL;
    const char *host = NULL;
    const char *port = NULL;
    const char *name = NULL;
    
    if (size > 0) {
        const char *input = (const char *)data;
        size_t input_len = strnlen(input, size);
        if (input_len < size) {
            name = input;
        }
        
        if (size > 100) {
            host = (const char *)(data + 50);
            if (strnlen(host, size - 50) >= size - 50) {
                host = NULL;
            }
        }
    }
    
    pcap_createsrcstr(source, type, host, port, name, errbuf);
}

static void fuzz_pcap_open(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (size == 0) return;
    
    const char *source = (const char *)data;
    size_t source_len = strnlen(source, size);
    if (source_len >= size) return;
    
    int snaplen = 65535;
    int flags = PCAP_OPENFLAG_PROMISCUOUS;
    int read_timeout = 1000;
    struct pcap_rmtauth auth = {0};
    
    if (size > 100) {
        snaplen = data[50] * 256 + data[51];
        flags = data[52];
        read_timeout = data[53] * 1000;
    }
    
    pcap_t *pcap = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

static void fuzz_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth = {0};
    
    if (size == 0) return;
    
    const char *source = (const char *)data;
    size_t source_len = strnlen(source, size);
    if (source_len >= size) return;
    
    if (size > 100) {
        auth.type = data[50];
        if (size > 150) {
            static char username[100];
            static char password[100];
            size_t user_len = strnlen((const char *)(data + 100), 50);
            if (user_len < 50) {
                memcpy(username, data + 100, user_len);
                username[user_len] = '\0';
                auth.username = username;
            }
            
            size_t pass_len = strnlen((const char *)(data + 150), size - 150);
            if (pass_len < 100 && pass_len < size - 150) {
                memcpy(password, data + 150, pass_len);
                password[pass_len] = '\0';
                auth.password = password;
            }
        }
    }
    
    int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (result == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector % 6) {
        case 0:
            fuzz_pcap_create(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_pcap_findalldevs(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_pcap_parsesrcstr(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_pcap_createsrcstr(fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_pcap_open(fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_pcap_findalldevs_ex(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}