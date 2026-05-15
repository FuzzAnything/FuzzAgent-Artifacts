// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_transform(cmsHTRANSFORM hTransform) {
    if (hTransform) {
        cmsDeleteTransform(hTransform);
    }
}

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfileTHR(ctx);
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return cmsCreate_sRGBProfileTHR(ctx);
    }
    
    fwrite(data, 1, size > 4096 ? 4096 : size, fp);
    fclose(fp);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        hProfile = cmsCreate_sRGBProfileTHR(ctx);
    }
    
    remove("./dummy_file");
    return hProfile;
}

int LLVMFuzzerTestOneInput_99(const uint8_t* Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }
    
    // Create some dummy profiles for testing
    cmsHPROFILE dummy_profiles[4];
    for (int i = 0; i < 4; i++) {
        dummy_profiles[i] = create_dummy_profile(ctx, Data, Size);
    }
    
    // Test cmsCreateExtendedTransform
    {
        cmsUInt32Number nProfiles = (Data[0] % 3) + 1;
        cmsBool BPC[4] = {0, 0, 0, 0};
        cmsUInt32Number Intents[4] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC, 
                                      INTENT_SATURATION, INTENT_ABSOLUTE_COLORIMETRIC};
        cmsFloat64Number AdaptationStates[4] = {1.0, 1.0, 1.0, 1.0};
        cmsUInt32Number InputFormat = TYPE_RGB_8;
        cmsUInt32Number OutputFormat = TYPE_RGB_8;
        cmsUInt32Number dwFlags = cmsFLAGS_NOCACHE;
        
        if (Size > 1) {
            dwFlags = Data[1] % 256;
        }
        
        cmsHTRANSFORM hTransform = cmsCreateExtendedTransform(
            ctx,
            nProfiles,
            dummy_profiles,
            BPC,
            Intents,
            AdaptationStates,
            NULL,
            0,
            InputFormat,
            OutputFormat,
            dwFlags
        );
        
        cleanup_transform(hTransform);
    }
    
    // Test cmsCreateProofingTransformTHR
    {
        cmsUInt32Number InputFormat = TYPE_RGB_8;
        cmsUInt32Number OutputFormat = TYPE_RGB_8;
        cmsUInt32Number Intent = INTENT_PERCEPTUAL;
        cmsUInt32Number ProofingIntent = INTENT_RELATIVE_COLORIMETRIC;
        cmsUInt32Number dwFlags = cmsFLAGS_SOFTPROOFING;
        
        if (Size > 2) {
            Intent = Data[2] % 4;
            ProofingIntent = Data[2] % 4;
        }
        if (Size > 3) {
            dwFlags = Data[3] % 256;
        }
        
        cmsHTRANSFORM hTransform = cmsCreateProofingTransformTHR(
            ctx,
            dummy_profiles[0],
            InputFormat,
            dummy_profiles[1],
            OutputFormat,
            dummy_profiles[2],
            Intent,
            ProofingIntent,
            dwFlags
        );
        
        cleanup_transform(hTransform);
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    {
        cmsUInt32Number nProfiles = (Data[0] % 3) + 1;
        cmsUInt32Number InputFormat = TYPE_RGB_8;
        cmsUInt32Number OutputFormat = TYPE_RGB_8;
        cmsUInt32Number Intent = INTENT_PERCEPTUAL;
        cmsUInt32Number dwFlags = cmsFLAGS_NOCACHE;
        
        if (Size > 4) {
            Intent = Data[4] % 4;
            dwFlags = Data[4] % 256;
        }
        
        cmsHTRANSFORM hTransform = cmsCreateMultiprofileTransformTHR(
            ctx,
            dummy_profiles,
            nProfiles,
            InputFormat,
            OutputFormat,
            Intent,
            dwFlags
        );
        
        cleanup_transform(hTransform);
    }
    
    // Test cmsCreateMultiprofileTransform
    {
        cmsUInt32Number nProfiles = (Data[0] % 3) + 1;
        cmsUInt32Number InputFormat = TYPE_RGB_8;
        cmsUInt32Number OutputFormat = TYPE_RGB_8;
        cmsUInt32Number Intent = INTENT_PERCEPTUAL;
        cmsUInt32Number dwFlags = cmsFLAGS_NOCACHE;
        
        if (Size > 5) {
            Intent = Data[5] % 4;
            dwFlags = Data[5] % 256;
        }
        
        cmsHTRANSFORM hTransform = cmsCreateMultiprofileTransform(
            dummy_profiles,
            nProfiles,
            InputFormat,
            OutputFormat,
            Intent,
            dwFlags
        );
        
        cleanup_transform(hTransform);
    }
    
    // Test cmsCreateProofingTransform
    {
        cmsUInt32Number InputFormat = TYPE_RGB_8;
        cmsUInt32Number OutputFormat = TYPE_RGB_8;
        cmsUInt32Number Intent = INTENT_PERCEPTUAL;
        cmsUInt32Number ProofingIntent = INTENT_RELATIVE_COLORIMETRIC;
        cmsUInt32Number dwFlags = cmsFLAGS_SOFTPROOFING;
        
        if (Size > 6) {
            Intent = Data[6] % 4;
            ProofingIntent = Data[6] % 4;
        }
        if (Size > 7) {
            dwFlags = Data[7] % 256;
        }
        
        cmsHTRANSFORM hTransform = cmsCreateProofingTransform(
            dummy_profiles[0],
            InputFormat,
            dummy_profiles[1],
            OutputFormat,
            dummy_profiles[2],
            Intent,
            ProofingIntent,
            dwFlags
        );
        
        cleanup_transform(hTransform);
    }
    
    // Test _cmsDefaultICCintents
    {
        cmsUInt32Number nProfiles = (Data[0] % 3) + 1;
        cmsUInt32Number Intents[4] = {INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC, 
                                      INTENT_SATURATION, INTENT_ABSOLUTE_COLORIMETRIC};
        cmsBool BPC[4] = {0, 0, 0, 0};
        cmsFloat64Number AdaptationStates[4] = {1.0, 1.0, 1.0, 1.0};
        cmsUInt32Number dwFlags = cmsFLAGS_NOCACHE;
        
        if (Size > 8) {
            for (int i = 0; i < 4 && i + 8 < Size; i++) {
                Intents[i] = Data[8 + i] % 4;
                BPC[i] = Data[8 + i] & 1;
            }
            dwFlags = Data[8] % 256;
        }
        
        cmsPipeline* pipeline = _cmsDefaultICCintents(
            ctx,
            nProfiles,
            Intents,
            dummy_profiles,
            BPC,
            AdaptationStates,
            dwFlags
        );
        
        cleanup_pipeline(pipeline);
    }
    
    // Clean up profiles
    for (int i = 0; i < 4; i++) {
        cleanup_profile(dummy_profiles[i]);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}