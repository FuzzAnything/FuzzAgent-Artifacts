// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsStageAllocPlaceholder at cmslut.c:31:21 in lcms2_plugin.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocIdentity at cmslut.c:70:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bit at cmslut.c:607:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
// cmsStageOutputChannels at cmslut.c:1223:28 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLut16bitGranular at cmslut.c:547:21 in lcms2.h
// cmsStageInputChannels at cmslut.c:1218:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void dummy_eval(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage* mpe) {
    (void)mpe;
    for (cmsUInt32Number i = 0; i < 16; ++i) {
        Out[i] = In[i];
    }
}

static void* dummy_dup(cmsStage* mpe) {
    (void)mpe;
    return NULL;
}

static void dummy_free(cmsStage* mpe) {
    (void)mpe;
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsUInt32Number input_channels = (Data[0] % 16) + 1;
    cmsUInt32Number output_channels = (Data[1] % 16) + 1;
    cmsUInt32Number grid_points = (Data[2] % 10) + 2;
    cmsUInt32Number identity_channels = (Data[3] % 16) + 1;
    
    size_t consumed = 4;
    
    cmsStage* placeholder = _cmsStageAllocPlaceholder(ctx, (cmsStageSignature)(Data[0] % 256), 
                                                     input_channels, output_channels,
                                                     dummy_eval, dummy_dup, dummy_free, NULL);
    if (placeholder) {
        cmsUInt32Number in_chans = cmsStageInputChannels(placeholder);
        cmsUInt32Number out_chans = cmsStageOutputChannels(placeholder);
        (void)in_chans;
        (void)out_chans;
        cmsStageFree(placeholder);
    }
    
    cmsStage* identity = cmsStageAllocIdentity(ctx, identity_channels);
    if (identity) {
        cmsUInt32Number in_chans = cmsStageInputChannels(identity);
        cmsUInt32Number out_chans = cmsStageOutputChannels(identity);
        (void)in_chans;
        (void)out_chans;
        cmsStageFree(identity);
    }
    
    size_t table_size = input_channels * output_channels * grid_points;
    cmsUInt16Number* table = NULL;
    if (consumed + table_size * sizeof(cmsUInt16Number) <= Size) {
        table = (cmsUInt16Number*)(Data + consumed);
        consumed += table_size * sizeof(cmsUInt16Number);
        
        cmsStage* clut16 = cmsStageAllocCLut16bit(ctx, grid_points, input_channels, 
                                                 output_channels, table);
        if (clut16) {
            cmsUInt32Number in_chans = cmsStageInputChannels(clut16);
            cmsUInt32Number out_chans = cmsStageOutputChannels(clut16);
            (void)in_chans;
            (void)out_chans;
            cmsStageFree(clut16);
        }
    }
    
    cmsUInt32Number clut_points[16];
    cmsUInt32Number max_input = input_channels < 16 ? input_channels : 16;
    size_t granular_points = 1;
    
    for (cmsUInt32Number i = 0; i < max_input; ++i) {
        if (consumed < Size) {
            clut_points[i] = (Data[consumed] % 10) + 2;
            granular_points *= clut_points[i];
            consumed++;
        } else {
            clut_points[i] = 2;
            granular_points *= 2;
        }
    }
    
    size_t granular_table_size = output_channels * granular_points;
    cmsUInt16Number* granular_table = NULL;
    if (consumed + granular_table_size * sizeof(cmsUInt16Number) <= Size) {
        granular_table = (cmsUInt16Number*)(Data + consumed);
        
        cmsStage* granular_clut = cmsStageAllocCLut16bitGranular(ctx, clut_points, 
                                                                max_input, output_channels, 
                                                                granular_table);
        if (granular_clut) {
            cmsUInt32Number in_chans = cmsStageInputChannels(granular_clut);
            cmsUInt32Number out_chans = cmsStageOutputChannels(granular_clut);
            (void)in_chans;
            (void)out_chans;
            cmsStageFree(granular_clut);
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}