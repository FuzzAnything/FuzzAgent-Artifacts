#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_003 {
/*
 * libpcap fuzzing harness - harness_003.cpp
 * Targets live packet capture device management APIs with complete lifecycle
 * 
 * Primary APIs targeted:
 * - pcap_findalldevs (device discovery)
 * - pcap_freealldevs (device list cleanup)
 * - pcap_open_live (legacy live capture)
 * - pcap_create (modern device creation)
 * - pcap_activate (device activation)
 * - pcap_setnonblock (non-blocking mode)
 * - pcap_getnonblock (get non-blocking status)
 * - pcap_setdirection (packet direction filtering)
 * - pcap_close (cleanup)
 * 
 * Differentiates from existing harnesses:
 * - harness_000: Core packet capture APIs (uses pcap_open_dead)
 * - harness_001: BPF filter handling functions  
 * - harness_002: File I/O subsystem operations
 * - harness_003: Live device management lifecycle (THIS HARNESS)
 * 
 * Targets the 200+ undiscovered complexity points in live device management
 * subsystem identified by CoverageAnalyzerAgent.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    int nonblock = fdp.ConsumeBool() ? 1 : 0;
    int direction_val = fdp.ConsumeIntegralInRange<int>(0, 2);
    pcap_direction_t direction;
    
    // Map direction value to enum
    switch (direction_val) {
        case 0: direction = PCAP_D_IN; break;
        case 1: direction = PCAP_D_OUT; break;
        case 2: direction = PCAP_D_INOUT; break;
        default: direction = PCAP_D_INOUT; break;
    }
    
    // Consume device name string (variable size, limited length)
    std::string device_name = fdp.ConsumeRandomLengthString(256);
    
    // Test 1: Device discovery with pcap_findalldevs
    pcap_if_t* alldevs = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    int find_result = pcap_findalldevs(&alldevs, errbuf);
    (void)find_result;  // Use result to avoid unused variable warning
    
    if (alldevs != nullptr) {
        // Test 2: Legacy pcap_open_live API
        pcap_t* legacy_handle = pcap_open_live(
            device_name.empty() ? "any" : device_name.c_str(),
            snaplen,
            promisc,
            timeout_ms,
            errbuf
        );
        
        if (legacy_handle != nullptr) {
            // Test non-blocking mode APIs on legacy handle
            char nonblock_errbuf[PCAP_ERRBUF_SIZE];
            int set_nonblock_result = pcap_setnonblock(legacy_handle, nonblock, nonblock_errbuf);
            (void)set_nonblock_result;
            
            int get_nonblock_result = pcap_getnonblock(legacy_handle, nonblock_errbuf);
            (void)get_nonblock_result;
            
            // Test direction setting
            int setdir_result = pcap_setdirection(legacy_handle, direction);
            (void)setdir_result;
            
            // Clean up legacy handle
            pcap_close(legacy_handle);
        }
        
        // Test 3: Modern pcap_create + pcap_activate API
        pcap_t* modern_handle = pcap_create(
            device_name.empty() ? "any" : device_name.c_str(),
            errbuf
        );
        
        if (modern_handle != nullptr) {
            // Configure the pcap handle before activation
            int set_snaplen_result = pcap_set_snaplen(modern_handle, snaplen);
            (void)set_snaplen_result;
            
            int set_promisc_result = pcap_set_promisc(modern_handle, promisc);
            (void)set_promisc_result;
            
            int set_timeout_result = pcap_set_timeout(modern_handle, timeout_ms);
            (void)set_timeout_result;
            
            // Test buffer size setting
            int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);  // 1KB to 1MB
            int set_buffer_result = pcap_set_buffer_size(modern_handle, buffer_size);
            (void)set_buffer_result;
            
            // Test immediate mode setting
            int immediate_mode = fdp.ConsumeBool() ? 1 : 0;
            int set_immediate_result = pcap_set_immediate_mode(modern_handle, immediate_mode);
            (void)set_immediate_result;
            
            // Activate the handle
            int activate_result = pcap_activate(modern_handle);
            (void)activate_result;
            
            if (activate_result >= 0) {
                // Handle successfully activated, test more APIs
                
                // Test non-blocking mode
                char modern_nb_errbuf[PCAP_ERRBUF_SIZE];
                int modern_set_nb = pcap_setnonblock(modern_handle, nonblock, modern_nb_errbuf);
                (void)modern_set_nb;
                
                int modern_get_nb = pcap_getnonblock(modern_handle, modern_nb_errbuf);
                (void)modern_get_nb;
                
                // Test direction setting
                int modern_setdir = pcap_setdirection(modern_handle, direction);
                (void)modern_setdir;
                
                // Test datalink
                int datalink_result = pcap_datalink(modern_handle);
                (void)datalink_result;
                
                // Test snapshot length
                int snapshot_result = pcap_snapshot(modern_handle);
                (void)snapshot_result;
                
                // Test stats (even though we may not have packets)
                struct pcap_stat stats;
                int stats_result = pcap_stats(modern_handle, &stats);
                (void)stats_result;
            }
            
            // Clean up modern handle
            pcap_close(modern_handle);
        }
        
        // Test 4: Iterate through discovered devices and test pcap_lookupnet
        pcap_if_t* current_dev = alldevs;
        int device_count = 0;
        const int MAX_DEVICES_TO_TEST = 5;  // Limit to prevent excessive testing
        
        while (current_dev != nullptr && device_count < MAX_DEVICES_TO_TEST) {
            bpf_u_int32 net, mask;
            char lookup_errbuf[PCAP_ERRBUF_SIZE];
            
            int lookup_result = pcap_lookupnet(current_dev->name, &net, &mask, lookup_errbuf);
            (void)lookup_result;
            
            current_dev = current_dev->next;
            device_count++;
            
            // Consume some fuzzer data to vary test parameters
            if (fdp.remaining_bytes() > 0) {
                int test_param = fdp.ConsumeIntegralInRange<int>(0, 10);
                (void)test_param;
            }
        }
    }
    
    // Test 5: pcap_findalldevs_ex with remote source simulation
    if (fdp.remaining_bytes() > 100) {
        std::string remote_source = fdp.ConsumeRandomLengthString(100);
        
        pcap_if_t* remote_devs = nullptr;
        char remote_errbuf[PCAP_ERRBUF_SIZE];
        
        // Note: pcap_findalldevs_ex may fail for invalid sources, but we test the API call
        int remote_find_result = pcap_findalldevs_ex(remote_source.c_str(), nullptr, &remote_devs, remote_errbuf);
        (void)remote_find_result;
        
        if (remote_devs != nullptr) {
            pcap_freealldevs(remote_devs);
        }
    }
    
    // Clean up device list
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    // Test 6: Additional error cases with invalid parameters
    if (fdp.remaining_bytes() > 0) {
        // Test with extremely large or small parameters
        int extreme_snaplen = fdp.ConsumeIntegralInRange<int>(-1000, 1000000);
        int extreme_timeout = fdp.ConsumeIntegralInRange<int>(-1000, 1000000);
        
        pcap_t* extreme_handle = pcap_create("invalid_device_xyz", errbuf);
        if (extreme_handle != nullptr) {
            // These may fail but test the API paths
            pcap_set_snaplen(extreme_handle, extreme_snaplen);
            pcap_set_timeout(extreme_handle, extreme_timeout);
            
            // Test activation with potentially invalid settings
            int extreme_activate = pcap_activate(extreme_handle);
            (void)extreme_activate;
            
            pcap_close(extreme_handle);
        }
    }
    
    return 0;
}

} // namespace harness_003
