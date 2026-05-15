#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_011 {
// Fuzzing harness for Little CMS 2 library
// Target: CGATS/IT8 data file manipulation operations with low coverage (21.93% region coverage)
// Strategy: Target specific IT8 APIs with high undiscovered branch counts as identified by coverage analysis
// Target APIs: cmsIT8SetTableByLabel (37 undiscovered branches), cmsIT8SetDataRowColDbl (6), 
//              cmsIT8SetDataRowCol (6), cmsIT8SetIndexColumn (7), cmsIT8SetPropertyStr (5),
//              cmsIT8SetComment (5), cmsIT8FindDataFormat (5), cmsIT8SaveToMem (16)
// Required setup: cmsIT8Alloc, cmsIT8LoadFromMem, cmsIT8SetTable, cmsIT8Free
// Invocation sequence: 
//   1. cmsIT8Alloc create handle
//   2. cmsIT8LoadFromMem load initial data  
//   3. cmsIT8SetTable establish structure
//   4. cmsIT8SetTableByLabel set table by label
//   5. cmsIT8SetIndexColumn define index column
//   6. cmsIT8SetDataRowColDbl and cmsIT8SetDataRowCol populate data
//   7. cmsIT8SetPropertyStr set properties
//   8. cmsIT8SetComment add comments
//   9. cmsIT8FindDataFormat locate data formats
//   10. cmsIT8SaveToMem save modified data
//   11. cmsIT8Free cleanup


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for IT8 data and various parameters
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create IT8/CGATS data from fuzzer input for memory loading
    // Generate a simple valid IT8/CGATS format data structure
    std::string it8_data = "CGATS.17\n";
    it8_data += "KEYWORD \"Description\" \"Test IT8 Data\"\n";
    it8_data += "KEYWORD \"MeasurementCondition\" \"M0\"\n";
    
    // Create tables with different names/labels
    it8_data += "BEGIN_DATA_FORMAT\n";
    it8_data += "SAMPLE_ID RGB_R RGB_G RGB_B LAB_L LAB_A LAB_B\n";
    it8_data += "END_DATA_FORMAT\n";
    
    it8_data += "NUMBER_OF_FIELDS 7\n";
    it8_data += "BEGIN_DATA\n";
    
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
        it8_data += row;
    }
    
    it8_data += "END_DATA\n";
    
    // Optional second table for testing table switching
    it8_data += "BEGIN_DATA_FORMAT TABLE_2\n";
    it8_data += "SAMPLE_ID X Y Z\n";
    it8_data += "END_DATA_FORMAT\n";
    it8_data += "BEGIN_DATA TABLE_2\n";
    it8_data += "Patch_0 0.1 0.2 0.3\n";
    it8_data += "Patch_1 0.4 0.5 0.6\n";
    it8_data += "END_DATA\n";
    
    // Step 2: Initialize context and allocate IT8 handle using cmsIT8Alloc
    cmsContext context = NULL; // Use default context
    
    // Step 3: Allocate empty IT8 handle using cmsIT8Alloc and test some operations on empty handle
    cmsHANDLE it8_handle = cmsIT8Alloc(context);
    if (it8_handle == NULL) {
        return 0; // Cannot proceed without valid IT8 handle
    }
    
    // Test some operations on the empty allocated handle before loading data
    // These may fail, but that's OK for fuzzing - we want to explore error paths too
    cmsIT8SetComment(it8_handle, "Allocated empty handle");
    cmsIT8SetPropertyStr(it8_handle, "TestProp", "Empty handle value");
    
    // Get sheet type from empty handle
    const char* sheet_type = cmsIT8GetSheetType(it8_handle);
    (void)sheet_type;
    
    // Free the empty handle before loading data
    cmsIT8Free(it8_handle);
    
    // Step 4: Load IT8 data from memory
    it8_handle = cmsIT8LoadFromMem(context, it8_data.c_str(), it8_data.length());
    if (it8_handle == NULL) {
        // If loading fails, try with simpler data
        std::string simple_it8 = "CGATS.17\nBEGIN_DATA_FORMAT\nSAMPLE_ID\nEND_DATA_FORMAT\nBEGIN_DATA\nPatch_1\nEND_DATA\n";
        it8_handle = cmsIT8LoadFromMem(context, simple_it8.c_str(), simple_it8.length());
        if (it8_handle == NULL) {
            return 0; // Cannot proceed without valid IT8 handle
        }
    }
    // Step 5: Test cmsIT8SetTable to switch tables
    // First get number of tables
    cmsUInt32Number table_count = cmsIT8TableCount(it8_handle);
    
    // Try switching to different tables if available
    if (table_count > 0) {
        for (cmsUInt32Number i = 0; i < table_count && i < 3; i++) {
            int result = cmsIT8SetTable(it8_handle, i);
            (void)result; // Result may be 0 or 1, ignore for fuzzing
        }
    }
    // Step 6: Test cmsIT8SetTableByLabel with both valid and fuzzed parameters
    // Try with valid table label from the generated data
    int valid_table_result = cmsIT8SetTableByLabel(it8_handle, "TABLE_2", "SAMPLE_ID", "SIDENT");
    (void)valid_table_result;
    
    // Also try with fuzzed parameters
    std::string set_name = fdp.ConsumeRandomLengthString(32);
    std::string field_name = fdp.ConsumeRandomLengthString(32);
    std::string expected_type = fdp.ConsumeRandomLengthString(32);
    
    // Try setting table by label (may fail if labels don't exist, that's OK)
    int table_by_label_result = cmsIT8SetTableByLabel(it8_handle, set_name.c_str(), 
                                                     field_name.c_str(), expected_type.c_str());
    (void)table_by_label_result;
    std::string index_column = fdp.ConsumeRandomLengthString(32);
    cmsBool index_result = cmsIT8SetIndexColumn(it8_handle, index_column.c_str());
    (void)index_result;
    
    // Step 8: Test cmsIT8SetDataRowCol and cmsIT8SetDataRowColDbl
    // Consume row and column indices
    int row_idx = fdp.ConsumeIntegralInRange<int>(0, 10);
    int col_idx = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // Test string data setting
    std::string cell_value = fdp.ConsumeRandomLengthString(64);
    cmsBool set_string_result = cmsIT8SetDataRowCol(it8_handle, row_idx, col_idx, cell_value.c_str());
    (void)set_string_result;
    
    // Test double data setting
    double double_value = fdp.ConsumeFloatingPoint<double>();
    cmsBool set_double_result = cmsIT8SetDataRowColDbl(it8_handle, row_idx, col_idx, double_value);
    (void)set_double_result;
    
    // Step 9: Test cmsIT8SetPropertyStr with fuzzed property names and values
    std::string prop_name = fdp.ConsumeRandomLengthString(32);
    std::string prop_value = fdp.ConsumeRandomLengthString(128);
    cmsBool prop_result = cmsIT8SetPropertyStr(it8_handle, prop_name.c_str(), prop_value.c_str());
    (void)prop_result;
    
    // Try some common property names that might be recognized
    cmsIT8SetPropertyStr(it8_handle, "Description", "Fuzzer-generated IT8 data");
    cmsIT8SetPropertyStr(it8_handle, "Originator", "Fuzzer");
    cmsIT8SetPropertyStr(it8_handle, "Created", "2024-01-01");
    
    // Step 10: Test cmsIT8SetComment with fuzzed comment
    std::string comment = fdp.ConsumeRandomLengthString(256);
    cmsBool comment_result = cmsIT8SetComment(it8_handle, comment.c_str());
    (void)comment_result;
    
    // Step 11: Test cmsIT8FindDataFormat with fuzzed sample name
    std::string sample_name = fdp.ConsumeRandomLengthString(32);
    int format_idx = cmsIT8FindDataFormat(it8_handle, sample_name.c_str());
    (void)format_idx;
    
    // Also try some likely sample names
    cmsIT8FindDataFormat(it8_handle, "RGB_R");
    cmsIT8FindDataFormat(it8_handle, "RGB_G");
    cmsIT8FindDataFormat(it8_handle, "RGB_B");
    cmsIT8FindDataFormat(it8_handle, "LAB_L");
    cmsIT8FindDataFormat(it8_handle, "SAMPLE_ID");
    
    // Step 12: Test cmsIT8SaveToMem to save modified data
    cmsUInt32Number buffer_size = 0;
    // First call to get required buffer size
    cmsBool save_result = cmsIT8SaveToMem(it8_handle, NULL, &buffer_size);
    
    if (save_result && buffer_size > 0 && buffer_size < 1024 * 1024) { // Reasonable size limit
        std::vector<char> save_buffer(buffer_size);
        cmsUInt32Number bytes_needed = buffer_size;
        save_result = cmsIT8SaveToMem(it8_handle, save_buffer.data(), &bytes_needed);
        (void)save_result;
    }
    
    // Step 13: Cleanup
    cmsIT8Free(it8_handle);
    
    return 0;
}

} // namespace harness_011
