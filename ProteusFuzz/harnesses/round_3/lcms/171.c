// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowColDbl at cmscgats.c:2848:19 in lcms2.h
// cmsIT8GetDataRowColDbl at cmscgats.c:2826:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_handle(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        // Note: cmsIT8Alloc returns a handle managed by the context.
        // In lcms2, IT8 handles are typically freed via cmsCloseProfile if they represent profiles,
        // but for IT8 specific handles, they are often freed by the context or specific IT8 free functions.
        // However, standard practice for cmsHANDLE returned by IT8Alloc in many versions is to use cmsCloseProfile
        // if it's a profile, or simply let the context cleanup. 
        // Since cmsIT8Alloc documentation often implies it returns a handle that should be closed with cmsCloseProfile 
        // if it's a profile-like object, or we rely on context deletion.
        // To be safe and avoid crashes if it's not a profile, we check if we can close it.
        // Actually, cmsIT8Alloc returns a cmsHANDLE that is NOT a cmsHPROFILE. 
        // The correct way to free an IT8 handle is usually not cmsCloseProfile.
        // However, without a specific cmsIT8Free in the provided API list, and knowing lcms2 internals,
        // IT8 handles are often just void* structures. 
        // If the context is deleted, resources are cleaned up. 
        // If we must explicitly free, we might need to cast or use a specific function not listed.
        // Given the constraints, we will rely on cmsDeleteContext to clean up the IT8 handle 
        // as it was created with ContextID.
        // If explicit freeing is needed and cmsCloseProfile fails, it might be because it's not a profile.
        // Let's try to avoid calling cmsCloseProfile on non-profiles.
        // Standard lcms2 IT8 usage: cmsIT8Alloc creates a handle. 
        // There is no explicit "cmsIT8Free" in the public API usually, context deletion handles it.
        // So we do nothing here for hIT8 specifically, relying on context deletion.
    }
}

int LLVMFuzzerTestOneInput_171(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fix: cmsCreateContext requires two arguments: Plugin and UserData.
    // Passing NULL for both creates a default context.
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Extract parameters from fuzz input
    int row = 0;
    int col = 0;
    int n = 0;
    cmsFloat64Number val = 0.0;
    const char *patch = "Patch";
    const char *sample = "Sample";
    const char *prop = "Property";
    const char *format = "Format";
    const char *strVal = "Value";

    // Parse integers if enough data
    if (Size >= sizeof(int)) {
        memcpy(&row, Data, sizeof(int));
        if (Size >= 2 * sizeof(int)) {
            memcpy(&col, Data + sizeof(int), sizeof(int));
        }
        if (Size >= 3 * sizeof(int)) {
            memcpy(&n, Data + 2 * sizeof(int), sizeof(int));
        }
    }

    // Parse double if enough data
    if (Size >= sizeof(double)) {
        memcpy(&val, Data, sizeof(double));
    }

    // Create dummy strings from input data
    size_t strLen = Size > 100 ? 100 : Size;
    char patchBuf[101] = {0};
    char sampleBuf[101] = {0};
    char propBuf[101] = {0};
    char formatBuf[101] = {0};
    char strValBuf[101] = {0};

    if (strLen > 0) {
        memcpy(patchBuf, Data, strLen);
        memcpy(sampleBuf, Data, strLen);
        memcpy(propBuf, Data, strLen);
        memcpy(formatBuf, Data, strLen);
        memcpy(strValBuf, Data, strLen);
        patchBuf[strLen] = '\0';
        sampleBuf[strLen] = '\0';
        propBuf[strLen] = '\0';
        formatBuf[strLen] = '\0';
        strValBuf[strLen] = '\0';
    }

    patch = patchBuf;
    sample = sampleBuf;
    prop = propBuf;
    format = formatBuf;
    strVal = strValBuf;

    // Fuzz cmsIT8SetDataDbl
    cmsIT8SetDataDbl(hIT8, patch, sample, val);

    // Fuzz cmsIT8SetPropertyDbl
    cmsIT8SetPropertyDbl(hIT8, prop, val);

    // Fuzz cmsIT8SetDataRowCol
    cmsIT8SetDataRowCol(hIT8, row, col, strVal);

    // Fuzz cmsIT8SetDataFormat
    cmsIT8SetDataFormat(hIT8, n, format);

    // Fuzz cmsIT8SetDataRowColDbl
    cmsIT8SetDataRowColDbl(hIT8, row, col, val);

    // Fuzz cmsIT8GetDataRowColDbl
    cmsFloat64Number result = cmsIT8GetDataRowColDbl(hIT8, row, col);
    (void)result; // Suppress unused variable warning

    // Cleanup: Delete context which should free the IT8 handle allocated within it.
    cmsDeleteContext(ContextID);

    return 0;
}