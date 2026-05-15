// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreateBCHSWabstractProfile(2, 1.0, 1.0, 0.0, 1.0, 6500, 5000);
    }
    
    const uint8_t* profile_data = data;
    size_t profile_size = size > 4096 ? 4096 : size;
    
    cmsHPROFILE profile = cmsOpenProfileFromMem(profile_data, (cmsUInt32Number)profile_size);
    if (profile == NULL) {
        profile = cmsCreate_sRGBProfile();
    }
    return profile;
}

static cmsContext create_dummy_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = NULL;
    cmsHPROFILE input_profile = NULL;
    cmsHPROFILE output_profile = NULL;
    cmsHPROFILE proofing_profile = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsHPROFILE abstract_profile = NULL;
    
    uint32_t seed = 0;
    memcpy(&seed, Data, sizeof(seed));
    size_t offset = sizeof(seed);
    
    context = create_dummy_context();
    
    input_profile = create_dummy_profile_from_data(Data + offset, Size - offset);
    offset = (offset + 64) % Size;
    
    output_profile = create_dummy_profile_from_data(Data + offset, Size - offset);
    offset = (offset + 64) % Size;
    
    proofing_profile = create_dummy_profile_from_data(Data + offset, Size - offset);
    offset = (offset + 64) % Size;
    
    cmsUInt32Number input_format = (seed >> 0) & 0xFF;
    cmsUInt32Number output_format = (seed >> 8) & 0xFF;
    cmsUInt32Number intent = (seed >> 16) & 0x03;
    cmsUInt32Number proofing_intent = (seed >> 18) & 0x03;
    cmsUInt32Number flags = (seed >> 20) & 0xFF;
    
    if (input_profile && output_profile && proofing_profile) {
        transform = cmsCreateProofingTransform(input_profile, input_format,
                                              output_profile, output_format,
                                              proofing_profile, intent,
                                              proofing_intent, flags);
        if (transform) {
            cmsDeleteTransform(transform);
            transform = NULL;
        }
    }
    
    if (input_profile && output_profile) {
        transform = cmsCreateTransformTHR(context, input_profile, input_format,
                                         output_profile, output_format,
                                         intent, flags);
        if (transform) {
            cmsDeleteTransform(transform);
            transform = NULL;
        }
        
        transform = cmsCreateTransform(input_profile, input_format,
                                      output_profile, output_format,
                                      intent, flags);
        if (transform) {
            cmsDeleteTransform(transform);
            transform = NULL;
        }
    }
    
    cmsUInt32Number nLUTPoints = 2 + ((seed >> 0) & 0x7);
    cmsFloat64Number bright = ((seed >> 3) & 0xFF) / 255.0 * 2.0 - 1.0;
    cmsFloat64Number contrast = 0.5 + ((seed >> 11) & 0xFF) / 255.0;
    cmsFloat64Number hue = ((seed >> 19) & 0xFF) / 255.0 * 360.0 - 180.0;
    cmsFloat64Number saturation = 0.5 + ((seed >> 27) & 0x7F) / 127.0;
    cmsUInt32Number temp_src = 3000 + ((seed >> 0) & 0x3FFF);
    cmsUInt32Number temp_dest = 3000 + ((seed >> 14) & 0x3FFF);
    
    abstract_profile = cmsCreateBCHSWabstractProfile(nLUTPoints, bright, contrast,
                                                    hue, saturation, temp_src, temp_dest);
    if (abstract_profile) {
        cmsCloseProfile(abstract_profile);
        abstract_profile = NULL;
    }
    
    if (context) {
        abstract_profile = cmsCreateBCHSWabstractProfileTHR(context, nLUTPoints, bright,
                                                           contrast, hue, saturation,
                                                           temp_src, temp_dest);
        if (abstract_profile) {
            cmsCloseProfile(abstract_profile);
            abstract_profile = NULL;
        }
    }
    
    if (Size >= offset + 8) {
        cmsFloat64Number adaptation;
        memcpy(&adaptation, Data + offset, sizeof(adaptation));
        adaptation = adaptation < 0.0 ? 0.0 : (adaptation > 1.0 ? 1.0 : adaptation);
        cmsSetAdaptationState(adaptation);
    }
    
    if (input_profile) cmsCloseProfile(input_profile);
    if (output_profile) cmsCloseProfile(output_profile);
    if (proofing_profile) cmsCloseProfile(proofing_profile);
    if (context) cmsDeleteContext(context);
    
    return 0;
}