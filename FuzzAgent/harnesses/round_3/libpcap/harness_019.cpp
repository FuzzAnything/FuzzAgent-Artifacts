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
#include <stdio.h>

namespace harness_019 {
/*
 * Harness for comprehensive pcapng file format parsing and manipulation
 * Primary Target: sf-pcapng.c module (15.57% line coverage, 301/320 branches undiscovered)
 * Targets: pcap_open_offline, pcap_fopen_offline, pcap_open_offline_with_tstamp_precision,
 *          pcap_next_ex, pcap_loop, pcap_compile, pcap_setfilter, pcap_close,
 *          pcap_dump_open, pcap_dump, pcap_dump_close, pcap_freecode
 * Coverage goal: Exercise critically under-covered pcap_ng_next_packet function and 
 *                complete pcapng parsing code paths in sf-pcapng.c
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful pcapng testing with filter compilation
    // Need enough data for pcapng structure + filter expressions + control parameters
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Consume control parameters for test configuration
    int opening_method = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which pcap_open variant to use
    int test_dump = fdp.ConsumeBool() ? 1 : 0;  // Whether to test dump/write functionality
    int test_filter = fdp.ConsumeBool() ? 1 : 0;  // Whether to test filter compilation
    int filter_optimize = fdp.ConsumeBool() ? 1 : 0;  // Filter optimization flag
    int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=microseconds, 1=nanoseconds
    int loop_method = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=pcap_next_ex, 1=pcap_loop
    
    // Parameters for pcapng generation
    int num_interfaces = fdp.ConsumeIntegralInRange<int>(1, 4);
    int num_packets = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    // Filter expression for compilation testing
    std::vector<std::string> filter_expressions = {
        "ip", "tcp", "udp", "port 80", "port 443", "host 1.2.3.4",
        "net 192.168.0.0/24", "ether src 00:11:22:33:44:55",
        "icmp", "arp", "vlan", "mpls", "length > 100", "less 1500"
    };
    std::string filter_expr;
    if (test_filter && !filter_expressions.empty()) {
        size_t filter_idx = fdp.ConsumeIntegralInRange<size_t>(0, filter_expressions.size() - 1);
        filter_expr = filter_expressions[filter_idx];
        // Also add some random bytes for edge cases
        std::string random_filter_part = fdp.ConsumeRandomLengthString(128);
        if (!random_filter_part.empty()) {
            filter_expr += " " + random_filter_part;
        }
    }
    
    // Phase 2: Generate pcapng data structure
    std::vector<uint8_t> pcapng_data;
    
    // PCAPNG constants from sf-pcapng.c
    const uint32_t BT_SHB = 0x0A0D0D0A;  // Section Header Block
    const uint32_t BT_IDB = 0x00000001;  // Interface Description Block
    const uint32_t BT_EPB = 0x00000006;  // Enhanced Packet Block
    const uint32_t BT_SPB = 0x00000003;  // Simple Packet Block
    const uint32_t BT_PB = 0x00000002;   // Packet Block
    const uint32_t BYTE_ORDER_MAGIC = 0x1A2B3C4D;
    
    // Helper functions for pcapng generation
    auto add_u32 = [&](uint32_t value) {
        uint8_t bytes[4];
        bytes[0] = (value >> 0) & 0xFF;
        bytes[1] = (value >> 8) & 0xFF;
        bytes[2] = (value >> 16) & 0xFF;
        bytes[3] = (value >> 24) & 0xFF;
        pcapng_data.insert(pcapng_data.end(), bytes, bytes + 4);
    };
    
    auto add_u16 = [&](uint16_t value) {
        uint8_t bytes[2];
        bytes[0] = (value >> 0) & 0xFF;
        bytes[1] = (value >> 8) & 0xFF;
        pcapng_data.insert(pcapng_data.end(), bytes, bytes + 2);
    };
    
    auto pad_to_32bit = [&]() {
        while (pcapng_data.size() % 4 != 0) {
            pcapng_data.push_back(0);
        }
    };
    
    auto start_block = [&](uint32_t block_type) {
        add_u32(block_type);
        size_t total_length_pos = pcapng_data.size();
        add_u32(0);  // Temporary total length
        return total_length_pos;
    };
    
    auto end_block = [&](size_t total_length_pos) {
        size_t block_end = pcapng_data.size();
        uint32_t total_length = block_end - total_length_pos + 4;
        
        // Update total_length field
        pcapng_data[total_length_pos] = (total_length >> 0) & 0xFF;
        pcapng_data[total_length_pos + 1] = (total_length >> 8) & 0xFF;
        pcapng_data[total_length_pos + 2] = (total_length >> 16) & 0xFF;
        pcapng_data[total_length_pos + 3] = (total_length >> 24) & 0xFF;
        
        add_u32(total_length);  // Block trailer
    };
    
    // 1. Section Header Block (SHB) - Required for pcapng
    size_t shb_pos = start_block(BT_SHB);
    add_u32(BYTE_ORDER_MAGIC);
    add_u16(1);  // major version
    add_u16(0);  // minor version
    add_u32(0xFFFFFFFF);  // section length (0xFFFFFFFF = unspecified)
    add_u32(0xFFFFFFFF);
    
    // SHB options
    pad_to_32bit();
    if (fdp.ConsumeBool()) {
        // Add comment option
        add_u16(1);  // OPT_COMMENT
        std::string shb_comment = "Fuzzed pcapng section";
        add_u16(shb_comment.size());
        pcapng_data.insert(pcapng_data.end(), shb_comment.begin(), shb_comment.end());
        pad_to_32bit();
    }
    
    // End of SHB options
    add_u16(0);  // OPT_ENDOFOPT
    add_u16(0);
    end_block(shb_pos);
    
    // 2. Interface Description Blocks (IDB)
    for (int i = 0; i < num_interfaces; i++) {
        size_t idb_pos = start_block(BT_IDB);
        
        // Link type (DLT) - consume from fuzzed data
        uint16_t link_type = fdp.ConsumeIntegralInRange<uint16_t>(0, 100);
        add_u16(link_type);
        add_u16(0);  // reserved
        
        // Snap length
        uint32_t snap_len = fdp.ConsumeIntegralInRange<uint32_t>(1, 262144);
        add_u32(snap_len);
        
        // IDB options
        pad_to_32bit();
        
        // Add interface name option
        if (fdp.ConsumeBool()) {
            add_u16(2);  // if_name option code
            std::string ifname = "eth" + std::to_string(i);
            add_u16(ifname.size());
            pcapng_data.insert(pcapng_data.end(), ifname.begin(), ifname.end());
            pad_to_32bit();
        }
        
        // Add timestamp resolution option
        if (fdp.ConsumeBool()) {
            add_u16(9);  // if_tsresol option code
            add_u16(1);  // option length
            uint8_t tsresol = fdp.ConsumeIntegral<uint8_t>() % 10;
            pcapng_data.push_back(tsresol);
            pad_to_32bit();
        }
        
        // End of IDB options
        add_u16(0);  // OPT_ENDOFOPT
        add_u16(0);
        end_block(idb_pos);
    }
    
    // 3. Packet Blocks (EPB/SPB)
    for (int i = 0; i < num_packets; i++) {
        uint32_t block_type;
        if (fdp.ConsumeBool()) {
            block_type = BT_EPB;
        } else {
            block_type = BT_SPB;
        }
        
        size_t packet_pos = start_block(block_type);
        
        if (block_type == BT_EPB) {
            // Enhanced Packet Block
            uint32_t interface_id = fdp.ConsumeIntegralInRange<uint32_t>(0, num_interfaces - 1);
            add_u32(interface_id);
            
            // Timestamp (high and low parts)
            uint32_t ts_high = fdp.ConsumeIntegral<uint32_t>();
            uint32_t ts_low = fdp.ConsumeIntegral<uint32_t>();
            add_u32(ts_high);
            add_u32(ts_low);
            
            // Captured length and original length
            uint32_t caplen = fdp.ConsumeIntegralInRange<uint32_t>(1, 1500);
            uint32_t origlen = fdp.ConsumeIntegralInRange<uint32_t>(caplen, 1500);
            add_u32(caplen);
            add_u32(origlen);
            
            // Packet data
            std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(caplen);
            pcapng_data.insert(pcapng_data.end(), packet_data.begin(), packet_data.end());
            pad_to_32bit();
            
        } else {  // BT_SPB
            // Simple Packet Block
            uint32_t origlen = fdp.ConsumeIntegralInRange<uint32_t>(1, 1500);
            add_u32(origlen);
            
            // Packet data
            std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(origlen);
            pcapng_data.insert(pcapng_data.end(), packet_data.begin(), packet_data.end());
            pad_to_32bit();
        }
        
        // Packet block options (optional)
        pad_to_32bit();
        if (fdp.ConsumeBool()) {
            // Add comment option
            add_u16(1);  // OPT_COMMENT
            std::string comment = "packet_" + std::to_string(i);
            add_u16(comment.size());
            pcapng_data.insert(pcapng_data.end(), comment.begin(), comment.end());
            pad_to_32bit();
        }
        
        // End of options
        add_u16(0);  // OPT_ENDOFOPT
        add_u16(0);
        end_block(packet_pos);
    }
    
    // Create temporary input file with pcapng data
    char temp_filename[] = "/tmp/libpcap_pcapng_fuzz_input_XXXXXX";
    int input_fd = mkstemp(temp_filename);
    if (input_fd < 0) {
        return 0;
    }
    
    if (write(input_fd, pcapng_data.data(), pcapng_data.size()) != static_cast<ssize_t>(pcapng_data.size())) {
        close(input_fd);
        unlink(temp_filename);
        return 0;
    }
    close(input_fd);
    
    // Phase 3: Open pcapng file using specified method
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    
    // PRIMARY TARGET: pcap_open_offline family APIs
    switch (opening_method) {
        case 0:
            // pcap_open_offline
            pcap_handle = pcap_open_offline(temp_filename, errbuf);
            break;
            
        case 1:
            // pcap_open_offline_with_tstamp_precision
            pcap_handle = pcap_open_offline_with_tstamp_precision(temp_filename, tstamp_precision, errbuf);
            break;
            
        case 2: {
            // pcap_fopen_offline
            FILE* f = fopen(temp_filename, "rb");
            if (f == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            pcap_handle = pcap_fopen_offline(f, errbuf);
            break;
        }
            
        case 3: {
            // pcap_fopen_offline_with_tstamp_precision
#ifndef _WIN32
            FILE* f2 = fopen(temp_filename, "rb");
            if (f2 == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            pcap_handle = pcap_fopen_offline_with_tstamp_precision(f2, tstamp_precision, errbuf);
#else
            pcap_handle = pcap_open_offline(temp_filename, errbuf);
#endif
            break;
        }
    }
    
    if (pcap_handle == nullptr) {
        // Opening failed - test error handling
        (void)pcap_strerror(PCAP_ERROR);
        unlink(temp_filename);
        return 0;
    }
    
    // Phase 4: Filter compilation and application (if requested)
    struct bpf_program fcode;
    memset(&fcode, 0, sizeof(fcode));
    int filter_applied = 0;
    
    if (test_filter && !filter_expr.empty()) {
        // PRIMARY TARGET: pcap_compile
        int compile_result = pcap_compile(pcap_handle, &fcode, filter_expr.c_str(), filter_optimize, PCAP_NETMASK_UNKNOWN);
        
        if (compile_result == 0) {
            // Compilation successful
            // PRIMARY TARGET: pcap_setfilter
            int setfilter_result = pcap_setfilter(pcap_handle, &fcode);
            filter_applied = (setfilter_result == 0) ? 1 : 0;
        }
    }
    
    // Phase 5: Packet reading from pcapng file
    int packets_read = 0;
    
    if (loop_method == 0) {
        // PRIMARY TARGET: pcap_next_ex (triggers pcap_ng_next_packet internally)
        struct pcap_pkthdr* header;
        const u_char* pkt_data;
        
        while (packets_read < 50) {  // Limit to avoid infinite loops
            int result = pcap_next_ex(pcap_handle, &header, &pkt_data);
            
            if (result == 1) {
                // Successfully read a packet
                packets_read++;
                
                if (header != nullptr && pkt_data != nullptr) {
                    // Access packet header fields
                    (void)header->ts.tv_sec;
                    (void)header->ts.tv_usec;
                    (void)header->caplen;
                    (void)header->len;
                }
                
            } else if (result == 0) {
                // No more packets available
                break;
                
            } else if (result == -1) {
                // Error occurred
                (void)pcap_geterr(pcap_handle);
                break;
                
            } else if (result == -2) {
                // EOF reached
                break;
            }
        }
    } else {
        // PRIMARY TARGET: pcap_loop (alternative packet reading method)
        struct loop_context {
            int packet_count;
        } ctx = {0};
        
        pcap_handler callback = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
            struct loop_context* ctx = reinterpret_cast<struct loop_context*>(user);
            ctx->packet_count++;
            (void)h;
            (void)bytes;
        };
        
        // Try pcap_loop with small count
        if (pcap_loop(pcap_handle, 10, callback, reinterpret_cast<u_char*>(&ctx)) != 0) {
            (void)pcap_geterr(pcap_handle);
        }
        packets_read = ctx.packet_count;
    }
    
    // Phase 6: Optional dump/write functionality
    pcap_dumper_t* dumper = nullptr;
    if (test_dump && packets_read > 0) {
        // Create output file for dumping
        char out_filename[] = "/tmp/libpcap_pcapng_fuzz_output_XXXXXX";
        int out_fd = mkstemp(out_filename);
        if (out_fd >= 0) {
            close(out_fd);
            
            // PRIMARY TARGET: pcap_dump_open
            dumper = pcap_dump_open(pcap_handle, out_filename);
            
            if (dumper != nullptr) {
                // Rewind and read packets again to dump them
                // Note: In real scenario, we would need to re-open or save packets
                // For simplicity, we'll just test the dump API with a dummy header
                struct pcap_pkthdr dummy_header;
                dummy_header.ts.tv_sec = 0;
                dummy_header.ts.tv_usec = 0;
                dummy_header.caplen = 10;
                dummy_header.len = 10;
                
                uint8_t dummy_data[10] = {0};
                
                // PRIMARY TARGET: pcap_dump
                pcap_dump(reinterpret_cast<u_char*>(dumper), &dummy_header, dummy_data);
                
                // PRIMARY TARGET: pcap_dump_close
                pcap_dump_close(dumper);
                dumper = nullptr;
            }
            
            unlink(out_filename);
        }
    }
    
    // Phase 7: Cleanup
    // PRIMARY TARGET: pcap_close (essential for resource management)
    if (filter_applied) {
        // PRIMARY TARGET: pcap_freecode (if filter was compiled)
        pcap_freecode(&fcode);
    }
    
    // PRIMARY TARGET: pcap_close
    pcap_close(pcap_handle);
    
    // Clean up temporary input file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_019
