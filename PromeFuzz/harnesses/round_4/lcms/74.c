// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
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

static cmsIOHANDLER* create_dummy_iohandler(cmsContext ctx) {
    FILE* fp = fopen("./dummy_file", "w+b");
    if (!fp) return NULL;
    
    cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(ctx, "./dummy_file", "w+b");
    fclose(fp);
    return io;
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfileTHR(ctx);
}

static cmsHTRANSFORM create_dummy_transform(cmsContext ctx) {
    cmsHPROFILE src = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE dst = cmsCreate_sRGBProfileTHR(ctx);
    if (!src || !dst) {
        if (src) cmsCloseProfile(src);
        if (dst) cmsCloseProfile(dst);
        return NULL;
    }
    
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, src, TYPE_RGB_8, dst, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);
    cmsCloseProfile(src);
    cmsCloseProfile(dst);
    return transform;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Extract parameters from fuzzer input with bounds checking
    cmsColorSpaceSignature colorspace = cmsSigCmykData; // Default to CMYK
    cmsFloat64Number limit = 100.0; // Default limit
    cmsFloat64Number version = 2.0; // Default version
    cmsUInt32Number flags = 0; // Default flags
    cmsUInt32Number intent = 0; // Default intent
    cmsUInt32Number format = 0; // Default format
    
    if (Size > 0) colorspace = (cmsColorSpaceSignature)(Data[0] % 20);
    if (Size > 1) limit = (Data[1] % 400) + 1.0;
    if (Size > 2) version = 2.0 + (Data[2] % 3);
    if (Size > 3) flags = Data[3];
    if (Size > 4) intent = Data[4] % 4;
    if (Size > 5) format = Data[5] % 16;
    
    // Test cmsCreateInkLimitingDeviceLinkTHR
    cmsHPROFILE link1 = cmsCreateInkLimitingDeviceLinkTHR(ctx, cmsSigCmykData, limit);
    if (link1) cmsCloseProfile(link1);
    
    // Test cmsCreateInkLimitingDeviceLink (non-THR version)
    cmsHPROFILE link2 = cmsCreateInkLimitingDeviceLink(cmsSigCmykData, limit);
    if (link2) cmsCloseProfile(link2);
    
    // Create dummy transform and test cmsTransform2DeviceLink
    cmsHTRANSFORM transform = create_dummy_transform(ctx);
    if (transform) {
        cmsHPROFILE devicelink = cmsTransform2DeviceLink(transform, version, flags);
        if (devicelink) cmsCloseProfile(devicelink);
        cmsDeleteTransform(transform);
    }
    
    // Test cmsGetPostScriptColorResource
    cmsHPROFILE profile = create_dummy_profile(ctx);
    if (profile) {
        cmsIOHANDLER* io = create_dummy_iohandler(ctx);
        if (io) {
            cmsUInt32Number result = cmsGetPostScriptColorResource(ctx, 
                (cmsPSResourceType)(flags % 2), 
                profile, 
                intent, 
                flags, 
                io);
            (void)result;
            cmsCloseIOhandler(io);
        }
        cmsCloseProfile(profile);
    }
    
    // Test cmsDetectTAC
    cmsHPROFILE profile2 = create_dummy_profile(ctx);
    if (profile2) {
        cmsFloat64Number tac = cmsDetectTAC(profile2);
        (void)tac;
        cmsCloseProfile(profile2);
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    cmsHPROFILE profiles[2];
    profiles[0] = create_dummy_profile(ctx);
    profiles[1] = create_dummy_profile(ctx);
    
    if (profiles[0] && profiles[1]) {
        cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransformTHR(ctx,
            profiles,
            2,
            format,
            format,
            intent,
            flags);
        if (multi_transform) cmsDeleteTransform(multi_transform);
    }
    
    if (profiles[0]) cmsCloseProfile(profiles[0]);
    if (profiles[1]) cmsCloseProfile(profiles[1]);
    
    // Cleanup
    cmsDeleteContext(ctx);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}