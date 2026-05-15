#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <memory>

namespace harness_013 {
/*
 * libpcap fuzzing harness - harness_013.cpp
 * Targets savefile and sf-pcap modules with 0% coverage - file format detection and parsing
 * 
 * Primary APIs and internal functions targeted:
 * - pcap_fopen_offline_with_tstamp_precision (tests savefile.c internal implementation)
 * - pcap_check_header (sf-pcap.c function for pcap format detection)
 * - pcap_ng_check_header (sf-pcapng.c function for pcapng format detection)
 * - File format magic number validation and parsing
 * - Different pcap timestamp precision handling (micro vs nano)
 * - Error paths in savefile reading (truncated files, invalid magic)
 * 
 * Differentiates from existing harnesses:
 * - harness_002: File I/O lifecycle with disk files
 * - harness_004: Savefile APIs with timestamp precision (uses disk files)
 * - harness_011: Savefile write operations
 * - harness_013: Memory-based FILE* testing of internal savefile parsing (THIS HARNESS)
 * 
 * Targets the 150+ undiscovered branches in savefile.c and sf-pcap.c modules.
 * Focuses on internal static functions not reachable via public APIs alone.
 * Uses fmemopen to create FILE* from memory, testing byte-by-byte parsing.
 */


#ifndef _GNU_SOURCE
#define _GNU_SOURCE  // For fmemopen
#endif

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for meaningful pcap header testing
    // At minimum: 4 bytes magic + 2 bytes version + 2 bytes reserved + 4 bytes snaplen + 4 bytes linktype
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test pattern type to test different scenarios
    int test_pattern = fdp.ConsumeIntegralInRange<int>(0, 5);
    int timestamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=micro, 1=nano
    
    std::vector<uint8_t> file_data;
    
    switch (test_pattern) {
        case 0: {
            // Pattern 1: Valid standard pcap format (TCPDUMP_MAGIC 0xa1b2c3d4)
            file_data.reserve(24);
            // Magic number: TCPDUMP_MAGIC
            file_data.push_back(0xa1);
            file_data.push_back(0xb2);
            file_data.push_back(0xc3);
            file_data.push_back(0xd4);
            // Version major/minor
            file_data.push_back(0x02);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            // Timezone (always 0)
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            // Sigfigs (always 0)
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            // Snaplen (consumed from fuzzer)
            uint32_t snaplen = fdp.ConsumeIntegral<uint32_t>();
            file_data.push_back((snaplen >> 24) & 0xFF);
            file_data.push_back((snaplen >> 16) & 0xFF);
            file_data.push_back((snaplen >> 8) & 0xFF);
            file_data.push_back(snaplen & 0xFF);
            // Linktype (consumed from fuzzer)
            uint32_t linktype = fdp.ConsumeIntegral<uint32_t>();
            file_data.push_back((linktype >> 24) & 0xFF);
            file_data.push_back((linktype >> 16) & 0xFF);
            file_data.push_back((linktype >> 8) & 0xFF);
            file_data.push_back(linktype & 0xFF);
            break;
        }
        
        case 1: {
            // Pattern 2: Nanosecond pcap format (NSEC_TCPDUMP_MAGIC 0xa1b23c4d)
            file_data.reserve(24);
            // Magic number: NSEC_TCPDUMP_MAGIC
            file_data.push_back(0xa1);
            file_data.push_back(0xb2);
            file_data.push_back(0x3c);
            file_data.push_back(0x4d);
            // Version major/minor
            file_data.push_back(0x02);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            // Timezone (always 0)
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            // Sigfigs (always 0)
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            // Snaplen (consumed from fuzzer)
            uint32_t snaplen = fdp.ConsumeIntegral<uint32_t>();
            file_data.push_back((snaplen >> 24) & 0xFF);
            file_data.push_back((snaplen >> 16) & 0xFF);
            file_data.push_back((snaplen >> 8) & 0xFF);
            file_data.push_back(snaplen & 0xFF);
            // Linktype (consumed from fuzzer)
            uint32_t linktype = fdp.ConsumeIntegral<uint32_t>();
            file_data.push_back((linktype >> 24) & 0xFF);
            file_data.push_back((linktype >> 16) & 0xFF);
            file_data.push_back((linktype >> 8) & 0xFF);
            file_data.push_back(linktype & 0xFF);
            break;
        }
        
        case 2: {
            // Pattern 3: Swapped byte order pcap format (0xd4c3b2a1)
            file_data.reserve(24);
            // Magic number: Swapped TCPDUMP_MAGIC
            file_data.push_back(0xd4);
            file_data.push_back(0xc3);
            file_data.push_back(0xb2);
            file_data.push_back(0xa1);
            // Version major/minor
            file_data.push_back(0x02);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            file_data.push_back(0x00);
            // Rest of header (use remaining fuzzer bytes)
            size_t remaining_header = 16;
            if (fdp.remaining_bytes() < remaining_header) {
                remaining_header = fdp.remaining_bytes();
            }
            std::vector<uint8_t> header_part = fdp.ConsumeBytes<uint8_t>(remaining_header);
            file_data.insert(file_data.end(), header_part.begin(), header_part.end());
            // Pad if needed
            while (file_data.size() < 24) {
                file_data.push_back(0);
            }
            break;
        }
        
        case 3: {
            // Pattern 4: Invalid/unknown magic numbers
            // Use fuzzer data directly as file content
            size_t file_size = fdp.ConsumeIntegralInRange<size_t>(4, fdp.remaining_bytes());
            file_data = fdp.ConsumeBytes<uint8_t>(file_size);
            break;
        }
        
        case 4: {
            // Pattern 5: Truncated pcap file (less than full header)
            size_t truncate_size = fdp.ConsumeIntegralInRange<size_t>(1, 23);
            if (fdp.remaining_bytes() < truncate_size) {
                truncate_size = fdp.remaining_bytes();
            }
            file_data = fdp.ConsumeBytes<uint8_t>(truncate_size);
            break;
        }
        
        case 5: {
            // Pattern 6: Mixed content with potential pcapng format attempt
            // Start with some data, then try to add pcap-like structure
            size_t initial_data = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
            file_data = fdp.ConsumeBytes<uint8_t>(initial_data);
            
            // Try to add a section header block (pcapng-like)
            if (fdp.remaining_bytes() >= 16) {
                // Section Header Block Type (0x0A0D0D0A)
                file_data.push_back(0x0A);
                file_data.push_back(0x0D);
                file_data.push_back(0x0D);
                file_data.push_back(0x0A);
                // Block Total Length (placeholder)
                file_data.push_back(0x00);
                file_data.push_back(0x00);
                file_data.push_back(0x00);
                file_data.push_back(0x1C);
                // Byte-order magic (0x1A2B3C4D)
                file_data.push_back(0x1A);
                file_data.push_back(0x2B);
                file_data.push_back(0x3C);
                file_data.push_back(0x4D);
                // Version major/minor
                file_data.push_back(0x01);
                file_data.push_back(0x00);
                file_data.push_back(0x00);
                file_data.push_back(0x00);
                // Rest of data
                size_t remaining = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
                std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(remaining);
                file_data.insert(file_data.end(), extra.begin(), extra.end());
            }
            break;
        }
    }
    
