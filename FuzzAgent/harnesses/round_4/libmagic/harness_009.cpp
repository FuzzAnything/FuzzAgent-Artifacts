#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "magic.h"

namespace harness_009 {
/*
 * Fuzzing harness for libmagic library - CDF (Compound Document Format) Office Document Parsing
 * Targets: CDF parsing functions in readcdf.c via magic_buffer -> file_trycdf
 * Goal: Target the critical coverage gap with 0% coverage across 8 core CDF functions with 562+ branches
 * Functions to exercise:
 * - cdf_check_summary_info (26 branches, 0% coverage)
 * - cdf_file_summary_info (30 branches, 0% coverage)
 * - cdf_file_property_info (96 branches, 0% coverage)
 * - cdf_file_catalog_info (4 branches, 0% coverage)
 * - cdf_unpack_summary_info (16 branches, 0% coverage)
 * - cdf_read_property_info (88 branches, 0% coverage)
 * - cdf_file_catalog (16 branches, 0% coverage)
 * - cdf_app_to_mime (12 branches, 0% coverage)
 * Strategy: Generate synthetic CDF files with proper headers, directory entries, property streams, 
 *           and summary information for Office documents (Word, Excel, PowerPoint)
 * Differentiated from previous harnesses: This is a new semantic domain (Office document parsing) not covered by previous harnesses
 * CDF file requirements:
 * - CDF magic header: D0 CF 11 E0 A1 B1 1A E1 (little-endian: 0xE11AB1A1E011CFD0LL)
 * - Structured directory entries with Office document types
 * - Property streams and summary information
 * - Both regular and encrypted CDF variants
 */


// CDF constants from cdf.h
#define CDF_MAGIC 0xE11AB1A1E011CFD0ULL
#define CDF_LOOP_LIMIT 10000
#define CDF_DIRECTORY_SIZE 128

// CDF directory types
#define CDF_DIR_TYPE_EMPTY 0
#define CDF_DIR_TYPE_USER_STORAGE 1
#define CDF_DIR_TYPE_USER_STREAM 2
#define CDF_DIR_TYPE_LOCKBYTES 3
#define CDF_DIR_TYPE_PROPERTY 4
#define CDF_DIR_TYPE_ROOT_STORAGE 5

// Office document stream names
const char* office_streams[] = {
    "WordDocument",
    "Book",
    "Workbook", 
    "PowerPoint",
    "EncryptedPackage",
    "EncryptedSummary",
    "SummaryInformation",
    "DocumentSummaryInformation",
    "Catalog",
    "FileHeader"
};

// Simple CDF header structure (from cdf.h)
struct SimpleCdfHeader {
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
};

// Simple CDF directory entry (from cdf.h)
struct SimpleCdfDirectory {
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
};

// Generate a CDF header with fuzzed parameters
void generate_cdf_header(FuzzedDataProvider& fdp, std::vector<uint8_t>& output) {
    // Check if we have enough data for all header fields
    // Minimum required: uuid[2] (16) + revision/version (4) + byte_order (2) + 
    // sec_size_p2/short_sec_size_p2 (4) + num_sectors_in_sat (4) + secid_first_directory (4) +
    // min_size_standard_stream (4) + secid_first_sector_in_short_sat (4) + 
    // num_sectors_in_short_sat (4) + secid_first_sector_in_master_sat (4) +
    // num_sectors_in_master_sat (4) + master_sat[109] (436) = 490 bytes
    const size_t MIN_HEADER_DATA = 490;
    
    if (fdp.remaining_bytes() < MIN_HEADER_DATA) {
        // Not enough data for proper header, generate minimal valid header
        SimpleCdfHeader header;
        memset(&header, 0, sizeof(header));
        header.h_magic = CDF_MAGIC;
        header.h_byte_order = 0xFFFE; // Little-endian
        header.h_sec_size_p2 = 9; // 512-byte sectors
        header.h_short_sec_size_p2 = 6; // 64-byte short sectors
        
        size_t offset = output.size();
        output.resize(offset + sizeof(SimpleCdfHeader));
        memcpy(output.data() + offset, &header, sizeof(SimpleCdfHeader));
        return;
    }
    
    SimpleCdfHeader header;
    
    // Always start with CDF magic
    header.h_magic = CDF_MAGIC;
    
    // UUID (can be fuzzed)
    header.h_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
    header.h_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
    
    // Revision and version
    header.h_revision = fdp.ConsumeIntegral<uint16_t>();
    header.h_version = fdp.ConsumeIntegral<uint16_t>();
    
    // Byte order (0xFFFE for little-endian, 0xFEFF for big-endian)
    uint8_t byte_order_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    if (byte_order_choice == 0) header.h_byte_order = 0xFFFE; // Little-endian
    else if (byte_order_choice == 1) header.h_byte_order = 0xFEFF; // Big-endian
    else header.h_byte_order = fdp.ConsumeIntegral<uint16_t>();
    
    // Sector sizes (typical: 9 for 512-byte sectors, 6 for 64-byte short sectors)
    header.h_sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(6, 12);
    header.h_short_sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(4, 8);
    
    // Unused fields
    memset(header.h_unused0, 0, sizeof(header.h_unused0));
    
    // Sector allocation table info
    header.h_num_sectors_in_sat = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
    header.h_secid_first_directory = fdp.ConsumeIntegral<uint32_t>();
    
    memset(header.h_unused1, 0, sizeof(header.h_unused1));
    
    // Stream size threshold
    header.h_min_size_standard_stream = fdp.ConsumeIntegralInRange<uint32_t>(4096, 65536);
    
    // Short SAT and master SAT
    header.h_secid_first_sector_in_short_sat = fdp.ConsumeIntegral<int32_t>();
    header.h_num_sectors_in_short_sat = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
    header.h_secid_first_sector_in_master_sat = fdp.ConsumeIntegral<int32_t>();
    header.h_num_sectors_in_master_sat = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
    
    // Master SAT entries
    for (int i = 0; i < 109; i++) {
        header.h_master_sat[i] = fdp.ConsumeIntegral<int32_t>();
    }
    
    // Write header to output
    size_t offset = output.size();
    output.resize(offset + sizeof(SimpleCdfHeader));
    memcpy(output.data() + offset, &header, sizeof(SimpleCdfHeader));
}

// Generate a CDF directory entry
void generate_cdf_directory(FuzzedDataProvider& fdp, std::vector<uint8_t>& output, 
                            const char* stream_name, uint8_t dir_type) {
    // Check if we have enough data for all directory fields
    // Minimum required: d_namelen (2) + dir_type (1) + d_color (1) + d_left_child (4) +
    // d_right_child (4) + d_storage (4) + d_storage_uuid[2] (16) + d_flags (4) +
    // d_created (8) + d_modified (8) + d_stream_first_sector (4) + d_size (4) + d_unused0 (4) = 60 bytes
    // Plus for name field: up to 32 * sizeof(uint16_t) = 64 bytes (if we need to generate random names)
    const size_t MIN_DIRECTORY_DATA = 60;
    
    if (fdp.remaining_bytes() < MIN_DIRECTORY_DATA) {
        // Not enough data, generate minimal valid directory
        SimpleCdfDirectory dir;
        memset(&dir, 0, sizeof(dir));
        if (stream_name) {
            size_t name_len = strlen(stream_name);
            dir.d_namelen = std::min<size_t>(name_len, 31) * 2;
            for (size_t i = 0; i < std::min<size_t>(name_len, 31); i++) {
                dir.d_name[i] = stream_name[i];
            }
        }
        dir.d_type = dir_type;
        dir.d_color = 0;
        
        size_t offset = output.size();
        output.resize(offset + CDF_DIRECTORY_SIZE);
        memcpy(output.data() + offset, &dir, CDF_DIRECTORY_SIZE);
        return;
    }
    
    SimpleCdfDirectory dir;
    
    // Clear the name field
    memset(dir.d_name, 0, sizeof(dir.d_name));
    
    // Set name from stream_name if provided
    if (stream_name) {
        size_t name_len = strlen(stream_name);
        dir.d_namelen = std::min<size_t>(name_len, 31) * 2; // UTF-16 characters
        for (size_t i = 0; i < std::min<size_t>(name_len, 31); i++) {
            dir.d_name[i] = stream_name[i]; // Simple ASCII to UTF-16
        }
    } else {
        // Check if we have enough for name generation (32 * sizeof(uint16_t) = 64 bytes)
        if (fdp.remaining_bytes() < 64) {
            dir.d_namelen = 0;
        } else {
            dir.d_namelen = fdp.ConsumeIntegralInRange<uint16_t>(0, 62);
            for (int i = 0; i < 32; i++) {
                dir.d_name[i] = fdp.ConsumeIntegral<uint16_t>();
            }
        }
    }
    
    // Directory type
    dir.d_type = dir_type;
    
    // Color (red-black tree)
    dir.d_color = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Tree pointers
    dir.d_left_child = fdp.ConsumeIntegral<int32_t>();
    dir.d_right_child = fdp.ConsumeIntegral<int32_t>();
    dir.d_storage = fdp.ConsumeIntegral<int32_t>();
    
    // Storage UUID
    dir.d_storage_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
    dir.d_storage_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
    
    // Flags
    dir.d_flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Timestamps (Windows FILETIME format)
    dir.d_created = fdp.ConsumeIntegral<uint64_t>();
    dir.d_modified = fdp.ConsumeIntegral<uint64_t>();
    
    // Stream info
    dir.d_stream_first_sector = fdp.ConsumeIntegral<int32_t>();
    dir.d_size = fdp.ConsumeIntegral<uint32_t>();
    dir.d_unused0 = fdp.ConsumeIntegral<uint32_t>();
    
    // Write directory to output
    size_t offset = output.size();
    output.resize(offset + CDF_DIRECTORY_SIZE);
    memcpy(output.data() + offset, &dir, CDF_DIRECTORY_SIZE);
}

// Generate synthetic property stream data (simplified)
void generate_property_stream(FuzzedDataProvider& fdp, std::vector<uint8_t>& output) {
    // Check if we have enough data for property count
    if (fdp.remaining_bytes() < sizeof(uint32_t)) {
        return; // Not enough data for property count
    }
    
    // Property stream header (simplified)
    uint32_t num_properties = fdp.ConsumeIntegralInRange<uint32_t>(1, 20);
    
    size_t offset = output.size();
    output.resize(offset + 4);
    memcpy(output.data() + offset, &num_properties, 4);
    
    // Generate some property data
    for (uint32_t i = 0; i < num_properties; i++) {
        // Check if we have enough data for property fields (3 * sizeof(uint32_t))
        if (fdp.remaining_bytes() < 3 * sizeof(uint32_t)) {
            break; // Not enough data for property fields
        }
        
        uint32_t prop_id = fdp.ConsumeIntegral<uint32_t>();
        uint32_t prop_type = fdp.ConsumeIntegral<uint32_t>() % 10;
        uint32_t prop_value = fdp.ConsumeIntegral<uint32_t>();
        
        offset = output.size();
        output.resize(offset + 12);
        memcpy(output.data() + offset, &prop_id, 4);
        memcpy(output.data() + offset + 4, &prop_type, 4);
        memcpy(output.data() + offset + 8, &prop_value, 4);
    }
}

// Generate summary information stream
void generate_summary_info(FuzzedDataProvider& fdp, std::vector<uint8_t>& output) {
    // Summary info header
    uint32_t version = 0xE11AB1A1; // Common version marker
    uint32_t system_id = 3; // Windows
    
    size_t offset = output.size();
    output.resize(offset + 8);
    memcpy(output.data() + offset, &version, 4);
    memcpy(output.data() + offset + 4, &system_id, 4);
    
    // Some summary data
    uint32_t num_sections = fdp.ConsumeIntegralInRange<uint32_t>(1, 5);
    
    offset = output.size();
    output.resize(offset + 4);
    memcpy(output.data() + offset, &num_sections, 4);
    
    for (uint32_t i = 0; i < num_sections; i++) {
        // Check if we have enough data for section size and section data
        if (fdp.remaining_bytes() < sizeof(uint32_t)) {
            break; // Not enough data for section size
        }
        
        uint32_t section_size = fdp.ConsumeIntegralInRange<uint32_t>(16, 256);
        
        // Check if we have enough data for the requested section size
        if (fdp.remaining_bytes() < section_size) {
            break; // Not enough data for section content
        }
        
        std::vector<uint8_t> section_data = fdp.ConsumeBytes<uint8_t>(section_size);
        
        // Check if section_data is not empty before using it
        if (section_data.empty()) {
            break; // No data available
        }
        
        offset = output.size();
        output.resize(offset + 4 + section_data.size());
        memcpy(output.data() + offset, &section_size, 4);
        memcpy(output.data() + offset + 4, section_data.data(), section_data.size());
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic CDF header
    if (size < 512) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Determine what type of CDF file to generate
    uint8_t cdf_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool generate_valid_header = (cdf_type == 0 || cdf_type == 1);
    bool include_office_streams = (cdf_type == 0 || cdf_type == 2);
    bool include_encrypted = (cdf_type == 1 || cdf_type == 3);
    
    std::vector<uint8_t> cdf_data;
    
    // Generate CDF header
    generate_cdf_header(fdp, cdf_data);
    
    // Pad to sector boundary (typically 512 bytes)
    size_t sector_size = 1 << 9; // Assume 512-byte sectors
    if (cdf_data.size() < sector_size) {
        cdf_data.resize(sector_size, 0);
    }
    
    // Generate directory sector
    size_t dir_sector_start = cdf_data.size();
    
    // Always include a root storage directory
    generate_cdf_directory(fdp, cdf_data, "Root Entry", CDF_DIR_TYPE_ROOT_STORAGE);
    
    // Include office document streams based on type
    if (include_office_streams) {
        // Choose random office document type
        uint8_t doc_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        const char* main_stream = nullptr;
        
        if (doc_type == 0) {
            // Word document
            main_stream = "WordDocument";
            generate_cdf_directory(fdp, cdf_data, "WordDocument", CDF_DIR_TYPE_USER_STREAM);
            generate_cdf_directory(fdp, cdf_data, "SummaryInformation", CDF_DIR_TYPE_USER_STREAM);
            generate_cdf_directory(fdp, cdf_data, "DocumentSummaryInformation", CDF_DIR_TYPE_USER_STREAM);
        } else if (doc_type == 1) {
            // Excel document
            main_stream = "Book";
            generate_cdf_directory(fdp, cdf_data, "Book", CDF_DIR_TYPE_USER_STREAM);
            generate_cdf_directory(fdp, cdf_data, "Workbook", CDF_DIR_TYPE_USER_STREAM);
            generate_cdf_directory(fdp, cdf_data, "SummaryInformation", CDF_DIR_TYPE_USER_STREAM);
        } else {
            // PowerPoint document
            main_stream = "PowerPoint Document";
            generate_cdf_directory(fdp, cdf_data, "PowerPoint Document", CDF_DIR_TYPE_USER_STREAM);
            generate_cdf_directory(fdp, cdf_data, "SummaryInformation", CDF_DIR_TYPE_USER_STREAM);
        }
        
        // Generate catalog stream for Office documents
        generate_cdf_directory(fdp, cdf_data, "Catalog", CDF_DIR_TYPE_USER_STREAM);
    }
    
    // Include encrypted streams if requested
    if (include_encrypted) {
        generate_cdf_directory(fdp, cdf_data, "EncryptedPackage", CDF_DIR_TYPE_USER_STREAM);
        generate_cdf_directory(fdp, cdf_data, "EncryptedSummary", CDF_DIR_TYPE_USER_STREAM);
    }
    
    // Fill remaining directory entries with random data
    size_t dir_entries_so_far = (cdf_data.size() - dir_sector_start) / CDF_DIRECTORY_SIZE;
    size_t dir_entries_per_sector = sector_size / CDF_DIRECTORY_SIZE;
    
    while (dir_entries_so_far < dir_entries_per_sector) {
        generate_cdf_directory(fdp, cdf_data, nullptr, fdp.ConsumeIntegral<uint8_t>() % 6);
        dir_entries_so_far++;
    }
    
    // Pad to next sector boundary
    if (cdf_data.size() % sector_size != 0) {
        size_t padding = sector_size - (cdf_data.size() % sector_size);
        cdf_data.resize(cdf_data.size() + padding, 0);
    }
    
    // Generate some property stream data
    size_t property_sector_start = cdf_data.size();
    generate_property_stream(fdp, cdf_data);
    
    // Pad property stream
    if (cdf_data.size() % sector_size != 0) {
        size_t padding = sector_size - (cdf_data.size() % sector_size);
        cdf_data.resize(cdf_data.size() + padding, 0);
    }
    
    // Generate summary information
    size_t summary_sector_start = cdf_data.size();
    generate_summary_info(fdp, cdf_data);
    
    // Pad summary stream
    if (cdf_data.size() % sector_size != 0) {
        size_t padding = sector_size - (cdf_data.size() % sector_size);
        cdf_data.resize(cdf_data.size() + padding, 0);
    }
    
    // Add some random sector data to exercise SAT parsing
    size_t extra_sectors = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < extra_sectors && fdp.remaining_bytes() > sector_size; i++) {
        std::vector<uint8_t> sector_data = fdp.ConsumeBytes<uint8_t>(sector_size);
        cdf_data.insert(cdf_data.end(), sector_data.begin(), sector_data.end());
    }
    
    // Now test with libmagic
    magic_t magic = magic_open(MAGIC_NONE);
    if (magic == NULL) {
        return 0;
    }
    
    // Try to load magic database
    if (magic_load(magic, NULL) == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Test the generated CDF data
    if (!cdf_data.empty()) {
        const char* result = magic_buffer(magic, cdf_data.data(), cdf_data.size());
        (void)result; // Result can be NULL, that's OK
    }
    
    magic_close(magic);
    return 0;
}

} // namespace harness_009
