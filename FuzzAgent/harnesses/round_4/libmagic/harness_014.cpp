#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include "magic.h"

namespace harness_014 {
/*
 * Fuzzing harness for libmagic library - CDF (Composite Document File) Format Parsing
 * Targets: CDF parser functions with extremely low coverage (19.54% branch coverage vs 46.32% overall)
 * Specific functions: 
 *   - cdf_check_summary_info (641 undiscovered branches)
 *   - cdf_file_summary_info (377 undiscovered branches)
 *   - cdf_file_property_info (195 undiscovered branches)
 *   - cdf_unpack_summary_info (152 undiscovered branches)
 *   - cdf_read_dir (42 undiscovered branches)
 * Focus: Microsoft Office Composite Document Files (pre-2007 formats: .doc, .xls, .ppt)
 * Goal: Generate CDF-like binary structures with summary info streams, property streams, directory structures
 * Edge cases: corrupted headers, malformed sector chains, invalid timestamps, both big-endian and little-endian variants
 * Semantic differentiation: Existing harnesses test general file detection; this harness exclusively targets
 *        CDF binary format parsing with synthetic CDF structure generation
 * Strategy: Use FuzzedDataProvider to generate valid and malformed CDF structures, test through magic_buffer API
 * Expected coverage gain: Target the 1400+ blocked branches in core CDF functions
 */


// Include libmagic header

// CDF structure definitions (from cdf.h)
#define CDF_MAGIC 0xE11AB1A1E011CFD0LL
#define CDF_SEC_SIZE_P2_DEFAULT 9  // 512 bytes
#define CDF_SHORT_SEC_SIZE_P2_DEFAULT 6  // 64 bytes
#define CDF_DIRECTORY_SIZE 128
#define CDF_SECID_NULL 0
#define CDF_SECID_FREE -1
#define CDF_SECID_END_OF_CHAIN -2
#define CDF_SECID_SAT -3
#define CDF_SECID_MSAT -4

// CDF header structure (simplified for generation)
struct CdfHeader {
    uint64_t magic;
    uint64_t uuid[2];
    uint16_t revision;
    uint16_t version;
    uint16_t byte_order;
    uint16_t sec_size_p2;
    uint16_t short_sec_size_p2;
    uint8_t unused0[10];
    uint32_t num_sectors_in_sat;
    uint32_t secid_first_directory;
    uint8_t unused1[4];
    uint32_t min_size_standard_stream;
    int32_t secid_first_sector_in_short_sat;
    uint32_t num_sectors_in_short_sat;
    int32_t secid_first_sector_in_master_sat;
    uint32_t num_sectors_in_master_sat;
    int32_t master_sat[109];  // 436 bytes / 4
};

// CDF directory entry structure (simplified)
struct CdfDirectory {
    uint16_t name[32];
    uint16_t namelen;
    uint8_t type;
    uint8_t color;
    int32_t left_child;
    int32_t right_child;
    int32_t storage;
    uint64_t storage_uuid[2];
    uint32_t flags;
    uint64_t created;
    uint64_t modified;
    int32_t stream_first_sector;
    uint32_t size;
    uint32_t unused0;
};

// Generate a CDF header with valid or corrupted structure
void generate_cdf_header(FuzzedDataProvider& fdp, CdfHeader& header, bool generate_valid) {
    // Always start with magic - sometimes corrupted
    if (generate_valid && fdp.ConsumeBool()) {
        header.magic = CDF_MAGIC;
    } else {
        header.magic = fdp.ConsumeIntegral<uint64_t>();
    }
    
    // UUID - can be random or specific
    header.uuid[0] = fdp.ConsumeIntegral<uint64_t>();
    header.uuid[1] = fdp.ConsumeIntegral<uint64_t>();
    
    // Revision and version - Office documents have specific values
    if (generate_valid && fdp.ConsumeBool()) {
        header.revision = 0x003E;  // Typical Office revision
        header.version = 0x0003;   // Version 3
    } else {
        header.revision = fdp.ConsumeIntegral<uint16_t>();
        header.version = fdp.ConsumeIntegral<uint16_t>();
    }
    
    // Byte order: 0xFEFF = little-endian, 0xFFFE = big-endian
    uint8_t endian_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    switch (endian_choice) {
        case 0: header.byte_order = 0xFEFF; break;  // Little-endian
        case 1: header.byte_order = 0xFFFE; break;  // Big-endian
        default: header.byte_order = fdp.ConsumeIntegral<uint16_t>(); break;
    }
    
    // Sector sizes - usually 512 bytes (2^9) and 64 bytes (2^6)
    if (generate_valid && fdp.ConsumeBool()) {
        header.sec_size_p2 = CDF_SEC_SIZE_P2_DEFAULT;
        header.short_sec_size_p2 = CDF_SHORT_SEC_SIZE_P2_DEFAULT;
    } else {
        header.sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(6, 12);
        header.short_sec_size_p2 = fdp.ConsumeIntegralInRange<uint16_t>(4, 8);
    }
    
    // Unused fields
    for (int i = 0; i < 10; i++) header.unused0[i] = fdp.ConsumeIntegral<uint8_t>();
    
    // Sector allocation table counts
    header.num_sectors_in_sat = fdp.ConsumeIntegral<uint32_t>();
    header.secid_first_directory = fdp.ConsumeIntegral<uint32_t>();
    
    // More unused fields
    for (int i = 0; i < 4; i++) header.unused1[i] = fdp.ConsumeIntegral<uint8_t>();
    
    // Minimum size for standard stream
    header.min_size_standard_stream = fdp.ConsumeIntegral<uint32_t>();
    
    // Short SAT and Master SAT pointers
    header.secid_first_sector_in_short_sat = fdp.ConsumeIntegral<int32_t>();
    header.num_sectors_in_short_sat = fdp.ConsumeIntegral<uint32_t>();
    header.secid_first_sector_in_master_sat = fdp.ConsumeIntegral<int32_t>();
    header.num_sectors_in_master_sat = fdp.ConsumeIntegral<uint32_t>();
    
    // Master SAT entries
    for (int i = 0; i < 109; i++) {
        header.master_sat[i] = fdp.ConsumeIntegral<int32_t>();
    }
}

// Generate a CDF directory entry
void generate_cdf_directory(FuzzedDataProvider& fdp, CdfDirectory& dir, bool is_root_storage) {
    // Directory name (UTF-16)
    for (int i = 0; i < 32; i++) {
        dir.name[i] = fdp.ConsumeIntegral<uint16_t>();
    }
    
    // Name length
    dir.namelen = fdp.ConsumeIntegralInRange<uint16_t>(0, 64);
    
    // Directory type
    uint8_t type_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    if (is_root_storage) {
        dir.type = 5;  // CDF_DIR_TYPE_ROOT_STORAGE
    } else {
        switch (type_choice) {
            case 0: dir.type = 0; break;  // EMPTY
            case 1: dir.type = 1; break;  // USER_STORAGE
            case 2: dir.type = 2; break;  // USER_STREAM
            case 3: dir.type = 3; break;  // LOCKBYTES
            case 4: dir.type = 4; break;  // PROPERTY
            default: dir.type = fdp.ConsumeIntegral<uint8_t>(); break;
        }
    }
    
    // Color (red-black tree)
    dir.color = fdp.ConsumeBool() ? 0 : 1;
    
    // Tree pointers
    dir.left_child = fdp.ConsumeIntegral<int32_t>();
    dir.right_child = fdp.ConsumeIntegral<int32_t>();
    dir.storage = fdp.ConsumeIntegral<int32_t>();
    
    // Storage UUID (CLSID)
    dir.storage_uuid[0] = fdp.ConsumeIntegral<uint64_t>();
    dir.storage_uuid[1] = fdp.ConsumeIntegral<uint64_t>();
    
    // Flags
    dir.flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Timestamps (can be valid or invalid)
    if (fdp.ConsumeBool()) {
        // Generate plausible timestamps (100-nanosecond intervals since 1601)
        uint64_t base_time = 0x01D2F2D8B2C0000;  // Approx year 2000
        int64_t time_offset = fdp.ConsumeIntegral<int64_t>();
        dir.created = base_time + time_offset;
        dir.modified = base_time + fdp.ConsumeIntegral<int64_t>();
    } else {
        // Random timestamps (may be invalid)
        dir.created = fdp.ConsumeIntegral<uint64_t>();
        dir.modified = fdp.ConsumeIntegral<uint64_t>();
    }
    
    // Stream sector pointer and size
    dir.stream_first_sector = fdp.ConsumeIntegral<int32_t>();
    dir.size = fdp.ConsumeIntegral<uint32_t>();
    dir.unused0 = fdp.ConsumeIntegral<uint32_t>();
}

// Generate summary info stream (property set stream)
std::vector<uint8_t> generate_summary_info_stream(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> stream;
    
    // Summary info header
    // Byte order mark
    uint16_t byte_order = fdp.ConsumeBool() ? 0xFEFF : 0xFFFE;
    stream.push_back(byte_order & 0xFF);
    stream.push_back((byte_order >> 8) & 0xFF);
    
    // Zero field
    stream.push_back(0);
    stream.push_back(0);
    
    // OS version
    uint16_t os_version = fdp.ConsumeIntegral<uint16_t>();
    stream.push_back(os_version & 0xFF);
    stream.push_back((os_version >> 8) & 0xFF);
    
    // OS identifier
    uint16_t os_id = fdp.ConsumeIntegral<uint16_t>();
    stream.push_back(os_id & 0xFF);
    stream.push_back((os_id >> 8) & 0xFF);
    
    // Class ID (CLSID) - 16 bytes
    for (int i = 0; i < 16; i++) {
        stream.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // Property count
    uint32_t prop_count = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
    stream.push_back(prop_count & 0xFF);
    stream.push_back((prop_count >> 8) & 0xFF);
    stream.push_back((prop_count >> 16) & 0xFF);
    stream.push_back((prop_count >> 24) & 0xFF);
    
    // Section declaration
    // Another CLSID
    for (int i = 0; i < 16; i++) {
        stream.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // Offset to property set
    uint32_t offset = 0x30 + 0x1C;  // Header + section declaration
    stream.push_back(offset & 0xFF);
    stream.push_back((offset >> 8) & 0xFF);
    stream.push_back((offset >> 16) & 0xFF);
    stream.push_back((offset >> 24) & 0xFF);
    
    // Property set header
    // Length
    uint32_t prop_set_len = fdp.ConsumeIntegralInRange<uint32_t>(8, 4096);
    stream.push_back(prop_set_len & 0xFF);
    stream.push_back((prop_set_len >> 8) & 0xFF);
    stream.push_back((prop_set_len >> 16) & 0xFF);
    stream.push_back((prop_set_len >> 24) & 0xFF);
    
    // Number of properties
    uint32_t num_props = std::min(prop_count, fdp.ConsumeIntegralInRange<uint32_t>(0, 50));
    stream.push_back(num_props & 0xFF);
    stream.push_back((num_props >> 8) & 0xFF);
    stream.push_back((num_props >> 16) & 0xFF);
    stream.push_back((num_props >> 24) & 0xFF);
    
    // Generate some properties
    for (uint32_t i = 0; i < num_props && stream.size() < 8192; i++) {
        // Property ID
        uint32_t prop_id = fdp.ConsumeIntegral<uint32_t>();
        for (int j = 0; j < 4; j++) {
            stream.push_back((prop_id >> (8 * j)) & 0xFF);
        }
        
        // Property offset
        uint32_t prop_offset = stream.size() + 8;  // After ID and offset fields
        for (int j = 0; j < 4; j++) {
            stream.push_back((prop_offset >> (8 * j)) & 0xFF);
        }
        
        // Property value (simple types for now)
        uint8_t prop_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        switch (prop_type) {
            case 0: // VT_I2 (signed 16-bit)
                stream.push_back(0x02); // VT_I2
                stream.push_back(0x00);
                stream.push_back(fdp.ConsumeIntegral<uint8_t>());
                stream.push_back(fdp.ConsumeIntegral<uint8_t>());
                break;
            case 1: // VT_I4 (signed 32-bit)
                stream.push_back(0x03); // VT_I4
                stream.push_back(0x00);
                for (int j = 0; j < 4; j++) {
                    stream.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
                break;
            case 2: // VT_LPSTR (string)
                stream.push_back(0x1E); // VT_LPSTR
                stream.push_back(0x00);
                {
                    uint32_t str_len = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
                    for (int j = 0; j < 4; j++) {
                        stream.push_back((str_len >> (8 * j)) & 0xFF);
                    }
                    for (uint32_t j = 0; j < str_len && j < 100; j++) {
                        stream.push_back(fdp.ConsumeIntegralInRange<uint8_t>(32, 126));
                    }
                    // Null terminator
                    if (str_len > 0) stream.push_back(0);
                }
                break;
            default: // Just some bytes
                for (int j = 0; j < 8; j++) {
                    stream.push_back(fdp.ConsumeIntegral<uint8_t>());
                }
                break;
        }
    }
    
    return stream;
}

// Serialize CDF header to byte vector
std::vector<uint8_t> serialize_cdf_header(const CdfHeader& header) {
    std::vector<uint8_t> data;
    
    // Helper lambda to push little-endian values
    auto push_le = [&](auto value) {
        for (size_t i = 0; i < sizeof(value); i++) {
            data.push_back((value >> (8 * i)) & 0xFF);
        }
    };
    
    // Push header fields
    push_le(header.magic);
    push_le(header.uuid[0]);
    push_le(header.uuid[1]);
    push_le(header.revision);
    push_le(header.version);
    push_le(header.byte_order);
    push_le(header.sec_size_p2);
    push_le(header.short_sec_size_p2);
    
    // Unused0
    for (int i = 0; i < 10; i++) {
        data.push_back(header.unused0[i]);
    }
    
    push_le(header.num_sectors_in_sat);
    push_le(header.secid_first_directory);
    
    // Unused1
    for (int i = 0; i < 4; i++) {
        data.push_back(header.unused1[i]);
    }
    
    push_le(header.min_size_standard_stream);
    push_le(header.secid_first_sector_in_short_sat);
    push_le(header.num_sectors_in_short_sat);
    push_le(header.secid_first_sector_in_master_sat);
    push_le(header.num_sectors_in_master_sat);
    
    // Master SAT
    for (int i = 0; i < 109; i++) {
        push_le(header.master_sat[i]);
    }
    
    return data;
}

// Serialize CDF directory to byte vector
std::vector<uint8_t> serialize_cdf_directory(const CdfDirectory& dir) {
    std::vector<uint8_t> data;
    
    // Helper lambda to push little-endian values
    auto push_le = [&](auto value) {
        for (size_t i = 0; i < sizeof(value); i++) {
            data.push_back((value >> (8 * i)) & 0xFF);
        }
    };
    
    // Name (UTF-16)
    for (int i = 0; i < 32; i++) {
        push_le(dir.name[i]);
    }
    
    push_le(dir.namelen);
    data.push_back(dir.type);
    data.push_back(dir.color);
    push_le(dir.left_child);
    push_le(dir.right_child);
    push_le(dir.storage);
    push_le(dir.storage_uuid[0]);
    push_le(dir.storage_uuid[1]);
    push_le(dir.flags);
    push_le(dir.created);
    push_le(dir.modified);
    push_le(dir.stream_first_sector);
    push_le(dir.size);
    push_le(dir.unused0);
    
    return data;
}

// Generate a synthetic CDF file
std::vector<uint8_t> generate_cdf_file(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> cdf_data;
    
    // Determine if we should generate a mostly valid CDF structure
    bool generate_valid = fdp.ConsumeBool();
    
    // Generate header
    CdfHeader header;
    generate_cdf_header(fdp, header, generate_valid);
    std::vector<uint8_t> header_data = serialize_cdf_header(header);
    cdf_data.insert(cdf_data.end(), header_data.begin(), header_data.end());
    
    // Calculate sector size
    size_t sector_size = 1 << header.sec_size_p2;
    if (sector_size < 64) sector_size = 512;  // Sanity check
    if (sector_size > 8192) sector_size = 4096;  // Limit
    
    // Pad to sector boundary if needed
    size_t padding_needed = sector_size - cdf_data.size();
    for (size_t i = 0; i < padding_needed; i++) {
        cdf_data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // Generate some directory sectors
    size_t num_dir_sectors = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    size_t dirs_per_sector = sector_size / CDF_DIRECTORY_SIZE;
    
    for (size_t sector = 0; sector < num_dir_sectors; sector++) {
        std::vector<uint8_t> sector_data;
        
        for (size_t dir_idx = 0; dir_idx < dirs_per_sector; dir_idx++) {
            bool is_root = (sector == 0 && dir_idx == 0 && generate_valid && fdp.ConsumeBool());
            CdfDirectory dir;
            generate_cdf_directory(fdp, dir, is_root);
            std::vector<uint8_t> dir_data = serialize_cdf_directory(dir);
            sector_data.insert(sector_data.end(), dir_data.begin(), dir_data.end());
        }
        
        // Pad sector if needed
        if (sector_data.size() < sector_size) {
            size_t pad = sector_size - sector_data.size();
            for (size_t i = 0; i < pad; i++) {
                sector_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
        }
        
        cdf_data.insert(cdf_data.end(), sector_data.begin(), sector_data.end());
    }
    
    // Generate some data sectors with summary info stream
    if (generate_valid && fdp.ConsumeBool()) {
        // Create a summary info stream
        std::vector<uint8_t> summary_stream = generate_summary_info_stream(fdp);
        
        // Pad to sector boundary
        size_t stream_sectors = (summary_stream.size() + sector_size - 1) / sector_size;
        for (size_t i = 0; i < stream_sectors * sector_size; i++) {
            if (i < summary_stream.size()) {
                cdf_data.push_back(summary_stream[i]);
            } else {
                cdf_data.push_back(fdp.ConsumeIntegral<uint8_t>());
            }
        }
    }
    
    // Add some random sectors to fill out the file
    size_t total_sectors = fdp.ConsumeIntegralInRange<size_t>(1, 50);
    while (cdf_data.size() < total_sectors * sector_size && fdp.remaining_bytes() > sector_size) {
        std::vector<uint8_t> random_sector = fdp.ConsumeBytes<uint8_t>(sector_size);
        cdf_data.insert(cdf_data.end(), random_sector.begin(), random_sector.end());
    }
    
    return cdf_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for meaningful CDF generation
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open
    int flags = 0;
    
    // Consume a byte to determine which flags to set
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;
    if (flag_byte & 0x20) flags |= MAGIC_CONTINUE;
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
    
    // Generate CDF test data
    std::vector<uint8_t> cdf_test_data = generate_cdf_file(fdp);
    
    if (!cdf_test_data.empty()) {
        // Test with magic_buffer - this will exercise CDF parser
        const char* result = magic_buffer(magic, cdf_test_data.data(), cdf_test_data.size());
        
        // Also test with MIME flag to exercise different code paths
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            magic_setflags(magic, flags | MAGIC_MIME_TYPE);
            const char* mime_result = magic_buffer(magic, cdf_test_data.data(), cdf_test_data.size());
            (void)mime_result;
        }
        
        // Also test with raw flag
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
            magic_setflags(magic, flags | MAGIC_RAW);
            const char* raw_result = magic_buffer(magic, cdf_test_data.data(), cdf_test_data.size());
            (void)raw_result;
        }
        
        (void)result; // Result can be NULL for unrecognized data
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_014
