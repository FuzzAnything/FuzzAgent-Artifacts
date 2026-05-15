#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include "magic.h"

namespace harness_023 {
/*
 * Fuzzing harness for libmagic library - Combined CDF Office Parsing with mget Indirect Offset Handling
 * Targets: CDF parsing functions AND mget indirect offset handling in softmagic.c
 * Based on coverage analysis: UNCOVERED FUNCTIONS: CDF parsing functions and mget indirect offset handling
 * MODULE: CDF module (cdf.c, readcdf.c) and softmagic.c
 * STRATEGY: Target Microsoft Office (CDF) file formats with embedded indirect offset patterns
 *           to trigger both CDF parsing and mget's complex indirect offset logic simultaneously
 * DIFFERENTIATION: Combines Office document parsing with indirect offset testing, 
 *           creating unique test cases that exercise both critical uncovered code paths
 * API SEQUENCE: magic_open() -> magic_load_buffers() with combined CDF/indirect patterns -> 
 *               magic_buffer() with Office document data containing indirect markers -> magic_close()
 * Goal: Break plateau state by targeting two critical coverage gaps simultaneously:
 *       1. CDF/Office file parsing functions (cdf.c, readcdf.c)
 *       2. mget indirect offset handling in softmagic.c
 */


// Include libmagic header

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

// Indirect operation patterns for mget testing
#define INDIRECT_PATTERN_BYTE 0x80
#define INDIRECT_PATTERN_SHORT 0x8000
#define INDIRECT_PATTERN_LONG 0x80000000
#define INDIRECT_PATTERN_QUAD 0x8000000000000000ULL

// Office document types
enum OfficeType {
    OFFICE_DOC = 0,
    OFFICE_XLS = 1,
    OFFICE_PPT = 2,
    OFFICE_MSI = 3,
    OFFICE_MIXED = 4
};

// Generate CDF header with embedded indirect offset patterns
void generate_cdf_header_with_indirect(FuzzedDataProvider& fdp, std::vector<uint8_t>& output, OfficeType office_type) {
    // Standard CDF header size
    const size_t header_size = 512;
    
    // Ensure we have enough space
    if (output.size() < header_size) {
        output.resize(header_size);
    }
    
    // Start with CDF magic
    uint64_t* magic_ptr = reinterpret_cast<uint64_t*>(output.data());
    *magic_ptr = CDF_MAGIC;
    
    // UUID fields (bytes 8-23) - embed indirect patterns here
    uint8_t* uuid_area = output.data() + 8;
    for (int i = 0; i < 16; i++) {
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            // Embed indirect patterns in UUID area
            uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (pattern_type) {
                case 0:
                    uuid_area[i] = INDIRECT_PATTERN_BYTE;
                    break;
                case 1:
                    if (i + 1 < 16) {
                        uuid_area[i] = 0x80;
                        uuid_area[i+1] = 0x00;
                        i++; // Skip next byte
                    }
                    break;
                case 2:
                    if (i + 3 < 16) {
                        uuid_area[i] = 0x80;
                        uuid_area[i+1] = 0x00;
                        uuid_area[i+2] = 0x00;
                        uuid_area[i+3] = 0x00;
                        i += 3; // Skip 3 bytes
                    }
                    break;
                case 3:
                    // Random byte from fuzzer
                    uuid_area[i] = fdp.ConsumeIntegral<uint8_t>();
                    break;
            }
        } else {
            uuid_area[i] = fdp.ConsumeIntegral<uint8_t>();
        }
    }
    
    // Revision and version (bytes 24-27)
    uint16_t* revision_ptr = reinterpret_cast<uint16_t*>(output.data() + 24);
    uint16_t* version_ptr = reinterpret_cast<uint16_t*>(output.data() + 26);
    *revision_ptr = 0x003E;  // Typical Office revision
    *version_ptr = 0x0003;   // Version 3
    
