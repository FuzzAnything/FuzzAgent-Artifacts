#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_024 {
// Fuzzing harness for Little CMS 2 library
// Target: IT8/CGATS table manipulation module with 61+ undiscovered branches
// Strategy: Comprehensive table manipulation focusing on label-based table navigation
// and file save operations as identified by CoverageAnalyzerAgent
// Target APIs: cmsIT8SetTableByLabel (37 undiscovered branches), cmsIT8SaveToFile (18),
//              cmsIT8TableCount (2), cmsIT8SetTable (4), cmsIT8SetIndexColumn (7),
//              cmsIT8GetDataRowCol (5), cmsIT8SetDataRowCol (5), cmsIT8SetPropertyStr (5)
// Required helpers: cmsIT8Alloc, cmsIT8LoadFromMem, cmsIT8Free, cmsIT8SetPropertyMulti
// Semantic diversity from existing harnesses (000-023): 
// - harness_005: Focuses on cube file device link creation with basic IT8 ops
// - harness_011: Tests cmsIT8SetTableByLabel but with limited table structure
// - harness_015: Focuses on property and data manipulation
// - harness_016: Tests file-based operations with simple table structures
// - harness_020: Focuses on patch name operations
// This harness (024): Implements complex multi-table structure with label-based navigation,
//                     comprehensive table switching, and error path exploration for 
//                     cmsIT8SetTableByLabel edge cases
// Invocation sequence:
//   1. Create complex IT8 structure with multiple labeled tables
//   2. Load IT8 from memory
//   3. Test cmsIT8TableCount to get table count
//   4. Test cmsIT8SetTable for direct table switching
//   5. Test cmsIT8SetTableByLabel with various parameter combinations
//   6. Test cmsIT8SetIndexColumn for column indexing
//   7. Test cmsIT8GetDataRowCol/cmsIT8SetDataRowCol for data manipulation
//   8. Test cmsIT8SetPropertyStr for property management
//   9. Test cmsIT8SaveToFile with temporary file
//   10. Test error conditions and edge cases


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for complex IT8 structure and parameters
    const size_t MIN_SIZE = 256;
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create complex IT8/CGATS data structure with multiple labeled tables
    // This creates a structure with 3 tables, each with different labels and formats
    std::string it8_content = "CGATS.17\n";
    it8_content += "KEYWORD \"Description\" \"Complex multi-table IT8 structure\"\n";
    it8_content += "KEYWORD \"Creator\" \"Fuzzer harness 024\"\n";
    it8_content += "KEYWORD \"Version\" \"1.0\"\n";
    
    // Table 1: RGB measurements with LABEL field for table identification
    it8_content += "BEGIN_DATA_FORMAT TABLE_RGB\n";
    it8_content += "SAMPLE_ID RGB_R RGB_G RGB_B LABEL\n";
    it8_content += "END_DATA_FORMAT\n";
    it8_content += "BEGIN_DATA TABLE_RGB\n";
    
    // Add data rows with label field containing table identification
    size_t rgb_rows = fdp.ConsumeIntegralInRange<size_t>(2, 6);
    for (size_t i = 0; i < rgb_rows && fdp.remaining_bytes() > 20; i++) {
        std::string sample_id = "RGB_Patch_" + std::to_string(i);
        float r = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        float g = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        float b = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        
        char row[256];
        snprintf(row, sizeof(row), "%s %.6f %.6f %.6f \"RGB_TABLE 1 RGB\"\n", 
                sample_id.c_str(), r, g, b);
        it8_content += row;
    }
    it8_content += "END_DATA\n";
    
    // Table 2: XYZ measurements with different label format
    it8_content += "BEGIN_DATA_FORMAT TABLE_XYZ\n";
    it8_content += "SAMPLE_ID XYZ_X XYZ_Y XYZ_Z LABEL\n";
    it8_content += "END_DATA_FORMAT\n";
    it8_content += "BEGIN_DATA TABLE_XYZ\n";
    
    size_t xyz_rows = fdp.ConsumeIntegralInRange<size_t>(2, 6);
    for (size_t i = 0; i < xyz_rows && fdp.remaining_bytes() > 20; i++) {
        std::string sample_id = "XYZ_Patch_" + std::to_string(i);
        float x = fdp.ConsumeFloatingPointInRange<float>(0.0f, 100.0f);
        float y = fdp.ConsumeFloatingPointInRange<float>(0.0f, 100.0f);
        float z = fdp.ConsumeFloatingPointInRange<float>(0.0f, 100.0f);
        
        char row[256];
        snprintf(row, sizeof(row), "%s %.6f %.6f %.6f \"XYZ_TABLE 2 XYZ\"\n", 
                sample_id.c_str(), x, y, z);
        it8_content += row;
    }
    it8_content += "END_DATA\n";
    
    // Table 3: LAB measurements with yet another label format
    it8_content += "BEGIN_DATA_FORMAT TABLE_LAB\n";
    it8_content += "SAMPLE_ID LAB_L LAB_A LAB_B LABEL\n";
    it8_content += "END_DATA_FORMAT\n";
    it8_content += "BEGIN_DATA TABLE_LAB\n";
    
    size_t lab_rows = fdp.ConsumeIntegralInRange<size_t>(2, 6);
    for (size_t i = 0; i < lab_rows && fdp.remaining_bytes() > 20; i++) {
        std::string sample_id = "LAB_Patch_" + std::to_string(i);
        float l = fdp.ConsumeFloatingPointInRange<float>(0.0f, 100.0f);
        float a = fdp.ConsumeFloatingPointInRange<float>(-128.0f, 127.0f);
        float b_val = fdp.ConsumeFloatingPointInRange<float>(-128.0f, 127.0f);
        
        char row[256];
        snprintf(row, sizeof(row), "%s %.6f %.6f %.6f \"LAB_TABLE 3 LAB\"\n", 
                sample_id.c_str(), l, a, b_val);
        it8_content += row;
    }
    it8_content += "END_DATA\n";
    
    // Step 2: Load IT8 from memory
    cmsContext context = NULL; // Use default context
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, it8_content.c_str(), it8_content.length());
    if (hIT8 == NULL) {
        // If loading fails, try with simpler data
        std::string simple_it8 = "CGATS.17\nBEGIN_DATA_FORMAT\nSAMPLE_ID LABEL\nEND_DATA_FORMAT\nBEGIN_DATA\nPatch_1 \"SIMPLE_TABLE 1 SIMPLE\"\nEND_DATA\n";
        hIT8 = cmsIT8LoadFromMem(context, simple_it8.c_str(), simple_it8.length());
        if (hIT8 == NULL) {
            return 0; // Cannot proceed without valid IT8 handle
        }
    }
    
    // Step 3: Test cmsIT8TableCount to get number of tables
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    (void)table_count; // Use value to avoid unused variable warning
    
    // Step 4: Test direct table switching with cmsIT8SetTable
    if (table_count > 0) {
        // Try switching to each table
        for (cmsUInt32Number i = 0; i < table_count && i < 5; i++) {
            int set_result = cmsIT8SetTable(hIT8, i);
            (void)set_result; // May fail for invalid indices, explore both paths
        }
        
        // Try invalid table indices to test error handling
        cmsIT8SetTable(hIT8, 999); // Invalid index
        cmsIT8SetTable(hIT8, table_count); // Just beyond valid range
    }
    
    // Step 5: Test cmsIT8SetTableByLabel with various parameter combinations
    // This is the primary target API with 37 undiscovered branches
    
    // Test 5.1: Valid table switching by label with correct expected type
    int label_result1 = cmsIT8SetTableByLabel(hIT8, "RGB_Patch_0", "LABEL", "RGB");
    (void)label_result1;
    
    int label_result2 = cmsIT8SetTableByLabel(hIT8, "XYZ_Patch_0", "LABEL", "XYZ");
    (void)label_result2;
    
    int label_result3 = cmsIT8SetTableByLabel(hIT8, "LAB_Patch_0", "LABEL", "LAB");
    (void)label_result3;
    
    // Test 5.2: Table switching by label with NULL field (should default to "LABEL")
    int label_result4 = cmsIT8SetTableByLabel(hIT8, "RGB_Patch_1", NULL, "RGB");
    (void)label_result4;
    
    // Test 5.3: Table switching by label with empty field string (should default to "LABEL")
    int label_result5 = cmsIT8SetTableByLabel(hIT8, "XYZ_Patch_1", "", "XYZ");
    (void)label_result5;
    
    // Test 5.4: Table switching by label with NULL expected type
    int label_result6 = cmsIT8SetTableByLabel(hIT8, "LAB_Patch_1", "LABEL", NULL);
    (void)label_result6;
    
    // Test 5.5: Table switching by label with empty expected type string
    int label_result7 = cmsIT8SetTableByLabel(hIT8, "RGB_Patch_2", "LABEL", "");
    (void)label_result7;
    
    // Test 5.6: Table switching with mismatched expected type (should fail)
    int label_result8 = cmsIT8SetTableByLabel(hIT8, "RGB_Patch_0", "LABEL", "WRONG_TYPE");
    (void)label_result8;
    
    // Test 5.7: Table switching with non-existent patch/set
    std::string fake_patch = fdp.ConsumeRandomLengthString(32);
    std::string fake_field = fdp.ConsumeRandomLengthString(32);
    std::string fake_type = fdp.ConsumeRandomLengthString(32);
    
    int label_result9 = cmsIT8SetTableByLabel(hIT8, fake_patch.c_str(), fake_field.c_str(), fake_type.c_str());
    (void)label_result9;
    
    // Test 5.8: Table switching with malformed label data (should fail parsing)
    // First add a patch with malformed label
    std::string malformed_patch = "MALFORMED_Patch_0";
    std::string malformed_data = "MALFORMED_LABEL NOT_A_NUMBER NOT_A_TYPE";
    cmsIT8SetData(hIT8, malformed_patch.c_str(), "LABEL", malformed_data.c_str());
    
    int label_result10 = cmsIT8SetTableByLabel(hIT8, malformed_patch.c_str(), "LABEL", NULL);
    (void)label_result10;
    
    // Step 6: Test cmsIT8SetIndexColumn for column indexing
    // Try with valid column names from our data format
    cmsIT8SetIndexColumn(hIT8, "SAMPLE_ID");
    cmsIT8SetIndexColumn(hIT8, "RGB_R");
    cmsIT8SetIndexColumn(hIT8, "RGB_G");
    cmsIT8SetIndexColumn(hIT8, "RGB_B");
    
    // Try with invalid column names
    std::string invalid_column = fdp.ConsumeRandomLengthString(32);
    cmsIT8SetIndexColumn(hIT8, invalid_column.c_str());
    
    // Step 7: Test cmsIT8GetDataRowCol and cmsIT8SetDataRowCol for data manipulation
    // Test with various row/column indices
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            // First try to get existing data
            const char* cell_data = cmsIT8GetDataRowCol(hIT8, row, col);
            (void)cell_data;
            
            // Then try to set new data with fuzzed content
            std::string new_cell_value = fdp.ConsumeRandomLengthString(64);
            cmsIT8SetDataRowCol(hIT8, row, col, new_cell_value.c_str());
        }
    }
    
    // Step 8: Test cmsIT8SetPropertyStr for property management
    // Set various properties
    std::string prop_name1 = fdp.ConsumeRandomLengthString(32);
    std::string prop_value1 = fdp.ConsumeRandomLengthString(128);
    cmsIT8SetPropertyStr(hIT8, prop_name1.c_str(), prop_value1.c_str());
    
    // Set standard properties
    cmsIT8SetPropertyStr(hIT8, "Description", "Fuzzer-modified IT8 data");
    cmsIT8SetPropertyStr(hIT8, "Modified", "by harness_024");
    cmsIT8SetPropertyStr(hIT8, "Originator", "Coverage-driven fuzzing");
    
    // Test cmsIT8SetPropertyMulti for multi-valued properties
    std::string multi_key = fdp.ConsumeRandomLengthString(16);
    std::string multi_subkey = fdp.ConsumeRandomLengthString(16);
    std::string multi_value = fdp.ConsumeRandomLengthString(256);
    cmsIT8SetPropertyMulti(hIT8, multi_key.c_str(), multi_subkey.c_str(), multi_value.c_str());
    
    // Step 9: Test cmsIT8SaveToFile with temporary file (primary target API)
    // Create temporary file for saving
    char temp_filename[] = "/tmp/it8_fuzz_024_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd >= 0) {
        close(temp_fd); // Close the file descriptor, we'll use cmsIT8SaveToFile
        
        // Test cmsIT8SaveToFile - this is a key target with 18 undiscovered branches
        cmsBool save_result = cmsIT8SaveToFile(hIT8, temp_filename);
        (void)save_result; // May succeed or fail, explore both paths
        
        // Clean up temporary file
        unlink(temp_filename);
    }
    
    // Step 10: Test additional edge cases
    // Switch back to first table and test operations again
    if (table_count > 0) {
        cmsIT8SetTable(hIT8, 0);
        
        // Test data operations on first table
        const char* sample_data = cmsIT8GetData(hIT8, "RGB_Patch_0", "RGB_R");
        (void)sample_data;
        
        // Set new data
        std::string new_rgb_value = std::to_string(fdp.ConsumeFloatingPoint<double>());
        cmsIT8SetData(hIT8, "RGB_Patch_0", "RGB_R", new_rgb_value.c_str());
    }
    
    // Step 11: Cleanup
    cmsIT8Free(hIT8);
    
    return 0;
}

} // namespace harness_024
