#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pcap/pcap.h>

namespace harness_019 {


// Helper function to write buffer to a temporary file
static int bufferToFile(const char *name, const uint8_t *data, size_t size) {
    FILE *fd;
    if (remove(name) != 0) {
        if (errno != ENOENT) {
            return -1;
        }
    }
    fd = fopen(name, "wb");
    if (fd == NULL) {
        return -2;
    }
    if (fwrite(data, 1, size, fd) != size) {
        fclose(fd);
        return -3;
    }
    fclose(fd);
    return 0;
}

// Helper to create minimal pcap file header
static void write_pcap_header(FILE *fd, uint32_t magic, uint16_t version_major, 
                             uint16_t version_minor, int32_t thiszone, uint32_t sigfigs,
                             uint32_t snaplen, uint32_t network) {
    fwrite(&magic, sizeof(magic), 1, fd);
    fwrite(&version_major, sizeof(version_major), 1, fd);
    fwrite(&version_minor, sizeof(version_minor), 1, fd);
    fwrite(&thiszone, sizeof(thiszone), 1, fd);
    fwrite(&sigfigs, sizeof(sigfigs), 1, fd);
    fwrite(&snaplen, sizeof(snaplen), 1, fd);
    fwrite(&network, sizeof(network), 1, fd);
}

// Helper to create pcap packet header
static void write_packet_header(FILE *fd, uint32_t ts_sec, uint32_t ts_usec,
                               uint32_t incl_len, uint32_t orig_len) {
    fwrite(&ts_sec, sizeof(ts_sec), 1, fd);
    fwrite(&ts_usec, sizeof(ts_usec), 1, fd);
    fwrite(&incl_len, sizeof(incl_len), 1, fd);
    fwrite(&orig_len, sizeof(orig_len), 1, fd);
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need minimum input for meaningful testing
    // At least: scenario + linktype + operation + some pcap data
    if (Size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for comprehensive file/dump operations testing
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: link type for pcap_open_dead
    int linktype = fdp.ConsumeIntegral<uint8_t>();
    
    // Third byte: operation mode for dump (0=open, 1=append, 2=ftell, 3=ftell64)
    uint8_t dump_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Fourth byte: number of dummy packets to write
    uint8_t packet_count = fdp.ConsumeIntegral<uint8_t>() % 10;
    
    // Consume bytes for temporary filenames
    std::string input_filename = "/tmp/libpcap_fuzz_019_input_" + 
                                 std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".pcap";
    std::string output_filename = "/tmp/libpcap_fuzz_019_output_" + 
                                  std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".pcap";
    
    // Consume remaining bytes for pcap file data
    std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    pcap_t *pcap_handle = nullptr;
    pcap_dumper_t *dumper = nullptr;
    FILE *dump_file = nullptr;
    
    // ALWAYS call pcap_lib_version() - this is one of our primary targets
    const char *lib_version = pcap_lib_version();
    // We don't need to use the result, but calling it exercises the code
    
    // Create input pcap file from consumed data
    if (bufferToFile(input_filename.c_str(), pcap_data.data(), pcap_data.size()) != 0) {
        // Clean up temporary files if they exist
        remove(input_filename.c_str());
        remove(output_filename.c_str());
        return 0;
    }
    
    // Try to open the input pcap file
    pcap_handle = pcap_open_offline(input_filename.c_str(), errbuf);
    
    // If we couldn't open a valid pcap file, create a dead pcap handle for testing
    if (pcap_handle == nullptr) {
        pcap_handle = pcap_open_dead(linktype, 65535);
        if (pcap_handle == nullptr) {
            remove(input_filename.c_str());
            remove(output_filename.c_str());
            return 0;
        }
    }
    
    // Test different dump scenarios
    switch (scenario) {
        case 0: {
            // Scenario 0: Test pcap_dump_open_append with file position queries
            if (dump_mode == 0 || dump_mode == 1) {
                // Use pcap_dump_open_append (primary target)
                dumper = pcap_dump_open_append(pcap_handle, output_filename.c_str());
            } else {
                // Regular pcap_dump_open
                dumper = pcap_dump_open(pcap_handle, output_filename.c_str());
            }
            
            if (dumper != nullptr) {
                // Test pcap_dump_ftell (primary target - NOT ftell64)
                long file_pos = pcap_dump_ftell(dumper);
                // We don't need to use the result, but calling exercises the code
                
                // Write some dummy packets to exercise dump functionality
                struct pcap_pkthdr dummy_header;
                memset(&dummy_header, 0, sizeof(dummy_header));
                dummy_header.caplen = 64;
                dummy_header.len = 64;
                
                for (int i = 0; i < packet_count; i++) {
                    std::vector<uint8_t> dummy_data(64, i);
                    pcap_dump((u_char *)dumper, &dummy_header, dummy_data.data());
                    
                    // Test pcap_dump_ftell after each write
                    long new_pos = pcap_dump_ftell(dumper);
                }
                
                pcap_dump_flush(dumper);
                pcap_dump_close(dumper);
                dumper = nullptr;
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test pcap_dump_fopen with both ftell and ftell64
            dump_file = fopen(output_filename.c_str(), "wb");
            if (dump_file != nullptr) {
                dumper = pcap_dump_fopen(pcap_handle, dump_file);
                // pcap_dump_fopen takes ownership of dump_file if successful
                if (dumper != nullptr) {
                    // dumper now owns the FILE*, so we should not close it ourselves
                    dump_file = nullptr; // Clear our reference since dumper owns it now
                    
                    // Test both pcap_dump_ftell and pcap_dump_ftell64
                    long file_pos = pcap_dump_ftell(dumper);
                    int64_t file_pos64 = pcap_dump_ftell64(dumper);
                    
                    // Write some data
                    struct pcap_pkthdr dummy_header;
                    memset(&dummy_header, 0, sizeof(dummy_header));
                    dummy_header.caplen = 32;
                    dummy_header.len = 32;
                    
                    std::vector<uint8_t> dummy_data(32, 0xAA);
                    pcap_dump((u_char *)dumper, &dummy_header, dummy_data.data());
                    
                    // Test positions again after write
                    long new_pos = pcap_dump_ftell(dumper);
                    int64_t new_pos64 = pcap_dump_ftell64(dumper);
                    
                    pcap_dump_flush(dumper);
                    pcap_dump_close(dumper);
                    dumper = nullptr;
                } else {
                    // pcap_dump_fopen failed, we still own dump_file and must close it
                    fclose(dump_file);
                    dump_file = nullptr;
                }
                // Note: If dumper was created, pcap_dump_close already closed the FILE*
                // If dumper creation failed, we closed it above
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Create a valid pcap file first, then test append operations
            // First create a minimal valid pcap file
            FILE *valid_pcap = fopen(output_filename.c_str(), "wb");
            if (valid_pcap != nullptr) {
                // Write pcap file header
                write_pcap_header(valid_pcap, 0xa1b2c3d4, 2, 4, 0, 0, 65535, 1);
                
                // Write a few packets
                for (int i = 0; i < 3; i++) {
                    write_packet_header(valid_pcap, 1000 + i, 500000, 64, 64);
                    std::vector<uint8_t> packet_data(64, i * 10);
                    fwrite(packet_data.data(), 1, 64, valid_pcap);
                }
                fclose(valid_pcap);
                
                // Now test pcap_dump_open_append on this valid file
                dumper = pcap_dump_open_append(pcap_handle, output_filename.c_str());
                
                if (dumper != nullptr) {
                    // Test pcap_dump_ftell on appended file
                    long append_pos = pcap_dump_ftell(dumper);
                    
                    // Add more data
                    struct pcap_pkthdr new_header;
                    memset(&new_header, 0, sizeof(new_header));
                    new_header.caplen = 128;
                    new_header.len = 128;
                    
                    std::vector<uint8_t> new_data(128, 0xFF);
                    pcap_dump((u_char *)dumper, &new_header, new_data.data());
                    
                    // Test position after append
                    long new_append_pos = pcap_dump_ftell(dumper);
                    
                    pcap_dump_flush(dumper);
                    pcap_dump_close(dumper);
                    dumper = nullptr;
                }
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test edge cases and error conditions
            // Try pcap_dump_open_append with invalid filename
            std::string invalid_filename = "";
            if (fdp.remaining_bytes() > 0) {
                invalid_filename = fdp.ConsumeRandomLengthString(50);
            }
            
            // This should fail, but we're testing the error path
            pcap_dumper_t *bad_dumper = pcap_dump_open_append(pcap_handle, invalid_filename.c_str());
            if (bad_dumper == nullptr) {
                // Expected failure - test pcap_dump_ftell on nullptr (should handle gracefully in actual code)
                // We won't call it to avoid crashes
            } else {
                // Unexpected success - test functions anyway
                long pos = pcap_dump_ftell(bad_dumper);
                pcap_dump_close(bad_dumper);
            }
            
            // Also test pcap_dump_file to get FILE pointer
            if (dumper == nullptr) {
                // Create a regular dumper for this test
                dumper = pcap_dump_open(pcap_handle, output_filename.c_str());
                if (dumper != nullptr) {
                    FILE *dumper_file = pcap_dump_file(dumper);
                    // Test pcap_dump_ftell on valid dumper
                    long file_pos = pcap_dump_ftell(dumper);
                    
                    pcap_dump_close(dumper);
                    dumper = nullptr;
                }
            }
            break;
        }
    }
    
    // Clean up pcap handle
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
        pcap_handle = nullptr;
    }
    
    // Clean up temporary files
    remove(input_filename.c_str());
    remove(output_filename.c_str());
    
    return 0;
}

} // namespace harness_019
