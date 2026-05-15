#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <memory>
#include <pcap/pcap.h>

namespace harness_011 {


// Helper function to create a minimal valid pcap file header
// Based on TCPDUMP_MAGIC (0xa1b2c3d4) and NSEC_TCPDUMP_MAGIC (0xa1b23c4d)
static std::vector<uint8_t> create_pcap_header(uint32_t magic_number, uint16_t version_major = 2, uint16_t version_minor = 4, 
                                               int32_t thiszone = 0, uint32_t sigfigs = 0, 
                                               uint32_t snaplen = 65535, uint32_t linktype = 1) {
    std::vector<uint8_t> header(24, 0); // pcap_file_header is 24 bytes
    
    // Write magic number (4 bytes)
    header[0] = (magic_number >> 24) & 0xFF;
    header[1] = (magic_number >> 16) & 0xFF;
    header[2] = (magic_number >> 8) & 0xFF;
    header[3] = magic_number & 0xFF;
    
    // Write version major (2 bytes)
    header[4] = (version_major >> 8) & 0xFF;
    header[5] = version_major & 0xFF;
    
    // Write version minor (2 bytes)
    header[6] = (version_minor >> 8) & 0xFF;
    header[7] = version_minor & 0xFF;
    
    // Write thiszone (4 bytes, signed)
    header[8] = (thiszone >> 24) & 0xFF;
    header[9] = (thiszone >> 16) & 0xFF;
    header[10] = (thiszone >> 8) & 0xFF;
    header[11] = thiszone & 0xFF;
    
    // Write sigfigs (4 bytes)
    header[12] = (sigfigs >> 24) & 0xFF;
    header[13] = (sigfigs >> 16) & 0xFF;
    header[14] = (sigfigs >> 8) & 0xFF;
    header[15] = sigfigs & 0xFF;
    
    // Write snaplen (4 bytes)
    header[16] = (snaplen >> 24) & 0xFF;
    header[17] = (snaplen >> 16) & 0xFF;
    header[18] = (snaplen >> 8) & 0xFF;
    header[19] = snaplen & 0xFF;
    
    // Write linktype (4 bytes)
    header[20] = (linktype >> 24) & 0xFF;
    header[21] = (linktype >> 16) & 0xFF;
    header[22] = (linktype >> 8) & 0xFF;
    header[23] = linktype & 0xFF;
    
    return header;
}

// Helper to write buffer to temporary file
static int write_temp_file(const char* filename, const uint8_t* data, size_t size) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return -1;
    
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    
    return (written == size) ? 0 : -2;
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need enough data for: scenario selection, pcap header creation, and some packet data
    if (Size < 32) {
        return 0;  // Not enough input for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: magic number type (0-3 for different pcap magic numbers)
    uint8_t magic_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint32_t magic_number;
    
    switch (magic_type) {
        case 0: magic_number = 0xa1b2c3d4; break;      // TCPDUMP_MAGIC (microsecond)
        case 1: magic_number = 0xa1b23c4d; break;      // NSEC_TCPDUMP_MAGIC (nanosecond)
        case 2: magic_number = 0xa1b2cd34; break;      // KUZNETZOV_TCPDUMP_MAGIC
        case 3: magic_number = fdp.ConsumeIntegral<uint32_t>(); break; // Random magic
    }
    
    // Consume parameters for pcap header
    uint16_t version_major = fdp.ConsumeIntegral<uint8_t>() % 10;
    uint16_t version_minor = fdp.ConsumeIntegral<uint8_t>() % 10;
    int32_t thiszone = fdp.ConsumeIntegral<int32_t>();
    uint32_t sigfigs = fdp.ConsumeIntegral<uint32_t>();
    uint32_t snaplen = fdp.ConsumeIntegralInRange<uint32_t>(1, 65535);
    uint32_t linktype = fdp.ConsumeIntegral<uint8_t>() % 20; // Common link types
    
    // Create pcap header
    std::vector<uint8_t> pcap_header = create_pcap_header(
        magic_number, version_major, version_minor, 
        thiszone, sigfigs, snaplen, linktype
    );
    
    // Use remaining data for packet data (if any)
    std::vector<uint8_t> packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Combine header and packet data
    std::vector<uint8_t> pcap_data = pcap_header;
    pcap_data.insert(pcap_data.end(), packet_data.begin(), packet_data.end());
    
    // Create temporary file
    char filename[] = "/tmp/libpcap_fuzz_savefile_XXXXXX";
    int fd = mkstemp(filename);
    if (fd < 0) {
        return 0;
    }
    close(fd);
    
    // Write pcap data to file
    if (write_temp_file(filename, pcap_data.data(), pcap_data.size()) != 0) {
        unlink(filename);
        return 0;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    pcap_t* pcap_handle = nullptr;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Test pcap_fopen_offline (FILE* based)
            FILE* fp = fopen(filename, "rb");
            if (!fp) {
                unlink(filename);
                return 0;
            }
            
            pcap_handle = pcap_fopen_offline(fp, errbuf);
            
            if (pcap_handle) {
                // Test error retrieval
                const char* error_msg = pcap_geterr(pcap_handle);
                // error_msg could be NULL or contain error string
                
                // Try to read packets
                struct pcap_pkthdr* header;
                const u_char* pkt;
                int packet_count = 0;
                
                int r = pcap_next_ex(pcap_handle, &header, &pkt);
                while (r > 0 && packet_count < 5) { // Limit to 5 packets
                    packet_count++;
                    r = pcap_next_ex(pcap_handle, &header, &pkt);
                }
                
                // Test pcap_next as well
                struct pcap_pkthdr next_header;
                const u_char* next_pkt = pcap_next(pcap_handle, &next_header);
                
                // Test other pcap functions that work on savefiles
                pcap_datalink(pcap_handle);
                pcap_snapshot(pcap_handle);
                pcap_is_swapped(pcap_handle);
                pcap_file(pcap_handle);
                pcap_fileno(pcap_handle);
                
                // Test pcap_strerror with a random error code
                int error_code = fdp.ConsumeIntegralInRange<int>(-100, 100);
                const char* strerror_msg = pcap_strerror(error_code);
                
                pcap_close(pcap_handle);
                // pcap_close() will close the FILE* through pcapint_sf_cleanup()
                // so we should NOT call fclose(fp) here
            } else {
                // Test pcap_strerror on failure
                const char* strerror_msg = pcap_strerror(errno);
                // pcap_fopen_offline failed, so we need to close the FILE* ourselves
                fclose(fp);
            }
            
            break;
        }
        
        case 1: {
            // Scenario 1: Test pcap_fopen_offline_with_tstamp_precision
            FILE* fp = fopen(filename, "rb");
            if (!fp) {
                unlink(filename);
                return 0;
            }
            
            // Use different timestamp precision values
            u_int precision = fdp.ConsumeIntegral<uint8_t>() % 2; // 0 = micro, 1 = nano
            precision = (precision == 0) ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
            
            pcap_handle = pcap_fopen_offline_with_tstamp_precision(fp, precision, errbuf);
            
            if (pcap_handle) {
                // Test timestamp precision retrieval
                u_int actual_precision = pcap_get_tstamp_precision(pcap_handle);
                
                // Read some packets
                struct pcap_pkthdr* header;
                const u_char* pkt;
                int packet_count = 0;
                
                int r = pcap_next_ex(pcap_handle, &header, &pkt);
                while (r > 0 && packet_count < 3) {
                    packet_count++;
                    r = pcap_next_ex(pcap_handle, &header, &pkt);
                }
                
                // Test pcap_loop with a simple handler
                if (packet_count > 0) {
                    auto packet_handler = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                        int* count = (int*)user;
                        (*count)++;
                    };
                    
                    int loop_count = 0;
                    pcap_loop(pcap_handle, 2, packet_handler, (u_char*)&loop_count);
                }
                
                pcap_close(pcap_handle);
                // pcap_close() will close the FILE* through pcapint_sf_cleanup()
                // so we should NOT call fclose(fp) here
            } else {
                // pcap_fopen_offline_with_tstamp_precision failed, so we need to close the FILE* ourselves
                fclose(fp);
            }
            
            break;
        }
        
