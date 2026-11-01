// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// Static helper to create a dummy pcap file for testing pcap_open with file sources
static int create_dummy_pcap_file(const char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    
    // Write a minimal libpcap header (magic number + version)
    uint32_t magic = 0xa1b2c3d4;
    uint16_t version_major = 2;
    uint16_t version_minor = 4;
    int32_t thiszone = 0;
    uint32_t sigfigs = 0;
    uint32_t snaplen = 65535;
    uint32_t network = 1; // Ethernet
    
    struct pcap_header {
        uint32_t magic;
        uint16_t version_major;
        uint16_t version_minor;
        int32_t thiszone;
        uint32_t sigfigs;
        uint32_t snaplen;
        uint32_t network;
    } header = {
        .magic = magic,
        .version_major = version_major,
        .version_minor = version_minor,
        .thiszone = thiszone,
        .sigfigs = sigfigs,
        .snaplen = snaplen,
        .network = network
    };
    
    if (write(fd, &header, sizeof(header)) != sizeof(header)) {
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to work with
    if (Size < 7) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    
    // Use the input data to determine which API to test and how to configure it
    int test_case = Data[0] % 6;
    
    // Common parameters derived from input
    int snaplen = (Data[1] % 1000) + 64;
    int timeout = (Data[2] % 10000) + 1;
    int flags = (Data[3] % 8);
    int buffer_size = (Data[4] % 100000) + 1000;
    int immediate_mode = (Data[5] % 2);
    int sampling_value = (Data[6] % 100) + 1;
    
    // Create dummy file for file-based capture tests
    if (test_case == 2) { // pcap_open with file
        if (create_dummy_pcap_file("./dummy_file") != 0) {
            return 0;
        }
    }
    
    switch (test_case) {
        case 0: {
            // Test pcap_create + pcap_set_buffer_size + pcap_set_immediate_mode + cleanup
            handle = pcap_create(NULL, errbuf);
            if (handle != NULL) {
                // Try to set buffer size before activation
                pcap_set_buffer_size(handle, buffer_size);
                
                // Try to set immediate mode before activation
                pcap_set_immediate_mode(handle, immediate_mode);
                
                // Close the handle
                pcap_close(handle);
                handle = NULL;
            }
            break;
        }
        
        case 1: {
            // Test pcap_create + pcap_setsampling + cleanup
            handle = pcap_create(NULL, errbuf);
            if (handle != NULL) {
                // Try to set sampling (will likely fail on local, but tests the API)
                struct pcap_samp *samp = pcap_setsampling(handle);
                if (samp != NULL) {
                    samp->method = (Data[7] % 3);
                    samp->value = sampling_value;
                }
                
                // Close the handle
                pcap_close(handle);
                handle = NULL;
            }
            break;
        }
        
        case 2: {
            // Test pcap_open with file source
            handle = pcap_open("./dummy_file", snaplen, flags, timeout, NULL, errbuf);
            if (handle != NULL) {
                // Try to break loop (no-op if not in loop, but tests the API)
                pcap_breakloop(handle);
                
                // Close the handle
                pcap_close(handle);
                handle = NULL;
            }
            break;
        }
        
        case 3: {
            // Test pcap_create + multiple config + activation failure scenario
            handle = pcap_create(NULL, errbuf);
            if (handle != NULL) {
                pcap_set_buffer_size(handle, buffer_size);
                pcap_set_immediate_mode(handle, immediate_mode);
                
                // Try to set buffer size again (should work if not activated)
                pcap_set_buffer_size(handle, -1); // Negative value should be ignored
                
                // Close the handle without activating
                pcap_close(handle);
                handle = NULL;
            }
            break;
        }
        
        case 4: {
            // Test pcap_create + immediate mode after failed config
            handle = pcap_create(NULL, errbuf);
            if (handle != NULL) {
                // Try to set immediate mode
                pcap_set_immediate_mode(handle, immediate_mode);
                
                // Try to set buffer size
                pcap_set_buffer_size(handle, buffer_size);
                
                // Close the handle
                pcap_close(handle);
                handle = NULL;
            }
            break;
        }
        
        case 5: {
            // Test pcap_open with "any" source (may fail depending on permissions)
            handle = pcap_open("any", snaplen, flags, timeout, NULL, errbuf);
            if (handle != NULL) {
                // Try to break loop
                pcap_breakloop(handle);
                
                // Close the handle
                pcap_close(handle);
                handle = NULL;
            }
            break;
        }
    }
    
    // Ensure handle is closed if not already
    if (handle != NULL) {
        pcap_close(handle);
    }
    
    // Clean up dummy file if it exists
    unlink("./dummy_file");
    
    return 0;
}