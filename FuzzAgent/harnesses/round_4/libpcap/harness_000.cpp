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
#include <pcap/pcap.h>

namespace harness_000 {


// Helper function to write buffer to a temporary file (from existing fuzz_pcap.c)
static int bufferToFile(const char * name, const uint8_t *Data, size_t Size) {
    FILE * fd;
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
    if (Size < 2) {
        return 0;  // Need at least 2 bytes for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input into multiple parts for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: link type for pcap_open_dead
    int linktype = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume some data for filter string
    std::string filter_str = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    pcap_t *pcap_handle = nullptr;
    struct bpf_program bpf;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Test pcap_open_live, pcap_compile, pcap_setfilter, pcap_close
            // Note: pcap_open_live requires a real device, so we'll use a dummy approach
            // In real fuzzing, this would be limited but we can test error paths
            std::string device = fdp.ConsumeRandomLengthString(32);
            
            // Try to open live capture (will likely fail without privileges)
            pcap_handle = pcap_open_live(device.c_str(), 65535, 1, 1000, errbuf);
            
            if (pcap_handle != nullptr) {
                // If we got a handle, try to compile and set filter
                if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                    pcap_setfilter(pcap_handle, &bpf);
                    pcap_freecode(&bpf);
                }
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test pcap_open_dead, pcap_compile, pcap_setfilter (from fuzz_filter.c)
            pcap_handle = pcap_open_dead(linktype, 65535);
            if (pcap_handle != nullptr) {
                if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                    pcap_setfilter(pcap_handle, &bpf);
                    pcap_freecode(&bpf);
                }
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test pcap_open_offline with packet reading (from fuzz_pcap.c)
            if (fdp.remaining_bytes() < 16) {
                break;  // Need enough data for a minimal pcap file
            }
            
            // Create a temporary file for pcap data
            char filename[] = "/tmp/libpcap_fuzz_XXXXXX";
            int fd = mkstemp(filename);
            if (fd < 0) {
                break;
            }
            close(fd);
            
            // Write remaining data to file
            std::vector<uint8_t> file_data = fdp.ConsumeRemainingBytes<uint8_t>();
            if (bufferToFile(filename, file_data.data(), file_data.size()) >= 0) {
                pcap_handle = pcap_open_offline(filename, errbuf);
                if (pcap_handle != nullptr) {
                    struct pcap_pkthdr *header;
                    const u_char *pkt;
                    
                    // Read some packets
                    int r = pcap_next_ex(pcap_handle, &header, &pkt);
                    int packet_count = 0;
                    while (r > 0 && packet_count < 10) {  // Limit to 10 packets
                        packet_count++;
                        r = pcap_next_ex(pcap_handle, &header, &pkt);
                    }
                    
                    // Try pcap_loop with limited count
                    if (packet_count > 0) {
                        // Create a simple handler
                        auto packet_handler = [](u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
                            // Do nothing, just count
                            int *count = (int *)user;
                            (*count)++;
                        };
                        
                        int loop_count = 0;
                        pcap_loop(pcap_handle, 3, packet_handler, (u_char *)&loop_count);
                    }
                    
                    // Try to get stats
                    struct pcap_stat stats;
                    pcap_stats(pcap_handle, &stats);
                    
                    pcap_close(pcap_handle);
                }
                unlink(filename);
            } else {
                unlink(filename);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test pcap_open_dead with various operations
            pcap_handle = pcap_open_dead(linktype, 65535);
            if (pcap_handle != nullptr) {
                // Try to compile filter
                if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                    // Test pcap_setfilter
                    pcap_setfilter(pcap_handle, &bpf);
                    
                    // Test other pcap functions
                    pcap_datalink(pcap_handle);
                    pcap_snapshot(pcap_handle);
                    pcap_is_swapped(pcap_handle);
                    pcap_major_version(pcap_handle);
                    pcap_minor_version(pcap_handle);
                    
                    pcap_freecode(&bpf);
                }
                pcap_close(pcap_handle);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_000
