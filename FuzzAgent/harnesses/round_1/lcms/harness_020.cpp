#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_020 {
// Fuzzing harness for Little CMS 2 library
// Target: IT8/CGATS patch and data format operations
// Strategy: Implement full IT8 lifecycle with patch name retrieval, data format enumeration,
//           and patch lookup operations targeting 37+ undiscovered branches in cmscgats.c
// Target APIs:
//   - cmsIT8GetPatchName (Target) - 9 undiscovered branches
//   - cmsIT8EnumDataFormat (Target) - 7 undiscovered branches
//   - cmsIT8GetPatchByName (Target) - 5 undiscovered branches
//   - cmsIT8GetPropertyMulti (Target) - 3 undiscovered branches
//   - cmsIT8DefineDblFormat (Target) - 6 undiscovered branches
//   - cmsIT8GetDataRowColDbl (Target) - 7 undiscovered branches
// Required Helper APIs:
//   - cmsIT8Alloc (Required Helper: Initialization)
//   - cmsIT8SetSheetType (Required Helper: Setup)
//   - cmsIT8SetDataFormat (Required Helper: Setup)
//   - cmsIT8SetPropertyStr/Dbl (Required Helper: Data population)
//   - cmsIT8Free (Required Helper: Cleanup)
// Invocation sequence:
//   1. Call cmsIT8Alloc to create a new IT8 handle
//   2. Call cmsIT8SetSheetType to set the sheet type
//   3. Call cmsIT8DefineDblFormat to define double formatting
//   4. Call cmsIT8SetDataFormat to set up data format columns
//   5. Call cmsIT8SetPropertyStr/Dbl to populate sample patch data
//   6. Call cmsIT8GetPatchName to retrieve patch names
//   7. Call cmsIT8EnumDataFormat to enumerate data formats
//   8. Call cmsIT8GetPatchByName to look up patches by name
//   9. Call cmsIT8GetPropertyMulti to retrieve multi-valued properties
//   10. Call cmsIT8GetDataRowColDbl to access numeric data
//   11. Call cmsIT8Free to clean up the IT8 handle
// Error handling scenarios included:
//   - Invalid patch indices and names
//   - Edge cases for property values
//   - Function return value checking
//   - Memory allocation failure handling
// Note: Uses FuzzedDataProvider for input-driven testing of all parameters
// Semantic diversity from harnesses 000-019: This harness focuses exclusively on IT8/CGATS
// operations, which are not covered in previous harnesses (all previous harnesses target
// profile operations, color transforms, deltaE calculations, etc.)


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for all parameters:
    // We need: context creation + sheet type string + format string + property strings + patch data
    const size_t MIN_SIZE = 64;
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 0: Create a context (optional for IT8 functions, but good practice)
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    
    // Step 1: Allocate IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 == NULL) {
        if (ctx) cmsDeleteContext(ctx);
        return 0;  // Allocation failed
    }
    
    // Step 2: Set sheet type from fuzzed input
    std::string sheet_type = fdp.ConsumeRandomLengthString(32);
    cmsIT8SetSheetType(hIT8, sheet_type.c_str());
    
    // Step 3: Define double format from fuzzed input
    std::string format_str = fdp.ConsumeRandomLengthString(16);
    cmsIT8DefineDblFormat(hIT8, format_str.c_str());
    
    // Step 4: Set up data format columns
    // Determine number of columns (1-8) from fuzzed input
    int num_columns = fdp.ConsumeIntegralInRange<int>(1, 8);
    
    // Column names for data format
    const char* column_names[] = {
        "SAMPLE_ID", "RGB_R", "RGB_G", "RGB_B", 
        "XYZ_X", "XYZ_Y", "XYZ_Z", "LAB_L"
    };
    
    for (int i = 0; i < num_columns && i < 8; i++) {
        cmsIT8SetDataFormat(hIT8, i, column_names[i]);
    }
    
    // Step 5: Set properties from fuzzed input
    // Set NUMBER_OF_FIELDS and NUMBER_OF_SETS properties
    int num_fields = num_columns;
    int num_sets = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    cmsIT8SetPropertyDbl(hIT8, "NUMBER_OF_FIELDS", num_fields);
    cmsIT8SetPropertyDbl(hIT8, "NUMBER_OF_SETS", num_sets);
    
    // Set additional properties
    std::string originator = fdp.ConsumeRandomLengthString(32);
    std::string manufacturer = fdp.ConsumeRandomLengthString(32);
    std::string descriptor = fdp.ConsumeRandomLengthString(32);
    
    cmsIT8SetPropertyStr(hIT8, "ORIGINATOR", originator.c_str());
    cmsIT8SetPropertyStr(hIT8, "MANUFACTURER", manufacturer.c_str());
    cmsIT8SetPropertyStr(hIT8, "DESCRIPTOR", descriptor.c_str());
    
    double created_val = fdp.ConsumeFloatingPoint<double>();
    double serial_val = fdp.ConsumeFloatingPoint<double>();
    
    cmsIT8SetPropertyDbl(hIT8, "CREATED", created_val);
    cmsIT8SetPropertyDbl(hIT8, "SERIAL", serial_val);
    
    // Set a multi-valued property
    std::string multi_key = fdp.ConsumeRandomLengthString(16);
    std::string multi_subkey = fdp.ConsumeRandomLengthString(16);
    std::string multi_value = fdp.ConsumeRandomLengthString(32);
    
    cmsIT8SetPropertyMulti(hIT8, multi_key.c_str(), multi_subkey.c_str(), multi_value.c_str());
    
    // Step 6: Populate sample patch data
    for (int row = 0; row < num_sets; row++) {
        // Create patch name
        char patch_name[32];
        snprintf(patch_name, sizeof(patch_name), "P%d", row);
        
        // Set patch name in first column
        cmsIT8SetDataRowCol(hIT8, row, 0, patch_name);
        
        // Set numeric data in other columns
        for (int col = 1; col < num_fields; col++) {
            double value = fdp.ConsumeFloatingPoint<double>();
            cmsIT8SetDataRowColDbl(hIT8, row, col, value);
        }
        
        // Break if we're running out of input
        if (fdp.remaining_bytes() < 100) break;
    }
    
    // Step 7: Test cmsIT8GetPatchName
    // Try to get patch names for valid and potentially invalid indices
    int max_patch_to_test = std::min(num_sets, 10);
    for (int patch_idx = 0; patch_idx < max_patch_to_test; patch_idx++) {
        char buffer[1024];
        const char* patch_name = cmsIT8GetPatchName(hIT8, patch_idx, buffer);
        // patch_name may be NULL for invalid indices, that's OK
    }
    
    // Also test with negative and out-of-bounds indices
    int test_indices[] = {-1, num_sets, num_sets + 1, 1000};
    for (int idx : test_indices) {
        char buffer[1024];
        cmsIT8GetPatchName(hIT8, idx, buffer);
    }
    
    // Step 8: Test cmsIT8EnumDataFormat
    char** sample_names = NULL;
    int num_formats = cmsIT8EnumDataFormat(hIT8, &sample_names);
    // num_formats should equal num_fields we set
    
    // Step 9: Test cmsIT8GetPatchByName
    // Try to find patches by name
    for (int patch_idx = 0; patch_idx < max_patch_to_test; patch_idx++) {
        char patch_name[32];
        snprintf(patch_name, sizeof(patch_name), "P%d", patch_idx);
        int found_idx = cmsIT8GetPatchByName(hIT8, patch_name);
        // found_idx should match patch_idx for valid patches
    }
    
    // Also test with non-existent patch names
    std::string non_existent1 = fdp.ConsumeRandomLengthString(16);
    std::string non_existent2 = fdp.ConsumeRandomLengthString(16);
    cmsIT8GetPatchByName(hIT8, non_existent1.c_str());
    cmsIT8GetPatchByName(hIT8, non_existent2.c_str());
    cmsIT8GetPatchByName(hIT8, "");  // Empty string
    
    // Step 10: Test cmsIT8GetPropertyMulti
    const char* multi_prop = cmsIT8GetPropertyMulti(hIT8, multi_key.c_str(), multi_subkey.c_str());
    // Also test with various key combinations
    std::string random_key = fdp.ConsumeRandomLengthString(16);
    std::string random_subkey = fdp.ConsumeRandomLengthString(16);
    cmsIT8GetPropertyMulti(hIT8, random_key.c_str(), random_subkey.c_str());
    cmsIT8GetPropertyMulti(hIT8, "ORIGINATOR", "");
    cmsIT8GetPropertyMulti(hIT8, "", "");
    
    // Step 11: Test cmsIT8GetDataRowColDbl
    // Access data at various positions
    for (int row = 0; row < std::min(num_sets, 5); row++) {
        for (int col = 0; col < std::min(num_fields, 5); col++) {
            double value = cmsIT8GetDataRowColDbl(hIT8, row, col);
            // Value may be 0.0 or other default for invalid access
        }
    }
    
    // Test edge cases
    cmsIT8GetDataRowColDbl(hIT8, -1, 0);
    cmsIT8GetDataRowColDbl(hIT8, 0, -1);
    cmsIT8GetDataRowColDbl(hIT8, num_sets, num_fields);
    cmsIT8GetDataRowColDbl(hIT8, 1000, 1000);
    
    // Step 12: Clean up
    cmsIT8Free(hIT8);
    if (ctx) cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_020
