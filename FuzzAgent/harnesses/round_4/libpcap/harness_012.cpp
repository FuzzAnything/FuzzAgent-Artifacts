#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>

namespace harness_012 {


int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Minimum input size needed for comprehensive handle configuration and datalink testing
    // We need: scenario selection + linktype + various configuration parameters
    if (Size < 32) {
        return 0;  // Not enough input for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: link type for pcap_open_dead
    int linktype = fdp.ConsumeIntegral<uint8_t>();
    
    // Third byte: snapshot length
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    
    // Fourth byte: configuration options
    uint8_t config_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Fifth byte: datalink operations selector
    uint8_t datalink_op = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Sixth byte: non-blocking mode flag
    int nonblock_mode = fdp.ConsumeBool() ? 1 : 0;
    
    // Seventh byte: timestamp precision
    int tstamp_precision = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0=default, 1=micro, 2=nano
    
    // Consume strings for device name and error buffers
    std::string device_name = fdp.ConsumeRandomLengthString(64);
    std::string errbuf_input = fdp.ConsumeRemainingBytesAsString();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    if (!errbuf_input.empty()) {
        size_t copy_len = std::min(errbuf_input.size(), sizeof(errbuf) - 1);
        memcpy(errbuf, errbuf_input.data(), copy_len);
        errbuf[copy_len] = '\0';
    }
    
    pcap_t *pcap_handle = nullptr;
    
    // Create a pcap handle - use pcap_open_dead for reliable testing
    // pcap_open_dead creates a dummy handle that doesn't require actual devices
    pcap_handle = pcap_open_dead(linktype, snaplen);
    
    if (pcap_handle == nullptr) {
        // If pcap_open_dead fails, we can't proceed
        return 0;
    }
    
    // Now test various handle configuration and datalink operations based on scenario
    switch (scenario) {
        case 0: {
            // Scenario 0: Comprehensive datalink API testing
            // Get current datalink type using both regular and extended functions
            int current_dlt = pcap_datalink(pcap_handle);
            int current_dlt_ext = pcap_datalink_ext(pcap_handle);
            
            // List available datalinks
            int *datalinks = nullptr;
            int datalink_count = pcap_list_datalinks(pcap_handle, &datalinks);
            
            if (datalink_count > 0 && datalinks != nullptr) {
                // Test datalink translation functions for each available datalink
                // Limit iterations to prevent excessive loops
                int max_iter = datalink_count < 10 ? datalink_count : 10;
                for (int i = 0; i < max_iter; i++) {
                    int dlt_val = datalinks[i];
                    
                    // Test pcap_datalink_val_to_name
                    const char *dlt_name = pcap_datalink_val_to_name(dlt_val);
                    
                    // Test pcap_datalink_val_to_description
                    const char *dlt_desc = pcap_datalink_val_to_description(dlt_val);
                    
                    // Test pcap_datalink_val_to_description_or_dlt (uncovered API)
                    const char *dlt_desc_or_dlt = pcap_datalink_val_to_description_or_dlt(dlt_val);
                    
                    // Test pcap_datalink_name_to_val if we got a valid name
                    if (dlt_name != nullptr) {
                        int val_from_name = pcap_datalink_name_to_val(dlt_name);
                        // Result is not critical - just exercising the code path
                    }
                    
                    // Try to set the datalink (may fail if not supported)
                    int set_result = pcap_set_datalink(pcap_handle, dlt_val);
                    // Don't check result - both success and failure are interesting
                }
                
                // Free the datalinks array
                pcap_free_datalinks(datalinks);
            }
            
            break;
        }
        
        case 1: {
            // Scenario 1: Handle configuration getter functions
            // Test pcap_get_tstamp_precision
            int precision = pcap_get_tstamp_precision(pcap_handle);
            
            // Test pcap_getnonblock
            char nonblock_errbuf[PCAP_ERRBUF_SIZE];
            memset(nonblock_errbuf, 0, sizeof(nonblock_errbuf));
            int nonblock_result = pcap_getnonblock(pcap_handle, nonblock_errbuf);
            
            // Test pcap_geterr - get error string from handle
            const char *handle_error = pcap_geterr(pcap_handle);
            
            // Test pcap_can_set_rfmon (uncovered API)
            int can_set_rfmon = pcap_can_set_rfmon(pcap_handle);
            
            // Test other informational functions
            int major_version = pcap_major_version(pcap_handle);
            int minor_version = pcap_minor_version(pcap_handle);
            int snapshot_val = pcap_snapshot(pcap_handle);
            int is_swapped = pcap_is_swapped(pcap_handle);
            int bufsize = pcap_bufsize(pcap_handle);
            
            // Test pcap_datalink_ext (uncovered API)
            int dlt_ext = pcap_datalink_ext(pcap_handle);
            
            // Test datalink functions again for consistency check
            int dlt = pcap_datalink(pcap_handle);
            const char *dlt_name = pcap_datalink_val_to_name(dlt);
            const char *dlt_desc = pcap_datalink_val_to_description(dlt);
            const char *dlt_desc_or_dlt = pcap_datalink_val_to_description_or_dlt(dlt);
            
            break;
        }
        
        case 2: {
            // Scenario 2: Configuration setter/getter combinations
            // First set some configuration, then get it back
            
            // Set non-blocking mode
            char setnonblock_errbuf[PCAP_ERRBUF_SIZE];
            memset(setnonblock_errbuf, 0, sizeof(setnonblock_errbuf));
            int set_nonblock_result = pcap_setnonblock(pcap_handle, nonblock_mode, setnonblock_errbuf);
            
            // Get it back to verify
            char getnonblock_errbuf[PCAP_ERRBUF_SIZE];
            memset(getnonblock_errbuf, 0, sizeof(getnonblock_errbuf));
            int get_nonblock_result = pcap_getnonblock(pcap_handle, getnonblock_errbuf);
            
            // Test setting datalink to various values
            // Generate some datalink values from fuzzer input
            std::vector<int> test_dlt_values;
            for (int i = 0; i < 8 && fdp.remaining_bytes() > 0; i++) {
                int test_dlt = fdp.ConsumeIntegral<uint8_t>();
                test_dlt_values.push_back(test_dlt);
            }
            
            // Try to set each datalink value
            for (int dlt_val : test_dlt_values) {
                int set_dlt_result = pcap_set_datalink(pcap_handle, dlt_val);
                
                // After setting, get the current datalink
                int current_dlt = pcap_datalink(pcap_handle);
                int current_dlt_ext = pcap_datalink_ext(pcap_handle);
                
                // Get name and description for current datalink
                const char *current_name = pcap_datalink_val_to_name(current_dlt);
                const char *current_desc = pcap_datalink_val_to_description(current_dlt);
                const char *current_desc_or_dlt = pcap_datalink_val_to_description_or_dlt(current_dlt);
            }
            
            // Test pcap_can_set_rfmon before and after potential configuration
            int can_rfmon_before = pcap_can_set_rfmon(pcap_handle);
            
            // Try to set RF monitor mode (will likely fail with dummy handle)
            int set_rfmon_result = pcap_set_rfmon(pcap_handle, config_flags & 0x01);
            
            int can_rfmon_after = pcap_can_set_rfmon(pcap_handle);
            
            break;
        }
        
        case 3: {
            // Scenario 3: Edge cases and error path testing
            // Test with extreme/edge case values
            
            // Test datalink translation with extreme values
            for (int extreme_val : {-1, 0, 127, 255, 1000, 10000, 65535}) {
                const char *extreme_name = pcap_datalink_val_to_name(extreme_val);
                const char *extreme_desc = pcap_datalink_val_to_description(extreme_val);
                const char *extreme_desc_or_dlt = pcap_datalink_val_to_description_or_dlt(extreme_val);
                
                // Try name_to_val with potentially invalid names
                if (extreme_name != nullptr) {
                    int val_from_extreme = pcap_datalink_name_to_val(extreme_name);
                }
            }
            
            // Test pcap_geterr with various states
            const char *error_string = pcap_geterr(pcap_handle);
            
            // Test pcap_can_set_rfmon multiple times
            for (int i = 0; i < 5; i++) {
                int rfmon_capable = pcap_can_set_rfmon(pcap_handle);
            }
            
            // Test pcap_datalink and pcap_datalink_ext multiple times
            for (int i = 0; i < 5; i++) {
                int dlt1 = pcap_datalink(pcap_handle);
                int dlt2 = pcap_datalink_ext(pcap_handle);
            }
            
            // Test pcap_get_tstamp_precision with handle in different states
            int precision1 = pcap_get_tstamp_precision(pcap_handle);
            
            // Test pcap_getnonblock with different error buffers
            char errbuf1[PCAP_ERRBUF_SIZE];
            char errbuf2[PCAP_ERRBUF_SIZE / 2];  // Smaller buffer
            char errbuf3[PCAP_ERRBUF_SIZE * 2];  // Larger buffer (stack allocated)
            
            memset(errbuf1, 0, sizeof(errbuf1));
            memset(errbuf2, 0, sizeof(errbuf2));
            memset(errbuf3, 0, sizeof(errbuf3));
            
            int nb1 = pcap_getnonblock(pcap_handle, errbuf1);
            int nb2 = pcap_getnonblock(pcap_handle, errbuf2);
            int nb3 = pcap_getnonblock(pcap_handle, errbuf3);
            
            // Test informational functions
            int major = pcap_major_version(pcap_handle);
            int minor = pcap_minor_version(pcap_handle);
            int snapshot = pcap_snapshot(pcap_handle);
            int swapped = pcap_is_swapped(pcap_handle);
            int bufsize = pcap_bufsize(pcap_handle);
            
            break;
        }
    }
    
    // Clean up
    pcap_close(pcap_handle);
    
    return 0;
}

} // namespace harness_012
