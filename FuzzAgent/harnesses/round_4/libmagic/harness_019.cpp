#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "magic.h"

namespace harness_019 {
/*
 * Fuzzing harness for libmagic library - CDF/Office Document Format Parsing
 * Targets: cdf_unpack_summary_info() (152 blocked branches) and other CDF processing functions
 * MODULE: CDF module (Compound Document Format for Office documents)
 * STRATEGY: Target CDF/Office document format parsing with specific Office document test data
 *           (DOC, XLS, PPT formats) to reach untouched CDF processing functions
 * DIFFERENTIATION: Unlike existing 19 harnesses, focus specifically on CDF/Office document 
 *           format parsing - use Office document data as input rather than regular files, 
 *           magic files, or ELF data
 * API SEQUENCE: magic_open() -> magic_buffer() with Office document data -> magic_close()
 * Goal: Reach untouched CDF processing functions with 0% coverage, particularly 
 *       cdf_unpack_summary_info() in cdf.c and cdf_time.c functions
 */


// Include libmagic header

// CDF structure definitions from cdf.h
#define CDF_MAGIC 0xE11AB1A1E011CFD0LL

// Simplified CDF header structure for test generation
typedef struct {
    uint64_t h_magic;
    uint64_t h_uuid[2];
    uint16_t h_revision;
    uint16_t h_version;
    uint16_t h_byte_order;
    uint16_t h_sec_size_p2;
    uint16_t h_short_sec_size_p2;
    uint8_t  h_unused0[10];
    uint32_t h_num_sectors_in_sat;
    uint32_t h_secid_first_directory;
    uint8_t  h_unused1[4];
    uint32_t h_min_size_standard_stream;
    int32_t  h_secid_first_sector_in_short_sat;
    uint32_t h_num_sectors_in_short_sat;
    int32_t  h_secid_first_sector_in_master_sat;
    uint32_t h_num_sectors_in_master_sat;
    int32_t  h_master_sat[436/4];
} cdf_test_header_t;

// Office document type identifiers
#define OFFICE_DOC  0
#define OFFICE_XLS  1
#define OFFICE_PPT  2
#define OFFICE_MSI  3

// Function to generate CDF-like Office document data
std::vector<uint8_t> generate_cdf_office_data(FuzzedDataProvider& fdp, uint8_t office_type) {
    std::vector<uint8_t> buffer;
    
    // Start with minimum required size
    size_t base_size = 512;
    if (fdp.remaining_bytes() < base_size) {
        base_size = fdp.remaining_bytes();
    }
    
    // Consume base data
    std::vector<uint8_t> base_data = fdp.ConsumeBytes<uint8_t>(base_size);
    buffer.insert(buffer.end(), base_data.begin(), base_data.end());
    
    // Ensure we have at least enough for CDF header
    if (buffer.size() < sizeof(cdf_test_header_t)) {
        buffer.resize(sizeof(cdf_test_header_t));
    }
    
    // Create CDF header at the beginning
    cdf_test_header_t* header = reinterpret_cast<cdf_test_header_t*>(buffer.data());
    
    // Set CDF magic number (little-endian)
    header->h_magic = CDF_MAGIC;
    
    // Set version information typical for Office documents
    header->h_revision = 0x003E;  // Typical Office revision
    header->h_version = 0x0003;   // Version 3
    
    // Set byte order (0xFFFE for little-endian)
    header->h_byte_order = 0xFFFE;
    
    // Set sector size (512 bytes = 2^9, so 9)
    header->h_sec_size_p2 = 9;
    
    // Set short sector size (64 bytes = 2^6, so 6)
    header->h_short_sec_size_p2 = 6;
    
    // Set sector allocation table info
    header->h_num_sectors_in_sat = fdp.ConsumeIntegral<uint32_t>();
    header->h_secid_first_directory = fdp.ConsumeIntegral<uint32_t>();
    
    // Set stream size thresholds
    header->h_min_size_standard_stream = 4096; // Typical Office threshold
    
    // Set short SAT info
    header->h_secid_first_sector_in_short_sat = fdp.ConsumeIntegral<int32_t>();
    header->h_num_sectors_in_short_sat = fdp.ConsumeIntegral<uint32_t>();
    
    // Set master SAT info
    header->h_secid_first_sector_in_master_sat = fdp.ConsumeIntegral<int32_t>();
    header->h_num_sectors_in_master_sat = fdp.ConsumeIntegral<uint32_t>();
    
    // Fill master SAT array with some valid and some free sectors
    for (int i = 0; i < 436/4; i++) {
        uint8_t sat_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (sat_type) {
            case 0:
                header->h_master_sat[i] = -1; // CDF_SECID_FREE
                break;
            case 1:
                header->h_master_sat[i] = -2; // CDF_SECID_END_OF_CHAIN
                break;
            case 2:
                header->h_master_sat[i] = -3; // CDF_SECID_SECTOR_ALLOCATION_TABLE
                break;
            default:
                header->h_master_sat[i] = fdp.ConsumeIntegralInRange<int32_t>(0, 1000);
                break;
        }
    }
    
    // Add Office-specific structures after header
    size_t office_data_size = fdp.ConsumeIntegralInRange<size_t>(256, 2048);
    if (fdp.remaining_bytes() < office_data_size) {
        office_data_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> office_data = fdp.ConsumeBytes<uint8_t>(office_data_size);
    buffer.insert(buffer.end(), office_data.begin(), office_data.end());
    
    // Add summary info section (targets cdf_unpack_summary_info)
    // Summary info typically appears in Office documents
    if (buffer.size() >= 1024 && fdp.remaining_bytes() >= 256) {
        size_t summary_offset = 1024;
        if (buffer.size() > summary_offset + 256) {
            // Add summary info header magic
            uint16_t si_magic = 0xFFFE; // Little-endian marker
            memcpy(buffer.data() + summary_offset, &si_magic, sizeof(si_magic));
            
            // Add OS version
            uint16_t os_version = 0x0100; // Windows
            memcpy(buffer.data() + summary_offset + 2, &os_version, sizeof(os_version));
            
            // Add OS identifier
            uint16_t os_id = 0x0002; // Windows NT
            memcpy(buffer.data() + summary_offset + 4, &os_id, sizeof(os_id));
            
            // Add class ID (CLSID)
            uint8_t clsid[16];
            for (int i = 0; i < 16; i++) {
                clsid[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            memcpy(buffer.data() + summary_offset + 6, clsid, sizeof(clsid));
            
            // Add property count
            uint32_t prop_count = fdp.ConsumeIntegral<uint32_t>() % 100;
            memcpy(buffer.data() + summary_offset + 22, &prop_count, sizeof(prop_count));
        }
    }
    
    // Add directory entries (simplified)
    if (buffer.size() >= 2048 && fdp.remaining_bytes() >= 128) {
        size_t dir_offset = 2048;
        
        // Add directory entry count
        uint32_t dir_count = 4; // Typical for Office docs
        memcpy(buffer.data() + dir_offset, &dir_count, sizeof(dir_count));
        
        // Add some directory entries
        for (int i = 0; i < 4 && (dir_offset + 128 + i*64) < buffer.size(); i++) {
            size_t entry_offset = dir_offset + 4 + i*64;
            
            // Entry name (WordDocument, Workbook, PowerPoint, etc.)
            const char* entry_names[] = {"WordDocument", "Workbook", "PowerPoint", "CompObj"};
            if (i < 4) {
                const char* name = entry_names[i];
                size_t name_len = strlen(name);
                for (size_t j = 0; j < name_len && j < 32; j++) {
                    buffer[entry_offset + j*2] = name[j];
                    buffer[entry_offset + j*2 + 1] = 0; // Unicode high byte
                }
            }
            
            // Entry type
            buffer[entry_offset + 64] = 2; // User stream
            buffer[entry_offset + 65] = fdp.ConsumeIntegral<uint8_t>(); // Color
            buffer[entry_offset + 66] = fdp.ConsumeIntegral<uint8_t>(); // Left sibling
            buffer[entry_offset + 67] = fdp.ConsumeIntegral<uint8_t>(); // Right sibling
            buffer[entry_offset + 68] = fdp.ConsumeIntegral<uint8_t>(); // Child
        }
    }
    
    return buffer;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - CDF parsing needs substantial data
    if (size < 1024) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - focus on CDF parsing flags
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;          // Enable debugging for parser errors
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;       // Important for compressed Office docs
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;      // Get MIME type for Office docs
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;       // Continue on errors
    if (flag_byte & 0x40) flags |= MAGIC_CHECK;
    if (flag_byte & 0x80) flags |= MAGIC_RAW;
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == NULL) {
        return 0; // Failed to open magic handle
    }
    
    // Try to load magic database - use default path
    const char* magic_path = NULL; // NULL means use default
    int load_result = magic_load(magic, magic_path);
    
    // If loading fails, close and return
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Choose Office document type to test
    uint8_t office_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Generate CDF/Office document test data
    std::vector<uint8_t> office_data = generate_cdf_office_data(fdp, office_type);
    
    if (!office_data.empty()) {
        // Test magic_buffer with Office document data
        const char* result = magic_buffer(magic, office_data.data(), office_data.size());
        
        // Result can be NULL or contain Office document identification
        // The goal is to exercise CDF parsing code paths
        (void)result; // Avoid unused variable warning
    }
    
    // Also test with raw fuzzer input (may contain partial Office docs)
    size_t remaining_size = fdp.ConsumeIntegralInRange<size_t>(256, fdp.remaining_bytes());
    std::vector<uint8_t> raw_data = fdp.ConsumeBytes<uint8_t>(remaining_size);
    
    if (!raw_data.empty()) {
        // Try magic_buffer with raw data (may trigger different CDF parsing paths)
        const char* raw_result = magic_buffer(magic, raw_data.data(), raw_data.size());
        (void)raw_result;
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_019
