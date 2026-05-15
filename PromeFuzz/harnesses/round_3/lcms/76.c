// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsStageAllocMatrix at cmslut.c:379:22 in lcms2.h
// cmsPipelineInsertStage at cmslut.c:1519:15 in lcms2.h
// cmsPipelineSetSaveAs8bitsFlag at cmslut.c:1637:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static cmsHPROFILE create_test_profile(void) {
    // Create a simple sRGB profile for testing
    return cmsCreate_sRGBProfile();
}

static cmsPipeline* create_test_pipeline(void) {
    // Create a simple pipeline with 3 input and 3 output channels
    cmsPipeline* pipeline = cmsPipelineAlloc(NULL, 3, 3);
    if (pipeline) {
        // Add a simple matrix stage
        double identity[9] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
        cmsStage* stage = cmsStageAllocMatrix(NULL, 3, 3, identity, NULL);
        if (stage) {
            cmsPipelineInsertStage(pipeline, cmsAT_END, stage);
        }
    }
    return pipeline;
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create test profile and pipeline
    cmsHPROFILE hProfile = create_test_profile();
    if (!hProfile) return 0;
    
    cmsPipeline* pipeline = create_test_pipeline();
    
    // Use first byte as selector for different code paths
    uint8_t selector = Data[0];
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Test cmsPipelineSetSaveAs8bitsFlag
    if (pipeline) {
        cmsBool prev_flag = cmsPipelineSetSaveAs8bitsFlag(pipeline, selector & 1);
        (void)prev_flag; // Use result to avoid unused variable warning
    }
    
    // Test cmsSaveProfileToMem
    if (fuzz_size > 0) {
        cmsUInt32Number bytes_needed = 0;
        
        // First call to get required size
        cmsSaveProfileToMem(hProfile, NULL, &bytes_needed);
        
        if (bytes_needed > 0 && bytes_needed < 1024 * 1024) { // Limit to 1MB
            void* buffer = malloc(bytes_needed);
            if (buffer) {
                cmsUInt32Number bytes_to_write = bytes_needed;
                if (fuzz_size > 4) {
                    // Use first 4 bytes of fuzz data as byte count
                    bytes_to_write = *(cmsUInt32Number*)fuzz_data % bytes_needed;
                }
                cmsSaveProfileToMem(hProfile, buffer, &bytes_to_write);
                free(buffer);
            }
        }
    }
    
    // Test cmsSaveProfileToFile
    const char* filename = "./dummy_file";
    cmsSaveProfileToFile(hProfile, filename);
    // Clean up the file
    remove(filename);
    
    // Test cmsWriteRawTag
    if (fuzz_size > 8) {
        // Use first 4 bytes as tag signature, next 4 as size
        cmsTagSignature sig = *(cmsTagSignature*)fuzz_data;
        cmsUInt32Number tag_size = *(cmsUInt32Number*)(fuzz_data + 4);
        
        // Limit tag size to avoid excessive memory usage
        if (tag_size > 0 && tag_size < 65536 && tag_size <= fuzz_size - 8) {
            const void* tag_data = fuzz_data + 8;
            cmsWriteRawTag(hProfile, sig, tag_data, tag_size);
        }
    }
    
    // Test cmsWriteTag
    if (fuzz_size > 4) {
        cmsTagSignature sig = *(cmsTagSignature*)fuzz_data;
        
        // Use NULL to delete tag or use fuzz data as tag data
        if (selector & 2) {
            // Delete tag
            cmsWriteTag(hProfile, sig, NULL);
        } else if (fuzz_size > 8) {
            // Write tag with data (limited to 256 bytes)
            size_t data_size = fuzz_size - 4;
            if (data_size > 256) data_size = 256;
            cmsWriteTag(hProfile, sig, fuzz_data + 4);
        }
    }
    
    // Test cmsLinkTag
    if (fuzz_size > 8) {
        cmsTagSignature src_sig = *(cmsTagSignature*)fuzz_data;
        cmsTagSignature dest_sig = *(cmsTagSignature*)(fuzz_data + 4);
        cmsLinkTag(hProfile, src_sig, dest_sig);
    }
    
    // Cleanup
    if (pipeline) {
        cmsPipelineFree(pipeline);
    }
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}