#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace harness_002 {

// Simple packet handler function for dumping packets
static void pcap_dump_handler(u_char* dumper, const struct pcap_pkthdr* h, const u_char* bytes) {
    pcap_dump(dumper, h, bytes);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different savefile workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume timestamp precision parameter
    u_int tstamp_precision = fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO;
    
    // Consume filter string from fuzzer input
    std::string filter_str = fdp.ConsumeRandomLengthString(256);
    
    // Consume netmask from fuzzer input
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Consume optimize flag
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume remaining bytes for file data
    std::vector<uint8_t> file_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Create temporary filenames
    std::string input_filename = "/tmp/libpcap_fuzz_input_XXXXXX";
    std::string output_filename = "/tmp/libpcap_fuzz_output_XXXXXX";
    std::string append_filename = "/tmp/libpcap_fuzz_append_XXXXXX";
    
    // Create actual temporary files
    char input_tmp[256], output_tmp[256], append_tmp[256];
    strncpy(input_tmp, input_filename.c_str(), sizeof(input_tmp) - 1);
    strncpy(output_tmp, output_filename.c_str(), sizeof(output_tmp) - 1);
    strncpy(append_tmp, append_filename.c_str(), sizeof(append_tmp) - 1);
    
    int input_fd = mkstemp(input_tmp);
    int output_fd = mkstemp(output_tmp);
    int append_fd = mkstemp(append_tmp);
    
    if (input_fd < 0 || output_fd < 0 || append_fd < 0) {
        if (input_fd >= 0) close(input_fd);
        if (output_fd >= 0) close(output_fd);
        if (append_fd >= 0) close(append_fd);
        return 0;
    }
    
    // Write fuzzer data to input file (simulating a pcap file)
    if (write(input_fd, file_data.data(), file_data.size()) != (ssize_t)file_data.size()) {
        close(input_fd); close(output_fd); close(append_fd);
        unlink(input_tmp); unlink(output_tmp); unlink(append_tmp);
        return 0;
    }
    
    close(input_fd);
    close(output_fd);
    close(append_fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    switch (operation) {
        case 0: {
            // Test basic pcap_open_offline
            pcap_t* pcap = pcap_open_offline(input_tmp, errbuf);
            if (pcap != nullptr) {
                // Read some packets if available
                struct pcap_pkthdr* header;
                const u_char* pkt_data;
                int ret;
                for (int i = 0; i < 10; i++) {
                    ret = pcap_next_ex(pcap, &header, &pkt_data);
                    if (ret <= 0) break;
                }
                pcap_close(pcap);
            }
            break;
        }
        
        case 1: {
            // Test pcap_open_offline_with_tstamp_precision
            pcap_t* pcap = pcap_open_offline_with_tstamp_precision(input_tmp, tstamp_precision, errbuf);
            if (pcap != nullptr) {
                // Test some informational functions
                pcap_datalink(pcap);
                pcap_snapshot(pcap);
                pcap_major_version(pcap);
                pcap_minor_version(pcap);
                
                pcap_close(pcap);
            }
            break;
        }
        
        case 2: {
            // Test pcap_dump_open - read from input and write to output
            pcap_t* pcap = pcap_open_offline(input_tmp, errbuf);
            if (pcap != nullptr) {
                pcap_dumper_t* dumper = pcap_dump_open(pcap, output_tmp);
                if (dumper != nullptr) {
                    // Use pcap_loop to read and dump packets
                    pcap_loop(pcap, -1, pcap_dump_handler, (u_char*)dumper);
                    
                    // Test pcap_dump_file and pcap_dump_ftell
                    FILE* dump_file = pcap_dump_file(dumper);
                    if (dump_file != nullptr) {
                        long pos = pcap_dump_ftell(dumper);
                        (void)pos;
                    }
                    
                    pcap_dump_flush(dumper);
                    pcap_dump_close(dumper);
                }
                pcap_close(pcap);
            }
            break;
        }
        
        case 3: {
            // Test pcap_dump_open_append
            pcap_t* pcap = pcap_open_offline(input_tmp, errbuf);
            if (pcap != nullptr) {
                // First create a file to append to
                pcap_dumper_t* initial_dumper = pcap_dump_open(pcap, append_tmp);
                if (initial_dumper != nullptr) {
                    pcap_dump_close(initial_dumper);
                }
                
                // Now append to it
                pcap_dumper_t* append_dumper = pcap_dump_open_append(pcap, append_tmp);
                if (append_dumper != nullptr) {
                    pcap_loop(pcap, 5, pcap_dump_handler, (u_char*)append_dumper);
                    pcap_dump_close(append_dumper);
                }
                
                pcap_close(pcap);
            }
            break;
        }
        
        case 4: {
            // Test pcap_fopen_offline (file handle version)
            FILE* input_file = fopen(input_tmp, "rb");
            if (input_file != nullptr) {
                pcap_t* pcap = pcap_fopen_offline(input_file, errbuf);
                if (pcap != nullptr) {
                    // Exercise the pcap handle
                    struct pcap_stat stats;
                    pcap_stats(pcap, &stats);
                    pcap_close(pcap);
                    // DO NOT fclose(input_file) - pcap_close() takes ownership of the FILE*
                } else {
                    // If pcap_fopen_offline failed, close the file ourselves
                    fclose(input_file);
                }
            }
            break;
        }
        
        case 5: {
            // Test pcap_fopen_offline_with_tstamp_precision
            FILE* input_file = fopen(input_tmp, "rb");
            if (input_file != nullptr) {
                pcap_t* pcap = pcap_fopen_offline_with_tstamp_precision(input_file, tstamp_precision, errbuf);
                if (pcap != nullptr) {
                    // Test pcap_dispatch with dump
                    pcap_dumper_t* dumper = pcap_dump_open(pcap, output_tmp);
                    if (dumper != nullptr) {
                        pcap_dispatch(pcap, 3, pcap_dump_handler, (u_char*)dumper);
                        pcap_dump_close(dumper);
                    }
                    pcap_close(pcap);
                    // DO NOT fclose(input_file) - pcap_close() takes ownership of the FILE*
                } else {
                    // If pcap_fopen_offline_with_tstamp_precision failed, close the file ourselves
                    fclose(input_file);
                }
            }
            break;
        }
        
        case 6: {
            // Test mixed operations: read, filter, dump using fuzzer-provided filter
            pcap_t* pcap = pcap_open_offline(input_tmp, errbuf);
            if (pcap != nullptr) {
                // Compile filter using fuzzer-provided parameters
                struct bpf_program fp;
                
                if (pcap_compile(pcap, &fp, filter_str.c_str(), optimize, netmask) == 0) {
                    pcap_setfilter(pcap, &fp);
                    
                    pcap_dumper_t* dumper = pcap_dump_open(pcap, output_tmp);
                    if (dumper != nullptr) {
                        pcap_loop(pcap, -1, pcap_dump_handler, (u_char*)dumper);
                        pcap_dump_close(dumper);
                    }
                    
                    pcap_freecode(&fp);
                }
                
                pcap_close(pcap);
            }
            break;
        }
        
        case 7: {
            // Test error paths with invalid files
            pcap_t* pcap = pcap_open_offline("/nonexistent/file.pcap", errbuf);
            if (pcap == nullptr) {
                // Expected error - cannot call pcap_perror or pcap_geterr with NULL handle
                // pcap_perror and pcap_geterr require a valid pcap_t pointer
                // The error is already in errbuf, which we can check
                if (strlen(errbuf) > 0) {
                    // Error message captured successfully
                }
            } else {
                pcap_close(pcap);
            }
            
            // Cannot test pcap_dump_open with nullptr as it would crash
            // pcap_dump_open expects a valid pcap_t pointer and dereferences it
            break;
        }
    }
    
    // Clean up temporary files
    unlink(input_tmp);
    unlink(output_tmp);
    unlink(append_tmp);
    
    return 0;
}

} // namespace harness_002
