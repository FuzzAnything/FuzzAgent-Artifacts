#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pcap/pcap.h>

namespace harness_020 {


// Helper function to write buffer to a temporary file
static int bufferToFile(const char *name, const uint8_t *data, size_t size) {
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
    if (fwrite(data, 1, size, fd) != size) {
        fclose(fd);
        return -3;
    }
    fclose(fd);
    return 0;
}

// Helper to create minimal pcap file header
static void write_pcap_header(FILE *fd, uint32_t magic, uint16_t version_major, 
                             uint16_t version_minor, int32_t thiszone, uint32_t sigfigs,
                             uint32_t snaplen, uint32_t network) {
    fwrite(&magic, sizeof(magic), 1, fd);
    fwrite(&version_major, sizeof(version_major), 1, fd);
    fwrite(&version_minor, sizeof(version_minor), 1, fd);
    fwrite(&thiszone, sizeof(thiszone), 1, fd);
    fwrite(&sigfigs, sizeof(sigfigs), 1, fd);
    fwrite(&snaplen, sizeof(snaplen), 1, fd);
    fwrite(&network, sizeof(network), 1, fd);
}

// Helper to create pcap packet header
static void write_packet_header(FILE *fd, uint32_t ts_sec, uint32_t ts_usec,
                               uint32_t incl_len, uint32_t orig_len) {
    fwrite(&ts_sec, sizeof(ts_sec), 1, fd);
    fwrite(&ts_usec, sizeof(ts_usec), 1, fd);
    fwrite(&incl_len, sizeof(incl_len), 1, fd);
    fwrite(&orig_len, sizeof(orig_len), 1, fd);
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need minimum input for comprehensive lifecycle testing
    // At least: scenario + device name + snaplen + promisc + timeout + linktype + pcap data
    if (Size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for comprehensive capture lifecycle testing
    // First byte: test scenario selector (0-7) for different lifecycle sequences
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume device name - test various device name scenarios
    std::string device_name;
    bool use_null_device = false;
    bool use_empty_device = false;
    
    uint8_t device_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
    switch (device_choice) {
        case 0:
            // Use NULL device name
            use_null_device = true;
            break;
        case 1:
            // Use empty string
            use_empty_device = true;
            device_name = "";
            break;
        case 2:
            // Use short device name from input
            device_name = fdp.ConsumeRandomLengthString(32);
            break;
        case 3:
            // Use longer device name
            device_name = fdp.ConsumeRandomLengthString(128);
            break;
        case 4:
            // Use special device name "any"
            device_name = "any";
            break;
    }
    
    // Consume capture parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
    int promisc = fdp.ConsumeIntegral<uint8_t>() % 2;
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);
    int linktype = fdp.ConsumeIntegral<int>();
    int tstamp_type = fdp.ConsumeIntegral<int>() % 6;  // PCAP_TSTAMP_* values
    int precision = fdp.ConsumeIntegral<uint8_t>() % 2; // PCAP_TSTAMP_PRECISION_MICRO/NANO
    
    // Consume bytes for temporary filenames
    std::string input_filename = "/tmp/libpcap_fuzz_020_input_" + 
                                 std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".pcap";
    std::string output_filename = "/tmp/libpcap_fuzz_020_output_" + 
                                  std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".pcap";
    
    // Consume filter string for scenario 2 (pcap_open_dead filter testing)
    std::string filter_str = fdp.ConsumeRandomLengthString(256);
    
    // Consume remaining bytes for pcap file data
    std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    pcap_t *pcap_handle = nullptr;
    pcap_dumper_t *dumper = nullptr;
    
    // ALWAYS call pcap_lib_version() - this is one of our primary targets
    const char *lib_version = pcap_lib_version();
    // We don't need to use the result, but calling it exercises the code
    
    // Create input pcap file from consumed data if we have enough data
    int file_created = 0;
    if (pcap_data.size() >= 24) {  // Minimum size for pcap header
        file_created = bufferToFile(input_filename.c_str(), pcap_data.data(), pcap_data.size());
    }
    
    // Test different capture lifecycle scenarios based on input
    switch (scenario) {
        case 0: {
            // Scenario 0: Traditional pcap_open_live lifecycle
            if (use_null_device) {
                pcap_handle = pcap_open_live(nullptr, snaplen, promisc, timeout_ms, errbuf);
            } else if (use_empty_device) {
                pcap_handle = pcap_open_live("", snaplen, promisc, timeout_ms, errbuf);
            } else {
                pcap_handle = pcap_open_live(device_name.c_str(), snaplen, promisc, timeout_ms, errbuf);
            }
            
            if (pcap_handle != nullptr) {
                // Test various operations on live handle
                pcap_setnonblock(pcap_handle, promisc, errbuf);
                pcap_setdirection(pcap_handle, PCAP_D_INOUT);
                pcap_set_buffer_size(pcap_handle, 1024 * 1024);
                
                // Try to get some statistics (may fail but that's ok)
                struct pcap_stat stats;
                pcap_stats(pcap_handle, &stats);
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Modern pcap_create + pcap_activate lifecycle
            if (use_null_device) {
                pcap_handle = pcap_create(nullptr, errbuf);
            } else if (use_empty_device) {
                pcap_handle = pcap_create("", errbuf);
            } else {
                pcap_handle = pcap_create(device_name.c_str(), errbuf);
            }
            
            if (pcap_handle != nullptr) {
                // Set various options before activation
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_promisc(pcap_handle, promisc);
                pcap_set_timeout(pcap_handle, timeout_ms);
                pcap_set_tstamp_type(pcap_handle, tstamp_type);
                pcap_set_tstamp_precision(pcap_handle, precision);
                pcap_set_buffer_size(pcap_handle, 1024 * 1024);
                pcap_set_immediate_mode(pcap_handle, promisc);
                
                // Activate the capture
                int activate_result = pcap_activate(pcap_handle);
                
                // If activation succeeded, test some operations
                if (activate_result >= 0) {
                    pcap_setnonblock(pcap_handle, promisc, errbuf);
                    pcap_setdirection(pcap_handle, PCAP_D_INOUT);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: pcap_open_dead for offline filter testing
            pcap_handle = pcap_open_dead(linktype, snaplen);
            if (pcap_handle != nullptr) {
                // Test with timestamp precision variant
                pcap_t *pcap_handle_prec = pcap_open_dead_with_tstamp_precision(
                    linktype, snaplen, precision);
                if (pcap_handle_prec != nullptr) {
                    pcap_close(pcap_handle_prec);
                }
                
                // Test filter compilation with fuzzer-provided filter string
                struct bpf_program fp;
                if (pcap_compile(pcap_handle, &fp, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                    pcap_freecode(&fp);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }

        
        case 3: {
            // Scenario 3: pcap_open_offline file capture lifecycle
            if (file_created == 0 && pcap_data.size() >= 24) {
                pcap_handle = pcap_open_offline(input_filename.c_str(), errbuf);
                if (pcap_handle != nullptr) {
                    // Test reading packets
                    struct pcap_pkthdr header;
                    const u_char *packet;
                    
                    // Try to read a few packets
                    for (int i = 0; i < 3; i++) {
                        packet = pcap_next(pcap_handle, &header);
                        if (packet == nullptr) {
                            break;
                        }
                    }
                    
                    // Test offline with timestamp precision
                    pcap_t *pcap_handle_prec = pcap_open_offline_with_tstamp_precision(
                        input_filename.c_str(), precision, errbuf);
                    if (pcap_handle_prec != nullptr) {
                        pcap_close(pcap_handle_prec);
                    }
                    
                    pcap_close(pcap_handle);
                }
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Mixed lifecycle - create, set options, activate, use, close
            if (use_null_device) {
                pcap_handle = pcap_create(nullptr, errbuf);
            } else {
                pcap_handle = pcap_create(device_name.c_str(), errbuf);
            }
            
            if (pcap_handle != nullptr) {
                // Set comprehensive options
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_promisc(pcap_handle, promisc);
                pcap_set_timeout(pcap_handle, timeout_ms);
                pcap_set_tstamp_type(pcap_handle, tstamp_type);
                pcap_set_tstamp_precision(pcap_handle, precision);
                pcap_set_buffer_size(pcap_handle, 1024 * 1024);
                pcap_set_immediate_mode(pcap_handle, promisc);
                pcap_setdirection(pcap_handle, PCAP_D_INOUT);
                
                // Try activation
                int activate_result = pcap_activate(pcap_handle);
                
                // If activated, try to use it
                if (activate_result >= 0) {
                    // Try to set non-blocking mode
                    pcap_setnonblock(pcap_handle, 1, errbuf);
                    
                    // Try to get some handle properties
                    int fd = pcap_fileno(pcap_handle);
                    int snap = pcap_snapshot(pcap_handle);
                    int link = pcap_datalink(pcap_handle);
                    
                    // Try to get statistics
                    struct pcap_stat stats;
                    pcap_stats(pcap_handle, &stats);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Error path testing - invalid parameters and sequences
            // Try pcap_open_live with extreme values
            pcap_handle = pcap_open_live(device_name.c_str(), 
                                        snaplen, 
                                        promisc, 
                                        timeout_ms, 
                                        errbuf);
            
            if (pcap_handle != nullptr) {
                // Try invalid operations
                pcap_setdirection(pcap_handle, static_cast<pcap_direction_t>(99)); // Invalid direction
                
                // Try to compile invalid filter
                struct bpf_program fp;
                const char *invalid_filter = "invalid filter syntax !!";
                pcap_compile(pcap_handle, &fp, invalid_filter, 1, PCAP_NETMASK_UNKNOWN);
                
                pcap_close(pcap_handle);
            }
            
            // Test pcap_create with empty error buffer
            char empty_errbuf[1] = {0};
            pcap_t *p2 = pcap_create(device_name.c_str(), empty_errbuf);
            if (p2 != nullptr) {
                pcap_close(p2);
            }
            break;
        }
        
        case 6: {
            // Scenario 6: File handle based offline capture
            if (file_created == 0 && pcap_data.size() >= 24) {
                FILE *fp = fopen(input_filename.c_str(), "rb");
                if (fp != nullptr) {
                    pcap_handle = pcap_fopen_offline(fp, errbuf);
                    if (pcap_handle != nullptr) {
                        // Test the file handle variant with timestamp precision
                        rewind(fp);
                        pcap_t *pcap_handle_prec = pcap_fopen_offline_with_tstamp_precision(
                            fp, precision, errbuf);
                        if (pcap_handle_prec != nullptr) {
                            pcap_close(pcap_handle_prec);
                        }
                        
                        pcap_close(pcap_handle);
                    } else {
                        fclose(fp);
                    }
                }
            }
            break;
        }
        
        case 7: {
            // Scenario 7: Complex lifecycle with dump operations
            if (use_null_device) {
                pcap_handle = pcap_create(nullptr, errbuf);
            } else {
                pcap_handle = pcap_create(device_name.c_str(), errbuf);
            }
            
            if (pcap_handle != nullptr) {
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_activate(pcap_handle);
                
                // Try to create a dumper if we have a file
                if (file_created == 0) {
                    dumper = pcap_dump_open(pcap_handle, output_filename.c_str());
                    if (dumper != nullptr) {
                        // Create a dummy packet header
                        struct pcap_pkthdr pkthdr;
                        pkthdr.ts.tv_sec = 1234567890;
                        pkthdr.ts.tv_usec = 0;
                        pkthdr.caplen = 64;
                        pkthdr.len = 64;
                        
                        // Create dummy packet data
                        uint8_t dummy_packet[64];
                        for (int i = 0; i < 64; i++) {
                            dummy_packet[i] = i;
                        }
                        
                        // Dump the packet
                        pcap_dump((u_char *)dumper, &pkthdr, dummy_packet);
                        pcap_dump_flush(dumper);
                        
                        // Get dump file position
                        long pos = pcap_dump_ftell(dumper);
                        
                        pcap_dump_close(dumper);
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
    }
    
    // Clean up temporary files
    if (file_created == 0) {
        remove(input_filename.c_str());
        remove(output_filename.c_str());
    }
    
    return 0;
}

} // namespace harness_020
