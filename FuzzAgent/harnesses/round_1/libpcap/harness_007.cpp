#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <climits>

namespace harness_007 {
/*
 * libpcap fuzzing harness - harness_007.cpp
 * Targets datalink configuration subsystem APIs with complete invocation sequence
 * 
 * Primary APIs targeted (datalink configuration functions):
 * - pcap_datalink (get current datalink type)
 * - pcap_datalink_ext (get extended datalink type)
 * - pcap_list_datalinks (list available datalink types for device)
 * - pcap_set_datalink (set datalink type for device)
 * - pcap_free_datalinks (free datalink list memory)
 * - pcap_datalink_name_to_val (convert datalink name to DLT value)
 * - pcap_datalink_val_to_name (convert DLT value to name)
 * - pcap_datalink_val_to_description (convert DLT value to description)
 * - pcap_datalink_val_to_description_or_dlt (convert DLT value to description or "DLT N")
 * 
 * Differentiates from existing harnesses:
 * - harness_000: Core packet capture APIs (uses pcap_open_dead for filtering)
 * - harness_001: BPF filter handling functions  
 * - harness_002: File I/O subsystem operations
 * - harness_003: Live device management lifecycle
 * - harness_004: Savefile-specific APIs with timestamp precision
 * - harness_005: Timestamp handling subsystem APIs
 * - harness_006: Network address translation subsystem
 * - harness_007: Datalink configuration subsystem (THIS HARNESS)
 * 
 * Specifically targets the 38+ undiscovered branches in datalink configuration
 * subsystem identified by CoverageAnalyzerAgent. Focuses on datalink type
 * enumeration, configuration, and validation functions NOT covered by previous harnesses.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int target_dlt = fdp.ConsumeIntegralInRange<int>(-10, 300);  // Test invalid values too
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 10);  // Which test sequence to run
    
    // Consume strings for datalink name conversion tests
    std::string dlt_name_str = fdp.ConsumeRandomLengthString(128);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Test 1: Standalone datalink name/value conversion APIs (don't need pcap handle)
    // These can be tested independently of any pcap handle
    
    // Test pcap_datalink_name_to_val
    int name_to_val_result = pcap_datalink_name_to_val(dlt_name_str.c_str());
    (void)name_to_val_result;  // Use result to avoid unused variable warning
    
    // Test pcap_datalink_val_to_name for a range of DLT values
    // Test both valid and invalid values to explore error paths
    for (int dlt_val = -5; dlt_val <= 300; dlt_val += 10) {
        const char* val_to_name_result = pcap_datalink_val_to_name(dlt_val);
        (void)val_to_name_result;
    }
    
    // Test pcap_datalink_val_to_description for a range of DLT values
    for (int dlt_val = -5; dlt_val <= 300; dlt_val += 10) {
        const char* val_to_desc_result = pcap_datalink_val_to_description(dlt_val);
        (void)val_to_desc_result;
    }
    
    // Test pcap_datalink_val_to_description_or_dlt for a range of DLT values
    for (int dlt_val = -5; dlt_val <= 300; dlt_val += 10) {
        const char* val_to_desc_or_dlt_result = pcap_datalink_val_to_description_or_dlt(dlt_val);
        (void)val_to_desc_or_dlt_result;
    }
    
    // Test 2: Create pcap handles for datalink configuration tests
    // We'll test with different handle types to explore different code paths
    
    // Option A: pcap_open_dead (simplest, always works)
    pcap_t* dead_handle = pcap_open_dead(linktype, snaplen);
    
    if (dead_handle != nullptr) {
        // Test pcap_datalink on dead handle
        int current_dlt = pcap_datalink(dead_handle);
        (void)current_dlt;
        
        // Test pcap_datalink_ext on dead handle
        int current_dlt_ext = pcap_datalink_ext(dead_handle);
        (void)current_dlt_ext;
        
        // Test pcap_list_datalinks on dead handle
        int* dlt_list = nullptr;
        int dlt_count = pcap_list_datalinks(dead_handle, &dlt_list);
        
        if (dlt_count > 0 && dlt_list != nullptr) {
            // Test pcap_set_datalink with each available DLT
            for (int i = 0; i < dlt_count && i < 10; i++) {  // Limit to 10 to avoid excessive loops
                int set_result = pcap_set_datalink(dead_handle, dlt_list[i]);
                (void)set_result;
                
                // Verify the datalink was set (or at least check it doesn't crash)
                int new_dlt = pcap_datalink(dead_handle);
                (void)new_dlt;
            }
            
            // Test pcap_set_datalink with invalid DLT values
            int invalid_set_result = pcap_set_datalink(dead_handle, -1);
            (void)invalid_set_result;
            
            int invalid_set_result2 = pcap_set_datalink(dead_handle, 9999);
            (void)invalid_set_result2;
            
            // Free the datalink list
            pcap_free_datalinks(dlt_list);
        }
        
        pcap_close(dead_handle);
    }
    
    // Option B: pcap_create + pcap_activate for more realistic testing
    // (if platform supports it)
    if (fdp.remaining_bytes() > 0) {
        std::string device_name = fdp.ConsumeRandomLengthString(64);
        pcap_t* live_handle = pcap_create(device_name.empty() ? "any" : device_name.c_str(), errbuf);
        
        if (live_handle != nullptr) {
            // Set basic parameters before activation
            int set_snaplen_result = pcap_set_snaplen(live_handle, snaplen);
            (void)set_snaplen_result;
            
            int set_promisc_result = pcap_set_promisc(live_handle, 0);  // Non-promiscuous
            (void)set_promisc_result;
            
            int set_timeout_result = pcap_set_timeout(live_handle, 1000);  // 1 second timeout
            (void)set_timeout_result;
            
            // Try to activate the handle
            int activate_result = pcap_activate(live_handle);
            
            if (activate_result >= 0) {
                // Handle activated successfully, test datalink APIs
                
                // Test pcap_datalink on activated handle
                int current_dlt = pcap_datalink(live_handle);
                (void)current_dlt;
                
                // Test pcap_datalink_ext on activated handle
                int current_dlt_ext = pcap_datalink_ext(live_handle);
                (void)current_dlt_ext;
                
                // Test pcap_list_datalinks on activated handle
                int* dlt_list2 = nullptr;
                int dlt_count2 = pcap_list_datalinks(live_handle, &dlt_list2);
                
                if (dlt_count2 > 0 && dlt_list2 != nullptr) {
                    // Try to set datalink to a random value from the list
                    if (dlt_count2 > 0) {
                        int random_index = fdp.ConsumeIntegralInRange<int>(0, dlt_count2 - 1);
                        int set_dlt_result = pcap_set_datalink(live_handle, dlt_list2[random_index]);
                        (void)set_dlt_result;
                    }
                    
                    // Free the datalink list
                    pcap_free_datalinks(dlt_list2);
                }
                
                // Test pcap_set_datalink with the original target_dlt
                int set_target_result = pcap_set_datalink(live_handle, target_dlt);
                (void)set_target_result;
            } else {
                // Handle not activated, but we can still test some APIs
                // Note: pcap_datalink should return PCAP_ERROR_NOT_ACTIVATED
                int not_activated_dlt = pcap_datalink(live_handle);
                (void)not_activated_dlt;
                
                int not_activated_dlt_ext = pcap_datalink_ext(live_handle);
                (void)not_activated_dlt_ext;
            }
            
            pcap_close(live_handle);
        }
    }
    
    // Test 3: Additional edge cases based on test_type
    switch (test_type % 4) {
        case 0: {
            // Test with NULL handle for APIs that should handle it gracefully
            // (In practice, these would likely crash, but we're testing robustness)
            break;
        }
        
        case 1: {
            // Test datalink name conversion with empty string
            int empty_name_result = pcap_datalink_name_to_val("");
            (void)empty_name_result;
            
            // Test with very long string
            if (fdp.remaining_bytes() > 0) {
                size_t long_str_len = fdp.ConsumeIntegralInRange<size_t>(100, 1000);
                if (fdp.remaining_bytes() >= long_str_len) {
                    std::string long_str = fdp.ConsumeBytesAsString(long_str_len);
                    int long_name_result = pcap_datalink_name_to_val(long_str.c_str());
                    (void)long_name_result;
                }
            }
            break;
        }
        
        case 2: {
            // Test pcap_set_datalink with extreme values
            pcap_t* edge_handle = pcap_open_dead(DLT_EN10MB, 65535);
            if (edge_handle != nullptr) {
                // Test with INT_MAX and INT_MIN
                int max_int_set = pcap_set_datalink(edge_handle, INT_MAX);
                (void)max_int_set;
                
                int min_int_set = pcap_set_datalink(edge_handle, INT_MIN);
                (void)min_int_set;
                
                pcap_close(edge_handle);
            }
            break;
        }
        
        case 3: {
            // Test pcap_list_datalinks with NULL dlt_buffer parameter
            pcap_t* null_test_handle = pcap_open_dead(DLT_NULL, 1514);
            if (null_test_handle != nullptr) {
                // This should either return an error or crash, testing error handling
                // We don't actually pass NULL because that would be undefined behavior
                int* dummy_buffer = nullptr;
                int dummy_count = pcap_list_datalinks(null_test_handle, &dummy_buffer);
                (void)dummy_count;
                
                if (dummy_buffer != nullptr) {
                    pcap_free_datalinks(dummy_buffer);
                }
                
                pcap_close(null_test_handle);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_007
