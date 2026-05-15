#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_020 {
/*
 * libpcap fuzzing harness - harness_020.cpp
 * Targets critical coverage gaps in Linux-specific pcap APIs with interface enumeration
 * and live capture with callbacks (different from harness_019's pcap_create/activate focus)
 * 
 * Critical Coverage Gap Strategy:
 * - pcap-linux.c has only 6.92% region coverage with 44 out of 55 functions uncovered
 * - Targets different Linux-specific APIs than harness_019
 * - Focuses on interface enumeration and callback mechanisms
 * 
 * APIs to Target from pcap-linux.c (different from harness_019):
 * 1. pcap_findalldevs - Interface discovery (heavily used in pcap-linux.c)
 * 2. pcap_open_live - Live capture with Linux-specific options
 * 3. pcap_setnonblock - Tests Linux non-blocking FD handling
 * 4. pcap_get_selectable_fd - Linux-specific file descriptor access
 * 5. pcap_dispatch/pcap_loop - Linux callback mechanisms
 * 6. pcap_setfilter - Linux BPF filter installation
 * 7. pcap_stats - Linux-specific statistics
 * 
 * Required Setup/Helpers (different from harness_019):
 * 1. pcap_create (initialization) - NOT used, using pcap_open_live instead
 * 2. pcap_activate (activation) - NOT used, pcap_open_live does activation
 * 3. pcap_close (cleanup)
 * 4. pcap_freecode (filter cleanup)
 * 5. pcap_lookupdev (device discovery alternative)
 * 
 * Invocation Sequence (different from harness_019):
 * 1. pcap_findalldevs to discover available interfaces
 * 2. pcap_open_live to open interface for live capture (not pcap_create)
 * 3. pcap_setnonblock to test non-blocking mode
 * 4. pcap_get_selectable_fd to get Linux file descriptor
 * 5. pcap_setfilter to install BPF filter
 * 6. pcap_dispatch with callback to capture packets
 * 7. pcap_stats to collect statistics
 * 8. pcap_close and pcap_freecode for cleanup
 * 
 * Semantic Diversity from Existing Harnesses:
 * - harness_000: Basic BPF compilation with pcap_open_dead
 * - harness_001: BPF filter handling with bpf_dump and bpf_image
 * - harness_002-017: File-based operations, memory FILE* operations, dump operations
 * - harness_018: BPF lexical scanner and evaluation subsystem
 * - harness_019: Linux-specific pcap_create/pcap_activate lifecycle APIs
 * - harness_020: INTERFACE ENUMERATION AND LIVE CAPTURE WITH CALLBACKS (THIS HARNESS)
 * 
 * Strategy:
 * 1. Use pcap_findalldevs to discover available interfaces
 * 2. Use pcap_open_live for live capture (not pcap_create like harness_019)
 * 3. Test pcap_get_selectable_fd for Linux file descriptor access
 * 4. Use pcap_dispatch with callback functions
 * 5. Handle permission issues gracefully for containerized fuzzing
 * 6. Generate diverse BPF filters from fuzzer input
 * 
 * This targets different code paths in pcap-linux.c than harness_019,
 * specifically focusing on interface discovery and callback mechanisms.
 */


// Packet handler callback for pcap_dispatch/pcap_loop
static void packet_handler_callback(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;  // Unused parameter
    (void)header;  // Unused parameter
    (void)packet;  // Unused parameter
    
    // This callback is called for each packet captured
    // In a fuzzing context, we just verify the callback mechanism works
}

