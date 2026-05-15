#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <functional>

namespace harness_007 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful packet I/O testing
    // Need enough data for configuration parameters + packet data
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration parameters first
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which test scenario to run
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);  // Snapshot length
    int promisc_mode = fdp.ConsumeBool() ? 1 : 0;  // Promiscuous mode
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);  // Timeout in ms
    int immediate_mode = fdp.ConsumeBool() ? 1 : 0;  // Immediate mode
    int direction = fdp.ConsumeIntegralInRange<int>(0, 2);  // Capture direction
    int loop_count = fdp.ConsumeIntegralInRange<int>(0, 10);  // For pcap_loop
    int dispatch_count = fdp.ConsumeIntegralInRange<int>(0, 10);  // For pcap_dispatch
    
    // Consume device name string - use "any" for Linux dummy interface
    size_t dev_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string dev_name = fdp.ConsumeBytesAsString(dev_name_size);
    
    // Use "any" as a dummy interface if the generated name is invalid
    if (dev_name.empty() || dev_name.find('/') != std::string::npos) {
        dev_name = "any";
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = nullptr;
    
    // Step 1: pcap_create for handle creation
    pcap = pcap_create(dev_name.c_str(), errbuf);
    if (pcap == nullptr) {
        // If pcap_create fails, we can still test other APIs with pcap_open_dead
        pcap = pcap_open_dead(DLT_EN10MB, snaplen);
        if (pcap == nullptr) {
            return 0;
        }
        
        // With pcap_open_dead, we can only test pcap_inject/pcap_sendpacket
        // since it's not an active capture handle
        if (fdp.remaining_bytes() > 16) {
            size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(1, 1500);
            std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
            
            // Test pcap_inject (returns bytes written or -1 on error)
            int inject_result = pcap_inject(pcap, packet_data.data(), packet_data.size());
            (void)inject_result;
            
            // Test pcap_sendpacket (returns 0 on success, -1 on error)
            int send_result = pcap_sendpacket(pcap, packet_data.data(), packet_data.size());
            (void)send_result;
        }
        
        pcap_close(pcap);
        return 0;
    }
    
    // Step 2: Configuration functions before activation
    // These should be called before pcap_activate
    pcap_set_snaplen(pcap, snaplen);
    pcap_set_promisc(pcap, promisc_mode);
    pcap_set_timeout(pcap, timeout_ms);
    pcap_set_immediate_mode(pcap, immediate_mode);
    
    // Set capture direction (cast to pcap_direction_t enum)
    pcap_direction_t dir_enum = static_cast<pcap_direction_t>(direction % 3);
    pcap_setdirection(pcap, dir_enum);
    
    // Step 3: pcap_activate for activation
    int activate_result = pcap_activate(pcap);
    
    if (activate_result >= 0) {
        // Activation succeeded (or returned warning)
        
        // Determine which packet I/O APIs to test based on test_type
        switch (test_type) {
            case 0: {
                // Test 1: Packet transmission with pcap_inject and pcap_sendpacket
                if (fdp.remaining_bytes() > 32) {
                    // Generate packet data for transmission
                    size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(14, 1500);  // Min Ethernet frame size
                    std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
                    
                    // Test pcap_inject - returns number of bytes written or -1
                    int inject_result = pcap_inject(pcap, packet_data.data(), packet_data.size());
                    (void)inject_result;
                    
                    // Test pcap_sendpacket - returns 0 on success, -1 on error
                    int send_result = pcap_sendpacket(pcap, packet_data.data(), packet_data.size());
                    (void)send_result;
                    
                    // Test edge cases
                    // Zero-length packet (should fail)
                    if (packet_data.size() > 0) {
                        pcap_inject(pcap, packet_data.data(), 0);
                        pcap_sendpacket(pcap, packet_data.data(), 0);
                    }
                    
                    // Very large packet (may fail)
                    if (fdp.remaining_bytes() > 100) {
                        size_t large_packet_size = fdp.ConsumeIntegralInRange<size_t>(1501, 10000);
                        std::vector<uint8_t> large_packet = fdp.ConsumeBytes<uint8_t>(
                            std::min(large_packet_size, fdp.remaining_bytes()));
                        pcap_inject(pcap, large_packet.data(), large_packet.size());
                        pcap_sendpacket(pcap, large_packet.data(), large_packet.size());
                    }
                }
                break;
            }
                
            case 1: {
                // Test 2: Packet reception with pcap_next and pcap_next_ex
                if (fdp.remaining_bytes() > 16) {
                    // Set non-blocking mode for timeout testing
                    pcap_setnonblock(pcap, immediate_mode, errbuf);
                    
                    // Test pcap_next - returns pointer to packet data or NULL
                    struct pcap_pkthdr header;
                    const u_char* packet = pcap_next(pcap, &header);
                    (void)packet;
                    
                    // Test pcap_next_ex - returns 1 if packet read, 0 if timeout, -1/2 on error
                    struct pcap_pkthdr* header_ptr = nullptr;
                    const u_char* packet_ptr = nullptr;
                    int next_ex_result = pcap_next_ex(pcap, &header_ptr, &packet_ptr);
                    (void)next_ex_result;
                    
                    // Test multiple reads in a loop (with small limit)
                    for (int i = 0; i < 5 && fdp.remaining_bytes() > 0; i++) {
                        next_ex_result = pcap_next_ex(pcap, &header_ptr, &packet_ptr);
                        if (next_ex_result <= 0) {
                            break;
                        }
                    }
                }
                break;
            }
                
            case 2: {
                // Test 3: Batch packet processing with pcap_dispatch and pcap_loop
                if (fdp.remaining_bytes() > 16) {
                    // Create packet callback handlers
                    auto packet_handler_dispatch = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                        int* count = reinterpret_cast<int*>(user);
                        (*count)++;
                    };
                    
                    auto packet_handler_loop = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                        int* count = reinterpret_cast<int*>(user);
                        (*count)++;
                        
                        // Simulate packet processing
                        if (h != nullptr && bytes != nullptr) {
                            // Access packet data to exercise code paths
                            size_t len = h->caplen;
                            (void)len;
                        }
                    };
                    
                    int dispatch_packet_count = 0;
                    int loop_packet_count = 0;
                    
                    // Test pcap_dispatch - processes up to dispatch_count packets
                    if (dispatch_count > 0) {
                        int dispatch_result = pcap_dispatch(pcap, dispatch_count, 
                                                           packet_handler_dispatch, 
                                                           reinterpret_cast<u_char*>(&dispatch_packet_count));
                        (void)dispatch_result;
                    }
                    
                    // Test pcap_loop - processes exactly loop_count packets or until error
                    if (loop_count > 0) {
                        // Set a breakloop condition based on fuzzer input
                        bool should_break = fdp.ConsumeBool();
                        if (should_break && loop_count > 1) {
                            // Test pcap_breakloop - interrupts pcap_loop
                            pcap_breakloop(pcap);
                        }
                        
                        int loop_result = pcap_loop(pcap, loop_count, 
                                                   packet_handler_loop, 
                                                   reinterpret_cast<u_char*>(&loop_packet_count));
                        (void)loop_result;
                    }
                }
                break;
            }
                
            case 3: {
                // Test 4: Combined transmission and reception
                if (fdp.remaining_bytes() > 48) {
                    // First transmit some packets
                    size_t num_packets = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                    for (size_t i = 0; i < num_packets && fdp.remaining_bytes() > 14; i++) {
                        size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(14, 512);
                        std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
                        
                        // Transmit packet
                        pcap_inject(pcap, packet_data.data(), packet_data.size());
                        
                        // Try to receive it (may not work on all interfaces)
                        struct pcap_pkthdr* header_ptr = nullptr;
                        const u_char* packet_ptr = nullptr;
                        pcap_next_ex(pcap, &header_ptr, &packet_ptr);
                    }
                    
                    // Then test batch reception
                    auto combined_handler = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                        int* stats = reinterpret_cast<int*>(user);
                        stats[0]++;  // Packet count
                        if (h != nullptr) {
                            stats[1] += h->caplen;  // Total bytes
                        }
                    };
                    
                    int stats[2] = {0, 0};
                    pcap_dispatch(pcap, 5, combined_handler, reinterpret_cast<u_char*>(stats));
                    
                    // Test pcap_stats to get capture statistics
                    struct pcap_stat stats_struct;
                    pcap_stats(pcap, &stats_struct);
                }
                break;
            }
        }
        
        // Additional common tests for active handle
        // Test pcap_getnonblock
        int nonblock = 0;
        pcap_getnonblock(pcap, errbuf);
        
        // Test pcap_datalink to get data link type
        int datalink = pcap_datalink(pcap);
        (void)datalink;
        
        // Test pcap_snapshot
        int snapshot = pcap_snapshot(pcap);
        (void)snapshot;
        
        // Test pcap_major_version and pcap_minor_version
        int major = pcap_major_version(pcap);
        int minor = pcap_minor_version(pcap);
        (void)major;
        (void)minor;
        
    } else {
        // Activation failed - test error handling paths
        // Get error string
        const char* err_str = pcap_geterr(pcap);
        (void)err_str;
        
        // Test pcap_perror
        pcap_perror(pcap, "Activation failed");
        
        // Even though activation failed, we can still test pcap_inject
        // which should return PCAP_ERROR_NOT_ACTIVATED
        if (fdp.remaining_bytes() > 16) {
            size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
            
            int inject_result = pcap_inject(pcap, packet_data.data(), packet_data.size());
            (void)inject_result;
            
            int send_result = pcap_sendpacket(pcap, packet_data.data(), packet_data.size());
            (void)send_result;
        }
    }
    
    // Step 6: pcap_close for cleanup
    pcap_close(pcap);
    
    // Test additional APIs with remaining input
    if (fdp.remaining_bytes() > 8) {
        // Test pcap_lookupnet with the device name
        bpf_u_int32 net = 0, mask = 0;
        pcap_lookupnet(dev_name.c_str(), &net, &mask, errbuf);
        
        // Test pcap_strerror with various error codes
        for (int err_code = -5; err_code <= 5; err_code++) {
            const char* err_msg = pcap_strerror(err_code);
            (void)err_msg;
        }
    }
    
    return 0;
}

} // namespace harness_007
