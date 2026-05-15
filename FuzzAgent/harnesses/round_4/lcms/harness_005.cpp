#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>

namespace harness_005 {
// Fuzzing harness for Little CMS 2 (lcms) library - Comprehensive IT8/CGATS file format module
// Targets CGATS/IT8 file format APIs with 0% coverage per coverage guidance
// Primary target APIs: cmsIT8LoadFromFile, cmsIT8SaveToFile, cmsIT8SetTableByLabel, 
//                      cmsIT8SetDataDbl, cmsIT8GetDataDbl (all 0% coverage)
// Supporting APIs: cmsIT8Alloc, cmsIT8Free, cmsIT8SetData
// Helper APIs: cmsIT8SetPropertyMulti, cmsIT8GetPropertyMulti, cmsIT8EnumProperties
// This harness focuses on complete IT8/CGATS file lifecycle with file-based operations,
// differentiating from harness_002.cpp which uses memory-based operations (LoadFromMem/SaveToMem)
// Invocation sequence: Alloc → LoadFromFile → SetTableByLabel → SetDataDbl/SetData → 
//                      SetPropertyMulti → GetDataDbl → GetPropertyMulti → SaveToFile → Free


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for multiple operations
    // Consume fixed-size data first for parameters
    uint8_t use_context = fdp.ConsumeBool();  // Whether to use thread-safe context
    uint8_t table_operation = fdp.ConsumeIntegral<uint8_t>() % 3;  // Different table operations
    uint8_t property_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);  // Number of properties to set
    uint8_t data_patch_count = fdp.ConsumeIntegralInRange<uint8_t>(1, 10);  // Number of data patches
    
    // Create context if requested
    cmsContext context = NULL;
    if (use_context) {
        context = cmsCreateContext(NULL, NULL);
        if (context == NULL) {
            return 0;  // Context creation failed
        }
    }
    
    // Generate IT8 file content from fuzzer input
    std::string it8_content = fdp.ConsumeRemainingBytesAsString();
    if (it8_content.length() < 64) {
        if (use_context) cmsDeleteContext(context);
        return 0;  // Need minimum IT8 content
    }
    
    // Create a temporary file for IT8 operations
    char temp_filename[] = "/tmp/lcms_it8_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        if (use_context) cmsDeleteContext(context);
        return 0;  // Failed to create temp file
    }
    
    // Write IT8 content to temporary file
    FILE* temp_file = fdopen(fd, "w");
    if (temp_file == NULL) {
        close(fd);
        if (use_context) cmsDeleteContext(context);
        return 0;
    }
    
    // Write minimal valid IT8 content
    // Based on testcms2.c example, need proper IT8 format
    fprintf(temp_file, "BEGIN_DATA_FORMAT\n");
    fprintf(temp_file, "SAMPLE_ID RGB_R RGB_G RGB_B\n");
    fprintf(temp_file, "END_DATA_FORMAT\n");
    fprintf(temp_file, "NUMBER_OF_SETS %d\n", data_patch_count);
    fprintf(temp_file, "BEGIN_DATA\n");
    
    // Add some sample data rows from fuzzer input
    size_t content_pos = 0;
    for (int i = 0; i < data_patch_count && content_pos + 20 < it8_content.length(); i++) {
        // Use characters from input for patch data
        fprintf(temp_file, "P%d %.3f %.3f %.3f\n", i,
                (double)(it8_content[content_pos] & 0xFF) / 255.0,
                (double)(it8_content[content_pos + 1] & 0xFF) / 255.0,
                (double)(it8_content[content_pos + 2] & 0xFF) / 255.0);
        content_pos += 3;
    }
    
    fprintf(temp_file, "END_DATA\n");
    fclose(temp_file);
    
    cmsHANDLE hIT8 = NULL;
    
    // Test 1: cmsIT8Alloc - Allocate IT8 structure
    hIT8 = cmsIT8Alloc(context);
    if (hIT8 == NULL) {
        remove(temp_filename);
        if (use_context) cmsDeleteContext(context);
        return 0;
    }
    
    // Test 2: cmsIT8LoadFromFile - Load IT8 data from file (0% coverage target)
    // Note: We need to free the previous handle and reload from file
    cmsIT8Free(hIT8);
    hIT8 = cmsIT8LoadFromFile(context, temp_filename);
    if (hIT8 == NULL) {
        // If loading fails, try creating a new IT8 structure
        hIT8 = cmsIT8Alloc(context);
        if (hIT8 == NULL) {
            remove(temp_filename);
            if (use_context) cmsDeleteContext(context);
            return 0;
        }
        
        // Set basic properties for new IT8 structure
        cmsIT8SetSheetType(hIT8, "LCMS/FUZZER");
        cmsIT8SetPropertyStr(hIT8, "ORIGINATOR", "Fuzzer Test");
        cmsIT8SetPropertyDbl(hIT8, "NUMBER_OF_SETS", data_patch_count);
        cmsIT8SetPropertyDbl(hIT8, "NUMBER_OF_FIELDS", 4);
        
        // Set data format
        cmsIT8SetDataFormat(hIT8, 0, "SAMPLE_ID");
        cmsIT8SetDataFormat(hIT8, 1, "RGB_R");
        cmsIT8SetDataFormat(hIT8, 2, "RGB_G");
        cmsIT8SetDataFormat(hIT8, 3, "RGB_B");
    }
    
    // Test 3: cmsIT8SetTableByLabel - Set data table by label (0% coverage target)
    // This API requires table to exist, so we try with various labels
    const char* table_labels[] = {"DATA", "DATA_FORMAT", "PROPERTIES", "DESCRIPTOR"};
    for (int i = 0; i < 4 && i <= table_operation; i++) {
        // Try setting table by label with different expected types
        const char* expected_types[] = {"DATA", "FORMAT", "PROPERTY", NULL};
        cmsIT8SetTableByLabel(hIT8, table_labels[i], "SAMPLE_ID", expected_types[i % 4]);
    }
    
    // Test 4: cmsIT8SetDataDbl and cmsIT8SetData - Set data values (0% coverage target)
    // Generate some patch data using remaining fuzzer bytes
    // First, consume some bytes for property values
    if (fdp.remaining_bytes() >= property_count * 16) {
        for (int i = 0; i < property_count; i++) {
            char prop_name[32];
            snprintf(prop_name, sizeof(prop_name), "PROP_%d", i);
            
            std::string prop_value = fdp.ConsumeRandomLengthString(16);
            cmsIT8SetPropertyStr(hIT8, prop_name, prop_value.c_str());
        }
    }
    
    // Set data values for patches
    for (int i = 0; i < data_patch_count && fdp.remaining_bytes() >= 32; i++) {
        char patch_name[32];
        snprintf(patch_name, sizeof(patch_name), "PATCH_%d", i);
        
        // Test cmsIT8SetDataDbl - set double precision data
        double rgb_r = fdp.ConsumeFloatingPoint<double>();
        double rgb_g = fdp.ConsumeFloatingPoint<double>();
        double rgb_b = fdp.ConsumeFloatingPoint<double>();
        
        cmsIT8SetDataDbl(hIT8, patch_name, "RGB_R", rgb_r);
        cmsIT8SetDataDbl(hIT8, patch_name, "RGB_G", rgb_g);
        cmsIT8SetDataDbl(hIT8, patch_name, "RGB_B", rgb_b);
        
        // Also test cmsIT8SetData for string values
        std::string sample_id = fdp.ConsumeRandomLengthString(16);
        cmsIT8SetData(hIT8, patch_name, "SAMPLE_ID", sample_id.c_str());
    }
    
    // Test 5: cmsIT8SetPropertyMulti - Set multi-valued property (0% coverage target)
    // This API requires property name and subkey
    if (fdp.remaining_bytes() >= 32) {
        std::string multi_key = fdp.ConsumeRandomLengthString(16);
        std::string multi_subkey = fdp.ConsumeRandomLengthString(16);
        std::string multi_value = fdp.ConsumeRandomLengthString(64);
        
        cmsIT8SetPropertyMulti(hIT8, multi_key.c_str(), multi_subkey.c_str(), multi_value.c_str());
        
        // Set a few more multi properties
        for (int i = 0; i < 3 && fdp.remaining_bytes() >= 48; i++) {
            char key[32], subkey[32];
            snprintf(key, sizeof(key), "MULTI_KEY_%d", i);
            snprintf(subkey, sizeof(subkey), "SUBKEY_%d", i);
            std::string value = fdp.ConsumeRandomLengthString(16);
            
            cmsIT8SetPropertyMulti(hIT8, key, subkey, value.c_str());
        }
    }
    
    // Test 6: cmsIT8GetDataDbl - Get double precision data (0% coverage target)
    // Retrieve previously set data
    for (int i = 0; i < data_patch_count && i < 5; i++) {
        char patch_name[32];
        snprintf(patch_name, sizeof(patch_name), "PATCH_%d", i);
        
        double retrieved_r = cmsIT8GetDataDbl(hIT8, patch_name, "RGB_R");
        double retrieved_g = cmsIT8GetDataDbl(hIT8, patch_name, "RGB_G");
        double retrieved_b = cmsIT8GetDataDbl(hIT8, patch_name, "RGB_B");
        (void)retrieved_r; (void)retrieved_g; (void)retrieved_b;  // Suppress unused warnings
    }
    
    // Test 7: cmsIT8GetPropertyMulti - Get multi-valued property (0% coverage target)
    if (fdp.remaining_bytes() >= 16) {
        std::string get_key = fdp.ConsumeRandomLengthString(16);
        std::string get_subkey = fdp.ConsumeRandomLengthString(16);
        
        const char* retrieved_multi = cmsIT8GetPropertyMulti(hIT8, get_key.c_str(), get_subkey.c_str());
        (void)retrieved_multi;
        
        // Try getting the multi property we set earlier
        const char* test_multi = cmsIT8GetPropertyMulti(hIT8, "MULTI_KEY_0", "SUBKEY_0");
        (void)test_multi;
    }
    
    // Test 8: cmsIT8EnumProperties - Enumerate properties (0% coverage target)
    char** property_names = NULL;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(hIT8, &property_names);
    if (prop_count > 0 && property_names != NULL) {
        // Properties enumerated successfully
        // Note: According to documentation, caller should free the array but not the strings
        // The strings are owned by the IT8 object
    }
    
    // Test 9: cmsIT8SaveToFile - Save IT8 data to file (0% coverage target)
    char output_filename[] = "/tmp/lcms_it8_out_XXXXXX";
    int out_fd = mkstemp(output_filename);
    if (out_fd != -1) {
        close(out_fd);  // Close the file descriptor since cmsIT8SaveToFile will open it
        
        cmsBool save_result = cmsIT8SaveToFile(hIT8, output_filename);
        (void)save_result;  // Suppress unused variable warning
        
        // Clean up output file
        remove(output_filename);
    }
    
    // Test 10: cmsIT8Free - Free IT8 structure
    cmsIT8Free(hIT8);
    
    // Clean up temporary file
    remove(temp_filename);
    
    // Clean up context if created
    if (use_context && context != NULL) {
        cmsDeleteContext(context);
    }
    
    return 0;
}

} // namespace harness_005
