#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include "lcms2.h"

namespace harness_015 {
/*
 * Fuzzing harness for LittleCMS2 library
 * Targets: Named color list management APIs in cmsnamed.c module
 * Semantic domain: Named color list management (untested in previous harnesses)
 * Previous harness semantic domains covered:
 *   000-basic profile parsing/transformation
 *   001-proofing/PostScript
 *   002-virtual profiles/gamut
 *   003-CGATS/IT8 basic
 *   004-CIECAM02 (appearance model, different from basic colorimetry)
 *   005-TAC detection/sampling
 *   006-device link creation/I/O
 *   007-BCHSW abstract profiles
 *   008-advanced CGATS/IT8
 *   009-cube file device link creation/stream I/O
 *   010-pipeline/MPE operations
 *   011-PostScript color resources
 *   012-MD5/half-precision
 *   013-context management/plugin system
 *   014-color science algorithms and metrics
 * This harness focuses on: Named color list allocation, appending, counting, indexing,
 *   info retrieval, and retrieval from transforms
 */


// Include Little CMS headers

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Minimum input size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create a context for the named color list operations
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Phase 1: Create and populate a named color list
    
    // Consume parameters for named color list creation
    cmsUInt32Number max_colors = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 100);
    cmsUInt32Number colorant_count = fdp.ConsumeIntegralInRange<cmsUInt32Number>(3, cmsMAXCHANNELS);
    
    // Consume prefix and suffix strings
    std::string prefix = fdp.ConsumeRandomLengthString(31);
    std::string suffix = fdp.ConsumeRandomLengthString(31);
    
    // Allocate named color list
    cmsNAMEDCOLORLIST* named_color_list = cmsAllocNamedColorList(
        context, max_colors, colorant_count, prefix.c_str(), suffix.c_str());
    
    if (named_color_list == NULL) {
        cmsDeleteContext(context);
        return 0;
    }
    
    // Phase 2: Append colors to the list
    
    // Determine how many colors to add (up to max_colors)
    cmsUInt32Number num_colors_to_add = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, max_colors);
    
    for (cmsUInt32Number i = 0; i < num_colors_to_add; i++) {
        // Check if we have enough data left
        if (fdp.remaining_bytes() < 100) {
            break;
        }
        
        // Consume color name
        std::string color_name = fdp.ConsumeRandomLengthString(63);
        
        // Consume PCS (Profile Connection Space) values (3 components)
        cmsUInt16Number pcs[3];
        pcs[0] = fdp.ConsumeIntegral<cmsUInt16Number>();
        pcs[1] = fdp.ConsumeIntegral<cmsUInt16Number>();
        pcs[2] = fdp.ConsumeIntegral<cmsUInt16Number>();
        
        // Consume colorant values (device-specific color components)
        std::vector<cmsUInt16Number> colorants(colorant_count);
        for (cmsUInt32Number j = 0; j < colorant_count; j++) {
            colorants[j] = fdp.ConsumeIntegral<cmsUInt16Number>();
        }
        
        // Append the named color to the list
        cmsBool append_result = cmsAppendNamedColor(
            named_color_list, color_name.c_str(), pcs, colorants.data());
        
        // The result is not critical for fuzzing - we continue regardless
        (void)append_result;
    }
    
    // Phase 3: Query information about the named color list
    
    // Get count of named colors
    cmsUInt32Number color_count = cmsNamedColorCount(named_color_list);
    
    // If we have colors in the list, test indexing and info retrieval
    if (color_count > 0) {
        // Test finding colors by name (for a few random colors)
        cmsUInt32Number num_searches = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 10);
        
        for (cmsUInt32Number i = 0; i < num_searches; i++) {
            if (fdp.remaining_bytes() < 10) {
                break;
            }
            
            // Consume a search name (might or might not exist in the list)
            std::string search_name = fdp.ConsumeRandomLengthString(63);
            
            // Search for the color by name
            cmsInt32Number color_index = cmsNamedColorIndex(named_color_list, search_name.c_str());
            
            // If found, retrieve detailed information about the color
            if (color_index >= 0 && color_index < (cmsInt32Number)color_count) {
                char retrieved_name[64];
                char retrieved_prefix[32];
                char retrieved_suffix[32];
                cmsUInt16Number retrieved_pcs[3];
                std::vector<cmsUInt16Number> retrieved_colorants(colorant_count);
                
                // Get color information
                cmsBool info_result = cmsNamedColorInfo(
                    named_color_list, color_index,
                    retrieved_name, retrieved_prefix, retrieved_suffix,
                    retrieved_pcs, retrieved_colorants.data());
                
                (void)info_result;
            }
        }
        
        // Test retrieving information for specific indices
        cmsUInt32Number num_info_queries = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, color_count);
        
        for (cmsUInt32Number i = 0; i < num_info_queries; i++) {
            if (fdp.remaining_bytes() < 10) {
                break;
            }
            
            // Pick a random color index to query
            cmsUInt32Number query_index = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, color_count - 1);
            
            char retrieved_name[64];
            char retrieved_prefix[32];
            char retrieved_suffix[32];
            cmsUInt16Number retrieved_pcs[3];
            std::vector<cmsUInt16Number> retrieved_colorants(colorant_count);
            
            // Get color information
            cmsBool info_result = cmsNamedColorInfo(
                named_color_list, query_index,
                retrieved_name, retrieved_prefix, retrieved_suffix,
                retrieved_pcs, retrieved_colorants.data());
            
            (void)info_result;
        }
    }
    
    // Phase 4: Test duplication of named color list
    
    // Duplicate the named color list
    cmsNAMEDCOLORLIST* duplicated_list = cmsDupNamedColorList(named_color_list);
    
    if (duplicated_list != NULL) {
        // Verify duplicated list has same count
        cmsUInt32Number dup_count = cmsNamedColorCount(duplicated_list);
        (void)dup_count;
        
        // Free the duplicated list
        cmsFreeNamedColorList(duplicated_list);
    }
    
    // Phase 5: Test named color list retrieval from a transform (requires profile creation)
    
    // Only attempt if we have enough data left
    if (fdp.remaining_bytes() > 500) {
        // Create a simple RGB profile to test cmsGetNamedColorList
        cmsHPROFILE rgb_profile = cmsCreate_sRGBProfile();
        
        if (rgb_profile != NULL) {
            // Create a transform using the profile
            cmsHTRANSFORM transform = cmsCreateTransform(
                rgb_profile, TYPE_RGB_8,
                rgb_profile, TYPE_RGB_8,
                INTENT_PERCEPTUAL, 0);
            
            if (transform != NULL) {
                // Try to get named color list from transform
                // Note: This may return NULL for standard profiles without named colors
                cmsNAMEDCOLORLIST* transform_named_list = cmsGetNamedColorList(transform);
                
                if (transform_named_list != NULL) {
                    // If a named color list is returned, we can test operations on it
                    cmsUInt32Number transform_color_count = cmsNamedColorCount(transform_named_list);
                    (void)transform_color_count;
                }
                
                // Clean up transform
                cmsDeleteTransform(transform);
            }
            
            // Clean up profile
            cmsCloseProfile(rgb_profile);
        }
    }
    
    // Phase 6: Cleanup
    
    // Free the named color list
    cmsFreeNamedColorList(named_color_list);
    
    // Delete the context
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_015
