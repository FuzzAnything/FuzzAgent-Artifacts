#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_005 {
// Fuzzing harness for LittleCMS (lcms) IT8/CGATS file handling and cube file processing
// This harness specifically targets IT8/CGATS handling functions from cmscgats.c module
// that are currently 0% covered according to coverage analysis
// Target APIs: cmsCreateDeviceLinkFromCubeFile, cmsCreateDeviceLinkFromCubeFileTHR,
// cmsIT8LoadFromMem, cmsIT8SaveToMem, cmsIT8SetData/cmsIT8GetData,
// cmsIT8SetProperty/cmsIT8GetProperty
// MODULE: cmscgats.c [current coverage: 0%]


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing of IT8 and cube file operations
    // Minimum: parameters for context + IT8 data + cube data + property operations
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create a context for thread-safe operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0; // Cannot proceed without context
    }
    
    // 2. Create IT8 object
    cmsHANDLE hIT8 = cmsIT8Alloc(context);
    if (hIT8 == NULL) {
        cmsDeleteContext(context);
        return 0; // Cannot create IT8 object
    }
    
    // 3. Configure sheet type
    // Consume sheet type from input
    std::string sheet_type = fdp.ConsumeRandomLengthString(32);
    cmsIT8SetSheetType(hIT8, sheet_type.c_str());
    
    // 4. Load IT8 data from memory
    // Split input: part for IT8 data, part for cube data, part for properties
    size_t it8_data_size = fdp.ConsumeIntegralInRange<size_t>(64, size / 2);
    std::vector<uint8_t> it8_data = fdp.ConsumeBytes<uint8_t>(it8_data_size);
    
    cmsHANDLE loaded_it8 = cmsIT8LoadFromMem(context, it8_data.data(), 
                                             static_cast<cmsUInt32Number>(it8_data.size()));
    
    // 5. Data manipulation on the original IT8 object (not the loaded one)
    // Set and get properties
    std::string prop_name = fdp.ConsumeRandomLengthString(32);
    std::string prop_value = fdp.ConsumeRandomLengthString(64);
    cmsIT8SetPropertyStr(hIT8, prop_name.c_str(), prop_value.c_str());
    
    // Try to get the property back
    const char* retrieved_prop = cmsIT8GetProperty(hIT8, prop_name.c_str());
    // (retrieved_prop could be NULL, that's OK)
    
    // Set and get numeric properties
    std::string dbl_prop_name = fdp.ConsumeRandomLengthString(32);
    cmsFloat64Number dbl_value = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
    cmsIT8SetPropertyDbl(hIT8, dbl_prop_name.c_str(), dbl_value);
    
    cmsFloat64Number retrieved_dbl = cmsIT8GetPropertyDbl(hIT8, dbl_prop_name.c_str());
    // (we don't need to use the retrieved value)
    
    // Set and get data cells
    std::string patch_name = fdp.ConsumeRandomLengthString(32);
    std::string sample_name = fdp.ConsumeRandomLengthString(32);
    std::string cell_value = fdp.ConsumeRandomLengthString(64);
    
    cmsIT8SetData(hIT8, patch_name.c_str(), sample_name.c_str(), cell_value.c_str());
    
    const char* retrieved_data = cmsIT8GetData(hIT8, patch_name.c_str(), sample_name.c_str());
    // (retrieved_data could be NULL, that's OK)
    
    // Set and get numeric data
    std::string dbl_patch = fdp.ConsumeRandomLengthString(32);
    std::string dbl_sample = fdp.ConsumeRandomLengthString(32);
    cmsFloat64Number dbl_cell_value = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
    
    cmsIT8SetDataDbl(hIT8, dbl_patch.c_str(), dbl_sample.c_str(), dbl_cell_value);
    
    cmsFloat64Number retrieved_dbl_data = cmsIT8GetDataDbl(hIT8, dbl_patch.c_str(), dbl_sample.c_str());
    // (we don't need to use the retrieved value)
    
    // 6. Process cube files
    // Create a temporary file with cube data from fuzzer input
    char cube_temp_filename[] = "/tmp/lcms_cube_fuzz_XXXXXX";
    int cube_fd = mkstemp(cube_temp_filename);
    if (cube_fd != -1) {
        // Write cube header and data
        FILE* cube_file = fdopen(cube_fd, "w");
        if (cube_file != NULL) {
            // Write a simple cube file header
            fprintf(cube_file, "TITLE \"Fuzzer-generated cube file\"\n");
            fprintf(cube_file, "LUT_3D_SIZE 2\n"); // Smallest valid size
            fprintf(cube_file, "DOMAIN_MIN 0.0 0.0 0.0\n");
            fprintf(cube_file, "DOMAIN_MAX 1.0 1.0 1.0\n");
            
            // Write cube data from remaining fuzzer input
            std::string cube_data = fdp.ConsumeRemainingBytesAsString();
            // Write cube lines (need at least 8 lines for 2x2x2 cube)
            // We'll write synthetic data if we don't have enough
            if (cube_data.length() < 100) {
                // Generate synthetic cube data
                for (int i = 0; i < 8; i++) {
                    double r = (i & 1) ? 1.0 : 0.0;
                    double g = (i & 2) ? 1.0 : 0.0;
                    double b = (i & 4) ? 1.0 : 0.0;
                    fprintf(cube_file, "%.6f %.6f %.6f\n", r, g, b);
                }
            } else {
                // Try to parse cube data from input
                // Write as much as we can in a cube-like format
                size_t lines = cube_data.length() / 20; // Approximate
                if (lines > 8) lines = 8;
                for (size_t i = 0; i < lines; i++) {
                    size_t offset = i * 20;
                    if (offset + 20 <= cube_data.length()) {
                        // Write 3 numbers derived from the data
                        uint8_t vals[3];
                        for (int j = 0; j < 3; j++) {
                            vals[j] = cube_data[offset + j] % 256;
                        }
                        fprintf(cube_file, "%.6f %.6f %.6f\n", 
                                vals[0] / 255.0, vals[1] / 255.0, vals[2] / 255.0);
                    }
                }
            }
            
            fclose(cube_file);
            
            // TEST: Create device link from cube file (thread-safe version)
            cmsHPROFILE cube_profile = cmsCreateDeviceLinkFromCubeFileTHR(context, cube_temp_filename);
            if (cube_profile != NULL) {
                // Use the profile in a simple transform
                cmsHPROFILE dest_profile = cmsCreate_sRGBProfile();
                if (dest_profile != NULL) {
                    cmsUInt32Number intent = 0; // Use perceptual intent
                    cmsHTRANSFORM transform = cmsCreateTransform(cube_profile, TYPE_RGB_8,
                                                                 dest_profile, TYPE_RGB_8,
                                                                 intent, 0);
                    if (transform != NULL) {
                        uint8_t rgb_buffer[3] = {128, 128, 128};
                        cmsDoTransform(transform, rgb_buffer, rgb_buffer, 1);
                        cmsDeleteTransform(transform);
                    }
                    cmsCloseProfile(dest_profile);
                }
                cmsCloseProfile(cube_profile);
            }
            
            // TEST: Create device link from cube file (non-thread-safe version)
            cube_profile = cmsCreateDeviceLinkFromCubeFile(cube_temp_filename);
            if (cube_profile != NULL) {
                cmsCloseProfile(cube_profile);
            }
            
            // Clean up temp file
            unlink(cube_temp_filename);
        } else {
            close(cube_fd);
            unlink(cube_temp_filename);
        }
    }
    
    // 7. Save IT8 data to memory (if we have a loaded IT8 object)
    if (loaded_it8 != NULL) {
        // First get required buffer size
        cmsUInt32Number bytes_needed = 0;
        cmsIT8SaveToMem(loaded_it8, NULL, &bytes_needed);
        
        if (bytes_needed > 0 && bytes_needed < 1024 * 1024) { // Reasonable limit
            std::vector<uint8_t> save_buffer(bytes_needed);
            cmsUInt32Number actual_bytes = bytes_needed;
            cmsBool save_result = cmsIT8SaveToMem(loaded_it8, save_buffer.data(), &actual_bytes);
            // (save_result indicates success/failure, we don't need to check)
        }
        
        cmsIT8Free(loaded_it8);
    }
    
    // Also try to save the original IT8 object
    cmsUInt32Number orig_bytes_needed = 0;
    cmsIT8SaveToMem(hIT8, NULL, &orig_bytes_needed);
    
    if (orig_bytes_needed > 0 && orig_bytes_needed < 1024 * 1024) {
        std::vector<uint8_t> orig_save_buffer(orig_bytes_needed);
        cmsUInt32Number orig_actual_bytes = orig_bytes_needed;
        cmsIT8SaveToMem(hIT8, orig_save_buffer.data(), &orig_actual_bytes);
    }
    
    // 8. Test table operations
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    // (table_count is informational)
    
    if (table_count > 0) {
        // Try to set table (use 0 or modulo if table_count > 0)
        cmsUInt32Number table_index = 0;
        cmsIT8SetTable(hIT8, table_index);
    }
    
    // Test data format operations
    std::string sample_format = fdp.ConsumeRandomLengthString(32);
    int format_index = cmsIT8FindDataFormat(hIT8, sample_format.c_str());
    // (format_index is informational, could be -1)
    
    // 9. Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_005
