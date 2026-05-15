// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Simple callback functions for sampling with correct return types */
static int Sample16bitCallback(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    /* Copy input to output with some simple transformation */
    for (int i = 0; i < 16; i++) {
        if (i < 4) { /* Limit to reasonable channels */
            Out[i] = In[i] ^ 0x7FFF; /* Simple transformation */
        } else {
            Out[i] = 0;
        }
    }
    return 0; /* Return 0 to continue */
}

static int SampleFloatCallback(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo) {
    /* Copy input to output with some simple transformation */
    for (int i = 0; i < 16; i++) {
        if (i < 4) { /* Limit to reasonable channels */
            Out[i] = In[i] * 0.5f; /* Simple transformation */
        } else {
            Out[i] = 0.0f;
        }
    }
    return 0; /* Return 0 to continue */
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    /* Create context */
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }

    /* Parse parameters from fuzzer input with bounds checking */
    if (Size < 16) {
        cmsDeleteContext(context);
        return 0;
    }

    /* Extract parameters with reasonable bounds */
    cmsUInt32Number nGridPoints = (Data[0] % 16) + 2; /* 2-17 grid points */
    cmsUInt32Number inputChan = (Data[1] % 8) + 1;    /* 1-8 input channels */
    cmsUInt32Number outputChan = (Data[2] % 8) + 1;   /* 1-8 output channels */
    cmsUInt32Number flags = Data[3] & 0x0F;           /* Limited flags */

    /* Calculate table size for uniform CLUT */
    size_t tableSize = 1;
    for (cmsUInt32Number i = 0; i < inputChan; i++) {
        tableSize *= nGridPoints;
        if (tableSize > 1024 * 1024) { /* Limit to prevent OOM */
            tableSize = 1024 * 1024;
            break;
        }
    }
    tableSize *= outputChan;

    /* Allocate and initialize table */
    cmsFloat32Number* table = (cmsFloat32Number*)malloc(tableSize * sizeof(cmsFloat32Number));
    if (!table) {
        cmsDeleteContext(context);
        return 0;
    }

    /* Fill table with data from fuzzer input */
    size_t dataToCopy = Size - 4;
    if (dataToCopy > tableSize * sizeof(cmsFloat32Number)) {
        dataToCopy = tableSize * sizeof(cmsFloat32Number);
    }
    memcpy(table, Data + 4, dataToCopy);

    /* Test cmsStageAllocCLutFloat */
    cmsStage* stage1 = cmsStageAllocCLutFloat(context, nGridPoints, inputChan, outputChan, table);
    if (stage1) {
        /* Test cmsStageSampleCLut16bit */
        cmsStageSampleCLut16bit(stage1, Sample16bitCallback, NULL, flags);
        
        /* Test cmsStageSampleCLutFloat */
        cmsStageSampleCLutFloat(stage1, SampleFloatCallback, NULL, flags);
        
        cmsStageFree(stage1);
    }

    /* Prepare granular CLUT points array */
    cmsUInt32Number* clutPoints = (cmsUInt32Number*)malloc(inputChan * sizeof(cmsUInt32Number));
    if (clutPoints) {
        for (cmsUInt32Number i = 0; i < inputChan; i++) {
            clutPoints[i] = (Data[(i + 4) % Size] % 8) + 2; /* 2-9 points per channel */
        }

        /* Test cmsStageAllocCLutFloatGranular */
        cmsStage* stage2 = cmsStageAllocCLutFloatGranular(context, clutPoints, inputChan, outputChan, table);
        if (stage2) {
            cmsStageFree(stage2);
        }

        /* Test cmsSliceSpaceFloat */
        cmsSliceSpaceFloat(inputChan, clutPoints, SampleFloatCallback, NULL);

        /* Test cmsSliceSpace16 */
        cmsSliceSpace16(inputChan, clutPoints, Sample16bitCallback, NULL);

        free(clutPoints);
    }

    /* Cleanup */
    free(table);
    cmsDeleteContext(context);
    
    return 0;
}