// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    // Create a minimal valid profile as base for testing
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (!hProfile) return 0;

    // Test cmsSaveProfileToMem
    if (Size > 0) {
        cmsUInt32Number bytesNeeded = 0;
        // First call to get required size
        cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
        
        if (bytesNeeded > 0 && bytesNeeded < 1024*1024) { // Reasonable size limit
            void *buffer = malloc(bytesNeeded);
            if (buffer) {
                cmsUInt32Number bytesWritten = bytesNeeded;
                // Try to save with different buffer sizes based on fuzzer input
                if (Size > 4) {
                    cmsUInt32Number modSize = (*(uint32_t*)Data) % bytesNeeded;
                    if (modSize > 0) bytesWritten = modSize;
                }
                cmsSaveProfileToMem(hProfile, buffer, &bytesWritten);
                free(buffer);
            }
        }
    }

    // Test cmsPipelineSetSaveAs8bitsFlag
    if (Size > 8) {
        // Create a simple pipeline
        cmsPipeline* lut = cmsPipelineAlloc(0, 3, 3);
        if (lut) {
            // Use fuzzer data to determine flag value
            cmsBool flag = (Data[0] & 1) ? TRUE : FALSE;
            cmsPipelineSetSaveAs8bitsFlag(lut, flag);
            
            // Test with opposite value
            cmsPipelineSetSaveAs8bitsFlag(lut, !flag);
            
            cmsPipelineFree(lut);
        }
    }

    // Test cmsSaveProfileToFile
    if (Size > 12) {
        const char* filename = "./dummy_file";
        // Write some data to the file first based on fuzzer input
        FILE* f = fopen(filename, "wb");
        if (f) {
            size_t writeSize = Size > 1024 ? 1024 : Size;
            fwrite(Data, 1, writeSize, f);
            fclose(f);
        }
        
        cmsSaveProfileToFile(hProfile, filename);
        // Clean up the dummy file
        remove(filename);
    }

    // Test cmsLinkTag
    if (Size > 16) {
        // Use fuzzer data to generate tag signatures
        cmsTagSignature sig1 = (cmsTagSignature)(*(uint32_t*)(Data));
        cmsTagSignature sig2 = (cmsTagSignature)(*(uint32_t*)(Data + 4));
        cmsLinkTag(hProfile, sig1, sig2);
        
        // Try another pair
        if (Size > 24) {
            cmsTagSignature sig3 = (cmsTagSignature)(*(uint32_t*)(Data + 8));
            cmsTagSignature sig4 = (cmsTagSignature)(*(uint32_t*)(Data + 12));
            cmsLinkTag(hProfile, sig3, sig4);
        }
    }

    // Test cmsWriteTag
    if (Size > 20) {
        cmsTagSignature sig = (cmsTagSignature)(*(uint32_t*)(Data));
        const void* tagData = (Size > 24) ? (const void*)(Data + 4) : NULL;
        
        // Try writing tag with data
        if (tagData) {
            size_t dataSize = Size - 4;
            if (dataSize > 1024) dataSize = 1024;
            cmsWriteTag(hProfile, sig, tagData);
        }
        
        // Try deleting a tag (NULL data)
        if (Size > 28) {
            cmsTagSignature sig2 = (cmsTagSignature)(*(uint32_t*)(Data + 20));
            cmsWriteTag(hProfile, sig2, NULL);
        }
    }

    // Test cmsWriteRawTag
    if (Size > 24) {
        cmsTagSignature sig = (cmsTagSignature)(*(uint32_t*)(Data));
        const void* rawData = Data + 4;
        cmsUInt32Number rawSize = (cmsUInt32Number)(Size - 4);
        
        // Limit size to prevent excessive memory usage
        if (rawSize > 65536) rawSize = 65536;
        
        cmsWriteRawTag(hProfile, sig, rawData, rawSize);
        
        // Try with different signature and size
        if (Size > 32) {
            cmsTagSignature sig2 = (cmsTagSignature)(*(uint32_t*)(Data + 16));
            cmsUInt32Number rawSize2 = (*(uint32_t*)(Data + 20)) % 65536;
            if (rawSize2 > 0 && (Size - 24) >= rawSize2) {
                cmsWriteRawTag(hProfile, sig2, Data + 24, rawSize2);
            }
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}