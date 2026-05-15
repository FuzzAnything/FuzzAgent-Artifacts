#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

namespace harness_021 {
/*
 * Harness for Linux-specific mmap packet capture implementation (pcap-linux.c)
 * Primary Target: pcap_read_linux_mmap_v2 (uncovered), pcap_read_linux_mmap_v3 (uncovered), 
 *                 pcap_setfilter_linux (uncovered), and other internal Linux mmap functions
 * Coverage Goal: Target Linux mmap packet capture, protocol configuration, and filter setup
 *                to cover hundreds of undiscovered branches with only 7.31% region coverage
 * Strategy: Create pcap handles that use Linux mmap mode (TPACKET_V2/V3) and test
 *           filter setup in both kernel and userland paths
 */


// Forward declaration of helper function
static void test_bpf_filters(pcap_t* pcap_handle, FuzzedDataProvider& fdp, int complexity);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for Linux mmap testing
    // Need enough data for device name, control parameters, BPF filter programs, and test scenarios
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Consume control parameters for test configuration
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which mmap test path to take
    int use_any_device = fdp.ConsumeBool() ? 1 : 0;            // Use "any" device vs specific interface
    int use_mmap_mode = 1;                                     // Always try mmap mode for this harness
    int promisc_mode = fdp.ConsumeBool() ? 1 : 0;              // Promiscuous mode
    int immediate_mode = fdp.ConsumeBool() ? 1 : 0;            // Immediate mode
    int test_filter_complexity = fdp.ConsumeIntegralInRange<int>(0, 2);  // Filter complexity level
    // Capture parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 1000);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);
    
    // TPACKET version control (if we can influence it)
    int try_tpacket_v3 = fdp.ConsumeBool() ? 1 : 0;            // Try to use TPACKET_V3 if available
    std::string device_name;
    if (use_any_device) {
        device_name = "any";  // Linux "any" device - may not support mmap but tests different paths
    } else {
        // Use common Linux interfaces that might support mmap
        const char* linux_devices[] = {"lo", "eth0", "wlan0", "docker0", "veth0"};
        size_t device_idx = fdp.ConsumeIntegralInRange<size_t>(0, 4);
        device_name = linux_devices[device_idx];
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    
    // Phase 2: Create pcap handle with pcap_create (allows configuration before activation)
    pcap_handle = pcap_create(device_name.c_str(), errbuf);
    if (pcap_handle == nullptr) {
        // If pcap_create fails, we can still test some error paths
        // Try pcap_open_live as fallback
        pcap_handle = pcap_open_live(device_name.c_str(), snaplen, promisc_mode, timeout_ms, errbuf);
        if (pcap_handle == nullptr) {
            // Both methods failed, test standalone error functions
            const char* error_str = pcap_strerror(PCAP_ERROR);
            (void)error_str;
            return 0;
        }
    } else {
        // Configure handle before activation (required for mmap mode setup)
        pcap_set_snaplen(pcap_handle, snaplen);
        pcap_set_promisc(pcap_handle, promisc_mode);
        pcap_set_timeout(pcap_handle, timeout_ms);
        pcap_set_buffer_size(pcap_handle, buffer_size);
        pcap_set_immediate_mode(pcap_handle, immediate_mode);
        
        // Try to influence TPACKET version if possible
        // Note: This may not be directly controllable via public API
        // but we try different configurations that might affect it
        
        // Activate the handle - this is where pcap_activate_linux sets up mmap mode
        int activate_result = pcap_activate(pcap_handle);
        if (activate_result < 0 && activate_result != PCAP_WARNING) {
            // Activation failed, clean up
            pcap_close(pcap_handle);
            pcap_handle = nullptr;
            
            // Test with a dead handle as fallback
            pcap_t* dead_handle = pcap_open_dead(DLT_EN10MB, snaplen);
            if (dead_handle != nullptr) {
                // Even with dead handle, we can test filter compilation
                test_bpf_filters(dead_handle, fdp, test_filter_complexity);
                pcap_close(dead_handle);
            }
            return 0;
        }
    }
    
    // Phase 3: Test different scenarios based on test_scenario
    switch (test_scenario) {
        case 0: {
            // Scenario 0: Basic mmap read path testing
            // Try to read packets (may block or return immediately depending on configuration)
            struct pcap_pkthdr* header = nullptr;
            const u_char* packet_data = nullptr;
            
            // Try pcap_next_ex which may call mmap read functions
            int read_result = pcap_next_ex(pcap_handle, &header, &packet_data);
            (void)read_result;  // Result varies based on available packets
            
            // Test pcap_dispatch with small count
            int dispatch_count = fdp.ConsumeIntegralInRange<int>(1, 10);
            int dispatch_result = pcap_dispatch(pcap_handle, dispatch_count, 
                [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                    // Simple callback that does nothing
                    (void)user;
                    (void)h;
                    (void)bytes;
                    return;
                }, nullptr);
            (void)dispatch_result;
            break;
        }
            
        case 1: {
            // Scenario 1: Filter testing with mmap mode
            // PRIMARY TARGET: pcap_setfilter_linux
            test_bpf_filters(pcap_handle, fdp, test_filter_complexity);
            break;
        }
            
        case 2: {
            // Scenario 2: Break loop testing with mmap
            // Set up a break loop condition
            pcap_breakloop(pcap_handle);
            
            // Try to read despite break loop
            struct pcap_pkthdr* header = nullptr;
            const u_char* packet_data = nullptr;
            int read_result = pcap_next_ex(pcap_handle, &header, &packet_data);
            (void)read_result;
            
            // Clear break loop and test again
            // Note: There's no pcap_clearloop, so we'll just continue
            break;
        }
            
        case 3: {
            // Scenario 3: Combined testing - filters with statistics
            test_bpf_filters(pcap_handle, fdp, test_filter_complexity);
            
            // Get statistics (may involve kernel interactions)
            struct pcap_stat stats;
            int stats_result = pcap_stats(pcap_handle, &stats);
            (void)stats_result;
            
            // Test non-blocking mode with mmap
            int nonblock_result = pcap_setnonblock(pcap_handle, 1, errbuf);
            (void)nonblock_result;
            
            if (nonblock_result == 0) {
                // Try immediate read in non-blocking mode
                struct pcap_pkthdr* header = nullptr;
                const u_char* packet_data = nullptr;
                int read_result = pcap_next_ex(pcap_handle, &header, &packet_data);
                (void)read_result;
                
                // Switch back to blocking
                pcap_setnonblock(pcap_handle, 0, errbuf);
            }
            break;
        }
    }
    
    // Phase 4: Cleanup
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
    }
    