    // Byte order (bytes 28-29) - can be little or big endian
    uint16_t* byte_order_ptr = reinterpret_cast<uint16_t*>(output.data() + 28);
    *byte_order_ptr = fdp.ConsumeBool() ? 0xFFFE : 0xFEFF; // Little or big endian
    
    // Sector sizes (bytes 30-33)
    uint16_t* sec_size_ptr = reinterpret_cast<uint16_t*>(output.data() + 30);
    uint16_t* short_sec_size_ptr = reinterpret_cast<uint16_t*>(output.data() + 32);
    *sec_size_ptr = 9;       // 512-byte sectors (2^9)
    *short_sec_size_ptr = 6; // 64-byte short sectors (2^6)
    
    // Unused area (bytes 34-43) - can contain indirect patterns
    uint8_t* unused0 = output.data() + 34;
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
        if (fdp.ConsumeBool()) {
            // Occasionally embed indirect marker
            unused0[i] = INDIRECT_PATTERN_BYTE;
        } else {
            unused0[i] = fdp.ConsumeIntegral<uint8_t>();
        }
    }
    
    // Sector allocation table info (bytes 44-51)
    uint32_t* num_sectors_ptr = reinterpret_cast<uint32_t*>(output.data() + 44);
    uint32_t* first_dir_ptr = reinterpret_cast<uint32_t*>(output.data() + 48);
    *num_sectors_ptr = fdp.ConsumeIntegralInRange<uint32_t>(1, 100);
    *first_dir_ptr = fdp.ConsumeIntegral<uint32_t>();
    
    // More unused area (bytes 52-55) - more indirect pattern opportunities
    uint8_t* unused1 = output.data() + 52;
    for (int i = 0; i < 4 && fdp.remaining_bytes() > 0; i++) {
        unused1[i] = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Stream size threshold (bytes 56-59)
    uint32_t* min_stream_ptr = reinterpret_cast<uint32_t*>(output.data() + 56);
    *min_stream_ptr = 4096; // Typical Office threshold
    
    // Short SAT and master SAT pointers (bytes 60-71)
    int32_t* short_sat_ptr = reinterpret_cast<int32_t*>(output.data() + 60);
    uint32_t* num_short_sat_ptr = reinterpret_cast<uint32_t*>(output.data() + 64);
    int32_t* master_sat_ptr = reinterpret_cast<int32_t*>(output.data() + 68);
    uint32_t* num_master_sat_ptr = reinterpret_cast<uint32_t*>(output.data() + 72);
    
    *short_sat_ptr = fdp.ConsumeIntegral<int32_t>();
    *num_short_sat_ptr = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
    *master_sat_ptr = fdp.ConsumeIntegral<int32_t>();
    *num_master_sat_ptr = fdp.ConsumeIntegralInRange<uint32_t>(0, 100);
    
    // Master SAT array (bytes 76-511) - prime area for indirect patterns
    int32_t* master_sat_array = reinterpret_cast<int32_t*>(output.data() + 76);
    int sat_entries = (header_size - 76) / sizeof(int32_t);
    
    for (int i = 0; i < sat_entries && fdp.remaining_bytes() > 0; i++) {
        if (fdp.ConsumeBool() && (i % 8 == 0)) {
            // Embed indirect pattern markers at regular intervals
            master_sat_array[i] = static_cast<int32_t>(INDIRECT_PATTERN_LONG);
        } else {
            master_sat_array[i] = fdp.ConsumeIntegral<int32_t>();
        }
    }
}

