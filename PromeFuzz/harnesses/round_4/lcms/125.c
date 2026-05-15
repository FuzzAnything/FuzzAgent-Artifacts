// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
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
#include <stdio.h>

static cmsToneCurve* create_test_tone_curve(const uint8_t* data, size_t size) {
    if (size < 256 * 2) return NULL;
    
    cmsUInt16Number table[256];
    for (int i = 0; i < 256; i++) {
        table[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    return cmsBuildTabulatedToneCurve16(NULL, 256, table);
}

static cmsHPROFILE create_test_profile(const uint8_t* data, size_t size) {
    if (size == 0) return NULL;
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    remove("./dummy_file");
    
    return profile;
}

int LLVMFuzzerTestOneInput_125(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test cmsIsMatrixShaper
    cmsHPROFILE profile1 = create_test_profile(Data, Size);
    if (profile1) {
        cmsBool isMatrixShaper = cmsIsMatrixShaper(profile1);
        (void)isMatrixShaper; // Use result to avoid unused variable warning
        cmsCloseProfile(profile1);
    }
    
    // Test cmsDetectDestinationBlackPoint
    cmsHPROFILE profile2 = create_test_profile(Data, Size % 1024); // Use different size
    if (profile2) {
        cmsCIEXYZ blackPoint1;
        cmsUInt32Number intent1 = Data[0] % 4;
        cmsUInt32Number flags1 = Data[1];
        cmsBool detected1 = cmsDetectDestinationBlackPoint(&blackPoint1, profile2, intent1, flags1);
        (void)detected1;
        cmsCloseProfile(profile2);
    }
    
    // Test cmsEstimateGamma
    cmsToneCurve* toneCurve = create_test_tone_curve(Data, Size);
    if (toneCurve) {
        cmsFloat64Number precision = (Data[0] % 100) / 100.0 + 0.001;
        cmsFloat64Number gamma = cmsEstimateGamma(toneCurve, precision);
        (void)gamma;
        cmsFreeToneCurve(toneCurve);
    }
    
    // Test cmsDetectTAC
    cmsHPROFILE profile3 = create_test_profile(Data + (Size/2), Size/2);
    if (profile3) {
        cmsFloat64Number tac = cmsDetectTAC(profile3);
        (void)tac;
        cmsCloseProfile(profile3);
    }
    
    // Test cmsDetectBlackPoint
    cmsHPROFILE profile4 = create_test_profile(Data, Size % 512);
    if (profile4) {
        cmsCIEXYZ blackPoint2;
        cmsUInt32Number intent2 = (Data[0] + Data[1]) % 4;
        cmsUInt32Number flags2 = Data[2];
        cmsBool detected2 = cmsDetectBlackPoint(&blackPoint2, profile4, intent2, flags2);
        (void)detected2;
        cmsCloseProfile(profile4);
    }
    
    // Test cmsDetectRGBProfileGamma
    cmsHPROFILE profile5 = create_test_profile(Data, Size);
    if (profile5) {
        cmsFloat64Number threshold = (Data[0] % 1000) / 1000.0;
        cmsFloat64Number rgbGamma = cmsDetectRGBProfileGamma(profile5, threshold);
        (void)rgbGamma;
        cmsCloseProfile(profile5);
    }
    
    return 0;
}