// Alternative packet handler with different signature for testing
static void alternative_packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;
    
    // Access header fields to test data flow
    if (header != nullptr) {
        uint32_t caplen = header->caplen;
        uint32_t len = header->len;
        struct timeval ts = header->ts;
        (void)caplen;
        (void)len;
        (void)ts;
    }
    
    // Access packet data if available
    if (packet != nullptr && header != nullptr && header->caplen > 0) {
        // Just read first byte to test access
        uint8_t first_byte = packet[0];
        (void)first_byte;
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);  // 0-10 seconds
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Consume filter string (variable size, limited to reasonable length)
    std::string filter_str = fdp.ConsumeRandomLengthString(512);
    
    // Consume max packet count for pcap_dispatch/pcap_loop
    int max_packets = fdp.ConsumeIntegralInRange<int>(-1, 100);
    if (max_packets == 0) max_packets = 1;  // Ensure at least 1 if not -1
    
    // Choose which packet handler to use
    bool use_alt_handler = fdp.ConsumeBool();
    
    // Step 1: Discover available interfaces using pcap_findalldevs
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    pcap_if_t *device = nullptr;
    std::string selected_device;
    
    int finddev_result = pcap_findalldevs(&alldevs, errbuf);
    
    if (finddev_result == 0 && alldevs != nullptr) {
        // Count devices
        int device_count = 0;
        for (device = alldevs; device != nullptr; device = device->next) {
            device_count++;
        }
        
        // Choose a device based on fuzzer input
        if (device_count > 0) {
            int device_index = fdp.ConsumeIntegralInRange<int>(0, device_count - 1);
            device = alldevs;
            for (int i = 0; i < device_index && device != nullptr; i++) {
                device = device->next;
            }
            
            if (device != nullptr && device->name != nullptr) {
                selected_device = device->name;
            }
        }
    }
    
    // Fallback to loopback if no devices found or pcap_findalldevs failed
    if (selected_device.empty()) {
        // Try pcap_lookupdev as alternative
        char *default_dev = pcap_lookupdev(errbuf);
        if (default_dev != nullptr) {
            selected_device = default_dev;
        } else {
            // Final fallback to loopback for containerized environment
            selected_device = "lo";
        }
    }
    
    // Step 2: Open device for live capture using pcap_open_live (not pcap_create like harness_019)
    pcap_t *pcap_handle = pcap_open_live(selected_device.c_str(), snaplen, promisc, timeout_ms, errbuf);
    
    // If pcap_open_live fails with permission error, try with timeout=0 (non-blocking immediate)
    if (pcap_handle == nullptr && timeout_ms > 0) {
        pcap_handle = pcap_open_live(selected_device.c_str(), snaplen, promisc, 0, errbuf);
    }
    
    // If still no handle, cleanup and return
    if (pcap_handle == nullptr) {
        if (alldevs != nullptr) {
            pcap_freealldevs(alldevs);
        }
        return 0;
    }
    
    // Step 3: Test pcap_setnonblock
    int nonblock_set = fdp.ConsumeBool() ? 1 : 0;
    int setnonblock_result = pcap_setnonblock(pcap_handle, nonblock_set, errbuf);
    (void)setnonblock_result;  // Use result to avoid unused variable warning
    
    // Step 4: Test pcap_get_selectable_fd (Linux-specific file descriptor)
    int selectable_fd = pcap_get_selectable_fd(pcap_handle);
    (void)selectable_fd;  // Use result to avoid unused variable warning
    
    // Step 5: Compile and set BPF filter
    struct bpf_program bpf_prog;
    int compile_result = pcap_compile(pcap_handle, &bpf_prog, filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // Filter compiled successfully, apply it
        int setfilter_result = pcap_setfilter(pcap_handle, &bpf_prog);
        (void)setfilter_result;
        
        // Free the compiled filter (will be done later in cleanup)
    }
    
    // Step 6: Test pcap_dispatch with callback
    // Since we're in a fuzzing environment without actual packets,
    // pcap_dispatch will typically return 0 (no packets) or -1 (error)
    int dispatch_result = 0;
    if (use_alt_handler) {
        dispatch_result = pcap_dispatch(pcap_handle, max_packets, alternative_packet_handler, nullptr);
    } else {
        dispatch_result = pcap_dispatch(pcap_handle, max_packets, packet_handler_callback, nullptr);
    }
    (void)dispatch_result;
    
    // Step 7: Test pcap_loop as alternative to pcap_dispatch
    if (max_packets > 0 && fdp.ConsumeBool()) {
        int loop_result = pcap_loop(pcap_handle, max_packets, packet_handler_callback, nullptr);
        (void)loop_result;
    }
    
    // Step 8: Test pcap_next_ex for single packet reading
    if (fdp.ConsumeBool()) {
        struct pcap_pkthdr *header = nullptr;
        const u_char *packet = nullptr;
        int next_ex_result = pcap_next_ex(pcap_handle, &header, &packet);
        (void)next_ex_result;
        (void)header;
        (void)packet;
    }
    
    // Step 9: Test pcap_stats
    struct pcap_stat stats;
    memset(&stats, 0, sizeof(stats));
    int stats_result = pcap_stats(pcap_handle, &stats);
    (void)stats_result;
    
    // Access stats fields to ensure they're read
    uint32_t ps_recv = stats.ps_recv;
    uint32_t ps_drop = stats.ps_drop;
    uint32_t ps_ifdrop = stats.ps_ifdrop;
    (void)ps_recv;
    (void)ps_drop;
    (void)ps_ifdrop;
    
    // Step 10: Test pcap_datalink
    int datalink = pcap_datalink(pcap_handle);
    (void)datalink;
    
    // Step 11: Test pcap_snapshot
    int snapshot = pcap_snapshot(pcap_handle);
    (void)snapshot;
    
    // Step 12: Test pcap_is_swapped (endianness detection)
    int is_swapped = pcap_is_swapped(pcap_handle);
    (void)is_swapped;
    
    // Step 13: Test pcap_major_version and pcap_minor_version
    int major_version = pcap_major_version(pcap_handle);
    int minor_version = pcap_minor_version(pcap_handle);
    (void)major_version;
    (void)minor_version;
    
    // Step 14: Test pcap_geterr to retrieve error message
    const char *last_error = pcap_geterr(pcap_handle);
    (void)last_error;
    
    // Cleanup
    if (compile_result == 0) {
        pcap_freecode(&bpf_prog);
    }
    
    pcap_close(pcap_handle);
    
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    return 0;
}

} // namespace harness_020
