// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfileTHR(ctx);
}

static void read_xyz_from_data(const uint8_t** data, size_t* size, cmsCIEXYZ* xyz) {
    if (*size >= sizeof(cmsFloat64Number) * 3) {
        memcpy(&xyz->X, *data, sizeof(cmsFloat64Number));
        *data += sizeof(cmsFloat64Number);
        memcpy(&xyz->Y, *data, sizeof(cmsFloat64Number));
        *data += sizeof(cmsFloat64Number);
        memcpy(&xyz->Z, *data, sizeof(cmsFloat64Number));
        *data += sizeof(cmsFloat64Number);
        *size -= sizeof(cmsFloat64Number) * 3;
    } else {
        xyz->X = xyz->Y = xyz->Z = 0.5;
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    const uint8_t* ptr = Data;
    size_t remaining = Size;
    
    // Extract parameters from fuzz data
    cmsUInt32Number nProfiles = 1;
    cmsHPROFILE hProfiles[4] = {0};
    cmsBool BPC[4] = {0};
    cmsUInt32Number Intents[4] = {INTENT_PERCEPTUAL};
    cmsFloat64Number AdaptationStates[4] = {1.0};
    
    // Create some dummy profiles
    for (int i = 0; i < 2 && i < 4; i++) {
        hProfiles[i] = create_dummy_profile(ctx);
    }
    
    // Test cmsCreateExtendedTransform
    if (remaining >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number InputFormat = *(cmsUInt32Number*)ptr;
        ptr += sizeof(cmsUInt32Number);
        remaining -= sizeof(cmsUInt32Number);
        
        cmsUInt32Number OutputFormat = InputFormat;
        cmsUInt32Number dwFlags = 0;
        
        if (remaining >= sizeof(cmsUInt32Number)) {
            dwFlags = *(cmsUInt32Number*)ptr;
            ptr += sizeof(cmsUInt32Number);
            remaining -= sizeof(cmsUInt32Number);
        }
        
        cmsHTRANSFORM transform = cmsCreateExtendedTransform(ctx, nProfiles, hProfiles,
                                                             BPC, Intents, AdaptationStates,
                                                             NULL, 0, InputFormat, OutputFormat, dwFlags);
        if (transform) {
            cmsDeleteTransform(transform);
        }
    }
    
    // Test cmsAdaptToIlluminant
    if (remaining >= sizeof(cmsFloat64Number) * 9) {
        cmsCIEXYZ Result, SourceWhitePt, Illuminant, Value;
        read_xyz_from_data(&ptr, &remaining, &SourceWhitePt);
        read_xyz_from_data(&ptr, &remaining, &Illuminant);
        read_xyz_from_data(&ptr, &remaining, &Value);
        
        cmsAdaptToIlluminant(&Result, &SourceWhitePt, &Illuminant, &Value);
    }
    
    // Test cmsSetAdaptationStateTHR
    if (remaining >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number d = *(cmsFloat64Number*)ptr;
        ptr += sizeof(cmsFloat64Number);
        remaining -= sizeof(cmsFloat64Number);
        
        cmsSetAdaptationStateTHR(ctx, d);
    }
    
    // Test cmsCreateTransformTHR
    if (remaining >= sizeof(cmsUInt32Number) * 3) {
        cmsUInt32Number InputFormat = *(cmsUInt32Number*)ptr;
        ptr += sizeof(cmsUInt32Number);
        cmsUInt32Number OutputFormat = *(cmsUInt32Number*)ptr;
        ptr += sizeof(cmsUInt32Number);
        cmsUInt32Number Intent = *(cmsUInt32Number*)ptr;
        ptr += sizeof(cmsUInt32Number);
        remaining -= sizeof(cmsUInt32Number) * 3;
        
        cmsUInt32Number dwFlags = 0;
        if (remaining >= sizeof(cmsUInt32Number)) {
            dwFlags = *(cmsUInt32Number*)ptr;
            ptr += sizeof(cmsUInt32Number);
            remaining -= sizeof(cmsUInt32Number);
        }
        
        cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, hProfiles[0], InputFormat,
                                                       hProfiles[1] ? hProfiles[1] : NULL,
                                                       OutputFormat, Intent, dwFlags);
        if (transform) {
            cmsDeleteTransform(transform);
        }
    }
    
    // Test cmsCreateBCHSWabstractProfileTHR
    if (remaining >= sizeof(cmsUInt32Number) + sizeof(cmsFloat64Number) * 4 + sizeof(cmsUInt32Number) * 2) {
        cmsUInt32Number nLUTPoints = *(cmsUInt32Number*)ptr;
        ptr += sizeof(cmsUInt32Number);
        cmsFloat64Number Bright = *(cmsFloat64Number*)ptr;
        ptr += sizeof(cmsFloat64Number);
        cmsFloat64Number Contrast = *(cmsFloat64Number*)ptr;
        ptr += sizeof(cmsFloat64Number);
        cmsFloat64Number Hue = *(cmsFloat64Number*)ptr;
        ptr += sizeof(cmsFloat64Number);
        cmsFloat64Number Saturation = *(cmsFloat64Number*)ptr;
        ptr += sizeof(cmsFloat64Number);
        cmsUInt32Number TempSrc = *(cmsUInt32Number*)ptr;
        ptr += sizeof(cmsUInt32Number);
        cmsUInt32Number TempDest = *(cmsUInt32Number*)ptr;
        ptr += sizeof(cmsUInt32Number);
        remaining -= sizeof(cmsUInt32Number) + sizeof(cmsFloat64Number) * 4 + sizeof(cmsUInt32Number) * 2;
        
        cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast,
                                                              Hue, Saturation, TempSrc, TempDest);
        if (profile) {
            cmsCloseProfile(profile);
        }
    }
    
    // Test cmsSetAdaptationState
    if (remaining >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number d = *(cmsFloat64Number*)ptr;
        cmsSetAdaptationState(d);
    }
    
    // Cleanup
    for (int i = 0; i < 4; i++) {
        if (hProfiles[i]) {
            cmsCloseProfile(hProfiles[i]);
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}