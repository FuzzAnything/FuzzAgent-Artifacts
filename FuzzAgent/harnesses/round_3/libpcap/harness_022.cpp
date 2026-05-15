#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/dlt.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

namespace harness_022 {
/*
 * Harness for comprehensive data link manipulation API testing
 * Primary Target: pcap_list_datalinks (12.5% coverage), pcap_free_datalinks (0% coverage)
 * Secondary Targets: pcap_datalink, pcap_datalink_ext, pcap_datalink_name_to_val, 
 *                    pcap_datalink_val_to_name, pcap_datalink_val_to_description,
 *                    pcap_datalink_val_to_description_or_dlt, pcap_set_datalink
 * Coverage Goal: Maximize coverage of data link manipulation functions, particularly
 *                pcap_free_datalinks which has 0% coverage, and pcap_list_datalinks
 *                which has only 12.5% coverage
 * Strategy: Create multiple pcap handles (live, dead, offline) and test all data link
 *           related functions on each handle type with fuzzed input
 */

// Forward declaration of helper function
static void test_standalone_datalink_functions(FuzzedDataProvider& fdp, 
                                               const std::string& datalink_name,
                                               int datalink_val1, 
                                               int datalink_val2);
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for comprehensive data link testing
    // Need enough data for control parameters, device names, and data link operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size control parameters first
    int handle_type = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which pcap handle type to create
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which test scenario to run
    int datalink_val = fdp.ConsumeIntegralInRange<int>(DLT_NULL, DLT_USER14);  // Common DLT range
    int datalink_val2 = fdp.ConsumeIntegralInRange<int>(DLT_NULL, DLT_USER14);  // Second DLT value
    int max_dlts_to_test = fdp.ConsumeIntegralInRange<int>(1, 10);  // Max DLTs to test from list
    int operation_flags = fdp.ConsumeIntegral<int>();  // Operation control flags
    
    // Consume device/interface name string
    size_t dev_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string dev_name = fdp.ConsumeBytesAsString(dev_name_size);
    
    // Consume data link name for conversion testing
    size_t datalink_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string datalink_name = fdp.ConsumeBytesAsString(datalink_name_size);
    
