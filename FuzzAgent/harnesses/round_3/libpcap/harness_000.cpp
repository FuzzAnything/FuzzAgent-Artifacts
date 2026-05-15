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

namespace harness_000 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    int netmask = fdp.ConsumeIntegral<uint32_t>();
    
    // Consume filter string
    size_t filter_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string filter_str = fdp.ConsumeBytesAsString(filter_size);
    
    // Check if we should test filter-only or pcap-file parsing
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    if (test_type == 0) {
        // Test 1: Filter compilation and setting only
        pcap_t* pcap = pcap_open_dead(linktype, snaplen);
        if (pcap == nullptr) {
            return 0;
        }
        
        struct bpf_program bpf;
        if (pcap_compile(pcap, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
            pcap_setfilter(pcap, &bpf);
            pcap_freecode(&bpf);
        }
        pcap_close(pcap);
        
    } else if (test_type == 1) {
        // Test 2: Create temporary pcap file from remaining data and parse it
        std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (pcap_data.size() < 24) {  // Minimum pcap header size
            return 0;
        }
        
        // Write to temporary file
        char filename[] = "/tmp/libpcap_fuzz_XXXXXX";
        int fd = mkstemp(filename);
        if (fd < 0) {
            return 0;
        }
        
        if (write(fd, pcap_data.data(), pcap_data.size()) != static_cast<ssize_t>(pcap_data.size())) {
            close(fd);
            unlink(filename);
            return 0;
        }
        close(fd);
        
        // Open and parse the pcap file
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcap = pcap_open_offline(filename, errbuf);
        if (pcap != nullptr) {
            struct pcap_pkthdr* header;
            const u_char* pkt_data;
            int r;
            
            // Read a few packets
            for (int i = 0; i < 10; i++) {
                r = pcap_next_ex(pcap, &header, &pkt_data);
                if (r <= 0) {
                    break;
                }
            }
            
            // Try to get stats
            struct pcap_stat stats;
            pcap_stats(pcap, &stats);
            
            pcap_close(pcap);
        }
        
        unlink(filename);
        
    } else {
        // Test 3: Combined - create pcap file and apply filter
        std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (pcap_data.size() < 24) {
            return 0;
        }
        
        char filename[] = "/tmp/libpcap_fuzz_XXXXXX";
        int fd = mkstemp(filename);
        if (fd < 0) {
            return 0;
        }
        
        if (write(fd, pcap_data.data(), pcap_data.size()) != static_cast<ssize_t>(pcap_data.size())) {
            close(fd);
            unlink(filename);
            return 0;
        }
        close(fd);
        
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcap = pcap_open_offline(filename, errbuf);
        if (pcap != nullptr) {
            struct bpf_program bpf;
            if (pcap_compile(pcap, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
                pcap_setfilter(pcap, &bpf);
                
                // Read packets with filter applied
                struct pcap_pkthdr* header;
                const u_char* pkt_data;
                int r;
                
                for (int i = 0; i < 10; i++) {
                    r = pcap_next_ex(pcap, &header, &pkt_data);
                    if (r <= 0) {
                        break;
                    }
                }
                
                pcap_freecode(&bpf);
            }
            
            pcap_close(pcap);
        }
        
        unlink(filename);
    }
    
    return 0;
}

} // namespace harness_000
