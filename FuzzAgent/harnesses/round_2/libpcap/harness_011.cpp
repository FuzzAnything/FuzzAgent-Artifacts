#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>

namespace harness_011 {

// Packet handler callback for pcap_loop and pcap_dispatch
static void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Simple packet handler that does nothing
    // In real fuzzing, we might want to analyze packet data
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    int result;
    
    // Consume operation mode: 0 = pcap_lookupdev, 1 = fuzzed device name, 2 = invalid device
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume capture parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(64, 65535);
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000); // 0-10 seconds
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576); // 1KB to 1MB
    
    // Consume operation parameters for capture functions
    int loop_count = fdp.ConsumeIntegralInRange<int>(-10, 100); // Negative, 0, positive
    int dispatch_count = fdp.ConsumeIntegralInRange<int>(0, 100);
    
    // Consume device name if not using pcap_lookupdev
    std::string device_name;
    if (mode == 1) {
        // Use fuzzed device name
        device_name = fdp.ConsumeRandomLengthString(256);
    } else if (mode == 2) {
        // Invalid device name
        device_name = fdp.ConsumeRandomLengthString(256);
        // Make it likely invalid by adding null bytes or special chars
        if (!device_name.empty() && fdp.ConsumeBool()) {
            device_name[0] = '\0';
        }
    }
    
    // Phase 1: Device discovery
    char *lookup_dev = nullptr;
    if (mode == 0) {
        // Try pcap_lookupdev (deprecated but we want to test it)
        lookup_dev = pcap_lookupdev(errbuf);
        if (lookup_dev != nullptr) {
            device_name = lookup_dev;
        } else {
            // pcap_lookupdev failed, fall back to "any" or loopback
            device_name = "any";
        }
    }
    
    // Phase 2: Open device (try different methods)
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    if (open_method == 0) {
        // Method 1: pcap_open_live
        pcap_handle = pcap_open_live(device_name.c_str(), snaplen, promisc, timeout_ms, errbuf);
    } else {
        // Method 2: pcap_create + pcap_activate
        pcap_handle = pcap_create(device_name.c_str(), errbuf);
        if (pcap_handle != nullptr) {
            // Configure before activate
            result = pcap_set_snaplen(pcap_handle, snaplen);
            result = pcap_set_promisc(pcap_handle, promisc);
            result = pcap_set_timeout(pcap_handle, timeout_ms);
            
            // Try to set buffer size (may fail on some platforms)
            result = pcap_set_buffer_size(pcap_handle, buffer_size);
            
            // Activate the capture
            result = pcap_activate(pcap_handle);
            if (result < 0) {
                // Activation failed, but we still have a handle for testing
                // Don't close yet, we want to test error paths
            }
        }
    }
    
    // If we couldn't open any device, try pcap_open_dead as fallback
    if (pcap_handle == nullptr) {
        // Create a dead handle for testing APIs that don't need live capture
        pcap_handle = pcap_open_dead(DLT_EN10MB, snaplen);
        if (pcap_handle == nullptr) {
            return 0; // Can't even create dead handle
        }
        
        // Test buffer size on dead handle (should work)
        result = pcap_set_buffer_size(pcap_handle, buffer_size);
        
        // Test other configuration APIs on dead handle
        const char* err = pcap_geterr(pcap_handle);
        
        pcap_close(pcap_handle);
        return 0;
    }
    
    // Phase 3: Configure device (if opened successfully)
    if (pcap_handle != nullptr) {
        // Test pcap_set_buffer_size
        result = pcap_set_buffer_size(pcap_handle, buffer_size);
        
        // Test getting error string (even if no error)
        const char* err = pcap_geterr(pcap_handle);
        
        // Test other informational APIs
        int datalink = pcap_datalink(pcap_handle);
        int snapshot = pcap_snapshot(pcap_handle);
        int swapped = pcap_is_swapped(pcap_handle);
        int major = pcap_major_version(pcap_handle);
        int minor = pcap_minor_version(pcap_handle);
        
        // Phase 4: Capture operations
        uint8_t capture_method = fdp.ConsumeIntegral<uint8_t>() % 3;
        
        switch (capture_method) {
            case 0:
                // Test pcap_next
                if (loop_count > 0) {
                    for (int i = 0; i < loop_count && i < 10; i++) {
                        struct pcap_pkthdr header;
                        const u_char *packet = pcap_next(pcap_handle, &header);
                        // packet may be null if no packet available
                        if (packet == nullptr) {
                            break;
                        }
                    }
                }
                break;
                
            case 1:
                // Test pcap_loop
                if (loop_count != 0) {
                    // pcap_loop with 0 means loop forever, negative means error
                    // Use absolute value with reasonable limit
                    int actual_count = loop_count;
                    if (actual_count < 0) {
                        actual_count = -actual_count;
                    }
                    if (actual_count > 100) {
                        actual_count = 100;
                    }
                    
                    u_char user_data[16] = {0};
                    result = pcap_loop(pcap_handle, actual_count, packet_handler, user_data);
                    // result may be -1, 0, or >0
                }
                break;
                
            case 2:
                // Test pcap_dispatch
                if (dispatch_count > 0) {
                    u_char user_data[16] = {0};
                    result = pcap_dispatch(pcap_handle, dispatch_count, packet_handler, user_data);
                    // result may be -1, 0, or >0
                }
                break;
        }
        
        // Test pcap_stats (may fail on some handles)
        struct pcap_stat stats;
        result = pcap_stats(pcap_handle, &stats);
        
        // Test non-blocking mode
        int nonblock = 0;
        result = pcap_getnonblock(pcap_handle, errbuf);
        if (result == 0) {
            // Try to set non-blocking mode
            int new_nonblock = fdp.ConsumeBool() ? 1 : 0;
            result = pcap_setnonblock(pcap_handle, new_nonblock, errbuf);
        }
        
        // Phase 5: Cleanup
        pcap_close(pcap_handle);
    }
    
    // Test pcap_lookupnet if we have a valid device name
    if (!device_name.empty() && device_name[0] != '\0') {
        bpf_u_int32 netp, maskp;
        result = pcap_lookupnet(device_name.c_str(), &netp, &maskp, errbuf);
    }
    
    return 0;
}

} // namespace harness_011
