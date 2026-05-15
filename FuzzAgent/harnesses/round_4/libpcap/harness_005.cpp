#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <climits>
#include <pcap/pcap.h>

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Minimum input size needed for meaningful testing
    // We need: device name (variable) + 11 configuration parameters
    if (Size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test components
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume device name - can be valid, empty, or NULL
    std::string device_name;
    bool use_null_device = false;
    bool use_empty_device = false;
    
    uint8_t device_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (device_choice) {
        case 0:
            // Use NULL device name
            use_null_device = true;
            break;
        case 1:
            // Use empty string
            use_empty_device = true;
            device_name = "";
            break;
        case 2:
            // Use short device name from input
            device_name = fdp.ConsumeRandomLengthString(32);
            break;
        case 3:
            // Use longer device name
            device_name = fdp.ConsumeRandomLengthString(128);
            break;
    }
    
    // Consume configuration parameters
    int datalink_type = fdp.ConsumeIntegral<int>();
    int tstamp_type = fdp.ConsumeIntegral<int>();
    int precision = fdp.ConsumeIntegral<int>();
    pcap_direction_t direction = static_cast<pcap_direction_t>(fdp.ConsumeIntegral<uint8_t>() % 3);
    int buffer_size = fdp.ConsumeIntegral<int>();
    int nonblock = fdp.ConsumeIntegral<uint8_t>() % 2;
    int immediate_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    int rfmon = fdp.ConsumeIntegral<uint8_t>() % 2;
    int protocol = fdp.ConsumeIntegral<int>();
    
    // Consume remaining input for error buffer testing
    std::string remaining_input = fdp.ConsumeRemainingBytesAsString();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    pcap_t *p = nullptr;
    
    // Test different scenarios based on input
    switch (scenario) {
        case 0:
            // Scenario 0: Normal sequence - create, activate, set all configs, close
            if (use_null_device) {
                p = pcap_create(nullptr, errbuf);
            } else {
                p = pcap_create(device_name.c_str(), errbuf);
            }
            
            if (p != nullptr) {
                // Activate the pcap handle
                pcap_activate(p);
                
                // Set all configuration options
                pcap_set_datalink(p, datalink_type);
                pcap_set_tstamp_type(p, tstamp_type);
                pcap_set_tstamp_precision(p, precision);
                pcap_setdirection(p, direction);
                pcap_set_buffer_size(p, buffer_size);
                pcap_setnonblock(p, nonblock, errbuf);
                pcap_set_immediate_mode(p, immediate_mode);
                pcap_set_rfmon(p, rfmon);
                pcap_set_protocol_linux(p, protocol);
                
                pcap_close(p);
            }
            break;
            
        case 1:
            // Scenario 1: Set configs before activation (test error paths)
            if (use_null_device) {
                p = pcap_create(nullptr, errbuf);
            } else {
                p = pcap_create(device_name.c_str(), errbuf);
            }
            
            if (p != nullptr) {
                // Try to set configs before activation (some may fail)
                pcap_set_datalink(p, datalink_type);
                pcap_set_tstamp_type(p, tstamp_type);
                pcap_set_tstamp_precision(p, precision);
                pcap_setdirection(p, direction);
                pcap_set_buffer_size(p, buffer_size);
                pcap_setnonblock(p, nonblock, errbuf);
                pcap_set_immediate_mode(p, immediate_mode);
                pcap_set_rfmon(p, rfmon);
                pcap_set_protocol_linux(p, protocol);
                
                // Then activate
                pcap_activate(p);
                
                pcap_close(p);
            }
            break;
            
        case 2:
            // Scenario 2: Create, activate, close without setting configs
            if (use_null_device) {
                p = pcap_create(nullptr, errbuf);
            } else {
                p = pcap_create(device_name.c_str(), errbuf);
            }
            
            if (p != nullptr) {
                pcap_activate(p);
                pcap_close(p);
            }
            break;
            
        case 3:
            // Scenario 3: Test edge cases and invalid parameters
            if (use_null_device) {
                p = pcap_create(nullptr, errbuf);
            } else {
                p = pcap_create(device_name.c_str(), errbuf);
            }
            
            if (p != nullptr) {
                pcap_activate(p);
                
                // Test extreme/edge case parameter values
                pcap_set_datalink(p, datalink_type);
                pcap_set_tstamp_type(p, tstamp_type);
                pcap_set_tstamp_precision(p, precision);
                pcap_setdirection(p, direction);
                
                // Test buffer size edge cases - safe absolute value computation
                if (buffer_size < 0) {
                    // Handle INT_MIN safely to avoid signed integer overflow
                    if (buffer_size == INT_MIN) {
                        buffer_size = INT_MAX;  // Use max positive value
                    } else {
                        buffer_size = -buffer_size;  // Safe negation
                    }
                }
                if (buffer_size == 0) buffer_size = 1;
                pcap_set_buffer_size(p, buffer_size);
                
                // Test error buffer with various inputs
                char custom_errbuf[PCAP_ERRBUF_SIZE];
                if (remaining_input.size() > 0 && remaining_input.size() < PCAP_ERRBUF_SIZE) {
                    strncpy(custom_errbuf, remaining_input.c_str(), PCAP_ERRBUF_SIZE - 1);
                    custom_errbuf[PCAP_ERRBUF_SIZE - 1] = '\0';
                    pcap_setnonblock(p, nonblock, custom_errbuf);
                } else {
                    pcap_setnonblock(p, nonblock, errbuf);
                }
                
                pcap_set_immediate_mode(p, immediate_mode);
                pcap_set_rfmon(p, rfmon);
                pcap_set_protocol_linux(p, protocol);
                
                pcap_close(p);
            }
            break;
    }
    
    return 0;
}

} // namespace harness_005
