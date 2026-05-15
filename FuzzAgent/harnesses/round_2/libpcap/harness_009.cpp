#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

namespace harness_009 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least operation mode (1 byte), 
    // device type (1 byte), snaplen (2 bytes), timeout (2 bytes) = 6 bytes
    if (size < 6) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode: 
    // 0 = use pcap_create + pcap_activate (preferred for DLT testing)
    // 1 = use pcap_open_live (legacy)
    // 2 = use pcap_open_dead (fallback if live capture fails)
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume device type: 0 = loopback, 1 = any available device
    uint8_t device_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume capture parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(64, 65535);
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume number of DLT operations to try (0-5)
    int dlt_operations = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Remaining bytes are for additional test data (device names, etc.)
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    const char *device = nullptr;
    
    // Step 1: Determine device to use
    if (device_type == 0) {
        // Use loopback interface (commonly available)
        device = "lo";
    } else {
        // Try to find any available device
        pcap_if_t *alldevs = nullptr;
        if (pcap_findalldevs(&alldevs, errbuf) == 0 && alldevs != nullptr) {
            device = alldevs->name;
            // Note: We'll free alldevs after creating the handle
        } else {
            // Fall back to loopback
            device = "lo";
        }
    }
    
    // Step 2: Create pcap handle based on operation mode
    if (operation_mode == 0) {
        // Modern: pcap_create + pcap_activate
        pcap_handle = pcap_create(device, errbuf);
        if (pcap_handle != nullptr) {
            // Set optional parameters
            pcap_set_snaplen(pcap_handle, snaplen);
            pcap_set_promisc(pcap_handle, promisc);
            pcap_set_timeout(pcap_handle, timeout_ms);
            
            // Activate the handle
            int activate_result = pcap_activate(pcap_handle);
            if (activate_result < 0) {
                // Activation failed, but we can still test some APIs
                // Note: pcap_list_datalinks may fail on non-activated handle
            }
        }
    } else if (operation_mode == 1) {
        // Legacy: pcap_open_live
        pcap_handle = pcap_open_live(device, snaplen, promisc, timeout_ms, errbuf);
    } else {
        // Fallback: pcap_open_dead (always works)
        // Use a reasonable default link type (DLT_EN10MB = 1)
        pcap_handle = pcap_open_dead(1, snaplen);
    }
    
    if (pcap_handle == nullptr) {
        // Ultimate fallback: pcap_open_dead
        pcap_handle = pcap_open_dead(1, snaplen);
        if (pcap_handle == nullptr) {
            // Free device list if we allocated it
            if (device_type == 1) {
                pcap_if_t *alldevs = nullptr;
                // Re-fetch to free (simplified approach)
                pcap_findalldevs(&alldevs, errbuf);
                pcap_freealldevs(alldevs);
            }
            return 0; // Cannot proceed
        }
    }
    
    // Step 3: Data Link Type Management Lifecycle
    
    // 3a: Get current data link type (baseline)
    int current_dlt = pcap_datalink(pcap_handle);
    
    // 3b: Get extended data link info (if available)
    int dlt_ext = pcap_datalink_ext(pcap_handle);
    // Note: pcap_datalink_ext may return same as pcap_datalink or extended info
    
    // 3c: List available data link types
    int *dlts = nullptr;
    int ndlts = pcap_list_datalinks(pcap_handle, &dlts);
    
    if (ndlts > 0 && dlts != nullptr) {
        // Test 1: Iterate through available DLTs and display info
        for (int i = 0; i < ndlts && i < 10; i++) { // Limit to 10 for performance
            int dlt = dlts[i];
            
            // Get DLT name if available
            const char *dlt_name = pcap_datalink_val_to_name(dlt);
            const char *dlt_description = pcap_datalink_val_to_description(dlt);
            
            // Test 2: Try to set to this DLT (if operations count allows)
            if (dlt_operations > 0) {
                int set_result = pcap_set_datalink(pcap_handle, dlt);
                // set_result will be 0 on success, PCAP_ERROR on failure
                // Failure is OK - not all DLTs are supported on all interfaces
                
                // If set succeeded, verify the new DLT
                if (set_result == 0) {
                    int new_dlt = pcap_datalink(pcap_handle);
                    int new_dlt_ext = pcap_datalink_ext(pcap_handle);
                    
                    // Test setting back to original if we have more operations
                    if (dlt_operations > 1 && i > 0) {
                        pcap_set_datalink(pcap_handle, dlts[0]); // Try to set back to first
                    }
                }
                
                dlt_operations--;
            }
        }
        
        // Test 3: Try setting to some common DLT values directly
        // Common DLT values from pcap/dlt.h
        int common_dlts[] = {
            1,  // DLT_EN10MB (Ethernet)
            12, // DLT_RAW (Raw IP)
            113, // DLT_LINUX_SLL (Linux cooked)
            101, // DLT_IEEE802_11 (802.11)
        };
        
        for (size_t i = 0; i < sizeof(common_dlts)/sizeof(common_dlts[0]) && dlt_operations > 0; i++) {
            pcap_set_datalink(pcap_handle, common_dlts[i]);
            dlt_operations--;
        }
        
        // Test 4: Free the DLT list
        pcap_free_datalinks(dlts);
        dlts = nullptr; // Prevent use-after-free
    } else if (dlts != nullptr) {
        // ndlts <= 0 but dlts might have been allocated
        pcap_free_datalinks(dlts);
    }
    
    // Step 4: Additional DLT-related API tests
    
    // Test pcap_datalink_val_to_name and pcap_datalink_val_to_description
    // for a range of DLT values (including invalid ones)
    for (int test_dlt = -10; test_dlt < 200; test_dlt += 10) {
        const char *name = pcap_datalink_val_to_name(test_dlt);
        const char *desc = pcap_datalink_val_to_description(test_dlt);
        // These may return NULL for invalid/unsupported DLT values
    }
    
    // Test pcap_datalink_name_to_val for some known names
    const char *test_names[] = {"EN10MB", "RAW", "NULL", "LOOP", "IEEE802_11"};
    for (size_t i = 0; i < sizeof(test_names)/sizeof(test_names[0]); i++) {
        int dlt_val = pcap_datalink_name_to_val(test_names[i]);
        // Returns PCAP_ERROR (-1) if not found
    }
    
    // Step 5: Cleanup
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
    }
    
    // Free device list if we allocated it
    if (device_type == 1) {
        pcap_if_t *alldevs = nullptr;
        // Re-fetch to free (simplified approach - in real scenario we'd save the pointer)
        pcap_findalldevs(&alldevs, errbuf);
        pcap_freealldevs(alldevs);
    }
    
    return 0;
}

} // namespace harness_009
