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
#include <sys/time.h>
#include <pcap/pcap.h>

namespace harness_018 {


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
    // Minimum size: scenario + linktype + snaplen + timeout + buffer_size + flags
    if (Size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for comprehensive async I/O testing
    // First byte: test scenario selector (0-5)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Second byte: link type for pcap_open_dead or offline files
    int linktype = fdp.ConsumeIntegral<uint8_t>();
    
    // Third and fourth bytes: snapshot length
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    
    // Fifth byte: timeout in seconds for select/poll testing
    int timeout_sec = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // Sixth byte: buffer size
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 65536);
    
    // Seventh byte: flags for various options
    uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Eighth byte: promiscuous mode flag
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    
    // Ninth byte: non-blocking mode flag
    int nonblock = fdp.ConsumeBool() ? 1 : 0;
    
    // Tenth byte: immediate mode flag
    int immediate_mode = fdp.ConsumeBool() ? 1 : 0;
    
    // Eleventh byte: direction (inbound/outbound/both)
    pcap_direction_t direction = static_cast<pcap_direction_t>(fdp.ConsumeIntegral<uint8_t>() % 3);
    
    // Consume device name (can be real device, "any", or synthetic)
    std::string device_name;
    uint8_t device_type = fdp.ConsumeIntegral<uint8_t>() % 5;
    switch (device_type) {
        case 0:
            device_name = "any";  // Linux special device for all interfaces
            break;
        case 1:
            device_name = "lo";   // Loopback interface
            break;
        case 2:
            device_name = "eth0"; // Common Ethernet interface name
            break;
        case 3:
            device_name = fdp.ConsumeRandomLengthString(32); // Random device name
            break;
        case 4:
            device_name = fdp.ConsumeRandomLengthString(16); // Short random name
            break;
    }
    
    // Consume filter string (optional)
    std::string filter_str = fdp.ConsumeRandomLengthString(128);
    
