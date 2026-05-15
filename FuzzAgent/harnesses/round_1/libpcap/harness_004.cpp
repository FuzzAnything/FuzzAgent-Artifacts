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

namespace harness_004 {
/*
 * libpcap fuzzing harness - harness_004.cpp
 * Targets savefile subsystem APIs NOT covered by harness_002 (file I/O operations)
 * 
 * Primary APIs targeted (different from harness_002):
 * - pcap_open_dead (create dead capture handle)
 * - pcap_open_dead_with_tstamp_precision (dead handle with timestamp precision)
 * - pcap_fopen_offline (read from FILE* handle)
 * - pcap_open_offline_with_tstamp_precision (file with timestamp precision)
 * - pcap_dump_fopen (create dump file from FILE*)
 * - pcap_dump_open_append (open dump file in append mode)
 * - pcap_set_tstamp_precision (set timestamp precision)
 * - pcap_get_tstamp_precision (get timestamp precision)
 * 
 * Differentiates from existing harnesses:
 * - harness_000: Core packet capture APIs (uses basic pcap_open_dead)
 * - harness_001: BPF filter handling functions
 * - harness_002: File I/O subsystem operations (pcap_open_offline, pcap_dump_open, etc.)
 * - harness_003: Live device management lifecycle
 * - harness_004: Savefile-specific APIs with timestamp precision (THIS HARNESS)
 * 
 * Targets the 150+ undiscovered branches in savefile subsystem identified by CoverageAnalyzerAgent.
 * Specifically focuses on APIs NOT covered by harness_002 to avoid homogenization.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters and some file data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=micro, 1=nano
    int append_mode = fdp.ConsumeBool() ? 1 : 0;
    
    // Create temporary input file for file-based APIs
    char input_filename[] = "/tmp/libpcap_fuzz_savefile_XXXXXX";
    int input_fd = mkstemp(input_filename);
    if (input_fd < 0) {
        return 0;
    }
    
    // Write fuzzer data to input file (simulating pcap file)
    // Need at least 4 bytes for file data
    if (fdp.remaining_bytes() < 4) {
        close(input_fd);
        unlink(input_filename);
        return 0;
    }
    size_t file_data_size = fdp.ConsumeIntegralInRange<size_t>(4, fdp.remaining_bytes());
    std::vector<uint8_t> input_data = fdp.ConsumeBytes<uint8_t>(file_data_size);
    // ConsumeBytes may return empty vector if insufficient input remains
    if (!input_data.empty()) {
        if (write(input_fd, input_data.data(), input_data.size()) != static_cast<ssize_t>(input_data.size())) {
            close(input_fd);
            unlink(input_filename);
            return 0;
        }
    }
    close(input_fd);
    
    // Create temporary output file for dump APIs
    char output_filename[] = "/tmp/libpcap_fuzz_dump_XXXXXX";
    int output_fd = mkstemp(output_filename);
    if (output_fd < 0) {
        unlink(input_filename);
        return 0;
    }
    close(output_fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Test 1: Basic pcap_open_dead (different from harness_000 which uses it for filtering)
    pcap_t* dead_handle = pcap_open_dead(linktype, snaplen);
    if (dead_handle != nullptr) {
        // Test timestamp precision APIs on dead handle
        int set_precision_result = pcap_set_tstamp_precision(dead_handle, tstamp_precision);
        (void)set_precision_result;
        
        int get_precision_result = pcap_get_tstamp_precision(dead_handle);
        (void)get_precision_result;
        
        // Test 2: pcap_dump_fopen with FILE* (different from harness_002's pcap_dump_open)
        FILE* dump_fp = fopen(output_filename, "wb");
        if (dump_fp != nullptr) {
            pcap_dumper_t* dumper_fp = pcap_dump_fopen(dead_handle, dump_fp);
            if (dumper_fp != nullptr) {
                // Test pcap_dump_file to get FILE* back
                FILE* retrieved_fp = pcap_dump_file(dumper_fp);
                (void)retrieved_fp;
                
                // Test pcap_dump_ftell
                long ftell_pos = pcap_dump_ftell(dumper_fp);
                (void)ftell_pos;
                
                // Create packet data from fuzzer input
                // First check if we have enough remaining fuzzer data for packet
                size_t packet_data_size = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
                packet_data_size = std::min(packet_data_size, static_cast<size_t>(1024));  // Limit size
                
                if (packet_data_size > 0) {
                    std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_data_size);
                    
                    // Only call pcap_dump if we actually got packet data
                    // ConsumeBytes may return empty vector if insufficient input remains
                    if (!packet_data.empty()) {
                        // Create packet header from fuzzer data
                        struct pcap_pkthdr dummy_header;
                        dummy_header.ts.tv_sec = fdp.ConsumeIntegral<uint32_t>();
                        dummy_header.ts.tv_usec = fdp.ConsumeIntegral<uint32_t>() % 1000000;
                        dummy_header.caplen = std::min(packet_data.size(), static_cast<size_t>(snaplen));
                        dummy_header.len = packet_data.size();
                        
                        // Write packet using fuzzer data
                        pcap_dump(reinterpret_cast<u_char*>(dumper_fp), &dummy_header, packet_data.data());
                    }
                }
                // Flush and close the dumper if it was created
                pcap_dump_flush(dumper_fp);
                pcap_dump_close(dumper_fp);
                // DO NOT call fclose(dump_fp) - pcap_dump_close() already closed it
            } else {
                // pcap_dump_fopen failed, so we need to close the FILE* ourselves
                fclose(dump_fp);
            }
        }
        
        // Test 3: pcap_dump_open_append (different from harness_002's pcap_dump_open)
        pcap_dumper_t* dumper_append = pcap_dump_open_append(dead_handle, output_filename);
        if (dumper_append != nullptr) {
            // Test ftell on append handle
            long append_ftell = pcap_dump_ftell(dumper_append);
            (void)append_ftell;
            pcap_dump_close(dumper_append);
        }
        
        pcap_close(dead_handle);
    }
    
    // Test 4: pcap_open_dead_with_tstamp_precision (extended version not in harness_000)
    pcap_t* dead_precise_handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, tstamp_precision);
    if (dead_precise_handle != nullptr) {
        // Verify timestamp precision was set
        int precise_get_result = pcap_get_tstamp_precision(dead_precise_handle);
        (void)precise_get_result;
        pcap_close(dead_precise_handle);
    }
    
    // Test 5: pcap_fopen_offline (FILE* based, different from harness_002's pcap_open_offline)
    FILE* input_fp = fopen(input_filename, "rb");
    if (input_fp != nullptr) {
        pcap_t* fopen_handle = pcap_fopen_offline(input_fp, errbuf);
        if (fopen_handle != nullptr) {
            // Test timestamp precision on fopen handle
            int fopen_set_result = pcap_set_tstamp_precision(fopen_handle, tstamp_precision);
            (void)fopen_set_result;
            
            int fopen_get_result = pcap_get_tstamp_precision(fopen_handle);
            (void)fopen_get_result;
            
            // Try to read packets (may fail if file isn't valid pcap)
            struct pcap_pkthdr *header;
            const u_char* packet_data;
            int packet_count = 0;
            const int MAX_PACKETS = 10;
            
            while (packet_count < MAX_PACKETS) {
                int result = pcap_next_ex(fopen_handle, &header, &packet_data);
                if (result <= 0) {
                    break;
                }
                packet_count++;
            }
            pcap_close(fopen_handle);
            input_fp = nullptr;  // FILE* was closed by pcap_close
        } else {
            // Close the file pointer since pcap_fopen_offline failed
            fclose(input_fp);
            input_fp = nullptr;
        }
    }
    
    // Test 6: pcap_open_offline_with_tstamp_precision (with precision parameter)
    pcap_t* offline_precise_handle = pcap_open_offline_with_tstamp_precision(input_filename, tstamp_precision, errbuf);
    if (offline_precise_handle != nullptr) {
        // Verify precision was set
        int offline_get_result = pcap_get_tstamp_precision(offline_precise_handle);
        (void)offline_get_result;
        
        // Test dump operations with this handle
        pcap_dumper_t* precise_dumper = nullptr;
        if (append_mode) {
            precise_dumper = pcap_dump_open_append(offline_precise_handle, output_filename);
        } else {
            FILE* dump_fp2 = fopen(output_filename, "ab");
            if (dump_fp2 != nullptr) {
                precise_dumper = pcap_dump_fopen(offline_precise_handle, dump_fp2);
                if (precise_dumper == nullptr) {
                    // pcap_dump_fopen failed, close the FILE* ourselves
                    fclose(dump_fp2);
                }
            }
        }
        
        if (precise_dumper != nullptr) {
            // Test dump file operations
            FILE* precise_dump_file = pcap_dump_file(precise_dumper);

            
            // Test ftell variants
            long precise_ftell = pcap_dump_ftell(precise_dumper);
            (void)precise_ftell;
            
            #ifdef pcap_dump_ftell64
            int64_t precise_ftell64 = pcap_dump_ftell64(precise_dumper);
            (void)precise_ftell64;
            #endif
            
            pcap_dump_close(precise_dumper);
        }
        
        pcap_close(offline_precise_handle);
    }
    
    // Test 7: Regular pcap_open_offline for comparison (but with timestamp precision APIs)
    pcap_t* offline_handle = pcap_open_offline(input_filename, errbuf);
    if (offline_handle != nullptr) {
        // Try to set timestamp precision on regular offline handle
        int offline_set_result = pcap_set_tstamp_precision(offline_handle, tstamp_precision);
        (void)offline_set_result;
        
        int offline_get_result2 = pcap_get_tstamp_precision(offline_handle);
        (void)offline_get_result2;
        
        pcap_close(offline_handle);
    }
    
    // Clean up file pointers if they're still open
    if (input_fp != nullptr) {
        fclose(input_fp);
    }
    
    // Clean up temporary files
    unlink(input_filename);
    unlink(output_filename);
    
    return 0;
}

} // namespace harness_004
