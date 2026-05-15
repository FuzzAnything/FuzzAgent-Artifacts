// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsPipelineAlloc at cmslut.c:1377:24 in lcms2.h
// cmsPipelineFree at cmslut.c:1426:16 in lcms2.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
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

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static cmsUInt32Number read_uint32(const uint8_t** data, size_t* size, cmsUInt32Number default_val) {
    if (*size >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number val;
        memcpy(&val, *data, sizeof(cmsUInt32Number));
        *data += sizeof(cmsUInt32Number);
        *size -= sizeof(cmsUInt32Number);
        return val;
    }
    return default_val;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = create_test_context();
    if (!ctx) return 0;
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    Data++;
    Size--;
    
    // Test cmsPipelineAlloc
    if (selector & 0x01) {
        cmsUInt32Number input_channels = read_uint32(&Data, &Size, 4) % 16 + 1;
        cmsUInt32Number output_channels = read_uint32(&Data, &Size, 4) % 16 + 1;
        
        cmsPipeline* pipeline = cmsPipelineAlloc(ctx, input_channels, output_channels);
        if (pipeline) {
            cmsPipelineFree(pipeline);
        }
    }
    
    // Test _cmsDupMem
    if (selector & 0x02 && Size > 0) {
        size_t copy_size = Size > 1024 ? 1024 : Size;
        void* original_data = malloc(copy_size);
        if (original_data) {
            memcpy(original_data, Data, copy_size);
            
            void* duplicated = _cmsDupMem(ctx, original_data, (cmsUInt32Number)copy_size);
            if (duplicated) {
                _cmsFree(ctx, duplicated);
            }
            free(original_data);
        }
    }
    
    // Test _cmsMallocZero
    if (selector & 0x04) {
        cmsUInt32Number alloc_size = read_uint32(&Data, &Size, 64) % 4096;
        void* zero_mem = _cmsMallocZero(ctx, alloc_size);
        if (zero_mem) {
            _cmsFree(ctx, zero_mem);
        }
    }
    
    // Test _cmsCalloc
    if (selector & 0x08) {
        cmsUInt32Number num_elements = read_uint32(&Data, &Size, 16) % 256;
        cmsUInt32Number element_size = read_uint32(&Data, &Size, 4) % 256;
        void* calloc_mem = _cmsCalloc(ctx, num_elements, element_size);
        if (calloc_mem) {
            _cmsFree(ctx, calloc_mem);
        }
    }
    
    // Test _cmsRealloc
    if (selector & 0x10) {
        cmsUInt32Number initial_size = read_uint32(&Data, &Size, 32) % 1024;
        cmsUInt32Number new_size = read_uint32(&Data, &Size, 64) % 2048;
        
        void* ptr = _cmsMallocZero(ctx, initial_size);
        if (ptr) {
            void* realloc_ptr = _cmsRealloc(ctx, ptr, new_size);
            if (realloc_ptr) {
                _cmsFree(ctx, realloc_ptr);
            } else {
                // If realloc fails, original pointer is still valid
                _cmsFree(ctx, ptr);
            }
        }
    }
    
    // Cleanup
    cmsDeleteContext(ctx);
    
    return 0;
}