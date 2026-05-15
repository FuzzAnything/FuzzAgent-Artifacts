#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "lcms2.h"

namespace harness_007 {
// Fuzzing harness for Little CMS (lcms) library
// Target: Named color profile APIs (cmsnamed.c module)
// Functions: cmsAllocNamedColorList, cmsFreeNamedColorList, cmsDupNamedColorList,
//            cmsAppendNamedColor, cmsNamedColorCount, cmsNamedColorIndex,
//            cmsNamedColorInfo, cmsGetNamedColorList
// Coverage goal: Target named color module which hasn't been explicitly targeted by previous harnesses
// Lifecycle sequence: Create context -> Allocate named color list -> Append colors -> 
//                     Query operations -> Duplicate list -> Transform integration -> Cleanup
// Semantically different from:
//   harness_000.cpp - profile parsing and basic transformations
//   harness_001.cpp - device link creation and virtual profiles
//   harness_002.cpp - PostScript generation
//   harness_003.cpp - CGATS parsing and manipulation
//   harness_004.cpp - Cube file device link creation
//   harness_005.cpp - CIECAM02 color appearance model
//   harness_006.cpp - Matrix operations and tone curves


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    // We need: context creation + basic list parameters + at least one color
    const size_t MIN_SIZE = sizeof(uint32_t) * 3 + 64; // For basic parameters and small strings
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) return 0;
    
    // Step 2: Consume parameters for named color list creation
    cmsUInt32Number initial_n = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 50);
    cmsUInt32Number colorant_count = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, cmsMAXCHANNELS);
    
    // Consume prefix and suffix strings (limited to reasonable lengths)
    std::string prefix = fdp.ConsumeRandomLengthString(32);
    std::string suffix = fdp.ConsumeRandomLengthString(32);
    
    // Step 3: Allocate named color list
    cmsNAMEDCOLORLIST* named_color_list = cmsAllocNamedColorList(context, initial_n, colorant_count, 
                                                                  prefix.c_str(), suffix.c_str());
    if (named_color_list == NULL) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Step 4: Append named colors to the list
    // Determine how many colors to append based on remaining input
    int colors_to_append = fdp.ConsumeIntegralInRange<int>(0, 20);
    
    for (int i = 0; i < colors_to_append && fdp.remaining_bytes() > 0; i++) {
        // Consume color name
        std::string color_name = fdp.ConsumeRandomLengthString(64);
        
        // Consume PCS values (3 components: Lab or XYZ)
        cmsUInt16Number pcs[3];
        for (int j = 0; j < 3 && fdp.remaining_bytes() >= sizeof(cmsUInt16Number); j++) {
            pcs[j] = fdp.ConsumeIntegral<cmsUInt16Number>();
        }
        
        // Consume colorant values (device-specific colorants)
        cmsUInt16Number colorants[cmsMAXCHANNELS];
        for (cmsUInt32Number j = 0; j < colorant_count && fdp.remaining_bytes() >= sizeof(cmsUInt16Number); j++) {
            colorants[j] = fdp.ConsumeIntegral<cmsUInt16Number>();
        }
        
        // Append the color to the list
        cmsAppendNamedColor(named_color_list, color_name.c_str(), pcs, colorants);
    }
    
    // Step 5: Test query operations on the named color list
    cmsUInt32Number color_count = cmsNamedColorCount(named_color_list);
    
    // Test cmsNamedColorInfo for some colors (if any exist)
    if (color_count > 0) {
        // Test info retrieval for a few colors
        int colors_to_query = fdp.ConsumeIntegralInRange<int>(0, color_count > 10 ? 10 : color_count);
        
        for (int i = 0; i < colors_to_query; i++) {
            // Choose which color to query
            cmsUInt32Number color_index = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, color_count - 1);
            
            char retrieved_name[65];
            char retrieved_prefix[33];
            char retrieved_suffix[33];
            cmsUInt16Number retrieved_pcs[3];
            cmsUInt16Number retrieved_colorants[cmsMAXCHANNELS];
            
            // Try with different combinations of NULL parameters
            uint8_t param_flags = fdp.ConsumeIntegral<uint8_t>();
            
            cmsNamedColorInfo(named_color_list, color_index,
                             (param_flags & 0x01) ? retrieved_name : NULL,
                             (param_flags & 0x02) ? retrieved_prefix : NULL,
                             (param_flags & 0x04) ? retrieved_suffix : NULL,
                             (param_flags & 0x08) ? retrieved_pcs : NULL,
                             (param_flags & 0x10) ? retrieved_colorants : NULL);
        }
    }
    
    // Step 6: Test cmsNamedColorIndex for search operations
    if (color_count > 0 && fdp.remaining_bytes() > 0) {
        // Try searching for existing color names
        int searches_to_perform = fdp.ConsumeIntegralInRange<int>(0, 5);
        
        for (int i = 0; i < searches_to_perform && fdp.remaining_bytes() > 0; i++) {
            // Create search string - sometimes existing, sometimes random
            uint8_t search_type = fdp.ConsumeIntegral<uint8_t>();
            std::string search_string;
            
            if ((search_type % 3) == 0 && color_count > 0) {
                // Try to search for an existing color (simulate by using first char of first color)
                // In real harness, we'd need actual color names - for simplicity, use fuzzed string
                search_string = fdp.ConsumeRandomLengthString(32);
            } else {
                // Random search string
                search_string = fdp.ConsumeRandomLengthString(32);
            }
            
            cmsNamedColorIndex(named_color_list, search_string.c_str());
        }
    }
    
    // Step 7: Test duplication of named color list
    cmsNAMEDCOLORLIST* duplicated_list = cmsDupNamedColorList(named_color_list);
    if (duplicated_list != NULL) {
        // Verify duplicate has same count
        cmsUInt32Number dup_count = cmsNamedColorCount(duplicated_list);
        
        // Test some operations on duplicated list
        if (dup_count > 0 && fdp.remaining_bytes() > 0) {
            // Append another color to the duplicated list
            std::string new_color_name = fdp.ConsumeRandomLengthString(32);
            cmsUInt16Number new_pcs[3] = {0, 0, 0};
            cmsUInt16Number new_colorants[cmsMAXCHANNELS] = {0};
            
            if (fdp.remaining_bytes() >= sizeof(cmsUInt16Number) * 3) {
                for (int j = 0; j < 3; j++) {
                    new_pcs[j] = fdp.ConsumeIntegral<cmsUInt16Number>();
                }
            }
            
            if (fdp.remaining_bytes() >= sizeof(cmsUInt16Number) * colorant_count) {
                for (cmsUInt32Number j = 0; j < colorant_count; j++) {
                    new_colorants[j] = fdp.ConsumeIntegral<cmsUInt16Number>();
                }
            }
            
            cmsAppendNamedColor(duplicated_list, new_color_name.c_str(), new_pcs, new_colorants);
        }
        
        // Free the duplicated list
        cmsFreeNamedColorList(duplicated_list);
    }
    
    // Step 8: Test integration with transforms (cmsGetNamedColorList)
    // This requires creating a profile with named color support
    // For simplicity, we'll create a simple transform and check if named color list can be retrieved
    if (fdp.remaining_bytes() > 100) {
        // Create simple profiles for testing
        cmsHPROFILE src_profile = cmsCreate_sRGBProfile();
        cmsHPROFILE dst_profile = cmsCreate_sRGBProfile();
        
        if (src_profile != NULL && dst_profile != NULL) {
            // Create a simple transform
            cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
            cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
            
            cmsHTRANSFORM transform = cmsCreateTransform(src_profile, 
                                                         TYPE_RGB_8, 
                                                         dst_profile, 
                                                         TYPE_RGB_8, 
                                                         intent, 
                                                         flags);
            
            if (transform != NULL) {
                // Try to get named color list from transform (may be NULL)
                cmsNAMEDCOLORLIST* transform_named_list = cmsGetNamedColorList(transform);
                // Note: transform_named_list will likely be NULL for sRGB profiles
                // but the function call itself exercises the code path
                
                cmsDeleteTransform(transform);
            }
            
            cmsCloseProfile(src_profile);
            cmsCloseProfile(dst_profile);
        }
    }
    
    // Step 9: Cleanup
    cmsFreeNamedColorList(named_color_list);
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_007
