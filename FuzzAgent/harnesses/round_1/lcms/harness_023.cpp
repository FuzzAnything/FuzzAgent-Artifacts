#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_023 {
// Fuzzing harness for Little CMS 2 library
// Target: Named color list manipulation operations
// Strategy: Target cmsAppendNamedColor (0% coverage, 8 undiscovered branches),
//           cmsNamedColorIndex (0% coverage, 7 undiscovered branches),
//           cmsGetNamedColorList (0 hits, 8 undiscovered branches),
//           with required helpers: cmsAllocNamedColorList, cmsFreeNamedColorList,
//           cmsNamedColorInfo, cmsNamedColorCount.
// Implement two workflows:
//   1. Custom list creation: cmsAllocNamedColorList, multiple cmsAppendNamedColor calls,
//      cmsNamedColorIndex, cmsNamedColorInfo, cmsNamedColorCount, cmsFreeNamedColorList.
//   2. Profile-based extraction: cmsOpenProfileFromMem/File, cmsGetNamedColorList,
//      cmsNamedColorIndex, cmsNamedColorInfo, cmsFreeNamedColorList, cmsCloseProfile.
// Semantic diversity from existing harnesses (000-022): This harness exclusively targets
// named color manipulation APIs which are not covered in any previous harnesses.
// Previous harnesses focus on: profile operations, transforms, context management,
// intent discovery, etc. Named color operations represent a unique functional area.


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for operation selection and basic parameters
    // We need: operation mode + color count + colorant count + prefix/suffix length + at least one color name
    const size_t MIN_SIZE = sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t) + 2 + 1;
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Determine operation mode from fuzzer input
    // 0: Custom list creation workflow
    // 1: Profile-based extraction workflow (requires valid ICC profile data)
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Step 2: Consume common parameters for both workflows
    uint32_t max_colors = fdp.ConsumeIntegralInRange<uint32_t>(1, 16);  // Reasonable limit
    uint32_t colorant_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 16);  // Up to cmsMAXCHANNELS
    
    // Consume prefix and suffix strings
    std::string prefix = fdp.ConsumeRandomLengthString(16);
    std::string suffix = fdp.ConsumeRandomLengthString(16);
    
    if (operation_mode == 0) {
        // Workflow 1: Custom named color list creation and manipulation
        // ==============================================================
        
        // Create context for named color list operations
        cmsContext ctx = NULL;  // Use default context
        
        // Allocate named color list
        cmsNAMEDCOLORLIST* named_color_list = cmsAllocNamedColorList(
            ctx,  // Context ID
            max_colors,  // Initial allocation size
            colorant_count,  // Number of colorants per color
            prefix.c_str(),  // Prefix for color names
            suffix.c_str()   // Suffix for color names
        );
        
        if (named_color_list == NULL) {
            // Failed to allocate list, nothing more to do
            return 0;
        }
        
        // Append multiple named colors to the list
        uint32_t colors_to_add = fdp.ConsumeIntegralInRange<uint32_t>(1, max_colors);
        
        for (uint32_t i = 0; i < colors_to_add && fdp.remaining_bytes() > 0; i++) {
            // Consume color name
            std::string color_name = fdp.ConsumeRandomLengthString(32);
            
            // Consume PCS values (3 values: L, a, b in Lab color space)
            cmsUInt16Number pcs[3];
            for (int j = 0; j < 3 && fdp.remaining_bytes() > 0; j++) {
                pcs[j] = fdp.ConsumeIntegral<cmsUInt16Number>();
            }
            
            // Consume colorant values (device-specific colorant amounts)
            cmsUInt16Number colorants[16] = {0};  // cmsMAXCHANNELS is 16
            uint32_t colorants_to_use = (colorant_count > 16) ? 16 : colorant_count;
            for (uint32_t j = 0; j < colorants_to_use && fdp.remaining_bytes() > 0; j++) {
                colorants[j] = fdp.ConsumeIntegral<cmsUInt16Number>();
            }
            
            // Append the named color to the list
            cmsBool append_success = cmsAppendNamedColor(
                named_color_list,
                color_name.c_str(),
                pcs,
                colorants
            );
            
            // We don't check success/failure - both paths need to be explored
            (void)append_success;
        }
        
        // Query the list after adding colors
        cmsUInt32Number color_count = cmsNamedColorCount(named_color_list);
        
        // Try to find colors by name if we have any colors in the list
        if (color_count > 0 && fdp.remaining_bytes() > 0) {
            // Create a search name from fuzzer input
            std::string search_name = fdp.ConsumeRandomLengthString(32);
            
            // Find index of color by name
            cmsInt32Number color_index = cmsNamedColorIndex(named_color_list, search_name.c_str());
            
            // If we found a color, get its information
            if (color_index >= 0 && color_index < (cmsInt32Number)color_count) {
                char retrieved_name[256];
                char retrieved_prefix[256];
                char retrieved_suffix[256];
                cmsUInt16Number retrieved_pcs[3];
                cmsUInt16Number retrieved_colorants[16];
                
                cmsBool info_success = cmsNamedColorInfo(
                    named_color_list,
                    (cmsUInt32Number)color_index,
                    retrieved_name,
                    retrieved_prefix,
                    retrieved_suffix,
                    retrieved_pcs,
                    retrieved_colorants
                );
                
                (void)info_success;
            }
            
            // Also try to get information for random indices (including invalid ones)
            if (fdp.remaining_bytes() > 0) {
                uint32_t random_index = fdp.ConsumeIntegralInRange<uint32_t>(0, color_count + 10);
                
                char random_name[256];
                char random_prefix[256];
                char random_suffix[256];
                cmsUInt16Number random_pcs[3];
                cmsUInt16Number random_colorants[16];
                
                cmsBool random_info = cmsNamedColorInfo(
                    named_color_list,
                    random_index,
                    random_name,
                    random_prefix,
                    random_suffix,
                    random_pcs,
                    random_colorants
                );
                
                (void)random_info;
            }
        }
        
        // Free the named color list
        cmsFreeNamedColorList(named_color_list);
        
    } else {
        // Workflow 2: Profile-based named color extraction
        // ================================================
        
        // Check if we have enough data for a minimal ICC profile plus search operations
        // ICC profiles have a minimum size (usually 128 bytes for header + basic tags)
        // We also need some bytes for search name and operations
        if (fdp.remaining_bytes() < 140) {  // 128 for profile + 12 for search operations
            // Not enough data for meaningful testing
            return 0;
        }
        
        // Reserve some bytes for search operations before consuming profile data
        size_t bytes_for_search = fdp.ConsumeIntegralInRange<size_t>(12, 64);
        if (fdp.remaining_bytes() < 128) {
            // Not enough data left for even a minimal profile
            return 0;
        }
        
        // Consume search name from reserved bytes
        std::string search_name = fdp.ConsumeRandomLengthString(bytes_for_search);
        
        // Now consume remaining bytes as profile data
        std::vector<uint8_t> profile_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        // Try to open profile from memory
        cmsHPROFILE profile = cmsOpenProfileFromMem(
            profile_data.data(),
            profile_data.size()
        );
        
        if (profile == NULL) {
            // Profile opening failed, nothing more to do
            return 0;
        }
        // Create a transform from the profile to access named color list
        // cmsGetNamedColorList expects cmsHTRANSFORM, not cmsHPROFILE
        cmsHTRANSFORM transform = cmsCreateTransform(
            profile,           // Input profile
            TYPE_Lab_DBL,      // Input format - using Lab double as common format
            NULL,              // Output profile (NULL for no output)
            TYPE_Lab_DBL,      // Output format  
            INTENT_PERCEPTUAL, // Intent
            0                  // Flags
        );
        
        if (transform == NULL) {
            // Transform creation failed, clean up and return
            cmsCloseProfile(profile);
            return 0;
        }
        
        // Get named color list from the transform (if it has one)
        cmsNAMEDCOLORLIST* profile_named_colors = cmsGetNamedColorList(transform);
        
        if (profile_named_colors != NULL) {
            // Query the extracted named color list
            cmsUInt32Number profile_color_count = cmsNamedColorCount(profile_named_colors);
            
            // If we have colors, try to find and query them
            if (profile_color_count > 0) {
                // Find index of color by name (using fuzzer-provided search name)
                cmsInt32Number profile_color_index = cmsNamedColorIndex(
                    profile_named_colors, 
                    search_name.c_str()
                );
                
                // If we found a color, get its information
                if (profile_color_index >= 0 && 
                    profile_color_index < (cmsInt32Number)profile_color_count) {
                    
                    char profile_name[256];
                    char profile_prefix[256];
                    char profile_suffix[256];
                    cmsUInt16Number profile_pcs[3];
                    cmsUInt16Number profile_colorants[16];
                    
                    cmsBool profile_info = cmsNamedColorInfo(
                        profile_named_colors,
                        (cmsUInt32Number)profile_color_index,
                        profile_name,
                        profile_prefix,
                        profile_suffix,
                        profile_pcs,
                        profile_colorants
                    );
                    
                    (void)profile_info;
                }
                
                // Also try to get information for the first color (index 0)
                if (profile_color_count > 0) {
                    char first_name[256];
                    char first_prefix[256];
                    char first_suffix[256];
                    cmsUInt16Number first_pcs[3];
                    cmsUInt16Number first_colorants[16];
                    
                    cmsBool first_info = cmsNamedColorInfo(
                        profile_named_colors,
                        0,  // First color
                        first_name,
                        first_prefix,
                        first_suffix,
                        first_pcs,
                        first_colorants
                    );
                    
                    (void)first_info;
                }
            }
            
            // Free the named color list extracted from transform
            cmsFreeNamedColorList(profile_named_colors);
        }
        
        // Delete the transform
        cmsDeleteTransform(transform);
        
        // Close the profile
        cmsCloseProfile(profile);
    }
    
    return 0;
}

} // namespace harness_023
