#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>

namespace harness_006 {


// Packet callback handler for pcap_loop
static void packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    // Simple packet handler that just counts packets
    int *packet_count = (int *)user;
    if (packet_count != nullptr) {
        (*packet_count)++;
    }
    
    // We could process the packet here, but for fuzzing we just exercise the callback
    // The packet data is already validated by libpcap
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need minimum input for meaningful testing:
    // - Scenario selection (1 byte)
    // - Device name (variable)
    // - Snaplen (2 bytes)
    // - Buffer size (2 bytes)
    // - Packet count (1 byte)
    // - Packet data (0+ bytes, can be empty)
    if (Size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume device name - can be NULL, empty, or a string
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
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(1, 10000);
    int packet_count_limit = fdp.ConsumeIntegralInRange<int>(0, 100);
    int immediate_mode = fdp.ConsumeBool() ? 1 : 0;
    int promisc_mode = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume packet data for injection
    size_t packet_data_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), static_cast<size_t>(8192)));
    std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_data_size);
    
    // Consume remaining input for error buffer
    std::string error_buffer_data = fdp.ConsumeRemainingBytesAsString();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    // Copy some error buffer data if available
    if (!error_buffer_data.empty()) {
        size_t copy_len = std::min(error_buffer_data.size(), sizeof(errbuf) - 1);
        memcpy(errbuf, error_buffer_data.data(), copy_len);
        errbuf[copy_len] = '\0';
    }
    
    pcap_t *pcap_handle = nullptr;
    
    // Create pcap handle using pcap_create (modern API)
    if (use_null_device) {
        pcap_handle = pcap_create(nullptr, errbuf);
    } else {
        pcap_handle = pcap_create(device_name.c_str(), errbuf);
    }
    
    if (pcap_handle == nullptr) {
        // Try pcap_open_live as fallback (legacy API)
        if (use_null_device) {
            pcap_handle = pcap_open_live(nullptr, snaplen, promisc_mode, timeout_ms, errbuf);
        } else {
            pcap_handle = pcap_open_live(device_name.c_str(), snaplen, promisc_mode, timeout_ms, errbuf);
        }
        
        if (pcap_handle == nullptr) {
            // If both fail, use pcap_open_dead for testing packet injection APIs
            pcap_handle = pcap_open_dead(DLT_EN10MB, snaplen);
            if (pcap_handle == nullptr) {
                return 0; // Couldn't create any handle
            }
        }
    } else {
        // pcap_create succeeded, need to activate
        // Set some options before activation
        pcap_set_snaplen(pcap_handle, snaplen);
        pcap_set_buffer_size(pcap_handle, buffer_size);
        pcap_set_promisc(pcap_handle, promisc_mode);
        pcap_set_timeout(pcap_handle, timeout_ms);
        pcap_set_immediate_mode(pcap_handle, immediate_mode);
        
        // Activate the handle
        int activate_result = pcap_activate(pcap_handle);
        // Activation may fail, but we can still test some APIs
        (void)activate_result; // Suppress unused variable warning
    }
    
    // Now test different scenarios based on input
    switch (scenario) {
        case 0: {
            // Scenario 0: Test packet injection with pcap_inject
            if (!packet_data.empty()) {
                int inject_result = pcap_inject(pcap_handle, packet_data.data(), packet_data.size());
                // Result can be -1 (error), 0, or positive (bytes sent)
                (void)inject_result;
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test packet sending with pcap_sendpacket
            if (!packet_data.empty()) {
                int send_result = pcap_sendpacket(pcap_handle, packet_data.data(), packet_data.size());
                // Returns -1 on error, 0 on success
                (void)send_result;
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test pcap_loop with packet callback
            int user_data = 0;
            int loop_result = pcap_loop(pcap_handle, packet_count_limit, packet_handler, (u_char *)&user_data);
            // Returns -1 on error, -2 if pcap_breakloop was called, or number of packets processed
            (void)loop_result;
            break;
        }
        
        case 3: {
            // Scenario 3: Test pcap_next and pcap_next_ex
            struct pcap_pkthdr header;
            const u_char *packet = nullptr;
            
            // Try pcap_next (old API)
            packet = pcap_next(pcap_handle, &header);
            // packet may be NULL if no packet available
            
            // Try pcap_next_ex (new API)
            struct pcap_pkthdr *header_ptr = nullptr;
            const u_char **packet_ptr = &packet;
            int next_ex_result = pcap_next_ex(pcap_handle, &header_ptr, packet_ptr);
            // Returns 1 if packet read, 0 if timeout, -1 error, -2 if EOF
            
            // Try reading multiple packets if limit > 0
            for (int i = 0; i < packet_count_limit && packet_count_limit > 0; i++) {
                next_ex_result = pcap_next_ex(pcap_handle, &header_ptr, packet_ptr);
                if (next_ex_result <= 0) {
                    break;
                }
            }
            break;
        }
    }
    
    // Test pcap_breakloop (can be called even if not in a loop)
    pcap_breakloop(pcap_handle);
    
    // Get some statistics if handle is activated
    struct pcap_stat stats;
    memset(&stats, 0, sizeof(stats));
    int stats_result = pcap_stats(pcap_handle, &stats);
    (void)stats_result;
    
    // Get datalink type
    int datalink = pcap_datalink(pcap_handle);
    (void)datalink;
    
    // Get error string
    const char *error_str = pcap_geterr(pcap_handle);
    (void)error_str;
    
    // Clean up
    pcap_close(pcap_handle);
    
    return 0;
}

} // namespace harness_006
