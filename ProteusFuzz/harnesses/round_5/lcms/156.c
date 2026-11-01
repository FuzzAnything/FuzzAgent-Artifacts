// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetPropertyDbl at cmscgats.c:1591:28 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8GetDataDbl at cmscgats.c:2883:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

static cmsHANDLE create_it8_handle(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return NULL;
    }
    write_dummy_file(Data, Size);
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return NULL;
    }

    // Allocate a new IT8 handle. 
    // Note: cmsIT8Open is not a public API in standard lcms2.h for loading files directly into a handle
    // in the way assumed previously. We rely on cmsIT8Alloc to create a valid handle for setting data.
    cmsHANDLE h = cmsIT8Alloc(ctx);
    if (h == NULL) {
        cmsDeleteContext(ctx);
        return NULL;
    }
    
    // We do not attempt to load the dummy file as a valid IT8 chart because the data is random.
    // Instead, we use the allocated handle to test setting and getting properties/data.
    
    static cmsContext current_ctx = NULL;
    if (current_ctx != NULL) {
        cmsDeleteContext(current_ctx);
        current_ctx = NULL;
    }
    current_ctx = ctx;
    
    return h;
}

static void cleanup_it8_handle(cmsHANDLE h) {
    if (h != NULL) {
        cmsIT8Free(h);
    }
    static cmsContext current_ctx = NULL;
    if (current_ctx != NULL) {
        cmsDeleteContext(current_ctx);
        current_ctx = NULL;
    }
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHANDLE h = create_it8_handle(Data, Size);
    if (h == NULL) {
        return 0;
    }

    // Fuzz cmsIT8GetSheetType
    const char *sheetType = cmsIT8GetSheetType(h);
    (void)sheetType;

    // Fuzz cmsIT8GetProperty
    const char *prop = cmsIT8GetProperty(h, "TestProp");
    (void)prop;

    // Fuzz cmsIT8GetPropertyDbl
    cmsFloat64Number propDbl = cmsIT8GetPropertyDbl(h, "TestPropDbl");
    (void)propDbl;

    // Fuzz cmsIT8SetPropertyDbl
    cmsBool setPropDblRes = cmsIT8SetPropertyDbl(h, "SetPropDbl", (cmsFloat64Number)Data[0] * 0.1);
    (void)setPropDblRes;

    // Fuzz cmsIT8SetDataDbl
    char patch[16] = "Patch";
    char sample[16] = "Sample";
    if (Size > 1) {
        patch[0] = Data[0];
        sample[0] = Data[1];
    }
    cmsBool setDataDblRes = cmsIT8SetDataDbl(h, patch, sample, (cmsFloat64Number)Data[0] * 0.1);
    (void)setDataDblRes;

    // Fuzz cmsIT8GetDataDbl
    cmsFloat64Number getDataDbl = cmsIT8GetDataDbl(h, patch, sample);
    (void)getDataDbl;

    cleanup_it8_handle(h);

    return 0;
}