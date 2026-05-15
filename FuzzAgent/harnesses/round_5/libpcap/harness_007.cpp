#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_007 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful timestamp API testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different timestamp API workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume pcap_init options
    unsigned int pcap_init_opts = 0;
    uint8_t init_flags = fdp.ConsumeIntegral<uint8_t>();
    if (init_flags & 0x01) pcap_init_opts |= PCAP_CHAR_ENC_UTF_8;
    if (init_flags & 0x02) pcap_init_opts |= PCAP_MMAP_32BIT;
    
    // Consume device name for pcap_create (or use "any" as default)
    std::string device_name = fdp.ConsumeRandomLengthString(128);
    if (device_name.empty()) {
        device_name = "any";
    }
    
    // Consume snaplen parameter
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
    
    // Consume timestamp type value for conversion functions
    int tstamp_type_val = fdp.ConsumeIntegralInRange<int>(-10, 20); // Range covering known and unknown types
    
    // Consume timestamp type name for pcap_tstamp_type_name_to_val
    std::string tstamp_type_name = fdp.ConsumeRandomLengthString(64);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    switch (operation) {
        case 0: {
            // Test pcap_init - global initialization
            char init_errbuf[PCAP_ERRBUF_SIZE];
            int init_result = pcap_init(pcap_init_opts, init_errbuf);
            (void)init_result; // Use result to avoid unused variable warning
            
            // Create a pcap handle to test timestamp functions
            pcap_t* handle = pcap_create(device_name.c_str(), errbuf);
            if (handle != nullptr) {
                pcap_close(handle);
            }
            break;
        }
        
        case 1: {
            // Test pcap_create and pcap_list_tstamp_types
            pcap_t* handle = pcap_create(device_name.c_str(), errbuf);
            if (handle != nullptr) {
                int* tstamp_types = nullptr;
                int tstamp_type_count = pcap_list_tstamp_types(handle, &tstamp_types);
                
                if (tstamp_type_count > 0 && tstamp_types != nullptr) {
                    // Iterate through available timestamp types
                    for (int i = 0; i < tstamp_type_count; i++) {
                        int type_val = tstamp_types[i];
                        
                        // Convert type value to name
                        const char* type_name = pcap_tstamp_type_val_to_name(type_val);
                        (void)type_name; // Use to avoid unused warning
                        
                        // Convert type value to description
                        const char* type_desc = pcap_tstamp_type_val_to_description(type_val);
                        (void)type_desc; // Use to avoid unused warning
                    }
                    
                    // Free the timestamp types array
                    pcap_free_tstamp_types(tstamp_types);
                }
                
                pcap_close(handle);
            }
            break;
        }
        
        case 2: {
            // Test timestamp type value to name/description conversions
            const char* name = pcap_tstamp_type_val_to_name(tstamp_type_val);
            (void)name;
            
            const char* desc = pcap_tstamp_type_val_to_description(tstamp_type_val);
            (void)desc;
            
            // Also test with some known timestamp type values
            const int known_types[] = {
                PCAP_TSTAMP_HOST,
                PCAP_TSTAMP_HOST_LOWPREC,
                PCAP_TSTAMP_HOST_HIPREC,
                PCAP_TSTAMP_ADAPTER,
                PCAP_TSTAMP_ADAPTER_UNSYNCED,
                PCAP_TSTAMP_HOST_HIPREC_UNSYNCED
            };
            
            for (int known_type : known_types) {
                const char* known_name = pcap_tstamp_type_val_to_name(known_type);
                (void)known_name;
                
                const char* known_desc = pcap_tstamp_type_val_to_description(known_type);
                (void)known_desc;
            }
            break;
        }
        
        case 3: {
            // Test timestamp type name to value conversion
            int val_from_name = pcap_tstamp_type_name_to_val(tstamp_type_name.c_str());
            (void)val_from_name;
            
            // Test with some known timestamp type names
            const char* known_names[] = {
                "host",
                "host_lowprec",
                "host_hiprec",
                "adapter",
                "adapter_unsynced",
                "host_hiprec_unsynced"
            };
            
            for (const char* known_name : known_names) {
                int known_val = pcap_tstamp_type_name_to_val(known_name);
                (void)known_val;
            }
            break;
        }
        
        case 4: {
            // Comprehensive test: create handle, list types, convert between name/value
            pcap_t* handle = pcap_create(device_name.c_str(), errbuf);
            if (handle != nullptr) {
                int* tstamp_types = nullptr;
                int tstamp_type_count = pcap_list_tstamp_types(handle, &tstamp_types);
                
                if (tstamp_type_count > 0 && tstamp_types != nullptr) {
                    // For each timestamp type, test conversion both ways
                    for (int i = 0; i < tstamp_type_count; i++) {
                        int type_val = tstamp_types[i];
                        
                        // Value to name
                        const char* type_name = pcap_tstamp_type_val_to_name(type_val);
                        if (type_name != nullptr) {
                            // Name back to value (round-trip test)
                            int roundtrip_val = pcap_tstamp_type_name_to_val(type_name);
                            (void)roundtrip_val;
                            
                            // Value to description
                            const char* type_desc = pcap_tstamp_type_val_to_description(type_val);
                            (void)type_desc;
                        }
                    }
                    
                    pcap_free_tstamp_types(tstamp_types);
                }
                
                pcap_close(handle);
            }
            break;
        }
        
        case 5: {
            // Test pcap_create with configuration and timestamp precision
            pcap_t* handle = pcap_create(device_name.c_str(), errbuf);
            if (handle != nullptr) {
                // Set some basic parameters
                pcap_set_snaplen(handle, snaplen);
                
                // Set timestamp precision (micro vs nano)
                int precision_choice = fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO;
                pcap_set_tstamp_precision(handle, precision_choice);
                
                // Get timestamp precision back
                int current_precision = pcap_get_tstamp_precision(handle);
                (void)current_precision;
                
                // List available timestamp types
                int* tstamp_types = nullptr;
                int tstamp_type_count = pcap_list_tstamp_types(handle, &tstamp_types);
                
                if (tstamp_type_count > 0 && tstamp_types != nullptr) {
                    // Try to set a timestamp type if available
                    if (tstamp_type_count > 0) {
                        int selected_type = tstamp_types[fdp.ConsumeIntegralInRange<size_t>(0, tstamp_type_count - 1)];
                        pcap_set_tstamp_type(handle, selected_type);
                    }
                    
                    pcap_free_tstamp_types(tstamp_types);
                }
                
                // Try to activate the handle (may fail if device doesn't exist, that's OK)
                int activate_result = pcap_activate(handle);
                (void)activate_result;
                
                pcap_close(handle);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_007
