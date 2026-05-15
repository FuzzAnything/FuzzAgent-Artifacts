// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateBCHSWabstractProfileTHR at cmsvirt.c:861:23 in lcms2.h
// cmsGetHeaderModel at cmsio0.c:1026:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_test_profile_from_data(const uint8_t *data, size_t size) {
    if (size < 128) {
        return cmsCreateNULLProfile();
    }
    
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    fwrite(data, 1, size > 4096 ? 4096 : size, f);
    fclose(f);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreateNULLProfile();
    }
    return profile;
}

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Extract parameters from fuzzer input
    const cmsUInt32Number *u32_data = (const cmsUInt32Number *)Data;
    const cmsFloat64Number *f64_data = (const cmsFloat64Number *)Data;
    
    size_t u32_count = Size / sizeof(cmsUInt32Number);
    size_t f64_count = Size / sizeof(cmsFloat64Number);
    
    // Test cmsCreateNULLProfile
    cmsHPROFILE null_profile = cmsCreateNULLProfile();
    if (null_profile) {
        cmsUInt32Number model = cmsGetHeaderModel(null_profile);
        (void)model;
        cmsCloseProfile(null_profile);
    }
    
    // Test cmsCreateBCHSWabstractProfile
    if (u32_count >= 1 && f64_count >= 4) {
        cmsUInt32Number nLUTPoints = u32_data[0] % 256 + 2;
        cmsFloat64Number Bright = f64_data[0];
        cmsFloat64Number Contrast = f64_data[1 % f64_count];
        cmsFloat64Number Hue = f64_data[2 % f64_count];
        cmsFloat64Number Saturation = f64_data[3 % f64_count];
        cmsUInt32Number TempSrc = u32_data[1 % u32_count];
        cmsUInt32Number TempDest = u32_data[2 % u32_count];
        
        cmsHPROFILE bchsw_profile = cmsCreateBCHSWabstractProfile(
            nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
        if (bchsw_profile) {
            cmsUInt32Number model = cmsGetHeaderModel(bchsw_profile);
            (void)model;
            cmsCloseProfile(bchsw_profile);
        }
    }
    
    // Test cmsCreateBCHSWabstractProfileTHR
    if (u32_count >= 1 && f64_count >= 4) {
        cmsContext context = create_test_context();
        if (context) {
            cmsUInt32Number nLUTPoints = u32_data[0] % 256 + 2;
            cmsFloat64Number Bright = f64_data[0];
            cmsFloat64Number Contrast = f64_data[1 % f64_count];
            cmsFloat64Number Hue = f64_data[2 % f64_count];
            cmsFloat64Number Saturation = f64_data[3 % f64_count];
            cmsUInt32Number TempSrc = u32_data[1 % u32_count];
            cmsUInt32Number TempDest = u32_data[2 % u32_count];
            
            cmsHPROFILE bchsw_thr_profile = cmsCreateBCHSWabstractProfileTHR(
                context, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
            if (bchsw_thr_profile) {
                cmsUInt32Number model = cmsGetHeaderModel(bchsw_thr_profile);
                (void)model;
                cmsCloseProfile(bchsw_thr_profile);
            }
            cmsDeleteContext(context);
        }
    }
    
    // Create test profiles for transform functions
    cmsHPROFILE input_profile = create_test_profile_from_data(Data, Size);
    cmsHPROFILE output_profile = create_test_profile_from_data(Data + Size/2, Size/2);
    
    // Test cmsCreateTransformTHR
    if (input_profile && u32_count >= 4) {
        cmsContext context = create_test_context();
        if (context) {
            cmsUInt32Number InputFormat = u32_data[0] % 16;
            cmsUInt32Number OutputFormat = u32_data[1 % u32_count] % 16;
            cmsUInt32Number Intent = u32_data[2 % u32_count] % 4;
            cmsUInt32Number dwFlags = u32_data[3 % u32_count];
            
            cmsHTRANSFORM transform = cmsCreateTransformTHR(
                context, input_profile, InputFormat, output_profile, 
                OutputFormat, Intent, dwFlags);
            if (transform) {
                cmsDeleteTransform(transform);
            }
            cmsDeleteContext(context);
        }
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    if (input_profile && output_profile && u32_count >= 4) {
        cmsContext context = create_test_context();
        if (context) {
            cmsHPROFILE profiles[3];
            profiles[0] = input_profile;
            profiles[1] = output_profile;
            profiles[2] = cmsCreateNULLProfile();
            
            cmsUInt32Number nProfiles = (u32_data[0] % 3) + 1;
            cmsUInt32Number InputFormat = u32_data[1 % u32_count] % 16;
            cmsUInt32Number OutputFormat = u32_data[2 % u32_count] % 16;
            cmsUInt32Number Intent = u32_data[3 % u32_count] % 4;
            cmsUInt32Number dwFlags = u32_data[0] & 0xFFFF;
            
            cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransformTHR(
                context, profiles, nProfiles, InputFormat, OutputFormat, Intent, dwFlags);
            if (multi_transform) {
                cmsDeleteTransform(multi_transform);
            }
            
            if (profiles[2]) cmsCloseProfile(profiles[2]);
            cmsDeleteContext(context);
        }
    }
    
    // Cleanup
    if (input_profile) cmsCloseProfile(input_profile);
    if (output_profile) cmsCloseProfile(output_profile);
    
    return 0;
}