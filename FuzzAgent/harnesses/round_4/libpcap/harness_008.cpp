#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>

namespace harness_008 {


int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Minimum size needed for meaningful testing:
    // - Device name (at least 1 byte)
    // - Error handling selection (1 byte)
    if (Size < 2) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Consume device name from fuzzer input
    // Use a reasonable max length to avoid excessive memory usage
    std::string device = fdp.ConsumeRandomLengthString(64);
    if (device.empty()) {
        // Use loopback as fallback
        device = "lo";
    }
    
    // Consume a byte for error handling scenario selection
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    // Step 1: pcap_create for dummy interface
    pcap_t *pcap_handle = pcap_create(device.c_str(), errbuf);
    if (pcap_handle == nullptr) {
        // Failed to create handle, nothing more to do
        return 0;
    }
    
    // Step 2: pcap_activate
    int activate_result = pcap_activate(pcap_handle);
    
    // Even if activation fails, some queries might still work
    // Continue testing based on scenario
    
    int *datalinks = nullptr;
    int *tstamp_types = nullptr;
    int datalink_count = 0;
    int tstamp_type_count = 0;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Normal path - try to get both lists
            // Step 3: pcap_list_datalinks
            datalink_count = pcap_list_datalinks(pcap_handle, &datalinks);
            
            // Step 4: pcap_list_tstamp_types
            tstamp_type_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
            
            break;
        }
        
        case 1: {
            // Scenario 1: Only get data links
            datalink_count = pcap_list_datalinks(pcap_handle, &datalinks);
            break;
        }
        
        case 2: {
            // Scenario 2: Only get timestamp types
            tstamp_type_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
            break;
        }
        
        case 3: {
            // Scenario 3: Try both but handle potential errors
            datalink_count = pcap_list_datalinks(pcap_handle, &datalinks);
            tstamp_type_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
            break;
        }
    }
    
    // Step 5: For each data link name call pcap_datalink_name_to_val
    if (datalink_count > 0 && datalinks != nullptr) {
        // Limit iteration to avoid excessive loops
        int max_iterations = datalink_count < 10 ? datalink_count : 10;
        for (int i = 0; i < max_iterations; i++) {
            // Get the data link value
            int link_val = datalinks[i];
            
            // Get the name for this value using pcap_datalink_val_to_name
            const char *link_name = pcap_datalink_val_to_name(link_val);
            
            // Step 5: Test pcap_datalink_name_to_val with the name
            if (link_name != nullptr) {
                int val_from_name = pcap_datalink_name_to_val(link_name);
                // Result can be used or ignored
            }
            
            // Also test with fuzzer-provided names
            if (fdp.remaining_bytes() > 0) {
                std::string fuzz_link_name = fdp.ConsumeRandomLengthString(32);
                if (!fuzz_link_name.empty()) {
                    int val = pcap_datalink_name_to_val(fuzz_link_name.c_str());
                }
            }
        }
    }
    // Step 6: For each timestamp type name call pcap_tstamp_type_name_to_val
    // Step 7: For each timestamp type value call pcap_tstamp_type_val_to_name
    // Step 8: For each timestamp type value call pcap_tstamp_type_val_to_description
    if (tstamp_type_count > 0 && tstamp_types != nullptr) {
        // Limit iteration
        int max_iterations = tstamp_type_count < 10 ? tstamp_type_count : 10;
        for (int i = 0; i < max_iterations; i++) {
            int tstamp_val = tstamp_types[i];
            
            // Step 7: Call pcap_tstamp_type_val_to_name
            const char *name = pcap_tstamp_type_val_to_name(tstamp_val);
            
            // Step 8: Call pcap_tstamp_type_val_to_description
            const char *desc = pcap_tstamp_type_val_to_description(tstamp_val);
            
            // Step 6: If we got a name back, test pcap_tstamp_type_name_to_val
            if (name != nullptr) {
                int val_from_name = pcap_tstamp_type_name_to_val(name);
                // Result can be used or ignored
            }
            
            // Also test with fuzzer-provided names
            if (fdp.remaining_bytes() > 0) {
                std::string tstamp_name = fdp.ConsumeRandomLengthString(32);
                if (!tstamp_name.empty()) {
                    int val = pcap_tstamp_type_name_to_val(tstamp_name.c_str());
                }
            }
        }
    }
    
    // Step 9: pcap_free_datalinks
    if (datalinks != nullptr) {
        pcap_free_datalinks(datalinks);
    }
    
    // Step 10: pcap_free_tstamp_types
    if (tstamp_types != nullptr) {
        pcap_free_tstamp_types(tstamp_types);
    }
    
    // Step 11: pcap_close
    pcap_close(pcap_handle);
    
    return 0;
}

} // namespace harness_008
