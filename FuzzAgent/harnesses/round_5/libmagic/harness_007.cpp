#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include "magic.h"

namespace harness_007 {
/*
 * Fuzzing harness for libmagic targeting CDF (Compound Document Format) detection capabilities
 * Primary Target: CDF format parsing functions with 0-30% coverage in cdf.c, cdf_time.c, readcdf.c
 * Focus on generating CDF-formatted test data with various structures and edge cases to exercise
 * uncovered functions: cdf_tole4, cdf_tole8, cdf_timespec_to_timestamp, cdf_check_summary_info,
 * cdf_file_summary_info, cdf_file_property_info, cdf_file_catalog_info, cdf_unpack_summary_info,
 * cdf_read_property_info, cdf_read_user_stream, cdf_unpack_catalog
 * 
 * APIs: magic_open -> magic_load -> magic_buffer with CDF-formatted data -> magic_close
 * Invocation sequence: Initialize libmagic, generate CDF test data with diverse structures,
 * pass to magic_buffer for detection, cleanup
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000: Basic buffer detection with random operations
 * - harness_001: Configuration management and parameter tuning  
 * - harness_002: Database operations (compile, check, list) with simple magic files
 * - harness_003: Basic file operations and filesystem objects
 * - harness_004: Filesystem magic logic with specific file formats (ELF, CDF, compressed)
 * - harness_005: Deep ELF parsing with complex section structures
 * - harness_006: Magic database parsing internals with diverse magic type testing
 * - harness_007: CDF format detection with Microsoft Office document structures
 */



// CDF header structure based on cdf.h
typedef struct {
    uint64_t h_magic;
    uint64_t h_uuid[2];
    uint16_t h_revision;
    uint16_t h_version;
    uint16_t h_byte_order;
    uint16_t h_sec_size_p2;
    uint16_t h_short_sec_size_p2;
    uint8_t h_unused0[10];
    uint32_t h_num_sectors_in_sat;
    uint32_t h_secid_first_directory;
    uint8_t h_unused1[4];
    uint32_t h_min_size_standard_stream;
    int32_t h_secid_first_sector_in_short_sat;
    uint32_t h_num_sectors_in_short_sat;
    int32_t h_secid_first_sector_in_master_sat;
    uint32_t h_num_sectors_in_master_sat;
    int32_t h_master_sat[109]; // 436/4
} cdf_header_t;

// CDF directory entry structure
typedef struct {
    uint16_t d_name[32];
    uint16_t d_namelen;
    uint8_t d_type;
    uint8_t d_color;
    int32_t d_left_child;
    int32_t d_right_child;
    int32_t d_storage;
    uint64_t d_storage_uuid[2];
    uint32_t d_flags;
    uint64_t d_created;
    uint64_t d_modified;
    int32_t d_stream_first_sector;
    uint32_t d_size;
    uint32_t d_unused0;
} cdf_directory_t;

// CDF magic value
#define CDF_MAGIC 0xE11AB1A1E011CFD0LL

// Maximum size for CDF data generation
#define MAX_CDF_DATA_SIZE 1024 * 1024  // 1MB

// Generate a minimal valid CDF header
void generate_cdf_header(FuzzedDataProvider& fdp, cdf_header_t* header) {
    // Set magic value (required for CDF detection)
    header->h_magic = CDF_MAGIC;
    
    // Consume UUID from fuzzer input
    header->h_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
    header->h_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
    
    // Set reasonable version values
    header->h_revision = fdp.ConsumeIntegralInRange<uint16_t>(0, 0xFFFF);
    header->h_version = fdp.ConsumeIntegralInRange<uint16_t>(0, 0xFFFF);
    
    // Byte order: 0xFFFE for little-endian, 0xFEFF for big-endian
    header->h_byte_order = fdp.ConsumeBool() ? 0xFFFE : 0xFEFF;
    
    // Sector sizes (power of 2 values, typically 9 for 512 bytes)
    header->h_sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(6, 12);
    header->h_short_sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(6, 12);
    
    // Fill unused fields
    for (int i = 0; i < 10; i++) {
        header->h_unused0[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Sector allocation table info
    header->h_num_sectors_in_sat = fdp.ConsumeIntegral<uint32_t>();
    header->h_secid_first_directory = fdp.ConsumeIntegral<uint32_t>();
    
    // Fill unused1
    for (int i = 0; i < 4; i++) {
        header->h_unused1[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Stream size threshold
    header->h_min_size_standard_stream = fdp.ConsumeIntegral<uint32_t>();
    
    // Short sector and master sector info
    header->h_secid_first_sector_in_short_sat = fdp.ConsumeIntegral<int32_t>();
    header->h_num_sectors_in_short_sat = fdp.ConsumeIntegral<uint32_t>();
    header->h_secid_first_sector_in_master_sat = fdp.ConsumeIntegral<int32_t>();
    header->h_num_sectors_in_master_sat = fdp.ConsumeIntegral<uint32_t>();
    
    // Fill master SAT entries
    for (int i = 0; i < 109; i++) {
        header->h_master_sat[i] = fdp.ConsumeIntegral<int32_t>();
    }
}

// Generate a CDF directory entry
void generate_cdf_directory(FuzzedDataProvider& fdp, cdf_directory_t* dir, size_t index) {
    // Generate name (UTF-16)
    for (int i = 0; i < 32; i++) {
        dir->d_name[i] = fdp.ConsumeIntegral<uint16_t>();
    }
    
    // Name length
    dir->d_namelen = fdp.ConsumeIntegralInRange<uint16_t>(0, 64);
    
    // Directory type (0-5 based on cdf.h defines)
    dir->d_type = fdp.ConsumeIntegralInRange<uint8_t>(0, 5);
    
    // Color (0 or 1)
    dir->d_color = fdp.ConsumeIntegral<uint8_t>() & 1;
    
    // Child and storage IDs
    dir->d_left_child = fdp.ConsumeIntegral<int32_t>();
    dir->d_right_child = fdp.ConsumeIntegral<int32_t>();
    dir->d_storage = fdp.ConsumeIntegral<int32_t>();
    
    // Storage UUID
    dir->d_storage_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
    dir->d_storage_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
    
    // Flags
    dir->d_flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Timestamps (CDF timestamps are 64-bit)
    dir->d_created = fdp.ConsumeIntegral<uint64_t>();
    dir->d_modified = fdp.ConsumeIntegral<uint64_t>();
    
    // Stream sector and size
    dir->d_stream_first_sector = fdp.ConsumeIntegral<int32_t>();
    dir->d_size = fdp.ConsumeIntegral<uint32_t>();
    dir->d_unused0 = fdp.ConsumeIntegral<uint32_t>();
}

// Generate summary info stream data
std::vector<uint8_t> generate_summary_info(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> summary_info;
    
    // Summary info header
    uint16_t byte_order = fdp.ConsumeBool() ? 0xFFFE : 0xFEFF;
    uint16_t version = fdp.ConsumeIntegral<uint16_t>();
    uint32_t system_id = fdp.ConsumeIntegral<uint32_t>();
    uint64_t class_id[2] = {
        fdp.ConsumeIntegral<uint64_t>(),
        fdp.ConsumeIntegral<uint64_t>()
    };
    
    // Write header data
    uint8_t* p = reinterpret_cast<uint8_t*>(&byte_order);
    for (size_t i = 0; i < sizeof(byte_order); i++) summary_info.push_back(p[i]);
    
    p = reinterpret_cast<uint8_t*>(&version);
    for (size_t i = 0; i < sizeof(version); i++) summary_info.push_back(p[i]);
    
    p = reinterpret_cast<uint8_t*>(&system_id);
    for (size_t i = 0; i < sizeof(system_id); i++) summary_info.push_back(p[i]);
    
    for (int i = 0; i < 2; i++) {
        p = reinterpret_cast<uint8_t*>(&class_id[i]);
        for (size_t j = 0; j < sizeof(uint64_t); j++) summary_info.push_back(p[j]);
    }
    
    // Add some property data
    uint32_t num_properties = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
    p = reinterpret_cast<uint8_t*>(&num_properties);
    for (size_t i = 0; i < sizeof(num_properties); i++) summary_info.push_back(p[i]);
    
    // Add property entries
    for (uint32_t i = 0; i < num_properties && fdp.remaining_bytes() > 8; i++) {
        uint32_t prop_id = fdp.ConsumeIntegral<uint32_t>();
        uint32_t prop_type = fdp.ConsumeIntegral<uint32_t>();
        
        p = reinterpret_cast<uint8_t*>(&prop_id);
        for (size_t j = 0; j < sizeof(prop_id); j++) summary_info.push_back(p[j]);
        
        p = reinterpret_cast<uint8_t*>(&prop_type);
        for (size_t j = 0; j < sizeof(prop_type); j++) summary_info.push_back(p[j]);
        
        // Add some value data
        uint32_t value = fdp.ConsumeIntegral<uint32_t>();
        p = reinterpret_cast<uint8_t*>(&value);
        for (size_t j = 0; j < sizeof(value); j++) summary_info.push_back(p[j]);
    }
    
    return summary_info;
}

// Generate a complete CDF document
std::vector<uint8_t> generate_cdf_document(FuzzedDataProvider& fdp, uint8_t doc_type) {
    std::vector<uint8_t> cdf_data;
    
    // Generate header
    cdf_header_t header;
    generate_cdf_header(fdp, &header);
    
    // Write header to output
    uint8_t* p = reinterpret_cast<uint8_t*>(&header);
    for (size_t i = 0; i < sizeof(header); i++) {
        cdf_data.push_back(p[i]);
    }
    
    // Generate directory entries based on document type
    uint32_t num_dirs = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
    
    for (uint32_t i = 0; i < num_dirs && fdp.remaining_bytes() > sizeof(cdf_directory_t); i++) {
        cdf_directory_t dir;
        generate_cdf_directory(fdp, &dir, i);
        
        p = reinterpret_cast<uint8_t*>(&dir);
        for (size_t j = 0; j < sizeof(dir); j++) {
            cdf_data.push_back(p[j]);
        }
    }
    
    // Add some sector data
    size_t sector_size = 1 << header.h_sec_size_p2;
    uint32_t num_sectors = fdp.ConsumeIntegralInRange<uint32_t>(0, 50);
    
    for (uint32_t i = 0; i < num_sectors && fdp.remaining_bytes() > sector_size; i++) {
        std::vector<uint8_t> sector = fdp.ConsumeBytes<uint8_t>(sector_size);
        cdf_data.insert(cdf_data.end(), sector.begin(), sector.end());
    }
    
    // Add summary info for certain document types
    if (doc_type == 1 || doc_type == 3) {
        std::vector<uint8_t> summary_info = generate_summary_info(fdp);
        cdf_data.insert(cdf_data.end(), summary_info.begin(), summary_info.end());
    }
    
    return cdf_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags from input
    int flags = fdp.ConsumeIntegral<int>();
    
    // Limit flags to valid combinations
    flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | MAGIC_PRESERVE_ATIME |
              MAGIC_RAW | MAGIC_ERROR | MAGIC_MIME_ENCODING | MAGIC_APPLE |
              MAGIC_EXTENSION | MAGIC_COMPRESS_TRANSP | MAGIC_NO_COMPRESS_FORK);
    
    // Open magic handle
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        return 0;  // Failed to open magic handle
    }
    
    // Try to load default magic database
    int load_result = magic_load(magic, nullptr);
    if (load_result == -1) {
        // If loading default fails, try to load from common paths
        const char* magic_paths[] = {"/etc/magic", "/usr/share/misc/magic"};
        for (const char* path : magic_paths) {
            load_result = magic_load(magic, path);
            if (load_result != -1) {
                break;
            }
        }
    }
    
    // If we still couldn't load magic database, close and return
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Determine CDF document type to generate
    uint8_t doc_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Generate CDF test data
    std::vector<uint8_t> cdf_test_data = generate_cdf_document(fdp, doc_type);
    
    if (cdf_test_data.empty()) {
        magic_close(magic);
        return 0;
    }
    
    // Test magic_buffer with CDF data
    const char* result = magic_buffer(magic, cdf_test_data.data(), cdf_test_data.size());
    // Result can be nullptr on error, which is fine for fuzzing
    (void)result;
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_007
