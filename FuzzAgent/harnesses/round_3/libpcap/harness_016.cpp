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

namespace harness_016 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful pcapng testing
    // Need enough data for pcapng structure: SHB + IDB + EPB blocks
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size control parameters first
    int opening_method = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which pcap_open variant to use
    int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=microseconds, 1=nanoseconds
    int block_type_variant = fdp.ConsumeIntegralInRange<int>(0, 3);  // Which block types to generate
    int num_interfaces = fdp.ConsumeIntegralInRange<int>(1, 4);  // Number of IDB blocks
    int num_packets = fdp.ConsumeIntegralInRange<int>(1, 10);  // Number of packet blocks
    
    // Generate pcapng data structure
    std::vector<uint8_t> pcapng_data;
    
    // PCAPNG constants
    const uint32_t BT_SHB = 0x0A0D0D0A;  // Section Header Block
    const uint32_t BT_IDB = 0x00000001;  // Interface Description Block
    const uint32_t BT_EPB = 0x00000006;  // Enhanced Packet Block
    const uint32_t BT_SPB = 0x00000003;  // Simple Packet Block
    const uint32_t BYTE_ORDER_MAGIC = 0x1A2B3C4D;
    
    // Helper function to add 32-bit value with proper alignment
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
    
    auto add_u64 = [&](uint64_t value) {
        add_u32(value & 0xFFFFFFFF);
        add_u32((value >> 32) & 0xFFFFFFFF);
    };
    
    // Pad to 32-bit boundary
    auto pad_to_32bit = [&]() {
        while (pcapng_data.size() % 4 != 0) {
            pcapng_data.push_back(0);
        }
    };
    
    // Add block with total length field
    auto start_block = [&](uint32_t block_type) {
        add_u32(block_type);
        // Placeholder for total_length - will be filled later
        size_t total_length_pos = pcapng_data.size();
        add_u32(0);  // Temporary total length
        return total_length_pos;
    };
    
    auto end_block = [&](size_t total_length_pos) {
        size_t block_end = pcapng_data.size();
        uint32_t total_length = block_end - total_length_pos + 4;  // +4 for the total_length field itself
        
        // Update total_length field
        pcapng_data[total_length_pos] = (total_length >> 0) & 0xFF;
        pcapng_data[total_length_pos + 1] = (total_length >> 8) & 0xFF;
        pcapng_data[total_length_pos + 2] = (total_length >> 16) & 0xFF;
        pcapng_data[total_length_pos + 3] = (total_length >> 24) & 0xFF;
        
        // Add block trailer (copy of total_length)
        add_u32(total_length);
    };
    
    // 1. Section Header Block (SHB)
    size_t shb_pos = start_block(BT_SHB);
    
    // Byte order magic
    add_u32(BYTE_ORDER_MAGIC);
    
    // Major/minor version (1.0)
    add_u16(1);  // major
    add_u16(0);  // minor
    
    // Section length (unlimited)
    add_u64(0xFFFFFFFFFFFFFFFF);
    
    // Add some options
    pad_to_32bit();
    // Option: comment (optional)
    if (fdp.ConsumeBool()) {
        std::string comment = fdp.ConsumeRandomLengthString(100);
        add_u16(1);  // OPT_COMMENT
        add_u16(comment.size());
        pcapng_data.insert(pcapng_data.end(), comment.begin(), comment.end());
        pad_to_32bit();
    }
    
    // End of options
    add_u16(0);  // OPT_ENDOFOPT
    add_u16(0);
    
    end_block(shb_pos);
    
    // 2. Interface Description Blocks (IDB)
    for (int i = 0; i < num_interfaces; i++) {
        size_t idb_pos = start_block(BT_IDB);
        
        // Link type (consume from fuzzer)
        uint16_t linktype = fdp.ConsumeIntegral<uint16_t>();
        add_u16(linktype);
        
        // Reserved
        add_u16(0);
        
        // Snaplen
        uint32_t snaplen = fdp.ConsumeIntegralInRange<uint32_t>(68, 65535);
        add_u32(snaplen);
        
        // Add interface options
        pad_to_32bit();
        
        // Interface name option
        if (fdp.ConsumeBool()) {
            std::string ifname = "if" + std::to_string(i);
            add_u16(2);  // IF_NAME
            add_u16(ifname.size());
            pcapng_data.insert(pcapng_data.end(), ifname.begin(), ifname.end());
            pad_to_32bit();
        }
        
        // Time stamp resolution option
        if (fdp.ConsumeBool()) {
            add_u16(9);  // IF_TSRESOL
            add_u16(1);
            uint8_t tsresol = fdp.ConsumeIntegral<uint8_t>();
            pcapng_data.push_back(tsresol);
            pad_to_32bit();
        }
        
        // End of options
        add_u16(0);  // OPT_ENDOFOPT
        add_u16(0);
        
        end_block(idb_pos);
    }
    
    // 3. Packet Blocks (Enhanced Packet Blocks or Simple Packet Blocks)
    for (int i = 0; i < num_packets; i++) {
        uint32_t block_type;
        if (block_type_variant == 0) {
            block_type = BT_EPB;
        } else if (block_type_variant == 1) {
            block_type = BT_SPB;
        } else {
            // Mix of both
            block_type = (i % 2 == 0) ? BT_EPB : BT_SPB;
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
        if (fdp.ConsumeBool() && block_type == BT_EPB) {
            // Add comment option
            std::string comment = "packet_" + std::to_string(i);
            add_u16(1);  // OPT_COMMENT
            add_u16(comment.size());
            pcapng_data.insert(pcapng_data.end(), comment.begin(), comment.end());
            pad_to_32bit();
        }
        
        // End of options
        add_u16(0);  // OPT_ENDOFOPT
        add_u16(0);
        
        end_block(packet_pos);
    }
    
    // Create temporary file with pcapng data
    char temp_filename[] = "/tmp/libpcap_pcapng_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    
    // Write pcapng data to temporary file
    if (write(fd, pcapng_data.data(), pcapng_data.size()) != static_cast<ssize_t>(pcapng_data.size())) {
        close(fd);
        unlink(temp_filename);
        return 0;
    }
    close(fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    
    // PRIMARY TARGET: pcap_open_offline (and variants) for pcapng parsing
    // These APIs should detect pcapng format and route to sf-pcapng.c code
    switch (opening_method) {
        case 0:
            // Basic pcap_open_offline
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
            // Note: pcap_fopen_offline takes ownership of the FILE*
            break;
        }
            
        case 3: {
            // pcap_fopen_offline_with_tstamp_precision (if available)
            #ifndef _WIN32
            FILE* f2 = fopen(temp_filename, "rb");
            if (f2 == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            pcap_handle = pcap_fopen_offline_with_tstamp_precision(f2, tstamp_precision, errbuf);
            #else
            // On Windows, fall back to regular pcap_open_offline
            pcap_handle = pcap_open_offline(temp_filename, errbuf);
            #endif
            break;
        }
    }
    
    if (pcap_handle == nullptr) {
        // Opening failed - test error handling
        const char* error_str = pcap_strerror(PCAP_ERROR);
        (void)error_str;  // Use to avoid unused variable warning
        
        // Clean up and return
        unlink(temp_filename);
        return 0;
    }
    
    // PRIMARY TARGET: pcap_next_ex to read packets from pcapng file
    // This should exercise pcap_ng_next_packet() in sf-pcapng.c
    int packets_read = 0;
    struct pcap_pkthdr* header;
    const u_char* pkt_data;
    
    while (packets_read < 50) {  // Limit to avoid infinite loops
        int result = pcap_next_ex(pcap_handle, &header, &pkt_data);
        
        if (result == 1) {
            // Successfully read a packet
            packets_read++;
            
            // Test packet inspection APIs
            if (header != nullptr && pkt_data != nullptr) {
                // Access some packet header fields
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
            const char* error_str = pcap_geterr(pcap_handle);
            (void)error_str;
            break;
            
        } else if (result == -2) {
            // EOF reached
            break;
        }
    }
    
    // Test pcap_loop as alternative packet reading method
    // This should also exercise pcapng parsing code paths
    struct loop_context {
        int packet_count;
    } ctx = {0};
    
    pcap_handler callback = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
        struct loop_context* ctx = reinterpret_cast<struct loop_context*>(user);
        ctx->packet_count++;
        (void)h;  // Use to avoid unused parameter warning
        (void)bytes;
    };
    
    // Try pcap_loop with small count
    if (pcap_loop(pcap_handle, 5, callback, reinterpret_cast<u_char*>(&ctx)) != 0) {
        // Error in pcap_loop
        const char* error_str = pcap_geterr(pcap_handle);
        (void)error_str;
    }
    
    // Test other pcap APIs on pcapng file
    // PRIMARY TARGET: pcap_file, pcap_fileno (though may not work for all pcapng files)
    FILE* pcap_file_ptr = pcap_file(pcap_handle);
    (void)pcap_file_ptr;
    
    int pcap_fd = pcap_fileno(pcap_handle);
    (void)pcap_fd;
    
    // Test stats (may work for some pcapng files)
    struct pcap_stat stats;
    if (pcap_stats(pcap_handle, &stats) == 0) {
        (void)stats.ps_recv;
        (void)stats.ps_drop;
        (void)stats.ps_ifdrop;
    }
    
    // PRIMARY TARGET: pcap_close - required cleanup
    pcap_close(pcap_handle);
    
    // Clean up temporary file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_016
