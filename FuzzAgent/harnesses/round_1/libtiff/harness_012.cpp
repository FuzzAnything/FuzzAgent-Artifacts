#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tiffio.h"
#include "tiff.h"

namespace harness_012 {
/*
 * Fuzzing harness for libtiff field extension and custom field management APIs
 * Target APIs: TIFFMergeFieldInfo (highest uncovered API with 46 accumulated branches, 33 undiscovered), 
 *              TIFFSetClientInfo, TIFFGetClientInfo, TIFFDeferStrileArrayWriting (completely uncovered)
 * Required helper APIs: TIFFOpen/TIFFOpenExt, TIFFClose, TIFFSetField, TIFFGetField
 * 
 * This harness focuses specifically on field extension and management operations:
 * 1. TIFFMergeFieldInfo - Field merging for custom tag extensions (highest complexity)
 * 2. TIFFSetClientInfo - Setting client-specific information 
 * 3. TIFFGetClientInfo - Retrieving client-specific information
 * 4. TIFFDeferStrileArrayWriting - Deferred strile array writing control
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_006 touches TIFFMergeFieldInfo but skips it due to missing proper TIFFFieldInfo structures
 * - This harness provides proper TIFFFieldInfo structures based on fuzzer input
 * - Tests field merging with valid and invalid field configurations
 * - Tests client info management API pair (set/get)
 * - Tests deferral of strile array writing for performance optimization
 * - Tests complex field management scenarios with multiple custom fields
 */



// Error handler to suppress warnings during fuzzing
extern "C" void tiff_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress errors during fuzzing
    return;
}

// Structure for custom client info
struct CustomClientInfo {
    uint32_t id;
    uint8_t flags;
    uint64_t timestamp;
    char name[32];
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for field info structures and parameters
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Set error handlers to suppress warnings
    TIFFSetErrorHandler(NULL);
    TIFFSetWarningHandler(NULL);

    // Create a temporary file for TIFF operations
    char temp_file_template[] = "/tmp/fuzz_tiff_field_mgmt_XXXXXX";
    int fd = mkstemp(temp_file_template);
    if (fd < 0) {
        return 0;
    }
    std::string temp_filename = temp_file_template;
    
    // Write minimal valid TIFF header to the temporary file
    uint8_t tiff_header[] = {
        0x49, 0x49, 0x2A, 0x00,  // Little-endian, classic TIFF
        0x08, 0x00, 0x00, 0x00,  // First IFD offset
        0x00, 0x00,              // Number of directory entries (0 = end)
        0x00, 0x00, 0x00, 0x00   // Next IFD offset (0 = no more)
    };
    
    if (write(fd, tiff_header, sizeof(tiff_header)) != (ssize_t)sizeof(tiff_header)) {
        close(fd);
        unlink(temp_filename.c_str());
        return 0;
    }
    close(fd);

    // Open the TIFF file for writing to set up test configuration
    TIFF* tif = nullptr;
    TIFFOpenOptions* opts = nullptr;
    
    // Choose opening method based on fuzzed input
    uint8_t open_method = fdp.ConsumeIntegral<uint8_t>() % 3;
    std::string mode = "w"; // Write mode to allow setting fields
    
    switch (open_method) {
        case 0:
            // Standard TIFFOpen
            tif = TIFFOpen(temp_filename.c_str(), mode.c_str());
            break;
            
        case 1:
            // TIFFOpenExt with options
            opts = TIFFOpenOptionsAlloc();
            if (opts) {
                // Configure some options if needed
                tif = TIFFOpenExt(temp_filename.c_str(), mode.c_str(), opts);
                TIFFOpenOptionsFree(opts);
            }
            break;
            
        case 2:
            // TIFFFdOpen
            fd = open(temp_filename.c_str(), O_RDWR);
            if (fd >= 0) {
                tif = TIFFFdOpen(fd, temp_filename.c_str(), mode.c_str());
            }
            break;
    }
    
