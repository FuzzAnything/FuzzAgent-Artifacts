// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

static void dummy_free_user_data(struct _cmsContext_struct* ContextID, void* Data) {
    (void)ContextID;
    (void)Data;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsHPROFILE hInProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE hOutProfile = cmsCreate_sRGBProfile();
    
    if (!hInProfile || !hOutProfile) {
        if (hInProfile) cmsCloseProfile(hInProfile);
        if (hOutProfile) cmsCloseProfile(hOutProfile);
        return 0;
    }
    
    cmsHTRANSFORM hTransform = cmsCreateTransform(hInProfile, TYPE_RGB_8, 
                                                   hOutProfile, TYPE_RGB_8, 
                                                   INTENT_PERCEPTUAL, 0);
    
    cmsCloseProfile(hInProfile);
    cmsCloseProfile(hOutProfile);
    
    if (!hTransform) {
        return 0;
    }
    
    struct _cmstransform_struct* internal_transform = (struct _cmstransform_struct*)hTransform;
    
    cmsUInt32Number input_format = cmsGetTransformInputFormat(hTransform);
    (void)input_format;
    
    cmsUInt32Number flags = _cmsGetTransformFlags(internal_transform);
    (void)flags;
    
    _cmsTransform2Fn worker_fn = _cmsGetTransformWorker(internal_transform);
    (void)worker_fn;
    
    void* user_data = NULL;
    if (Size > 0) {
        user_data = malloc(Size);
        if (user_data) {
            memcpy(user_data, Data, Size);
            _cmsSetTransformUserData(internal_transform, user_data, dummy_free_user_data);
        }
    } else {
        _cmsSetTransformUserData(internal_transform, NULL, NULL);
    }
    
    void* retrieved_data = _cmsGetTransformUserData(internal_transform);
    (void)retrieved_data;
    
    cmsFormatterFloat from_input = NULL;
    cmsFormatterFloat to_output = NULL;
    _cmsGetTransformFormattersFloat(internal_transform, &from_input, &to_output);
    
    if (user_data) {
        free(user_data);
    }
    
    cmsDeleteTransform(hTransform);
    
    return 0;
}

#ifdef __cplusplus
}
#endif