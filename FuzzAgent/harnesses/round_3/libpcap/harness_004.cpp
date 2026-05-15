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

namespace harness_004 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);  // DLT value
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 4);  // Which test to run
    int packet_count = fdp.ConsumeIntegralInRange<int>(1, 10);  // Number of packets to generate
    
    // Consume file name string (used for some tests)
    size_t filename_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string filename = fdp.ConsumeBytesAsString(filename_size);
    
    // Create temporary file name for testing
    char temp_filename[] = "/tmp/pcap_dump_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    close(fd);  // Close, we'll reopen with pcap functions
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = nullptr;
    pcap_dumper_t* dumper = nullptr;
    
    // Test different scenarios based on test_type
    switch (test_type) {
        case 0: {
            // Test 1: Basic pcap_dump_open with pcap_open_dead
            pcap = pcap_open_dead(linktype, snaplen);
            if (pcap == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            
            dumper = pcap_dump_open(pcap, temp_filename);
            break;
        }
            
        case 1: {
            // Test 2: pcap_dump_open_append (append mode)
            pcap = pcap_open_dead(linktype, snaplen);
            if (pcap == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            
            dumper = pcap_dump_open_append(pcap, temp_filename);
            break;
        }
            
        case 2: {
            // Test 3: pcap_dump_fopen with FILE* stream
            pcap = pcap_open_dead(linktype, snaplen);
            if (pcap == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            
            // Open a file for writing
            FILE* f = fopen(temp_filename, "wb");
            if (f == nullptr) {
                pcap_close(pcap);
                unlink(temp_filename);
                return 0;
            }
            
            dumper = pcap_dump_fopen(pcap, f);
            // Note: pcap_dump_fopen takes ownership of the FILE*
            break;
        }
            
        case 3: {
            // Test 4: Create a pcap file first using fuzzer data, then append to it
            pcap = pcap_open_dead(linktype, snaplen);
            if (pcap == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            
            // First create a file with some data from fuzzer input
            pcap_dumper_t* temp_dumper = pcap_dump_open(pcap, temp_filename);
            if (temp_dumper != nullptr) {
                // Write a packet using fuzzer data
                if (fdp.remaining_bytes() >= 20) {
                    struct pcap_pkthdr dummy_header;
                    dummy_header.ts.tv_sec = fdp.ConsumeIntegral<uint32_t>();
                    dummy_header.ts.tv_usec = fdp.ConsumeIntegral<uint32_t>();
                    dummy_header.caplen = fdp.ConsumeIntegralInRange<uint32_t>(14, 256);
                    dummy_header.len = dummy_header.caplen;
                    
                    if (dummy_header.caplen <= fdp.remaining_bytes()) {
                        std::vector<uint8_t> dummy_data = fdp.ConsumeBytes<uint8_t>(dummy_header.caplen);
                        pcap_dump((u_char*)temp_dumper, &dummy_header, dummy_data.data());
                    }
                }
                pcap_dump_close(temp_dumper);
            }
            
            // Now try to append
            dumper = pcap_dump_open_append(pcap, temp_filename);
            break;
        }
            
        case 4: {
            // Test 5: Test with stdout (special case)
            pcap = pcap_open_dead(linktype, snaplen);
            if (pcap == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            
            // pcap_dump_open with "-" should write to stdout
            dumper = pcap_dump_open(pcap, "-");
            break;
        }
    }
    
    // If we successfully created a dumper, test dumping operations
    if (dumper != nullptr) {
        // Generate and write some packets using remaining fuzzer data
        size_t remaining = fdp.remaining_bytes();
        if (remaining > 0) {
            // Split remaining data into packets
            size_t avg_packet_size = remaining / packet_count;
            if (avg_packet_size < 14) {  // Minimum Ethernet frame size
                avg_packet_size = 14;
            }
            
            for (int i = 0; i < packet_count && fdp.remaining_bytes() > 0; i++) {
                // Create packet header
                struct pcap_pkthdr header;
                header.ts.tv_sec = fdp.ConsumeIntegral<uint32_t>();
                header.ts.tv_usec = fdp.ConsumeIntegral<uint32_t>();
                
                // Determine packet size (capped at remaining bytes and reasonable maximum)
                size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(14, avg_packet_size * 2);
                if (packet_size > fdp.remaining_bytes()) {
                    packet_size = fdp.remaining_bytes();
                }
                
                header.caplen = packet_size;
                header.len = packet_size;
                
                // Get packet data
                std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
                
                // Write packet using pcap_dump
                pcap_dump((u_char*)dumper, &header, packet_data.data());
                
                // Occasionally test pcap_dump_flush
                if (i % 3 == 0) {
                    pcap_dump_flush(dumper);
                }
            }
            
            // Test pcap_dump_ftell and pcap_dump_ftell64
            long ftell_pos = pcap_dump_ftell(dumper);
            (void)ftell_pos;  // Use to avoid unused variable warning
            
            int64_t ftell64_pos = pcap_dump_ftell64(dumper);
            (void)ftell64_pos;
            
            // Test pcap_dump_file to get the FILE* handle
            FILE* dump_file = pcap_dump_file(dumper);
            (void)dump_file;
            
            // Final flush
            pcap_dump_flush(dumper);
        }
        
        // Close the dumper
        pcap_dump_close(dumper);
    }
    
    // Clean up pcap handle if it was created
    if (pcap != nullptr) {
        pcap_close(pcap);
    }
    
    // Clean up temporary file (unless we wrote to stdout)
    if (test_type != 4) {  // Not stdout test
        unlink(temp_filename);
    }
    
    // Remove intentional NULL pointer tests that cause crashes
    // These were testing invalid inputs and causing harness misuse issues
    
    return 0;
}

} // namespace harness_004
