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

namespace harness_010 {
// Fuzzing harness for LittleCMS (lcms) IT8/CGATS file manipulation functions
// This harness specifically targets IT8/CGATS file format handling functions from cmscgats.c module
// that are currently 0% covered according to coverage analysis
// Target APIs: cmsIT8LoadFromFile, cmsIT8SaveToFile, cmsIT8SetTableByLabel, cmsIT8EnumPropertyMulti,
// cmsIT8EnumProperties, cmsIT8GetPatchName, cmsIT8SetIndexColumn
// MODULE: cmscgats.c [current coverage: low]
// STRATEGY: Target comprehensive IT8/CGATS file format manipulation workflow
// DIFFERENTIATION: Unlike existing harnesses focused on transforms, PostScript, virtual profiles, I/O operations, dictionary management, CIECAM02, MD5, and tone curves, target specialized IT8/CGATS file format handling
// API SEQUENCE: Create context -> Create IT8 handle -> Load IT8 file from disk -> Enumerate properties -> Modify tables -> Configure columns -> Save modified file -> Cleanup


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing of IT8 file operations
    // Minimum: context + IT8 file data + property operations + table modifications
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create a temporary IT8 file with fuzzer-provided content
    char temp_filename[] = "/tmp/lcms_it8_fuzz_XXXXXX";
    int temp_fd = mkstemp(temp_filename);
    if (temp_fd == -1) {
        return 0; // Cannot create temporary file
    }
    
    // Write IT8 file content based on fuzzer input
    // Use a simple IT8 format similar to what's in testcms2.c
    FILE* temp_file = fdopen(temp_fd, "w");
    if (temp_file == NULL) {
        close(temp_fd);
        return 0;
    }
    
    // Write IT8 header
    fprintf(temp_file, "BEGIN_DATA_FORMAT\n");
    fprintf(temp_file, "SAMPLE_ID\tRGB_R\tRGB_G\tRGB_B\n");
    fprintf(temp_file, "END_DATA_FORMAT\n");
    
    // Write IT8 data section header
    fprintf(temp_file, "BEGIN_DATA\n");
    
    // Write some data rows using fuzzer-provided values
    // Consume parameters for number of rows
    int num_rows = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < num_rows; i++) {
        if (fdp.remaining_bytes() < 16) break;
        
        // Generate patch name
        std::string patch_name = "P" + std::to_string(i);
        
        // Consume RGB values
        double r = fdp.ConsumeFloatingPointInRange<double>(0.0, 255.0);
        double g = fdp.ConsumeFloatingPointInRange<double>(0.0, 255.0);
        double b = fdp.ConsumeFloatingPointInRange<double>(0.0, 255.0);
        
        fprintf(temp_file, "%s\t%.6g\t%.6g\t%.6g\n", 
                patch_name.c_str(), r, g, b);
    }
    
    fprintf(temp_file, "END_DATA\n");
    fclose(temp_file);
    
    // 2. Create a context for thread-safe operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        remove(temp_filename);
        return 0; // Cannot proceed without context
    }
    
    // 3. Load IT8 file from disk (Primary Target: cmsIT8LoadFromFile)
    cmsHANDLE it8_handle = cmsIT8LoadFromFile(context, temp_filename);
    if (it8_handle == NULL) {
        cmsDeleteContext(context);
        remove(temp_filename);
        return 0; // Cannot load IT8 file
    }
    
    // Set sheet type for the loaded IT8 handle
    cmsIT8SetSheetType(it8_handle, "LCMS/FUZZ");
    
    // 4. Enumerate properties (Secondary Target: cmsIT8EnumProperties)
    char** property_names = NULL;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(it8_handle, &property_names);
    
    // Try to access some properties if they exist
    if (prop_count > 0 && property_names != NULL) {
        // Just accessing the property names is enough for coverage
        // We don't need to use them further
    }
    
    // 5. Enumerate property multi (Secondary Target: cmsIT8EnumPropertyMulti)
    // Need a property name to enumerate subproperties
    // Use a known property name or one from the list if available
    if (prop_count > 0 && property_names != NULL) {
        const char* test_prop = property_names[0];
        const char** subprop_names = NULL;
        cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(it8_handle, test_prop, &subprop_names);
        
        // Access result for coverage
        (void)subprop_count;
    }
    
    // 6. Get patch names (Secondary Target: cmsIT8GetPatchName)
    // Try to get patch names for first few patches
    for (int i = 0; i < 5; i++) {
        char buffer[256];
        const char* patch_name = cmsIT8GetPatchName(it8_handle, i, buffer);
        // patch_name could be NULL, that's OK for coverage
        (void)patch_name;
    }
    
    // 7. Set table by label (Primary Target: cmsIT8SetTableByLabel)
    // Consume strings for table manipulation
    if (fdp.remaining_bytes() >= 32) {
        std::string set_name = fdp.ConsumeRandomLengthString(16);
        std::string field_name = fdp.ConsumeRandomLengthString(16);
        
        // Try to set table by label - may fail if labels don't exist
        int table_result = cmsIT8SetTableByLabel(it8_handle, set_name.c_str(), 
                                                 field_name.c_str(), "DATA");
        // Result can be negative (error) or positive (success), both are fine for coverage
        (void)table_result;
    }
    
    // 8. Set index column (Secondary Target: cmsIT8SetIndexColumn)
    if (fdp.remaining_bytes() >= 16) {
        std::string sample_name = fdp.ConsumeRandomLengthString(16);
        cmsBool index_result = cmsIT8SetIndexColumn(it8_handle, sample_name.c_str());
        // Result can be TRUE or FALSE, both are fine for coverage
        (void)index_result;
    }
    
    // 9. Save modified file to a different location (Primary Target: cmsIT8SaveToFile)
    char save_filename[] = "/tmp/lcms_it8_save_XXXXXX";
    int save_fd = mkstemp(save_filename);
    if (save_fd != -1) {
        close(save_fd); // Close since cmsIT8SaveToFile will open with fopen
        
        cmsBool save_result = cmsIT8SaveToFile(it8_handle, save_filename);
        // save_result can be TRUE or FALSE, both are fine for coverage
        (void)save_result;
        
        // Clean up saved file
        remove(save_filename);
    }
    
    // 10. Cleanup
    cmsIT8Free(it8_handle);
    cmsDeleteContext(context);
    
    // Clean up temporary files
    remove(temp_filename);
    
    return 0;
}

} // namespace harness_010
