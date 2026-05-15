// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;

    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;

    // Extract parameters from fuzzer input
    cmsColorSpaceSignature color_space = cmsSigCmykData;
    cmsFloat64Number limit = (Data[0] % 400) + 1.0;
    cmsUInt32Number input_format = *(cmsUInt32Number*)(Data + 4) % 0xFFFF;
    cmsUInt32Number output_format = *(cmsUInt32Number*)(Data + 8) % 0xFFFF;
    cmsUInt32Number intent = *(cmsUInt32Number*)(Data + 12) % 4;
    cmsUInt32Number flags = *(cmsUInt32Number*)(Data + 16);
    cmsFloat64Number version = (Data[20] % 50) / 10.0;
    cmsUInt32Number ps_type = *(cmsUInt32Number*)(Data + 24) % 3;

    // 1. Test cmsCreateInkLimitingDeviceLinkTHR
    cmsHPROFILE ink_limit_profile = cmsCreateInkLimitingDeviceLinkTHR(
        context, color_space, limit);
    if (ink_limit_profile) {
        cmsCloseProfile(ink_limit_profile);
    }

    // 2. Test cmsCreateMultiprofileTransformTHR
    cmsHPROFILE profiles[2];
    profiles[0] = cmsCreate_sRGBProfileTHR(context);
    profiles[1] = cmsCreateInkLimitingDeviceLinkTHR(context, color_space, limit);
    
    if (profiles[0] && profiles[1]) {
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
            context, profiles, 2, input_format, output_format, intent, flags);
        
        // 3. Test cmsTransform2DeviceLink
        if (transform) {
            cmsHPROFILE devicelink = cmsTransform2DeviceLink(transform, version, flags);
            if (devicelink) {
                cmsCloseProfile(devicelink);
            }
            cmsDeleteTransform(transform);
        }
    }
    
    if (profiles[0]) cmsCloseProfile(profiles[0]);
    if (profiles[1]) cmsCloseProfile(profiles[1]);

    // 4. Test cmsGetPostScriptColorResource
    cmsHPROFILE test_profile = cmsCreate_sRGBProfileTHR(context);
    if (test_profile) {
        // Create a file-based I/O handler
        FILE* dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            fclose(dummy_file);
        }
        
        cmsIOHANDLER* io_handler = cmsOpenIOhandlerFromFile(context, "./dummy_file", "w");
        if (io_handler) {
            cmsGetPostScriptColorResource(context, (cmsPSResourceType)ps_type,
                                          test_profile, intent, flags, io_handler);
            cmsCloseIOhandler(io_handler);
        }
        
        // Clean up the dummy file
        remove("./dummy_file");
        cmsCloseProfile(test_profile);
    }

    // 5. Test cmsDetectTAC
    cmsHPROFILE tac_profile = cmsCreateInkLimitingDeviceLinkTHR(
        context, color_space, limit);
    if (tac_profile) {
        cmsDetectTAC(tac_profile);
        cmsCloseProfile(tac_profile);
    }

    // 6. Test cmsCreateInkLimitingDeviceLink (non-THR version)
    cmsHPROFILE ink_limit_profile2 = cmsCreateInkLimitingDeviceLink(
        color_space, limit);
    if (ink_limit_profile2) {
        cmsCloseProfile(ink_limit_profile2);
    }

    cmsDeleteContext(context);
    return 0;
}