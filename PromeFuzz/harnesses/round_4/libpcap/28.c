// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_dispatch at pcap.c:2893:1 in pcap.h
// pcap_dump_open at sf-pcap.c:902:1 in pcap.h
// pcap_dump at sf-pcap.c:819:1 in pcap.h
// pcap_dump_close at sf-pcap.c:1262:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *data) {
    // Do nothing - just a dummy callback
    (void)user;
    (void)h;
    (void)data;
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    pcap_dumper_t *dumper = NULL;
    
    // Try to create a pcap handle from dummy file first
    write_dummy_file(Data, Size);
    pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    
    // If that fails, try to create a dead handle
    if (pcap_handle == NULL) {
        pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    // If we still don't have a handle, try to create one for live capture
    if (pcap_handle == NULL && Size > 0) {
        // Use first byte as device name length (modulo 16 for safety)
        size_t dev_len = Data[0] % 16;
        if (dev_len > 0 && dev_len + 1 < Size) {
            char dev_name[17];
            memcpy(dev_name, Data + 1, dev_len);
            dev_name[dev_len] = '\0';
            pcap_handle = pcap_create(dev_name, errbuf);
        }
    }
    
    // If we have a handle, test the API functions
    if (pcap_handle != NULL) {
        int immediate_mode = 0;
        int timeout = 0;
        int count = 0;
        
        // Extract parameters from fuzzing data if available
        if (Size >= 4) {
            immediate_mode = Data[0] % 2;
            timeout = (Data[1] << 8) | Data[2];
            count = Data[3] % 100;
        }
        
        // Test pcap_set_immediate_mode
        pcap_set_immediate_mode(pcap_handle, immediate_mode);
        
        // Test pcap_set_timeout
        pcap_set_timeout(pcap_handle, timeout);
        
        // Test pcap_activate (may fail if already activated)
        pcap_activate(pcap_handle);
        
        // Test pcap_loop with dummy callback
        pcap_loop(pcap_handle, count, dummy_packet_handler, NULL);
        
        // Test pcap_dispatch with dummy callback
        pcap_dispatch(pcap_handle, count, dummy_packet_handler, NULL);
        
        // Test pcap_dump by creating a dumper
        dumper = pcap_dump_open(pcap_handle, "./dummy_output.pcap");
        if (dumper != NULL) {
            // Create a dummy packet header
            struct pcap_pkthdr pkthdr;
            gettimeofday(&pkthdr.ts, NULL);
            pkthdr.caplen = Size > 65535 ? 65535 : Size;
            pkthdr.len = pkthdr.caplen;
            
            // Test pcap_dump
            pcap_dump((u_char *)dumper, &pkthdr, Data);
            
            pcap_dump_close(dumper);
            unlink("./dummy_output.pcap");
        }
        
        pcap_close(pcap_handle);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}