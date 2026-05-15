// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profiles for testing
    cmsHPROFILE rgb_profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE lab_profile = cmsCreateLab4ProfileTHR(ctx, NULL);
    cmsHPROFILE cmyk_profile = cmsCreateNULLProfileTHR(ctx);
    
    // Test cmsCreateInkLimitingDeviceLinkTHR
    cmsFloat64Number limit = (Data[0] % 400) + 1.0;
    cmsColorSpaceSignature colorspace = cmsSigCmykData;
    cmsHPROFILE ink_limit_profile = cmsCreateInkLimitingDeviceLinkTHR(ctx, colorspace, limit);
    if (ink_limit_profile) cmsCloseProfile(ink_limit_profile);
    
    // Test cmsCreateMultiprofileTransformTHR
    if (rgb_profile && lab_profile && cmyk_profile) {
        cmsHPROFILE profiles[3] = {rgb_profile, lab_profile, cmyk_profile};
        cmsUInt32Number nProfiles = 3;
        cmsUInt32Number inputFormat = TYPE_RGB_8;
        cmsUInt32Number outputFormat = TYPE_CMYK_8;
        cmsUInt32Number intent = INTENT_PERCEPTUAL;
        cmsUInt32Number flags = 0;
        
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(ctx, profiles, nProfiles,
                                                                    inputFormat, outputFormat,
                                                                    intent, flags);
        if (transform) {
            // Test cmsTransform2DeviceLink
            cmsFloat64Number version = 4.2;
            cmsUInt32Number transform_flags = 0;
            cmsHPROFILE devicelink = cmsTransform2DeviceLink(transform, version, transform_flags);
            if (devicelink) cmsCloseProfile(devicelink);
            
            cmsDeleteTransform(transform);
        }
    }
    
    // Test cmsGetPostScriptColorResource
    if (rgb_profile) {
        // Create a memory-based I/O handler
        cmsIOHANDLER* io = cmsOpenIOhandlerFromMem(ctx, NULL, 0, "w");
        if (io) {
            cmsUInt32Number result = cmsGetPostScriptColorResource(ctx, cmsPS_RESOURCE_CSA,
                                                                   rgb_profile, INTENT_PERCEPTUAL,
                                                                   0, io);
            cmsCloseIOhandler(io);
        }
    }
    
    // Test cmsDetectTAC
    if (cmyk_profile) {
        cmsFloat64Number tac = cmsDetectTAC(cmyk_profile);
        (void)tac;
    }
    
    // Test cmsCreateInkLimitingDeviceLink (non-THR version)
    limit = (Data[1] % 400) + 1.0;
    cmsHPROFILE ink_limit_profile2 = cmsCreateInkLimitingDeviceLink(colorspace, limit);
    if (ink_limit_profile2) cmsCloseProfile(ink_limit_profile2);
    
    // Cleanup
    if (rgb_profile) cmsCloseProfile(rgb_profile);
    if (lab_profile) cmsCloseProfile(lab_profile);
    if (cmyk_profile) cmsCloseProfile(cmyk_profile);
    
    cmsDeleteContext(ctx);
    
    return 0;
}