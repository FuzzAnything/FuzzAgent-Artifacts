// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsAllocNamedColorList at cmsnamed.c:749:30 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsNamedColorIndex at cmsnamed.c:882:26 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
}

static void cleanup_named_color_list(cmsNAMEDCOLORLIST *list) {
    if (list != NULL) {
        cmsFreeNamedColorList(list);
    }
}

int LLVMFuzzerTestOneInput_168(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Create a dummy IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Create a dummy named color list using the correct API
    // cmsAllocNamedColorList requires 5 arguments: ContextID, n, ColorantCount, Prefix, Suffix
    cmsNAMEDCOLORLIST *ncList = cmsAllocNamedColorList(ctx, 0, 0, "", "");
    if (ncList == NULL) {
        cleanup_it8(hIT8);
        cmsDeleteContext(ctx);
        return 0;
    }

    // Use input data to generate test parameters
    const char *prop = (const char *)Data;
    const char *patchName = (const char *)Data;
    const char *set = (const char *)Data;
    const char *field = (const char *)Data;
    const char *expectedType = (const char *)Data;
    const char *sample = (const char *)Data;
    const char *colorName = (const char *)Data;

    // Ensure strings are null-terminated for safety
    char safeProp[256];
    char safePatchName[256];
    char safeSet[256];
    char safeField[256];
    char safeExpectedType[256];
    char safeSample[256];
    char safeColorName[256];

    size_t len = Size < 255 ? Size : 255;
    memcpy(safeProp, Data, len);
    safeProp[len] = '\0';
    memcpy(safePatchName, Data, len);
    safePatchName[len] = '\0';
    memcpy(safeSet, Data, len);
    safeSet[len] = '\0';
    memcpy(safeField, Data, len);
    safeField[len] = '\0';
    memcpy(safeExpectedType, Data, len);
    safeExpectedType[len] = '\0';
    memcpy(safeSample, Data, len);
    safeSample[len] = '\0';
    memcpy(safeColorName, Data, len);
    safeColorName[len] = '\0';

    // Test cmsIT8SetPropertyHex
    cmsUInt32Number val = (Size > 0) ? Data[0] : 0;
    cmsIT8SetPropertyHex(hIT8, safeProp, val);

    // Test cmsIT8GetDataRowCol
    int row = (Size > 1) ? Data[1] : 0;
    int col = (Size > 2) ? Data[2] : 0;
    const char *cellData = cmsIT8GetDataRowCol(hIT8, row, col);
    (void)cellData; // Suppress unused variable warning

    // Test cmsNamedColorIndex
    cmsInt32Number index = cmsNamedColorIndex(ncList, safeColorName);
    (void)index; // Suppress unused variable warning

    // Test cmsIT8GetPatchByName
    int patchIndex = cmsIT8GetPatchByName(hIT8, safePatchName);
    (void)patchIndex; // Suppress unused variable warning

    // Test cmsIT8SetTableByLabel
    int tableIndex = cmsIT8SetTableByLabel(hIT8, safeSet, safeField, safeExpectedType);
    (void)tableIndex; // Suppress unused variable warning

    // Test cmsIT8SetIndexColumn
    cmsBool result = cmsIT8SetIndexColumn(hIT8, safeSample);
    (void)result; // Suppress unused variable warning

    // Cleanup
    cleanup_it8(hIT8);
    cleanup_named_color_list(ncList);
    cmsDeleteContext(ctx);

    return 0;
}