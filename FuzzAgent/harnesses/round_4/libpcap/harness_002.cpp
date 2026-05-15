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
#include <sys/stat.h>
#include <pcap/pcap.h>

namespace harness_002 {


// Helper function to write buffer to a temporary file (similar to existing fuzz harnesses)
static int bufferToFile(const char *name, const uint8_t *Data, size_t Size) {
    FILE *fd;
    if (remove(name) != 0) {
        if (errno != ENOENT) {
            return -1;
        }
    }
    fd = fopen(name, "wb");
    if (fd == NULL) {
        return -2;
    }
    if (fwrite(Data, 1, Size, fd) != Size) {
        fclose(fd);
        return -3;
    }
    fclose(fd);
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need enough data for meaningful testing
    // Minimum: 2 bytes for scenario + 1 byte for linktype + some pcap data
    if (Size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: scenario selector (0-2)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Second byte: link type for pcap_open_dead (used when no valid pcap file)
    int linktype = fdp.ConsumeIntegral<uint8_t>();
    
    // Third byte: tstamp precision (0 = microsecond, 1 = nanosecond)
    int tstamp_precision = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Fourth byte: operation selector for dump mode
    uint8_t dump_mode = fdp.ConsumeIntegral<uint8_t>() % 3; // 0=open, 1=append, 2=fopen
    
    // Consume some bytes for temporary filenames
    std::string input_filename = "/tmp/libpcap_fuzz_input_" + 
                                 std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".pcap";
    std::string output_filename = "/tmp/libpcap_fuzz_output_" + 
                                  std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".pcap";
    
    // Consume remaining bytes for pcap file data
    std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    pcap_t *pcap_handle = nullptr;
    pcap_dumper_t *dumper = nullptr;
    FILE *dump_file = nullptr;
    
    // Create input pcap file from consumed data
    if (bufferToFile(input_filename.c_str(), pcap_data.data(), pcap_data.size()) != 0) {
        // Clean up temporary files if they exist
        remove(input_filename.c_str());
        remove(output_filename.c_str());
        return 0;
    }
    
    // Try to open the input pcap file with appropriate timestamp precision
    if (tstamp_precision == 0) {
        pcap_handle = pcap_open_offline(input_filename.c_str(), errbuf);
    } else {
        pcap_handle = pcap_open_offline_with_tstamp_precision(input_filename.c_str(), 
                                                              PCAP_TSTAMP_PRECISION_NANO, 
                                                              errbuf);
    }
    
    // If we couldn't open a valid pcap file, create a dead pcap handle for testing
    if (pcap_handle == nullptr) {
        pcap_handle = pcap_open_dead(linktype, 65535);
        if (pcap_handle == nullptr) {
            remove(input_filename.c_str());
            remove(output_filename.c_str());
            return 0;
        }
    }
    
    // Now test different dump scenarios based on scenario selector
    switch (scenario) {
        case 0: {
            // Scenario 0: Test pcap_dump_open and pcap_dump_open_append
            if (dump_mode == 0) {
                dumper = pcap_dump_open(pcap_handle, output_filename.c_str());
            } else if (dump_mode == 1) {
                dumper = pcap_dump_open_append(pcap_handle, output_filename.c_str());
            } else {
                // Try to open with fopen first, then fall back to dump_open
                dump_file = fopen(output_filename.c_str(), "wb");
                if (dump_file != nullptr) {
                    dumper = pcap_dump_fopen(pcap_handle, dump_file);
                } else {
                    dumper = pcap_dump_open(pcap_handle, output_filename.c_str());
                }
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test pcap_dump_fopen alternative path
            // First create the file, then use pcap_dump_fopen
            dump_file = fopen(output_filename.c_str(), "wb");
            if (dump_file != nullptr) {
                dumper = pcap_dump_fopen(pcap_handle, dump_file);
            } else {
                // Fall back to regular dump_open
                dumper = pcap_dump_open(pcap_handle, output_filename.c_str());
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test append mode after creating initial file
            // First create file with dump_open, then close and try append
            dumper = pcap_dump_open(pcap_handle, output_filename.c_str());
            if (dumper != nullptr) {
                // Write a dummy packet header to create valid pcap file
                struct pcap_pkthdr dummy_header;
                memset(&dummy_header, 0, sizeof(dummy_header));
                dummy_header.caplen = 0;
                dummy_header.len = 0;
                gettimeofday(&dummy_header.ts, nullptr);
                pcap_dump((u_char *)dumper, &dummy_header, nullptr);
                
                // Flush and close
                pcap_dump_flush(dumper);
                pcap_dump_close(dumper);
                dumper = nullptr;
                
                // Now try to append
                dumper = pcap_dump_open_append(pcap_handle, output_filename.c_str());
            }
            break;
        }
    }
    
    // If we got a dumper, test dump operations
    if (dumper != nullptr) {
        // Test pcap_dump_file to get the FILE pointer
        FILE *dumper_file = pcap_dump_file(dumper);
        
        // Test pcap_dump_ftell64 to get file position
        int64_t file_pos = pcap_dump_ftell64(dumper);
        
        // Create some dummy packet data to dump
        struct pcap_pkthdr packet_header;
        memset(&packet_header, 0, sizeof(packet_header));
        
        // Use some consumed data for packet parameters
        packet_header.caplen = fdp.ConsumeIntegralInRange<uint32_t>(0, 1500);
        packet_header.len = packet_header.caplen;
        
        // Get current time for timestamp
        gettimeofday(&packet_header.ts, nullptr);
        
        // Create dummy packet data
        std::vector<u_char> packet_data(packet_header.caplen, 0);
        
        // Fill with some pattern if we have data
        for (size_t i = 0; i < packet_data.size() && i < pcap_data.size(); i++) {
            packet_data[i] = pcap_data[i % pcap_data.size()];
        }
        
        // Dump the packet
        pcap_dump((u_char *)dumper, &packet_header, packet_data.data());
        
        // Flush the dumper
        pcap_dump_flush(dumper);
        
        // Get file position again
        int64_t new_file_pos = pcap_dump_ftell64(dumper);
        
        // Close the dumper
        pcap_dump_close(dumper);
        
        // If we opened a separate FILE*, close it
        if (dump_file != nullptr && dump_file != dumper_file) {
            fclose(dump_file);
        }
    }
    
    // Close the pcap handle
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
    }
    
    // Clean up temporary files
    remove(input_filename.c_str());
    remove(output_filename.c_str());
    
    return 0;
}

} // namespace harness_002
