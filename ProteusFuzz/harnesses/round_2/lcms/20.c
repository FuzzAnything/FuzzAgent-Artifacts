// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsGetHeaderFlags at cmsio0.c:996:27 in lcms2.h
// cmsGetHeaderCreator at cmsio0.c:1020:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    cmsHPROFILE hProfile = NULL;
    
    // Try to create a profile from the fuzzed data if it's large enough
    if (size > 0) {
        hProfile = cmsOpenProfileFromMem(data, size);
        if (hProfile) {
            return hProfile;
        }
    }
    
    // Fallback: create a simple Gray profile using standard D50 white point
    cmsToneCurve *curve = cmsBuildGamma(NULL, 2.2f);
    if (!curve) return NULL;
    
    // Define D50 white point
    cmsCIExyY WhitePoint;
    WhitePoint.x = 0.9642f;
    WhitePoint.y = 1.0000f;
    WhitePoint.Y = 1.0000f;
    
    hProfile = cmsCreateGrayProfile(&WhitePoint, curve);
    
    cmsFreeToneCurve(curve);
    return hProfile;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (!hProfile) return 0;
    
    // Extract values from input data for fuzzing parameters
    cmsUInt32Number flags = 0;
    cmsUInt32Number version = 0;
    cmsUInt32Number renderingIntent = 0;
    cmsUInt32Number manufacturer = 0;
    
    if (Size >= 4) {
        memcpy(&flags, Data, 4);
        if (Size >= 8) {
            memcpy(&version, Data + 4, 4);
        }
        if (Size >= 12) {
            memcpy(&renderingIntent, Data + 8, 4);
        }
        if (Size >= 16) {
            memcpy(&manufacturer, Data + 12, 4);
        }
    }
    
    // Fuzz cmsSetHeaderFlags
    cmsSetHeaderFlags(hProfile, flags);
    
    // Fuzz cmsSetEncodedICCversion
    cmsSetEncodedICCversion(hProfile, version);
    
    // Fuzz cmsSetHeaderRenderingIntent (limit to valid range 0-3)
    cmsSetHeaderRenderingIntent(hProfile, renderingIntent & 0x3);
    
    // Fuzz cmsSetHeaderManufacturer
    cmsSetHeaderManufacturer(hProfile, manufacturer);
    
    // Fuzz cmsGetHeaderFlags
    cmsUInt32Number retrievedFlags = cmsGetHeaderFlags(hProfile);
    (void)retrievedFlags; // Suppress unused variable warning
    
    // Fuzz cmsGetHeaderCreator
    cmsUInt32Number creator = cmsGetHeaderCreator(hProfile);
    (void)creator; // Suppress unused variable warning
    
    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}