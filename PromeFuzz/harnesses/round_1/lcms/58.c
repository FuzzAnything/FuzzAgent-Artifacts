// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateMultiprofileTransform at cmsxform.c:1311:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateProofingTransform at cmsxform.c:1393:25 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformInputFormat at cmsxform.c:1424:27 in lcms2.h
// cmsGetTransformOutputFormat at cmsxform.c:1432:27 in lcms2.h
// cmsDoTransformLineStride at cmsxform.c:229:16 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t readUint32(const uint8_t* data, size_t size, size_t* offset) {
    if (*offset + 4 > size) return 0;
    uint32_t value;
    memcpy(&value, data + *offset, 4);
    *offset += 4;
    return value;
}

static cmsHPROFILE createDummyProfile(void) {
    // Create a simple sRGB profile as a dummy profile
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_58(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;
    
    size_t offset = 0;
    
    // Create dummy profiles for testing
    cmsHPROFILE hProfile1 = createDummyProfile();
    cmsHPROFILE hProfile2 = createDummyProfile();
    cmsHPROFILE hProfile3 = createDummyProfile();
    
    if (!hProfile1 || !hProfile2 || !hProfile3) {
        if (hProfile1) cmsCloseProfile(hProfile1);
        if (hProfile2) cmsCloseProfile(hProfile2);
        if (hProfile3) cmsCloseProfile(hProfile3);
        return 0;
    }
    
    // Test cmsCreateTransform
    uint32_t InputFormat = readUint32(Data, Size, &offset);
    uint32_t OutputFormat = readUint32(Data, Size, &offset);
    uint32_t Intent = readUint32(Data, Size, &offset) % 4;
    uint32_t dwFlags = readUint32(Data, Size, &offset);
    
    cmsHTRANSFORM hTransform = cmsCreateTransform(hProfile1, InputFormat,
                                                  hProfile2, OutputFormat,
                                                  Intent, dwFlags);
    
    if (hTransform) {
        // Test cmsGetTransformInputFormat and cmsGetTransformOutputFormat
        uint32_t inFmt = cmsGetTransformInputFormat(hTransform);
        uint32_t outFmt = cmsGetTransformOutputFormat(hTransform);
        
        // Test cmsDoTransformLineStride
        uint32_t PixelsPerLine = readUint32(Data, Size, &offset) % 256;
        uint32_t LineCount = readUint32(Data, Size, &offset) % 256;
        uint32_t BytesPerLineIn = readUint32(Data, Size, &offset) % 1024;
        uint32_t BytesPerLineOut = readUint32(Data, Size, &offset) % 1024;
        uint32_t BytesPerPlaneIn = readUint32(Data, Size, &offset) % 1024;
        uint32_t BytesPerPlaneOut = readUint32(Data, Size, &offset) % 1024;
        
        if (PixelsPerLine > 0 && LineCount > 0) {
            size_t inputSize = BytesPerLineIn * LineCount;
            size_t outputSize = BytesPerLineOut * LineCount;
            
            if (inputSize > 0 && outputSize > 0 && inputSize < 65536 && outputSize < 65536) {
                void* InputBuffer = malloc(inputSize);
                void* OutputBuffer = malloc(outputSize);
                
                if (InputBuffer && OutputBuffer) {
                    // Fill input buffer with fuzzer data
                    size_t copySize = Size - offset;
                    if (copySize > inputSize) copySize = inputSize;
                    if (copySize > 0) {
                        memcpy(InputBuffer, Data + offset, copySize);
                    }
                    
                    cmsDoTransformLineStride(hTransform, InputBuffer, OutputBuffer,
                                            PixelsPerLine, LineCount,
                                            BytesPerLineIn, BytesPerLineOut,
                                            BytesPerPlaneIn, BytesPerPlaneOut);
                    
                    free(InputBuffer);
                    free(OutputBuffer);
                }
            }
        }
        
        cmsDeleteTransform(hTransform);
    }
    
    // Test cmsCreateMultiprofileTransform
    uint32_t nProfiles = readUint32(Data, Size, &offset) % 255;
    if (nProfiles == 0) nProfiles = 1;
    
    cmsHPROFILE* profiles = malloc(sizeof(cmsHPROFILE) * nProfiles);
    if (profiles) {
        for (uint32_t i = 0; i < nProfiles; i++) {
            profiles[i] = (i % 3 == 0) ? hProfile1 : 
                         (i % 3 == 1) ? hProfile2 : hProfile3;
        }
        
        InputFormat = readUint32(Data, Size, &offset);
        OutputFormat = readUint32(Data, Size, &offset);
        Intent = readUint32(Data, Size, &offset) % 4;
        dwFlags = readUint32(Data, Size, &offset);
        
        hTransform = cmsCreateMultiprofileTransform(profiles, nProfiles,
                                                   InputFormat, OutputFormat,
                                                   Intent, dwFlags);
        
        if (hTransform) {
            cmsGetTransformInputFormat(hTransform);
            cmsGetTransformOutputFormat(hTransform);
            cmsDeleteTransform(hTransform);
        }
        
        free(profiles);
    }
    
    // Test cmsCreateProofingTransform
    uint32_t ProofingIntent = readUint32(Data, Size, &offset) % 4;
    
    hTransform = cmsCreateProofingTransform(hProfile1, InputFormat,
                                           hProfile2, OutputFormat,
                                           hProfile3,
                                           Intent, ProofingIntent,
                                           dwFlags);
    
    if (hTransform) {
        cmsGetTransformInputFormat(hTransform);
        cmsGetTransformOutputFormat(hTransform);
        cmsDeleteTransform(hTransform);
    }
    
    // Cleanup
    cmsCloseProfile(hProfile1);
    cmsCloseProfile(hProfile2);
    cmsCloseProfile(hProfile3);
    
    return 0;
}