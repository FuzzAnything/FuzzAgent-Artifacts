// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCompute at cmssm.c:550:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsDesaturateLab at cmsgmt.c:515:19 in lcms2.h
// cmsGDBAddPoint at cmssm.c:358:19 in lcms2.h
// cmsGDBCheckPoint at cmssm.c:390:19 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

#ifdef __cplusplus
extern "C" {
#endif

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void destroy_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) * 7) {
        return 0;
    }

    cmsContext ctx = create_context();
    if (!ctx) {
        return 0;
    }

    cmsHANDLE hGBD = cmsGBDAlloc(ctx);
    if (!hGBD) {
        destroy_context(ctx);
        return 0;
    }

    size_t offset = 0;
    cmsCIELab lab_point;
    double params[4];

    memcpy(&lab_point.L, Data + offset, sizeof(double));
    offset += sizeof(double);
    memcpy(&lab_point.a, Data + offset, sizeof(double));
    offset += sizeof(double);
    memcpy(&lab_point.b, Data + offset, sizeof(double));
    offset += sizeof(double);

    memcpy(params, Data + offset, sizeof(double) * 4);
    offset += sizeof(double) * 4;

    cmsGDBAddPoint(hGBD, &lab_point);

    if (Size >= offset + sizeof(cmsUInt32Number)) {
        cmsUInt32Number flags;
        memcpy(&flags, Data + offset, sizeof(cmsUInt32Number));
        cmsGDBCompute(hGBD, flags);
    }

    cmsGDBCheckPoint(hGBD, &lab_point);

    cmsDesaturateLab(&lab_point, params[0], params[1], params[2], params[3]);

    size_t remaining = Size - offset;
    if (remaining >= sizeof(cmsCIELab)) {
        size_t points = remaining / sizeof(cmsCIELab);
        const uint8_t* lab_data = Data + offset;
        
        for (size_t i = 0; i < points; i++) {
            cmsCIELab additional_lab;
            memcpy(&additional_lab, lab_data + i * sizeof(cmsCIELab), sizeof(cmsCIELab));
            cmsGDBAddPoint(hGBD, &additional_lab);
            cmsGDBCheckPoint(hGBD, &additional_lab);
        }
    }

    cmsGBDFree(hGBD);
    destroy_context(ctx);
    
    return 0;
}

#ifdef __cplusplus
}
#endif