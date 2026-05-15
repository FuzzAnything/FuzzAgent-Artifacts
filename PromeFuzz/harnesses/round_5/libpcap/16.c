// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    pcap_if_t *alldevs1 = NULL;
    pcap_if_t *alldevs2 = NULL;
    pcap_if_t *alldevs3 = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 net, mask;
    
    // Prepare source strings from fuzzer input
    size_t source_len = Size > 100 ? 100 : Size;
    char *source = (char *)malloc(source_len + 1);
    if (!source) return 0;
    memcpy(source, Data, source_len);
    source[source_len] = '\0';
    
    // Prepare auth structure
    struct pcap_rmtauth auth;
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;
    
    // First pcap_findalldevs_ex call with file source
    write_dummy_file(Data, Size > 256 ? 256 : Size);
    char file_source[256];
    snprintf(file_source, sizeof(file_source), "file://./dummy_file");
    pcap_findalldevs_ex(file_source, &auth, &alldevs1, errbuf);
    
    // Second pcap_findalldevs_ex call with local/remote source
    pcap_findalldevs_ex(source, &auth, &alldevs2, errbuf);
    
    // pcap_findalldevs call
    pcap_findalldevs(&alldevs3, errbuf);
    
    // pcap_lookupnet calls on available devices
    if (alldevs1) {
        pcap_lookupnet(alldevs1->name, &net, &mask, errbuf);
        if (alldevs1->next) {
            pcap_lookupnet(alldevs1->next->name, &net, &mask, errbuf);
        }
    }
    if (alldevs2) {
        pcap_lookupnet(alldevs2->name, &net, &mask, errbuf);
    }
    if (alldevs3) {
        pcap_lookupnet(alldevs3->name, &net, &mask, errbuf);
        if (alldevs3->next) {
            pcap_lookupnet(alldevs3->next->name, &net, &mask, errbuf);
        }
    }
    
    // Also try pcap_lookupnet with some fuzzer-generated device names
    if (Size > 10) {
        char devname[256];
        size_t name_len = Size > 255 ? 255 : Size;
        memcpy(devname, Data, name_len);
        devname[name_len] = '\0';
        pcap_lookupnet(devname, &net, &mask, errbuf);
    }
    
    // pcap_freealldevs calls
    pcap_freealldevs(alldevs1);
    pcap_freealldevs(alldevs2);
    pcap_freealldevs(alldevs3);
    
    free(source);
    unlink("./dummy_file");
    
    return 0;
}