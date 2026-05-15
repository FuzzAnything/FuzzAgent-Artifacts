#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_010 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing of Linux-specific advanced configuration
    // We need enough data for device name, control parameters, and data link names
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size control parameters first
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 2);  // Which test scenario to run
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);   // Snapshot length
    int promisc_mode = fdp.ConsumeBool() ? 1 : 0;               // Promiscuous mode
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000); // Timeout in ms
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576); // Buffer size
    int rfmon_enable = fdp.ConsumeBool() ? 1 : 0;               // Monitor mode enable
    int protocol_value = fdp.ConsumeIntegral<int>();           // Linux protocol value
    int datalink_val = fdp.ConsumeIntegralInRange<int>(0, 255); // DLT value for testing
    
    // Consume device name string - use dummy interfaces for testing
    size_t dev_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string dev_name = fdp.ConsumeBytesAsString(dev_name_size);
    
    // Use "any" as a dummy interface if the generated name is invalid or too short
    if (dev_name.empty() || dev_name.find('/') != std::string::npos || dev_name.size() < 2) {
        dev_name = "any";  // Common dummy interface on Linux
    }
    
    // Consume data link name for conversion testing
    size_t datalink_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string datalink_name = fdp.ConsumeBytesAsString(datalink_name_size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = nullptr;
    
    // Step 1: pcap_create() to initialize a handle
    pcap = pcap_create(dev_name.c_str(), errbuf);
    if (pcap == nullptr) {
        // If pcap_create fails, we can still test some standalone functions
        // Test pcap_datalink_name_to_val with fuzzed data link name
        int dlt_val = pcap_datalink_name_to_val(datalink_name.c_str());
        (void)dlt_val;
        
        // Test pcap_can_set_rfmon with a dead handle
        pcap_t* dead_pcap = pcap_open_dead(DLT_EN10MB, snaplen);
        if (dead_pcap != nullptr) {
            int can_rfmon = pcap_can_set_rfmon(dead_pcap);
            (void)can_rfmon;
            pcap_close(dead_pcap);
        }
        
        return 0;
    }
    
    // Step 2: Basic configuration before monitor mode
    pcap_set_snaplen(pcap, snaplen);
    pcap_set_promisc(pcap, promisc_mode);
    pcap_set_timeout(pcap, timeout_ms);
    pcap_set_buffer_size(pcap, buffer_size);
    
    // Step 3: Check if monitor mode can be set, then try to set it
    int can_rfmon = pcap_can_set_rfmon(pcap);
    (void)can_rfmon;  // Use result to avoid unused variable warning
    
    if (rfmon_enable && can_rfmon == 1) {
        // Only try to set monitor mode if the interface supports it
        pcap_set_rfmon(pcap, 1);
    }
    
    // Step 4: Linux-specific protocol setting (only on Linux)
    #ifdef __linux__
    pcap_set_protocol_linux(pcap, protocol_value);
    #endif
    
    // Step 5: Data link type operations
    // First, test pcap_datalink_name_to_val with fuzzed name
    int name_to_val_result = pcap_datalink_name_to_val(datalink_name.c_str());
    (void)name_to_val_result;
    
    // Test pcap_datalink_ext (extended data link information)
    int datalink_ext_result = pcap_datalink_ext(pcap);
    (void)datalink_ext_result;
    
    // Try to set data link type
    pcap_set_datalink(pcap, datalink_val);
    
    // Get current data link type
    int current_datalink = pcap_datalink(pcap);
    (void)current_datalink;
    
    // Step 6: Buffer size management
    int current_bufsize = pcap_bufsize(pcap);
    (void)current_bufsize;
    
    // Step 7: File descriptor operations
    int selectable_fd = pcap_get_selectable_fd(pcap);
    (void)selectable_fd;
    
    const struct timeval* required_timeout = pcap_get_required_select_timeout(pcap);
    (void)required_timeout;
    
    // Additional Linux-specific tests based on test scenario
    switch (test_scenario) {
        case 0: {
            // Scenario 0: Test pcap_list_datalinks and pcap_datalink_val_to_name
            int* dlt_list = nullptr;
            int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
            
            if (dlt_list != nullptr && dlt_count > 0) {
                // Test conversion for available data links
                int max_to_test = (dlt_count < 5) ? dlt_count : 5;
                for (int i = 0; i < max_to_test; i++) {
                    const char* dlt_name = pcap_datalink_val_to_name(dlt_list[i]);
                    const char* dlt_desc = pcap_datalink_val_to_description(dlt_list[i]);
                    (void)dlt_name;
                    (void)dlt_desc;
                }
                
                pcap_free_datalinks(dlt_list);
            }
            break;
        }
            
        case 1: {
            // Scenario 1: Test pcap_setnonblock and pcap_getnonblock
            int nonblock_result = pcap_setnonblock(pcap, 1, errbuf);
            (void)nonblock_result;
            
            // pcap_getnonblock returns the nonblock status as return value
            int nonblock_status = pcap_getnonblock(pcap, errbuf);
            (void)nonblock_status;
            
            // Switch back to blocking mode
            pcap_setnonblock(pcap, 0, errbuf);
            break;
        }
            
        case 2: {
            // Scenario 2: Test pcap_stats and pcap_geterr
            struct pcap_stat stats;
            int stats_result = pcap_stats(pcap, &stats);
            (void)stats_result;
            
            const char* pcap_error = pcap_geterr(pcap);
            (void)pcap_error;
            
            // Test pcap_statustostr with various status codes
            const char* status_str = pcap_statustostr(0);  // PCAP_OK
            (void)status_str;
            
            status_str = pcap_statustostr(-1);  // PCAP_ERROR
            (void)status_str;
            
            status_str = pcap_statustostr(-2);  // PCAP_ERROR_BREAK
            (void)status_str;
            
            status_str = pcap_statustostr(1);   // PCAP_WARNING
            (void)status_str;
            
            break;
        }
    }
    
    // Step 8: Cleanup
    pcap_close(pcap);
    
    return 0;
}

} // namespace harness_010
