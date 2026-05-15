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

namespace harness_001 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);
    int immediate_mode = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume device name string
    size_t dev_name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string dev_name = fdp.ConsumeBytesAsString(dev_name_size);
    
    // Consume filter string
    size_t filter_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string filter_str;
    if (filter_size > 0) {
        filter_str = fdp.ConsumeBytesAsString(filter_size);
    }
    
    // Determine which test to run
    int test_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    if (test_type == 0) {
        // Test 1: Device discovery APIs
        pcap_if_t* alldevs = nullptr;
        char errbuf[PCAP_ERRBUF_SIZE];
        
        // Try to find all devices
        if (pcap_findalldevs(&alldevs, errbuf) == 0) {
            // Iterate through devices (just a few to avoid excessive processing)
            pcap_if_t* d = alldevs;
            int count = 0;
            while (d != nullptr && count < 5) {
                // Try to get network address for device
                pcap_addr_t* a = d->addresses;
                while (a != nullptr) {
                    a = a->next;
                }
                d = d->next;
                count++;
            }
            
            // Free device list
            pcap_freealldevs(alldevs);
        }
        
        // Try deprecated pcap_lookupdev
        char* default_dev = pcap_lookupdev(errbuf);
        // Don't need to free, returns pointer to static buffer
        
    } else if (test_type == 1) {
        // Test 2: Live capture with pcap_open_live (deprecated but common)
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcap = pcap_open_live(dev_name.c_str(), snaplen, promisc, timeout_ms, errbuf);
        
        if (pcap != nullptr) {
            // Try to get various properties
            int linktype = pcap_datalink(pcap);
            int major = pcap_major_version(pcap);
            int minor = pcap_minor_version(pcap);
            int snapshot = pcap_snapshot(pcap);
            
            // Try to compile and set a filter if we have one
            if (!filter_str.empty()) {
                struct bpf_program bpf;
                if (pcap_compile(pcap, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                    pcap_setfilter(pcap, &bpf);
                    pcap_freecode(&bpf);
                }
            }
            
            // Try to set some options
            pcap_setnonblock(pcap, immediate_mode, errbuf);
            
            pcap_close(pcap);
        }
        
    } else if (test_type == 2) {
        // Test 3: Modern pcap_create/pcap_activate API
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t* pcap = pcap_create(dev_name.c_str(), errbuf);
        
        if (pcap != nullptr) {
            // Set various options
            pcap_set_snaplen(pcap, snaplen);
            pcap_set_promisc(pcap, promisc);
            pcap_set_timeout(pcap, timeout_ms);
            pcap_set_buffer_size(pcap, buffer_size);
            pcap_set_immediate_mode(pcap, immediate_mode);
            
            // Try to activate
            if (pcap_activate(pcap) >= 0) {
                // Capture is active, try some operations
                int datalink = pcap_datalink(pcap);
                
                // Try to get stats
                struct pcap_stat stats;
                pcap_stats(pcap, &stats);
                
                // Try to compile and set a filter if we have one
                if (!filter_str.empty()) {
                    struct bpf_program bpf;
                    if (pcap_compile(pcap, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                        pcap_setfilter(pcap, &bpf);
                        pcap_freecode(&bpf);
                    }
                }
                
                // Try non-blocking mode
                pcap_setnonblock(pcap, immediate_mode, errbuf);
            }
            
            pcap_close(pcap);
        }
        
    } else {
        // Test 4: File operations with pcap_open_offline and pcap_dump APIs
        std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
        if (pcap_data.size() < 24) {  // Minimum pcap header size
            return 0;
        }
        
        // Write to temporary file
        char filename[] = "/tmp/libpcap_fuzz_file_XXXXXX";
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
            // Try dump APIs - create output file
            char out_filename[] = "/tmp/libpcap_fuzz_out_XXXXXX";
            int out_fd = mkstemp(out_filename);
            if (out_fd >= 0) {
                close(out_fd);
                
                pcap_dumper_t* dumper = pcap_dump_open(pcap, out_filename);
                if (dumper != nullptr) {
                    // Read packets and dump them
                    struct pcap_pkthdr* header;
                    const u_char* pkt_data;
                    int r;
                    
                    for (int i = 0; i < 5; i++) {
                        r = pcap_next_ex(pcap, &header, &pkt_data);
                        if (r <= 0) {
                            break;
                        }
                        
                        // Dump the packet
                        pcap_dump((u_char*)dumper, header, pkt_data);
                    }
                    
                    // Try to flush
                    pcap_dump_flush(dumper);
                    
                    // Get file position
                    long pos = pcap_dump_ftell(dumper);
                    
                    pcap_dump_close(dumper);
                }
                
                unlink(out_filename);
            }
            
            pcap_close(pcap);
        }
        
        unlink(filename);
    }
    
    return 0;
}

} // namespace harness_001
