// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsGetStageContextID at cmslut.c:1238:22 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lcms2.h"
#include "lcms2_plugin.h"

// Dummy functions for placeholder stage with correct signatures
static void dummy_eval(const float* in, float* out, const struct _cmsStage_struct* stage) {
    (void)in; (void)out; (void)stage;
}

static void* dummy_dup(struct _cmsStage_struct* stage) {
    (void)stage;
    return NULL;
}

static void dummy_free(struct _cmsStage_struct* stage) {
    (void)stage;
}

// Helper to read integers from fuzz data
static uint32_t read_uint32(const uint8_t** data, size_t* size) {
    if (*size < 4) return 0;
    uint32_t val = *(const uint32_t*)(*data);
    *data += 4;
    *size -= 4;
    return val;
}

static uint16_t read_uint16(const uint8_t** data, size_t* size) {
    if (*size < 2) return 0;
    uint16_t val = *(const uint16_t*)(*data);
    *data += 2;
    *size -= 2;
    return val;
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    const uint8_t* ptr = Data;
    size_t remaining = Size;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Test cmsStageAllocIdentity
    uint32_t nChannels = read_uint32(&ptr, &remaining) % 16;
    if (nChannels == 0) nChannels = 1;
    
    cmsStage* identity_stage = cmsStageAllocIdentity(ctx, nChannels);
    if (identity_stage) {
        cmsContext stage_ctx = cmsGetStageContextID(identity_stage);
        cmsStageFree(identity_stage);
    }
    
    // Test cmsStageAllocToneCurves
    uint32_t tone_channels = read_uint32(&ptr, &remaining) % 8;
    if (tone_channels == 0) tone_channels = 1;
    
    cmsToneCurve* curves[8];
    for (uint32_t i = 0; i < tone_channels && i < 8; i++) {
        // Create simple parametric tone curve
        double params[10] = {0};
        for (int j = 0; j < 10 && remaining >= 8; j++) {
            params[j] = (double)read_uint32(&ptr, &remaining);
        }
        curves[i] = cmsBuildParametricToneCurve(ctx, 1, params);
    }
    
    if (tone_channels > 0) {
        cmsStage* tone_stage = cmsStageAllocToneCurves(ctx, tone_channels, curves);
        if (tone_stage) {
            cmsStageFree(tone_stage);
        }
    }
    
    // Clean up tone curves
    for (uint32_t i = 0; i < tone_channels && i < 8; i++) {
        if (curves[i]) cmsFreeToneCurve(curves[i]);
    }
    
    // Test _cmsStageAllocPlaceholder
    uint32_t placeholder_type = read_uint32(&ptr, &remaining);
    uint32_t in_channels = read_uint32(&ptr, &remaining) % 8;
    uint32_t out_channels = read_uint32(&ptr, &remaining) % 8;
    if (in_channels == 0) in_channels = 1;
    if (out_channels == 0) out_channels = 1;
    
    void* placeholder_data = NULL;
    if (remaining > 0) {
        placeholder_data = malloc(remaining % 1024);
        if (placeholder_data) {
            size_t copy_size = remaining % 1024;
            if (copy_size > remaining) copy_size = remaining;
            memcpy(placeholder_data, ptr, copy_size);
        }
    }
    
    cmsStage* placeholder_stage = _cmsStageAllocPlaceholder(ctx, 
                                                           (cmsStageSignature)placeholder_type,
                                                           in_channels,
                                                           out_channels,
                                                           dummy_eval,
                                                           dummy_dup,
                                                           dummy_free,
                                                           placeholder_data);
    if (placeholder_stage) {
        cmsStageFree(placeholder_stage);
    } else {
        free(placeholder_data);
    }
    
    // Test cmsStageAllocCLut16bit
    uint32_t grid_points = read_uint32(&ptr, &remaining) % 32;
    if (grid_points < 2) grid_points = 2;
    uint32_t in_chan = read_uint32(&ptr, &remaining) % 8;
    uint32_t out_chan = read_uint32(&ptr, &remaining) % 8;
    if (in_chan == 0) in_chan = 1;
    if (out_chan == 0) out_chan = 1;
    
    // Calculate table size
    size_t table_size = 1;
    for (uint32_t i = 0; i < in_chan; i++) {
        table_size *= grid_points;
    }
    table_size *= out_chan;
    
    cmsUInt16Number* table = NULL;
    if (table_size > 0 && table_size < 1000000) { // Reasonable limit
        table = (cmsUInt16Number*)malloc(table_size * sizeof(cmsUInt16Number));
        if (table) {
            for (size_t i = 0; i < table_size && remaining >= 2; i++) {
                table[i] = read_uint16(&ptr, &remaining);
            }
            
            cmsStage* clut_stage = cmsStageAllocCLut16bit(ctx, grid_points, in_chan, out_chan, table);
            if (clut_stage) {
                cmsStageFree(clut_stage);
            }
            free(table);
        }
    }
    
    // Test cmsStageAllocCLut16bitGranular
    uint32_t granular_in = read_uint32(&ptr, &remaining) % 8;
    uint32_t granular_out = read_uint32(&ptr, &remaining) % 8;
    if (granular_in == 0) granular_in = 1;
    if (granular_out == 0) granular_out = 1;
    
    cmsUInt32Number clut_points[8];
    for (uint32_t i = 0; i < granular_in && i < 8; i++) {
        clut_points[i] = read_uint32(&ptr, &remaining) % 32;
        if (clut_points[i] < 2) clut_points[i] = 2;
    }
    
    // Calculate granular table size
    size_t granular_table_size = 1;
    for (uint32_t i = 0; i < granular_in; i++) {
        granular_table_size *= clut_points[i];
    }
    granular_table_size *= granular_out;
    
    cmsUInt16Number* granular_table = NULL;
    if (granular_table_size > 0 && granular_table_size < 1000000) {
        granular_table = (cmsUInt16Number*)malloc(granular_table_size * sizeof(cmsUInt16Number));
        if (granular_table) {
            for (size_t i = 0; i < granular_table_size && remaining >= 2; i++) {
                granular_table[i] = read_uint16(&ptr, &remaining);
            }
            
            cmsStage* granular_stage = cmsStageAllocCLut16bitGranular(ctx, clut_points, 
                                                                     granular_in, granular_out, 
                                                                     granular_table);
            if (granular_stage) {
                cmsStageFree(granular_stage);
            }
            free(granular_table);
        }
    }
    
    // Cleanup
    cmsDeleteContext(ctx);
    
    return 0;
}