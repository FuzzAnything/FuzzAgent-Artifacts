// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfileTHR(ctx);
}

static void parse_xyz(const uint8_t* data, size_t size, size_t* offset, cmsCIEXYZ* xyz) {
    if (*offset + sizeof(double) * 3 <= size) {
        memcpy(&xyz->X, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyz->Y, data + *offset, sizeof(double));
        *offset += sizeof(double);
        memcpy(&xyz->Z, data + *offset, sizeof(double));
        *offset += sizeof(double);
    } else {
        xyz->X = xyz->Y = xyz->Z = 0.5;
    }
}

static cmsFloat64Number parse_double(const uint8_t* data, size_t size, size_t* offset) {
    cmsFloat64Number val = 0.5;
    if (*offset + sizeof(double) <= size) {
        memcpy(&val, data + *offset, sizeof(double));
        *offset += sizeof(double);
    }
    return val;
}

static cmsUInt32Number parse_uint32(const uint8_t* data, size_t size, size_t* offset) {
    cmsUInt32Number val = 1;
    if (*offset + sizeof(cmsUInt32Number) <= size) {
        memcpy(&val, data + *offset, sizeof(cmsUInt32Number));
        *offset += sizeof(cmsUInt32Number);
    }
    return val;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 3) return 0;
    
    size_t offset = 0;
    cmsContext ctx = create_test_context();
    
    // Test cmsAdaptToIlluminant
    cmsCIEXYZ result, sourceWhitePt, illuminant, value;
    parse_xyz(Data, Size, &offset, &sourceWhitePt);
    parse_xyz(Data, Size, &offset, &illuminant);
    parse_xyz(Data, Size, &offset, &value);
    cmsAdaptToIlluminant(&result, &sourceWhitePt, &illuminant, &value);
    
    // Test cmsSetAdaptationStateTHR
    cmsFloat64Number adapt_state = parse_double(Data, Size, &offset);
    cmsSetAdaptationStateTHR(ctx, adapt_state);
    
    // Test cmsSetAdaptationState
    cmsSetAdaptationState(adapt_state);
    
    // Test cmsCreateBCHSWabstractProfileTHR
    cmsUInt32Number nLUTPoints = parse_uint32(Data, Size, &offset);
    if (nLUTPoints < 2) nLUTPoints = 2;
    cmsFloat64Number bright = parse_double(Data, Size, &offset);
    cmsFloat64Number contrast = parse_double(Data, Size, &offset);
    cmsFloat64Number hue = parse_double(Data, Size, &offset);
    cmsFloat64Number saturation = parse_double(Data, Size, &offset);
    cmsUInt32Number tempSrc = parse_uint32(Data, Size, &offset);
    cmsUInt32Number tempDest = parse_uint32(Data, Size, &offset);
    
    cmsHPROFILE bchsw_profile = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints,
                                                                 bright, contrast,
                                                                 hue, saturation,
                                                                 tempSrc, tempDest);
    
    // Test cmsCreateTransformTHR
    cmsHPROFILE dummy_profile = create_dummy_profile(ctx);
    cmsUInt32Number intent = parse_uint32(Data, Size, &offset) % 4;
    cmsUInt32Number flags = parse_uint32(Data, Size, &offset);
    
    if (dummy_profile) {
        cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, dummy_profile,
                                                       TYPE_RGB_8,
                                                       bchsw_profile ? bchsw_profile : dummy_profile,
                                                       TYPE_RGB_8,
                                                       intent,
                                                       flags);
        if (transform) {
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(dummy_profile);
    }
    
    // Test cmsCreateExtendedTransform
    if (bchsw_profile) {
        cmsHPROFILE profiles[2];
        profiles[0] = create_dummy_profile(ctx);
        profiles[1] = bchsw_profile;
        
        if (profiles[0]) {
            cmsBool bpc[] = {FALSE, FALSE};
            cmsUInt32Number intents[] = {intent, intent};
            cmsFloat64Number adapt_states[] = {adapt_state, adapt_state};
            
            cmsHTRANSFORM ext_transform = cmsCreateExtendedTransform(ctx, 2, profiles,
                                                                     bpc, intents, adapt_states,
                                                                     NULL, 0,
                                                                     TYPE_RGB_8, TYPE_RGB_8,
                                                                     flags);
            if (ext_transform) {
                cmsDeleteTransform(ext_transform);
            }
            cmsCloseProfile(profiles[0]);
        }
        cmsCloseProfile(bchsw_profile);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}