#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_000 {
/*
 * libpcap fuzzing harness - harness_000.cpp
 * Targets core libpcap APIs for packet capture and filtering operations
 * 
 * Primary APIs targeted:
 * - pcap_open_live (simulated with pcap_open_dead for fuzzing)
 * - pcap_compile
 * - pcap_setfilter
 * - pcap_next
 * - pcap_loop
 * - pcap_datalink
 * - pcap_stats
 * - pcap_close
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    
    // Consume filter string (variable size, limited to reasonable length)
    std::string filter_str = fdp.ConsumeRandomLengthString(256);
    
    // Leave some bytes for packet simulation if available
    size_t bytes_for_packet = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
    std::vector<uint8_t> packet_data;
    if (bytes_for_packet > 0) {
        packet_data = fdp.ConsumeBytes<uint8_t>(bytes_for_packet);
    }
    
    // Create a pcap handle for a dead capture (no actual device needed)
    pcap_t *pcap_handle = pcap_open_dead(linktype, snaplen);
    if (pcap_handle == nullptr) {
        return 0;
    }

    // Test pcap_datalink
    int datalink_result = pcap_datalink(pcap_handle);
    (void)datalink_result;  // Use result to avoid unused variable warning

    // Test filter compilation and setting
    struct bpf_program bpf;
    int compile_result = pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN);
    
    if (compile_result == 0) {
        // Filter compiled successfully, try to set it
        int setfilter_result = pcap_setfilter(pcap_handle, &bpf);
        (void)setfilter_result;
        
        // Free the compiled filter
        pcap_freecode(&bpf);
    }

    // Test pcap_stats (even though we have no packets)
    struct pcap_stat stats;
    int stats_result = pcap_stats(pcap_handle, &stats);
    (void)stats_result;

    // Test pcap_next with dummy packet header if we have packet data
    if (packet_data.size() > 0) {
        struct pcap_pkthdr header;
        // Use the packet data size for realistic header values
        header.caplen = std::min(packet_data.size(), static_cast<size_t>(snaplen));
        header.len = packet_data.size();
        
        // For testing pcap_next, we need to simulate having packets
        // Since we're using pcap_open_dead, pcap_next will return NULL
        const u_char *packet = pcap_next(pcap_handle, &header);
        (void)packet;
    }

    // Define packet handler callback for pcap_loop
    auto packet_handler = [](u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
        (void)user;
        (void)h;
        (void)bytes;
    };
    
    // Test pcap_loop with a simple packet handler
    // Note: pcap_loop won't actually process packets with pcap_open_dead,
    // but we're testing the API call itself
    int loop_result = pcap_loop(pcap_handle, 0, packet_handler, nullptr);
    (void)loop_result;

    // Clean up
    pcap_close(pcap_handle);

    return 0;
}

} // namespace harness_000
