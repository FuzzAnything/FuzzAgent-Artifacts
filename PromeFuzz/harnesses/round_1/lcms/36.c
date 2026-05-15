// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// _cmsEncodeDateTimeNumber at cmsplugin.c:407:16 in lcms2_plugin.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// _cmsDecodeDateTimeNumber at cmsplugin.c:390:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <time.h>

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    // Create a profile from the fuzz data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile == NULL) {
        // If we can't create a profile from data, create a default sRGB profile
        // to still exercise the API functions
        hProfile = cmsCreate_sRGBProfile();
        if (hProfile == NULL) {
            return 0;
        }
    }

    // 1. Test cmsGetHeaderRenderingIntent
    cmsUInt32Number intent = cmsGetHeaderRenderingIntent(hProfile);
    (void)intent; // Use the result to avoid unused variable warning

    // 2. Test cmsGetHeaderCreationDateTime
    struct tm creation_time;
    cmsBool success = cmsGetHeaderCreationDateTime(hProfile, &creation_time);
    (void)success; // Use the result

    // 3. Test cmsReadTag with various tag signatures
    // Try some common tag signatures
    cmsTagSignature tags[] = {
        cmsSigRedTRCTag,
        cmsSigGreenTRCTag,
        cmsSigBlueTRCTag,
        cmsSigRedMatrixColumnTag,
        cmsSigGreenMatrixColumnTag,
        cmsSigBlueMatrixColumnTag,
        cmsSigMediaWhitePointTag,
        cmsSigCopyrightTag,
        cmsSigProfileDescriptionTag
    };
    
    for (size_t i = 0; i < sizeof(tags)/sizeof(tags[0]); i++) {
        void* tag_data = cmsReadTag(hProfile, tags[i]);
        (void)tag_data; // Use the result
    }

    // 4. Test _cmsEncodeDateTimeNumber
    cmsDateTimeNumber encoded_time;
    _cmsEncodeDateTimeNumber(&encoded_time, &creation_time);

    // 5. Test cmsGetTagCount
    cmsInt32Number tag_count = cmsGetTagCount(hProfile);
    (void)tag_count; // Use the result

    // 6. Test _cmsDecodeDateTimeNumber
    struct tm decoded_time;
    _cmsDecodeDateTimeNumber(&encoded_time, &decoded_time);

    // Clean up
    cmsCloseProfile(hProfile);

    return 0;
}