#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

namespace harness_002 {
// Fuzzing harness for Little CMS (lcms) library
// Target: PostScript Color Rendering Dictionary (CRD) and Color Space Array (CSA) generation
// Focus: cmsps2.c module functions with highest undiscovered complexity (1104+ branches, 0% coverage)
// Semantically different from:
//   harness_000.cpp - profile parsing and basic transformations
//   harness_001.cpp - device link creation and virtual profiles


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for multiple parameters
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for different operations
    // 1. Consume fixed-size parameters first
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 15); // 0-3 base intents + K-preserve variants
    cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    cmsUInt32Number buffer_size = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1024, 65536);
    
    // 2. Create thread context for THR functions
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    // 3. Create source profile (sRGB)
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(ctx);
    if (hProfile == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // 4. Test both memory buffer and NULL handler approaches
    
    // Approach A: Memory buffer output
    void* buffer = malloc(buffer_size);
    if (buffer != NULL) {
        // Test cmsGetPostScriptCRD with memory buffer
        cmsUInt32Number crd_size = cmsGetPostScriptCRD(ctx, hProfile, intent, flags, buffer, buffer_size);
        // Note: crd_size may be 0 on error or return required buffer size
        
        // Test cmsGetPostScriptCSA with memory buffer  
        cmsUInt32Number csa_size = cmsGetPostScriptCSA(ctx, hProfile, intent, flags, buffer, buffer_size);
        
        free(buffer);
    }
    
    // Approach B: IO handler output (more flexible)
    // Create IO handler for memory output
    cmsUInt32Number io_buffer_size = fdp.ConsumeIntegralInRange<cmsUInt32Number>(512, 16384);
    std::vector<uint8_t> io_buffer = fdp.ConsumeBytes<uint8_t>(io_buffer_size);
    
    if (!io_buffer.empty()) {
        cmsIOHANDLER* io = cmsOpenIOhandlerFromMem(ctx, io_buffer.data(), io_buffer.size(), "w");
        if (io != NULL) {
            // Test unified color resource function with CRD type
            cmsUInt32Number crd_resource_size = cmsGetPostScriptColorResource(ctx, 
                                                                              cmsPS_RESOURCE_CRD,
                                                                              hProfile,
                                                                              intent,
                                                                              flags,
                                                                              io);
            
            // Rewind or recreate handler for CSA type
            // Note: Need to close and reopen since position is at end
            cmsCloseIOhandler(io);
            io = cmsOpenIOhandlerFromMem(ctx, io_buffer.data(), io_buffer.size(), "w");
            
            if (io != NULL) {
                cmsUInt32Number csa_resource_size = cmsGetPostScriptColorResource(ctx,
                                                                                  cmsPS_RESOURCE_CSA,
                                                                                  hProfile,
                                                                                  intent,
                                                                                  flags,
                                                                                  io);
                cmsCloseIOhandler(io);
            }
        } else {
            // Fallback to NULL handler if memory handler fails
            cmsIOHANDLER* null_io = cmsOpenIOhandlerFromNULL(ctx);
            if (null_io != NULL) {
                // Test both CRD and CSA with NULL handler
                cmsUInt32Number null_crd_size = cmsGetPostScriptColorResource(ctx,
                                                                             cmsPS_RESOURCE_CRD,
                                                                             hProfile,
                                                                             intent,
                                                                             flags,
                                                                             null_io);
                
                // Note: For NULL handler, position doesn't matter
                cmsUInt32Number null_csa_size = cmsGetPostScriptColorResource(ctx,
                                                                             cmsPS_RESOURCE_CSA,
                                                                             hProfile,
                                                                             intent,
                                                                             flags,
                                                                             null_io);
                
                cmsCloseIOhandler(null_io);
            }
        }
    }
    
    // 5. Test different flag combinations
    // Common flag combinations for PostScript generation
    cmsUInt32Number flag_variants[] = {
        0,  // No flags
        cmsFLAGS_NOCACHE,
        cmsFLAGS_NOOPTIMIZE,
        cmsFLAGS_NULLTRANSFORM,
        cmsFLAGS_GAMUTCHECK,
        cmsFLAGS_SOFTPROOFING,
        cmsFLAGS_BLACKPOINTCOMPENSATION,
        cmsFLAGS_NOWHITEONWHITEFIXUP,
        cmsFLAGS_HIGHRESPRECALC,
        cmsFLAGS_LOWRESPRECALC,
        cmsFLAGS_GRIDPOINTS(2),  // 2^2 = 4 grid points
        cmsFLAGS_GRIDPOINTS(4),  // 2^4 = 16 grid points
        cmsFLAGS_GRIDPOINTS(6)   // 2^6 = 64 grid points
    };
    
    // Test a subset of flag combinations based on fuzzer input
    uint8_t flag_choice = fdp.ConsumeIntegral<uint8_t>() % (sizeof(flag_variants) / sizeof(flag_variants[0]));
    cmsUInt32Number test_flags = flag_variants[flag_choice];
    
    // Create a small test buffer for flag testing
    cmsUInt32Number test_buffer_size = 4096;
    void* test_buffer = malloc(test_buffer_size);
    if (test_buffer != NULL) {
        // Test CRD with selected flags
        cmsUInt32Number test_crd_size = cmsGetPostScriptCRD(ctx, hProfile, intent, test_flags, test_buffer, test_buffer_size);
        
        // Test CSA with selected flags
        cmsUInt32Number test_csa_size = cmsGetPostScriptCSA(ctx, hProfile, intent, test_flags, test_buffer, test_buffer_size);
        
        free(test_buffer);
    }
    
    // 6. Test different intents (basic 4 + K-preserve variants)
    cmsUInt32Number intents_to_test[] = {
        INTENT_PERCEPTUAL,
        INTENT_RELATIVE_COLORIMETRIC,
        INTENT_SATURATION,
        INTENT_ABSOLUTE_COLORIMETRIC,
        INTENT_PRESERVE_K_ONLY_PERCEPTUAL,
        INTENT_PRESERVE_K_ONLY_RELATIVE_COLORIMETRIC,
        INTENT_PRESERVE_K_ONLY_SATURATION,
        INTENT_PRESERVE_K_PLANE_PERCEPTUAL,
        INTENT_PRESERVE_K_PLANE_RELATIVE_COLORIMETRIC,
        INTENT_PRESERVE_K_PLANE_SATURATION
    };
    
    // Test a random intent from the list
    uint8_t intent_choice = fdp.ConsumeIntegral<uint8_t>() % (sizeof(intents_to_test) / sizeof(intents_to_test[0]));
    cmsUInt32Number test_intent = intents_to_test[intent_choice];
    
    // Quick test with NULL handler for intent variation
    cmsIOHANDLER* intent_test_io = cmsOpenIOhandlerFromNULL(ctx);
    if (intent_test_io != NULL) {
        cmsUInt32Number intent_test_size = cmsGetPostScriptColorResource(ctx,
                                                                         cmsPS_RESOURCE_CRD,
                                                                         hProfile,
                                                                         test_intent,
                                                                         0,  // No flags
                                                                         intent_test_io);
        cmsCloseIOhandler(intent_test_io);
    }
    
    // 7. Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_002
