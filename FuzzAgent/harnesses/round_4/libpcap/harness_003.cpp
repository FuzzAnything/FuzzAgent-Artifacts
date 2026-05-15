#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>

namespace harness_003 {


int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need minimum input for meaningful testing
    // At least 1 byte for scenario selection + some for errbuf
    if (Size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: flags for device iteration
    uint8_t max_devices = fdp.ConsumeIntegral<uint8_t>();
    if (max_devices == 0) max_devices = 10; // Default limit
    
    // Third byte: network lookup flags
    uint8_t network_lookup_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume some bytes for error buffer initialization
    std::vector<char> errbuf_data = fdp.ConsumeBytes<char>(PCAP_ERRBUF_SIZE);
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    // Copy consumed data into errbuf, but ensure null termination
    size_t copy_len = std::min(errbuf_data.size(), sizeof(errbuf) - 1);
    if (copy_len > 0) {
        memcpy(errbuf, errbuf_data.data(), copy_len);
        errbuf[copy_len] = '\0';
    }
    
    pcap_if_t *alldevs = nullptr;
    int ret;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Basic pcap_findalldevs -> pcap_freealldevs
            ret = pcap_findalldevs(&alldevs, errbuf);
            if (ret == 0 && alldevs != nullptr) {
                // Successfully found devices, free them
                pcap_freealldevs(alldevs);
                alldevs = nullptr;
            }
            break;
        }
        
        case 1: {
            // Scenario 1: pcap_findalldevs -> iterate devices -> pcap_lookupnet for each -> pcap_freealldevs
            ret = pcap_findalldevs(&alldevs, errbuf);
            if (ret == 0 && alldevs != nullptr) {
                pcap_if_t *dev = alldevs;
                int device_count = 0;
                
                // Iterate through devices (limit to prevent excessive iteration)
                while (dev != nullptr && device_count < max_devices) {
                    bpf_u_int32 netp = 0, maskp = 0;
                    char local_errbuf[PCAP_ERRBUF_SIZE];
                    memset(local_errbuf, 0, sizeof(local_errbuf));
                    
                    // Try to get network info for this device
                    int lookup_ret = pcap_lookupnet(dev->name, &netp, &maskp, local_errbuf);
                    // Result doesn't matter for fuzzing - we're exercising the code paths
                    
                    dev = dev->next;
                    device_count++;
                }
                
                pcap_freealldevs(alldevs);
                alldevs = nullptr;
            }
            break;
        }
        
        case 2: {
            // Scenario 2: pcap_lookupdev (deprecated but still testable) -> pcap_findalldevs -> pcap_freealldevs
            char lookupdev_errbuf[PCAP_ERRBUF_SIZE];
            memset(lookupdev_errbuf, 0, sizeof(lookupdev_errbuf));
            
            // Consume some data for lookupdev buffer
            std::vector<char> lookupdev_buf = fdp.ConsumeBytes<char>(PCAP_ERRBUF_SIZE);
            if (!lookupdev_buf.empty()) {
                // pcap_lookupdev expects a char* buffer, but it's deprecated
                // We'll still call it to exercise the code path
                char *default_dev = pcap_lookupdev(lookupdev_errbuf);
                // Note: pcap_lookupdev may return NULL if no default device
                // We don't need to use the result
            }
            
            // Now do the modern approach
            ret = pcap_findalldevs(&alldevs, errbuf);
            if (ret == 0 && alldevs != nullptr) {
                pcap_freealldevs(alldevs);
                alldevs = nullptr;
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Comprehensive test - find devices, lookup networks, test edge cases
            ret = pcap_findalldevs(&alldevs, errbuf);
            
            if (ret == 0 && alldevs != nullptr) {
                pcap_if_t *dev = alldevs;
                int device_count = 0;
                
                while (dev != nullptr && device_count < max_devices) {
                    // Test with NULL device name (should fail)
                    if ((network_lookup_flags & 0x01) && device_count == 0) {
                        bpf_u_int32 netp = 0, maskp = 0;
                        char null_errbuf[PCAP_ERRBUF_SIZE];
                        memset(null_errbuf, 0, sizeof(null_errbuf));
                        pcap_lookupnet(nullptr, &netp, &maskp, null_errbuf);
                    }
                    
                    // Test with empty device name
                    if ((network_lookup_flags & 0x02) && device_count == 1) {
                        bpf_u_int32 netp = 0, maskp = 0;
                        char empty_errbuf[PCAP_ERRBUF_SIZE];
                        memset(empty_errbuf, 0, sizeof(empty_errbuf));
                        pcap_lookupnet("", &netp, &maskp, empty_errbuf);
                    }
                    
                    // Normal lookup for actual device
                    bpf_u_int32 netp = 0, maskp = 0;
                    char dev_errbuf[PCAP_ERRBUF_SIZE];
                    memset(dev_errbuf, 0, sizeof(dev_errbuf));
                    
                    if (dev->name != nullptr) {
                        pcap_lookupnet(dev->name, &netp, &maskp, dev_errbuf);
                    }
                    
                    dev = dev->next;
                    device_count++;
                }
                
                pcap_freealldevs(alldevs);
                alldevs = nullptr;
            } else {
                // No devices found or error - test pcap_lookupdev as fallback
                char fallback_errbuf[PCAP_ERRBUF_SIZE];
                memset(fallback_errbuf, 0, sizeof(fallback_errbuf));
                char *default_dev = pcap_lookupdev(fallback_errbuf);
                // Don't need to use the result
            }
            break;
        }
    }
    
    // Final cleanup (in case we missed something)
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    return 0;
}

} // namespace harness_003