    // Consume error prefix for pcap_perror
    std::string error_prefix = fdp.ConsumeRandomLengthString(64);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    pcap_t *pcap_handle = nullptr;
    struct bpf_program bpf;
    memset(&bpf, 0, sizeof(bpf));
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Test with pcap_open_dead (always succeeds)
            pcap_handle = pcap_open_dead(linktype, snaplen);
            if (pcap_handle != nullptr) {
                // Test async I/O functions on dead handle
                int selectable_fd = pcap_get_selectable_fd(pcap_handle);
                // selectable_fd should be -1 for dead handles
                
                const struct timeval *required_timeout = pcap_get_required_select_timeout(pcap_handle);
                // required_timeout should be NULL for dead handles
                
                // Test pcap_perror - should print any existing error
                pcap_perror(pcap_handle, error_prefix.c_str());
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test with pcap_open_live (may fail without privileges)
            pcap_handle = pcap_open_live(device_name.c_str(), snaplen, promisc, timeout_sec * 1000, errbuf);
            
            if (pcap_handle != nullptr) {
                // Set non-blocking mode
                if (nonblock) {
                    pcap_setnonblock(pcap_handle, 1, errbuf);
                }
                
                // Set direction if specified
                if (direction != PCAP_D_INOUT) {
                    pcap_setdirection(pcap_handle, direction);
                }
                
                // Test async I/O functions
                int selectable_fd = pcap_get_selectable_fd(pcap_handle);
                // selectable_fd could be -1 or a valid file descriptor
                
                const struct timeval *required_timeout = pcap_get_required_select_timeout(pcap_handle);
                // required_timeout could be NULL or point to a timeout
                
                // Test pcap_perror with current error state
                pcap_perror(pcap_handle, error_prefix.c_str());
                
                // Try to compile and set filter
                if (!filter_str.empty()) {
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    } else {
                        // Compilation failed, test pcap_perror with filter error
                        pcap_perror(pcap_handle, "Filter error");
                    }
                }
                
                pcap_close(pcap_handle);
            } else {
                // Open failed, test pcap_perror with open error
                // Create a dummy handle to test pcap_perror
                pcap_t *dummy = pcap_open_dead(linktype, snaplen);
                if (dummy != nullptr) {
                    // Use pcap_geterr to get error buffer and test pcap_perror
                    pcap_perror(dummy, "Open failed");
                    pcap_close(dummy);
                }
            }
            break;
        }
        case 2: {
            // Scenario 2: Test with pcap_open_offline (file-based)
            if (fdp.remaining_bytes() < 64) {
                break;  // Need enough data for a minimal pcap file
            }
            
            // Create a temporary file with minimal pcap data
            char filename[] = "/tmp/libpcap_async_fuzz_XXXXXX";
            int fd = mkstemp(filename);
            if (fd < 0) {
                break;
            }
            close(fd);
            
            FILE *pcap_file = fopen(filename, "wb");
            if (pcap_file != nullptr) {
                // Write pcap file header (magic number for microsecond timestamps)
                uint32_t magic = 0xA1B2C3D4;
                uint16_t version_major = 2;
                uint16_t version_minor = 4;
                int32_t thiszone = 0;
                uint32_t sigfigs = 0;
                uint32_t network = linktype;
                
                write_pcap_header(pcap_file, magic, version_major, version_minor,
                                 thiszone, sigfigs, snaplen, network);
                
                // Write a minimal packet if we have enough data
                if (fdp.remaining_bytes() >= 16) {
                    uint32_t ts_sec = fdp.ConsumeIntegral<uint32_t>();
                    uint32_t ts_usec = fdp.ConsumeIntegral<uint32_t>();
                    uint32_t incl_len = fdp.ConsumeIntegralInRange<uint32_t>(1, 1024);
                    uint32_t orig_len = incl_len;
                    
                    write_packet_header(pcap_file, ts_sec, ts_usec, incl_len, orig_len);
                    
                    // Write packet data
                    if (fdp.remaining_bytes() >= incl_len) {
                        std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(incl_len);
                        fwrite(packet_data.data(), 1, packet_data.size(), pcap_file);
                    }
                }
                
                fclose(pcap_file);
                
                // Now open the file with pcap_open_offline
                pcap_handle = pcap_open_offline(filename, errbuf);
                if (pcap_handle != nullptr) {
                    // Test async I/O functions on offline handle
                    int selectable_fd = pcap_get_selectable_fd(pcap_handle);
                    // selectable_fd should be -1 for offline handles
                    
                    const struct timeval *required_timeout = pcap_get_required_select_timeout(pcap_handle);
                    // required_timeout should be NULL for offline handles
                    
                    // Test pcap_perror
                    pcap_perror(pcap_handle, error_prefix.c_str());
                    
                    // Try to read packets
                    struct pcap_pkthdr *pkt_header;
                    const u_char *pkt_data;
                    int ret = pcap_next_ex(pcap_handle, &pkt_header, &pkt_data);
                    // ret could be 1 (packet read), 0 (timeout), -1 (error), or -2 (EOF)
                    
                    if (ret == -1) {
                        // Read error, test pcap_perror
                        pcap_perror(pcap_handle, "Read error");
                    }
                    
                    pcap_close(pcap_handle);
                } else {
                    // Open failed, create dummy for pcap_perror test
                    pcap_t *dummy = pcap_open_dead(linktype, snaplen);
                    if (dummy != nullptr) {
                        // Use pcap_geterr to get error buffer and test pcap_perror
                        pcap_perror(dummy, "Offline open failed");
                        pcap_close(dummy);
                    }
                }
                
                remove(filename);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test pcap_create + pcap_activate sequence
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            if (pcap_handle != nullptr) {
                // Set various options before activation
                if (snaplen > 0) {
                    pcap_set_snaplen(pcap_handle, snaplen);
                }
                
                if (promisc) {
                    pcap_set_promisc(pcap_handle, 1);
                }
                
                if (timeout_sec > 0) {
                    pcap_set_timeout(pcap_handle, timeout_sec * 1000);
                }
                
                if (immediate_mode) {
                    pcap_set_immediate_mode(pcap_handle, 1);
                }
                
                if (buffer_size > 0) {
                    pcap_set_buffer_size(pcap_handle, buffer_size);
                }
                
                // Activate the handle
                int activate_result = pcap_activate(pcap_handle);
                
                if (activate_result >= 0) {
                    // Activation succeeded or had non-fatal warning
                    
                    // Test async I/O functions
                    int selectable_fd = pcap_get_selectable_fd(pcap_handle);
                    
                    const struct timeval *required_timeout = pcap_get_required_select_timeout(pcap_handle);
                    
                    // Test pcap_perror with current state
                    pcap_perror(pcap_handle, error_prefix.c_str());
                    
                    // Set non-blocking if requested
                    if (nonblock) {
                        pcap_setnonblock(pcap_handle, 1, errbuf);
                    }
                    
                    // Try to compile and set filter
                    if (!filter_str.empty()) {
                        if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                            pcap_setfilter(pcap_handle, &bpf);
                            pcap_freecode(&bpf);
                        }
                    }
                    
                } else {
                    // Activation failed, test pcap_perror with activation error
                    pcap_perror(pcap_handle, "Activation failed");
                }
                
                pcap_close(pcap_handle);
            } else {
                // Create failed, test with dummy handle
                pcap_t *dummy = pcap_open_dead(linktype, snaplen);
                if (dummy != nullptr) {
                    // Test pcap_perror on dummy handle
                    pcap_perror(dummy, "Create failed");
                    pcap_close(dummy);
                }
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Test error scenarios and pcap_perror extensively
            // Create multiple error conditions and test pcap_perror
            
            // Test 1: Invalid device name
            std::string invalid_device = fdp.ConsumeRandomLengthString(256);
            pcap_handle = pcap_open_live(invalid_device.c_str(), snaplen, 0, 1000, errbuf);
            if (pcap_handle == nullptr) {
                pcap_t *dummy = pcap_open_dead(linktype, snaplen);
                if (dummy != nullptr) {
                    // Test pcap_perror on dummy handle
                    pcap_perror(dummy, "Device error");
                    pcap_close(dummy);
                }
            } else {
                pcap_close(pcap_handle);
            }
            
            // Test 2: Compile invalid filter
            pcap_handle = pcap_open_dead(linktype, snaplen);
            if (pcap_handle != nullptr) {
                std::string invalid_filter = fdp.ConsumeRandomLengthString(512);
                if (pcap_compile(pcap_handle, &bpf, invalid_filter.c_str(), 1, PCAP_NETMASK_UNKNOWN) != 0) {
                    pcap_perror(pcap_handle, "Compile error");
                }
                
                // Test async functions on dead handle
                int selectable_fd = pcap_get_selectable_fd(pcap_handle);
                const struct timeval *required_timeout = pcap_get_required_select_timeout(pcap_handle);
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Test interaction between async functions and other operations
            pcap_handle = pcap_open_dead(linktype, snaplen);
            if (pcap_handle != nullptr) {
                // Baseline async function calls
                int initial_fd = pcap_get_selectable_fd(pcap_handle);
                const struct timeval *initial_timeout = pcap_get_required_select_timeout(pcap_handle);
                
                // Test pcap_perror with no error
                pcap_perror(pcap_handle, "No error");
                
                // Try to compile a filter (may fail)
                if (!filter_str.empty()) {
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) != 0) {
                        // Compilation failed, test pcap_perror
                        pcap_perror(pcap_handle, "Filter compilation");
                        
                        // Test async functions after error
                        int after_error_fd = pcap_get_selectable_fd(pcap_handle);
                        const struct timeval *after_error_timeout = pcap_get_required_select_timeout(pcap_handle);
                    } else {
                        // Compilation succeeded, set filter
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                        
                        // Test async functions after successful operation
                        int after_success_fd = pcap_get_selectable_fd(pcap_handle);
                        const struct timeval *after_success_timeout = pcap_get_required_select_timeout(pcap_handle);
                    }
                }
                
                // Test pcap_geterr to get error string
                char *err_str = pcap_geterr(pcap_handle);
                
                pcap_close(pcap_handle);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_018
