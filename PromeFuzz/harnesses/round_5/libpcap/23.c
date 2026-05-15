// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    int fd = -1;
    
    // Write input data to a dummy file for pcap_open_offline
    fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }
    
    // Try to create handles using different methods
    const char *devices[] = {"any", "lo", "eth0", "wlan0", NULL};
    
    for (int i = 0; devices[i] != NULL; i++) {
        pcap_handle = pcap_create(devices[i], errbuf);
        if (pcap_handle != NULL) {
            break;
        }
    }
    
    // If pcap_create failed, try pcap_open_dead
    if (pcap_handle == NULL) {
        pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    // If still NULL, try pcap_open_offline with dummy file
    if (pcap_handle == NULL) {
        pcap_handle = pcap_open_offline("./dummy_file", errbuf);
    }
    
    if (pcap_handle == NULL) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract parameters from fuzz data
    int param1 = 0, param2 = 0, param3 = 0;
    pcap_direction_t direction = PCAP_D_INOUT;
    
    if (Size >= 1) param1 = Data[0] % 256;
    if (Size >= 2) param2 = Data[1] % 256;
    if (Size >= 3) param3 = Data[2] % 256;
    if (Size >= 4) direction = (pcap_direction_t)(Data[3] % 3);
    
    // Try different API calls in various orders
    int snaplen = 68 + (param1 * 100);
    int rfmon = param2 & 1;
    int immediate_mode = param3 & 1;
    int datalink_type = DLT_EN10MB + (param1 % 10);
    
    // Pre-activation configurations
    pcap_set_snaplen(pcap_handle, snaplen);
    pcap_set_rfmon(pcap_handle, rfmon);
    pcap_set_immediate_mode(pcap_handle, immediate_mode);
    pcap_set_datalink(pcap_handle, datalink_type);
    
    // Try activation
    int activate_result = pcap_activate(pcap_handle);
    
    // Post-activation configurations (some may fail)
    if (activate_result >= 0) {
        pcap_setdirection(pcap_handle, direction);
        
        // Try to set parameters again (should fail for some)
        pcap_set_snaplen(pcap_handle, snaplen + 100);
        pcap_set_rfmon(pcap_handle, rfmon ^ 1);
        pcap_set_immediate_mode(pcap_handle, immediate_mode ^ 1);
        pcap_set_datalink(pcap_handle, datalink_type + 1);
        
        // Try a capture loop with minimal packets
        pcap_loop(pcap_handle, 1, dummy_handler, NULL);
    }
    
    // Clean up
    pcap_close(pcap_handle);
    unlink("./dummy_file");
    
    return 0;
}