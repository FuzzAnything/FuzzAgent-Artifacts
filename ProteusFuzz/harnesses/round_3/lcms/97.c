// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *data, size_t size) {
    if (size < 128) return NULL;
    
    unsigned char *profile_data = (unsigned char *)malloc(size);
    if (!profile_data) return NULL;
    
    memcpy(profile_data, data, size);
    
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data, size);
    free(profile_data);
    
    return hProfile;
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test _cmsICCcolorSpace with known PT_* constants available in lcms2.h
    int valid_notations[] = {
        PT_GRAY, PT_RGB, PT_XYZ, PT_CMYK, PT_YCbCr, PT_Yxy, PT_HSV
    };
    int num_notations = sizeof(valid_notations) / sizeof(valid_notations[0]);
    
    for (int i = 0; i < num_notations; i++) {
        cmsColorSpaceSignature sig = _cmsICCcolorSpace(valid_notations[i]);
        (void)sig;
    }
    
    // Try random values from input data as notations
    for (size_t i = 0; i < Size && i < 100; i++) {
        int notation = Data[i];
        cmsColorSpaceSignature sig = _cmsICCcolorSpace(notation);
        (void)sig;
    }
    
    cmsHPROFILE hProfile = NULL;
    
    if (Size >= 128) {
        hProfile = create_dummy_profile(Data, Size);
    }
    
    if (hProfile) {
        // Test cmsSetColorSpace with correct signature names
        cmsColorSpaceSignature test_spaces[] = {
            cmsSigRgbData, cmsSigXYZData, cmsSigLabData, 
            cmsSigGrayData, cmsSigCmykData
        };
        
        for (int i = 0; i < 5; i++) {
            cmsSetColorSpace(hProfile, test_spaces[i]);
        }
        
        // Test with random signature from input
        if (Size >= 4) {
            uint32_t random_sig = *(uint32_t *)Data;
            cmsSetColorSpace(hProfile, (cmsColorSpaceSignature)random_sig);
        }
        
        // Test cmsGetColorSpace
        cmsColorSpaceSignature retrieved_space = cmsGetColorSpace(hProfile);
        (void)retrieved_space;
        
        // Test cmsSetPCS with correct signature names
        cmsColorSpaceSignature test_pcs[] = {
            cmsSigXYZData, cmsSigLabData
        };
        
        for (int i = 0; i < 2; i++) {
            cmsSetPCS(hProfile, test_pcs[i]);
        }
        
        // Test with random PCS from input
        if (Size >= 8) {
            uint32_t random_pcs = *(uint32_t *)(Data + 4);
            cmsSetPCS(hProfile, (cmsColorSpaceSignature)random_pcs);
        }
        
        // Test cmsGetPCS
        cmsColorSpaceSignature retrieved_pcs = cmsGetPCS(hProfile);
        (void)retrieved_pcs;
        
        // Test _cmsLCMScolorSpace with various signatures
        cmsColorSpaceSignature test_sigs[] = {
            cmsSigRgbData, cmsSigXYZData, cmsSigLabData, 
            cmsSigGrayData, cmsSigCmykData, cmsSigYxyData
        };
        
        for (int i = 0; i < 6; i++) {
            int pixel_type = _cmsLCMScolorSpace(test_sigs[i]);
            (void)pixel_type;
        }
        
        // Test with random signatures from input
        for (size_t i = 0; i + 4 <= Size; i += 4) {
            uint32_t sig_val = *(uint32_t *)(Data + i);
            int pixel_type = _cmsLCMScolorSpace((cmsColorSpaceSignature)sig_val);
            (void)pixel_type;
        }
        
        cleanup_profile(hProfile);
    }
    
    return 0;
}