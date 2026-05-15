// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsGetTransformWorkerFlags at cmsxform.c:801:27 in lcms2_plugin.h
// _cmsGetTransformFlags at cmsxform.c:780:27 in lcms2_plugin.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsCreateMultiprofileTransformTHR at cmsxform.c:1281:25 in lcms2.h
// cmsChangeBuffersFormat at cmsxform.c:1441:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

#define MAX_PROFILES 4
#define MAX_BUFFER_SIZE 4096

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0},
        {0.30, 0.60, 1.0},
        {0.15, 0.06, 1.0}
    };
    
    return cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, NULL);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    // Create dummy profiles for testing
    cmsHPROFILE profiles[MAX_PROFILES];
    for (int i = 0; i < MAX_PROFILES; i++) {
        profiles[i] = create_dummy_profile(ctx);
    }
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // For cmsChangeBuffersFormat
    cmsUInt32Number inputFormat = 0;
    cmsUInt32Number outputFormat = 0;
    if (offset + 8 <= Size) {
        memcpy(&inputFormat, Data + offset, 4);
        offset += 4;
        memcpy(&outputFormat, Data + offset, 4);
        offset += 4;
    }
    
    // For transform creation
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    if (offset + 8 <= Size) {
        memcpy(&intent, Data + offset, 4);
        offset += 4;
        memcpy(&flags, Data + offset, 4);
        offset += 4;
    }
    
    // Test cmsCreateTransformTHR
    cmsHTRANSFORM transform1 = cmsCreateTransformTHR(ctx,
                                                    profiles[0],
                                                    inputFormat,
                                                    profiles[1],
                                                    outputFormat,
                                                    intent,
                                                    flags);
    
    // Test cmsCreateMultiprofileTransformTHR
    cmsUInt32Number nProfiles = 2;
    if (Size >= offset + 4) {
        memcpy(&nProfiles, Data + offset, 4);
        offset += 4;
        nProfiles = nProfiles % (MAX_PROFILES + 1);
    }
    
    cmsHTRANSFORM transform2 = cmsCreateMultiprofileTransformTHR(ctx,
                                                                profiles,
                                                                nProfiles,
                                                                inputFormat,
                                                                outputFormat,
                                                                intent,
                                                                flags);
    
    // Test cmsChangeBuffersFormat
    if (transform1) {
        cmsChangeBuffersFormat(transform1, inputFormat, outputFormat);
    }
    
    // Test cmsDoTransformLineStride
    if (transform1) {
        uint8_t inputBuffer[MAX_BUFFER_SIZE];
        uint8_t outputBuffer[MAX_BUFFER_SIZE];
        
        size_t bufferSize = Size - offset;
        if (bufferSize > MAX_BUFFER_SIZE) bufferSize = MAX_BUFFER_SIZE;
        
        if (bufferSize > 0 && offset < Size) {
            memcpy(inputBuffer, Data + offset, bufferSize);
            
            cmsUInt32Number pixelsPerLine = 1;
            cmsUInt32Number lineCount = 1;
            cmsUInt32Number bytesPerLineIn = bufferSize;
            cmsUInt32Number bytesPerLineOut = bufferSize;
            cmsUInt32Number bytesPerPlaneIn = bufferSize;
            cmsUInt32Number bytesPerPlaneOut = bufferSize;
            
            if (Size >= offset + 24) {
                memcpy(&pixelsPerLine, Data + offset, 4);
                offset += 4;
                memcpy(&lineCount, Data + offset, 4);
                offset += 4;
                memcpy(&bytesPerLineIn, Data + offset, 4);
                offset += 4;
                memcpy(&bytesPerLineOut, Data + offset, 4);
                offset += 4;
                memcpy(&bytesPerPlaneIn, Data + offset, 4);
                offset += 4;
                memcpy(&bytesPerPlaneOut, Data + offset, 4);
                offset += 4;
                
                // Ensure reasonable values to avoid crashes
                if (pixelsPerLine > 1000) pixelsPerLine = 1000;
                if (lineCount > 1000) lineCount = 1000;
                if (bytesPerLineIn > MAX_BUFFER_SIZE) bytesPerLineIn = MAX_BUFFER_SIZE;
                if (bytesPerLineOut > MAX_BUFFER_SIZE) bytesPerLineOut = MAX_BUFFER_SIZE;
                if (bytesPerPlaneIn > MAX_BUFFER_SIZE) bytesPerPlaneIn = MAX_BUFFER_SIZE;
                if (bytesPerPlaneOut > MAX_BUFFER_SIZE) bytesPerPlaneOut = MAX_BUFFER_SIZE;
            }
            
            cmsDoTransformLineStride(transform1,
                                    inputBuffer,
                                    outputBuffer,
                                    pixelsPerLine,
                                    lineCount,
                                    bytesPerLineIn,
                                    bytesPerLineOut,
                                    bytesPerPlaneIn,
                                    bytesPerPlaneOut);
        }
    }
    
    // Test internal functions if transform exists
    if (transform1) {
        struct _cmstransform_struct* transform_struct = (struct _cmstransform_struct*)transform1;
        _cmsGetTransformWorkerFlags(transform_struct);
        _cmsGetTransformFlags(transform_struct);
    }
    
    // Cleanup
    if (transform1) cmsDeleteTransform(transform1);
    if (transform2) cmsDeleteTransform(transform2);
    
    for (int i = 0; i < MAX_PROFILES; i++) {
        if (profiles[i]) cmsCloseProfile(profiles[i]);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}