    if (!tif) {
        // Clean up temporary file
        unlink(temp_filename.c_str());
        return 0;
    }

    // Set up basic image configuration to make the file valid
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 4096);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 16);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height); // One strip

    // Consume operation type to determine which APIs to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    switch (operation) {
        case 0: {
            // Test TIFFMergeFieldInfo with custom field definitions
            // Create custom field info based on fuzzer input
            uint8_t num_fields = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);
            std::vector<TIFFFieldInfo> field_info(num_fields);
            
            for (uint8_t i = 0; i < num_fields && fdp.remaining_bytes() > 20; i++) {
                // Generate a unique tag in private range (65000-65535)
                field_info[i].field_tag = 65000 + (fdp.ConsumeIntegral<uint16_t>() % 535);
                
                // Consume field counts (-3 to 10 range for testing)
                int16_t read_count = fdp.ConsumeIntegralInRange<int16_t>(-3, 10);
                int16_t write_count = fdp.ConsumeIntegralInRange<int16_t>(-3, 10);
                field_info[i].field_readcount = read_count;
                field_info[i].field_writecount = write_count;
                
                // Consume field type from valid TIFF data types
                uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % 13;
                TIFFDataType field_types[] = {
                    TIFF_BYTE, TIFF_ASCII, TIFF_SHORT, TIFF_LONG,
                    TIFF_RATIONAL, TIFF_SBYTE, TIFF_UNDEFINED,
                    TIFF_SSHORT, TIFF_SLONG, TIFF_SRATIONAL,
                    TIFF_FLOAT, TIFF_DOUBLE, TIFF_IFD8
                };
                field_info[i].field_type = field_types[type_idx];
                
                // Consume field bit (0-31)
                field_info[i].field_bit = fdp.ConsumeIntegral<uint8_t>() % 32;
                
                // Consume boolean flags
                field_info[i].field_oktochange = fdp.ConsumeBool() ? 1 : 0;
                field_info[i].field_passcount = fdp.ConsumeBool() ? 1 : 0;
                
                // Generate field name from fuzzer input
                std::string field_name = fdp.ConsumeRandomLengthString(32);
                if (!field_name.empty()) {
                    // Note: This creates a memory leak but is acceptable for fuzzing
                    char* name_copy = strdup(field_name.c_str());
                    field_info[i].field_name = name_copy;
                } else {
                    field_info[i].field_name = NULL;
                }
            }
            
            // Test TIFFMergeFieldInfo with the constructed field info
            int merge_result = TIFFMergeFieldInfo(tif, field_info.data(), num_fields);
            (void)merge_result;
            
            // Clean up allocated field names
            for (uint8_t i = 0; i < num_fields; i++) {
                if (field_info[i].field_name) {
                    free((void*)field_info[i].field_name);
                }
            }
            break;
        }
        
        case 1: {
            // Test client info management APIs (TIFFSetClientInfo/TIFFGetClientInfo)
            // Create custom client info structure
            CustomClientInfo client_info;
            client_info.id = fdp.ConsumeIntegral<uint32_t>();
            client_info.flags = fdp.ConsumeIntegral<uint8_t>();
            client_info.timestamp = fdp.ConsumeIntegral<uint64_t>();
            
            std::string client_name = fdp.ConsumeRandomLengthString(31);
            strncpy(client_info.name, client_name.c_str(), sizeof(client_info.name) - 1);
            client_info.name[sizeof(client_info.name) - 1] = '\0';
            
            // Generate client key from fuzzer input
            std::string client_key = fdp.ConsumeRandomLengthString(64);
            
            // Test TIFFSetClientInfo
            TIFFSetClientInfo(tif, &client_info, client_key.c_str());
            
            // Test TIFFGetClientInfo
            void* retrieved_info = TIFFGetClientInfo(tif, client_key.c_str());
            (void)retrieved_info;
            
            // Test with different key to check non-existent case
            std::string other_key = fdp.ConsumeRandomLengthString(64);
            void* other_info = TIFFGetClientInfo(tif, other_key.c_str());
            (void)other_info;
            
            // Test setting NULL client info (removal)
            if (fdp.ConsumeBool()) {
                TIFFSetClientInfo(tif, NULL, client_key.c_str());
            }
            break;
        }
        
        case 2: {
            // Test TIFFDeferStrileArrayWriting
            // This API defers writing of strile arrays (strip/tile offsets and byte counts)
            int defer_result = TIFFDeferStrileArrayWriting(tif);
            (void)defer_result;
            
            // After deferring, we should still be able to set fields
            // Set some additional fields to exercise deferred path
            if (fdp.ConsumeBool()) {
                uint32_t x_resolution = fdp.ConsumeIntegral<uint32_t>();
                uint32_t y_resolution = fdp.ConsumeIntegral<uint32_t>();
                TIFFSetField(tif, TIFFTAG_XRESOLUTION, static_cast<float>(x_resolution));
                TIFFSetField(tif, TIFFTAG_YRESOLUTION, static_cast<float>(y_resolution));
            }
            
            if (fdp.ConsumeBool()) {
                std::string software = fdp.ConsumeRandomLengthString(128);
                TIFFSetField(tif, TIFFTAG_SOFTWARE, software.c_str());
            }
            
            // Try to force writing (complementary API)
            if (fdp.ConsumeBool()) {
                int force_result = TIFFForceStrileArrayWriting(tif);
                (void)force_result;
            }
            break;
        }
        
        case 3: {
            // Combined test: Merge field info and then use client info
            // Create a simple custom field
            TIFFFieldInfo simple_field;
            simple_field.field_tag = 65000 + (fdp.ConsumeIntegral<uint16_t>() % 100);
            simple_field.field_readcount = 1;
            simple_field.field_writecount = 1;
            simple_field.field_type = TIFF_LONG;
            simple_field.field_bit = FIELD_CUSTOM;
            simple_field.field_oktochange = 1;
            simple_field.field_passcount = 0;
            
            std::string field_name = "CustomField" + std::to_string(simple_field.field_tag);
            char* name_copy = strdup(field_name.c_str());
            simple_field.field_name = name_copy;
            
            // Merge the field
            int merge_result = TIFFMergeFieldInfo(tif, &simple_field, 1);
            (void)merge_result;
            
            // Set client info related to this field
            uint32_t field_data = fdp.ConsumeIntegral<uint32_t>();
            std::string client_key = "FieldData_" + std::to_string(simple_field.field_tag);
            TIFFSetClientInfo(tif, &field_data, client_key.c_str());
            
            // Retrieve client info
            void* retrieved_data = TIFFGetClientInfo(tif, client_key.c_str());
            (void)retrieved_data;
            
            free(name_copy);
            break;
        }
        
        case 4: {
            // Test field management with directory operations
            // First create a directory
            TIFFCreateDirectory(tif);
            
            // Test deferral in new directory
            int defer_result = TIFFDeferStrileArrayWriting(tif);
            (void)defer_result;
            
            // Set client info in this directory
            uint32_t dir_info = fdp.ConsumeIntegral<uint32_t>();
            std::string dir_key = "DirectoryInfo";
            TIFFSetClientInfo(tif, &dir_info, dir_key.c_str());
            
            // Try to get it back
            void* retrieved_dir_info = TIFFGetClientInfo(tif, dir_key.c_str());
            (void)retrieved_dir_info;
            
            // Test with multiple client info entries
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                uint32_t multi_info = fdp.ConsumeIntegral<uint32_t>();
                std::string multi_key = "MultiInfo_" + std::to_string(i);
                TIFFSetClientInfo(tif, &multi_info, multi_key.c_str());
                
                void* retrieved_multi = TIFFGetClientInfo(tif, multi_key.c_str());
                (void)retrieved_multi;
            }
            break;
        }
    }
    
    // Clean up
    TIFFClose(tif);
    
    // Clean up temporary file
    unlink(temp_filename.c_str());
    
    return 0;
}

} // namespace harness_012
