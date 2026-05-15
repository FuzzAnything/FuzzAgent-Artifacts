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

namespace harness_014 {
/*
 * libpcap fuzzing harness - harness_014.cpp
 * Comprehensive targeting of pcap dump file operations in sf-pcap.c module
 * 
 * Critical Coverage Gap Identified:
 * - pcap_dump_open: 12 branches, 100% undiscovered (0% function lines hit)
 * - pcap_dump_open_append: 56 branches, 100% undiscovered (0% function lines hit)
 * - pcap_dump: 4 branches, 100% undiscovered (0% function lines hit)
 * - pcap_dump_flush: 2 branches, 100% undiscovered (0% function lines hit)
 * - pcap_dump_close: 2 branches, 100% undiscovered (0% function lines hit)
 * - pcap_dump_fopen: 2 branches, 100% undiscovered (0% function lines hit)
 * 
 * Differentiates from existing harnesses:
 * - harness_002: General file I/O lifecycle operations
 * - harness_004: Savefile APIs with timestamp precision (reading focused)
 * - harness_011: Savefile write subsystem with format-specific dumping (partial coverage)
 * - harness_013: Memory-based FILE* testing of internal savefile parsing
 * - harness_014: COMPREHENSIVE dump file operations targeting ALL sf-pcap.c APIs (THIS HARNESS)
 * 
 * Strategy:
 * 1. Test both pcap_open_dead() and pcap_open_dead_with_tstamp_precision()
 * 2. Test all dump opening variants: pcap_dump_open(), pcap_dump_open_append(), pcap_dump_fopen()
 * 3. Write diverse packet data with varying timestamp precisions
 * 4. Test all auxiliary functions: pcap_dump_flush(), pcap_dump_ftell(), pcap_dump_ftell64()
 * 5. Test error handling paths and edge cases
 * 6. Ensure proper cleanup with pcap_dump_close() and pcap_close()
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int use_timestamp_precision = fdp.ConsumeBool() ? 1 : 0;
    int test_variant = fdp.ConsumeIntegralInRange<int>(0, 5);  // Which API variant to test
    int use_append_mode = fdp.ConsumeBool() ? 1 : 0;
    int test_flush_operations = fdp.ConsumeBool() ? 1 : 0;
    int test_ftell_operations = fdp.ConsumeBool() ? 1 : 0;
    int num_packets = fdp.ConsumeIntegralInRange<int>(0, 100);
    
    // Create temporary output file for dump operations
    char output_filename[] = "/tmp/libpcap_fuzz_dump_014_XXXXXX";
    int output_fd = mkstemp(output_filename);
    if (output_fd < 0) {
        return 0;
    }
    close(output_fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    pcap_dumper_t* dumper = nullptr;
    
    // Step 1: Initialize pcap handle with different precision variants
    if (use_timestamp_precision) {
        // Use pcap_open_dead_with_tstamp_precision with various precisions
        int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=PCAP_TSTAMP_PRECISION_MICRO, 1=PCAP_TSTAMP_PRECISION_NANO
        pcap_handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, tstamp_precision);
    } else {
        // Standard pcap_open_dead
        pcap_handle = pcap_open_dead(linktype, snaplen);
    }
    
    if (pcap_handle == nullptr) {
        unlink(output_filename);
        return 0;
    }
    
    // Step 2: Test different dump opening variants
    switch (test_variant) {
        case 0:  // Standard pcap_dump_open
            if (use_append_mode) {
                dumper = pcap_dump_open_append(pcap_handle, output_filename);
            } else {
                dumper = pcap_dump_open(pcap_handle, output_filename);
            }
            break;
            
        case 1:  // pcap_dump_fopen with FILE*
            {
                FILE* dump_fp = fopen(output_filename, use_append_mode ? "ab" : "wb");
                if (dump_fp != nullptr) {
                    dumper = pcap_dump_fopen(pcap_handle, dump_fp);
                    if (dumper == nullptr) {
                        // pcap_dump_fopen failed, close the FILE* ourselves
                        fclose(dump_fp);
                    }
                }
            }
            break;
            
        case 2:  // Test with stdout (special file handle)
            {
                FILE* stdout_dup = fdopen(dup(STDOUT_FILENO), "wb");
                if (stdout_dup != nullptr) {
                    dumper = pcap_dump_fopen(pcap_handle, stdout_dup);
                    // Don't close stdout_dup - it's stdout
                }
            }
            break;
            
        case 3:  // Test with stderr
            {
                FILE* stderr_dup = fdopen(dup(STDERR_FILENO), "wb");
                if (stderr_dup != nullptr) {
                    dumper = pcap_dump_fopen(pcap_handle, stderr_dup);
                    // Don't close stderr_dup - it's stderr
                }
            }
            break;
            
        case 4:  // Test with /dev/null
            {
                FILE* dev_null = fopen("/dev/null", "wb");
                if (dev_null != nullptr) {
                    dumper = pcap_dump_fopen(pcap_handle, dev_null);
                    if (dumper == nullptr) {
                        fclose(dev_null);
                    }
                }
            }
            break;
            
        case 5:  // Test with invalid filename (error path testing)
            dumper = pcap_dump_open(pcap_handle, "/nonexistent/path/to/file.pcap");
            break;
    }
    // If dumper creation failed (except for case 5 which tests error paths), cleanup and exit
    if (dumper == nullptr && test_variant != 5) {
        pcap_close(pcap_handle);
        unlink(output_filename);
        return 0;
    }
    
    // Step 3: Test pcap_dump_file() to get FILE* from dumper
    FILE* dump_file = nullptr;
    if (dumper != nullptr) {
        dump_file = pcap_dump_file(dumper);
        (void)dump_file;  // Use to avoid unused variable warning
    }
    // Step 4: Write diverse packet data
    int packets_written = 0;
    if (dumper != nullptr && num_packets > 0) {
        for (int i = 0; i < num_packets && fdp.remaining_bytes() >= 16; i++) {
            // Consume packet header parameters from fuzzer input
            uint32_t tv_sec = fdp.ConsumeIntegral<uint32_t>();
            uint32_t tv_usec = fdp.ConsumeIntegral<uint32_t>();
            
            // Determine packet size
            size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(0, 
                fdp.remaining_bytes() > 2048 ? 2048 : fdp.remaining_bytes());
            
            if (packet_size == 0 && fdp.remaining_bytes() == 0) {
                break;  // No more data
            }
            
            std::vector<uint8_t> packet_data;
            if (packet_size > 0) {
                packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
            } else {
                // Create empty packet
                packet_data = std::vector<uint8_t>();
            }
            
            // Create packet header
            struct pcap_pkthdr header;
            header.ts.tv_sec = tv_sec;
            header.ts.tv_usec = tv_usec;
            header.caplen = packet_data.size();
            header.len = packet_data.size();
            
            // Write packet using pcap_dump()
            if (!packet_data.empty()) {
                pcap_dump(reinterpret_cast<u_char*>(dumper), &header, packet_data.data());
                packets_written++;
            } else {
                // Test with zero-length packet
                pcap_dump(reinterpret_cast<u_char*>(dumper), &header, nullptr);
                packets_written++;
            }
            
            // Test pcap_dump_ftell after each write (if enabled)
            if (test_ftell_operations) {
                long ftell_pos = pcap_dump_ftell(dumper);
                (void)ftell_pos;
                
                // Test 64-bit version if available
                #ifdef pcap_dump_ftell64
                int64_t ftell64_pos = pcap_dump_ftell64(dumper);
                (void)ftell64_pos;
                #endif
            }
            
            // Test pcap_dump_flush periodically (if enabled)
            if (test_flush_operations && (i % 10 == 0)) {
                int flush_result = pcap_dump_flush(dumper);
                (void)flush_result;
            }
        }
    }
    
    // Step 5: Test additional operations after writing
    if (dumper != nullptr) {
        // Final flush test
        if (test_flush_operations) {
            int flush_result = pcap_dump_flush(dumper);
            (void)flush_result;
        }
        
        // Final ftell test
        if (test_ftell_operations) {
            long final_ftell = pcap_dump_ftell(dumper);
            (void)final_ftell;
            
            #ifdef pcap_dump_ftell64
            int64_t final_ftell64 = pcap_dump_ftell64(dumper);
            (void)final_ftell64;
            #endif
        }
        
        // Step 6: Cleanup
        pcap_dump_close(dumper);
    }
    
    // Cleanup pcap handle
    pcap_close(pcap_handle);
    
    // Clean up temporary file (except for special cases)
    if (test_variant != 2 && test_variant != 3 && test_variant != 4) {
        unlink(output_filename);
    }
    
    return 0;
}

} // namespace harness_014
