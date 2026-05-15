// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsGetPCS at cmsio0.c:1069:34 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
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

static cmsHPROFILE create_test_profile(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cmsCreateProfilePlaceholder(NULL);
    }
    
    size_t profile_size = size > 1024 ? 1024 : size;
    char *profile_data = (char*)malloc(profile_size);
    if (!profile_data) {
        return cmsCreateProfilePlaceholder(NULL);
    }
    
    memcpy(profile_data, data, profile_size);
    cmsHPROFILE profile = cmsOpenProfileFromMem(profile_data, profile_size);
    free(profile_data);
    
    if (!profile) {
        profile = cmsCreateProfilePlaceholder(NULL);
    }
    
    return profile;
}

static cmsColorSpaceSignature get_signature_from_data(const uint8_t *data, size_t size, size_t offset) {
    if (offset + sizeof(uint32_t) <= size) {
        uint32_t sig_val;
        memcpy(&sig_val, data + offset, sizeof(uint32_t));
        return (cmsColorSpaceSignature)sig_val;
    }
    return (cmsColorSpaceSignature)0;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsHPROFILE hProfile = create_test_profile(Data, Size);
    if (!hProfile) {
        return 0;
    }
    
    cmsBool is_matrix = cmsIsMatrixShaper(hProfile);
    (void)is_matrix;
    
    cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
    (void)pcs;
    
    cmsColorSpaceSignature colorspace = cmsGetColorSpace(hProfile);
    (void)colorspace;
    
    int lcms_space = _cmsLCMScolorSpace(colorspace);
    (void)lcms_space;
    
    cmsColorSpaceSignature new_pcs = get_signature_from_data(Data, Size, 0);
    cmsSetPCS(hProfile, new_pcs);
    
    cmsColorSpaceSignature new_colorspace = get_signature_from_data(Data, Size, sizeof(uint32_t) % Size);
    cmsSetColorSpace(hProfile, new_colorspace);
    
    cmsCloseProfile(hProfile);
    return 0;
}