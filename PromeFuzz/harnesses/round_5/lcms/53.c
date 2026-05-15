// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// _cmsSetTransformUserData at cmsxform.c:750:16 in lcms2_plugin.h
// _cmsGetTransformUserData at cmsxform.c:758:18 in lcms2_plugin.h
// _cmsGetTransformWorker at cmsxform.c:787:28 in lcms2_plugin.h
// _cmsGetTransformFormattersFloat at cmsxform.c:772:16 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

static void dummy_free_user_data(struct _cmsContext_struct* ContextID, void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

    cmsHPROFILE hInProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hInProfile) {
        remove("./dummy_file");
        return 0;
    }

    cmsHPROFILE hOutProfile = cmsCreate_sRGBProfile();
    if (!hOutProfile) {
        cmsCloseProfile(hInProfile);
        remove("./dummy_file");
        return 0;
    }

    cmsUInt32Number input_format = TYPE_RGB_8;
    cmsUInt32Number output_format = TYPE_RGB_8;
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsUInt32Number flags = cmsFLAGS_NOCACHE;

    cmsHTRANSFORM hTransform = cmsCreateTransform(hInProfile, input_format,
                                                  hOutProfile, output_format,
                                                  intent, flags);
    if (!hTransform) {
        cmsCloseProfile(hInProfile);
        cmsCloseProfile(hOutProfile);
        remove("./dummy_file");
        return 0;
    }

    struct _cmstransform_struct* transform = (struct _cmstransform_struct*)hTransform;

    cmsUInt32Number input_fmt = cmsGetTransformInputFormat(hTransform);

    void* user_data = malloc(16);
    if (user_data) {
        memset(user_data, 0xAA, 16);
        _cmsSetTransformUserData(transform, user_data, dummy_free_user_data);
    }

    void* retrieved_data = _cmsGetTransformUserData(transform);

    _cmsTransform2Fn worker = _cmsGetTransformWorker(transform);

    cmsFormatterFloat from_input_fmt = NULL;
    cmsFormatterFloat to_output_fmt = NULL;
    _cmsGetTransformFormattersFloat(transform, &from_input_fmt, &to_output_fmt);

    cmsUInt32Number transform_flags = _cmsGetTransformFlags(transform);

    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hInProfile);
    cmsCloseProfile(hOutProfile);
    remove("./dummy_file");

    return 0;
}