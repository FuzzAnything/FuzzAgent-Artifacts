#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_000 {

// Simple packet handler function for pcap_loop/pcap_dispatch
static void packet_handler(u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
    // Do nothing - just exercise the callback
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different API paths
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Consume common parameters
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
    
    // Consume BPF filter string
    std::string filter_str = fdp.ConsumeRandomLengthString(256);
    
    // Consume netmask for pcap_compile
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Consume packet count for pcap_loop
    int packet_count = fdp.ConsumeIntegralInRange<int>(0, 100);
    
    // Consume remaining bytes as dummy packet data
    std::vector<uint8_t> packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    switch (operation) {
        case 0: {
            // Test pcap_open_dead with pcap_compile and pcap_setfilter
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            struct bpf_program fp;
            int optimize = fdp.ConsumeBool() ? 1 : 0;
            if (pcap_compile(handle, &fp, filter_str.c_str(), optimize, netmask) == 0) {
                pcap_setfilter(handle, &fp);
                pcap_freecode(&fp);
            }
            
            pcap_close(handle);
            break;
        }
        
        case 1: {
            // Test pcap_compile with pcap_offline_filter
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            struct bpf_program fp;
            if (pcap_compile(handle, &fp, filter_str.c_str(), 1, netmask) == 0) {
                // Test pcap_offline_filter if we have packet data
                if (!packet_data.empty() && packet_data.size() >= 14) { // Minimum Ethernet header
                    struct pcap_pkthdr pkthdr;
                    pkthdr.caplen = packet_data.size();
                    pkthdr.len = packet_data.size();
                    pcap_offline_filter(&fp, &pkthdr, packet_data.data());
                }
                pcap_freecode(&fp);
            }
            
            pcap_close(handle);
            break;
        }
        
        case 2: {
            // Test pcap_loop with callback
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            pcap_loop(handle, packet_count, packet_handler, nullptr);
            pcap_close(handle);
            break;
        }
        
        case 3: {
            // Test pcap_dispatch with callback
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            pcap_dispatch(handle, packet_count, packet_handler, nullptr);
            pcap_close(handle);
            break;
        }
        
        case 4: {
            // Test pcap_next_ex and pcap_next
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            struct pcap_pkthdr* pkthdr = nullptr;
            const u_char* pkt_data = nullptr;
            
            // Test pcap_next_ex (will return -2 for dead capture)
            int ret = pcap_next_ex(handle, &pkthdr, &pkt_data);
            (void)ret;
            
            // Test pcap_next (will return NULL for dead capture)
            struct pcap_pkthdr hdr;
            const u_char* pkt = pcap_next(handle, &hdr);
            (void)pkt;
            
            pcap_close(handle);
            break;
        }
        
        case 5: {
            // Test various informational functions
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            // Test stats functions
            struct pcap_stat stats;
            pcap_stats(handle, &stats);
            
            // Test informational functions
            pcap_datalink(handle);
            pcap_snapshot(handle);
            pcap_is_swapped(handle);
            pcap_major_version(handle);
            pcap_minor_version(handle);
            
            // Test error functions
            char errbuf[PCAP_ERRBUF_SIZE];
            pcap_geterr(handle);
            pcap_strerror(0); // Test with error code 0
            
            pcap_close(handle);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_000
