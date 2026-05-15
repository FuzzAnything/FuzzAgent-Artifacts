#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

namespace harness_007 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least operation mode (1 byte),
    // device name length (1 byte), snaplen (2 bytes), promisc flag (1 byte),
    // timeout (2 bytes), packet operation mode (1 byte), 
    // packet size (2 bytes) = 10 bytes minimum
    if (size < 10) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode for device discovery
    uint8_t dev_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume device name length and string
    size_t dev_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
    std::string device_name = fdp.ConsumeBytesAsString(dev_name_len);
    
    // Consume capture parameters for pcap_open_live
    int snaplen = fdp.ConsumeIntegralInRange<int>(64, 65535);  // Reasonable snaplen range
    int promisc = fdp.ConsumeBool() ? 1 : 0;  // Promiscuous mode flag
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 10000);  // Timeout in milliseconds
    
    // Consume configuration options
    uint8_t config_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3 configuration combinations
    bool test_nonblock = (config_mode & 0x01) != 0;
    bool test_direction = (config_mode & 0x02) != 0;
    
    // Consume packet operation mode
    uint8_t packet_op = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0=inject, 1=sendpacket, 2=both
    
    // Consume packet size (for test packets)
    size_t max_packet_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    // Remaining bytes are for packet data
    std::vector<uint8_t> packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (packet_data.empty()) {
        return 0;  // Need some packet data for injection
    }
    
    // Ensure packet data doesn't exceed reasonable size
    if (packet_data.size() > max_packet_size) {
        packet_data.resize(max_packet_size);
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    pcap_if_t *alldevs = nullptr;
    const char *device = nullptr;
    
    // Step 1: Device discovery (similar to harness_003 but focused on injection)
    if (dev_mode == 0) {
        // Use provided device name
        device = device_name.c_str();
    } else {
        // Try to find actual devices
        if (pcap_findalldevs(&alldevs, errbuf) == -1) {
            // Device discovery failed, use provided name
            device = device_name.c_str();
        } else if (alldevs != nullptr) {
            // Use the first device found if available
            device = alldevs->name;
        } else {
            // No devices found, use provided name
            device = device_name.c_str();
        }
    }
    
    // Step 2: Create live capture handle for packet injection
    pcap_handle = pcap_open_live(device, snaplen, promisc, timeout_ms, errbuf);
    
    if (pcap_handle == nullptr) {
        // If live capture fails, try with loopback as fallback
        pcap_handle = pcap_open_live("lo", 65535, 0, 1000, errbuf);
        if (pcap_handle == nullptr) {
            // If still fails, use pcap_open_dead as last resort
            // Note: pcap_open_dead handles don't support injection, but we can test other APIs
            pcap_handle = pcap_open_dead(DLT_EN10MB, snaplen);
        }
    }
    
    if (pcap_handle == nullptr) {
        // Clean up device list if we used pcap_findalldevs
        if (alldevs != nullptr) {
            pcap_freealldevs(alldevs);
        }
        return 0;  // Can't proceed without a handle
    }
    
    // Step 3: Test configuration APIs (optional based on config_mode)
    if (test_nonblock) {
        // Test non-blocking mode APIs
        char nonblock_err[PCAP_ERRBUF_SIZE];
        
        // Try to set non-blocking mode
        int set_result = pcap_setnonblock(pcap_handle, 1, nonblock_err);
        
        // Get non-blocking mode status
        int get_result = pcap_getnonblock(pcap_handle, nonblock_err);
        
        // Try toggling back
        if (set_result == 0) {
            pcap_setnonblock(pcap_handle, 0, nonblock_err);
        }
    }
    
    if (test_direction) {
        // Test direction setting APIs
        // Note: pcap_direction_t is an enum, use PCAP_D_INOUT as a reasonable value
        pcap_setdirection(pcap_handle, PCAP_D_INOUT);
        // Could also test PCAP_D_IN or PCAP_D_OUT if we had more input
    }
    
    // Step 4: Test packet injection APIs (primary targets)
    int inject_result = -1;
    int sendpacket_result = -1;
    
    switch (packet_op) {
        case 0:  // Test pcap_inject only
            inject_result = pcap_inject(pcap_handle, packet_data.data(), packet_data.size());
            break;
            
        case 1:  // Test pcap_sendpacket only
            // pcap_sendpacket expects int size, ensure it fits
            if (packet_data.size() <= INT_MAX) {
                sendpacket_result = pcap_sendpacket(pcap_handle, 
                                                   reinterpret_cast<const u_char*>(packet_data.data()), 
                                                   static_cast<int>(packet_data.size()));
            }
            break;
            
        case 2:  // Test both APIs
            // First inject
            inject_result = pcap_inject(pcap_handle, packet_data.data(), packet_data.size());
            
            // Then sendpacket (with potentially modified or same data)
            if (packet_data.size() <= INT_MAX) {
                sendpacket_result = pcap_sendpacket(pcap_handle, 
                                                   reinterpret_cast<const u_char*>(packet_data.data()), 
                                                   static_cast<int>(packet_data.size()));
            }
            break;
    }
    
    // Step 5: Test edge cases for injection APIs
    
    // Test with empty buffer (should fail)
    if (packet_data.size() > 0) {
        // Test with zero size (should fail according to pcap_inject implementation)
        // Note: We don't check result as it's expected to fail
        pcap_inject(pcap_handle, packet_data.data(), 0);
        
        // Test with NULL buffer (should crash or fail)
        // Note: This is dangerous, but we're fuzzing
        // pcap_inject(pcap_handle, nullptr, packet_data.size());
    }
    
    // Test with various packet sizes
    if (packet_data.size() >= 2) {
        // Test with small packet
        size_t small_size = packet_data.size() > 1 ? 1 : packet_data.size();
        pcap_inject(pcap_handle, packet_data.data(), small_size);
        
        // Test with medium packet (half size)
        size_t medium_size = packet_data.size() / 2;
        if (medium_size > 0) {
            pcap_inject(pcap_handle, packet_data.data(), medium_size);
        }
    }
    
    // Step 6: Test injection with different data patterns
    
    // Create alternate packet data by modifying first byte
    if (!packet_data.empty()) {
        std::vector<uint8_t> modified_packet = packet_data;
        if (modified_packet.size() > 0) {
            modified_packet[0] = ~modified_packet[0];  // Invert first byte
        }
        
        // Test injection with modified data
        pcap_inject(pcap_handle, modified_packet.data(), 
                   std::min(modified_packet.size(), static_cast<size_t>(1024)));
    }
    
    // Step 7: Clean up
    pcap_close(pcap_handle);
    
    // Clean up device list if we used pcap_findalldevs
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    return 0;
}

} // namespace harness_007
