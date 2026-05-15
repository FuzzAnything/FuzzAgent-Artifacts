// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

// Dummy functions for placeholder stage with correct signatures
static void dummy_eval(const cmsFloat32Number* In, cmsFloat32Number* Out, const cmsStage* mpe) {
    (void)In;
    (void)Out;
    (void)mpe;
    // Simple identity transform - we can't access mpe internals directly
    // but the library will handle this appropriately
}

static void* dummy_dup(cmsStage* mpe) {
    (void)mpe;
    return NULL;
}

static void dummy_free(cmsStage* mpe) {
    (void)mpe;
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    // Create context
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    // Extract parameters from fuzzer input
    cmsUInt32Number param1 = *(cmsUInt32Number*)Data;
    cmsUInt32Number param2 = *(cmsUInt32Number*)(Data + 4);
    
    // Bound parameters to reasonable values to avoid excessive memory allocation
    param1 = param1 % 16 + 1;  // 1-16 channels
    param2 = param2 % 16 + 1;  // 1-16 channels
    
    // Test 1: _cmsStageAllocPlaceholder
    cmsStage* placeholder = _cmsStageAllocPlaceholder(
        context,
        (cmsStageSignature)(param1 % 256),
        param1,
        param2,
        dummy_eval,
        dummy_dup,
        dummy_free,
        NULL
    );
    
    if (placeholder) {
        // Test 2 & 3: Query input/output channels
        cmsUInt32Number in_channels = cmsStageInputChannels(placeholder);
        cmsUInt32Number out_channels = cmsStageOutputChannels(placeholder);
        (void)in_channels;
        (void)out_channels;
        
        cmsStageFree(placeholder);
    }
    
    // Test 4: cmsStageAllocIdentity
    cmsStage* identity = cmsStageAllocIdentity(context, param1);
    if (identity) {
        cmsStageFree(identity);
    }
    
    // Test 5: cmsStageAllocCLut16bit
    // Calculate grid points from input (bounded to 2-10)
    cmsUInt32Number grid_points = (param1 % 9) + 2;
    
    // Use remaining fuzzer data for CLUT table if available
    const cmsUInt16Number* table = NULL;
    size_t table_size = grid_points * param1 * param2;
    
    if (Size >= 8 + table_size * sizeof(cmsUInt16Number)) {
        table = (const cmsUInt16Number*)(Data + 8);
    }
    
    cmsStage* clut16 = cmsStageAllocCLut16bit(
        context,
        grid_points,
        param1,
        param2,
        table
    );
    
    if (clut16) {
        cmsStageFree(clut16);
    }
    
    // Test 6: cmsStageAllocCLut16bitGranular
    // Allocate array for granular grid points
    cmsUInt32Number* clut_points = (cmsUInt32Number*)malloc(param1 * sizeof(cmsUInt32Number));
    if (clut_points) {
        for (cmsUInt32Number i = 0; i < param1; i++) {
            clut_points[i] = ((param1 + i) % 9) + 2;  // 2-10 grid points
        }
        
        // Calculate total table size for granular CLUT
        cmsUInt32Number total_points = 1;
        for (cmsUInt32Number i = 0; i < param1; i++) {
            total_points *= clut_points[i];
        }
        
        const cmsUInt16Number* granular_table = NULL;
        size_t granular_table_size = total_points * param2;
        
        if (Size >= 8 + granular_table_size * sizeof(cmsUInt16Number)) {
            granular_table = (const cmsUInt16Number*)(Data + 8);
        }
        
        cmsStage* granular_clut = cmsStageAllocCLut16bitGranular(
            context,
            clut_points,
            param1,
            param2,
            granular_table
        );
        
        if (granular_clut) {
            cmsStageFree(granular_clut);
        }
        
        free(clut_points);
    }
    
    // Cleanup
    cmsDeleteContext(context);
    
    return 0;
}