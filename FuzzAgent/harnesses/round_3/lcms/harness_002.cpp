#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_002 {
// Fuzzing harness for LittleCMS (lcms) PostScript color resource generation
// This harness specifically targets PostScript color resource functions that are
// currently 0% covered according to coverage analysis (cmsps2.c module)
// Target APIs: cmsGetPostScriptCRD, cmsGetPostScriptCSA, cmsGetPostScriptColorResource


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful testing
    // Minimum: some profile data + parameters for intents/flags
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: first part for profile data, rest for fuzzing parameters
    // Reserve at least 32 bytes for parameters
    size_t max_profile_size = size - 32;
    if (max_profile_size < 32) {
        max_profile_size = 32;
    }
    
    size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(32, max_profile_size);
    std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(profile_size);
    
    // Consume parameters for PostScript functions
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
    cmsUInt32Number flags = 0;
    
    // Randomly set some flags from known flag values
    // Note: cmsFLAGS_NODEFAULTRESOURCEDEF is relevant for PostScript functions
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_NODEFAULTRESOURCEDEF;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_BLACKPOINTCOMPENSATION;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_HIGHRESPRECALC;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_LOWRESPRECALC;
    }
    if (fdp.ConsumeBool()) {
        flags |= cmsFLAGS_GRIDPOINTS(2); // 2^2 = 4 grid points
    }
    
    // Try to open the input as an ICC profile first
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data.data(), 
                                                 static_cast<cmsUInt32Number>(profile_data.size()));
    
    // If not a valid profile, fall back to creating a standard sRGB profile
    bool created_profile = false;
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfile();
        if (hProfile == NULL) {
            return 0; // Couldn't create any profile
        }
        created_profile = true;
    }
    
    // Get default context ID (0 for default context)
    cmsContext contextID = 0;
    
    // Test different buffer sizes for PostScript output
    // Consume buffer size from remaining input
    size_t buffer_size = 0;
    if (fdp.remaining_bytes() >= sizeof(size_t)) {
        buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, 65536); // Reasonable max
    }
    
    // Allocate buffer for PostScript output if size > 0
    std::vector<uint8_t> buffer;
    if (buffer_size > 0) {
        buffer.resize(buffer_size);
    }
    
    // TEST 1: cmsGetPostScriptCRD - Color Rendering Dictionary
    // This function generates PostScript CRD for the given profile
    cmsUInt32Number crd_bytes = cmsGetPostScriptCRD(contextID, hProfile, intent, flags,
                                                    buffer_size > 0 ? buffer.data() : NULL,
                                                    static_cast<cmsUInt32Number>(buffer_size));
    
    // TEST 2: cmsGetPostScriptCSA - Color Space Array  
    // This function generates PostScript CSA for the given profile
    cmsUInt32Number csa_bytes = cmsGetPostScriptCSA(contextID, hProfile, intent, flags,
                                                    buffer_size > 0 ? buffer.data() : NULL,
                                                    static_cast<cmsUInt32Number>(buffer_size));
    
    // TEST 3: cmsGetPostScriptColorResource - Unified function for both CRD and CSA
    // This requires an I/O handler, but we can test with NULL handler
    // or create a memory handler if we have buffer space
    
    // For cmsGetPostScriptColorResource, we need an I/O handler
    // Try to create one if we have buffer space
    if (buffer_size > 0) {
        // Create memory I/O handler
        cmsIOHANDLER* mem_io = cmsOpenIOhandlerFromMem(contextID, buffer.data(), 
                                                       static_cast<cmsUInt32Number>(buffer_size), "w");
        if (mem_io != NULL) {
            // Test with CRD type
            cmsUInt32Number crd_via_unified = cmsGetPostScriptColorResource(contextID,
                                                                           cmsPS_RESOURCE_CRD,
                                                                           hProfile,
                                                                           intent,
                                                                           flags,
                                                                           mem_io);
            
            // Close and reopen for CSA test
            cmsCloseIOhandler(mem_io);
            mem_io = cmsOpenIOhandlerFromMem(contextID, buffer.data(),
                                            static_cast<cmsUInt32Number>(buffer_size), "w");
            
            if (mem_io != NULL) {
                // Test with CSA type
                cmsUInt32Number csa_via_unified = cmsGetPostScriptColorResource(contextID,
                                                                               cmsPS_RESOURCE_CSA,
                                                                               hProfile,
                                                                               intent,
                                                                               flags,
                                                                               mem_io);
                cmsCloseIOhandler(mem_io);
            }
        }
    }
    
    // Also test with NULL buffer (just to get size)
    cmsUInt32Number crd_size_only = cmsGetPostScriptCRD(contextID, hProfile, intent, flags, NULL, 0);
    cmsUInt32Number csa_size_only = cmsGetPostScriptCSA(contextID, hProfile, intent, flags, NULL, 0);
    
    // Test with different intents using remaining input
    if (fdp.remaining_bytes() > 0) {
        cmsUInt32Number alt_intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
        
        // Quick test with alternative intent
        cmsUInt32Number alt_crd_bytes = cmsGetPostScriptCRD(contextID, hProfile, alt_intent, flags,
                                                           buffer_size > 0 ? buffer.data() : NULL,
                                                           static_cast<cmsUInt32Number>(buffer_size));
        
        cmsUInt32Number alt_csa_bytes = cmsGetPostScriptCSA(contextID, hProfile, alt_intent, flags,
                                                           buffer_size > 0 ? buffer.data() : NULL,
                                                           static_cast<cmsUInt32Number>(buffer_size));
    }
    
    // Test with different flag combinations if we have more data
    if (fdp.remaining_bytes() > sizeof(cmsUInt32Number)) {
        cmsUInt32Number alt_flags = fdp.ConsumeIntegral<cmsUInt32Number>();
        
        // Test with random flags (may include invalid combinations)
        cmsUInt32Number random_crd_bytes = cmsGetPostScriptCRD(contextID, hProfile, intent, alt_flags,
                                                              buffer_size > 0 ? buffer.data() : NULL,
                                                              static_cast<cmsUInt32Number>(buffer_size));
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}

} // namespace harness_002