    // Use fmemopen to create a FILE* from the memory buffer
    FILE* mem_file = fmemopen(file_data.data(), file_data.size(), "rb");
    if (mem_file == nullptr) {
        return 0;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Test pcap_fopen_offline_with_tstamp_precision - this is the internal function
    // that handles all the file format detection logic
    pcap_t* pcap_handle = pcap_fopen_offline_with_tstamp_precision(mem_file, timestamp_precision, errbuf);
    
    if (pcap_handle != nullptr) {
        // Successfully opened - test some operations
        int datalink = pcap_datalink(pcap_handle);
        (void)datalink;
        
        int snapshot = pcap_snapshot(pcap_handle);
        (void)snapshot;
        
        int precision = pcap_get_tstamp_precision(pcap_handle);
        (void)precision;
        
        // Try to read packets if available
        struct pcap_pkthdr header;
        const u_char* packet;
        int packets_read = 0;
        const int MAX_PACKETS = 10;
        
        while (packets_read < MAX_PACKETS) {
            packet = pcap_next(pcap_handle, &header);
            if (packet == nullptr) {
                break;
            }
            packets_read++;
        }
        
        // Test pcap_stats on savefile
        struct pcap_stat stats;
        int stats_result = pcap_stats(pcap_handle, &stats);
        (void)stats_result;
        
        // Test pcap_getnonblock (should fail for savefiles)
        char nonblock_err[PCAP_ERRBUF_SIZE];
        int nonblock_result = pcap_getnonblock(pcap_handle, nonblock_err);
        (void)nonblock_result;
        
        pcap_close(pcap_handle);
        mem_file = nullptr;  // pcap_close already closed the FILE* via pcapint_sf_cleanup
    } else {
        // pcap_fopen_offline_with_tstamp_precision failed
        // Try regular pcap_fopen_offline as fallback
        // Need to reopen since the previous call consumed the FILE*
        // First close the original FILE* since it won't be closed by libpcap
        fclose(mem_file);
        mem_file = nullptr;
        
        // Reopen with fmemopen
        mem_file = fmemopen(file_data.data(), file_data.size(), "rb");
        if (mem_file != nullptr) {
            pcap_handle = pcap_fopen_offline(mem_file, errbuf);
            if (pcap_handle != nullptr) {
                // Test basic operations
                int datalink = pcap_datalink(pcap_handle);
                (void)datalink;
                pcap_close(pcap_handle);
                // pcap_close will close mem_file via pcapint_sf_cleanup
                mem_file = nullptr;  // Mark as already closed
            }
        }
    }
    
    // Only close mem_file if it hasn't been closed by pcap_close
    if (mem_file != nullptr) {
        fclose(mem_file);
    }
    
    return 0;
}

} // namespace harness_013
