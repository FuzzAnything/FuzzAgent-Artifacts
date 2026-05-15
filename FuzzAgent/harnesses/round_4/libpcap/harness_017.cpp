#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <pcap/pcap.h>

namespace harness_017 {

// Helper function to write buffer to a temporary file
static int bufferToFile(const char *name, const uint8_t *data, size_t size) {
    FILE *fd;
    if (remove(name) != 0) {
        // Ignore ENOENT errors
    }
    fd = fopen(name, "wb");
    if (fd == NULL) {
        return -1;
    }
    if (fwrite(data, 1, size, fd) != size) {
        fclose(fd);
        return -2;
    }
    fclose(fd);
    return 0;
}

// pcapng block type constants
#define BT_SHB 0x0A0D0D0A          // Section Header Block
#define BT_IDB 0x00000001          // Interface Description Block
#define BT_EPB 0x00000006          // Enhanced Packet Block
#define BT_SPB 0x00000003          // Simple Packet Block
#define BYTE_ORDER_MAGIC 0x1A2B3C4D

// Helper to write a pcapng block header
static void write_block_header(FILE *fd, uint32_t block_type, uint32_t total_length, bool swap) {
    uint32_t type = swap ? __builtin_bswap32(block_type) : block_type;
    uint32_t length = swap ? __builtin_bswap32(total_length) : total_length;
    fwrite(&type, sizeof(type), 1, fd);
    fwrite(&length, sizeof(length), 1, fd);
}

// Helper to write a pcapng block trailer
static void write_block_trailer(FILE *fd, uint32_t total_length, bool swap) {
    uint32_t length = swap ? __builtin_bswap32(total_length) : total_length;
    fwrite(&length, sizeof(length), 1, fd);
}

// Helper to write a Section Header Block (SHB)
static void write_shb(FILE *fd, uint16_t major, uint16_t minor, uint64_t section_length, 
                      bool swap, bool include_options, const std::vector<uint8_t>& options) {
    uint32_t block_length = 28; // SHB fixed part
    if (include_options && !options.empty()) {
        block_length += options.size();
        // Align to 4 bytes
        while (block_length % 4 != 0) block_length++;
    }
    block_length += 4; // trailer
    
    write_block_header(fd, BT_SHB, block_length, swap);
    
    uint32_t bom = swap ? __builtin_bswap32(BYTE_ORDER_MAGIC) : BYTE_ORDER_MAGIC;
    uint16_t maj = swap ? __builtin_bswap16(major) : major;
    uint16_t min = swap ? __builtin_bswap16(minor) : minor;
    uint64_t slen = swap ? __builtin_bswap64(section_length) : section_length;
    
    fwrite(&bom, sizeof(bom), 1, fd);
    fwrite(&maj, sizeof(maj), 1, fd);
    fwrite(&min, sizeof(min), 1, fd);
    fwrite(&slen, sizeof(slen), 1, fd);
    
    if (include_options && !options.empty()) {
        fwrite(options.data(), 1, options.size(), fd);
        // Padding
        size_t pad = (4 - (options.size() % 4)) % 4;
        for (size_t i = 0; i < pad; i++) fputc(0, fd);
    }
    
    write_block_trailer(fd, block_length, swap);
}

// Helper to write an Interface Description Block (IDB)
static void write_idb(FILE *fd, uint16_t linktype, uint32_t snaplen, bool swap,
                     bool include_options, const std::vector<uint8_t>& options) {
    uint32_t block_length = 12; // IDB fixed part
    if (include_options && !options.empty()) {
        block_length += options.size();
        while (block_length % 4 != 0) block_length++;
    }
    block_length += 4; // trailer
    
    write_block_header(fd, BT_IDB, block_length, swap);
    
    uint16_t lt = swap ? __builtin_bswap16(linktype) : linktype;
    uint16_t reserved = 0;
    uint32_t snap = swap ? __builtin_bswap32(snaplen) : snaplen;
    
    fwrite(&lt, sizeof(lt), 1, fd);
    fwrite(&reserved, sizeof(reserved), 1, fd);
    fwrite(&snap, sizeof(snap), 1, fd);
    
    if (include_options && !options.empty()) {
        fwrite(options.data(), 1, options.size(), fd);
        size_t pad = (4 - (options.size() % 4)) % 4;
        for (size_t i = 0; i < pad; i++) fputc(0, fd);
    }
    
    write_block_trailer(fd, block_length, swap);
}

// Helper to write an Enhanced Packet Block (EPB)
static void write_epb(FILE *fd, uint32_t interface_id, uint32_t timestamp_high,
                      uint32_t timestamp_low, const std::vector<uint8_t>& packet_data,
                      bool swap, bool include_options, const std::vector<uint8_t>& options) {
    uint32_t caplen = packet_data.size();
    uint32_t len = caplen;
    uint32_t block_length = 32 + caplen; // EPB fixed part + packet data
    if (include_options && !options.empty()) {
        block_length += options.size();
        while (block_length % 4 != 0) block_length++;
    }
    block_length += 4; // trailer
    
    write_block_header(fd, BT_EPB, block_length, swap);
    
    uint32_t ifid = swap ? __builtin_bswap32(interface_id) : interface_id;
    uint32_t ts_high = swap ? __builtin_bswap32(timestamp_high) : timestamp_high;
    uint32_t ts_low = swap ? __builtin_bswap32(timestamp_low) : timestamp_low;
    uint32_t cap = swap ? __builtin_bswap32(caplen) : caplen;
    uint32_t packet_len = swap ? __builtin_bswap32(len) : len;
    
    fwrite(&ifid, sizeof(ifid), 1, fd);
    fwrite(&ts_high, sizeof(ts_high), 1, fd);
    fwrite(&ts_low, sizeof(ts_low), 1, fd);
    fwrite(&cap, sizeof(cap), 1, fd);
    fwrite(&packet_len, sizeof(packet_len), 1, fd);
    
    if (caplen > 0) {
        fwrite(packet_data.data(), 1, caplen, fd);
        // Padding for packet data to 4-byte boundary
        size_t pad = (4 - (caplen % 4)) % 4;
        for (size_t i = 0; i < pad; i++) fputc(0, fd);
    }
    
    if (include_options && !options.empty()) {
        fwrite(options.data(), 1, options.size(), fd);
        size_t pad = (4 - (options.size() % 4)) % 4;
        for (size_t i = 0; i < pad; i++) fputc(0, fd);
    }
    
    write_block_trailer(fd, block_length, swap);
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need sufficient input for pcapng file generation
    // Minimum: scenario + basic block data
    if (Size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different pcapng test scenarios
    // First byte: test scenario selector (0-5)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Second byte: byte order (0 = native, 1 = swapped)
    bool byte_swap = fdp.ConsumeBool();
    
    // Third byte: pcapng version major
    uint8_t version_major = fdp.ConsumeIntegral<uint8_t>();
    if (version_major == 0) version_major = 1;
    
    // Fourth byte: pcapng version minor
    uint8_t version_minor = fdp.ConsumeIntegral<uint8_t>();
    
    // Fifth and sixth bytes: link type
    uint16_t linktype = fdp.ConsumeIntegral<uint16_t>();
    
    // Seventh and eighth bytes: snapshot length
    uint32_t snaplen = fdp.ConsumeIntegralInRange<uint32_t>(1, 65535);
    
    // Ninth and tenth bytes: section length (high 32 bits)
    uint32_t section_length_high = fdp.ConsumeIntegral<uint32_t>();
    
    // Eleventh and twelfth bytes: section length (low 32 bits)
    uint32_t section_length_low = fdp.ConsumeIntegral<uint32_t>();
    uint64_t section_length = ((uint64_t)section_length_high << 32) | section_length_low;
    
    // Consume remaining bytes for various uses
    std::vector<uint8_t> remaining_bytes = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    // Create a temporary file
    char filename[] = "/tmp/libpcap_pcapng_fuzz_XXXXXX";
    int fd = mkstemp(filename);
    if (fd < 0) {
        return 0;
    }
    close(fd);
    
    FILE *tmpfile = fopen(filename, "wb");
    if (tmpfile == NULL) {
        unlink(filename);
        return 0;
    }
    
    pcap_t *pcap_handle = nullptr;
    int result = 0;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Minimal valid pcapng file (SHB only)
            write_shb(tmpfile, version_major, version_minor, section_length, 
                     byte_swap, false, {});
            fclose(tmpfile);
            
            pcap_handle = pcap_open_offline(filename, errbuf);
            if (pcap_handle != nullptr) {
                // Try to read packets (should get none)
                struct pcap_pkthdr *header;
                const u_char *pkt;
                int r = pcap_next_ex(pcap_handle, &header, &pkt);
                // r should be -2 (no more packets) or -1 (error)
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Complete pcapng file with SHB + IDB + EPB
            write_shb(tmpfile, version_major, version_minor, section_length, 
                     byte_swap, false, {});
            
            write_idb(tmpfile, linktype, snaplen, byte_swap, false, {});
            
            // Create packet data from remaining bytes
            size_t packet_size = remaining_bytes.size() > 1024 ? 1024 : remaining_bytes.size();
            std::vector<uint8_t> packet_data(remaining_bytes.begin(), 
                                            remaining_bytes.begin() + packet_size);
            
            write_epb(tmpfile, 0, 0, 0, packet_data, byte_swap, false, {});
            
            fclose(tmpfile);
            
            pcap_handle = pcap_open_offline(filename, errbuf);
            if (pcap_handle != nullptr) {
                // Read packets
                struct pcap_pkthdr *header;
                const u_char *pkt;
                int packet_count = 0;
                int r = pcap_next_ex(pcap_handle, &header, &pkt);
                while (r >= 0 && packet_count < 10) {
                    packet_count++;
                    r = pcap_next_ex(pcap_handle, &header, &pkt);
                }
                
                // Test other pcap functions
                pcap_datalink(pcap_handle);
                pcap_snapshot(pcap_handle);
                pcap_major_version(pcap_handle);
                pcap_minor_version(pcap_handle);
                
                struct pcap_stat stats;
                pcap_stats(pcap_handle, &stats);
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: pcapng file with options
            std::vector<uint8_t> shb_options;
            if (remaining_bytes.size() > 100) {
                shb_options.insert(shb_options.end(), 
                                  remaining_bytes.begin(), 
                                  remaining_bytes.begin() + 100);
            }
            
            write_shb(tmpfile, version_major, version_minor, section_length, 
                     byte_swap, true, shb_options);
            
            std::vector<uint8_t> idb_options;
            if (remaining_bytes.size() > 200) {
                idb_options.insert(idb_options.end(),
                                  remaining_bytes.begin() + 100,
                                  remaining_bytes.begin() + 200);
            }
            
            write_idb(tmpfile, linktype, snaplen, byte_swap, true, idb_options);
            
            fclose(tmpfile);
            
            pcap_handle = pcap_open_offline(filename, errbuf);
            if (pcap_handle != nullptr) {
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Multiple IDBs and EPBs
            write_shb(tmpfile, version_major, version_minor, section_length, 
                     byte_swap, false, {});
            
            // Write multiple IDBs
            for (int i = 0; i < 3 && remaining_bytes.size() > i * 50; i++) {
                uint16_t lt = linktype + i;
                uint32_t snap = snaplen / (i + 1);
                if (snap == 0) snap = 1;
                
                write_idb(tmpfile, lt, snap, byte_swap, false, {});
            }
            
            // Write multiple EPBs with different interface IDs
            for (int i = 0; i < 5 && remaining_bytes.size() > 150 + i * 100; i++) {
                size_t offset = 150 + i * 100;
                size_t packet_size = 100;
                if (offset + packet_size > remaining_bytes.size()) {
                    packet_size = remaining_bytes.size() - offset;
                }
                if (packet_size == 0) break;
                
                std::vector<uint8_t> packet_data(remaining_bytes.begin() + offset,
                                                remaining_bytes.begin() + offset + packet_size);
                
                write_epb(tmpfile, i % 3, i * 1000, i * 100, packet_data, 
                         byte_swap, false, {});
            }
            
            fclose(tmpfile);
            
            pcap_handle = pcap_open_offline(filename, errbuf);
            if (pcap_handle != nullptr) {
                // Read all packets
                struct pcap_pkthdr *header;
                const u_char *pkt;
                int packet_count = 0;
                int r = pcap_next_ex(pcap_handle, &header, &pkt);
                while (r >= 0 && packet_count < 50) {
                    packet_count++;
                    r = pcap_next_ex(pcap_handle, &header, &pkt);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Corrupted pcapng file to test error paths
            // Write invalid block
            uint32_t invalid_block_type = 0xDEADBEEF;
            uint32_t invalid_length = 8; // Too small
            write_block_header(tmpfile, invalid_block_type, invalid_length, byte_swap);
            write_block_trailer(tmpfile, invalid_length, byte_swap);
            
            fclose(tmpfile);
            
            pcap_handle = pcap_open_offline(filename, errbuf);
            // Should fail
            if (pcap_handle != nullptr) {
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Mixed valid and invalid data
            // Start with valid SHB
            write_shb(tmpfile, version_major, version_minor, section_length, 
                     byte_swap, false, {});
            
            // Write some random data (could be interpreted as invalid blocks)
            if (!remaining_bytes.empty()) {
                fwrite(remaining_bytes.data(), 1, remaining_bytes.size(), tmpfile);
            }
            
            fclose(tmpfile);
            
            pcap_handle = pcap_open_offline(filename, errbuf);
            if (pcap_handle != nullptr) {
                // Try to read (might succeed partially or fail)
                struct pcap_pkthdr *header;
                const u_char *pkt;
                int r = pcap_next_ex(pcap_handle, &header, &pkt);
                
                pcap_close(pcap_handle);
            }
            break;
        }
    }
    
    // Clean up
    if (tmpfile && !feof(tmpfile)) {
        // File already closed in switch cases
    }
    unlink(filename);
    
    return 0;
}

} // namespace harness_017
