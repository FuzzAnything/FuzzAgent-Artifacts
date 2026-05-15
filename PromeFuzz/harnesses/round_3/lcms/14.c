// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
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

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t* data, size_t size) {
    if (size < 128) {
        return cmsCreate_sRGBProfile();
    }
    
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return cmsCreate_sRGBProfile();
    }
    
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    cmsHPROFILE profile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    remove("./dummy_file");
    return profile;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsUInt16Number alarmCodes[cmsMAXCHANNELS];
    cmsUInt16Number newAlarm[cmsMAXCHANNELS];
    
    for (int i = 0; i < cmsMAXCHANNELS; i++) {
        if (Size > i * 2 + 1) {
            newAlarm[i] = (Data[i * 2 % Size] << 8) | Data[(i * 2 + 1) % Size];
        } else {
            newAlarm[i] = i * 100;
        }
    }
    
    cmsSetAlarmCodes(newAlarm);
    cmsGetAlarmCodes(alarmCodes);
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsSetAlarmCodesTHR(context, newAlarm);
        cmsGetAlarmCodesTHR(context, alarmCodes);
    }
    
    cmsHPROFILE inputProfile = create_dummy_profile_from_data(Data, Size);
    cmsHPROFILE outputProfile = create_dummy_profile_from_data(Data + Size/2, Size/2);
    cmsHPROFILE proofingProfile = create_dummy_profile_from_data(Data + Size/3, Size/3);
    
    uint32_t inputFormat = (Size > 0) ? Data[0] : TYPE_RGB_8;
    uint32_t outputFormat = (Size > 1) ? Data[1] : TYPE_RGB_8;
    uint32_t intent = (Size > 2) ? Data[2] % 4 : INTENT_PERCEPTUAL;
    uint32_t proofingIntent = (Size > 3) ? Data[3] % 4 : INTENT_ABSOLUTE_COLORIMETRIC;
    uint32_t flags = (Size > 4) ? Data[4] : 0;
    
    cmsHTRANSFORM transform1 = cmsCreateTransformTHR(context,
                                                    inputProfile,
                                                    inputFormat,
                                                    outputProfile,
                                                    outputFormat,
                                                    intent,
                                                    flags);
    if (transform1) {
        cmsDeleteTransform(transform1);
    }
    
    cmsHTRANSFORM transform2 = cmsCreateProofingTransformTHR(context,
                                                            inputProfile,
                                                            inputFormat,
                                                            outputProfile,
                                                            outputFormat,
                                                            proofingProfile,
                                                            intent,
                                                            proofingIntent,
                                                            flags);
    if (transform2) {
        cmsDeleteTransform(transform2);
    }
    
    if (inputProfile && inputProfile != cmsCreate_sRGBProfile()) {
        cmsCloseProfile(inputProfile);
    }
    if (outputProfile && outputProfile != cmsCreate_sRGBProfile()) {
        cmsCloseProfile(outputProfile);
    }
    if (proofingProfile && proofingProfile != cmsCreate_sRGBProfile()) {
        cmsCloseProfile(proofingProfile);
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}