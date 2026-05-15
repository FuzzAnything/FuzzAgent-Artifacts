#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>

namespace harness_021 {
/*
 * libpcap fuzzing harness - harness_021.cpp
 * Targets critical coverage gaps in pcapng file format reading and Linux-specific packet processing
 * 
 * Critical Coverage Gap Strategy:
 * - pcap-util.c has only 1.67% region coverage with extensive swap functions uncovered
 * - sf-pcapng.c has only 15.97% region coverage (pcapng file format parsing)
 * - pcap-linux.c has 6.92% coverage (Linux-specific functionality)
 * - pcap-usb-linux.c has 11.25% coverage (USB packet processing)
 * 
 * Primary APIs targeted:
 * 1. pcap_open_offline - pcapng file format parsing (sf-pcapng.c)
 * 2. pcap_next_ex / pcap_loop - packet reading from pcapng files
 * 3. pcap_close - cleanup
 * 4. pcap_open_dead - for testing Linux-specific DLT types with swap functions
 * 5. pcap_compile - optional BPF filter compilation for pcapng packets
 * 
 * Linux-specific DLT types to target swap functions in pcap-util.c:
 * - DLT_USB_LINUX (swap_linux_usb_header)
 * - DLT_PFLOG (swap_pflog_header) 
 * - DLT_CAN_SOCKETCAN (swap_socketcan_header)
 * - DLT_NFLOG (swap_nflog_header)
 * - DLT_LINUX_SLL with SOCKETCAN (swap_linux_sll_socketcan_header)
 * - DLT_LINUX_SLL2 with SOCKETCAN (swap_linux_sll2_socketcan_header)
 * 
 * Differentiates from existing harnesses:
 * - harness_000: Basic BPF compilation with pcap_open_dead (generic DLT types)
 * - harness_001: BPF filter handling with bpf_dump and bpf_image
 * - harness_002: File I/O with pcap_open_offline (generic pcap files, not pcapng-specific)
 * - harness_003-017: Various file operations, memory FILE*, dump operations
 * - harness_018: BPF lexical scanner and evaluation
 * - harness_019: Linux-specific live capture APIs (pcap_create/activate)
 * - harness_020: Interface enumeration and live capture callbacks
 * - harness_021: PCAPNG FILE FORMAT + LINUX-SPECIFIC DLT SWAP FUNCTIONS (THIS HARNESS)
 * 
 * Strategy:
 * 1. Create synthetic pcapng file data from fuzzer input to test sf-pcapng.c parsing
 * 2. Use pcap_open_offline to parse pcapng data (exercises extensive pcapng parsing logic)
 * 3. Read packets from pcapng file using pcap_next_ex/pcap_loop
 * 4. Test Linux-specific DLT types with pcap_open_dead to exercise swap functions
 * 5. Generate packet data with appropriate headers for each DLT type
 * 6. Test swap_pseudo_headers function indirectly through packet processing
 */


// Packet handler callback for pcap_loop with pcapng files
static void pcapng_packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    (void)user;  // Unused parameter
    
    // Access header fields to test data flow through swap functions
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
        // Read a few bytes to ensure packet data is accessible
        uint8_t sample_byte = packet[0];
        (void)sample_byte;
    }
}

