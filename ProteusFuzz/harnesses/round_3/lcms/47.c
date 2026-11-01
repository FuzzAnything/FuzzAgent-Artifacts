// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8GetDataDbl at cmscgats.c:2883:28 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetPropertyDbl at cmscgats.c:1591:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void safe_cleanup(cmsHANDLE hIT8, cmsContext ContextID) {
    if (hIT8 != NULL) {
        // hIT8 is an IT8 handle, not a profile. 
        // The correct way to free an IT8 handle is cmsIT8Free.
        cmsIT8Free(hIT8);
    }
    if (ContextID != NULL) {
        cmsDeleteContext(ContextID);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Create a default context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        remove(dummy_file);
        return 0;
    }

    // Allocate IT8 handle with the context
    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        remove(dummy_file);
        return 0;
    }

    // Extract strings and numbers from input data to use as parameters
    char patch[64] = {0};
    char sample[64] = {0};
    char prop[64] = {0};
    cmsFloat64Number val = 0.0;

    // Safely extract strings from input
    size_t max_len = Size / 4;
    if (max_len > 63) max_len = 63;
    if (Size >= 1) {
        memcpy(patch, Data, max_len);
        patch[max_len] = '\0';
    }
    if (Size >= 2) {
        size_t offset = 1;
        size_t len = (Size - offset) / 3;
        if (len > 63) len = 63;
        memcpy(sample, Data + offset, len);
        sample[len] = '\0';
    }
    if (Size >= 3) {
        size_t offset = 2;
        size_t len = (Size - offset) / 2;
        if (len > 63) len = 63;
        memcpy(prop, Data + offset, len);
        prop[len] = '\0';
    }

    // Extract a double value if enough data
    if (Size >= 8) {
        memcpy(&val, Data + Size - 8, 8);
    } else if (Size > 0) {
        val = (cmsFloat64Number)(Data[0]);
    }

    // Fuzz cmsIT8SetDataDbl
    cmsBool res1 = cmsIT8SetDataDbl(hIT8, patch, sample, val);
    (void)res1;

    // Fuzz cmsIT8SetPropertyDbl
    cmsBool res2 = cmsIT8SetPropertyDbl(hIT8, prop, val);
    (void)res2;

    // Fuzz cmsIT8GetDataDbl
    cmsFloat64Number res3 = cmsIT8GetDataDbl(hIT8, patch, sample);
    (void)res3;

    // Fuzz cmsIT8GetData
    const char *res4 = cmsIT8GetData(hIT8, patch, sample);
    (void)res4;

    // Fuzz cmsIT8GetPropertyDbl
    cmsFloat64Number res5 = cmsIT8GetPropertyDbl(hIT8, prop);
    (void)res5;

    // Fuzz cmsIT8GetProperty
    const char *res6 = cmsIT8GetProperty(hIT8, prop);
    (void)res6;

    // Cleanup
    safe_cleanup(hIT8, ContextID);
    remove(dummy_file);

    return 0;
}