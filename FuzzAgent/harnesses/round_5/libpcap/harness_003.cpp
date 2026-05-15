#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_003 {

// Simple packet handler for pcap_loop
static void packet_handler(u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
    // Do nothing - just exercise the callback
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful device enumeration testing
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different device enumeration workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume device enumeration parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
    int promisc = fdp.ConsumeBool() ? 1 : 0;
    int to_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);
    
    // Consume buffer size parameter
    int buffer_size = fdp.ConsumeIntegralInRange<int>(0, 1048576); // 0-1MB
    
    // Consume timestamp precision parameter
    u_int tstamp_precision = fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO;
    
    // Consume direction parameter for pcap_setdirection
    pcap_direction_t direction = PCAP_D_INOUT;
    uint8_t dir_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (dir_choice) {
        case 0: direction = PCAP_D_IN; break;
        case 1: direction = PCAP_D_OUT; break;
        case 2: direction = PCAP_D_INOUT; break;
    }
    
    // Consume filter string from fuzzer input
    std::string filter_str = fdp.ConsumeRandomLengthString(256);
    
    // Consume netmask from fuzzer input
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Consume optimize flag
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Consume remaining bytes for any additional data needs
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;
    
    switch (operation) {
        case 0: {
            // Test basic pcap_findalldevs with pcap_open_live on first device
            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                // If enumeration fails, exit gracefully
                return 0;
            }
            
            if (alldevs != nullptr) {
                // Try to open the first device
                pcap_t* handle = pcap_open_live(alldevs->name, snaplen, promisc, to_ms, errbuf);
                if (handle != nullptr) {
                    // Test some informational functions on the live handle
                    pcap_datalink(handle);
                    pcap_snapshot(handle);
                    pcap_major_version(handle);
                    pcap_minor_version(handle);
                    
                    // Try to compile and set a filter
                    struct bpf_program fp;
                    if (pcap_compile(handle, &fp, filter_str.c_str(), optimize, netmask) == 0) {
                        pcap_setfilter(handle, &fp);
                        pcap_freecode(&fp);
                    }
                    
                    // Test pcap_setdirection
                    pcap_setdirection(handle, direction);
                    
                    pcap_close(handle);
                }
            }
            
            if (alldevs != nullptr) {
                pcap_freealldevs(alldevs);
            }
            break;
        }
        
        case 1: {
            // Test pcap_findalldevs with device iteration and pcap_lookupnet
            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                return 0;
            }
            
            // Iterate through all devices and get network info
            for (pcap_if_t* d = alldevs; d != nullptr; d = d->next) {
                bpf_u_int32 net = 0, mask = 0;
                if (pcap_lookupnet(d->name, &net, &mask, errbuf) == 0) {
                    // Network lookup succeeded
                    (void)net;
                    (void)mask;
                }
                
                // Try to open each device
                pcap_t* handle = pcap_open_live(d->name, snaplen, promisc, to_ms, errbuf);
                if (handle != nullptr) {
                    // Test pcap_getnonblock and pcap_setnonblock
                    int nonblock = 0;
                    pcap_getnonblock(handle, errbuf);
                    pcap_setnonblock(handle, fdp.ConsumeBool() ? 1 : 0, errbuf);
                    
                    pcap_close(handle);
                }
            }
            
            if (alldevs != nullptr) {
                pcap_freealldevs(alldevs);
            }
            break;
        }
        
        case 2: {
            // Test pcap_create and pcap_activate with device enumeration
            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                return 0;
            }
            
            if (alldevs != nullptr) {
                // Use pcap_create instead of pcap_open_live
                pcap_t* handle = pcap_create(alldevs->name, errbuf);
                if (handle != nullptr) {
                    // Set various options before activation
                    pcap_set_snaplen(handle, snaplen);
                    pcap_set_promisc(handle, promisc);
                    pcap_set_timeout(handle, to_ms);
                    pcap_set_buffer_size(handle, buffer_size);
                    pcap_set_tstamp_precision(handle, tstamp_precision);
                    pcap_set_tstamp_type(handle, fdp.ConsumeIntegralInRange<int>(0, 10));
                    
                    // Activate the handle
                    if (pcap_activate(handle) >= 0) {
                        // Handle activated successfully
                        pcap_stat stats;
                        pcap_stats(handle, &stats);
                        
                        // Try to set direction
                        pcap_setdirection(handle, direction);
                    }
                    
                    pcap_close(handle);
                }
            }
            
            if (alldevs != nullptr) {
                pcap_freealldevs(alldevs);
            }
            break;
        }
        
        case 3: {
            // Test pcap_findalldevs_ex with remote device enumeration (simulated)
            // Note: We'll simulate remote source strings from fuzzer input
            if (remaining_data.size() > 0) {
                std::string source_str(reinterpret_cast<const char*>(remaining_data.data()),
                                      std::min(remaining_data.size(), (size_t)256));
                
                // Try pcap_findalldevs_ex with the source string
                if (pcap_findalldevs_ex(source_str.c_str(), nullptr, &alldevs, errbuf) == 0) {
                    // Successfully got devices (might be empty list)
                    if (alldevs != nullptr) {
                        pcap_freealldevs(alldevs);
                    }
                }
            }
            break;
        }
        
        case 4: {
            // Test pcap_open_live with various configurations and packet capture
            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                return 0;
            }
            
            if (alldevs != nullptr) {
                pcap_t* handle = pcap_open_live(alldevs->name, snaplen, promisc, to_ms, errbuf);
                if (handle != nullptr) {
                    // Test pcap_loop with limited packet count
                    int packet_count = fdp.ConsumeIntegralInRange<int>(0, 10);
                    pcap_loop(handle, packet_count, packet_handler, nullptr);
                    
                    // Test pcap_dispatch
                    pcap_dispatch(handle, 1, packet_handler, nullptr);
                    
                    // Test pcap_next and pcap_next_ex
                    struct pcap_pkthdr* header;
                    const u_char* pkt_data;
                    int ret = pcap_next_ex(handle, &header, &pkt_data);
                    (void)ret;
                    
                    pcap_close(handle);
                }
                
                pcap_freealldevs(alldevs);
            }
            break;
        }
        
        case 5: {
            // Test pcap_setfilter and error handling
            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                return 0;
            }
            
            if (alldevs != nullptr) {
                pcap_t* handle = pcap_open_live(alldevs->name, snaplen, promisc, to_ms, errbuf);
                if (handle != nullptr) {
                    struct bpf_program fp;
                    
                    // Try to compile filter with various optimize settings
                    for (int opt = 0; opt <= 1; opt++) {
                        if (pcap_compile(handle, &fp, filter_str.c_str(), opt, netmask) == 0) {
                            pcap_setfilter(handle, &fp);
                            pcap_freecode(&fp);
                        }
                    }
                    
                    // Test pcap_geterr
                    const char* err = pcap_geterr(handle);
                    (void)err;
                    
                    pcap_close(handle);
                }
                
                pcap_freealldevs(alldevs);
            }
            break;
        }
        
        case 6: {
            // Test device flags and properties inspection
            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                return 0;
            }
            
            if (alldevs != nullptr) {
                // Iterate and inspect device properties
                for (pcap_if_t* d = alldevs; d != nullptr; d = d->next) {
                    // Check device flags
                    (void)d->flags;
                    
                    // Check if device has description
                    if (d->description != nullptr) {
                        (void)d->description;
                    }
                    
                    // Inspect addresses (if any)
                    for (pcap_addr_t* a = d->addresses; a != nullptr; a = a->next) {
                        if (a->addr != nullptr) {
                            (void)a->addr->sa_family;
                        }
                    }
                }
                
                pcap_freealldevs(alldevs);
            }
            break;
        }
        
        case 7: {
            // Test pcap_can_set_rfmon and pcap_set_rfmon (wireless monitoring)
            if (pcap_findalldevs(&alldevs, errbuf) == -1) {
                return 0;
            }
            
            if (alldevs != nullptr) {
                pcap_t* handle = pcap_create(alldevs->name, errbuf);
                if (handle != nullptr) {
                    // Check if we can set RFMON mode
                    int can_rfmon = pcap_can_set_rfmon(handle);
                    (void)can_rfmon;
                    
                    // Try to set RFMON mode
                    if (can_rfmon == 1) {
                        pcap_set_rfmon(handle, fdp.ConsumeBool() ? 1 : 0);
                    }
                    
                    // Set other options and try to activate
                    pcap_set_snaplen(handle, snaplen);
                    pcap_set_promisc(handle, promisc);
                    pcap_set_timeout(handle, to_ms);
                    
                    pcap_activate(handle);
                    pcap_close(handle);
                }
                
                pcap_freealldevs(alldevs);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_003
