#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_005 {
/*
 * libpcap fuzzing harness - harness_005.cpp
 * Targets timestamp handling subsystem APIs with complete invocation sequence
 * 
 * Primary APIs targeted (7 uncovered APIs identified by CoverageAnalyzerAgent):
 * - pcap_set_tstamp_type (timestamp type configuration)
 * - pcap_list_tstamp_types (list available timestamp types)
 * - pcap_tstamp_type_val_to_description (value to description conversion)
 * - pcap_tstamp_type_val_to_name (value to name conversion)
 * - pcap_tstamp_type_name_to_val (name to value conversion)
 * - pcap_set_tstamp_precision (timestamp precision configuration)
 * - pcap_get_tstamp_precision (get current timestamp precision)
 * 
 * Differentiates from existing harnesses:
 * - harness_000: Core packet capture APIs (uses pcap_open_dead for filtering)
 * - harness_001: BPF filter handling functions  
 * - harness_002: File I/O subsystem operations
 * - harness_003: Live device management lifecycle
 * - harness_004: Savefile-specific APIs (tests only pcap_set_tstamp_precision/pcap_get_tstamp_precision)
 * - harness_005: Comprehensive timestamp handling subsystem (THIS HARNESS)
 * 
 * Specifically targets the 54+ undiscovered branches in timestamp handling
 * subsystem identified by CoverageAnalyzerAgent. Focuses on timestamp type
 * APIs NOT covered by harness_004.
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
    
    // Timestamp-related parameters
    int tstamp_type = fdp.ConsumeIntegralInRange<int>(0, 5);  // PCAP_TSTAMP_* values (0-5)
    int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=micro, 1=nano
    
    // Consume string for timestamp type name conversion tests
    std::string tstamp_name = fdp.ConsumeRandomLengthString(64);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Test 1: Timestamp type name/value conversion APIs (standalone, don't need pcap handle)
    // These can be tested independently of any pcap handle
    
    // Test pcap_tstamp_type_name_to_val
    int name_to_val_result = pcap_tstamp_type_name_to_val(tstamp_name.c_str());
    (void)name_to_val_result;
    
    // Test pcap_tstamp_type_val_to_name for valid timestamp type values
    for (int val = 0; val <= 5; val++) {
        const char* val_to_name_result = pcap_tstamp_type_val_to_name(val);
        (void)val_to_name_result;
    }
    
    // Test pcap_tstamp_type_val_to_description for valid timestamp type values
    for (int val = 0; val <= 5; val++) {
        const char* val_to_desc_result = pcap_tstamp_type_val_to_description(val);
        (void)val_to_desc_result;
    }
    
    // Test 2: Create a pcap handle for timestamp configuration tests
    // Use pcap_create + pcap_activate for live device simulation
    // or pcap_open_dead for simpler testing
    
    // Option 1: Try pcap_create with dummy device first
    pcap_t* pcap_handle = pcap_create("any", errbuf);
    bool handle_activated = false;
    
    if (pcap_handle != nullptr) {
        // Set basic parameters before activation
        int set_snaplen_result = pcap_set_snaplen(pcap_handle, snaplen);
        (void)set_snaplen_result;
        
        int set_promisc_result = pcap_set_promisc(pcap_handle, 0);  // Non-promiscuous
        (void)set_promisc_result;
        
        int set_timeout_result = pcap_set_timeout(pcap_handle, 1000);  // 1 second timeout
        (void)set_timeout_result;
        
        // Test timestamp precision APIs before activation
        int set_precision_result = pcap_set_tstamp_precision(pcap_handle, tstamp_precision);
        (void)set_precision_result;
        
        int get_precision_result = pcap_get_tstamp_precision(pcap_handle);
        (void)get_precision_result;
        
        // Try to set timestamp type before activation
        int set_tstamp_type_result = pcap_set_tstamp_type(pcap_handle, tstamp_type);
        (void)set_tstamp_type_result;
        
        // Try to activate the handle
        int activate_result = pcap_activate(pcap_handle);
        if (activate_result >= 0) {
            handle_activated = true;
            
            // After activation, test timestamp APIs again
            int set_tstamp_type_after = pcap_set_tstamp_type(pcap_handle, tstamp_type);
            (void)set_tstamp_type_after;
            
            int set_precision_after = pcap_set_tstamp_precision(pcap_handle, tstamp_precision);
            (void)set_precision_after;
            
            int get_precision_after = pcap_get_tstamp_precision(pcap_handle);
            (void)get_precision_after;
            
            // Test pcap_list_tstamp_types - requires activated handle
            int* tstamp_types = nullptr;
            int list_result = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
            
            if (list_result > 0 && tstamp_types != nullptr) {
                // Free the timestamp types list
                pcap_free_tstamp_types(tstamp_types);
            }
        }
    }
    
    // If pcap_create failed or handle couldn't be activated,
    // try pcap_open_dead as fallback
    if (pcap_handle == nullptr || !handle_activated) {
        if (pcap_handle != nullptr) {
            pcap_close(pcap_handle);
        }
        
        // Test 3: Use pcap_open_dead (simpler, always works)
        pcap_handle = pcap_open_dead(linktype, snaplen);
        if (pcap_handle == nullptr) {
            return 0;
        }
        
        // Test timestamp precision APIs on dead handle
        int set_precision_dead = pcap_set_tstamp_precision(pcap_handle, tstamp_precision);
        (void)set_precision_dead;
        
        int get_precision_dead = pcap_get_tstamp_precision(pcap_handle);
        (void)get_precision_dead;
        
        // Try to set timestamp type on dead handle
        // Note: pcap_set_tstamp_type may fail on dead handles, but we test the API call
        int set_tstamp_type_dead = pcap_set_tstamp_type(pcap_handle, tstamp_type);
        (void)set_tstamp_type_dead;
        
        // Test pcap_list_tstamp_types on dead handle
        int* tstamp_types_dead = nullptr;
        int list_result_dead = pcap_list_tstamp_types(pcap_handle, &tstamp_types_dead);
        
        if (list_result_dead > 0 && tstamp_types_dead != nullptr) {
            // Free the timestamp types list
            pcap_free_tstamp_types(tstamp_types_dead);
        }
    }
    
    // Test 4: Additional edge cases with remaining fuzzer input
    if (fdp.remaining_bytes() > 0) {
        // Test timestamp type name conversion with random strings
        std::string random_name = fdp.ConsumeRandomLengthString(32);
        int random_name_to_val = pcap_tstamp_type_name_to_val(random_name.c_str());
        (void)random_name_to_val;
        
        // Test timestamp type value conversions with random values
        if (fdp.remaining_bytes() >= sizeof(int)) {
            int random_val = fdp.ConsumeIntegral<int>();
            
            // Safe absolute value calculation to avoid integer overflow with INT_MIN
            unsigned int abs_val;
            if (random_val < 0) {
                // Handle INT_MIN specially to avoid overflow when negating
                if (random_val == INT_MIN) {
                    abs_val = (unsigned int)INT_MAX + 1;  // Safe: 2147483648
                } else {
                    abs_val = (unsigned int)(-random_val);
                }
            } else {
                abs_val = (unsigned int)random_val;
            }
            abs_val = abs_val % 100;  // Limit to reasonable range
            
            const char* random_val_to_name = pcap_tstamp_type_val_to_name((int)abs_val);
            (void)random_val_to_name;
            
            const char* random_val_to_desc = pcap_tstamp_type_val_to_description((int)abs_val);
            (void)random_val_to_desc;
        }
        
        // Test timestamp precision with various values
        if (fdp.remaining_bytes() >= sizeof(int)) {
            // Use unsigned modulo to avoid negative results from signed modulo
            int random_precision = (unsigned int)fdp.ConsumeIntegral<int>() % 4;  // 0-3
            int set_random_precision = pcap_set_tstamp_precision(pcap_handle, random_precision);
            (void)set_random_precision;
            
            int get_random_precision = pcap_get_tstamp_precision(pcap_handle);
            (void)get_random_precision;
        }
        
        // Test timestamp type setting with various values
        if (fdp.remaining_bytes() >= sizeof(int)) {
            // Use unsigned modulo to avoid negative results from signed modulo
            int random_tstamp_type = (unsigned int)fdp.ConsumeIntegral<int>() % 10;  // 0-9
            int set_random_tstamp_type = pcap_set_tstamp_type(pcap_handle, random_tstamp_type);
            (void)set_random_tstamp_type;
        }
    }
    
    // Clean up
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
    }
    
    return 0;
}

} // namespace harness_005
