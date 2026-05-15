// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) cmsDeleteTransform(transform);
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) cmsCloseProfile(profile);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 64) return 0;
    
    // Create dummy file for potential profile loading
    create_dummy_file(Data, Size);
    
    // Parse parameters from fuzzer input
    size_t offset = 0;
    
    // Extract parameters for BCHSW profiles
    cmsUInt32Number nLUTPoints = *(cmsUInt32Number*)(Data + offset);
    offset += sizeof(cmsUInt32Number);
    
    cmsFloat64Number Bright = *(cmsFloat64Number*)(Data + offset);
    offset += sizeof(cmsFloat64Number);
    
    cmsFloat64Number Contrast = *(cmsFloat64Number*)(Data + offset);
    offset += sizeof(cmsFloat64Number);
    
    cmsFloat64Number Hue = *(cmsFloat64Number*)(Data + offset);
    offset += sizeof(cmsFloat64Number);
    
    cmsFloat64Number Saturation = *(cmsFloat64Number*)(Data + offset);
    offset += sizeof(cmsFloat64Number);
    
    cmsUInt32Number TempSrc = *(cmsUInt32Number*)(Data + offset);
    offset += sizeof(cmsUInt32Number);
    
    cmsUInt32Number TempDest = *(cmsUInt32Number*)(Data + offset);
    offset += sizeof(cmsUInt32Number);
    
    // Extract parameters for transforms
    cmsUInt32Number InputFormat = *(cmsUInt32Number*)(Data + offset);
    offset += sizeof(cmsUInt32Number);
    
    cmsUInt32Number OutputFormat = *(cmsUInt32Number*)(Data + offset);
    offset += sizeof(cmsUInt32Number);
    
    cmsUInt32Number Intent = *(cmsUInt32Number*)(Data + offset);
    offset += sizeof(cmsUInt32Number);
    
    cmsUInt32Number dwFlags = *(cmsUInt32Number*)(Data + offset);
    offset += sizeof(cmsUInt32Number);
    
    cmsUInt32Number ProofingIntent = *(cmsUInt32Number*)(Data + offset);
    offset += sizeof(cmsUInt32Number);
    
    // Extract adaptation state
    cmsFloat64Number adaptation = *(cmsFloat64Number*)(Data + offset);
    offset += sizeof(cmsFloat64Number);
    
    // Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    
    // Create BCHSW profiles
    cmsHPROFILE bchsw_profile_thr = NULL;
    cmsHPROFILE bchsw_profile = NULL;
    
    if (context) {
        bchsw_profile_thr = cmsCreateBCHSWabstractProfileTHR(
            context, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    }
    
    bchsw_profile = cmsCreateBCHSWabstractProfile(
        nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    
    // Set adaptation state
    cmsFloat64Number prev_adaptation = cmsSetAdaptationState(adaptation);
    
    // Create transforms using the BCHSW profiles
    cmsHTRANSFORM transform1 = NULL;
    cmsHTRANSFORM transform2 = NULL;
    cmsHTRANSFORM transform3 = NULL;
    
    if (bchsw_profile) {
        // Create regular transform
        transform1 = cmsCreateTransform(
            bchsw_profile, InputFormat, bchsw_profile, OutputFormat, Intent, dwFlags);
        
        // Create THR transform
        if (context) {
            transform2 = cmsCreateTransformTHR(
                context, bchsw_profile, InputFormat, bchsw_profile, OutputFormat, Intent, dwFlags);
        }
        
        // Create proofing transform (using same profile for all three roles)
        transform3 = cmsCreateProofingTransform(
            bchsw_profile, InputFormat, bchsw_profile, OutputFormat, bchsw_profile,
            Intent, ProofingIntent, dwFlags);
    }
    
    // Cleanup
    cleanup_transform(transform1);
    cleanup_transform(transform2);
    cleanup_transform(transform3);
    cleanup_profile(bchsw_profile_thr);
    cleanup_profile(bchsw_profile);
    
    if (context) cmsDeleteContext(context);
    
    // Restore adaptation state
    cmsSetAdaptationState(prev_adaptation);
    
    return 0;
}