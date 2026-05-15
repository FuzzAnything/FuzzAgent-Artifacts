#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <unistd.h>

namespace harness_006 {
/*
 * Specialized fuzzing harness for libmagic targeting CDF (Compound Document Format) module
 * 
 * Primary targets: 
 * - CDF module with 580+ blocked branches in cdf_check_summary_info, cdf_file_summary_info,
 *   cdf_file_catalog_info, cdf_file_property_info, cdf_unpack_summary_info, cdf_read_property_info
 * - Functions reachable via file_trycdf -> cdf_read_summary_info -> cdf_check_summary_info
 * 
 * APIs targeted:
 * - magic_open, magic_buffer (primary), magic_close, optionally magic_setflags
 * 
 * Strategy:
 * 1. Generate valid CDF (Composite Document File) structures from fuzzer input
 * 2. Create synthetic Microsoft Office document structures (OLE compound files)
 * 3. Ensure CDF header validation passes (magic number, sector allocation tables)
 * 4. Trigger deep parsing via magic_buffer to exercise blocked branches
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: General API testing with default magic database
 * - harness_001: Parameter configuration functions  
 * - harness_002: File/descriptor operations
 * - harness_003: Custom magic database compilation + float/double
 * - harness_004: Focused float comparison branch testing
 * - harness_005: Apprentice parsing + compress decompression
 * - harness_006: CDF/OLE compound file parsing (NEW)
 */


// CDF header structure based on cdf.h
struct CDFHeader {
    uint64_t h_magic;           // 0xE11AB1A1E011CFD0LL
    uint64_t h_uuid[2];         // 16-byte UUID
    uint16_t h_revision;
    uint16_t h_version;
    uint16_t h_byte_order;
    uint16_t h_sec_size_p2;     // Sector size as power of 2 (typically 9 for 512 bytes)
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
    int32_t  h_master_sat[109]; // 436 bytes / 4 bytes per entry
};

// CDF directory entry structure
struct CDFDirectory {
    uint16_t d_name[32];        // UTF-16 name
    uint16_t d_namelen;
    uint8_t  d_type;            // CDF_DIR_TYPE_*
    uint8_t  d_color;           // CDF_DIR_COLOR_*
    int32_t  d_left_child;
    int32_t  d_right_child;
    int32_t  d_storage;
    uint64_t d_storage_uuid[2];
    uint32_t d_flags;
    int64_t  d_created;
    int64_t  d_modified;
    int32_t  d_stream_first_sector;
    uint32_t d_size;
    uint32_t d_unused0;
};

// Generate synthetic CDF file data
static std::vector<uint8_t> generate_cdf_data(FuzzedDataProvider& fdp, size_t max_size = 65536) {
    std::vector<uint8_t> cdf_data;
    
    // Minimum size check
    if (fdp.remaining_bytes() < 512) {
        return cdf_data;
    }
    
    // Strategy selection: different CDF structure generation approaches
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    switch (strategy) {
        case 0: {
            // Strategy 0: Simple valid CDF header with minimal structure
            CDFHeader header;
            
            // Set magic number (0xE11AB1A1E011CFD0)
            header.h_magic = 0xE11AB1A1E011CFD0LL;
            
            // Generate random UUID
            header.h_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
            header.h_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
            
            // Set reasonable revision/version (typical values)
            header.h_revision = 0x003E; // 62 decimal
            header.h_version = 0x0003;  // 3 decimal
            
            // Byte order (0xFFFE for little-endian)
            header.h_byte_order = 0xFFFE;
            
            // Sector size: 512 bytes = 2^9
            header.h_sec_size_p2 = 9;
            header.h_short_sec_size_p2 = 6; // 64 bytes
            
            // Fill unused areas
            memset(header.h_unused0, 0, sizeof(header.h_unused0));
            memset(header.h_unused1, 0, sizeof(header.h_unused1));
            
            // Set sector allocation table info
            header.h_num_sectors_in_sat = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
            header.h_secid_first_directory = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            
            // Stream size threshold
            header.h_min_size_standard_stream = 4096;
            
            // Short SAT info
            header.h_secid_first_sector_in_short_sat = fdp.ConsumeIntegralInRange<int32_t>(-4, 100);
            header.h_num_sectors_in_short_sat = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            
            // Master SAT info
            header.h_secid_first_sector_in_master_sat = fdp.ConsumeIntegralInRange<int32_t>(-4, 100);
            header.h_num_sectors_in_master_sat = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
            
            // Fill master SAT with valid sector IDs
            for (int i = 0; i < 109; i++) {
                header.h_master_sat[i] = fdp.ConsumeIntegralInRange<int32_t>(-4, 100);
            }
            
            // Write header to buffer
            cdf_data.resize(sizeof(CDFHeader));
            memcpy(cdf_data.data(), &header, sizeof(CDFHeader));
            
            // Add some sector data
            size_t sector_size = 1 << header.h_sec_size_p2;
            size_t num_sectors = fdp.ConsumeIntegralInRange<size_t>(1, 10);
            
            for (size_t i = 0; i < num_sectors && fdp.remaining_bytes() >= sector_size; i++) {
                std::vector<uint8_t> sector = fdp.ConsumeBytes<uint8_t>(sector_size);
                cdf_data.insert(cdf_data.end(), sector.begin(), sector.end());
            }
            
            break;
        }
        
        case 1: {
            // Strategy 1: Office document-like structure with directory entries
            CDFHeader header;
            header.h_magic = 0xE11AB1A1E011CFD0LL;
            header.h_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
            header.h_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
            header.h_revision = 0x003E;
            header.h_version = 0x0003;
            header.h_byte_order = 0xFFFE;
            header.h_sec_size_p2 = 9;
            header.h_short_sec_size_p2 = 6;
            memset(header.h_unused0, 0, sizeof(header.h_unused0));
            memset(header.h_unused1, 0, sizeof(header.h_unused1));
            header.h_num_sectors_in_sat = 10;
            header.h_secid_first_directory = 1;
            header.h_min_size_standard_stream = 4096;
            header.h_secid_first_sector_in_short_sat = -2; // END_OF_CHAIN
            header.h_num_sectors_in_short_sat = 0;
            header.h_secid_first_sector_in_master_sat = -2; // END_OF_CHAIN
            header.h_num_sectors_in_master_sat = 0;
            
            // Initialize master SAT
            for (int i = 0; i < 109; i++) {
                header.h_master_sat[i] = (i < 10) ? -1 : -2; // First 10 free, rest end-of-chain
            }
            
            cdf_data.resize(sizeof(CDFHeader));
            memcpy(cdf_data.data(), &header, sizeof(CDFHeader));
            
            // Add directory sector
            size_t sector_size = 512;
            std::vector<uint8_t> sector(sector_size, 0);
            
            // Create a few directory entries (simplified)
            CDFDirectory dir;
            
            // Root storage entry
            memset(dir.d_name, 0, sizeof(dir.d_name));
            dir.d_name[0] = 'R';
            dir.d_name[1] = 'o';
            dir.d_name[2] = 'o';
            dir.d_name[3] = 't';
            dir.d_name[4] = ' ';
            dir.d_name[5] = 'E';
            dir.d_name[6] = 'n';
            dir.d_name[7] = 't';
            dir.d_name[8] = 'r';
            dir.d_name[9] = 'y';
            dir.d_namelen = 20; // 10 UTF-16 chars * 2
            
            dir.d_type = 5; // CDF_DIR_TYPE_ROOT_STORAGE
            dir.d_color = 1; // Black
            dir.d_left_child = -1;
            dir.d_right_child = -1;
            dir.d_storage = -1;
            dir.d_storage_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
            dir.d_storage_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
            dir.d_flags = 0;
            dir.d_created = 0;
            dir.d_modified = 0;
            dir.d_stream_first_sector = -2; // END_OF_CHAIN
            dir.d_size = 0;
            dir.d_unused0 = 0;
            
            // Write directory entry to sector
            memcpy(sector.data(), &dir, sizeof(CDFDirectory));
            
            cdf_data.insert(cdf_data.end(), sector.begin(), sector.end());
            
            break;
        }
        
        case 2: {
            // Strategy 2: Try to include "SummaryInformation" stream
            // Start with basic header
            CDFHeader header;
            header.h_magic = 0xE11AB1A1E011CFD0LL;
            header.h_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
            header.h_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
            header.h_revision = 0x003E;
            header.h_version = 0x0003;
            header.h_byte_order = 0xFFFE;
            header.h_sec_size_p2 = 9;
            header.h_short_sec_size_p2 = 6;
            memset(header.h_unused0, 0, sizeof(header.h_unused0));
            memset(header.h_unused1, 0, sizeof(header.h_unused1));
            header.h_num_sectors_in_sat = 5;
            header.h_secid_first_directory = 0;
            header.h_min_size_standard_stream = 4096;
            header.h_secid_first_sector_in_short_sat = -2;
            header.h_num_sectors_in_short_sat = 0;
            header.h_secid_first_sector_in_master_sat = -2;
            header.h_num_sectors_in_master_sat = 0;
            
            for (int i = 0; i < 109; i++) {
                header.h_master_sat[i] = (i < 5) ? i+1 : -2;
            }
            header.h_master_sat[4] = -2; // End chain after 5 sectors
            
            cdf_data.resize(sizeof(CDFHeader));
            memcpy(cdf_data.data(), &header, sizeof(CDFHeader));
            
            // Add some random data that might be interpreted as streams
            size_t remaining = std::min(fdp.remaining_bytes(), max_size - cdf_data.size());
            std::vector<uint8_t> random_data = fdp.ConsumeBytes<uint8_t>(remaining);
            cdf_data.insert(cdf_data.end(), random_data.begin(), random_data.end());
            
            break;
        }
        
        case 3: {
            // Strategy 3: Generate corrupted but interesting CDF structures
            // Start with valid magic
            CDFHeader header;
            header.h_magic = 0xE11AB1A1E011CFD0LL;
            
            // Randomize everything else to explore edge cases
            header.h_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
            header.h_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
            header.h_revision = fdp.ConsumeIntegral<uint16_t>();
            header.h_version = fdp.ConsumeIntegral<uint16_t>();
            header.h_byte_order = fdp.ConsumeIntegral<uint16_t>();
            header.h_sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(0, 16);
            header.h_short_sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(0, 16);
            
            for (int i = 0; i < 10; i++) {
                header.h_unused0[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            
            header.h_num_sectors_in_sat = fdp.ConsumeIntegral<uint32_t>();
            header.h_secid_first_directory = fdp.ConsumeIntegral<uint32_t>();
            
            for (int i = 0; i < 4; i++) {
                header.h_unused1[i] = fdp.ConsumeIntegral<uint8_t>();
            }
            
            header.h_min_size_standard_stream = fdp.ConsumeIntegral<uint32_t>();
            header.h_secid_first_sector_in_short_sat = fdp.ConsumeIntegral<int32_t>();
            header.h_num_sectors_in_short_sat = fdp.ConsumeIntegral<uint32_t>();
            header.h_secid_first_sector_in_master_sat = fdp.ConsumeIntegral<int32_t>();
            header.h_num_sectors_in_master_sat = fdp.ConsumeIntegral<uint32_t>();
            
            for (int i = 0; i < 109; i++) {
                header.h_master_sat[i] = fdp.ConsumeIntegral<int32_t>();
            }
            
            cdf_data.resize(sizeof(CDFHeader));
            memcpy(cdf_data.data(), &header, sizeof(CDFHeader));
            
            // Add variable amount of trailing data
            size_t extra_bytes = fdp.ConsumeIntegralInRange<size_t>(0, 8192);
            if (extra_bytes > 0 && fdp.remaining_bytes() >= extra_bytes) {
                std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(extra_bytes);
                cdf_data.insert(cdf_data.end(), extra.begin(), extra.end());
            }
            
            break;
        }
        
        case 4: {
            // Strategy 4: Direct byte manipulation - start with CDF magic and random data
            std::vector<uint8_t> random_data;
            
            // Ensure we have CDF magic at the beginning
            uint64_t cdf_magic = 0xE11AB1A1E011CFD0LL;
            random_data.resize(sizeof(cdf_magic));
            memcpy(random_data.data(), &cdf_magic, sizeof(cdf_magic));
            
            // Add random bytes for the rest
            size_t random_size = fdp.ConsumeIntegralInRange<size_t>(128, 16384);
            if (random_size > 0 && fdp.remaining_bytes() >= random_size) {
                std::vector<uint8_t> more_data = fdp.ConsumeBytes<uint8_t>(random_size);
                random_data.insert(random_data.end(), more_data.begin(), more_data.end());
            }
            
            cdf_data = std::move(random_data);
            break;
        }
    }
    
    return cdf_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for CDF testing
    // Need: strategy selection + CDF generation parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    bool use_mime = fdp.ConsumeBool();
    bool use_check = fdp.ConsumeBool();
    bool use_continue = fdp.ConsumeBool();
    
    // Build flags
    int actual_flags = flags & 0xFFFFF;
    if (use_mime) actual_flags |= MAGIC_MIME;
    if (use_check) actual_flags |= MAGIC_CHECK;
    if (use_continue) actual_flags |= MAGIC_CONTINUE;
    
    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }
    
    // Try to load magic database - use default location
    const char* magic_path = "/usr/share/misc/magic";
    if (access(magic_path, R_OK) != 0) {
        magic_path = "magic"; // Try local magic file
    }
    
    // Load magic database - ignore errors for fuzzing
    magic_load(magic, magic_path);
    
    // Generate CDF test data
    std::vector<uint8_t> cdf_buffer = generate_cdf_data(fdp);
    
    if (cdf_buffer.empty()) {
        magic_close(magic);
        return 0;
    }
    
    // Test with magic_buffer - this should trigger file_trycdf -> CDF parsing
    const char* result = magic_buffer(magic, cdf_buffer.data(), cdf_buffer.size());
    
    // Result can be null, that's OK for fuzzing
    (void)result;
    
    // Try a few variations with the same CDF data
    // 1. Test with different flags
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        int alt_flags = fdp.ConsumeIntegral<int>() & 0xFFFFF;
        magic_setflags(magic, alt_flags);
        
        const char* alt_result = magic_buffer(magic, cdf_buffer.data(), cdf_buffer.size());
        (void)alt_result;
    }
    
    // 2. Test with sliced buffer (partial CDF data)
    if (fdp.ConsumeBool() && cdf_buffer.size() > 64) {
        size_t slice_size = fdp.ConsumeIntegralInRange<size_t>(64, cdf_buffer.size());
        const char* slice_result = magic_buffer(magic, cdf_buffer.data(), slice_size);
        (void)slice_result;
    }
    
    // 3. Test with appended random data
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        size_t append_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        if (fdp.remaining_bytes() >= append_size) {
            std::vector<uint8_t> extended_buffer = cdf_buffer;
            std::vector<uint8_t> extra = fdp.ConsumeBytes<uint8_t>(append_size);
            extended_buffer.insert(extended_buffer.end(), extra.begin(), extra.end());
            
            const char* extended_result = magic_buffer(magic, extended_buffer.data(), extended_buffer.size());
            (void)extended_result;
        }
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_006
