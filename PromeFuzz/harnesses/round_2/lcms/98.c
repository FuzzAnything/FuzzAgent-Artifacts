// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback functions for samplers */
static int dummy_sampler_float(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo) {
    /* Do nothing */
    return 1;
}

static int dummy_sampler_16(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    /* Do nothing */
    return 1;
}

/* Fuzz driver entry point */
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    /* Ensure minimum input size for basic parameters */
    if (Size < 4) return 0;

    /* Create a context */
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    /* Extract parameters from fuzzer input */
    cmsUInt32Number nInputs = (Data[0] % 4) + 1;  /* 1-4 input channels */
    cmsUInt32Number outputChan = (Data[1] % 4) + 1; /* 1-4 output channels */
    cmsUInt32Number nGridPoints = (Data[2] % 16) + 2; /* 2-17 grid points */
    cmsUInt32Number flags = Data[3];

    /* Allocate arrays for clutPoints */
    cmsUInt32Number clutPoints[4];
    for (cmsUInt32Number i = 0; i < nInputs; i++) {
        clutPoints[i] = nGridPoints;
    }

    /* Test cmsSliceSpaceFloat */
    if (nInputs <= 4) {
        cmsSliceSpaceFloat(nInputs, clutPoints, dummy_sampler_float, NULL);
    }

    /* Test cmsSliceSpace16 */
    if (nInputs <= 4) {
        cmsSliceSpace16(nInputs, clutPoints, dummy_sampler_16, NULL);
    }

    /* Test cmsStageAllocCLutFloatGranular */
    cmsStage* stage1 = cmsStageAllocCLutFloatGranular(ctx, clutPoints, nInputs, outputChan, NULL);
    if (stage1) {
        /* Test cmsStageSampleCLutFloat */
        cmsStageSampleCLutFloat(stage1, dummy_sampler_float, NULL, flags);
        cmsStageFree(stage1);
    }

    /* Test cmsStageAllocCLutFloat */
    cmsStage* stage2 = cmsStageAllocCLutFloat(ctx, nGridPoints, nInputs, outputChan, NULL);
    if (stage2) {
        /* Test cmsStageSampleCLut16bit */
        cmsStageSampleCLut16bit(stage2, dummy_sampler_16, NULL, flags);
        cmsStageFree(stage2);
    }

    /* Cleanup */
    cmsDeleteContext(ctx);
    return 0;
}