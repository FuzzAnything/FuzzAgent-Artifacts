#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_016 {
// Fuzzing harness for Little CMS 2 library
// Target: CGATS/IT8 file format operations with 69+ undiscovered branches
// Strategy: Specifically target file-based IT8 operations as identified by coverage analysis
// Target APIs: cmsIT8LoadFromFile (19 undiscovered branches), cmsIT8SaveToFile (18),
//              cmsIT8SetTableByLabel (32), cmsIT8GetDataRowCol (5), cmsIT8SetDataRowCol (5)
// Required helpers: cmsIT8Alloc, cmsIT8Free, cmsIT8SetPropertyStr, cmsIT8GetProperty
// Invocation sequence:
//   1. cmsIT8Alloc create handle
//   2. cmsIT8SetPropertyStr configure sheet type/properties
//   3. cmsIT8LoadFromFile load CGATS file from disk
//   4. cmsIT8SetTableByLabel manipulate table structure
//   5. cmsIT8GetDataRowCol / cmsIT8SetDataRowCol read/write data cells
//   6. cmsIT8SaveToFile save modified data to disk
//   7. cmsIT8Free cleanup
// Note: Uses temporary files for file I/O with fuzzer-generated content


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for IT8 content and various parameters
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create temporary file with IT8/CGATS content from fuzzer input
    // Generate simple IT8/CGATS format data for file loading
    std::string it8_content = "CGATS.17\n";
    it8_content += "KEYWORD \"Description\" \"Fuzzer-generated IT8 file\"\n";
    it8_content += "KEYWORD \"MeasurementCondition\" \"M0\"\n";
    it8_content += "KEYWORD \"Manufacturer\" \"FuzzLab\"\n";
    
    // Create main table with data format
    it8_content += "BEGIN_DATA_FORMAT\n";
    it8_content += "SAMPLE_ID RGB_R RGB_G RGB_B LAB_L LAB_A LAB_B\n";
    it8_content += "END_DATA_FORMAT\n";
    
    it8_content += "NUMBER_OF_FIELDS 7\n";
    it8_content += "NUMBER_OF_SETS 5\n";
    it8_content += "BEGIN_DATA\n";
    
    // Add some data rows from fuzzer input
    size_t num_rows = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < num_rows && fdp.remaining_bytes() > 20; i++) {
        std::string sample_id = "Patch_" + std::to_string(i);
        float r = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        float g = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        float b = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        float l = fdp.ConsumeFloatingPointInRange<float>(0.0f, 100.0f);
        float a = fdp.ConsumeFloatingPointInRange<float>(-128.0f, 127.0f);
        float b_val = fdp.ConsumeFloatingPointInRange<float>(-128.0f, 127.0f);
        
        char row[256];
        snprintf(row, sizeof(row), "%s %.6f %.6f %.6f %.6f %.6f %.6f\n", 
                sample_id.c_str(), r, g, b, l, a, b_val);
        it8_content += row;
    }
    
    it8_content += "END_DATA\n";
    
    // Add a second table for testing table operations
    std::string table_name = fdp.ConsumeRandomLengthString(16);
    if (table_name.empty()) table_name = "TABLE_2";
    
    std::string col1 = fdp.ConsumeRandomLengthString(8);
    std::string col2 = fdp.ConsumeRandomLengthString(8);
    std::string col3 = fdp.ConsumeRandomLengthString(8);
    if (col1.empty()) col1 = "X";
    if (col2.empty()) col2 = "Y";
    if (col3.empty()) col3 = "Z";
    
    it8_content += "BEGIN_DATA_FORMAT " + table_name + "\n";
    it8_content += "SAMPLE_ID " + col1 + " " + col2 + " " + col3 + "\n";
    it8_content += "END_DATA_FORMAT\n";
    it8_content += "BEGIN_DATA " + table_name + "\n";
    
    // Add some patch data with fuzzed values
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
        std::string patch_name = "Patch_" + std::to_string(i) + "_" + fdp.ConsumeRandomLengthString(4);
        float val1 = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        float val2 = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        float val3 = fdp.ConsumeFloatingPointInRange<float>(0.0f, 1.0f);
        
        char patch_row[256];
        snprintf(patch_row, sizeof(patch_row), "%s %.6f %.6f %.6f\n", 
                patch_name.c_str(), val1, val2, val3);
        it8_content += patch_row;
    }
    
    it8_content += "END_DATA\n";
    
    // Create temporary file for IT8 content
    char temp_filename[] = "/tmp/it8_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0; // Cannot create temp file
    }
    
    // Write IT8 content to temp file
    ssize_t written = write(fd, it8_content.c_str(), it8_content.size());
    close(fd);
    
    if (written != static_cast<ssize_t>(it8_content.size())) {
        unlink(temp_filename);
        return 0; // Failed to write file
    }
    
    // Step 2: Initialize context (use default context for file operations)
    cmsContext context = NULL;
    
    // Step 3: Allocate IT8 handle using cmsIT8Alloc
    cmsHANDLE it8_handle = cmsIT8Alloc(context);
    if (it8_handle == NULL) {
        unlink(temp_filename);
        return 0; // Cannot proceed without IT8 handle
    }
    
    // Step 4: Configure sheet type and properties using cmsIT8SetPropertyStr
    // Use fuzzed values for property configuration
    std::string prop_name = fdp.ConsumeRandomLengthString(32);
    std::string prop_value = fdp.ConsumeRandomLengthString(64);
    
    // Set some standard properties
    cmsIT8SetPropertyStr(it8_handle, "Description", "Fuzzer test IT8 file");
    cmsIT8SetPropertyStr(it8_handle, "Originator", "FuzzerAgent");
    cmsIT8SetPropertyStr(it8_handle, "MeasurementCondition", "M0");
    
    // Also try setting with fuzzed property name/value
    cmsIT8SetPropertyStr(it8_handle, prop_name.c_str(), prop_value.c_str());
    
    // Step 5: Load IT8 data from file using cmsIT8LoadFromFile
    // Free the allocated handle first since LoadFromFile creates its own
    cmsIT8Free(it8_handle);
    
    it8_handle = cmsIT8LoadFromFile(context, temp_filename);
    if (it8_handle == NULL) {
        // Try with a simpler file content if loading fails
        std::string simple_content = "CGATS.17\nBEGIN_DATA_FORMAT\nSAMPLE_ID\nEND_DATA_FORMAT\nBEGIN_DATA\nPatch_1\nEND_DATA\n";
        
        int simple_fd = mkstemp(temp_filename);
        if (simple_fd >= 0) {
            write(simple_fd, simple_content.c_str(), simple_content.size());
            close(simple_fd);
            
            it8_handle = cmsIT8LoadFromFile(context, temp_filename);
        }
    }
    
    if (it8_handle == NULL) {
        unlink(temp_filename);
        return 0; // Failed to load IT8 file
    }
    
    // Step 6: Test cmsIT8SetTableByLabel to manipulate table structure
    // Use fuzzed parameters for table operations
    std::string set_name = fdp.ConsumeRandomLengthString(32);
    std::string field_name = fdp.ConsumeRandomLengthString(32);
    std::string expected_type = fdp.ConsumeRandomLengthString(32);
    
    // Try with valid table label first
    int table_result = cmsIT8SetTableByLabel(it8_handle, "TABLE_2", "SAMPLE_ID", "SIDENT");
    (void)table_result; // Result may be success or failure
    
    // Try with fuzzed parameters
    table_result = cmsIT8SetTableByLabel(it8_handle, set_name.c_str(), 
                                         field_name.c_str(), expected_type.c_str());
    
    // Step 7: Test cmsIT8GetDataRowCol and cmsIT8SetDataRowCol for data cell operations
    // Use fuzzed row and column indices
    int row_idx = fdp.ConsumeIntegralInRange<int>(0, 10);
    int col_idx = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // First try to get data from a cell
    const char* cell_data = cmsIT8GetDataRowCol(it8_handle, row_idx, col_idx);
    (void)cell_data; // May be NULL if indices are out of bounds
    
    // Try to set data in a cell with fuzzed value
    std::string new_cell_value = fdp.ConsumeRandomLengthString(64);
    cmsBool set_result = cmsIT8SetDataRowCol(it8_handle, row_idx, col_idx, new_cell_value.c_str());
    (void)set_result; // Result may be success or failure
    
    // Also try with more reasonable indices for the actual data
    if (num_rows > 0) {
        int safe_row = fdp.ConsumeIntegralInRange<int>(0, static_cast<int>(num_rows) - 1);
        int safe_col = fdp.ConsumeIntegralInRange<int>(0, 6); // 0-6 for our 7 columns
        
        const char* safe_cell_data = cmsIT8GetDataRowCol(it8_handle, safe_row, safe_col);
        (void)safe_cell_data;
        
        std::string safe_new_value = fdp.ConsumeRandomLengthString(32);
        cmsIT8SetDataRowCol(it8_handle, safe_row, safe_col, safe_new_value.c_str());
    }
    
    // Step 8: Test cmsIT8GetProperty to verify properties
    const char* desc_prop = cmsIT8GetProperty(it8_handle, "Description");
    (void)desc_prop;
    
    // Try with fuzzed property name
    std::string fuzzed_prop_name = fdp.ConsumeRandomLengthString(32);
    const char* fuzzed_prop = cmsIT8GetProperty(it8_handle, fuzzed_prop_name.c_str());
    (void)fuzzed_prop;
    
    // Step 9: Save modified IT8 data to a new file using cmsIT8SaveToFile
    char save_filename[] = "/tmp/it8_fuzz_save_XXXXXX";
    int save_fd = mkstemp(save_filename);
    if (save_fd >= 0) {
        close(save_fd); // Close immediately, cmsIT8SaveToFile will open it
        
        cmsBool save_result = cmsIT8SaveToFile(it8_handle, save_filename);
        (void)save_result; // Result may be success or failure
        
        // If save succeeded, try loading the saved file
        if (save_result) {
            cmsHANDLE saved_it8 = cmsIT8LoadFromFile(context, save_filename);
            if (saved_it8 != NULL) {
                // Verify some properties from saved file
                const char* saved_desc = cmsIT8GetProperty(saved_it8, "Description");
                (void)saved_desc;
                
                cmsIT8Free(saved_it8);
            }
        }
        
        unlink(save_filename);
    }
    
    // Step 10: Cleanup
    cmsIT8Free(it8_handle);
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_016
