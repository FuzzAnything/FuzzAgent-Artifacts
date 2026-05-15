// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsUInt32Number dummy_read(void* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    return 0;
}

static cmsBool dummy_seek(void* iohandler, cmsUInt32Number offset) {
    return 1;
}

static cmsBool dummy_close(void* iohandler) {
    return 1;
}

static cmsUInt32Number dummy_tell(void* iohandler) {
    return 0;
}

static cmsBool dummy_write(void* iohandler, cmsUInt32Number size, const void* Buffer) {
    return 1;
}

static cmsIOHANDLER* create_dummy_iohandler(cmsContext ctx) {
    cmsIOHANDLER* io = cmsOpenIOhandlerFromMem(ctx, NULL, 0, "w");
    if (!io) return NULL;
    
    return io;
}

static void cleanup_iohandler(cmsIOHANDLER* io) {
    if (io) {
        cmsCloseIOhandler(io);
    }
}

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t* Data, size_t Size) {
    if (Size < 128) {
        return cmsCreate_sRGBProfile();
    }
    return cmsOpenProfileFromMem(Data, Size);
}

int LLVMFuzzerTestOneInput_82(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsContext ctx = NULL;
    cmsHPROFILE profile = NULL;
    cmsHPROFILE profiles[2] = {NULL, NULL};
    cmsHTRANSFORM transform = NULL;
    cmsIOHANDLER* io = NULL;
    
    // Create context
    ctx = cmsCreateContext(NULL, NULL);
    
    // Create dummy profile from fuzzer data
    profile = create_dummy_profile_from_data(Data, Size);
    if (!profile) {
        profile = cmsCreate_sRGBProfile();
    }
    
    profiles[0] = profile;
    profiles[1] = profile;
    
    // Safely extract values from Data with bounds checking
    uint8_t d0 = Size > 0 ? Data[0] : 0;
    uint8_t d1 = Size > 1 ? Data[1] : 0;
    uint8_t d2 = Size > 2 ? Data[2] : 0;
    uint8_t d3 = Size > 3 ? Data[3] : 0;
    uint8_t d4 = Size > 4 ? Data[4] : 0;
    uint8_t d5 = Size > 5 ? Data[5] : 0;
    uint8_t d6 = Size > 6 ? Data[6] : 0;
    uint8_t d7 = Size > 7 ? Data[7] : 0;
    uint8_t d8 = Size > 8 ? Data[8] : 0;
    uint8_t d9 = Size > 9 ? Data[9] : 0;
    
    // 1. Test cmsCreateMultiprofileTransformTHR
    uint32_t intent = d0 % 4;
    uint32_t flags = d1;
    transform = cmsCreateMultiprofileTransformTHR(ctx, profiles, 2, 
                                                 TYPE_RGB_8, TYPE_RGB_8, 
                                                 intent, flags);
    if (transform) {
        cmsDeleteTransform(transform);
        transform = NULL;
    }
    
    // 2. Test cmsGetPostScriptColorResource
    io = create_dummy_iohandler(ctx);
    if (io) {
        cmsUInt32Number result = cmsGetPostScriptColorResource(ctx, 
                                                              (cmsPSResourceType)(d2 % 2), 
                                                              profile, 
                                                              d3 % 4, 
                                                              d4, 
                                                              io);
        (void)result;
        cleanup_iohandler(io);
        io = NULL;
    }
    
    // 3. Test cmsCreateInkLimitingDeviceLink
    cmsColorSpaceSignature colorspace = d5 % 2 ? cmsSigCmykData : cmsSigRgbData;
    cmsFloat64Number limit = (d6 % 400) + 1.0;
    cmsHPROFILE ink_limit_profile = cmsCreateInkLimitingDeviceLink(colorspace, limit);
    if (ink_limit_profile) {
        cmsCloseProfile(ink_limit_profile);
    }
    
    // 4. Test cmsCreateInkLimitingDeviceLinkTHR
    cmsHPROFILE ink_limit_profile_thr = cmsCreateInkLimitingDeviceLinkTHR(ctx, colorspace, limit);
    if (ink_limit_profile_thr) {
        cmsCloseProfile(ink_limit_profile_thr);
    }
    
    // 5. Test cmsDetectTAC
    if (profile) {
        cmsFloat64Number tac = cmsDetectTAC(profile);
        (void)tac;
    }
    
    // 6. Test cmsTransform2DeviceLink
    // First create a transform to convert
    transform = cmsCreateTransform(profile, TYPE_RGB_8, profile, TYPE_RGB_8, 
                                   d7 % 4, d8);
    if (transform) {
        cmsFloat64Number version = 4.2;
        cmsHPROFILE devicelink = cmsTransform2DeviceLink(transform, version, d9);
        if (devicelink) {
            cmsCloseProfile(devicelink);
        }
        cmsDeleteTransform(transform);
        transform = NULL;
    }
    
    // Cleanup
    if (profile) {
        cmsCloseProfile(profile);
    }
    if (ctx) {
        cmsDeleteContext(ctx);
    }
    
    return 0;
}