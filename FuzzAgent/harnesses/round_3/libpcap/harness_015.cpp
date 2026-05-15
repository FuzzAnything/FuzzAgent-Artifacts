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

namespace harness_015 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing of packet file reading operations
    // Need enough data for pcap file content, control parameters, and testing scenarios
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size control parameters first
    // These determine the behavior of the packet file reading test
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 2);  // Which opening method to use
    int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=microseconds, 1=nanoseconds
    int max_packets = fdp.ConsumeIntegralInRange<int>(1, 50);  // Maximum packets to read
    int breakloop_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);  // When to call pcap_breakloop
    
    // Consume remaining bytes as potential pcap file content
    std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (pcap_data.size() < 24) {  // Minimum pcap header size
        return 0;
    }
    
    // Create temporary file for testing
    char temp_filename[] = "/tmp/libpcap_file_read_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    
    // Write pcap data to temporary file
    if (write(fd, pcap_data.data(), pcap_data.size()) != static_cast<ssize_t>(pcap_data.size())) {
        close(fd);
        unlink(temp_filename);
        return 0;
    }
    close(fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    
    // Step 1: Open pcap file handle using different methods
    // PRIMARY TARGET: pcap_open_offline (Required Helper: Init, 11 undiscovered branches)
    // PRIMARY TARGET: pcap_open_offline_with_tstamp_precision (Alternative init, 11 undiscovered branches)
    switch (test_scenario) {
        case 0:
            // Basic file path opening
            pcap_handle = pcap_open_offline(temp_filename, errbuf);
            break;
            
        case 1:
            // File path opening with timestamp precision
            pcap_handle = pcap_open_offline_with_tstamp_precision(temp_filename, tstamp_precision, errbuf);
            break;
            
        case 2:
            // FILE* based opening (tests different code path)
            FILE* f = fopen(temp_filename, "rb");
            if (f == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            pcap_handle = pcap_fopen_offline(f, errbuf);
            // Note: pcap_fopen_offline takes ownership of the FILE*
            break;
    }
    
    if (pcap_handle == nullptr) {
        // Opening failed, test error handling
        const char* error_str = pcap_strerror(PCAP_ERROR);
        (void)error_str;
        
        // Clean up and return
        unlink(temp_filename);
        return 0;
    }
    
    // Step 2: Test file descriptor inspection APIs
    // PRIMARY TARGET: pcap_file (Uncovered: 0% coverage)
    // PRIMARY TARGET: pcap_fileno (Uncovered: 0% coverage)
    FILE* pcap_file_ptr = pcap_file(pcap_handle);
    (void)pcap_file_ptr;  // Use to avoid unused variable warning
    
    int pcap_fd = pcap_fileno(pcap_handle);
    (void)pcap_fd;  // Use to avoid unused variable warning
    
    // Step 3: Read packets in a loop with pcap_next_ex
    // PRIMARY TARGET: pcap_next_ex (Primary Target: 0% function lines hit, 4 undiscovered branches)
    int packets_read = 0;
    bool breakloop_called = false;
    bool should_break = false;
    
    for (int i = 0; i < max_packets && !should_break; i++) {
        struct pcap_pkthdr* packet_header = nullptr;
        const u_char* packet_data = nullptr;
        
        int next_result = pcap_next_ex(pcap_handle, &packet_header, &packet_data);
        
        // Handle different return values from pcap_next_ex
        // This tests the 4 undiscovered branches in pcap_next_ex
        switch (next_result) {
            case 1:  // Packet read successfully
                packets_read++;
                
                // Access packet data if available
                if (packet_header != nullptr && packet_data != nullptr) {
                    // Test packet header fields
                    size_t packet_len = packet_header->len;
                    size_t capture_len = packet_header->caplen;
                    struct timeval timestamp = packet_header->ts;
                    
                    (void)packet_len;
                    (void)capture_len;
                    (void)timestamp;
                    
                    // Test packet data access (just a simple check)
                    if (packet_len > 0) {
                        uint8_t first_byte = packet_data[0];
                        (void)first_byte;
                    }
                }
                break;
                
            case 0:  // Timeout expired (should not happen with offline files)
                // This return code is for live captures only
                // For offline files, this indicates no timeout mechanism
                break;
                
            case -1:  // Error occurred
                {
                    // Get error string for the error
                    const char* capture_error = pcap_geterr(pcap_handle);
                    (void)capture_error;
                    
                    // Test pcap_perror as well
                    pcap_perror(pcap_handle, "pcap_next_ex error");
                }
                break;
                
            case -2:  // EOF from savefile
                // This is the expected return when we reach end of file
                should_break = true;
                break;
        }
        
        // Step 4: Test pcap_breakloop functionality
        // PRIMARY TARGET: pcap_breakloop (Uncovered: 0% coverage)
        // Call breakloop based on the breakloop_scenario
        if (!breakloop_called) {
            switch (breakloop_scenario) {
                case 0:
                    // Call breakloop after first packet
                    if (packets_read >= 1) {
                        pcap_breakloop(pcap_handle);
                        breakloop_called = true;
                        // After calling breakloop, pcap_next_ex should return -2 on next call
                        // for offline files, or -2 for live captures
                    }
                    break;
                    
                case 1:
                    // Call breakloop in the middle of reading
                    if (i == max_packets / 2) {
                        pcap_breakloop(pcap_handle);
                        breakloop_called = true;
                    }
                    break;
                    
                case 2:
                    // Call breakloop at the end
                    if (i == max_packets - 1) {
                        pcap_breakloop(pcap_handle);
                        breakloop_called = true;
                    }
                    break;
                    
                case 3:
                    // Never call breakloop (control scenario)
                    break;
            }
        }
        
        // If breakloop was called, the next pcap_next_ex should return -2
        // We'll let the loop continue to test this behavior
    }
    
    // Step 5: Test additional file operations after reading
    // Test pcap_file and pcap_fileno again after some packets have been read
    FILE* pcap_file_after = pcap_file(pcap_handle);
    (void)pcap_file_after;
    
    int pcap_fd_after = pcap_fileno(pcap_handle);
    (void)pcap_fd_after;
    
    // Test pcap_is_swapped for endianness detection
    int swapped = pcap_is_swapped(pcap_handle);
    (void)swapped;
    
    // Test datalink type
    int datalink = pcap_datalink(pcap_handle);
    (void)datalink;
    
    // Test snapshot length
    int snaplen = pcap_snapshot(pcap_handle);
    (void)snaplen;
    
    // Step 6: Clean up resources
    // Required helper: pcap_close (Required Helper: Cleanup)
    pcap_close(pcap_handle);
    
    // Step 7: Test error cases with invalid inputs
    // Try to open the same file again but with invalid precision
    if (fdp.remaining_bytes() == 0) {  // Avoid infinite loops
        pcap_t* invalid_pcap = pcap_open_offline_with_tstamp_precision(temp_filename, 999, errbuf);
        if (invalid_pcap != nullptr) {
            // Should not happen, but clean up if it does
            pcap_close(invalid_pcap);
        }
        
        // Try to open non-existent file
        invalid_pcap = pcap_open_offline("/nonexistent/file.pcap", errbuf);
        if (invalid_pcap != nullptr) {
            pcap_close(invalid_pcap);
        }
    }
    
    // Clean up temporary file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_015
