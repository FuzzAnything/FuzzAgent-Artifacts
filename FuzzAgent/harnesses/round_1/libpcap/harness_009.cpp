#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <climits>

namespace harness_009 {
/*
 * libpcap fuzzing harness - harness_009.cpp
 * Targets packet transmission subsystem APIs with comprehensive sending tests
 * 
 * Primary APIs targeted:
 * - pcap_sendpacket (send single packet - WinPcap compatible)
 * - pcap_inject (send single packet - OpenBSD compatible)
 * - pcap_sendqueue_alloc (Windows: allocate send queue)
 * - pcap_sendqueue_destroy (Windows: free send queue)
 * - pcap_sendqueue_queue (Windows: add packet to queue)
 * - pcap_sendqueue_transmit (Windows: transmit queued packets)
 * - pcap_setdirection (set direction for transmission testing)
 * - pcap_setnonblock (set non-blocking mode for transmission)
 * 
 * Differentiates from existing harnesses:
 * - harness_000: Core packet capture APIs (uses pcap_open_dead for filtering)
 * - harness_001: BPF filter handling functions  
 * - harness_002: File I/O subsystem operations
 * - harness_003: Live device management lifecycle
 * - harness_004: Savefile-specific APIs with timestamp precision
 * - harness_005: Timestamp handling subsystem APIs
 * - harness_006: Network address translation subsystem
 * - harness_007: Datalink configuration subsystem
 * - harness_008: Ethernet address database parsing
 * - harness_009: Packet transmission subsystem (THIS HARNESS)
 * 
 * Specifically targets the 26+ undiscovered branch points in packet transmission
 * subsystem identified by CoverageAnalyzerAgent. Focuses on comprehensive testing
 * of packet sending APIs with various packet sizes, error conditions, and
 * transmission modes.
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
    int nonblock_mode = fdp.ConsumeBool() ? 1 : 0;
    int direction_val = fdp.ConsumeIntegralInRange<int>(0, 2);
    pcap_direction_t direction;
    
    // Map direction value to enum
    switch (direction_val) {
        case 0: direction = PCAP_D_IN; break;
        case 1: direction = PCAP_D_OUT; break;
        case 2: direction = PCAP_D_INOUT; break;
        default: direction = PCAP_D_INOUT; break;
    }
    
    // Consume device name string
    std::string device_name = fdp.ConsumeRandomLengthString(128);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Test 1: Basic transmission APIs with pcap_open_dead (will fail but test error paths)
    pcap_t* dead_handle = pcap_open_dead(linktype, snaplen);
    if (dead_handle != nullptr) {
        // Set non-blocking mode (will fail on dead handle but test API)
        char nonblock_err[PCAP_ERRBUF_SIZE];
        int set_nonblock_result = pcap_setnonblock(dead_handle, nonblock_mode, nonblock_err);
        (void)set_nonblock_result;
        
        // Set direction (will fail on dead handle but test API)
        int setdir_result = pcap_setdirection(dead_handle, direction);
        (void)setdir_result;
        
        // Test pcap_sendpacket with various packet sizes
        // Consume packet data for transmission tests
        size_t max_packet_size = fdp.remaining_bytes() < 4096 ? fdp.remaining_bytes() : 4096;
        size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(0, max_packet_size);
        std::vector<uint8_t> packet_data;
        
        if (packet_size > 0 && fdp.remaining_bytes() >= packet_size) {
            packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
            
            // Test with valid packet data
            if (!packet_data.empty()) {
                int send_result = pcap_sendpacket(dead_handle, packet_data.data(), 
                                                  static_cast<int>(packet_data.size()));
                (void)send_result;  // Expected to fail (-1) on dead handle
                
                // Test pcap_inject (OpenBSD style)
                int inject_result = pcap_inject(dead_handle, packet_data.data(), 
                                                packet_data.size());
                (void)inject_result;  // Expected to fail (-1) on dead handle
            }
            
            // Test edge cases for pcap_sendpacket
            // Test with size = 0 (should fail with error)
            int send_zero_result = pcap_sendpacket(dead_handle, packet_data.data(), 0);
            (void)send_zero_result;
            
            // Test with size = -1 (should fail with error)
            int send_neg_result = pcap_sendpacket(dead_handle, packet_data.data(), -1);
            (void)send_neg_result;
        }
        
        // Test pcap_inject edge cases
        if (!packet_data.empty()) {
            // Test with size = 0 (should fail with error)
            int inject_zero_result = pcap_inject(dead_handle, packet_data.data(), 0);
            (void)inject_zero_result;
            
            // Test with size > INT_MAX (should fail with error)
            // Note: size_t to size_t conversion, but pcap_inject checks if size > INT_MAX
            size_t huge_size = static_cast<size_t>(INT_MAX) + 1;
            int inject_huge_result = pcap_inject(dead_handle, packet_data.data(), huge_size);
            (void)inject_huge_result;
        }
        
        // Clean up
        pcap_close(dead_handle);
    }
    
    // Test 2: Try with pcap_create + pcap_activate for potential live handle
    // (may succeed if running with appropriate permissions)
    pcap_t* modern_handle = pcap_create(
        device_name.empty() ? "any" : device_name.c_str(),
        errbuf
    );
    
    if (modern_handle != nullptr) {
        // Set various parameters before activation
        pcap_set_snaplen(modern_handle, snaplen);
        pcap_set_promisc(modern_handle, 1);  // Try promiscuous mode
        pcap_set_timeout(modern_handle, 100);  // 100ms timeout
        
        // Try to activate the handle
        int activate_result = pcap_activate(modern_handle);
        
        if (activate_result >= 0 || activate_result == PCAP_WARNING) {
            // Handle activated (or warning), test transmission APIs
            // Set non-blocking mode
            char nb_err[PCAP_ERRBUF_SIZE];
            pcap_setnonblock(modern_handle, nonblock_mode, nb_err);
            
            // Set direction
            pcap_setdirection(modern_handle, direction);
            
            // Test transmission if we have packet data
            if (fdp.remaining_bytes() > 0) {
                size_t tx_packet_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                    fdp.remaining_bytes() < 1500 ? fdp.remaining_bytes() : 1500);
                std::vector<uint8_t> tx_packet = fdp.ConsumeBytes<uint8_t>(tx_packet_size);
                
                if (!tx_packet.empty()) {
                    // Test pcap_sendpacket
                    int send_result = pcap_sendpacket(modern_handle, tx_packet.data(), 
                                                      static_cast<int>(tx_packet.size()));
                    (void)send_result;
                    
                    // Test pcap_inject
                    int inject_result = pcap_inject(modern_handle, tx_packet.data(), 
                                                    tx_packet.size());
                    (void)inject_result;
                }
            }
        }
        
        // Clean up
        pcap_close(modern_handle);
    }
    
    // Test 3: Windows-specific send queue APIs (compile-time conditional)
#ifdef _WIN32
    if (fdp.remaining_bytes() >= 16) {
        // Allocate send queue
        u_int queue_size = fdp.ConsumeIntegralInRange<u_int>(1024, 65536);
        pcap_send_queue* send_queue = pcap_sendqueue_alloc(queue_size);
        
        if (send_queue != nullptr) {
            // Try to queue some packets
            int packets_to_queue = fdp.ConsumeIntegralInRange<int>(0, 10);
            
            for (int i = 0; i < packets_to_queue && fdp.remaining_bytes() >= 32; i++) {
                // Create packet header
                struct pcap_pkthdr pkt_header;
                pkt_header.ts.tv_sec = fdp.ConsumeIntegral<time_t>();
                pkt_header.ts.tv_usec = fdp.ConsumeIntegral<suseconds_t>();
                pkt_header.caplen = fdp.ConsumeIntegralInRange<u_int>(0, 1514);
                pkt_header.len = pkt_header.caplen;
                
                // Get packet data if available
                if (pkt_header.caplen > 0 && fdp.remaining_bytes() >= pkt_header.caplen) {
                    std::vector<uint8_t> pkt_data = fdp.ConsumeBytes<uint8_t>(pkt_header.caplen);
                    
                    // Queue the packet
                    int queue_result = pcap_sendqueue_queue(send_queue, &pkt_header, 
                                                           pkt_data.data());
                    (void)queue_result;
                }
            }
            
            // Try to transmit the queue if we have a handle
            pcap_t* win_handle = pcap_open_dead(linktype, snaplen);
            if (win_handle != nullptr) {
                int sync_mode = fdp.ConsumeBool() ? 1 : 0;
                u_int transmit_result = pcap_sendqueue_transmit(win_handle, send_queue, 
                                                               sync_mode);
                (void)transmit_result;
                pcap_close(win_handle);
            }
            
            // Clean up send queue
            pcap_sendqueue_destroy(send_queue);
        }
    }
#endif // _WIN32
    
    // Test 4: Additional transmission tests with different packet types
    if (fdp.remaining_bytes() > 0) {
        // Create another dead handle for more transmission tests
        pcap_t* tx_handle = pcap_open_dead(linktype, snaplen);
        if (tx_handle != nullptr) {
            // Test with various packet sizes to exercise different code paths
            const size_t test_sizes[] = {0, 1, 60, 128, 256, 512, 1024, 1500, 65535};
            
            for (size_t test_size : test_sizes) {
                if (fdp.remaining_bytes() >= test_size) {
                    std::vector<uint8_t> test_packet = fdp.ConsumeBytes<uint8_t>(test_size);
                    
                    if (!test_packet.empty() && test_size > 0) {
                        // Test both APIs
                        pcap_sendpacket(tx_handle, test_packet.data(), 
                                       static_cast<int>(test_size));
                        pcap_inject(tx_handle, test_packet.data(), test_size);
                    } else if (test_size == 0) {
                        // Explicitly test size=0 edge case
                        uint8_t dummy = 0;
                        pcap_sendpacket(tx_handle, &dummy, 0);
                        pcap_inject(tx_handle, &dummy, 0);
                    }
                }
            }
            
            pcap_close(tx_handle);
        }
    }
    
    return 0;
}

} // namespace harness_009