// Generate Office document stream with indirect offset patterns
std::vector<uint8_t> generate_office_stream(FuzzedDataProvider& fdp, OfficeType office_type, bool include_indirect) {
    std::vector<uint8_t> stream;
    
    // Determine stream size based on office type
    size_t stream_size;
    switch (office_type) {
        case OFFICE_DOC:
            stream_size = fdp.ConsumeIntegralInRange<size_t>(1024, 8192);
            break;
        case OFFICE_XLS:
            stream_size = fdp.ConsumeIntegralInRange<size_t>(2048, 16384);
            break;
        case OFFICE_PPT:
            stream_size = fdp.ConsumeIntegralInRange<size_t>(4096, 32768);
            break;
        case OFFICE_MSI:
            stream_size = fdp.ConsumeIntegralInRange<size_t>(5120, 65536);
            break;
        default:
            stream_size = fdp.ConsumeIntegralInRange<size_t>(1024, 16384);
            break;
    }
    
    // Consume data for the stream
    if (fdp.remaining_bytes() < stream_size) {
        stream_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> base_data = fdp.ConsumeBytes<uint8_t>(stream_size);
    stream.insert(stream.end(), base_data.begin(), base_data.end());
    
    // Embed indirect offset patterns if requested
    if (include_indirect && stream.size() > 100) {
        // Embed indirect markers at strategic locations
        size_t num_markers = fdp.ConsumeIntegralInRange<size_t>(1, 10);
        
        for (size_t i = 0; i < num_markers && i * 10 < stream.size(); i++) {
            size_t marker_pos = i * 10;
            uint8_t marker_type = fdp.ConsumeIntegral<uint8_t>() % 5;
            
            switch (marker_type) {
                case 0: // Byte indirect marker
                    if (marker_pos < stream.size()) {
                        stream[marker_pos] = INDIRECT_PATTERN_BYTE;
                    }
                    break;
                case 1: // Short indirect marker (big endian)
                    if (marker_pos + 1 < stream.size()) {
                        stream[marker_pos] = 0x80;
                        stream[marker_pos + 1] = 0x00;
                    }
                    break;
                case 2: // Short indirect marker (little endian)
                    if (marker_pos + 1 < stream.size()) {
                        stream[marker_pos] = 0x00;
                        stream[marker_pos + 1] = 0x80;
                    }
                    break;
                case 3: // Long indirect marker
                    if (marker_pos + 3 < stream.size()) {
                        stream[marker_pos] = 0x80;
                        stream[marker_pos + 1] = 0x00;
                        stream[marker_pos + 2] = 0x00;
                        stream[marker_pos + 3] = 0x00;
                    }
                    break;
                case 4: // Quad indirect marker (first byte only)
                    if (marker_pos < stream.size()) {
                        stream[marker_pos] = 0x80;
                    }
                    break;
            }
        }
    }
    
    // Add Office-specific signatures
    switch (office_type) {
        case OFFICE_DOC:
            if (stream.size() > 8) {
                // WordDocument signature
                memcpy(stream.data(), "\xD0\xCF\x11\xE0\xA1\xB1\x1A\xE1", 8);
            }
            break;
        case OFFICE_XLS:
            if (stream.size() > 8) {
                // Excel signature
                memcpy(stream.data(), "\x09\x08\x10\x00\x00\x06\x05\x00", 8);
            }
            break;
        case OFFICE_PPT:
            if (stream.size() > 8) {
                // PowerPoint signature
                memcpy(stream.data(), "\x00\x6E\x1E\xF0", 4);
            }
            break;
        case OFFICE_MSI:
            if (stream.size() > 8) {
                // MSI signature
                memcpy(stream.data(), "\xD0\xCF\x11\xE0\xA1\xB1\x1A\xE1", 8);
            }
            break;
        default:
            // Mixed - no specific signature
            break;
    }
    
    return stream;
}

// Generate combined CDF document with indirect patterns
std::vector<uint8_t> generate_combined_cdf_document(FuzzedDataProvider& fdp) {
    std::vector<uint8_t> document;
    
    // Choose office type
    OfficeType office_type = static_cast<OfficeType>(fdp.ConsumeIntegral<uint8_t>() % 5);
    bool include_indirect = fdp.ConsumeBool();
    
    // Generate CDF header with embedded indirect patterns
    generate_cdf_header_with_indirect(fdp, document, office_type);
    
    // Add some padding between header and streams
    size_t padding = fdp.ConsumeIntegralInRange<size_t>(0, 512);
    std::vector<uint8_t> pad_data(padding, 0);
    document.insert(document.end(), pad_data.begin(), pad_data.end());
    
    // Generate Office document streams
    size_t num_streams = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    for (size_t i = 0; i < num_streams && fdp.remaining_bytes() > 100; i++) {
        std::vector<uint8_t> stream = generate_office_stream(fdp, office_type, include_indirect);
        document.insert(document.end(), stream.begin(), stream.end());
        
        // Add stream separator
        if (i < num_streams - 1) {
            size_t separator = fdp.ConsumeIntegralInRange<size_t>(0, 256);
            std::vector<uint8_t> sep_data(separator, 0);
            document.insert(document.end(), sep_data.begin(), sep_data.end());
        }
    }
    
    return document;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for CDF header + some content
    if (size < 1024) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume flags for magic_open - include both CDF and debug flags
    int flags = 0;
    
    uint8_t flag_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flag_byte & 0x01) flags |= MAGIC_DEBUG;        // Helpful for debugging
    if (flag_byte & 0x02) flags |= MAGIC_SYMLINK;
    if (flag_byte & 0x04) flags |= MAGIC_COMPRESS;     // Office docs might be compressed
    if (flag_byte & 0x08) flags |= MAGIC_DEVICES;
    if (flag_byte & 0x10) flags |= MAGIC_MIME_TYPE;    // Get MIME type for Office docs
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
    
    // If loading fails, try to load from buffers with CDF-specific patterns
    if (load_result == -1) {
        // Create a simple magic buffer with CDF pattern recognition
        std::string cdf_magic_pattern = 
            "0\tstring\t\\xD0\\xCF\\x11\\xE0\\xA1\\xB1\\x1A\\xE1\tComposite Document File V2 Document\n"
            "0\tstring\t\\x09\\x08\\x10\\x00\\x00\\x06\\x05\\x00\tMicrosoft Excel\n"
            "0\tstring\t\\x00\\x6E\\x1E\\xF0\tMicrosoft PowerPoint\n"
            "0\tstring\t\\xD0\\xCF\\x11\\xE0\\xA1\\xB1\\x1A\\xE1\tMSI Installer\n";
        
        void* buffers[1];
        size_t sizes[1];
        
        buffers[0] = malloc(cdf_magic_pattern.size());
        if (buffers[0] == NULL) {
            magic_close(magic);
            return 0;
        }
        
        memcpy(buffers[0], cdf_magic_pattern.data(), cdf_magic_pattern.size());
        sizes[0] = cdf_magic_pattern.size();
        
        load_result = magic_load_buffers(magic, buffers, sizes, 1);
        free(buffers[0]);
        
        if (load_result == -1) {
            magic_close(magic);
            return 0;
        }
    }
    
    // Generate combined CDF document with indirect patterns
    std::vector<uint8_t> test_document = generate_combined_cdf_document(fdp);
    
    if (!test_document.empty()) {
        // Test with magic_buffer - this will trigger both CDF parsing and mget indirect logic
        const char* result = magic_buffer(magic, test_document.data(), test_document.size());
        
        // Also test error handling by calling with partial data
        if (test_document.size() > 100) {
            const char* partial_result = magic_buffer(magic, test_document.data(), 100);
            (void)partial_result; // Result can be NULL
        }
        
        // Test with offset variations to trigger different mget paths
        if (test_document.size() > 500) {
            // Test with data starting at different offsets
            for (int offset = 0; offset < 3 && offset * 100 < test_document.size(); offset++) {
                size_t test_size = std::min<size_t>(test_document.size() - offset * 100, 300);
                const char* offset_result = magic_buffer(magic, test_document.data() + offset * 100, test_size);
                (void)offset_result;
            }
        }
        
        (void)result; // Result can be NULL for unrecognized types
    }
    
    // Clean up
    magic_close(magic);
    
    return 0;
}

} // namespace harness_023
