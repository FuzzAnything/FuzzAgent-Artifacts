// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsOpenProfileFromMemTHR at cmsio0.c:1271:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsFormatterForColorspaceOfProfile at cmspack.c:4025:27 in lcms2.h
// cmsCreateProofingTransformTHR at cmsxform.c:1361:25 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size, cmsContext context) {
    if (size == 0) {
        return cmsCreate_sRGBProfileTHR(context);
    }
    cmsHPROFILE profile = cmsOpenProfileFromMemTHR(context, data, size);
    if (!profile) {
        profile = cmsCreate_sRGBProfileTHR(context);
    }
    return profile;
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) {
        cmsDeleteTransform(transform);
    }
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    size_t profile_size = Size / 4;
    const uint8_t *profile1_data = Data;
    const uint8_t *profile2_data = Data + profile_size;
    const uint8_t *profile3_data = Data + 2 * profile_size;
    const uint8_t *profile4_data = Data + 3 * profile_size;
    size_t profile2_size = (Size >= 2 * profile_size) ? profile_size : 0;
    size_t profile3_size = (Size >= 3 * profile_size) ? profile_size : 0;
    size_t profile4_size = Size - 3 * profile_size;

    cmsHPROFILE profile1 = create_profile_from_data(profile1_data, profile_size, context);
    cmsHPROFILE profile2 = create_profile_from_data(profile2_data, profile2_size, context);
    cmsHPROFILE profile3 = create_profile_from_data(profile3_data, profile3_size, context);
    cmsHPROFILE profile4 = create_profile_from_data(profile4_data, profile4_size, context);

    cmsColorSpaceSignature colorspace = cmsGetColorSpace(profile1);
    (void)colorspace;

    cmsUInt32Number nProfiles = 4;
    cmsHPROFILE hProfiles[4] = {profile1, profile2, profile3, profile4};
    cmsBool BPC[4] = {Data[0] & 1, Data[1] & 1, Data[2] & 1, Data[3] & 1};
    cmsUInt32Number Intents[4] = {Data[0] % 5, Data[1] % 5, Data[2] % 5, Data[3] % 5};
    cmsFloat64Number AdaptationStates[4] = {Data[0] / 255.0, Data[1] / 255.0, Data[2] / 255.0, Data[3] / 255.0};
    cmsHPROFILE hGamutProfile = (Data[0] & 2) ? profile2 : NULL;
    cmsUInt32Number nGamutPCSposition = Data[1] % 3;
    cmsUInt32Number InputFormat = (Data[2] % 2) ? TYPE_RGB_8 : TYPE_RGB_16;
    cmsUInt32Number OutputFormat = (Data[3] % 2) ? TYPE_CMYK_8 : TYPE_CMYK_16;
    cmsUInt32Number dwFlags = Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3];

    cmsHTRANSFORM transform = cmsCreateExtendedTransform(context, nProfiles, hProfiles, BPC, Intents, AdaptationStates, hGamutProfile, nGamutPCSposition, InputFormat, OutputFormat, dwFlags);
    cleanup_transform(transform);

    cmsUInt32Number nBytes = (Data[0] & 1) ? 1 : 2;
    cmsBool lIsFloat = (Data[1] & 1) ? TRUE : FALSE;
    cmsUInt32Number formatter = cmsFormatterForColorspaceOfProfile(profile1, nBytes, lIsFloat);
    (void)formatter;

    cmsUInt32Number InputFormat2 = (Data[0] % 2) ? TYPE_RGB_8 : TYPE_RGB_16;
    cmsUInt32Number OutputFormat2 = (Data[1] % 2) ? TYPE_CMYK_8 : TYPE_CMYK_16;
    cmsUInt32Number Intent = Data[2] % 5;
    cmsUInt32Number ProofingIntent = Data[3] % 5;
    cmsUInt32Number dwFlags2 = Data[0] << 24 | Data[1] << 16 | Data[2] << 8 | Data[3];

    cmsHTRANSFORM proofing_transform = cmsCreateProofingTransformTHR(context, profile1, InputFormat2, profile2, OutputFormat2, profile3, Intent, ProofingIntent, dwFlags2);
    cleanup_transform(proofing_transform);

    cmsUInt32Number pcs_formatter = cmsFormatterForPCSOfProfile(profile1, nBytes, lIsFloat);
    (void)pcs_formatter;

    cmsUInt32Number channels = cmsChannelsOf(colorspace);
    (void)channels;

    cleanup_profile(profile1);
    cleanup_profile(profile2);
    cleanup_profile(profile3);
    cleanup_profile(profile4);

    cmsDeleteContext(context);
    return 0;
}