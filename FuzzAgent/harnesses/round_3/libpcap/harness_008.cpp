#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_008 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful timestamp type testing
    // We need enough data for device name and control parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size control parameters first
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which test scenario to run
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);   // Snapshot length
    int promisc_mode = fdp.ConsumeBool() ? 1 : 0;               // Promiscuous mode
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000); // Timeout in ms
    int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1); // 0=micro, 1=nano
    
    // Consume device name string - use dummy interfaces for testing
    size_t dev_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string dev_name = fdp.ConsumeBytesAsString(dev_name_size);
    
    // Use "any" as a dummy interface if the generated name is invalid
    if (dev_name.empty() || dev_name.find('/') != std::string::npos) {
        dev_name = "any";
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = nullptr;
    
    // Step 1: pcap_create for handle creation
    pcap = pcap_create(dev_name.c_str(), errbuf);
    if (pcap == nullptr) {
        // If pcap_create fails, we can still test standalone timestamp functions
        // using pcap_open_dead_with_tstamp_precision or just test the conversion functions
        
        // Test standalone timestamp conversion functions with fuzzer data
        if (fdp.remaining_bytes() > 16) {
            size_t tstamp_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            std::string tstamp_name = fdp.ConsumeBytesAsString(tstamp_name_size);
            
            // Test pcap_tstamp_type_name_to_val
            int tstamp_val = pcap_tstamp_type_name_to_val(tstamp_name.c_str());
            
            // Test pcap_tstamp_type_val_to_name (with valid and invalid values)
            const char* name_result = pcap_tstamp_type_val_to_name(tstamp_val);
            (void)name_result;
            
            // Test pcap_tstamp_type_val_to_description
            const char* desc_result = pcap_tstamp_type_val_to_description(tstamp_val);
            (void)desc_result;
            
            // Test with known timestamp type values (0-5)
            for (int known_val = 0; known_val <= 5; known_val++) {
                const char* known_name = pcap_tstamp_type_val_to_name(known_val);
                const char* known_desc = pcap_tstamp_type_val_to_description(known_val);
                (void)known_name;
                (void)known_desc;
                
                // Try converting the name back to value
                if (known_name != nullptr) {
                    int back_val = pcap_tstamp_type_name_to_val(known_name);
                    (void)back_val;
                }
            }
        }
        
        return 0;
    }
    
    // Step 2: Configuration before activation
    pcap_set_snaplen(pcap, snaplen);
    pcap_set_promisc(pcap, promisc_mode);
    pcap_set_timeout(pcap, timeout_ms);
    pcap_set_tstamp_precision(pcap, tstamp_precision);
    
    // Step 3: pcap_activate for activation
    int activate_result = pcap_activate(pcap);
    
    // Only proceed with timestamp type testing if activation succeeded
    if (activate_result >= 0) {
        // Step 4: pcap_list_tstamp_types to get available types
        int* tstamp_types = nullptr;
        int tstamp_count = pcap_list_tstamp_types(pcap, &tstamp_types);
        
        if (tstamp_count > 0 && tstamp_types != nullptr) {
            // Process each timestamp type
            int types_to_process = std::min(tstamp_count, 10);  // Limit processing to avoid timeout
            
            for (int i = 0; i < types_to_process; i++) {
                int tstamp_type = tstamp_types[i];
                
                // Step 5: For each type, call conversion functions
                
                // pcap_tstamp_type_val_to_name
                const char* type_name = pcap_tstamp_type_val_to_name(tstamp_type);
                (void)type_name;
                
                // pcap_tstamp_type_val_to_description
                const char* type_desc = pcap_tstamp_type_val_to_description(tstamp_type);
                (void)type_desc;
                
                // pcap_tstamp_type_name_to_val (if we have a valid name)
                if (type_name != nullptr) {
                    int back_to_val = pcap_tstamp_type_name_to_val(type_name);
                    (void)back_to_val;
                }
                
                // Optionally try pcap_set_tstamp_type based on test scenario
                if (test_scenario == 0 || test_scenario == 2) {
                    // Only set timestamp type for certain scenarios
                    int set_result = pcap_set_tstamp_type(pcap, tstamp_type);
                    (void)set_result;
                    
                    // Verify current timestamp precision after setting
                    int current_precision = pcap_get_tstamp_precision(pcap);
                    (void)current_precision;
                }
                
                // Also test with some additional fuzzer-provided type names
                if (fdp.remaining_bytes() > 8) {
                    size_t custom_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                    std::string custom_name = fdp.ConsumeBytesAsString(custom_name_size);
                    
                    int custom_val = pcap_tstamp_type_name_to_val(custom_name.c_str());
                    (void)custom_val;
                    
                    if (custom_val >= 0) {
                        const char* custom_name_back = pcap_tstamp_type_val_to_name(custom_val);
                        const char* custom_desc = pcap_tstamp_type_val_to_description(custom_val);
                        (void)custom_name_back;
                        (void)custom_desc;
                    }
                }
            }
            
            // Test pcap_set_tstamp_type with various values from fuzzer input
            if (fdp.remaining_bytes() > 4) {
                int fuzzed_tstamp_type = fdp.ConsumeIntegralInRange<int>(0, 10);  // Some may be invalid
                int set_result = pcap_set_tstamp_type(pcap, fuzzed_tstamp_type);
                (void)set_result;
                
                // Try again with a known valid type if we have one
                if (tstamp_count > 0) {
                    int valid_type = tstamp_types[0];
                    set_result = pcap_set_tstamp_type(pcap, valid_type);
                    (void)set_result;
                }
            }
            
            // Step 6: pcap_free_tstamp_types to free the list
            pcap_free_tstamp_types(tstamp_types);
            tstamp_types = nullptr;
        } else {
            // No timestamp types available, but we can still test conversion functions
            if (fdp.remaining_bytes() > 8) {
                // Test known timestamp type constants (0-5)
                for (int known_val = 0; known_val <= 5; known_val++) {
                    const char* known_name = pcap_tstamp_type_val_to_name(known_val);
                    const char* known_desc = pcap_tstamp_type_val_to_description(known_val);
                    (void)known_name;
                    (void)known_desc;
                    
                    if (known_name != nullptr) {
                        int back_val = pcap_tstamp_type_name_to_val(known_name);
                        (void)back_val;
                    }
                }
                
                // Test with fuzzer-generated type names
                size_t fuzzed_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
                std::string fuzzed_name = fdp.ConsumeBytesAsString(fuzzed_name_size);
                
                int fuzzed_val = pcap_tstamp_type_name_to_val(fuzzed_name.c_str());
                (void)fuzzed_val;
            }
        }
        
        // Additional test: Try to get timestamp types again after some operations
        if (test_scenario == 1 || test_scenario == 3) {
            int* tstamp_types2 = nullptr;
            int tstamp_count2 = pcap_list_tstamp_types(pcap, &tstamp_types2);
            
            if (tstamp_count2 > 0 && tstamp_types2 != nullptr) {
                pcap_free_tstamp_types(tstamp_types2);
                tstamp_types2 = nullptr;
            }
        }
    } else {
        // Activation failed, but we can still test standalone conversion functions
        if (fdp.remaining_bytes() > 8) {
            // Test known timestamp type constants (0-5)
            for (int known_val = 0; known_val <= 5; known_val++) {
                const char* known_name = pcap_tstamp_type_val_to_name(known_val);
                const char* known_desc = pcap_tstamp_type_val_to_description(known_val);
                (void)known_name;
                (void)known_desc;
                
                if (known_name != nullptr) {
                    int back_val = pcap_tstamp_type_name_to_val(known_name);
                    (void)back_val;
                }
            }
            
            // Test with fuzzer-generated names
            size_t name_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            std::string fuzzed_name = fdp.ConsumeBytesAsString(name_size);
            
            int name_val = pcap_tstamp_type_name_to_val(fuzzed_name.c_str());
            (void)name_val;
        }
    }
    
    // Step 7: pcap_close for cleanup
    pcap_close(pcap);
    
    return 0;
}

} // namespace harness_008
