#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <pcap/dlt.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing of handle configuration
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 2);  // Which test scenario to run
    int datalink_val = fdp.ConsumeIntegralInRange<int>(DLT_NULL, DLT_USER14);  // Common DLT range
    int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=micro, 1=nano
    int tstamp_type = fdp.ConsumeIntegralInRange<int>(0, 5);  // PCAP_TSTAMP_* values
    int direction = fdp.ConsumeIntegralInRange<int>(0, 2);  // PCAP_D_INOUT/IN/OUT
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);  // Buffer size
    int immediate_mode = fdp.ConsumeBool() ? 1 : 0;
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);  // Timeout in ms
    int promisc_mode = fdp.ConsumeBool() ? 1 : 0;
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);  // Snapshot length
    int loop_count = fdp.ConsumeIntegralInRange<int>(0, 10);  // Number of iterations for pcap_loop
    
    // Consume device name string (use dummy interfaces like "any", "lo", or fuzzed string)
    size_t dev_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string dev_name = fdp.ConsumeBytesAsString(dev_name_size);
    
    // If device name is too short, use a dummy interface
    if (dev_name.size() < 2) {
        dev_name = "any";  // Common dummy interface on Linux
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = nullptr;
    
    // Step 1: pcap_create for dummy interface
    pcap = pcap_create(dev_name.c_str(), errbuf);
    if (pcap == nullptr) {
        // If pcap_create fails, we can still test pcap_init
        // Step 2: pcap_init for library initialization
        unsigned int opts = fdp.ConsumeIntegral<unsigned int>();
        pcap_init(opts, errbuf);
        return 0;
    }
    
    // Step 2: pcap_init for library initialization
    unsigned int opts = fdp.ConsumeIntegral<unsigned int>();
    pcap_init(opts, errbuf);
    
    // Step 3: Configuration functions on inactive handle
    // Note: These should be called before pcap_activate
    int config_result;
    
    // pcap_set_datalink - set data link type
    config_result = pcap_set_datalink(pcap, datalink_val);
    
    // pcap_set_tstamp_precision - set timestamp precision
    config_result = pcap_set_tstamp_precision(pcap, tstamp_precision);
    
    // pcap_set_tstamp_type - set timestamp type
    config_result = pcap_set_tstamp_type(pcap, tstamp_type);
    
    // pcap_setdirection - set capture direction (cast to pcap_direction_t enum)
    pcap_direction_t dir_enum = static_cast<pcap_direction_t>(direction % 3);  // 0,1,2 correspond to PCAP_D_INOUT, PCAP_D_IN, PCAP_D_OUT
    config_result = pcap_setdirection(pcap, dir_enum);
    // pcap_set_buffer_size - set buffer size
    config_result = pcap_set_buffer_size(pcap, buffer_size);
    
    // pcap_set_immediate_mode - set immediate mode
    config_result = pcap_set_immediate_mode(pcap, immediate_mode);
    
    // pcap_set_timeout - set timeout
    config_result = pcap_set_timeout(pcap, timeout_ms);
    
    // pcap_set_promisc - set promiscuous mode
    config_result = pcap_set_promisc(pcap, promisc_mode);
    
    // pcap_set_snaplen - set snapshot length
    config_result = pcap_set_snaplen(pcap, snaplen);
    
    // Step 4: pcap_lookupnet - lookup network address for interface
    bpf_u_int32 net = 0, mask = 0;
    int lookup_result = pcap_lookupnet(dev_name.c_str(), &net, &mask, errbuf);
    
    // Step 5: pcap_list_datalinks - get available data links
    int* dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
    if (dlt_list != nullptr && dlt_count > 0) {
        // Test pcap_set_datalink with some available data links
        int max_dlt_to_test = (dlt_count < 5) ? dlt_count : 5;
        for (int i = 0; i < max_dlt_to_test; i++) {
            pcap_set_datalink(pcap, dlt_list[i]);
        }
        pcap_free_datalinks(dlt_list);
    }
    
    // Step 6: pcap_activate - activate the configured handle
    int activate_result = pcap_activate(pcap);
    
    if (activate_result >= 0) {
        // Activation succeeded (or returned warning)
        
        // Step 7: pcap_loop - test packet processing if handle is active
        if (loop_count > 0 && activate_result == 0) {
            // Create a simple packet callback that does nothing
            auto packet_handler = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                // Empty handler - just count packets
                int* count = reinterpret_cast<int*>(user);
                (*count)++;
            };
            
            int packet_count = 0;
            pcap_loop(pcap, loop_count, packet_handler, reinterpret_cast<u_char*>(&packet_count));
        }
        
        // Test pcap_datalink to get actual data link type
        int actual_datalink = pcap_datalink(pcap);
        
        // Test pcap_list_datalinks again after activation
        dlt_list = nullptr;
        dlt_count = pcap_list_datalinks(pcap, &dlt_list);
        if (dlt_list != nullptr) {
            pcap_free_datalinks(dlt_list);
        }
        
        // Test pcap_getnonblock
        int nonblock = 0;
        pcap_getnonblock(pcap, errbuf);  // Check error return path
        
        // Test pcap_setnonblock
        pcap_setnonblock(pcap, fdp.ConsumeBool() ? 1 : 0, errbuf);
        
    } else {
        // Activation failed - this is still valid testing of error paths
        // Try to get error string
        const char* err_str = pcap_geterr(pcap);
        (void)err_str;
    }
    
    // Step 8: pcap_close - clean up the handle
    pcap_close(pcap);
    
    // Additional test: Try pcap_init again with different options
    if (fdp.remaining_bytes() > 4) {
        unsigned int opts2 = fdp.ConsumeIntegral<unsigned int>();
        pcap_init(opts2, errbuf);
    }
    
    // Test scenario variation based on test_type
    switch (test_type) {
        case 0:
            // Normal configuration sequence (already tested above)
            break;
            
        case 1: {
            // Test with invalid/edge-case parameters
            pcap_t* pcap2 = pcap_create("invalid_device_12345", errbuf);
            if (pcap2 != nullptr) {
                // Try configuration functions on handle for invalid device
                pcap_set_datalink(pcap2, -1);  // Invalid DLT
                pcap_set_tstamp_precision(pcap2, 99);  // Invalid precision
                pcap_set_tstamp_type(pcap2, 99);  // Invalid type
                pcap_setdirection(pcap2, static_cast<pcap_direction_t>(99));  // Invalid direction
                pcap_set_buffer_size(pcap2, 0);  // Zero buffer size
                pcap_set_immediate_mode(pcap2, 2);  // Invalid mode
                pcap_set_timeout(pcap2, -1);  // Negative timeout
                pcap_set_promisc(pcap2, 2);  // Invalid promisc value
                pcap_set_snaplen(pcap2, 0);  // Zero snaplen
                
                // Try to activate invalid handle
                pcap_activate(pcap2);
                pcap_close(pcap2);
            }
            break;
        }
            
        case 2: {
            // Test multiple create/configure/activate cycles
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 16; i++) {
                pcap_t* pcap3 = pcap_create(dev_name.c_str(), errbuf);
                if (pcap3 != nullptr) {
                    // Configure with some fuzzed values
                    int dlt = fdp.ConsumeIntegralInRange<int>(DLT_NULL, DLT_USER14);
                    pcap_set_datalink(pcap3, dlt);
                    
                    pcap_set_tstamp_precision(pcap3, fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO);
                    pcap_set_tstamp_type(pcap3, fdp.ConsumeIntegralInRange<int>(0, 5));
                    pcap_direction_t dir_enum3 = static_cast<pcap_direction_t>(fdp.ConsumeIntegralInRange<int>(0, 2) % 3);
                    pcap_setdirection(pcap3, dir_enum3);
                    pcap_set_buffer_size(pcap3, fdp.ConsumeIntegralInRange<int>(1024, 65536));
                    pcap_set_immediate_mode(pcap3, fdp.ConsumeBool() ? 1 : 0);
                    pcap_set_timeout(pcap3, fdp.ConsumeIntegralInRange<int>(0, 1000));
                    pcap_set_promisc(pcap3, fdp.ConsumeBool() ? 1 : 0);
                    pcap_set_snaplen(pcap3, fdp.ConsumeIntegralInRange<int>(68, 1518));
                    
                    // Try activation
                    pcap_activate(pcap3);
                    pcap_close(pcap3);
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_005