        case 2: {
            // Scenario 2: Test pcap_open_offline (filename based)
            pcap_handle = pcap_open_offline(filename, errbuf);
            
            if (pcap_handle) {
                // Test pcap_dispatch
                auto dispatch_handler = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                    int* count = (int*)user;
                    (*count)++;
                };
                
                int dispatch_count = 0;
                pcap_dispatch(pcap_handle, 3, dispatch_handler, (u_char*)&dispatch_count);
                
                // Test pcap_stats
                struct pcap_stat stats;
                pcap_stats(pcap_handle, &stats);
                
                // Test pcap_getnonblock and pcap_setnonblock (should fail for savefiles)
                char nonblock_err[PCAP_ERRBUF_SIZE];
                int nonblock_result = pcap_getnonblock(pcap_handle, nonblock_err);
                pcap_setnonblock(pcap_handle, 1, nonblock_err);
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test pcap_open_offline_with_tstamp_precision
            u_int precision = fdp.ConsumeIntegral<uint8_t>() % 2;
            precision = (precision == 0) ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
            
            pcap_handle = pcap_open_offline_with_tstamp_precision(filename, precision, errbuf);
            
            if (pcap_handle) {
                // Test various pcap functions
                u_int actual_precision = pcap_get_tstamp_precision(pcap_handle);
                
                // Try to read with both next and next_ex
                struct pcap_pkthdr header1;
                const u_char* pkt1 = pcap_next(pcap_handle, &header1);
                
                struct pcap_pkthdr* header2;
                const u_char* pkt2;
                int r = pcap_next_ex(pcap_handle, &header2, &pkt2);
                
                // Test pcap_breakloop (should work even if not in a loop)
                pcap_breakloop(pcap_handle);
                
                // Test pcap_setdirection (may fail for savefiles)
                pcap_setdirection(pcap_handle, PCAP_D_INOUT);
                
                pcap_close(pcap_handle);
            }
            break;
        }
    }
    
    // Clean up temporary file
    unlink(filename);
    
    return 0;
}

} // namespace harness_011
