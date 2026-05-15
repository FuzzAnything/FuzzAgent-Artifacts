// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    if (size < 28) {
        return cmsCreateBCHSWabstractProfile(2, 0.5, 0.5, 0.0, 1.0, 5000, 6500);
    }
    
    cmsUInt32Number nLUTPoints = *(cmsUInt32Number*)data;
    cmsFloat64Number Bright = *(cmsFloat64Number*)(data + 4);
    cmsFloat64Number Contrast = *(cmsFloat64Number*)(data + 12);
    cmsFloat64Number Hue = *(cmsFloat64Number*)(data + 20);
    cmsUInt32Number TempSrc = *(cmsUInt32Number*)(data + 28) % 15000;
    cmsUInt32Number TempDest = *(cmsUInt32Number*)(data + 32) % 15000;
    
    return cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, Hue, 1.0, TempSrc, TempDest);
}

static cmsHPROFILE create_test_profile_thr(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 28) {
        return cmsCreateBCHSWabstractProfileTHR(ctx, 2, 0.5, 0.5, 0.0, 1.0, 5000, 6500);
    }
    
    cmsUInt32Number nLUTPoints = *(cmsUInt32Number*)data;
    cmsFloat64Number Bright = *(cmsFloat64Number*)(data + 4);
    cmsFloat64Number Contrast = *(cmsFloat64Number*)(data + 12);
    cmsFloat64Number Hue = *(cmsFloat64Number*)(data + 20);
    cmsUInt32Number TempSrc = *(cmsUInt32Number*)(data + 28) % 15000;
    cmsUInt32Number TempDest = *(cmsUInt32Number*)(data + 32) % 15000;
    
    return cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast, Hue, 1.0, TempSrc, TempDest);
}

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    unsigned int selector = *(unsigned int*)Data % 6;
    const uint8_t* fuzz_data = Data + 4;
    size_t fuzz_size = Size - 4;
    
    switch (selector) {
        case 0: {
            // Test cmsCreateBCHSWabstractProfile
            if (fuzz_size < 36) break;
            
            cmsUInt32Number nLUTPoints = *(cmsUInt32Number*)fuzz_data;
            cmsFloat64Number Bright = *(cmsFloat64Number*)(fuzz_data + 4);
            cmsFloat64Number Contrast = *(cmsFloat64Number*)(fuzz_data + 12);
            cmsFloat64Number Hue = *(cmsFloat64Number*)(fuzz_data + 20);
            cmsFloat64Number Saturation = *(cmsFloat64Number*)(fuzz_data + 28);
            cmsUInt32Number TempSrc = *(cmsUInt32Number*)(fuzz_data + 36) % 15000;
            cmsUInt32Number TempDest = *(cmsUInt32Number*)(fuzz_data + 40) % 15000;
            
            cmsHPROFILE profile = cmsCreateBCHSWabstractProfile(
                nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
            if (profile) cmsCloseProfile(profile);
            break;
        }
        
        case 1: {
            // Test cmsCreateProofingTransform
            if (fuzz_size < 24) break;
            
            cmsHPROFILE input = create_test_profile(fuzz_data, fuzz_size);
            cmsHPROFILE output = create_test_profile(fuzz_data + 8, fuzz_size - 8);
            cmsHPROFILE proofing = create_test_profile(fuzz_data + 16, fuzz_size - 16);
            
            cmsUInt32Number InputFormat = *(cmsUInt32Number*)(fuzz_data + 24) % 10;
            cmsUInt32Number OutputFormat = *(cmsUInt32Number*)(fuzz_data + 28) % 10;
            cmsUInt32Number Intent = *(cmsUInt32Number*)(fuzz_data + 32) % 4;
            cmsUInt32Number ProofingIntent = *(cmsUInt32Number*)(fuzz_data + 36) % 4;
            cmsUInt32Number dwFlags = *(cmsUInt32Number*)(fuzz_data + 40);
            
            cmsHTRANSFORM transform = cmsCreateProofingTransform(
                input, InputFormat, output, OutputFormat, proofing, 
                Intent, ProofingIntent, dwFlags);
            
            if (transform) cmsDeleteTransform(transform);
            if (input) cmsCloseProfile(input);
            if (output) cmsCloseProfile(output);
            if (proofing) cmsCloseProfile(proofing);
            break;
        }
        
        case 2: {
            // Test cmsCreateTransform
            if (fuzz_size < 16) break;
            
            cmsHPROFILE input = create_test_profile(fuzz_data, fuzz_size);
            cmsHPROFILE output = create_test_profile(fuzz_data + 8, fuzz_size - 8);
            
            cmsUInt32Number InputFormat = *(cmsUInt32Number*)(fuzz_data + 16) % 10;
            cmsUInt32Number OutputFormat = *(cmsUInt32Number*)(fuzz_data + 20) % 10;
            cmsUInt32Number Intent = *(cmsUInt32Number*)(fuzz_data + 24) % 4;
            cmsUInt32Number dwFlags = *(cmsUInt32Number*)(fuzz_data + 28);
            
            cmsHTRANSFORM transform = cmsCreateTransform(
                input, InputFormat, output, OutputFormat, Intent, dwFlags);
            
            if (transform) cmsDeleteTransform(transform);
            if (input) cmsCloseProfile(input);
            if (output) cmsCloseProfile(output);
            break;
        }
        
        case 3: {
            // Test cmsCreateTransformTHR
            if (fuzz_size < 16) break;
            
            cmsContext ctx = cmsCreateContext(NULL, NULL);
            if (!ctx) break;
            
            cmsHPROFILE input = create_test_profile_thr(ctx, fuzz_data, fuzz_size);
            cmsHPROFILE output = create_test_profile_thr(ctx, fuzz_data + 8, fuzz_size - 8);
            
            cmsUInt32Number InputFormat = *(cmsUInt32Number*)(fuzz_data + 16) % 10;
            cmsUInt32Number OutputFormat = *(cmsUInt32Number*)(fuzz_data + 20) % 10;
            cmsUInt32Number Intent = *(cmsUInt32Number*)(fuzz_data + 24) % 4;
            cmsUInt32Number dwFlags = *(cmsUInt32Number*)(fuzz_data + 28);
            
            cmsHTRANSFORM transform = cmsCreateTransformTHR(
                ctx, input, InputFormat, output, OutputFormat, Intent, dwFlags);
            
            if (transform) cmsDeleteTransform(transform);
            if (input) cmsCloseProfile(input);
            if (output) cmsCloseProfile(output);
            cmsDeleteContext(ctx);
            break;
        }
        
        case 4: {
            // Test cmsCreateBCHSWabstractProfileTHR
            if (fuzz_size < 36) break;
            
            cmsContext ctx = cmsCreateContext(NULL, NULL);
            if (!ctx) break;
            
            cmsUInt32Number nLUTPoints = *(cmsUInt32Number*)fuzz_data;
            cmsFloat64Number Bright = *(cmsFloat64Number*)(fuzz_data + 4);
            cmsFloat64Number Contrast = *(cmsFloat64Number*)(fuzz_data + 12);
            cmsFloat64Number Hue = *(cmsFloat64Number*)(fuzz_data + 20);
            cmsFloat64Number Saturation = *(cmsFloat64Number*)(fuzz_data + 28);
            cmsUInt32Number TempSrc = *(cmsUInt32Number*)(fuzz_data + 36) % 15000;
            cmsUInt32Number TempDest = *(cmsUInt32Number*)(fuzz_data + 40) % 15000;
            
            cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(
                ctx, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
            
            if (profile) cmsCloseProfile(profile);
            cmsDeleteContext(ctx);
            break;
        }
        
        case 5: {
            // Test cmsSetAdaptationState
            if (fuzz_size < 8) break;
            
            cmsFloat64Number d = *(cmsFloat64Number*)fuzz_data;
            cmsFloat64Number prev = cmsSetAdaptationState(d);
            // Restore previous state
            cmsSetAdaptationState(prev);
            break;
        }
    }
    
    return 0;
}