    return 0;
}

// Helper function to test BPF filters with varying complexity
// This targets pcap_setfilter_linux and related compilation paths
static void test_bpf_filters(pcap_t* pcap_handle, FuzzedDataProvider& fdp, int complexity) {
    struct bpf_program bpf;
    
    // Generate filter strings based on complexity
    std::string filter_str;
    switch (complexity) {
        case 0:
            // Simple filters
            filter_str = fdp.ConsumeRandomLengthString(64);
            break;
            
        case 1: {
            // Medium complexity - common BPF expressions
            const char* common_filters[] = {
                "tcp", "udp", "port 80", "host 192.168.1.1",
                "icmp", "arp", "vlan", "broadcast",
                "greater 100", "less 512", "ip proto 6"
            };
            size_t filter_idx = fdp.ConsumeIntegralInRange<size_t>(0, 10);
            filter_str = common_filters[filter_idx];
            break;
        }
            
        case 2: {
            // Complex filters - combined expressions
            const char* complex_filters[] = {
                "tcp port 80 and host 192.168.1.1",
                "udp port 53 or udp port 67",
                "icmp and (host 192.168.1.1 or host 192.168.1.2)",
                "not ether broadcast",
                "ip[0] & 0xf0 == 0x40",  // IPv4 header check
                "tcp[tcpflags] & (tcp-syn|tcp-fin) != 0",
                "ether[0] & 1 = 0",  // Not multicast
                "ip len > 576"
            };
            size_t filter_idx = fdp.ConsumeIntegralInRange<size_t>(0, 7);
            filter_str = complex_filters[filter_idx];
            
            // Add some fuzzed variation
            if (fdp.ConsumeBool()) {
                filter_str += " and port " + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
            }
            break;
        }
    }
    
    // Consume network mask from fuzzer input
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Try to compile and set the filter
    // PRIMARY TARGET: pcap_compile -> may call pcap_compile_linux
    // PRIMARY TARGET: pcap_setfilter -> calls pcap_setfilter_linux for Linux handles
    int compile_result = pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, netmask);
    
    if (compile_result == 0) {
        // Filter compiled successfully, try to set it
        // This will call pcap_setfilter_linux for Linux handles with mmap mode
        int setfilter_result = pcap_setfilter(pcap_handle, &bpf);
        (void)setfilter_result;
        
        // Free the compiled code
        pcap_freecode(&bpf);
    } else {
        // Compilation failed - test error path
        const char* err = pcap_geterr(pcap_handle);
        (void)err;
    }
    
    // Test with empty/invalid filter string
    if (fdp.ConsumeBool()) {
        std::string empty_filter = fdp.ConsumeRandomLengthString(32);
        // Might be empty or invalid
        int empty_compile_result = pcap_compile(pcap_handle, &bpf, empty_filter.c_str(), 0, netmask);
        (void)empty_compile_result;
        if (empty_compile_result == 0) {
            pcap_freecode(&bpf);
        }
    }
}

} // namespace harness_021
