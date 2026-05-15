// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCIECAM02Forward at cmscam02.c:440:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCIECAM02Init at cmscam02.c:363:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCIECAM02Reverse at cmscam02.c:466:16 in lcms2.h
// cmsCIECAM02Done at cmscam02.c:432:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cmsD50_XYZ(void) {
    const cmsCIEXYZ* d50 = cmsD50_XYZ();
    (void)d50;
}

static void fuzz_cmsCIECAM02Forward(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIEXYZ) + sizeof(cmsViewingConditions)) {
        return;
    }
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;
    
    const cmsViewingConditions* vc = (const cmsViewingConditions*)data;
    cmsHANDLE hModel = cmsCIECAM02Init(ctx, vc);
    if (!hModel) {
        cmsDeleteContext(ctx);
        return;
    }
    
    const cmsCIEXYZ* pIn = (const cmsCIEXYZ*)(data + sizeof(cmsViewingConditions));
    cmsJCh pOut;
    
    cmsCIECAM02Forward(hModel, pIn, &pOut);
    
    cmsCIECAM02Done(hModel);
    cmsDeleteContext(ctx);
}

static void fuzz_cmsXYZ2Lab(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIEXYZ) * 2) {
        return;
    }
    
    const cmsCIEXYZ* WhitePoint = (const cmsCIEXYZ*)data;
    const cmsCIEXYZ* xyz = (const cmsCIEXYZ*)(data + sizeof(cmsCIEXYZ));
    cmsCIELab Lab;
    
    cmsXYZ2Lab(WhitePoint, &Lab, xyz);
    
    cmsXYZ2Lab(NULL, &Lab, xyz);
}

static void fuzz_cmsCIECAM02Reverse(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsJCh) + sizeof(cmsViewingConditions)) {
        return;
    }
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return;
    
    const cmsViewingConditions* vc = (const cmsViewingConditions*)data;
    cmsHANDLE hModel = cmsCIECAM02Init(ctx, vc);
    if (!hModel) {
        cmsDeleteContext(ctx);
        return;
    }
    
    const cmsJCh* pIn = (const cmsJCh*)(data + sizeof(cmsViewingConditions));
    cmsCIEXYZ pOut;
    
    cmsCIECAM02Reverse(hModel, pIn, &pOut);
    
    cmsCIECAM02Done(hModel);
    cmsDeleteContext(ctx);
}

static void fuzz_cmsLab2XYZ(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIELab) + sizeof(cmsCIEXYZ)) {
        return;
    }
    
    const cmsCIEXYZ* WhitePoint = (const cmsCIEXYZ*)data;
    const cmsCIELab* Lab = (const cmsCIELab*)(data + sizeof(cmsCIEXYZ));
    cmsCIEXYZ xyz;
    
    cmsLab2XYZ(WhitePoint, &xyz, Lab);
    
    cmsLab2XYZ(NULL, &xyz, Lab);
}

static void fuzz_cmsAdaptToIlluminant(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCIEXYZ) * 4) {
        return;
    }
    
    cmsCIEXYZ Result;
    const cmsCIEXYZ* SourceWhitePt = (const cmsCIEXYZ*)data;
    const cmsCIEXYZ* Illuminant = (const cmsCIEXYZ*)(data + sizeof(cmsCIEXYZ));
    const cmsCIEXYZ* Value = (const cmsCIEXYZ*)(data + 2 * sizeof(cmsCIEXYZ));
    
    cmsAdaptToIlluminant(&Result, SourceWhitePt, Illuminant, Value);
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_cmsD50_XYZ();
    fuzz_cmsCIECAM02Forward(Data, Size);
    fuzz_cmsXYZ2Lab(Data, Size);
    fuzz_cmsCIECAM02Reverse(Data, Size);
    fuzz_cmsLab2XYZ(Data, Size);
    fuzz_cmsAdaptToIlluminant(Data, Size);
    
    return 0;
}