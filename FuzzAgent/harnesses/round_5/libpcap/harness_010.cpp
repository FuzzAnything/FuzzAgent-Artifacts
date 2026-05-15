#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>

namespace harness_010 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for configuration parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different configuration workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume device name for pcap_create (try "any" or "lo" as likely valid devices)
    std::string device_name = fdp.ConsumeBool() ? "any" : "lo";
    
    // Consume configuration parameters
    int protocol = fdp.ConsumeIntegral<int>();
    int immediate_mode = fdp.ConsumeBool() ? 1 : 0;
    pcap_direction_t direction = static_cast<pcap_direction_t>(
        fdp.ConsumeIntegralInRange<int>(0, 2)); // PCAP_D_IN, PCAP_D_OUT, PCAP_D_INOUT
    int nonblock = fdp.ConsumeBool() ? 1 : 0;
    int rfmon = fdp.ConsumeBool() ? 1 : 0;
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
    int timeout = fdp.ConsumeIntegralInRange<int>(-1, 10000);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(0, 1048576);
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    switch (operation) {
        case 0: {
            // Test Linux-specific configuration APIs with pcap_create and activation
            pcap_t* handle = pcap_create(device_name.c_str(), errbuf);
            if (handle == nullptr) {
                return 0;
            }
            
            // Apply various configuration settings (should work on unactivated handle)
            #ifdef __linux__
            pcap_set_protocol_linux(handle, protocol);
            #endif
            
            pcap_set_immediate_mode(handle, immediate_mode);
            pcap_setdirection(handle, direction);
            pcap_setnonblock(handle, nonblock, errbuf);
            
            // Check if RFMON is supported before trying to set it
            if (pcap_can_set_rfmon(handle) == 1) {
                pcap_set_rfmon(handle, rfmon);
            }
            
            pcap_set_snaplen(handle, snaplen);
            pcap_set_timeout(handle, timeout);
            pcap_set_buffer_size(handle, buffer_size);
            pcap_set_promisc(handle, promisc);
            
            // Try to activate the handle (may fail in fuzzing environment, that's OK)
            int activate_result = pcap_activate(handle);
            
            // Query various properties (should work even if activation failed)
            if (activate_result >= 0 || activate_result == PCAP_WARNING_PROMISC_NOTSUP || 
                activate_result == PCAP_WARNING_TSTAMP_TYPE_NOTSUP) {
                // Query buffer size
                int current_bufsize = pcap_bufsize(handle);
                (void)current_bufsize;
                
                // Query data link type
                int datalink = pcap_datalink(handle);
                int datalink_ext = pcap_datalink_ext(handle);
                (void)datalink;
                (void)datalink_ext;
                
                // Get non-blocking status
                int current_nonblock = pcap_getnonblock(handle, errbuf);
                (void)current_nonblock;
            }
            
            pcap_close(handle);
            break;
        }
        
        case 1: {
            // Test configuration APIs on pcap_open_dead (dead handle)
            int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            // Some APIs should still work on dead handles
            #ifdef __linux__
            // pcap_set_protocol_linux might not work on dead handles, but try
            pcap_set_protocol_linux(handle, protocol);
            #endif
            
            pcap_set_immediate_mode(handle, immediate_mode);
            
            // pcap_setdirection should work
            pcap_setdirection(handle, direction);
            
            // pcap_setnonblock on dead handle
            pcap_setnonblock(handle, nonblock, errbuf);
            
            // Query buffer size (should return PCAP_ERROR_NOT_ACTIVATED)
            int current_bufsize = pcap_bufsize(handle);
            (void)current_bufsize;
            
            // Query data link type (should work)
            int datalink = pcap_datalink(handle);
            int datalink_ext = pcap_datalink_ext(handle);
            (void)datalink;
            (void)datalink_ext;
            
            pcap_close(handle);
            break;
        }
        
        case 2: {
            // Test libpcap version APIs and misc queries
            // Get libpcap version string
            const char* version_str = pcap_lib_version();
            (void)version_str;
            
            // Create a handle for testing misc queries
            pcap_t* handle = pcap_create(device_name.c_str(), errbuf);
            if (handle == nullptr) {
                return 0;
            }
            
            // Test timestamp precision APIs
            int tstamp_precision = fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
            pcap_set_tstamp_precision(handle, tstamp_precision);
            int current_precision = pcap_get_tstamp_precision(handle);
            (void)current_precision;
            
            // Test timestamp type APIs
            int* tstamp_types = nullptr;
            int tstamp_type_count = pcap_list_tstamp_types(handle, &tstamp_types);
            if (tstamp_type_count > 0 && tstamp_types != nullptr) {
                // Try to set a timestamp type if available
                if (tstamp_type_count > 0) {
                    pcap_set_tstamp_type(handle, tstamp_types[0]);
                }
                pcap_free_tstamp_types(tstamp_types);
            }
            
            // Test major/minor version queries (may fail on unactivated handle)
            int major_version = pcap_major_version(handle);
            int minor_version = pcap_minor_version(handle);
            (void)major_version;
            (void)minor_version;
            
            pcap_close(handle);
            break;
        }
        
        case 3: {
            // Test comprehensive configuration sequence with error handling
            pcap_t* handle = pcap_create(device_name.c_str(), errbuf);
            if (handle == nullptr) {
                // Try with different device names
                handle = pcap_create("lo", errbuf);
                if (handle == nullptr) {
                    handle = pcap_create("eth0", errbuf);
                    if (handle == nullptr) {
                        return 0;
                    }
                }
            }
            
            // Apply configuration in different orders
            std::vector<uint8_t> config_order = fdp.ConsumeBytes<uint8_t>(10);
            
            for (size_t i = 0; i < config_order.size(); i++) {
                uint8_t config_step = config_order[i] % 10;
                switch (config_step) {
                    case 0:
                        pcap_set_snaplen(handle, snaplen);
                        break;
                    case 1:
                        pcap_set_timeout(handle, timeout);
                        break;
                    case 2:
                        pcap_set_buffer_size(handle, buffer_size);
                        break;
                    case 3:
                        pcap_set_promisc(handle, promisc);
                        break;
                    case 4:
                        #ifdef __linux__
                        pcap_set_protocol_linux(handle, protocol);
                        #endif
                        break;
                    case 5:
                        pcap_set_immediate_mode(handle, immediate_mode);
                        break;
                    case 6:
                        pcap_setdirection(handle, direction);
                        break;
                    case 7:
                        pcap_setnonblock(handle, nonblock, errbuf);
                        break;
                    case 8:
                        if (pcap_can_set_rfmon(handle) == 1) {
                            pcap_set_rfmon(handle, rfmon);
                        }
                        break;
                    case 9:
                        pcap_set_tstamp_precision(handle, 
                            fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO);
                        break;
                }
            }
            
            // Try activation
            int activate_result = pcap_activate(handle);
            
            // If activation succeeded or gave warnings, do more queries
            if (activate_result >= 0 || activate_result == PCAP_WARNING || 
                activate_result == PCAP_WARNING_PROMISC_NOTSUP ||
                activate_result == PCAP_WARNING_TSTAMP_TYPE_NOTSUP) {
                
                // Get file descriptor
                int fd = pcap_fileno(handle);
                (void)fd;
                
                #ifndef _WIN32
                // Get selectable FD (Linux-specific)
                int selectable_fd = pcap_get_selectable_fd(handle);
                (void)selectable_fd;
                #endif
                
                // Check if handle is swapped
                int swapped = pcap_is_swapped(handle);
                (void)swapped;
            }
            
            pcap_close(handle);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_010
