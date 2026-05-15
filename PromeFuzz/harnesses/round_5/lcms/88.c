// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfileTHR at cmsvirt.c:653:23 in lcms2.h
// cmsCreateInkLimitingDeviceLinkTHR at cmsvirt.c:393:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsOpenIOhandlerFromStream at cmsio0.c:477:25 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsCreateInkLimitingDeviceLink at cmsvirt.c:466:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create a dummy profile from fuzzer data
    cmsHPROFILE dummy_profile = cmsOpenProfileFromMem(Data, Size);
    if (!dummy_profile) {
        // If we can't create from fuzzer data, use a built-in profile
        dummy_profile = cmsCreate_sRGBProfileTHR(ctx);
    }
    
    // Test cmsCreateInkLimitingDeviceLinkTHR
    if (Size >= 8) {
        cmsUInt32Number color_space_val = *(cmsUInt32Number*)Data;
        cmsFloat64Number limit = (Data[4] % 400) + 1.0;
        cmsHPROFILE ink_limit_profile = cmsCreateInkLimitingDeviceLinkTHR(
            ctx, (cmsColorSpaceSignature)color_space_val, limit);
        if (ink_limit_profile) {
            cmsCloseProfile(ink_limit_profile);
        }
    }
    
    // Test cmsCreateMultiprofileTransformTHR
    if (dummy_profile) {
        cmsHPROFILE profiles[] = {dummy_profile, dummy_profile};
        cmsUInt32Number input_format = Size >= 12 ? *(cmsUInt32Number*)(Data + 8) : 0;
        cmsUInt32Number output_format = Size >= 16 ? *(cmsUInt32Number*)(Data + 12) : 0;
        cmsUInt32Number intent = Size >= 20 ? *(cmsUInt32Number*)(Data + 16) : 0;
        cmsUInt32Number flags = Size >= 24 ? *(cmsUInt32Number*)(Data + 20) : 0;
        
        cmsHTRANSFORM transform = cmsCreateMultiprofileTransformTHR(
            ctx, profiles, 2, input_format, output_format, intent, flags);
        
        // Test cmsTransform2DeviceLink if transform was created
        if (transform) {
            cmsFloat64Number version = Size >= 28 ? (Data[24] % 5) + 1.0 : 4.0;
            cmsUInt32Number transform_flags = Size >= 32 ? *(cmsUInt32Number*)(Data + 28) : 0;
            
            cmsHPROFILE devicelink = cmsTransform2DeviceLink(transform, version, transform_flags);
            if (devicelink) {
                cmsCloseProfile(devicelink);
            }
            cmsDeleteTransform(transform);
        }
    }
    
    // Test cmsGetPostScriptColorResource
    if (dummy_profile) {
        // Create a file for I/O handler
        FILE* dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file) {
            cmsIOHANDLER* io = cmsOpenIOhandlerFromStream(ctx, dummy_file);
            if (io) {
                cmsUInt32Number type = Size >= 36 ? *(cmsUInt32Number*)(Data + 32) : 0;
                cmsUInt32Number intent = Size >= 40 ? *(cmsUInt32Number*)(Data + 36) : 0;
                cmsUInt32Number flags = Size >= 44 ? *(cmsUInt32Number*)(Data + 40) : 0;
                
                cmsGetPostScriptColorResource(ctx, (cmsPSResourceType)type, 
                                             dummy_profile, intent, flags, io);
                cmsCloseIOhandler(io);
            }
            fclose(dummy_file);
            remove("./dummy_file");
        }
    }
    
    // Test cmsDetectTAC
    if (dummy_profile) {
        cmsFloat64Number tac = cmsDetectTAC(dummy_profile);
        (void)tac; // Use result to avoid unused variable warning
    }
    
    // Test cmsCreateInkLimitingDeviceLink (non-THR version)
    if (Size >= 48) {
        cmsUInt32Number color_space_val = *(cmsUInt32Number*)(Data + 44);
        cmsFloat64Number limit = (Data[48] % 400) + 1.0;
        cmsHPROFILE ink_limit_profile2 = cmsCreateInkLimitingDeviceLink(
            (cmsColorSpaceSignature)color_space_val, limit);
        if (ink_limit_profile2) {
            cmsCloseProfile(ink_limit_profile2);
        }
    }
    
    // Cleanup
    if (dummy_profile) {
        cmsCloseProfile(dummy_profile);
    }
    cmsDeleteContext(ctx);
    
    return 0;
}