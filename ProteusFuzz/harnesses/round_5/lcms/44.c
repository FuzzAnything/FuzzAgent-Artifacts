// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetHeaderManufacturer at cmsio0.c:1014:16 in lcms2.h
// cmsSetHeaderRenderingIntent at cmsio0.c:990:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsGetPostScriptColorResource at cmsps2.c:1525:27 in lcms2.h
// cmsSaveProfileToIOhandler at cmsio0.c:1446:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsContext create_context_from_data(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    return ctx;
}

static cmsHPROFILE create_profile_from_data(cmsContext ctx, const uint8_t *Data, size_t Size) {
    (void)ctx;
    if (Size >= 12) {
        cmsHPROFILE hProfile = cmsOpenProfileFromMem((const void*)Data, (cmsUInt32Number)Size);
        if (hProfile != NULL) {
            return hProfile;
        }
    }

    // Create a dummy RGB profile if input data is not a valid profile
    cmsCIExyY WhitePoint = {0.3127f, 0.3290f, 1.0f};
    cmsCIExyY Primaries[3] = {
        {0.6400f, 0.3300f, 0.0f},
        {0.3000f, 0.6000f, 0.0f},
        {0.1500f, 0.0600f, 0.0f}
    };
    cmsToneCurve* TransferFunction[3] = {NULL, NULL, NULL};

    return cmsCreateRGBProfile(&WhitePoint, Primaries, TransferFunction);
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context_from_data(Data, Size);
    if (ctx == NULL) return 0;

    cmsHPROFILE hProfile = create_profile_from_data(ctx, Data, Size);
    if (hProfile == NULL) {
        cleanup_context(ctx);
        return 0;
    }

    cmsUInt32Number manufacturer = 0;
    cmsUInt32Number intent = 0;
    cmsUInt32Number model = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number ps_type = 0;
    cmsUInt32Number ps_flags = 0;

    if (Size >= 4) {
        manufacturer = *(const cmsUInt32Number*)(Data);
        if (Size >= 8) intent = *(const cmsUInt32Number*)(Data + 4);
        if (Size >= 12) model = *(const cmsUInt32Number*)(Data + 8);
        if (Size >= 16) flags = *(const cmsUInt32Number*)(Data + 12);
        if (Size >= 20) ps_type = *(const cmsUInt32Number*)(Data + 16);
        if (Size >= 24) ps_flags = *(const cmsUInt32Number*)(Data + 20);
    }

    intent = intent % 4;

    cmsSetHeaderManufacturer(hProfile, manufacturer);
    cmsSetHeaderRenderingIntent(hProfile, intent);
    cmsSetHeaderModel(hProfile, model);
    cmsSetHeaderFlags(hProfile, flags);

    cmsUInt32Number res1 = cmsGetPostScriptColorResource(ctx, (cmsPSResourceType)ps_type, hProfile, intent, ps_flags, NULL);
    (void)res1;

    // Use pointer to cmsIOHANDLER since the struct is opaque in lcms2.h
    cmsIOHANDLER* ioHandler = NULL;
    
    // Try to allocate memory for the handler if needed, but for fuzzing NULL is often sufficient
    // or we can try to create a file-based handler if we had a file, but we stick to NULL or simple struct init if possible.
    // Since struct is opaque, we cannot initialize it directly. 
    // We will rely on NULL for the IO handler in this driver to avoid compilation errors with opaque structs.
    // If specific IO handler testing is needed, one would need to use cmsOpenIOHandlerFromFile or similar.
    
    cmsUInt32Number res2 = cmsGetPostScriptColorResource(ctx, (cmsPSResourceType)ps_type, hProfile, intent, ps_flags, ioHandler);
    (void)res2;

    cmsUInt32Number saved1 = cmsSaveProfileToIOhandler(hProfile, NULL);
    (void)saved1;

    cmsUInt32Number saved2 = cmsSaveProfileToIOhandler(hProfile, ioHandler);
    (void)saved2;

    cleanup_profile(hProfile);
    cleanup_context(ctx);

    return 0;
}