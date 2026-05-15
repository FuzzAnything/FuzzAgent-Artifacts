// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_next_ex at pcap.c:568:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

static void setup_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_41(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    struct pcap_pkthdr *pkt_header;
    const u_char *pkt_data;
    
    // Use first byte to determine which code path to take
    uint8_t path_selector = data[0] % 6;
    size_t data_offset = 1;
    
    // Extract parameters from fuzz data with bounds checking
    int snapshot_len = 0;
    int promisc = 0;
    int timeout_ms = 0;
    int tstamp_precision = 0;
    int tstamp_type = 0;
    
    if (size - data_offset >= sizeof(int)) {
        memcpy(&snapshot_len, data + data_offset, sizeof(int));
        data_offset += sizeof(int);
    }
    if (size - data_offset >= sizeof(int)) {
        memcpy(&promisc, data + data_offset, sizeof(int));
        data_offset += sizeof(int);
    }
    if (size - data_offset >= sizeof(int)) {
        memcpy(&timeout_ms, data + data_offset, sizeof(int));
        data_offset += sizeof(int);
    }
    if (size - data_offset >= sizeof(int)) {
        memcpy(&tstamp_precision, data + data_offset, sizeof(int));
        data_offset += sizeof(int);
    }
    if (size - data_offset >= sizeof(int)) {
        memcpy(&tstamp_type, data + data_offset, sizeof(int));
        data_offset += sizeof(int);
    }
    
    // Ensure non-negative values for certain parameters
    snapshot_len = abs(snapshot_len) % 65536;
    timeout_ms = abs(timeout_ms) % 10000;
    promisc = promisc & 1;
    tstamp_precision = tstamp_precision & 1;
    tstamp_type = abs(tstamp_type) % 10;
    
    switch (path_selector) {
        case 0: {
            // Test pcap_open_live
            const char *device = "any";
            if (size - data_offset > 0) {
                // Use fuzz data as device name (truncated to safe length)
                char dev_name[256];
                size_t dev_len = (size - data_offset) > 255 ? 255 : (size - data_offset);
                memcpy(dev_name, data + data_offset, dev_len);
                dev_name[dev_len] = '\0';
                device = dev_name;
            }
            
            handle = pcap_open_live(device, snapshot_len, promisc, timeout_ms, errbuf);
            if (handle) {
                // Test pcap_next_ex on the live handle
                int ret = pcap_next_ex(handle, &pkt_header, &pkt_data);
                (void)ret; // Use return value to avoid unused variable warning
                
                // Test pcap_inject if we have data
                if (size - data_offset > 0) {
                    size_t inject_size = (size - data_offset) > 1500 ? 1500 : (size - data_offset);
                    pcap_inject(handle, data + data_offset, inject_size);
                }
                
                pcap_close(handle);
            }
            break;
        }
        
        case 1: {
            // Test pcap_create + pcap_activate
            const char *device = "any";
            if (size - data_offset > 0) {
                char dev_name[256];
                size_t dev_len = (size - data_offset) > 255 ? 255 : (size - data_offset);
                memcpy(dev_name, data + data_offset, dev_len);
                dev_name[dev_len] = '\0';
                device = dev_name;
            }
            
            handle = pcap_create(device, errbuf);
            if (handle) {
                // Set some options before activation
                pcap_set_snaplen(handle, snapshot_len);
                pcap_set_promisc(handle, promisc);
                pcap_set_timeout(handle, timeout_ms);
                
                // Test pcap_set_tstamp_precision
                pcap_set_tstamp_precision(handle, tstamp_precision);
                
                // Test pcap_set_tstamp_type
                pcap_set_tstamp_type(handle, tstamp_type);
                
                // Test pcap_activate
                int activate_ret = pcap_activate(handle);
                (void)activate_ret;
                
                if (pcap_activate(handle) >= 0) {
                    // Test pcap_next_ex
                    int ret = pcap_next_ex(handle, &pkt_header, &pkt_data);
                    (void)ret;
                    
                    // Test pcap_inject
                    if (size - data_offset > 0) {
                        size_t inject_size = (size - data_offset) > 1500 ? 1500 : (size - data_offset);
                        pcap_inject(handle, data + data_offset, inject_size);
                    }
                }
                
                pcap_close(handle);
            }
            break;
        }
        
        case 2: {
            // Test pcap_open_live with various parameters
            const char *device = "lo";
            handle = pcap_open_live(device, snapshot_len, promisc, timeout_ms, errbuf);
            if (handle) {
                // Test multiple pcap_next_ex calls
                for (int i = 0; i < 3; i++) {
                    int ret = pcap_next_ex(handle, &pkt_header, &pkt_data);
                    (void)ret;
                }
                pcap_close(handle);
            }
            break;
        }
        
        case 3: {
            // Test file-based capture with dummy file
            setup_dummy_file(data, size);
            
            handle = pcap_open_offline("./dummy_file", errbuf);
            if (handle) {
                // Test pcap_next_ex on file
                while (pcap_next_ex(handle, &pkt_header, &pkt_data) == 1) {
                    // Continue reading packets
                }
                pcap_close(handle);
            }
            cleanup_dummy_file();
            break;
        }
        
        case 4: {
            // Test pcap_create with timestamp settings
            handle = pcap_create("any", errbuf);
            if (handle) {
                // Try different timestamp precision values (0 and 1 are common)
                pcap_set_tstamp_precision(handle, 0); // Microsecond precision
                pcap_set_tstamp_precision(handle, 1); // Nanosecond precision
                
                // Try different timestamp type values (common values)
                // These are integer values that might be defined in pcap.h
                // We'll use small integers that are likely valid
                for (int i = 0; i < 5; i++) {
                    pcap_set_tstamp_type(handle, i);
                }
                
                pcap_close(handle);
            }
            break;
        }
        
        case 5: {
            // Test pcap_inject with various data sizes
            handle = pcap_open_live("lo", snapshot_len, promisc, timeout_ms, errbuf);
            if (handle) {
                // Try injecting packets of different sizes
                if (size - data_offset > 0) {
                    // Small packet
                    size_t small_size = (size - data_offset) > 64 ? 64 : (size - data_offset);
                    pcap_inject(handle, data + data_offset, small_size);
                    
                    // Medium packet
                    if (size - data_offset > 128) {
                        size_t medium_size = (size - data_offset) > 512 ? 512 : (size - data_offset);
                        pcap_inject(handle, data + data_offset, medium_size);
                    }
                    
                    // Larger packet (up to MTU)
                    if (size - data_offset > 512) {
                        size_t large_size = (size - data_offset) > 1500 ? 1500 : (size - data_offset);
                        pcap_inject(handle, data + data_offset, large_size);
                    }
                }
                
                // Also test pcap_next_ex
                int ret = pcap_next_ex(handle, &pkt_header, &pkt_data);
                (void)ret;
                
                pcap_close(handle);
            }
            break;
        }
    }
    
    return 0;
}