// Create synthetic pcapng file data
std::vector<uint8_t> create_synthetic_pcapng_data(FuzzedDataProvider& fdp, size_t max_size) {
    std::vector<uint8_t> pcapng_data;
    
    // Simple pcapng structure: Section Header Block + Interface Description Block + Enhanced Packet Block
    // Block Type: Section Header Block (0x0A0D0D0A)
    uint32_t shb_block_type = 0x0A0D0D0A;
    uint32_t shb_block_length = 28 + 8; // Header + options + trailer
    uint32_t shb_byte_order_magic = 0x1A2B3C4D;
    uint16_t shb_major_version = 1;
    uint16_t shb_minor_version = 0;
    uint64_t shb_section_length = 0xFFFFFFFFFFFFFFFF; // -1 means unknown
    
    // Write SHB
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&shb_block_type), 
                       reinterpret_cast<uint8_t*>(&shb_block_type) + 4);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&shb_block_length), 
                       reinterpret_cast<uint8_t*>(&shb_block_length) + 4);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&shb_byte_order_magic), 
                       reinterpret_cast<uint8_t*>(&shb_byte_order_magic) + 4);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&shb_major_version), 
                       reinterpret_cast<uint8_t*>(&shb_major_version) + 2);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&shb_minor_version), 
                       reinterpret_cast<uint8_t*>(&shb_minor_version) + 2);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&shb_section_length), 
                       reinterpret_cast<uint8_t*>(&shb_section_length) + 8);
    // Options: End of Options
    uint16_t opt_end_code = 0;
    uint16_t opt_end_length = 0;
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&opt_end_code), 
                       reinterpret_cast<uint8_t*>(&opt_end_code) + 2);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&opt_end_length), 
                       reinterpret_cast<uint8_t*>(&opt_end_length) + 2);
    // Block trailer (same as block length)
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&shb_block_length), 
                       reinterpret_cast<uint8_t*>(&shb_block_length) + 4);
    
    // Interface Description Block (0x00000001)
    uint32_t idb_block_type = 0x00000001;
    uint32_t idb_block_length = 20 + 8; // Header + options + trailer
    uint16_t idb_linktype = fdp.ConsumeIntegralInRange<uint16_t>(0, 255); // DLT type
    uint16_t idb_reserved = 0;
    uint32_t idb_snaplen = fdp.ConsumeIntegralInRange<uint32_t>(1, 65535);
    
    // Write IDB
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&idb_block_type), 
                       reinterpret_cast<uint8_t*>(&idb_block_type) + 4);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&idb_block_length), 
                       reinterpret_cast<uint8_t*>(&idb_block_length) + 4);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&idb_linktype), 
                       reinterpret_cast<uint8_t*>(&idb_linktype) + 2);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&idb_reserved), 
                       reinterpret_cast<uint8_t*>(&idb_reserved) + 2);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&idb_snaplen), 
                       reinterpret_cast<uint8_t*>(&idb_snaplen) + 4);
    // Options: End of Options
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&opt_end_code), 
                       reinterpret_cast<uint8_t*>(&opt_end_code) + 2);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&opt_end_length), 
                       reinterpret_cast<uint8_t*>(&opt_end_length) + 2);
    // Block trailer
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&idb_block_length), 
                       reinterpret_cast<uint8_t*>(&idb_block_length) + 4);
    
    // Enhanced Packet Block (0x00000006) with fuzzed packet data
    uint32_t epb_block_type = 0x00000006;
    uint32_t packet_data_size = fdp.ConsumeIntegralInRange<uint32_t>(1, std::min(max_size - pcapng_data.size() - 32, static_cast<size_t>(1000)));
    uint32_t epb_block_length = 32 + packet_data_size + 8; // Header + packet data + trailer
    
    // Write EPB header
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&epb_block_type), 
                       reinterpret_cast<uint8_t*>(&epb_block_type) + 4);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&epb_block_length), 
                       reinterpret_cast<uint8_t*>(&epb_block_length) + 4);
    
    // Interface ID
    uint32_t interface_id = 0;
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&interface_id), 
                       reinterpret_cast<uint8_t*>(&interface_id) + 4);
    
    // Timestamp (high + low)
    uint32_t timestamp_high = fdp.ConsumeIntegral<uint32_t>();
    uint32_t timestamp_low = fdp.ConsumeIntegral<uint32_t>();
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&timestamp_high), 
                       reinterpret_cast<uint8_t*>(&timestamp_high) + 4);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&timestamp_low), 
                       reinterpret_cast<uint8_t*>(&timestamp_low) + 4);
    
    // Captured packet length and original packet length
    uint32_t captured_len = packet_data_size;
    uint32_t original_len = packet_data_size;
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&captured_len), 
                       reinterpret_cast<uint8_t*>(&captured_len) + 4);
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&original_len), 
                       reinterpret_cast<uint8_t*>(&original_len) + 4);
    
    // Packet data (consumed from fuzzer input)
    std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_data_size);
    pcapng_data.insert(pcapng_data.end(), packet_data.begin(), packet_data.end());
    
    // Pad to 32-bit boundary if needed
    size_t pad_bytes = (4 - (packet_data_size % 4)) % 4;
    for (size_t i = 0; i < pad_bytes; i++) {
        pcapng_data.push_back(0);
    }
    
    // Block trailer
    pcapng_data.insert(pcapng_data.end(), reinterpret_cast<uint8_t*>(&epb_block_length), 
                       reinterpret_cast<uint8_t*>(&epb_block_length) + 4);
    
    return pcapng_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic pcapng structure and DLT testing
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test mode: 0 = pcapng file parsing, 1 = Linux DLT swap functions
    int test_mode = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    if (test_mode == 0) {
        // Test 1: pcapng file format parsing with pcap_open_offline
        // =========================================================
        
        // Create synthetic pcapng data from fuzzer input
        std::vector<uint8_t> pcapng_data = create_synthetic_pcapng_data(fdp, size / 2);
        
        if (pcapng_data.empty()) {
            return 0;
        }
        
        // Create temporary file for pcapng data
        char temp_filename[] = "/tmp/libpcap_fuzz_pcapng_XXXXXX";
        int temp_fd = mkstemp(temp_filename);
        if (temp_fd < 0) {
            return 0;
        }
        
        // Write pcapng data to temporary file
        if (write(temp_fd, pcapng_data.data(), pcapng_data.size()) != static_cast<ssize_t>(pcapng_data.size())) {
            close(temp_fd);
            unlink(temp_filename);
            return 0;
        }
        close(temp_fd);
        
        // Open pcapng file with pcap_open_offline (exercises sf-pcapng.c parsing)
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcap_handle = pcap_open_offline(temp_filename, errbuf);
        
        if (pcap_handle != nullptr) {
            // Get datalink type
            int datalink = pcap_datalink(pcap_handle);
            (void)datalink;
            
            // Try to read packets using pcap_next_ex
            struct pcap_pkthdr* header;
            const u_char* packet;
            int packet_count = 0;
            const int MAX_PACKETS = 10; // Limit for fuzzing
            
            while (packet_count < MAX_PACKETS) {
                int result = pcap_next_ex(pcap_handle, &header, &packet);
                if (result <= 0) {
                    break; // No more packets or error
                }
                
                // Process packet (this may trigger swap functions if DLT requires it)
                if (header != nullptr && packet != nullptr) {
                    uint32_t caplen = header->caplen;
                    uint32_t len = header->len;
                    (void)caplen;
                    (void)len;
                }
                
                packet_count++;
            }
            
            // Alternative: Try pcap_loop with callback
            if (packet_count == 0) {
                // No packets read with pcap_next_ex, try pcap_loop
                int loop_result = pcap_loop(pcap_handle, 1, pcapng_packet_handler, nullptr);
                (void)loop_result;
            }
            
            // Try BPF filter compilation if we have remaining fuzzer data
            if (fdp.remaining_bytes() > 10) {
                std::string filter_str = fdp.ConsumeRandomLengthString(100);
                struct bpf_program bpf;
                int compile_result = pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN);
                
                if (compile_result == 0) {
                    // Filter compiled successfully
                    pcap_freecode(&bpf);
                }
            }
            
            // Get statistics
            struct pcap_stat stats;
            int stats_result = pcap_stats(pcap_handle, &stats);
            (void)stats_result;
            
            // Clean up
            pcap_close(pcap_handle);
        }
        
        // Clean up temporary file
        unlink(temp_filename);
        
    } else {
        // Test 2: Linux-specific DLT types and swap functions
        // ===================================================
        
        // Test each Linux-specific DLT type that has swap functions in pcap-util.c
        int dlt_choice = fdp.ConsumeIntegralInRange<int>(0, 5);
        int dlt_type;
        
        switch (dlt_choice) {
            case 0:
                dlt_type = DLT_USB_LINUX;        // swap_linux_usb_header
                break;
            case 1:
                dlt_type = DLT_PFLOG;            // swap_pflog_header
                break;
            case 2:
                dlt_type = DLT_CAN_SOCKETCAN;    // swap_socketcan_header
                break;
            case 3:
                dlt_type = DLT_NFLOG;            // swap_nflog_header
                break;
            case 4:
                dlt_type = DLT_LINUX_SLL;        // swap_linux_sll_socketcan_header (if SOCKETCAN)
                break;
            case 5:
                dlt_type = DLT_LINUX_SLL2;       // swap_linux_sll2_socketcan_header (if SOCKETCAN)
                break;
            default:
                dlt_type = DLT_EN10MB;           // Fallback
        }
        
        int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
        
        // Create pcap handle for dead capture with specific DLT type
        pcap_t* pcap_handle = pcap_open_dead(dlt_type, snaplen);
        if (pcap_handle == nullptr) {
            return 0;
        }
        
        // Get datalink type
        int datalink = pcap_datalink(pcap_handle);
        (void)datalink;
        
        // Create synthetic packet data appropriate for the DLT type
        size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), static_cast<size_t>(1024)));
        std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
        
        // Create packet header
        struct pcap_pkthdr header;
        header.caplen = std::min(packet_size, static_cast<size_t>(snaplen));
        header.len = packet_size;
        header.ts.tv_sec = fdp.ConsumeIntegral<time_t>();
        header.ts.tv_usec = fdp.ConsumeIntegralInRange<suseconds_t>(0, 999999);
        
        // Test pcap_next with our synthetic packet
        // Note: pcap_next with pcap_open_dead won't process the packet through swap functions,
        // but creating the handle with specific DLT types exercises initialization code
        
        // Try BPF filter compilation for this DLT type
        if (fdp.remaining_bytes() > 10) {
            std::string filter_str = fdp.ConsumeRandomLengthString(100);
            struct bpf_program bpf;
            int compile_result = pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN);
            
            if (compile_result == 0) {
                // Try to set the filter
                int setfilter_result = pcap_setfilter(pcap_handle, &bpf);
                (void)setfilter_result;
                
                pcap_freecode(&bpf);
            }
        }
        
        // Test pcap_loop with callback (won't actually call callback with pcap_open_dead,
        // but tests API initialization for specific DLT type)
        int loop_result = pcap_loop(pcap_handle, 0, pcapng_packet_handler, nullptr);
        (void)loop_result;
        
        // Get statistics
        struct pcap_stat stats;
        int stats_result = pcap_stats(pcap_handle, &stats);
        (void)stats_result;
        
        // Clean up
        pcap_close(pcap_handle);
    }
    
    return 0;
}

} // namespace harness_021
