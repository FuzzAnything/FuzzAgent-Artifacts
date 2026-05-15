#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <string>

namespace harness_000 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode: 0 = filter only, 1 = pcap only, 2 = both
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume link type for pcap_open_dead
    int linktype = fdp.ConsumeIntegralInRange<int>(1, 255); // Reasonable link type range
    int snaplen = fdp.ConsumeIntegralInRange<int>(64, 65535); // Standard snaplen range
    
    // Consume filter string
    size_t filter_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string filter_str = fdp.ConsumeBytesAsString(filter_len);
    
    // Remaining bytes are for pcap data (if needed)
    std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    struct bpf_program bpf;
    int r;
    
    // Create a dead capture handle for filter testing
    pcap_handle = pcap_open_dead(linktype, snaplen);
    if (pcap_handle == nullptr) {
        return 0;
    }
    
    // Test pcap_compile with the filter string
    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
        // Test pcap_setfilter if compilation succeeded
        if (pcap_setfilter(pcap_handle, &bpf) == 0) {
            // Filter set successfully
        }
        
        // Test pcap_datalink
        int datalink = pcap_datalink(pcap_handle);
        
        // Test pcap_snapshot
        int snapshot = pcap_snapshot(pcap_handle);
        
        // Test pcap_is_swapped
        int swapped = pcap_is_swapped(pcap_handle);
        
        // Test pcap_geterr
        const char* err = pcap_geterr(pcap_handle);
        
        // Test pcap_stats (may fail on dead handle, but call it anyway)
        struct pcap_stat stats;
        pcap_stats(pcap_handle, &stats);
        
        // Free the compiled filter
        pcap_freecode(&bpf);
    } else {
        // Compilation failed - get error
        const char* compile_err = pcap_geterr(pcap_handle);
    }
    
    // Close the dead handle
    pcap_close(pcap_handle);
    
    // If we have pcap data, try to parse it as a pcap file
    if (!pcap_data.empty() && (mode == 1 || mode == 2)) {
        // Create temporary file for pcap data
        char filename[] = "/tmp/libpcap_fuzz_XXXXXX";
        int fd = mkstemp(filename);
        if (fd >= 0) {
            // Write pcap data to temp file
            if (write(fd, pcap_data.data(), pcap_data.size()) == (ssize_t)pcap_data.size()) {
                close(fd);
                
                // Try to open as pcap file
                pcap_handle = pcap_open_offline(filename, errbuf);
                if (pcap_handle != nullptr) {
                    // Test pcap_next_ex to read packets
                    struct pcap_pkthdr* header;
                    const u_char* pkt;
                    
                    r = pcap_next_ex(pcap_handle, &header, &pkt);
                    int packet_count = 0;
                    
                    while (r > 0 && packet_count < 10) { // Limit to 10 packets
                        // Test pcap_offline_filter if we have a compiled filter
                        if (mode == 2 && filter_str.length() > 0) {
                            // Recompile filter for this handle
                            if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                                int filter_result = pcap_offline_filter(&bpf, header, pkt);
                                pcap_freecode(&bpf);
                            }
                        }
                        
                        packet_count++;
                        r = pcap_next_ex(pcap_handle, &header, &pkt);
                    }
                    
                    // Test pcap_stats on the pcap file
                    struct pcap_stat pcap_statistics;
                    pcap_stats(pcap_handle, &pcap_statistics);
                    
                    pcap_close(pcap_handle);
                }
            } else {
                close(fd);
            }
            
            // Clean up temp file
            unlink(filename);
        }
    }
    // Additional API tests that don't require handles
    // Test pcap_lib_version
    const char* version = pcap_lib_version();
    
    // Test pcap_datalink_val_to_name and pcap_datalink_name_to_val
    const char* dlt_name = pcap_datalink_val_to_name(linktype);
    if (dlt_name != nullptr) {
        int dlt_val = pcap_datalink_name_to_val(dlt_name);
    }
    
    return 0;
}

} // namespace harness_000
