#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <memory>

namespace harness_016 {
/*
 * libpcap fuzzing harness - harness_016.cpp
 * Comprehensive dump/savefile functionality testing with complete lifecycle
 * 
 * Critical Coverage Gap Identified:
 * - pcap_dump_open_append: 56 undiscovered branches (largest single API gap)
 * - pcap_dump_open: 12 branches, 100% undiscovered (0% function lines hit)
 * - pcap_dump: 4 branches, 100% undiscovered (0% function lines hit)
 * - pcap_dump_flush: 2 branches, 100% undiscovered (0% function lines hit)
 * - pcap_dump_close: 2 branches, 100% undiscovered (0% function lines hit)
 * 
 * These APIs are reached by some fuzzers but show 0% execution due to:
 * 1. Parameter validation failures
 * 2. Missing prerequisite setup
 * 3. Invalid pcap file headers for append operations
 * 
 * Differentiates from existing harnesses:
 * - harness_002: Basic file I/O operations with pcap_open_offline
 * - harness_011: Savefile write subsystem with format-specific dumping
 * - harness_014: Comprehensive dump file operations with timestamp precision
 * - harness_015: pcap_dump_open_append with valid pcap file creation
 * - harness_016: COMPLETE LIFECYCLE with packet retrieval, real pcap handle, and append testing (THIS HARNESS)
 * 
 * Strategy for harness_016:
 * 1. Create valid pcap file with proper header and packet data
 * 2. Use pcap_open_offline (not pcap_open_dead) to get real pcap_t handle
 * 3. Retrieve packets with pcap_next_ex loop (not simulated packets)
 * 4. Test complete dump lifecycle: open, write, flush, append, close
 * 5. Test with both micro and nanosecond timestamp precision
 * 6. Focus on error paths and edge cases in append mode
 * 7. Ensure semantic diversity by using real packet data flow
 * 
 * This harness specifically addresses the 76+ undiscovered branches
 * by creating the proper preconditions AND using real packet retrieval
 * that existing harnesses miss.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for pcap header and at least one packet
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int use_timestamp_precision = fdp.ConsumeBool() ? 1 : 0;
    int test_append_mode = fdp.ConsumeBool() ? 1 : 0;
    int test_flush_operations = fdp.ConsumeBool() ? 1 : 0;
    int max_packets_to_read = fdp.ConsumeIntegralInRange<int>(0, 50);
    // Note: tstamp_precision will be consumed later if use_timestamp_precision is true
    // Consume packet header timestamp values
    uint32_t packet_tv_sec = fdp.ConsumeIntegral<uint32_t>();
    uint32_t packet_tv_usec = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume packet data (leave some bytes for potential future use)
    size_t packet_data_size = fdp.ConsumeIntegralInRange<size_t>(16, fdp.remaining_bytes());
    std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_data_size);
    
    // Create temporary input file with valid pcap header and packet data
    char input_filename[] = "/tmp/libpcap_fuzz_input_016_XXXXXX";
    int input_fd = mkstemp(input_filename);
    if (input_fd < 0) {
        return 0;
    }
    
    // Write a valid pcap file header (magic number, version, etc.)
    // This ensures pcap_open_offline will succeed
    struct pcap_file_header file_header;
    file_header.magic = 0xa1b2c3d4;  // Standard pcap magic number
    file_header.version_major = 2;
    file_header.version_minor = 4;
    file_header.thiszone = 0;
    file_header.sigfigs = 0;
    file_header.snaplen = snaplen;
    file_header.linktype = linktype;
    
    if (write(input_fd, &file_header, sizeof(file_header)) != sizeof(file_header)) {
        close(input_fd);
        unlink(input_filename);
        return 0;
    }
    
    // Write at least one packet header and data if we have enough
    if (packet_data.size() >= 16) {
        struct pcap_pkthdr packet_header;
        packet_header.ts.tv_sec = packet_tv_sec;
        packet_header.ts.tv_usec = packet_tv_usec;
        packet_header.caplen = std::min(static_cast<size_t>(snaplen), packet_data.size());
        packet_header.len = packet_data.size();
        
        if (write(input_fd, &packet_header, sizeof(packet_header)) == sizeof(packet_header)) {
            write(input_fd, packet_data.data(), packet_header.caplen);
        }
    }
    
    close(input_fd);
    
    // Create temporary output file for dump operations
    char output_filename[] = "/tmp/libpcap_fuzz_output_016_XXXXXX";
    int output_fd = mkstemp(output_filename);
    if (output_fd < 0) {
        unlink(input_filename);
        return 0;
    }
    close(output_fd);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    pcap_dumper_t* dumper = nullptr;
    
    // Step 1: Open the input file with pcap_open_offline
    // This creates a real pcap_t handle that can read packets
    if (use_timestamp_precision) {
        int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=micro, 1=nano
        pcap_handle = pcap_open_offline_with_tstamp_precision(input_filename, 
                                                              tstamp_precision, errbuf);
    } else {
        pcap_handle = pcap_open_offline(input_filename, errbuf);
    }
    
    if (pcap_handle == nullptr) {
        unlink(input_filename);
        unlink(output_filename);
        return 0;
    }
    
    // Step 2: Open dump file for writing
    // Test both standard open and append modes based on configuration
    if (test_append_mode) {
        // First create an empty valid pcap file for append testing
        pcap_dumper_t* temp_dumper = pcap_dump_open(pcap_handle, output_filename);
        if (temp_dumper != nullptr) {
            pcap_dump_close(temp_dumper);
            
            // Now append to the existing valid pcap file
            dumper = pcap_dump_open_append(pcap_handle, output_filename);
        } else {
            // Fallback to standard open if append setup failed
            dumper = pcap_dump_open(pcap_handle, output_filename);
        }
    } else {
        dumper = pcap_dump_open(pcap_handle, output_filename);
    }
    
    if (dumper == nullptr) {
        // Try alternative: pcap_dump_fopen with FILE*
        FILE* dump_fp = fopen(output_filename, test_append_mode ? "ab" : "wb");
        if (dump_fp != nullptr) {
            dumper = pcap_dump_fopen(pcap_handle, dump_fp);
            if (dumper == nullptr) {
                fclose(dump_fp);
            }
        }
    }
    
    if (dumper == nullptr) {
        pcap_close(pcap_handle);
        unlink(input_filename);
        unlink(output_filename);
        return 0;
    }
    
    // Step 3: Test auxiliary dump functions
    FILE* dump_file = pcap_dump_file(dumper);
    (void)dump_file;  // Use result to avoid unused variable warning
    
    long ftell_pos = pcap_dump_ftell(dumper);
    (void)ftell_pos;
    
    #ifdef pcap_dump_ftell64
    int64_t ftell64_pos = pcap_dump_ftell64(dumper);
    (void)ftell64_pos;
    #endif
    
    // Step 4: Read packets from input file and write to dump file
    // This is the key differentiation - using pcap_next_ex to get real packets
    struct pcap_pkthdr* packet_header_ptr;
    const u_char* packet_data_ptr;
    int packet_count = 0;
    int read_result;
    
    while (packet_count < max_packets_to_read) {
        read_result = pcap_next_ex(pcap_handle, &packet_header_ptr, &packet_data_ptr);
        
        if (read_result == 1) {
            // Valid packet retrieved
            pcap_dump((u_char*)dumper, packet_header_ptr, packet_data_ptr);
            packet_count++;
            
            // Test flush operations intermittently
            if (test_flush_operations && (packet_count % 5 == 0)) {
                pcap_dump_flush(dumper);
            }
        } else if (read_result == 0) {
            // Timeout (shouldn't happen with offline files)
            break;
        } else if (read_result == -1) {
            // Error or end of file
            break;
        } else if (read_result == -2) {
            // Breakloop called (shouldn't happen here)
            break;
        }
    }
    
    // Final flush if requested
    if (test_flush_operations) {
        pcap_dump_flush(dumper);
    }
    
    // Step 5: Test append mode with additional packets if configured
    if (test_append_mode && packet_count > 0) {
        // Close current dumper
        pcap_dump_close(dumper);
        dumper = nullptr;
        
        // Reopen the same output file in append mode
        dumper = pcap_dump_open_append(pcap_handle, output_filename);
        
        if (dumper != nullptr) {
            // Try to read and append a few more packets
            int append_count = 0;
            while (append_count < 5) {
                read_result = pcap_next_ex(pcap_handle, &packet_header_ptr, &packet_data_ptr);
                if (read_result == 1) {
                    pcap_dump((u_char*)dumper, packet_header_ptr, packet_data_ptr);
                    append_count++;
                } else {
                    break;
                }
            }
            
            if (test_flush_operations) {
                pcap_dump_flush(dumper);
            }
        }
    }
    
    // Step 6: Cleanup
    if (dumper != nullptr) {
        pcap_dump_close(dumper);
    }
    
    pcap_close(pcap_handle);
    
    // Clean up temporary files
    unlink(input_filename);
    unlink(output_filename);
    
    return 0;
}

} // namespace harness_016
