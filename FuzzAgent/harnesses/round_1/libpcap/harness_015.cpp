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

namespace harness_015 {
/*
 * libpcap fuzzing harness - harness_015.cpp
 * Specialized targeting of pcap_dump_open_append with existing valid pcap files
 * 
 * Critical Coverage Gap Identified:
 * - pcap_dump_open_append: 56 undiscovered branches (HIGHEST VALUE)
 * These APIs are reached by fuzzers but have 0% execution coverage due to missing preconditions.
 * 
 * Analysis of existing harnesses:
 * - harness_002: Tests basic pcap_dump_open and pcap_dump_open_append but doesn't create valid pcap files for append
 * - harness_011: Tests dump write operations but doesn't focus on append with existing files
 * - harness_014: Comprehensive dump operations but likely fails append due to missing valid pcap headers
 * 
 * Root cause: pcap_dump_open_append requires existing files with valid pcap headers to work properly.
 * It reads the header to determine if file is empty or has existing packets.
 * 
 * Strategy for harness_015:
 * 1. Create a valid pcap file with proper header first (using pcap_dump_open)
 * 2. Test pcap_dump_open_append on that existing valid file
 * 3. Focus on error paths and edge cases specific to append mode
 * 4. Test different scenarios: empty file, file with packets, truncated file, invalid header
 * 
 * This harness specifically addresses the 56 undiscovered branches in pcap_dump_open_append
 * by creating the proper preconditions that existing harnesses miss.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters and packet data
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int timestamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=micro, 1=nano
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 5);  // Different test scenarios
    int num_initial_packets = fdp.ConsumeIntegralInRange<int>(0, 10);
    int num_append_packets = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // Create temporary output file for dump operations
    char output_filename[] = "/tmp/libpcap_fuzz_append_XXXXXX";
    int output_fd = mkstemp(output_filename);
    if (output_fd < 0) {
        return 0;
    }
    close(output_fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    pcap_dumper_t* initial_dumper = nullptr;
    pcap_dumper_t* append_dumper = nullptr;
    
    // Step 1: Create pcap handle with timestamp precision
    pcap_handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, timestamp_precision);
    if (pcap_handle == nullptr) {
        unlink(output_filename);
        return 0;
    }
    
    // Step 2: Test different append scenarios based on test_scenario
    bool append_successful = false;
    
    switch (test_scenario) {
        case 0: { // Scenario 1: Create empty pcap file then append
            // First create an empty pcap file with valid header using pcap_dump_open
            initial_dumper = pcap_dump_open(pcap_handle, output_filename);
            if (initial_dumper != nullptr) {
                // Write no packets - just create header
                pcap_dump_close(initial_dumper);
                initial_dumper = nullptr;
                
                // Now append to the empty but valid pcap file
                append_dumper = pcap_dump_open_append(pcap_handle, output_filename);
                append_successful = (append_dumper != nullptr);
            }
            break;
        }
            
        case 1: { // Scenario 2: Create pcap file with packets then append
            // Create pcap file with some initial packets
            initial_dumper = pcap_dump_open(pcap_handle, output_filename);
            if (initial_dumper != nullptr) {
                // Write some initial packets
                for (int i = 0; i < num_initial_packets && fdp.remaining_bytes() >= 20; i++) {
                    uint32_t tv_sec = fdp.ConsumeIntegral<uint32_t>();
                    uint32_t tv_usec = fdp.ConsumeIntegral<uint32_t>();
                    size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(0, 
                        fdp.remaining_bytes() > 1024 ? 1024 : fdp.remaining_bytes());
                    
                    struct pcap_pkthdr header;
                    header.ts.tv_sec = tv_sec;
                    header.ts.tv_usec = tv_usec;
                    header.caplen = packet_size;
                    header.len = packet_size;
                    
                    if (packet_size > 0) {
                        std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
                        pcap_dump(reinterpret_cast<u_char*>(initial_dumper), &header, packet_data.data());
                    } else {
                        pcap_dump(reinterpret_cast<u_char*>(initial_dumper), &header, nullptr);
                    }
                }
                
                pcap_dump_close(initial_dumper);
                initial_dumper = nullptr;
                
                // Now append to the file with existing packets
                append_dumper = pcap_dump_open_append(pcap_handle, output_filename);
                append_successful = (append_dumper != nullptr);
            }
            break;
        }
            
        case 2: { // Scenario 3: Test pcap_dump_fopen then append
            // First create file using pcap_dump_fopen
            FILE* initial_fp = fopen(output_filename, "wb");
            if (initial_fp != nullptr) {
                initial_dumper = pcap_dump_fopen(pcap_handle, initial_fp);
                if (initial_dumper != nullptr) {
                    // Write a header by closing immediately
                    pcap_dump_close(initial_dumper);
                    initial_dumper = nullptr;
                    
                    // Now try to append
                    append_dumper = pcap_dump_open_append(pcap_handle, output_filename);
                    append_successful = (append_dumper != nullptr);
                } else {
                    fclose(initial_fp);
                }
            }
            break;
        }
            
        case 3: { // Scenario 4: Test append to non-existent file (should create new file)
            // Don't create file first - let pcap_dump_open_append handle it
            append_dumper = pcap_dump_open_append(pcap_handle, output_filename);
            append_successful = (append_dumper != nullptr);
            break;
        }
            
        case 4: { // Scenario 5: Test error paths - append to file with invalid header
            // Create a file with invalid content (not a valid pcap header)
            FILE* invalid_fp = fopen(output_filename, "wb");
            if (invalid_fp != nullptr) {
                // Write some garbage data
                std::vector<uint8_t> garbage = fdp.ConsumeBytes<uint8_t>(32);
                fwrite(garbage.data(), 1, garbage.size(), invalid_fp);
                fclose(invalid_fp);
                
                // Try to append - should fail due to invalid header
                append_dumper = pcap_dump_open_append(pcap_handle, output_filename);
                append_successful = (append_dumper != nullptr);
            }
            break;
        }
            
        case 5: { // Scenario 6: Test append with "-" filename (stdout)
            // Special case: "-" means stdout
            append_dumper = pcap_dump_open_append(pcap_handle, "-");
            append_successful = (append_dumper != nullptr);
            break;
        }
    }
    
    // Step 3: If append was successful, write some packets in append mode
    if (append_successful && append_dumper != nullptr) {
        // Write packets in append mode
        for (int i = 0; i < num_append_packets && fdp.remaining_bytes() >= 20; i++) {
            uint32_t tv_sec = fdp.ConsumeIntegral<uint32_t>();
            uint32_t tv_usec = fdp.ConsumeIntegral<uint32_t>();
            size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(0, 
                fdp.remaining_bytes() > 1024 ? 1024 : fdp.remaining_bytes());
            
            struct pcap_pkthdr header;
            header.ts.tv_sec = tv_sec;
            header.ts.tv_usec = tv_usec;
            header.caplen = packet_size;
            header.len = packet_size;
            
            if (packet_size > 0) {
                std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
                pcap_dump(reinterpret_cast<u_char*>(append_dumper), &header, packet_data.data());
            } else {
                pcap_dump(reinterpret_cast<u_char*>(append_dumper), &header, nullptr);
            }
            
            // Test pcap_dump_flush occasionally
            if (i % 5 == 0) {
                pcap_dump_flush(append_dumper);
            }
            
            // Test pcap_dump_ftell
            long ftell_pos = pcap_dump_ftell(append_dumper);
            (void)ftell_pos;
            
            #ifdef pcap_dump_ftell64
            int64_t ftell64_pos = pcap_dump_ftell64(append_dumper);
            (void)ftell64_pos;
            #endif
        }
        
        // Test pcap_dump_file
        FILE* dump_file = pcap_dump_file(append_dumper);
        (void)dump_file;
        
        // Final flush
        pcap_dump_flush(append_dumper);
        
        // Close append dumper
        pcap_dump_close(append_dumper);
    }
    
    // Step 4: Cleanup
    if (initial_dumper != nullptr) {
        pcap_dump_close(initial_dumper);
    }
    
    pcap_close(pcap_handle);
    
    // Clean up temporary file (except for stdout case)
    if (test_scenario != 5) {
        unlink(output_filename);
    }
    
    return 0;
}

} // namespace harness_015