    // Clean up device name if invalid
    if (dev_name.empty() || dev_name.find('/') != std::string::npos || dev_name.size() < 2) {
        dev_name = "any";  // Common dummy interface on Linux
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    
    // Step 1: Create appropriate pcap handle based on handle_type
    switch (handle_type) {
        case 0: {
            // Live capture handle using pcap_create
            pcap_handle = pcap_create(dev_name.c_str(), errbuf);
            if (pcap_handle != nullptr) {
                // Configure basic parameters before activation
                pcap_set_snaplen(pcap_handle, 65535);
                pcap_set_promisc(pcap_handle, 0);
                pcap_set_timeout(pcap_handle, 1000);
                
                // Try to activate - this may fail but that's OK for testing
                int activate_result = pcap_activate(pcap_handle);
                if (activate_result < 0 && activate_result != PCAP_WARNING) {
                    // Activation failed, close and use dead handle instead
                    pcap_close(pcap_handle);
                    pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
                }
            } else {
                // pcap_create failed, use dead handle
                pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
            }
            break;
        }
            
        case 1: {
            // Dead handle - always works
            pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
            break;
        }
            
        case 2: {
            // Try pcap_open_live directly
            pcap_handle = pcap_open_live(dev_name.c_str(), 65535, 0, 1000, errbuf);
            if (pcap_handle == nullptr) {
                // Fall back to dead handle
                pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
            }
            break;
        }
            
        case 3: {
            // Create a temporary pcap file for offline testing
            std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
            if (pcap_data.size() >= 24) {  // Minimum pcap header size
                char temp_filename[] = "/tmp/libpcap_datalink_fuzz_XXXXXX";
                int fd = mkstemp(temp_filename);
                if (fd >= 0) {
                    if (write(fd, pcap_data.data(), pcap_data.size()) == static_cast<ssize_t>(pcap_data.size())) {
                        close(fd);
                        pcap_handle = pcap_open_offline(temp_filename, errbuf);
                        unlink(temp_filename);
                    } else {
                        close(fd);
                        unlink(temp_filename);
                    }
                }
            }
            
            if (pcap_handle == nullptr) {
                // Fall back to dead handle
                pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
            }
            break;
        }
    }
    
    if (pcap_handle == nullptr) {
        // All attempts failed, test standalone functions only
        test_standalone_datalink_functions(fdp, datalink_name, datalink_val, datalink_val2);
        return 0;
    }
    
    // Step 2: Test data link functions based on test_scenario
    switch (test_scenario) {
        case 0: {
            // Comprehensive pcap_list_datalinks and pcap_free_datalinks testing
            // PRIMARY TARGET: pcap_list_datalinks (12.5% coverage)
            // PRIMARY TARGET: pcap_free_datalinks (0% coverage)
            
            int* dlt_list = nullptr;
            int dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
            
            if (dlt_count > 0 && dlt_list != nullptr) {
                // Test 1: Iterate through available DLTs and get their names/descriptions
                int dlts_to_test = (dlt_count < max_dlts_to_test) ? dlt_count : max_dlts_to_test;
                for (int i = 0; i < dlts_to_test; i++) {
                    int current_dlt = dlt_list[i];
                    
                    // Get DLT name
                    const char* dlt_name = pcap_datalink_val_to_name(current_dlt);
                    (void)dlt_name;
                    
                    // Get DLT description
                    const char* dlt_desc = pcap_datalink_val_to_description(current_dlt);
                    (void)dlt_desc;
                    
                    // Get DLT description or DLT
                    const char* dlt_desc_or_dlt = pcap_datalink_val_to_description_or_dlt(current_dlt);
                    (void)dlt_desc_or_dlt;
                    
                    // Try to set this DLT (may fail if not supported)
                    pcap_set_datalink(pcap_handle, current_dlt);
                }
                
                // Test 2: Get current data link type after possible changes
                int current_datalink = pcap_datalink(pcap_handle);
                (void)current_datalink;
                
                // Test 3: Get extended data link information
                int datalink_ext = pcap_datalink_ext(pcap_handle);
                (void)datalink_ext;
                
                // Test 4: Try setting a fuzzed DLT value
                pcap_set_datalink(pcap_handle, datalink_val);
                
                // PRIMARY TARGET: pcap_free_datalinks must be called!
                pcap_free_datalinks(dlt_list);
                dlt_list = nullptr;
            } else if (dlt_count == PCAP_ERROR) {
                // pcap_list_datalinks failed, test error handling
                const char* error_str = pcap_geterr(pcap_handle);
                (void)error_str;
            }
            
            // Test pcap_list_datalinks again with NULL pointer to test error paths
            // (though this would likely crash, so we skip it)
            break;
        }
            
        case 1: {
            // Focused pcap_free_datalinks testing with multiple allocations
            // PRIMARY TARGET: pcap_free_datalinks (0% coverage) - multiple calls
            
            // First allocation and free
            int* dlt_list1 = nullptr;
            int dlt_count1 = pcap_list_datalinks(pcap_handle, &dlt_list1);
            if (dlt_list1 != nullptr) {
                pcap_free_datalinks(dlt_list1);
            }
            
            // Change data link type and try again
            pcap_set_datalink(pcap_handle, datalink_val);
            
            // Second allocation and free
            int* dlt_list2 = nullptr;
            int dlt_count2 = pcap_list_datalinks(pcap_handle, &dlt_list2);
            if (dlt_list2 != nullptr) {
                pcap_free_datalinks(dlt_list2);
            }
            
            // Test with a different handle type if possible
            pcap_t* dead_handle = pcap_open_dead(DLT_EN10MB, 65535);
            if (dead_handle != nullptr) {
                int* dead_dlt_list = nullptr;
                int dead_dlt_count = pcap_list_datalinks(dead_handle, &dead_dlt_list);
                if (dead_dlt_list != nullptr) {
                    pcap_free_datalinks(dead_dlt_list);
                }
                pcap_close(dead_handle);
            }
            break;
        }
            
        case 2: {
            // Data link conversion and name testing
            // Test pcap_datalink_name_to_val with fuzzed input
            int converted_val = pcap_datalink_name_to_val(datalink_name.c_str());
            (void)converted_val;
            
            // Test conversion for various known DLT values
            int test_dlts[] = {DLT_NULL, DLT_EN10MB, DLT_IEEE802_11, DLT_RAW, DLT_LINUX_SLL};
            for (size_t i = 0; i < sizeof(test_dlts)/sizeof(test_dlts[0]); i++) {
                const char* name = pcap_datalink_val_to_name(test_dlts[i]);
                const char* desc = pcap_datalink_val_to_description(test_dlts[i]);
                const char* desc_or_dlt = pcap_datalink_val_to_description_or_dlt(test_dlts[i]);
                (void)name;
                (void)desc;
                (void)desc_or_dlt;
                
                // Try to convert back from name
                if (name != nullptr) {
                    int back_val = pcap_datalink_name_to_val(name);
                    (void)back_val;
                }
            }
            
            // Also test pcap_list_datalinks and free
            int* dlt_list = nullptr;
            int dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
            if (dlt_list != nullptr) {
                pcap_free_datalinks(dlt_list);
            }
            break;
        }
            
        case 3: {
            // Error condition and edge case testing
            // Test pcap_set_datalink with various values
            pcap_set_datalink(pcap_handle, datalink_val);
            pcap_set_datalink(pcap_handle, datalink_val2);
            
            // Test with invalid DLT values (negative)
            pcap_set_datalink(pcap_handle, -1);
            pcap_set_datalink(pcap_handle, -100);
            
            // Test pcap_list_datalinks on potentially problematic handles
            int* dlt_list = nullptr;
            int dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
            
            // Force pcap_free_datalinks with NULL (should be safe per free() semantics)
            pcap_free_datalinks(nullptr);
            
            if (dlt_list != nullptr) {
                pcap_free_datalinks(dlt_list);
                // Double free test (shouldn't crash in robust implementation)
                // pcap_free_datalinks(dlt_list); // Dangerous, skipping
            }
            
            // Test after handle closure (should fail)
            // pcap_close(pcap_handle);
            // pcap_list_datalinks(pcap_handle, &dlt_list); // Use after free, skipping
            break;
        }
    }
    
    // Step 3: Additional data link API testing
    // Always test these basic functions regardless of scenario
    int current_datalink = pcap_datalink(pcap_handle);
    (void)current_datalink;
    
    int datalink_ext = pcap_datalink_ext(pcap_handle);
    (void)datalink_ext;
    
    // Test setting data link one more time
    pcap_set_datalink(pcap_handle, datalink_val);
    
    // Step 4: Cleanup
    pcap_close(pcap_handle);
    
    return 0;
}

// Helper function for standalone data link function testing (when no pcap handle available)
static void test_standalone_datalink_functions(FuzzedDataProvider& fdp, 
                                               const std::string& datalink_name,
                                               int datalink_val1, 
                                               int datalink_val2) {
    // Test pcap_datalink_name_to_val with fuzzed name
    int name_to_val_result = pcap_datalink_name_to_val(datalink_name.c_str());
    (void)name_to_val_result;
    
    // Test pcap_datalink_val_to_name with various values
    const char* val_to_name_result1 = pcap_datalink_val_to_name(datalink_val1);
    const char* val_to_name_result2 = pcap_datalink_val_to_name(datalink_val2);
    (void)val_to_name_result1;
    (void)val_to_name_result2;
    
    // Test pcap_datalink_val_to_description
    const char* val_to_desc_result1 = pcap_datalink_val_to_description(datalink_val1);
    const char* val_to_desc_result2 = pcap_datalink_val_to_description(datalink_val2);
    (void)val_to_desc_result1;
    (void)val_to_desc_result2;
    
    // Test pcap_datalink_val_to_description_or_dlt
    const char* val_to_desc_or_dlt1 = pcap_datalink_val_to_description_or_dlt(datalink_val1);
    const char* val_to_desc_or_dlt2 = pcap_datalink_val_to_description_or_dlt(datalink_val2);
    (void)val_to_desc_or_dlt1;
    (void)val_to_desc_or_dlt2;
    
    // Test with a dead handle for pcap_list_datalinks
    pcap_t* dead_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (dead_handle != nullptr) {
        int* dlt_list = nullptr;
        int dlt_count = pcap_list_datalinks(dead_handle, &dlt_list);
        if (dlt_list != nullptr) {
            // PRIMARY TARGET: pcap_free_datalinks (0% coverage)
            pcap_free_datalinks(dlt_list);
        }
        pcap_close(dead_handle);
    }
}

} // namespace harness_022
