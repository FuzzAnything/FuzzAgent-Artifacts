// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    // First, try to create a profile from the input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    
    // If we couldn't create a profile from memory, try to create one from a file
    if (hProfile == NULL) {
        // Write data to a dummy file and try to open it
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
            // Clean up the dummy file
            remove("./dummy_file");
        }
    }
    
    // If we still don't have a profile, create a simple sRGB profile as fallback
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfile();
    }
    
    // Only proceed if we have a valid profile
    if (hProfile != NULL) {
        // 1. Test cmsIsMatrixShaper
        (void)cmsIsMatrixShaper(hProfile);
        
        // 2. Test cmsDetectTAC
        (void)cmsDetectTAC(hProfile);
        
        // 3. Test cmsReadTag with various common tag signatures
        // Use the input data to select which tags to try
        static const cmsTagSignature commonTags[] = {
            cmsSigRedColorantTag,
            cmsSigGreenColorantTag,
            cmsSigBlueColorantTag,
            cmsSigRedTRCTag,
            cmsSigGreenTRCTag,
            cmsSigBlueTRCTag,
            cmsSigGrayTRCTag,
            cmsSigMediaWhitePointTag,
            cmsSigMediaBlackPointTag,
            cmsSigCopyrightTag,
            cmsSigProfileDescriptionTag,
            cmsSigDeviceModelDescTag,
            cmsSigCalibrationDateTimeTag
        };
        
        // Use first byte of data to select a tag (if available)
        if (Size > 0) {
            size_t tagIndex = Data[0] % (sizeof(commonTags) / sizeof(commonTags[0]));
            (void)cmsReadTag(hProfile, commonTags[tagIndex]);
        }
        
        // 4. Test cmsDetectBlackPoint with different intents and flags
        cmsCIEXYZ blackPoint;
        static const cmsUInt32Number intents[] = {
            INTENT_PERCEPTUAL,
            INTENT_RELATIVE_COLORIMETRIC,
            INTENT_SATURATION,
            INTENT_ABSOLUTE_COLORIMETRIC
        };
        
        static const cmsUInt32Number flags[] = {
            0,
            cmsFLAGS_NOWHITEONWHITEFIXUP,
            cmsFLAGS_HIGHRESPRECALC,
            cmsFLAGS_LOWRESPRECALC
        };
        
        // Use input data to select intent and flags (if available)
        if (Size > 1) {
            size_t intentIndex = Data[1] % (sizeof(intents) / sizeof(intents[0]));
            size_t flagIndex = Size > 2 ? Data[2] % (sizeof(flags) / sizeof(flags[0])) : 0;
            
            (void)cmsDetectBlackPoint(&blackPoint, hProfile, intents[intentIndex], flags[flagIndex]);
        }
        
        // 5. Test cmsDetectRGBProfileGamma with different thresholds
        // Use input data to generate threshold values
        double threshold = 0.001;
        if (Size > 3) {
            // Generate a threshold between 0.0001 and 0.1
            threshold = 0.0001 + (Data[3] / 255.0) * 0.0999;
        }
        (void)cmsDetectRGBProfileGamma(hProfile, threshold);
        
        // 6. Test cmsGetProfileVersion
        (void)cmsGetProfileVersion(hProfile);
        
        // Clean up
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}