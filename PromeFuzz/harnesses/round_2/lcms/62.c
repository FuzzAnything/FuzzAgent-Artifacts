// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsFormatterForPCSOfProfile at cmspack.c:4044:27 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsTransform2DeviceLink at cmsvirt.c:1194:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsDefaultICCintents at cmscnvrt.c:655:25 in lcms2_plugin.h
// cmsCreateExtendedTransform at cmsxform.c:1123:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    cmsToneCurve* curve = cmsBuildGamma(ctx, 2.2);
    if (!curve) return NULL;
    
    cmsHPROFILE profile = cmsCreateRGBProfileTHR(ctx,
        cmsD50_xyY(),
        &(cmsCIExyYTRIPLE){{0.64, 0.33, 1.0}, {0.3, 0.6, 1.0}, {0.15, 0.06, 1.0}},
        (cmsToneCurve*[3]){curve, curve, curve}
    );
    
    cmsFreeToneCurve(curve);
    return profile;
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) cmsCloseProfile(profile);
}

static void cleanup_transform(cmsHTRANSFORM transform) {
    if (transform) cmsDeleteTransform(transform);
}

static void cleanup_pipeline(cmsPipeline* pipeline) {
    if (pipeline) cmsPipelineFree(pipeline);
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy profiles for testing
    cmsHPROFILE profiles[4] = {0};
    for (int i = 0; i < 4; i++) {
        profiles[i] = create_dummy_profile(ctx);
        if (!profiles[i]) {
            for (int j = 0; j < i; j++) cleanup_profile(profiles[j]);
            cmsDeleteContext(ctx);
            return 0;
        }
    }
    
    // Parse parameters from fuzzer input
    size_t offset = 0;
    
    // For _cmsDefaultICCintents
    cmsUInt32Number nProfiles = (Data[offset++] % 3) + 1;
    cmsUInt32Number intents[4];
    cmsBool bpc[4];
    cmsFloat64Number adaptation[4];
    
    for (cmsUInt32Number i = 0; i < nProfiles && offset < Size; i++) {
        intents[i] = Data[offset++] % 4;
        bpc[i] = Data[offset++] % 2;
        if (offset + 8 <= Size) {
            memcpy(&adaptation[i], Data + offset, 8);
            offset += 8;
        } else {
            adaptation[i] = 1.0;
        }
    }
    cmsUInt32Number flags1 = (offset + 4 <= Size) ? *(cmsUInt32Number*)(Data + offset) : 0;
    offset = (offset + 4 < Size) ? offset + 4 : Size;
    
    cmsPipeline* pipeline = _cmsDefaultICCintents(ctx, nProfiles, intents, profiles, bpc, adaptation, flags1);
    cleanup_pipeline(pipeline);
    
    // For cmsCreateExtendedTransform
    cmsUInt32Number inputFormat = (offset + 4 <= Size) ? *(cmsUInt32Number*)(Data + offset) : TYPE_RGB_8;
    offset = (offset + 4 < Size) ? offset + 4 : Size;
    cmsUInt32Number outputFormat = (offset + 4 <= Size) ? *(cmsUInt32Number*)(Data + offset) : TYPE_RGB_8;
    offset = (offset + 4 < Size) ? offset + 4 : Size;
    cmsUInt32Number flags2 = (offset + 4 <= Size) ? *(cmsUInt32Number*)(Data + offset) : 0;
    offset = (offset + 4 < Size) ? offset + 4 : Size;
    
    cmsHTRANSFORM transform1 = cmsCreateExtendedTransform(ctx, nProfiles, profiles, bpc, intents, adaptation,
                                                          NULL, 0, inputFormat, outputFormat, flags2);
    cleanup_transform(transform1);
    
    // For cmsCreateMultiprofileTransformTHR
    cmsUInt32Number intent3 = (offset < Size) ? Data[offset++] % 4 : 0;
    cmsUInt32Number flags3 = (offset + 4 <= Size) ? *(cmsUInt32Number*)(Data + offset) : 0;
    offset = (offset + 4 < Size) ? offset + 4 : Size;
    
    cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransformTHR(ctx, profiles, nProfiles, 
                                                                 inputFormat, outputFormat, intent3, flags3);
    cleanup_transform(transform2);
    
    // For cmsCreateProofingTransform
    if (nProfiles >= 3) {
        cmsUInt32Number proofingIntent = (offset < Size) ? Data[offset++] % 4 : 0;
        cmsUInt32Number flags4 = (offset + 4 <= Size) ? *(cmsUInt32Number*)(Data + offset) : 0;
        offset = (offset + 4 < Size) ? offset + 4 : Size;
        
        cmsHTRANSFORM transform3 = cmsCreateProofingTransform(profiles[0], inputFormat,
                                                             profiles[1], outputFormat,
                                                             profiles[2], intent3, proofingIntent, flags4);
        cleanup_transform(transform3);
    }
    
    // For cmsFormatterForPCSOfProfile
    cmsUInt32Number nBytes = (offset < Size) ? (Data[offset++] % 4) + 1 : 1;
    cmsBool isFloat = (offset < Size) ? Data[offset++] % 2 : 0;
    
    cmsUInt32Number formatter = cmsFormatterForPCSOfProfile(profiles[0], nBytes, isFloat);
    (void)formatter; // Suppress unused variable warning
    
    // For cmsTransform2DeviceLink - need a transform first
    cmsHTRANSFORM tempTransform = cmsCreateMultiprofileTransformTHR(ctx, profiles, nProfiles, 
                                                                   inputFormat, outputFormat, intent3, flags3);
    if (tempTransform) {
        cmsFloat64Number version = (offset + 8 <= Size) ? *(cmsFloat64Number*)(Data + offset) : 4.0;
        offset = (offset + 8 < Size) ? offset + 8 : Size;
        cmsUInt32Number flags5 = (offset + 4 <= Size) ? *(cmsUInt32Number*)(Data + offset) : 0;
        
        cmsHPROFILE devicelink = cmsTransform2DeviceLink(tempTransform, version, flags5);
        cleanup_profile(devicelink);
        cleanup_transform(tempTransform);
    }
    
    // Cleanup
    for (int i = 0; i < 4; i++) {
        cleanup_profile(profiles[i]);
    }
    cmsDeleteContext(ctx);
    
    return 0;
}