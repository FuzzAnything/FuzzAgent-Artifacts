#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_015 {
// Fuzzing harness for Little CMS 2 library
// Target: IT8/CGATS data format manipulation with 130+ undiscovered branches
// Coverage: cmscgats.c module has only 25.21% region coverage
// Target APIs: cmsIT8LoadFromMem (14 undiscovered branches), cmsIT8SaveToMem (16),
//              cmsIT8SetPropertyDbl (1), cmsIT8SetPropertyMulti (1), 
//              cmsIT8GetPropertyDbl (4), cmsIT8GetPropertyMulti (3),
//              cmsIT8SetData (20), cmsIT8GetData (9), 
//              cmsIT8SetDataDbl (24), cmsIT8GetDataDbl (9),
//              cmsIT8EnumProperties (11), cmsIT8EnumPropertyMulti (17)
// Required helpers: cmsCreateContext (69 undiscovered branches), cmsIT8Alloc (2),
//                   cmsIT8Free (4), cmsDeleteContext (10)
// Invocation sequence: 
//   1. cmsCreateContext
//   2. cmsIT8Alloc
//   3. cmsIT8LoadFromMem
//   4. cmsIT8SetPropertyStr/SetPropertyDbl/SetPropertyMulti
//   5. cmsIT8GetProperty/GetPropertyDbl/GetPropertyMulti
//   6. cmsIT8SetData/SetDataDbl
//   7. cmsIT8GetData/GetDataDbl
//   8. cmsIT8EnumProperties/EnumPropertyMulti
//   9. cmsIT8SaveToMem
//  10. cmsIT8Free
//  11. cmsDeleteContext
// Strategy: Test various IT8/CGATS operations using memory buffers to avoid file system dependencies
//           Generate fuzzed IT8 data format, load it, manipulate properties and data,
//           enumerate properties, and save back to memory


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for IT8 operations and property/data manipulation
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context for IT8 operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Step 2: Generate IT8 data from fuzzer input
    // We'll create a simple IT8 format with fuzzed content
    std::string it8_content;
    
    // Start with IT8 header
    it8_content = "CGATS.17\n";
    it8_content += "BEGIN_DATA_FORMAT\n";
    it8_content += "SAMPLE_ID RGB_R RGB_G RGB_B\n";
    it8_content += "END_DATA_FORMAT\n";
    
    // Determine number of data rows (1-50 for reasonable testing)
    uint32_t num_rows = fdp.ConsumeIntegralInRange<uint32_t>(1, 50);
    
    // Add NUMBER_OF_SETS property
    it8_content += "NUMBER_OF_SETS " + std::to_string(num_rows) + "\n";
    it8_content += "NUMBER_OF_FIELDS 4\n";
    it8_content += "ORIGINATOR \"Fuzzer\"\n";
    it8_content += "DESCRIPTOR \"Test IT8 Data\"\n";
    
    it8_content += "BEGIN_DATA\n";
    
    // Generate fuzzed data rows
    for (uint32_t i = 0; i < num_rows; i++) {
        std::string patch_name = "P" + std::to_string(i);
        
        // Consume RGB values from fuzzer input
        double r_val = fdp.ConsumeFloatingPointInRange<double>(0.0, 255.0);
        double g_val = fdp.ConsumeFloatingPointInRange<double>(0.0, 255.0);
        double b_val = fdp.ConsumeFloatingPointInRange<double>(0.0, 255.0);
        
        // Format row
        char row[256];
        snprintf(row, sizeof(row), "%s %.6f %.6f %.6f\n", 
                 patch_name.c_str(), r_val, g_val, b_val);
        it8_content += row;
        
        // Check if we have enough fuzzer input left
        if (fdp.remaining_bytes() < sizeof(double) * 3) {
            break;
        }
    }
    
    it8_content += "END_DATA\n";
    
    // Step 3: Load IT8 from memory
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(context, it8_content.c_str(), it8_content.size());
    if (hIT8 == NULL) {
        // If loading fails, try allocating fresh IT8 instead
        hIT8 = cmsIT8Alloc(context);
        if (hIT8 == NULL) {
            cmsDeleteContext(context);
            return 0;
        }
        
        // Set basic properties for new IT8
        cmsIT8SetSheetType(hIT8, "LCMS/TESTING");
        cmsIT8SetPropertyStr(hIT8, "ORIGINATOR", "Fuzzer");
        cmsIT8SetPropertyDbl(hIT8, "NUMBER_OF_SETS", 10);
        cmsIT8SetPropertyDbl(hIT8, "NUMBER_OF_FIELDS", 4);
        
        cmsIT8SetDataFormat(hIT8, 0, "SAMPLE_ID");
        cmsIT8SetDataFormat(hIT8, 1, "RGB_R");
        cmsIT8SetDataFormat(hIT8, 2, "RGB_G");
        cmsIT8SetDataFormat(hIT8, 3, "RGB_B");
        
        // Add some sample data
        for (int i = 0; i < 10; i++) {
            char patch[20];
            snprintf(patch, sizeof(patch), "P%d", i);
            cmsIT8SetData(hIT8, patch, "SAMPLE_ID", patch);
            cmsIT8SetDataDbl(hIT8, patch, "RGB_R", i * 10.0);
            cmsIT8SetDataDbl(hIT8, patch, "RGB_G", i * 15.0);
            cmsIT8SetDataDbl(hIT8, patch, "RGB_B", i * 20.0);
        }
    }
    
    // Step 4: Test property operations
    // Set various property types
    cmsIT8SetPropertyDbl(hIT8, "TEST_DOUBLE", fdp.ConsumeFloatingPoint<double>());
    
    // Set multi property if we have enough input
    if (fdp.remaining_bytes() > 20) {
        std::string key = fdp.ConsumeRandomLengthString(10);
        std::string subkey = fdp.ConsumeRandomLengthString(10);
        std::string value = fdp.ConsumeRandomLengthString(50);
        cmsIT8SetPropertyMulti(hIT8, key.c_str(), subkey.c_str(), value.c_str());
    }
    
    // Set string property
    if (fdp.remaining_bytes() > 10) {
        std::string prop_name = fdp.ConsumeRandomLengthString(8);
        std::string prop_value = fdp.ConsumeRandomLengthString(20);
        cmsIT8SetPropertyStr(hIT8, prop_name.c_str(), prop_value.c_str());
    }
    
    // Step 5: Get property operations
    // Get properties back
    double test_dbl = cmsIT8GetPropertyDbl(hIT8, "TEST_DOUBLE");
    (void)test_dbl; // Use to avoid unused variable warning
    
    // Try to get a property that might exist
    const char* desc = cmsIT8GetProperty(hIT8, "DESCRIPTOR");
    if (desc != NULL) {
        // Property exists, do nothing
    }
    
    // Step 6: Test data operations
    // Set and get data using patch names
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 20; i++) {
        char patch[20];
        snprintf(patch, sizeof(patch), "PATCH%d", i);
        
        // Set data as string
        std::string str_val = fdp.ConsumeRandomLengthString(15);
        cmsIT8SetData(hIT8, patch, "SAMPLE_ID", str_val.c_str());
        
        // Set data as double
        double dbl_val = fdp.ConsumeFloatingPoint<double>();
        cmsIT8SetDataDbl(hIT8, patch, "RGB_R", dbl_val);
        
        // Get data back
        const char* retrieved_str = cmsIT8GetData(hIT8, patch, "SAMPLE_ID");
        (void)retrieved_str;
        
        double retrieved_dbl = cmsIT8GetDataDbl(hIT8, patch, "RGB_R");
        (void)retrieved_dbl;
    }
    
    // Step 7: Enumerate properties
    char** property_names = NULL;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(hIT8, &property_names);
    if (property_names != NULL) {
        // Free the allocated property names array
        for (cmsUInt32Number i = 0; i < prop_count; i++) {
            // Property names are allocated by the library
        }
        // Free the array itself if the library allocates it
        // Note: The library documentation should specify who frees this
        // For safety, we don't free it as the library might manage it
    }
    
    // Step 8: Enumerate multi properties
    // Try to enumerate multi properties for a known property
    const char** subproperty_names = NULL;
    cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(hIT8, "TEST", &subproperty_names);
    if (subproperty_names != NULL) {
    }
    
    // Step 9: Save IT8 to memory
    cmsUInt32Number bytes_needed = 0;
    
    // First call to get required buffer size
    cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
    
    if (bytes_needed > 0 && bytes_needed < 10 * 1024 * 1024) { // Reasonable size limit
        std::vector<char> save_buffer(bytes_needed);
        cmsBool save_result = cmsIT8SaveToMem(hIT8, save_buffer.data(), &bytes_needed);
        (void)save_result; // Use to avoid unused variable warning
    }
    // Step 10: Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_015
