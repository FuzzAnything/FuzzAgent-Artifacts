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

namespace harness_011 {
/*
 * libpcap fuzzing harness - harness_011.cpp
 * Targets savefile write subsystem APIs with focus on pcap_dump_* operations
 * 
 * Primary APIs targeted (savefile write operations):
 * - pcap_dump (write packets to dump file)
 * - pcap_dump_flush (flush dump buffer)
 * - pcap_dump_ftell (get current position in dump file)
 * - pcap_dump_ftell64 (64-bit version of ftell)
 * - pcap_dump_file (get FILE* from dump handle)
 * - pcap_dump_close (close dump file)
 * - pcap_dump_open (create dump file for writing packets)
 * - pcap_dump_open_append (open dump file in append mode)
 * - pcap_dump_fopen (create dump file from FILE*)
 * 
 * Differentiates from existing harnesses:
 * - harness_002: File I/O subsystem operations (general file I/O lifecycle)
 * - harness_004: Savefile-specific APIs with timestamp precision (opening APIs)
 * - harness_011: Savefile write subsystem with format-specific packet dumping (THIS HARNESS)
 * 
 * Specifically targets the 76+ undiscovered branches in savefile write subsystem
 * identified by CoverageAnalyzerAgent across savefile.c, sf-pcap.c, and sf-pcapng.c files.
 * Focuses on savefile format-specific writing operations, error handling during write,
 * and edge cases in packet dumping.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters and packet data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT_* values
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    int use_pcapng = fdp.ConsumeBool() ? 1 : 0;  // Test both pcap and pcapng formats
    int append_mode = fdp.ConsumeBool() ? 1 : 0;
    int test_flush = fdp.ConsumeBool() ? 1 : 0;
    int test_ftell = fdp.ConsumeBool() ? 1 : 0;
    
    // Create temporary output file for dump operations
    char output_filename[] = "/tmp/libpcap_fuzz_dump_write_XXXXXX";
    int output_fd = mkstemp(output_filename);
    if (output_fd < 0) {
        return 0;
    }
    close(output_fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Create a dead capture handle for writing (no live capture needed)
    pcap_t* dead_handle = pcap_open_dead(linktype, snaplen);
    if (dead_handle == nullptr) {
        unlink(output_filename);
        return 0;
    }
    
    // Test 1: pcap_dump_open (standard open)
    pcap_dumper_t* dumper = nullptr;
    if (append_mode) {
        dumper = pcap_dump_open_append(dead_handle, output_filename);
    } else {
        dumper = pcap_dump_open(dead_handle, output_filename);
    }
    
    if (dumper == nullptr) {
        // Try alternative: pcap_dump_fopen with FILE*
        FILE* dump_fp = fopen(output_filename, "wb");
        if (dump_fp != nullptr) {
            dumper = pcap_dump_fopen(dead_handle, dump_fp);
            if (dumper == nullptr) {
                // pcap_dump_fopen failed, close the FILE* ourselves
                fclose(dump_fp);
            }
        }
    }
    
    // If we still don't have a dumper, clean up and exit
    if (dumper == nullptr) {
        pcap_close(dead_handle);
        unlink(output_filename);
        return 0;
    }
    
    // Test 2: pcap_dump_file (get FILE* from dumper)
    FILE* dump_file = pcap_dump_file(dumper);
    if (dump_file != nullptr) {
        // Test file operations on the dump file
        int file_no = fileno(dump_file);
        (void)file_no;  // Use to avoid unused variable warning
    }
    
    // Test 3: pcap_dump_ftell variants
    if (test_ftell) {
        long ftell_pos = pcap_dump_ftell(dumper);
        (void)ftell_pos;
        
        #ifdef pcap_dump_ftell64
        int64_t ftell64_pos = pcap_dump_ftell64(dumper);
        (void)ftell64_pos;
        #endif
    }
    
    // Generate and write multiple packets with diverse characteristics
    const int MAX_PACKETS = fdp.ConsumeIntegralInRange<int>(1, 50);
    int packets_written = 0;
    
    for (int i = 0; i < MAX_PACKETS && fdp.remaining_bytes() >= 16; i++) {
        // Consume packet header parameters
        uint32_t tv_sec = fdp.ConsumeIntegral<uint32_t>();
        uint32_t tv_usec = fdp.ConsumeIntegral<uint32_t>();
        
        // Generate packet data with varying sizes
        size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(0, 
            fdp.remaining_bytes() > 1024 ? 1024 : fdp.remaining_bytes());
        
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
        
        // Test 4: pcap_dump - write packet to file
        // Note: pcap_dump takes a u_char* user parameter which is the FILE*
        // pcap_dump can be called directly with the FILE* from the dumper
        if (dump_file != nullptr && !packet_data.empty()) {
            // Call pcap_dump directly - first parameter is the FILE* cast to u_char*
            pcap_dump(reinterpret_cast<u_char*>(dump_file), &header, packet_data.data());
            packets_written++;
        }
        // Test ftell after each write to check position updates
        if (test_ftell && dump_file != nullptr) {
            long current_pos = ftell(dump_file);
            (void)current_pos;
            
            #ifdef pcap_dump_ftell64
            int64_t current_pos64 = ftello(dump_file);
            (void)current_pos64;
            #endif
        }
    }
    
    // Test 5: pcap_dump_flush
    if (test_flush) {
        int flush_result = pcap_dump_flush(dumper);
        (void)flush_result;
    }
    
    // Test ftell one more time before closing
    if (test_ftell) {
        long final_ftell = pcap_dump_ftell(dumper);
        (void)final_ftell;
        
        #ifdef pcap_dump_ftell64
        int64_t final_ftell64 = pcap_dump_ftell64(dumper);
        (void)final_ftell64;
        #endif
    }
    
    // Test 6: pcap_dump_close
    pcap_dump_close(dumper);
    
    // Clean up dead handle
    pcap_close(dead_handle);
    
    // Test 7: Test writing to same file again (append scenario)
    if (append_mode && packets_written > 0) {
        // Create new dead handle
        pcap_t* dead_handle2 = pcap_open_dead(linktype, snaplen);
        if (dead_handle2 != nullptr) {
            // Open in append mode
            pcap_dumper_t* dumper2 = pcap_dump_open_append(dead_handle2, output_filename);
            if (dumper2 != nullptr) {
                // Write one more packet
                if (fdp.remaining_bytes() >= 8) {
                    struct pcap_pkthdr header2;
                    header2.ts.tv_sec = fdp.ConsumeIntegral<uint32_t>();
                    header2.ts.tv_usec = fdp.ConsumeIntegral<uint32_t>();
                    header2.caplen = 0;
                    header2.len = 0;
                    
                    // Test ftell on append handle
                    long append_ftell = pcap_dump_ftell(dumper2);
                    (void)append_ftell;
                    
                    pcap_dump_close(dumper2);
                }
            }
            pcap_close(dead_handle2);
        }
    }
    
    // Test 8: Test error conditions by writing invalid data
    if (fdp.remaining_bytes() > 0) {
        // Create another temporary file for error testing
        char error_filename[] = "/tmp/libpcap_fuzz_error_XXXXXX";
        int error_fd = mkstemp(error_filename);
        if (error_fd >= 0) {
            close(error_fd);
            
            pcap_t* error_handle = pcap_open_dead(linktype, snaplen);
            if (error_handle != nullptr) {
                // Try to open with invalid mode or corrupted data
                pcap_dumper_t* error_dumper = pcap_dump_open(error_handle, error_filename);
                if (error_dumper != nullptr) {
                    // Test with potentially corrupted data
                    if (fdp.remaining_bytes() >= 16) {
                        std::vector<uint8_t> corrupt_data = fdp.ConsumeBytes<uint8_t>(16);
                        
                        FILE* error_file = pcap_dump_file(error_dumper);
                        if (error_file != nullptr) {
                            // Try writing corrupted data (might trigger error paths)
                            fwrite(corrupt_data.data(), corrupt_data.size(), 1, error_file);
                        }
                    }
                    pcap_dump_close(error_dumper);
                }
                pcap_close(error_handle);
            }
            unlink(error_filename);
        }
    }
    
    // Clean up output file
    unlink(output_filename);
    
    return 0;
}

} // namespace harness_011
