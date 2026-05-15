#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least init options (4 bytes), mode (1 byte),
    // snaplen (2 bytes), flags (4 bytes), timeout (4 bytes), device name (1 byte)
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume initialization options for pcap_init
    unsigned int init_opts = fdp.ConsumeIntegral<unsigned int>();
    // Mask to valid options: PCAP_CHAR_ENC_LOCAL, PCAP_CHAR_ENC_UTF_8, PCAP_MMAP_32BIT
    init_opts &= (PCAP_CHAR_ENC_LOCAL | PCAP_CHAR_ENC_UTF_8 | PCAP_MMAP_32BIT);
    
    // Consume operation mode: 0 = local device, 1 = file, 2 = remote
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume pcap_open parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 262144);  // Reasonable snaplen range
    int flags = fdp.ConsumeIntegral<int>();  // Will mask to valid flags
    int read_timeout = fdp.ConsumeIntegralInRange<int>(-1, 10000);  // Timeout in ms
    
    // Consume device/interface name or source string
    size_t device_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string device_name = fdp.ConsumeBytesAsString(device_len);
    
    // Consume authentication data if needed
    uint8_t auth_type = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0 = NULL, 1 = PWD
    size_t username_len = fdp.ConsumeIntegralInRange<size_t>(0, 64);
    std::string username = fdp.ConsumeBytesAsString(username_len);
    size_t password_len = fdp.ConsumeIntegralInRange<size_t>(0, 64);
    std::string password = fdp.ConsumeBytesAsString(password_len);
    
    // Consume configuration parameters for timestamp and direction
    uint8_t tstamp_precision = fdp.ConsumeIntegral<uint8_t>() % 2;  // 0 = micro, 1 = nano
    uint8_t tstamp_type_choice = fdp.ConsumeIntegral<uint8_t>() % 6;  // 0-5 for different timestamp types
    uint8_t direction_choice = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0-2 for different directions
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    struct pcap_rmtauth auth;
    bpf_u_int32 netp = 0, maskp = 0;
    
    // Step 1: Initialize pcap library
    int init_result = pcap_init(init_opts, errbuf);
    if (init_result != 0) {
        // Initialization failed, but we can still test some APIs
        // However, according to documentation, pcap_lookupdev will fail
        // if pcap_init was called with pcapint_new_api set
    }
    
    // Step 2: Look up network information for device
    // Only attempt if we have a device name
    if (!device_name.empty()) {
        int lookup_result = pcap_lookupnet(device_name.c_str(), &netp, &maskp, errbuf);
        // Result may be -1 on error, 0 on success
        // We'll store the network and mask values for potential use
    }
    
    // Step 3: Create source string based on mode
    std::string source_string;
    struct pcap_rmtauth *auth_ptr = nullptr;
    
    if (mode == 0) {
        // Local device mode
        if (!device_name.empty()) {
            source_string = device_name;
        } else {
            source_string = "any";  // Default device
        }
        auth_ptr = nullptr;  // No auth for local devices
    } else if (mode == 1) {
        // File mode - create a file:// source
        source_string = "file://";
        if (!device_name.empty()) {
            source_string += device_name;
        } else {
            source_string += "/tmp/test.pcap";
        }
        auth_ptr = nullptr;  // No auth for files
    } else {
        // Remote mode - create rpcap:// source
        source_string = "rpcap://";
        if (!device_name.empty()) {
            source_string += device_name;
        } else {
            source_string += "127.0.0.1/";
        }
        
        // Set up authentication if requested
        if (auth_type == 1) {
            auth.type = RPCAP_RMTAUTH_PWD;
            auth.username = const_cast<char*>(username.c_str());
            auth.password = const_cast<char*>(password.c_str());
            auth_ptr = &auth;
        } else {
            auth.type = RPCAP_RMTAUTH_NULL;
            auth.username = nullptr;
            auth.password = nullptr;
            auth_ptr = &auth;
        }
    }
    
    // Mask flags to valid values
    int valid_flags = 0;
    if (flags & PCAP_OPENFLAG_PROMISCUOUS) valid_flags |= PCAP_OPENFLAG_PROMISCUOUS;
    if (flags & PCAP_OPENFLAG_DATATX_UDP) valid_flags |= PCAP_OPENFLAG_DATATX_UDP;
    if (flags & PCAP_OPENFLAG_NOCAPTURE_RPCAP) valid_flags |= PCAP_OPENFLAG_NOCAPTURE_RPCAP;
    if (flags & PCAP_OPENFLAG_NOCAPTURE_LOCAL) valid_flags |= PCAP_OPENFLAG_NOCAPTURE_LOCAL;
    if (flags & PCAP_OPENFLAG_MAX_RESPONSIVENESS) valid_flags |= PCAP_OPENFLAG_MAX_RESPONSIVENESS;
    
    // Step 4: Open pcap handle using pcap_open
    pcap_handle = pcap_open(source_string.c_str(), snaplen, valid_flags, 
                           read_timeout, auth_ptr, errbuf);
    
    if (pcap_handle == nullptr) {
        // pcap_open failed - try to open a dead handle as fallback for API testing
        pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (pcap_handle == nullptr) {
            // Even dead handle failed, can't proceed
            return 0;
        }
    }
    
    // Step 5: Configure timestamp precision if supported
    int precision_result = -1;
    if (tstamp_precision == 0) {
        precision_result = pcap_set_tstamp_precision(pcap_handle, PCAP_TSTAMP_PRECISION_MICRO);
    } else {
        precision_result = pcap_set_tstamp_precision(pcap_handle, PCAP_TSTAMP_PRECISION_NANO);
    }
    // precision_result is 0 on success, -1 on error
    
    // Step 6: Configure timestamp type if supported
    int tstamp_type = PCAP_TSTAMP_HOST;  // Default
    switch (tstamp_type_choice) {
        case 0: tstamp_type = PCAP_TSTAMP_HOST; break;
        case 1: tstamp_type = PCAP_TSTAMP_HOST_LOWPREC; break;
        case 2: tstamp_type = PCAP_TSTAMP_HOST_HIPREC; break;
        case 3: tstamp_type = PCAP_TSTAMP_ADAPTER; break;
        case 4: tstamp_type = PCAP_TSTAMP_ADAPTER_UNSYNCED; break;
        case 5: tstamp_type = PCAP_TSTAMP_HOST_HIPREC_UNSYNCED; break;
    }
    
    int tstamp_result = pcap_set_tstamp_type(pcap_handle, tstamp_type);
    // tstamp_result is 0 on success, -1 on error or unsupported
    
    // Step 7: Configure capture direction
    pcap_direction_t direction = PCAP_D_INOUT;  // Default
    switch (direction_choice) {
        case 0: direction = PCAP_D_INOUT; break;
        case 1: direction = PCAP_D_IN; break;
        case 2: direction = PCAP_D_OUT; break;
    }
    
    int direction_result = pcap_setdirection(pcap_handle, direction);
    // direction_result is 0 on success, -1 on error
    
    // Step 8: Test other configuration APIs that might work
    // Try to get current timestamp precision
    int current_precision = pcap_get_tstamp_precision(pcap_handle);
    
    // Try to list available timestamp types
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    if (tstamp_count > 0 && tstamp_types != nullptr) {
        // We have timestamp types - test conversion functions
        for (int i = 0; i < tstamp_count && i < 3; i++) {  // Limit to 3
            const char* type_name = pcap_tstamp_type_val_to_name(tstamp_types[i]);
            const char* type_desc = pcap_tstamp_type_val_to_description(tstamp_types[i]);
            
            // Test reverse lookup
            if (type_name != nullptr) {
                int type_val = pcap_tstamp_type_name_to_val(type_name);
            }
        }
        // Free the timestamp types array
        pcap_free_tstamp_types(tstamp_types);
    }
    
    // Step 9: Clean up
    pcap_close(pcap_handle);
    
    return 0;
}

} // namespace harness_006
