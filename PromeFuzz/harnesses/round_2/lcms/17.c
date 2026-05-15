// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return cmsCreate_sRGBProfile();
    
    fwrite(data, 1, size > 1024 ? 1024 : size, f);
    fclose(f);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = create_test_context();
    if (!context) return 0;
    
    cmsHPROFILE profiles[3];
    cmsUInt32Number intents[3];
    cmsBool bpc[3];
    cmsFloat64Number adaptation[3];
    
    for (int i = 0; i < 3; i++) {
        profiles[i] = create_dummy_profile(Data + (i * 10) % Size, Size / 3);
        
        // Safe access to Data with bounds checking
        if (i < Size) {
            intents[i] = Data[i] % 5;
        } else {
            intents[i] = 0;
        }
        
        if ((i + 1) < Size) {
            bpc[i] = Data[i + 1] & 1;
        } else {
            bpc[i] = 0;
        }
        
        if ((i + 2) < Size) {
            adaptation[i] = (double)(Data[i + 2]) / 255.0;
        } else {
            adaptation[i] = 0.0;
        }
    }
    
    cmsHPROFILE gamut_profile = create_dummy_profile(Data, Size);
    
    // Safe extraction of 32-bit values
    uint32_t flags = 0;
    if (Size >= 4) {
        flags = *((uint32_t*)Data);
    }
    
    uint32_t input_format = 0;
    uint32_t output_format = 0;
    if (Size >= 4) {
        input_format = *((uint32_t*)Data) & 0xFFFF;
        output_format = (*((uint32_t*)Data) >> 16) & 0xFFFF;
    }
    
    uint32_t n_profiles = (Size > 0) ? (Data[0] % 3 + 1) : 1;
    uint32_t intent = (Size > 1) ? (Data[1] % 5) : 0;
    uint32_t proofing_intent = (Size > 2) ? (Data[2] % 5) : 0;
    uint32_t gamut_pos = (Size > 3) ? (Data[3] % 3) : 0;
    
    cmsHTRANSFORM transform1 = cmsCreateExtendedTransform(
        context,
        n_profiles,
        profiles,
        bpc,
        intents,
        adaptation,
        gamut_profile,
        gamut_pos,
        input_format,
        output_format,
        flags
    );
    
    if (transform1) {
        cmsUInt32Number fmt = cmsGetTransformInputFormat(transform1);
        (void)fmt;
        cmsDeleteTransform(transform1);
    }
    
    cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransform(
        profiles,
        n_profiles,
        input_format,
        output_format,
        intent,
        flags
    );
    
    if (transform2) {
        cmsUInt32Number fmt = cmsGetTransformInputFormat(transform2);
        (void)fmt;
        cmsDeleteTransform(transform2);
    }
    
    cmsHTRANSFORM transform3 = cmsCreateMultiprofileTransformTHR(
        context,
        profiles,
        n_profiles,
        input_format,
        output_format,
        intent,
        flags
    );
    
    if (transform3) {
        cmsUInt32Number fmt = cmsGetTransformInputFormat(transform3);
        (void)fmt;
        cmsDeleteTransform(transform3);
    }
    
    cmsHTRANSFORM transform4 = cmsCreateTransformTHR(
        context,
        profiles[0],
        input_format,
        profiles[1],
        output_format,
        intent,
        flags
    );
    
    if (transform4) {
        cmsUInt32Number fmt = cmsGetTransformInputFormat(transform4);
        (void)fmt;
        cmsDeleteTransform(transform4);
    }
    
    cmsHTRANSFORM transform5 = cmsCreateProofingTransformTHR(
        context,
        profiles[0],
        input_format,
        profiles[1],
        output_format,
        profiles[2],
        intent,
        proofing_intent,
        flags
    );
    
    if (transform5) {
        cmsUInt32Number fmt = cmsGetTransformInputFormat(transform5);
        (void)fmt;
        cmsDeleteTransform(transform5);
    }
    
    for (int i = 0; i < 3; i++) {
        if (profiles[i]) cmsCloseProfile(profiles[i]);
    }
    
    if (gamut_profile) cmsCloseProfile(gamut_profile);
    
    if (context) cmsDeleteContext(context);
    
    return 0;
}