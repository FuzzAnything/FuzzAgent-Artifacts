// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static pcap_t *create_test_handle(void) {
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    
    // Try to create a live capture handle first
    pcap_t *handle = pcap_create("lo", errbuf);
    if (handle == NULL) {
        // If that fails, try any device
        handle = pcap_create("any", errbuf);
        if (handle == NULL) {
            // Last resort: create a dead handle for offline operations
            handle = pcap_open_dead(DLT_EN10MB, 65535);
        }
    }
    return handle;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from fuzzing input
    int param1 = *(const int *)Data % 65536;
    int param2 = *(const int *)(Data + 4) % 65536;
    
    // Create a test handle
    pcap_t *handle = create_test_handle();
    if (handle == NULL) {
        return 0;
    }

    // Test pcap_set_protocol_linux
    pcap_set_protocol_linux(handle, param1);

    // Test pcap_set_rfmon
    pcap_set_rfmon(handle, param1 & 1);

    // Test pcap_set_snaplen
    pcap_set_snaplen(handle, param2 & 0xFFFF);

    // Test pcap_set_buffer_size
    pcap_set_buffer_size(handle, param2);

    // Test pcap_set_promisc
    pcap_set_promisc(handle, param2 & 1);

    // Test pcap_open_live
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    const char *device = "lo";
    if (Size > 8) {
        // Use first few bytes as device name if available
        size_t dev_len = Size > 64 ? 64 : Size;
        char temp_dev[65] = {0};
        memcpy(temp_dev, Data, dev_len);
        temp_dev[dev_len] = '\0';
        device = temp_dev;
    }
    
    pcap_t *live_handle = pcap_open_live(device, 
                                         param1 & 0xFFFF,  // snaplen
                                         param2 & 1,       // promisc
                                         1000,             // timeout_ms
                                         errbuf);
    if (live_handle != NULL) {
        pcap_close(live_handle);
    }

    // Cleanup
    pcap_close(handle);

    // Test with file operations if we have enough data
    if (Size > 16) {
        write_dummy_file(Data, Size > 1024 ? 1024 : Size);
        
        // Try to open the dummy file as offline capture
        pcap_t *offline_handle = pcap_open_offline("./dummy_file", errbuf);
        if (offline_handle != NULL) {
            // Test setting functions on offline handle (should fail gracefully)
            pcap_set_rfmon(offline_handle, param1 & 1);
            pcap_set_snaplen(offline_handle, param2 & 0xFFFF);
            pcap_set_promisc(offline_handle, param2 & 1);
            pcap_close(offline_handle);
        }
        unlink("./dummy_file");
    }

    return 0;
}