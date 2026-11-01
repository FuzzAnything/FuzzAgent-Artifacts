// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void safe_cleanup(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a default context
    cmsContext ContextID = cmsCreateContext(NULL, 0);
    if (ContextID == NULL) {
        return 0;
    }

    // Create an IT8 handle with the context
    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Ensure we have enough data to work with
    if (Size < 1) {
        safe_cleanup(hIT8);
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Extract parameters from fuzz input
    int patchIndex = (int)Data[0];
    int row = (int)Data[0];
    int col = (int)Data[0];
    int sampleIndex = (int)Data[0];

    // Create buffers for string operations
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // 1. cmsIT8SetSheetType
    if (Size >= 2) {
        size_t len = (Size - 1) < 255 ? (Size - 1) : 255;
        memcpy(buffer, Data + 1, len);
        buffer[len] = '\0';
        cmsIT8SetSheetType(hIT8, buffer);
    }

    // 2. cmsIT8GetPatchName
    if (Size >= 3) {
        size_t len = (Size - 2) < 255 ? (Size - 2) : 255;
        memcpy(buffer, Data + 2, len);
        buffer[len] = '\0';
        const char* result = cmsIT8GetPatchName(hIT8, patchIndex, buffer);
        (void)result; // Suppress unused variable warning
    } else {
        const char* result = cmsIT8GetPatchName(hIT8, patchIndex, NULL);
        (void)result;
    }

    // 3. cmsIT8GetDataRowCol
    const char* dataResult = cmsIT8GetDataRowCol(hIT8, row, col);
    (void)dataResult;

    // 4. cmsIT8SetDataFormat
    if (Size >= 4) {
        size_t len = (Size - 3) < 255 ? (Size - 3) : 255;
        memcpy(buffer, Data + 3, len);
        buffer[len] = '\0';
        cmsIT8SetDataFormat(hIT8, sampleIndex, buffer);
    }

    // 5. cmsIT8SetData
    if (Size >= 6) {
        size_t len1 = (Size - 3) < 255 ? (Size - 3) : 255;
        size_t len2 = (Size - 3) < 255 ? (Size - 3) : 255;
        size_t len3 = (Size - 3) < 255 ? (Size - 3) : 255;
        
        char patchName[256], sampleName[256], value[256];
        memset(patchName, 0, sizeof(patchName));
        memset(sampleName, 0, sizeof(sampleName));
        memset(value, 0, sizeof(value));
        
        memcpy(patchName, Data + 3, len1);
        patchName[len1] = '\0';
        memcpy(sampleName, Data + 3, len2);
        sampleName[len2] = '\0';
        memcpy(value, Data + 3, len3);
        value[len3] = '\0';
        
        cmsIT8SetData(hIT8, patchName, sampleName, value);
    }

    // 6. cmsIT8SetPropertyStr
    if (Size >= 5) {
        size_t len1 = (Size - 4) < 255 ? (Size - 4) : 255;
        size_t len2 = (Size - 4) < 255 ? (Size - 4) : 255;
        
        char prop[256], str[256];
        memset(prop, 0, sizeof(prop));
        memset(str, 0, sizeof(str));
        
        memcpy(prop, Data + 4, len1);
        prop[len1] = '\0';
        memcpy(str, Data + 4, len2);
        str[len2] = '\0';
        
        cmsIT8SetPropertyStr(hIT8, prop, str);
    }

    // Cleanup
    safe_cleanup(hIT8);
    cmsDeleteContext(ContextID);
    return 0;
}