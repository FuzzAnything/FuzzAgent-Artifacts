// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsStageAllocCLutFloatGranular at cmslut.c:642:21 in lcms2.h
// cmsStageSampleCLutFloat at cmslut.c:813:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsStageAllocCLutFloat at cmslut.c:624:21 in lcms2.h
// cmsStageSampleCLut16bit at cmslut.c:747:19 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsSliceSpaceFloat at cmslut.c:910:26 in lcms2.h
// cmsSliceSpace16 at cmslut.c:879:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static int DummySampler16(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo) {
    // Return 0 to continue, 1 to stop
    return 0;
}

static int DummySamplerFloat(const cmsFloat32Number In[], cmsFloat32Number Out[], void* Cargo) {
    // Return 0 to continue, 1 to stop
    return 0;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // For clutPoints arrays - limit to reasonable sizes
    cmsUInt32Number clutPoints[4];
    cmsUInt32Number nInputs = (Data[offset++] % 4) + 1;
    if (nInputs > 4) nInputs = 4;
    
    for (cmsUInt32Number i = 0; i < nInputs && offset < Size; i++) {
        clutPoints[i] = (Data[offset++] % 10) + 2; // Grid points between 2 and 11
    }
    
    cmsUInt32Number inputChan = (nInputs > 0) ? nInputs : 1;
    cmsUInt32Number outputChan = (offset < Size) ? ((Data[offset++] % 4) + 1) : 1;
    cmsUInt32Number nGridPoints = (offset < Size) ? ((Data[offset++] % 10) + 2) : 2;
    cmsUInt32Number flags = (offset < Size) ? Data[offset++] : 0;
    
    // Test cmsStageAllocCLutFloatGranular
    cmsStage* stage1 = cmsStageAllocCLutFloatGranular(context, clutPoints, inputChan, outputChan, NULL);
    if (stage1) {
        // Test cmsStageSampleCLutFloat
        cmsStageSampleCLutFloat(stage1, DummySamplerFloat, NULL, flags);
        cmsStageFree(stage1);
    }
    
    // Test cmsStageAllocCLutFloat
    cmsStage* stage2 = cmsStageAllocCLutFloat(context, nGridPoints, inputChan, outputChan, NULL);
    if (stage2) {
        // Test cmsStageSampleCLut16bit
        cmsStageSampleCLut16bit(stage2, DummySampler16, NULL, flags);
        cmsStageFree(stage2);
    }
    
    // Test cmsSliceSpaceFloat
    if (nInputs > 0) {
        cmsSliceSpaceFloat(nInputs, clutPoints, DummySamplerFloat, NULL);
    }
    
    // Test cmsSliceSpace16
    if (nInputs > 0) {
        cmsSliceSpace16(nInputs, clutPoints, DummySampler16, NULL);
    }
    
    cmsDeleteContext(context);
    return 0;
}