// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    // Create a simple sRGB profile to work with
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) return 0;

    // 1. Test cmsSaveProfileToMem
    cmsUInt32Number bytesNeeded = 0;
    
    // First call to get required size
    cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    
    // Second call to actually save if we have enough data
    if (bytesNeeded > 0 && Size > 0) {
        // Limit allocation to reasonable size to avoid OOM
        size_t allocSize = bytesNeeded < 1024 * 1024 ? bytesNeeded : 1024 * 1024;
        void* buffer = malloc(allocSize);
        if (buffer) {
            cmsUInt32Number bytesToWrite = (cmsUInt32Number)(Size < allocSize ? Size : allocSize);
            cmsSaveProfileToMem(hProfile, buffer, &bytesToWrite);
            free(buffer);
        }
    }

    // 2. Test cmsPipelineSetSaveAs8bitsFlag
    // Create a simple pipeline first
    cmsPipeline* lut = cmsPipelineAlloc(0, 3, 3);
    if (lut) {
        // Create a simple tone curve
        cmsToneCurve* curve = cmsBuildGamma(0, 2.2);
        if (curve) {
            // Insert the curve into pipeline
            cmsPipelineInsertStage(lut, cmsAT_END, cmsStageAllocToneCurves(0, 1, &curve));
            
            // Test the flag setting with fuzzer data
            cmsBool flag = Size > 0 ? (Data[0] & 1) : 0;
            cmsPipelineSetSaveAs8bitsFlag(lut, flag);
            
            cmsFreeToneCurve(curve);
        }
        cmsPipelineFree(lut);
    }

    // 3. Test cmsSaveProfileToFile
    const char* filename = "./dummy_file";
    cmsSaveProfileToFile(hProfile, filename);
    // Clean up the file if it was created
    remove(filename);

    // 4. Test cmsLinkTag
    // Use fuzzer data to select tags if we have enough data
    if (Size >= 8) {
        cmsTagSignature srcSig = *((cmsTagSignature*)(Data));
        cmsTagSignature destSig = *((cmsTagSignature*)(Data + 4));
        cmsLinkTag(hProfile, srcSig, destSig);
    } else {
        // Test with some common tags if not enough fuzzer data
        cmsLinkTag(hProfile, cmsSigRedTRCTag, cmsSigGreenTRCTag);
    }

    // 5. Test cmsWriteTag
    // Create some tag data based on fuzzer input
    if (Size >= 12) {
        cmsTagSignature sig = *((cmsTagSignature*)(Data));
        
        // Create a simple tone curve for writing
        cmsToneCurve* writeCurve = cmsBuildGamma(0, 1.8);
        if (writeCurve) {
            cmsWriteTag(hProfile, sig, writeCurve);
            cmsFreeToneCurve(writeCurve);
        }
        
        // Also test deleting a tag
        cmsWriteTag(hProfile, sig, NULL);
    }

    // 6. Test cmsWriteRawTag
    if (Size >= 16) {
        cmsTagSignature sig = *((cmsTagSignature*)(Data + 8));
        cmsUInt32Number dataSize = (cmsUInt32Number)(Size - 12);
        if (dataSize > 0) {
            // Limit size to avoid excessive memory usage
            if (dataSize > 65536) dataSize = 65536;
            cmsWriteRawTag(hProfile, sig, Data + 12, dataSize);
        }
    }

    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}