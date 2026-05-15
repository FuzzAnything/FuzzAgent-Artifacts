// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsHPROFILE create_test_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cmsCreate_sRGBProfile();
    }
    
    cmsHPROFILE profile = cmsOpenProfileFromMem(data, size);
    if (profile == NULL) {
        profile = cmsCreate_sRGBProfile();
    }
    return profile;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    
    cmsContext context = NULL;
    cmsHPROFILE profiles[2] = {NULL, NULL};
    cmsHTRANSFORM transform = NULL;
    cmsHPROFILE devicelink = NULL;
    
    uint32_t seed = *(const uint32_t*)data;
    const uint8_t* profile_data = data + 4;
    size_t profile_size = size > 4 ? size - 4 : 0;
    
    profiles[0] = create_test_profile_from_data(profile_data, profile_size);
    profiles[1] = cmsCreate_sRGBProfile();
    
    cmsUInt32Number input_format = (seed >> 0) & 0xF;
    cmsUInt32Number output_format = (seed >> 4) & 0xF;
    cmsUInt32Number intent = (seed >> 8) & 0x3;
    cmsUInt32Number flags = (seed >> 12) & 0xFFFF;
    cmsColorSpaceSignature colorspace = (seed >> 28) & 0xF;
    cmsFloat64Number limit = ((seed >> 16) & 0xFFF) / 10.0;
    
    switch (colorspace) {
        case 0: colorspace = cmsSigCmykData; break;
        case 1: colorspace = cmsSigRgbData; break;
        case 2: colorspace = cmsSigGrayData; break;
        case 3: colorspace = cmsSigLabData; break;
        case 4: colorspace = cmsSigLuvData; break;
        case 5: colorspace = cmsSigYCbCrData; break;
        case 6: colorspace = cmsSigYxyData; break;
        case 7: colorspace = cmsSigHsvData; break;
        case 8: colorspace = cmsSigHlsData; break;
        case 9: colorspace = cmsSigCmyData; break;
        default: colorspace = cmsSigCmykData; break;
    }
    
    if (limit < 1.0) limit = 50.0;
    if (limit > 400.0) limit = 400.0;
    
    transform = cmsCreateMultiprofileTransformTHR(context, profiles, 2,
                                                  input_format, output_format,
                                                  intent, flags);
    
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file != NULL) {
        cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
        if (io != NULL) {
            cmsGetPostScriptColorResource(context, cmsPS_RESOURCE_CSA,
                                          profiles[0], intent, flags, io);
            cmsCloseIOhandler(io);
        }
        fclose(dummy_file);
        remove("./dummy_file");
    }
    
    devicelink = cmsCreateInkLimitingDeviceLink(colorspace, limit);
    if (devicelink != NULL) {
        cmsCloseProfile(devicelink);
    }
    
    devicelink = cmsCreateInkLimitingDeviceLinkTHR(context, colorspace, limit);
    if (devicelink != NULL) {
        cmsCloseProfile(devicelink);
    }
    
    cmsFloat64Number tac = cmsDetectTAC(profiles[0]);
    
    if (transform != NULL) {
        cmsFloat64Number version = ((seed >> 20) & 0xF) / 10.0 + 2.0;
        if (version < 2.0) version = 2.0;
        if (version > 4.0) version = 4.0;
        
        devicelink = cmsTransform2DeviceLink(transform, version, flags);
        if (devicelink != NULL) {
            cmsCloseProfile(devicelink);
        }
        cmsDeleteTransform(transform);
    }
    
    if (profiles[0] != NULL) cmsCloseProfile(profiles[0]);
    if (profiles[1] != NULL) cmsCloseProfile(profiles[1]);
    
    return 0;
}