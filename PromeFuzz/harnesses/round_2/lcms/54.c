// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAdaptationStateTHR at cmsxform.c:57:28 in lcms2.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static double extract_double(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(double) > size) {
        *offset = size;
        return 0.0;
    }
    double value;
    memcpy(&value, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return value;
}

static uint32_t extract_uint32(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + sizeof(uint32_t) > size) {
        *offset = size;
        return 0;
    }
    uint32_t value;
    memcpy(&value, data + *offset, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    return value;
}

static void fill_xyz(cmsCIEXYZ* xyz, const uint8_t* data, size_t size, size_t* offset) {
    xyz->X = extract_double(data, size, offset);
    xyz->Y = extract_double(data, size, offset);
    xyz->Z = extract_double(data, size, offset);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    size_t offset = 0;
    
    // Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // 1. Test cmsSetAdaptationStateTHR
    double adapt_state = extract_double(Data, Size, &offset);
    cmsSetAdaptationStateTHR(context, adapt_state);
    
    // 2. Test cmsAdaptToIlluminant
    cmsCIEXYZ result, source_white, illuminant, value;
    fill_xyz(&source_white, Data, Size, &offset);
    fill_xyz(&illuminant, Data, Size, &offset);
    fill_xyz(&value, Data, Size, &offset);
    cmsAdaptToIlluminant(&result, &source_white, &illuminant, &value);
    
    // 3. Test cmsCreateBCHSWabstractProfileTHR
    uint32_t nLUTPoints = extract_uint32(Data, Size, &offset);
    double bright = extract_double(Data, Size, &offset);
    double contrast = extract_double(Data, Size, &offset);
    double hue = extract_double(Data, Size, &offset);
    double saturation = extract_double(Data, Size, &offset);
    uint32_t temp_src = extract_uint32(Data, Size, &offset);
    uint32_t temp_dest = extract_uint32(Data, Size, &offset);
    
    cmsHPROFILE bchsw_profile = cmsCreateBCHSWabstractProfileTHR(
        context, nLUTPoints, bright, contrast, hue, saturation, temp_src, temp_dest);
    if (bchsw_profile) cmsCloseProfile(bchsw_profile);
    
    // 4. Test cmsSetAdaptationState
    double adapt_state2 = extract_double(Data, Size, &offset);
    cmsSetAdaptationState(adapt_state2);
    
    // 5. Test cmsCreateExtendedTransform
    // Create some dummy profiles first
    cmsHPROFILE dummy_profiles[2];
    dummy_profiles[0] = cmsCreateBCHSWabstractProfileTHR(context, 2, 0.5, 0.5, 0.0, 1.0, 6500, 5000);
    dummy_profiles[1] = cmsCreateBCHSWabstractProfileTHR(context, 2, 0.5, 0.5, 0.0, 1.0, 5000, 6500);
    
    if (dummy_profiles[0] && dummy_profiles[1]) {
        uint32_t nProfiles = 2;
        cmsBool bpc[] = {0, 0};
        uint32_t intents[] = {INTENT_PERCEPTUAL, INTENT_PERCEPTUAL};
        double adapt_states[] = {0.5, 0.5};
        uint32_t input_format = TYPE_RGB_8;
        uint32_t output_format = TYPE_RGB_8;
        uint32_t flags = 0;
        
        cmsHTRANSFORM ext_transform = cmsCreateExtendedTransform(
            context, nProfiles, dummy_profiles, bpc, intents, adapt_states,
            NULL, 0, input_format, output_format, flags);
        if (ext_transform) cmsDeleteTransform(ext_transform);
    }
    
    if (dummy_profiles[0]) cmsCloseProfile(dummy_profiles[0]);
    if (dummy_profiles[1]) cmsCloseProfile(dummy_profiles[1]);
    
    // 6. Test cmsCreateTransformTHR
    cmsHPROFILE input_profile = cmsCreateBCHSWabstractProfileTHR(context, 2, 0.5, 0.5, 0.0, 1.0, 6500, 5000);
    cmsHPROFILE output_profile = cmsCreateBCHSWabstractProfileTHR(context, 2, 0.5, 0.5, 0.0, 1.0, 5000, 6500);
    
    if (input_profile) {
        uint32_t input_format = TYPE_RGB_8;
        uint32_t output_format = TYPE_RGB_8;
        uint32_t intent = INTENT_PERCEPTUAL;
        uint32_t flags = 0;
        
        cmsHTRANSFORM transform = cmsCreateTransformTHR(
            context, input_profile, input_format, output_profile, 
            output_format, intent, flags);
        if (transform) cmsDeleteTransform(transform);
        
        // Test with output as NULL (device link)
        cmsHTRANSFORM device_link = cmsCreateTransformTHR(
            context, input_profile, input_format, NULL, 
            output_format, intent, flags);
        if (device_link) cmsDeleteTransform(device_link);
    }
    
    if (input_profile) cmsCloseProfile(input_profile);
    if (output_profile) cmsCloseProfile(output_profile);
    
    // Cleanup
    cmsDeleteContext(context);
    
    return 0;
}