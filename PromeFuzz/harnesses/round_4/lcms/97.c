// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetAdaptationState at cmsxform.c:77:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static cmsHPROFILE create_dummy_profile(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size > 1024 ? 1024 : size, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    remove("./dummy_file");
    
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    return profile;
}

static cmsContext create_dummy_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    uint32_t seed = 0;
    memcpy(&seed, Data, sizeof(seed) > Size ? Size : sizeof(seed));
    srand(seed);
    
    cmsContext context = create_dummy_context();
    cmsHPROFILE input_profile = create_dummy_profile(Data, Size);
    cmsHPROFILE output_profile = create_dummy_profile(Data + (Size/2), Size/2);
    cmsHPROFILE proofing_profile = create_dummy_profile(Data + (Size/3), Size/3);
    
    cmsUInt32Number input_format = (Size > 4) ? *((cmsUInt32Number*)Data) : TYPE_RGB_8;
    cmsUInt32Number output_format = (Size > 8) ? *((cmsUInt32Number*)(Data + 4)) : TYPE_CMYK_8;
    cmsUInt32Number intent = (Size > 12) ? *((cmsUInt32Number*)(Data + 8)) % 4 : INTENT_PERCEPTUAL;
    cmsUInt32Number proofing_intent = (Size > 16) ? *((cmsUInt32Number*)(Data + 12)) % 4 : INTENT_ABSOLUTE_COLORIMETRIC;
    cmsUInt32Number flags = (Size > 20) ? *((cmsUInt32Number*)(Data + 16)) : 0;
    cmsUInt32Number nLUTPoints = (Size > 24) ? *((cmsUInt32Number*)(Data + 20)) : 2;
    
    double brightness = (Size > 28) ? (double)(*((int32_t*)(Data + 24))) / 1000.0 : 1.0;
    double contrast = (Size > 32) ? (double)(*((int32_t*)(Data + 28))) / 1000.0 : 1.0;
    double hue = (Size > 36) ? (double)(*((int32_t*)(Data + 32))) / 1000.0 : 0.0;
    double saturation = (Size > 40) ? (double)(*((int32_t*)(Data + 36))) / 1000.0 : 1.0;
    cmsUInt32Number temp_src = (Size > 44) ? *((cmsUInt32Number*)(Data + 40)) : 6500;
    cmsUInt32Number temp_dest = (Size > 48) ? *((cmsUInt32Number*)(Data + 44)) : 5000;
    double adaptation = (Size > 52) ? (double)(*((int32_t*)(Data + 48))) / 1000.0 : 0.5;
    
    cmsHTRANSFORM transform1 = cmsCreateProofingTransform(
        input_profile, input_format,
        output_profile, output_format,
        proofing_profile,
        intent, proofing_intent,
        flags
    );
    if (transform1) cmsDeleteTransform(transform1);
    
    cmsHTRANSFORM transform2 = cmsCreateTransformTHR(
        context,
        input_profile, input_format,
        output_profile, output_format,
        intent, flags
    );
    if (transform2) cmsDeleteTransform(transform2);
    
    cmsHTRANSFORM transform3 = cmsCreateTransform(
        input_profile, input_format,
        output_profile, output_format,
        intent, flags
    );
    if (transform3) cmsDeleteTransform(transform3);
    
    cmsHPROFILE profile1 = cmsCreateBCHSWabstractProfile(
        nLUTPoints,
        brightness, contrast, hue, saturation,
        temp_src, temp_dest
    );
    if (profile1) cmsCloseProfile(profile1);
    
    cmsHPROFILE profile2 = cmsCreateBCHSWabstractProfileTHR(
        context,
        nLUTPoints,
        brightness, contrast, hue, saturation,
        temp_src, temp_dest
    );
    if (profile2) cmsCloseProfile(profile2);
    
    cmsSetAdaptationState(adaptation);
    
    if (input_profile) cmsCloseProfile(input_profile);
    if (output_profile) cmsCloseProfile(output_profile);
    if (proofing_profile) cmsCloseProfile(proofing_profile);
    if (context) cmsDeleteContext(context);
    
    return 0;
}