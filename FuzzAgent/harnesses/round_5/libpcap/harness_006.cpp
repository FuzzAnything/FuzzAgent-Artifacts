#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful packet transmission testing
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different packet transmission workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume linktype (DLT) parameter - use a subset of common DLT values
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);
    
    // Consume snaplen parameter
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
    
    // Consume timestamp precision parameter
    u_int tstamp_precision = fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO;
    
    // Consume packet data from fuzzer input
    size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(packet_size);
    
    // Consume remaining bytes for any additional data needs
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    switch (operation) {
        case 0: {
            // Test pcap_set_datalink with pcap_open_dead
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            // Try to set datalink type - should work for pcap_open_dead
            int set_result = pcap_set_datalink(handle, linktype);
            // Result may be 0 (success) or -1 (failure) depending on DLT validity
            // Also test pcap_list_datalinks and pcap_free_datalinks
            int* dlt_list = nullptr;
            int count = pcap_list_datalinks(handle, &dlt_list);
            if (dlt_list != nullptr) {
                pcap_free_datalinks(dlt_list);
            }
            
            pcap_close(handle);
            break;
        }
        
        case 1: {
            // Test pcap_sendpacket with pcap_open_dead (should fail with error)
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            // Attempt to send packet - should fail on dead handle
            if (!packet_data.empty()) {
                int send_result = pcap_sendpacket(handle, packet_data.data(), packet_data.size());
                // Result should be -1 for dead handle
            }
            
            pcap_close(handle);
            break;
        }
        
        case 2: {
            // Test pcap_inject with pcap_open_dead (should fail with error)
            pcap_t* handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, tstamp_precision);
            if (handle == nullptr) {
                return 0;
            }
            
            // Attempt to inject packet - should fail on dead handle
            if (!packet_data.empty()) {
                int inject_result = pcap_inject(handle, packet_data.data(), packet_data.size());
                // Result should be -1 for dead handle
            }
            
            pcap_close(handle);
            break;
        }
        
        case 3: {
            // Test combination of datalink operations and error handling
            pcap_t* handle = pcap_open_dead(linktype, snaplen);
            if (handle == nullptr) {
                return 0;
            }
            
            // Get current datalink
            int current_dlt = pcap_datalink(handle);
            
            // Try to set to a different DLT (if we have remaining data for another DLT value)
            if (!remaining_data.empty()) {
                FuzzedDataProvider remaining_fdp(remaining_data.data(), remaining_data.size());
                int new_dlt = remaining_fdp.ConsumeIntegralInRange<int>(0, 255);
                int set_result = pcap_set_datalink(handle, new_dlt);
            }
            
            // Test pcap_sendpacket and pcap_inject error paths
            if (!packet_data.empty()) {
                // Both should fail on dead handle
                pcap_sendpacket(handle, packet_data.data(), packet_data.size());
                pcap_inject(handle, packet_data.data(), packet_data.size());
            }
            
            // Test datalink name conversion functions
            const char* dlt_name = pcap_datalink_val_to_name(linktype);
            if (dlt_name != nullptr) {
                int dlt_val = pcap_datalink_name_to_val(dlt_name);
                // Just exercise the functions
            }
            
            pcap_close(handle);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_006
