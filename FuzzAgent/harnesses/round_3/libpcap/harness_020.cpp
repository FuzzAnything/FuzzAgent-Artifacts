#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_020 {
/*
 * Harness for libpcap timestamp precision operations
 * Primary Target: pcap_get_tstamp_precision (0% covered) and related timestamp APIs
 * Targets: pcap_get_tstamp_precision, pcap_set_tstamp_precision, pcap_list_tstamp_types,
 *          pcap_free_tstamp_types, pcap_tstamp_type_val_to_description,
 *          pcap_tstamp_type_val_to_name, pcap_tstamp_type_name_to_val
 * Coverage goal: Exercise critically under-covered timestamp precision functions
 *                with 12+ undiscovered branches across related functions
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful timestamp precision testing
    // Need enough data for control parameters + device name + error conditions
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Consume control parameters for test configuration
    int test_mode = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which test path to take
    int use_open_live = fdp.ConsumeBool() ? 1 : 0;  // Use pcap_open_live vs pcap_create
    int test_error_conditions = fdp.ConsumeBool() ? 1 : 0;  // Test error paths
    
    // Parameters for pcap handle creation
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 1000);
    
    // Device name (could be dummy device like "any" or "lo")
    std::string device_name;
    if (test_error_conditions && fdp.ConsumeBool()) {
        // Test with invalid device name
        device_name = fdp.ConsumeRandomLengthString(256);
    } else {
        // Use common dummy device names
        const char* dummy_devices[] = {"any", "lo", "eth0", "wlan0"};
        size_t device_idx = fdp.ConsumeIntegralInRange<size_t>(0, 3);
        device_name = dummy_devices[device_idx];
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = nullptr;
    
    // Phase 2: Create pcap handle using different methods based on test_mode
    if (use_open_live) {
        // Method 1: Use pcap_open_live (creates and activates in one call)
        pcap = pcap_open_live(device_name.c_str(), snaplen, promisc, timeout_ms, errbuf);
    } else {
        // Method 2: Use pcap_create + pcap_activate
        pcap = pcap_create(device_name.c_str(), errbuf);
        if (pcap != nullptr) {
            // Set some options before activation
            pcap_set_snaplen(pcap, snaplen);
            pcap_set_promisc(pcap, promisc);
            pcap_set_timeout(pcap, timeout_ms);
            
            // Try to activate
            int activate_result = pcap_activate(pcap);
            if (activate_result < 0 && activate_result != PCAP_WARNING) {
                // Activation failed, clean up
                pcap_close(pcap);
                pcap = nullptr;
            }
        }
    }
    
    if (pcap == nullptr) {
        // Handle creation failed, but that's OK - fuzzer explores error paths
        return 0;
    }
    
    // Phase 3: Test timestamp precision APIs based on test_mode
    switch (test_mode) {
        case 0: {
            // Test 1: Basic precision setting and getting
            // Consume precision values from input
            int precision_val = fdp.ConsumeIntegralInRange<int>(0, 3);
            
            // Try to set the precision (may fail if unsupported)
            int set_result = pcap_set_tstamp_precision(pcap, precision_val);
            
            // Always get the current precision
            int get_result = pcap_get_tstamp_precision(pcap);
            
            // If set succeeded, the get should match (unless precision_val is invalid)
            if (set_result == 0) {
                // Set succeeded, precision should be what we set
                // (This is just for coverage, we don't assert)
                (void)get_result;
            }
            break;
        }
        
        case 1: {
            // Test 2: Enumerate timestamp types and convert between representations
            int* tstamp_types = nullptr;
            int tstamp_type_count = pcap_list_tstamp_types(pcap, &tstamp_types);
            
            if (tstamp_type_count > 0 && tstamp_types != nullptr) {
                // Test each timestamp type with conversion functions
                for (int i = 0; i < tstamp_type_count; i++) {
                    int tstamp_type = tstamp_types[i];
                    
                    // Convert value to name
                    const char* type_name = pcap_tstamp_type_val_to_name(tstamp_type);
                    
                    // Convert value to description
                    const char* type_desc = pcap_tstamp_type_val_to_description(tstamp_type);
                    
                    // If we got a name, try to convert it back to value
                    if (type_name != nullptr) {
                        int roundtrip_val = pcap_tstamp_type_name_to_val(type_name);
                        // roundtrip_val should equal tstamp_type if mapping is consistent
                        (void)roundtrip_val;
                    }
                    
                    // Also test with some random strings from fuzzer input
                    if (fdp.remaining_bytes() > 0) {
                        std::string random_name = fdp.ConsumeRandomLengthString(64);
                        int random_val = pcap_tstamp_type_name_to_val(random_name.c_str());
                        // random_val will likely be PCAP_ERROR, but that's OK for coverage
                        (void)random_val;
                    }
                }
                
                // Free the timestamp types list
                pcap_free_tstamp_types(tstamp_types);
            }
            break;
        }
        
        case 2: {
            // Test 3: Combined precision and timestamp type operations
            // First set precision to microsecond or nanosecond based on input
            int precision_choice = fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
            pcap_set_tstamp_precision(pcap, precision_choice);
            
            // Get current precision
            int current_precision = pcap_get_tstamp_precision(pcap);
            (void)current_precision;
            
            // List timestamp types
            int* tstamp_types = nullptr;
            int tstamp_type_count = pcap_list_tstamp_types(pcap, &tstamp_types);
            
            if (tstamp_type_count > 0 && tstamp_types != nullptr) {
                // Test a few conversions
                for (int i = 0; i < tstamp_type_count && i < 3; i++) {
                    const char* name = pcap_tstamp_type_val_to_name(tstamp_types[i]);
                    const char* desc = pcap_tstamp_type_val_to_description(tstamp_types[i]);
                    (void)name;
                    (void)desc;
                }
                
                pcap_free_tstamp_types(tstamp_types);
            }
            break;
        }
        
        case 3: {
            // Test 4: Error condition testing
            // Test with NULL handle (should crash or return error in real API)
            // We'll test with invalid precision values instead
            int invalid_precision = fdp.ConsumeIntegralInRange<int>(-100, -1);
            int set_invalid_result = pcap_set_tstamp_precision(pcap, invalid_precision);
            // Should return PCAP_ERROR_TSTAMP_PRECISION_NOTSUP for negative values
            (void)set_invalid_result;
            
            // Also test with very large precision values
            int large_precision = fdp.ConsumeIntegralInRange<int>(100, 1000);
            int set_large_result = pcap_set_tstamp_precision(pcap, large_precision);
            // Likely to fail unless somehow supported
            (void)set_large_result;
            
            // Test pcap_list_tstamp_types with potential malloc failure simulation
            // (We can't simulate malloc failure, but we test the API)
            int* tstamp_types = nullptr;
            int count = pcap_list_tstamp_types(pcap, &tstamp_types);
            if (count > 0 && tstamp_types != nullptr) {
                pcap_free_tstamp_types(tstamp_types);
            }
            break;
        }
    }
    
    // Phase 4: Test pcap_set_tstamp_type if we have remaining input
    if (fdp.remaining_bytes() > 0 && test_mode != 3) {
        // Consume a timestamp type value
        int tstamp_type_val = fdp.ConsumeIntegralInRange<int>(0, 10);
        pcap_set_tstamp_type(pcap, tstamp_type_val);
    }
    
    // Phase 5: Cleanup
    pcap_close(pcap);
    
    return 0;
}

} // namespace harness_020
