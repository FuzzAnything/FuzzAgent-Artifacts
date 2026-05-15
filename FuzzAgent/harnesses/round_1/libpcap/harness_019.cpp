#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace harness_019 {
/*
 * libpcap fuzzing harness - harness_019.cpp
 * Targets Linux-specific pcap APIs in pcap-linux.c module (critical coverage gap)
 * 
 * Critical Coverage Gap Strategy:
 * - pcap-linux.c has only 6.92% region coverage with 44 out of 55 functions uncovered
 * - Largest remaining coverage gap after 18 fuzzers
 * - Targets Linux-specific live capture APIs rather than file operations
 * 
 * APIs to Target from pcap-linux.c:
 * 1. pcap_activate_linux - Main activation function for Linux devices
 * 2. pcap_inject_linux - Packet injection on Linux
 * 3. pcap_setfilter_linux - BPF filter setup on Linux
 * 4. pcap_setdirection_linux - Set packet capture direction
 * 5. pcap_getnonblock_linux - Get non-blocking mode
 * 6. pcap_setnonblock_linux - Set non-blocking mode
 * 7. pcap_stats_linux - Get statistics
 * 8. pcap_set_datalink_linux - Set datalink type
 * 
 * Required Helper APIs:
 * 1. pcap_create - Create pcap handle
 * 2. pcap_close - Clean up resources
 * 3. pcap_compile - Compile BPF filter
 * 4. pcap_freecode - Free BPF program
 * 
 * Invocation Sequence:
 * 1. Call pcap_create to create a pcap handle for a network interface
 * 2. Call pcap_activate_linux to activate the handle
 * 3. Call pcap_setnonblock_linux to set non-blocking mode
 * 4. Call pcap_getnonblock_linux to verify mode
 * 5. Call pcap_compile to create a BPF filter program
 * 6. Call pcap_setfilter_linux to apply the filter
 * 7. Call pcap_setdirection_linux to set capture direction
 * 8. Call pcap_set_datalink_linux to set datalink type
 * 9. Call pcap_inject_linux to inject test packets
 * 10. Call pcap_stats_linux to retrieve statistics
 * 11. Call pcap_freecode to free BPF program
 * 12. Call pcap_close to clean up
 * 
 * Differentiates from existing harnesses:
 * - harness_000: Basic BPF compilation with pcap_open_dead
 * - harness_001: BPF filter handling with bpf_dump and bpf_image
 * - harness_002-017: File-based operations, memory FILE* operations, dump operations
 * - harness_018: BPF lexical scanner and evaluation subsystem
 * - harness_019: LINUX-SPECIFIC LIVE CAPTURE APIs (THIS HARNESS)
 * 
 * Strategy:
 * 1. Use pcap_create with "lo" or "any" interface for containerized environment
 * 2. Test complete Linux API lifecycle from creation to cleanup
 * 3. Generate diverse BPF filters and packet data from fuzzer input
 * 4. Test different capture directions and datalink types
 * 5. Handle permission issues gracefully for containerized fuzzing
 * 
 * This targets 44 currently uncovered functions in pcap-linux.c alone.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int timeout = fdp.ConsumeIntegralInRange<int>(0, 1000);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(0, 10 * 1024 * 1024); // 0-10MB
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    int rfmon = fdp.ConsumeBool() ? 1 : 0;
    int immediate = fdp.ConsumeBool() ? 1 : 0;
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Choose interface - use "lo" (loopback) for containerized environment
    // or "any" if loopback not available
    std::string interface;
    int interface_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
    switch (interface_choice) {
        case 0:
            interface = "lo";
            break;
        case 1:
            interface = "any";
            break;
        case 2:
            interface = "eth0";
            break;
    }
    
    // Consume filter string (variable size, limited to reasonable length)
    std::string filter_str = fdp.ConsumeRandomLengthString(512);
    
    // Consume packet data for injection
    size_t packet_data_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
    std::vector<uint8_t> packet_data;
    if (packet_data_size > 0) {
        packet_data = fdp.ConsumeBytes<uint8_t>(packet_data_size);
    }
    
    // Step 1: Create pcap handle
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_create(interface.c_str(), errbuf);
    if (pcap_handle == nullptr) {
        // pcap_create failed, can't proceed
        return 0;
    }
    
    // Set pcap options before activation
    pcap_set_snaplen(pcap_handle, snaplen);
    pcap_set_timeout(pcap_handle, timeout);
    pcap_set_buffer_size(pcap_handle, buffer_size);
    pcap_set_promisc(pcap_handle, promisc);
    pcap_set_rfmon(pcap_handle, rfmon);
    pcap_set_immediate_mode(pcap_handle, immediate);
    
    // Step 2: Activate the handle (calls pcap_activate_linux internally)
    int activate_result = pcap_activate(pcap_handle);
    
    // If activation fails with permission error, we can still test some APIs
    // before cleanup. For other errors, cleanup and return.
    if (activate_result < 0 && activate_result != PCAP_ERROR_PERM_DENIED) {
        pcap_close(pcap_handle);
        return 0;
    }
    
    // Step 3: Set non-blocking mode
    int nonblock_result = pcap_setnonblock(pcap_handle, 1, errbuf);
    (void)nonblock_result; // Use result to avoid unused variable warning
    
    // Step 4: Get non-blocking mode
    int nonblock_mode = 0;
    int getnonblock_result = pcap_getnonblock(pcap_handle, errbuf);
    if (getnonblock_result >= 0) {
        nonblock_mode = getnonblock_result;
    }
    
    // Step 5: Compile BPF filter
    struct bpf_program bpf_prog;
    int compile_result = pcap_compile(pcap_handle, &bpf_prog, 
                                      filter_str.c_str(), optimize, netmask);
    
    if (compile_result == 0) {
        // Step 6: Apply filter (calls pcap_setfilter_linux internally)
        int setfilter_result = pcap_setfilter(pcap_handle, &bpf_prog);
        (void)setfilter_result;
        
        // Step 11: Free BPF program (will be called later)
    }
    
    // Step 7: Set capture direction (if supported)
    // Note: pcap_setdirection is the public API that calls pcap_setdirection_linux
    int direction_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
    pcap_direction_t direction;
    switch (direction_choice) {
        case 0:
            direction = PCAP_D_IN;
            break;
        case 1:
            direction = PCAP_D_OUT;
            break;
        case 2:
            direction = PCAP_D_INOUT;
            break;
        default:
            direction = PCAP_D_INOUT;
            break;
    }
    int setdirection_result = pcap_setdirection(pcap_handle, direction);
    (void)setdirection_result;
    
    // Step 8: Set datalink type (calls pcap_set_datalink_linux internally)
    // Get list of supported datalinks
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
    if (dlt_count > 0 && dlt_list != nullptr) {
        // Choose a datalink type from the supported list
        int dlt_index = fdp.ConsumeIntegralInRange<int>(0, dlt_count - 1);
        int setdatalink_result = pcap_set_datalink(pcap_handle, dlt_list[dlt_index]);
        (void)setdatalink_result;
        pcap_free_datalinks(dlt_list);
    }
    
    // Step 9: Inject packet (calls pcap_inject_linux internally)
    if (packet_data.size() > 0) {
        int inject_result = pcap_inject(pcap_handle, packet_data.data(), packet_data.size());
        (void)inject_result;
    }
    
    // Step 10: Get statistics (calls pcap_stats_linux internally)
    struct pcap_stat stats;
    int stats_result = pcap_stats(pcap_handle, &stats);
    (void)stats_result;
    
    // Step 11: Free BPF program if it was compiled
    if (compile_result == 0) {
        pcap_freecode(&bpf_prog);
    }
    
    // Step 12: Clean up
    pcap_close(pcap_handle);
    
    return 0;
}

} // namespace harness_019
