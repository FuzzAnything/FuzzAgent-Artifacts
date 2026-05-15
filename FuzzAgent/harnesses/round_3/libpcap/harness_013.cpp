#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_013 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing of Linux monitor mode APIs
    // We need enough data for device name, rfmon settings, protocol values, and other parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size control parameters first
    // Test scenario selector to explore different code paths
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Monitor mode specific parameters
    int rfmon_enable = fdp.ConsumeBool() ? 1 : 0;               // Whether to enable monitor mode
    int rfmon_value = fdp.ConsumeIntegralInRange<int>(0, 1);    // 0 or 1 for pcap_set_rfmon
    
    // Linux-specific protocol value (for pcap_set_protocol_linux)
    int protocol_value = fdp.ConsumeIntegral<int>();           // Linux protocol value
    
    // Standard pcap configuration parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);   // Snapshot length (68 is minimum for Ethernet)
    int promisc_mode = fdp.ConsumeBool() ? 1 : 0;               // Promiscuous mode
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(-1, 10000); // Timeout in ms (-1 means infinite)
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576); // Buffer size
    
    // Device index for creating dummy interface names
    int device_idx = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // Consume device name string - generate dummy interface names for testing
    size_t dev_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string dev_name = fdp.ConsumeBytesAsString(dev_name_size);
    
    // Use different dummy interface names based on test scenario
    // This helps test different code paths in pcap-linux.c
    if (test_scenario == 0) {
        dev_name = "any";  // "any" device - should not support monitor mode
    } else if (test_scenario == 1) {
        dev_name = "eth" + std::to_string(device_idx % 5);  // Ethernet-like device
    } else if (test_scenario == 2) {
        dev_name = "wlan" + std::to_string(device_idx % 5);  // Wireless device
    } else if (test_scenario == 3) {
        dev_name = "mon" + std::to_string(device_idx % 10);  // Monitor interface
    }
    // test_scenario == 4 uses the fuzzed dev_name
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = nullptr;
    
    // Step 1: Create unactivated pcap handle with pcap_create()
    pcap = pcap_create(dev_name.c_str(), errbuf);
    if (pcap == nullptr) {
        // If pcap_create fails, we can still test error handling functions
        // Test pcap_strerror with dummy error code
        const char* error_str = pcap_strerror(PCAP_ERROR);
        (void)error_str;
        
        // Test pcap_geterr with NULL handle (should return NULL)
        const char* null_err = pcap_geterr(nullptr);
        (void)null_err;
        
        return 0;
    }
    
    // Step 2: Check monitor mode support with pcap_can_set_rfmon()
    // This is important for understanding the coverage guidance
    int can_set_rfmon = pcap_can_set_rfmon(pcap);
    (void)can_set_rfmon;  // Use result to avoid unused variable warning
    
    // Step 3: Enable monitor mode with pcap_set_rfmon(p, rfmon_value) - PRIMARY TARGET
    // According to coverage guidance, pcap_set_rfmon has 0 hits and 2 undiscovered branches
    // We need to test both enabling (1) and disabling (0) monitor mode
    int rfmon_result = pcap_set_rfmon(pcap, rfmon_value);
    (void)rfmon_result;
    
    // Step 4: Set Linux-specific protocol with pcap_set_protocol_linux()
    // This is Linux-specific and targets pcap-linux.c module
    #ifdef __linux__
    int protocol_result = pcap_set_protocol_linux(pcap, protocol_value);
    (void)protocol_result;
    #endif
    
    // Step 5: Configure standard options
    // These are standard pcap configuration functions that may interact with rfmon
    pcap_set_snaplen(pcap, snaplen);
    pcap_set_promisc(pcap, promisc_mode);
    pcap_set_timeout(pcap, timeout_ms);
    pcap_set_buffer_size(pcap, buffer_size);
    
    // Step 6: Try to get error information at various points
    // Test pcap_geterr to get error string from handle
    const char* pcap_error = pcap_geterr(pcap);
    (void)pcap_error;
    
    // Step 7: Activate handle with pcap_activate()
    // This is where Linux-specific activation paths in pcap-linux.c get exercised
    int activate_result = pcap_activate(pcap);
    
    // Step 8: If activation succeeds, optionally test packet reading with pcap_next_ex()
    // According to coverage guidance, pcap_next_ex has 0 hits and 4 undiscovered branches
    if (activate_result == 0) {
        // Try reading a few packets (if any are available)
        for (int i = 0; i < 3; i++) {
            struct pcap_pkthdr* pkthdr = nullptr;
            const u_char* pktdata = nullptr;
            
            int next_result = pcap_next_ex(pcap, &pkthdr, &pktdata);
            
            // Test different return values: 
            // 1 = packet read successfully
            // 0 = timeout expired
            // -1 = error occurred
            // -2 = loop terminated by pcap_breakloop()
            (void)next_result;
            
            // If we got a packet, test packet data access
            if (next_result == 1 && pkthdr != nullptr && pktdata != nullptr) {
                // Access some packet metadata to ensure coverage
                size_t caplen = pkthdr->caplen;
                size_t len = pkthdr->len;
                (void)caplen;
                (void)len;
            }
            
            // Test pcap_breakloop() to explore that code path
            if (i == 1 && test_scenario % 2 == 0) {
                pcap_breakloop(pcap);
            }
        }
        
        // Test pcap_stats() to get statistics
        struct pcap_stat stats;
        int stats_result = pcap_stats(pcap, &stats);
        (void)stats_result;
        
        // Test getting the file descriptor
        int pcap_fd = pcap_get_selectable_fd(pcap);
        (void)pcap_fd;
        
        // pcap_getevent is Windows-only, so we skip it on Linux
        // Test pcap_get_selectable_fd is already done above
    } else {
        // Activation failed - test error handling
        // Get more detailed error information
        const char* activate_error = pcap_geterr(pcap);
        (void)activate_error;
        
        // Test pcap_strerror with the activation error code
        const char* strerror_result = pcap_strerror(activate_result);
        (void)strerror_result;
    }
    
    // Step 9: Test additional Linux-specific configurations
    // Try setting datalink type (may fail depending on activation state)
    int datalink_val = fdp.ConsumeIntegralInRange<int>(0, 255);
    pcap_set_datalink(pcap, datalink_val);
    
    // Get current datalink type
    int current_datalink = pcap_datalink(pcap);
    (void)current_datalink;
    
    // Test pcap_datalink_val_to_name and pcap_datalink_name_to_val
    const char* dlt_name = pcap_datalink_val_to_name(current_datalink);
    (void)dlt_name;
    
    if (dlt_name != nullptr) {
        int dlt_val = pcap_datalink_name_to_val(dlt_name);
        (void)dlt_val;
    }
    
    // Test pcap_datalink_val_to_description
    const char* dlt_desc = pcap_datalink_val_to_description(current_datalink);
    (void)dlt_desc;
    
    // Step 10: Test pcap_is_swapped (endianness check)
    int is_swapped = pcap_is_swapped(pcap);
    (void)is_swapped;
    
    // Step 11: Test pcap_major_version and pcap_minor_version
    int major_version = pcap_major_version(pcap);
    int minor_version = pcap_minor_version(pcap);
    (void)major_version;
    (void)minor_version;
    
    // Step 12: Cleanup with pcap_close()
    // This should handle any monitor mode cleanup in pcap-linux.c
    pcap_close(pcap);
    
    return 0;
}

} // namespace harness_013
