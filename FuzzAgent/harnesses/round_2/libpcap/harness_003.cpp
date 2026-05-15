#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

namespace harness_003 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least operation mode (1 byte), 
    // snaplen (2 bytes), promisc flag (1 byte), timeout (2 bytes),
    // buffer size (2 bytes), filter length (1 byte) = 9 bytes
    if (size < 9) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode: 0 = use pcap_lookupdev, 1 = use pcap_findalldevs
    uint8_t dev_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume capture parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(64, 65535);  // Reasonable snaplen range
    int promisc = fdp.ConsumeBool() ? 1 : 0;  // Promiscuous mode flag
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);  // Timeout in milliseconds
    int buffer_size = fdp.ConsumeIntegralInRange<int>(0, 1048576);  // Buffer size (0 means default)
    
    // Consume filter string
    size_t filter_len = fdp.ConsumeIntegralInRange<size_t>(0, 512);
    std::string filter_str = fdp.ConsumeBytesAsString(filter_len);
    
    // Consume number of packets to capture (0-10)
    int packet_count = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // Remaining bytes are for test packets (if any)
    std::vector<uint8_t> test_packets = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    struct bpf_program bpf;
    pcap_if_t *alldevs = nullptr;
    const char *device = nullptr;
    
    // Step 1: Device discovery
    if (dev_mode == 0) {
        // Use pcap_lookupdev - deprecated but still part of API
        device = pcap_lookupdev(errbuf);
        if (device == nullptr) {
            // If no device found, use a dummy device name
            device = "lo";  // Loopback interface as fallback
        }
    } else {
        // Use pcap_findalldevs
        if (pcap_findalldevs(&alldevs, errbuf) == -1) {
            // Device discovery failed, use dummy device
            device = "lo";
        } else if (alldevs != nullptr) {
            // Use the first device found
            device = alldevs->name;
        } else {
            // No devices found
            device = "lo";
        }
    }
    
    // Step 2: Create live capture handle
    pcap_handle = pcap_open_live(device, snaplen, promisc, timeout_ms, errbuf);
    
    if (pcap_handle == nullptr) {
        // If live capture fails, try with default parameters as fallback
        pcap_handle = pcap_open_live("lo", 65535, 0, 1000, errbuf);
        if (pcap_handle == nullptr) {
            // If still fails, use pcap_open_dead for testing APIs
            pcap_handle = pcap_open_dead(1, snaplen);  // DLT_EN10MB
        }
    }
    
    if (pcap_handle == nullptr) {
        // Clean up device list if we used pcap_findalldevs
        if (alldevs != nullptr) {
            pcap_freealldevs(alldevs);
        }
        return 0;  // Can't proceed without a handle
    }
    
    // Step 3: Test optional configuration APIs
    // These may fail on some handles (especially dead handles), but we test them anyway
    
    // Test pcap_set_datalink
    pcap_set_datalink(pcap_handle, DLT_EN10MB);  // Try setting to Ethernet
    
    // Test pcap_set_snaplen if different from initial value
    if (snaplen != 65535) {
        pcap_set_snaplen(pcap_handle, snaplen);
    }
    
    // Test pcap_set_promisc
    pcap_set_promisc(pcap_handle, promisc);
    
    // Test pcap_set_timeout if different from initial value  
    if (timeout_ms != 1000) {
        pcap_set_timeout(pcap_handle, timeout_ms);
    }
    
    // Test pcap_set_buffer_size if specified
    if (buffer_size > 0) {
        pcap_set_buffer_size(pcap_handle, buffer_size);
    }
    
    // Step 4: Setup BPF filter if filter string is not empty
    if (!filter_str.empty()) {
        bpf_u_int32 netmask = PCAP_NETMASK_UNKNOWN;
        
        // Try to compile the filter
        if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, netmask) == 0) {
            // Try to set the filter
            if (pcap_setfilter(pcap_handle, &bpf) == 0) {
                // Filter set successfully
            } else {
                // Filter set failed - get error
                const char* filter_err = pcap_geterr(pcap_handle);
            }
            
            // Free the compiled filter
            pcap_freecode(&bpf);
        } else {
            // Compilation failed - get error
            const char* compile_err = pcap_geterr(pcap_handle);
        }
    }
    
    // Step 5: Test packet reading APIs
    
    // Test pcap_next (single packet)
    struct pcap_pkthdr header;
    const u_char *packet = pcap_next(pcap_handle, &header);
    // pcap_next returns NULL if no packet available or error
    
    // Test pcap_loop with a simple packet handler
    if (packet_count > 0) {
        // Define a simple packet handler callback
        auto packet_handler = [](u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
            // Simple handler that does nothing
            int *count = (int *)user;
            (*count)++;
        };
        
        int count = 0;
        // Call pcap_loop with limited packet count
        pcap_loop(pcap_handle, packet_count, packet_handler, (u_char *)&count);
    }
    
    // Test pcap_dispatch (similar to pcap_loop but returns after timeout)
    if (timeout_ms > 0) {
        auto dispatch_handler = [](u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
            // Simple handler
            int *count = (int *)user;
            (*count)++;
        };
        
        int dispatch_count = 0;
        pcap_dispatch(pcap_handle, 1, dispatch_handler, (u_char *)&dispatch_count);
    }
    
    // Test pcap_next_ex (extended version)
    struct pcap_pkthdr *header_ptr;
    const u_char *packet_ptr;
    int next_ex_result = pcap_next_ex(pcap_handle, &header_ptr, &packet_ptr);
    // next_ex_result: 1=packet read, 0=timeout, -1=error, -2=EOF (for files)
    
    // Step 6: Test other utility functions
    
    // Test pcap_datalink
    int datalink = pcap_datalink(pcap_handle);
    
    // Test pcap_snapshot
    int snapshot = pcap_snapshot(pcap_handle);
    
    // Test pcap_is_swapped
    int swapped = pcap_is_swapped(pcap_handle);
    
    // Test pcap_geterr
    const char* pcap_error = pcap_geterr(pcap_handle);
    
    // Test pcap_stats (may fail on some handles)
    struct pcap_stat stats;
    pcap_stats(pcap_handle, &stats);
    
    // Test pcap_get_selectable_fd (POSIX systems)
    #ifndef _WIN32
    int selectable_fd = pcap_get_selectable_fd(pcap_handle);
    #endif
    
    // Step 7: Cleanup
    
    // Close the pcap handle
    pcap_close(pcap_handle);
    
    // Free device list if we used pcap_findalldevs
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    return 0;
}

} // namespace harness_003
