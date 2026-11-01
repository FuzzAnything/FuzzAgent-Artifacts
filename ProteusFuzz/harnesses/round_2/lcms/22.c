// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_it8(cmsHANDLE hIT8, cmsContext ctx) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void free_string_array(char ***arr, int count) {
    if (arr == NULL || *arr == NULL) return;
    for (int i = 0; i < count; i++) {
        free((*arr)[i]);
    }
    free(*arr);
    *arr = NULL;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Helper to safely extract strings from fuzz data
    static char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // Use data to generate inputs
    size_t idx = 0;
    
    // 1. Test cmsIT8SetSheetType
    if (idx < Size) {
        size_t len = (Size - idx) < 255 ? (Size - idx) : 255;
        memcpy(buffer, Data + idx, len);
        buffer[len] = '\0';
        cmsIT8SetSheetType(hIT8, buffer);
        idx += len;
    }

    // 2. Test cmsIT8SetPropertyHex
    if (idx < Size) {
        size_t len = (Size - idx) < 32 ? (Size - idx) : 32;
        memcpy(buffer, Data + idx, len);
        buffer[len] = '\0';
        cmsUInt32Number val = 0;
        if (idx + len < Size) {
            val = Data[idx + len];
        }
        cmsIT8SetPropertyHex(hIT8, buffer, val);
        idx += len + 1;
    }

    // 3. Test cmsIT8SetDataFormat
    if (idx < Size) {
        int n = Data[idx] % 100; // Limit index to reasonable range
        idx++;
        if (idx < Size) {
            size_t len = (Size - idx) < 64 ? (Size - idx) : 64;
            memcpy(buffer, Data + idx, len);
            buffer[len] = '\0';
            cmsIT8SetDataFormat(hIT8, n, buffer);
            idx += len;
        }
    }

    // 4. Test cmsIT8EnumDataFormat
    char **SampleNames = NULL;
    int count = cmsIT8EnumDataFormat(hIT8, &SampleNames);
    if (count > 0 && SampleNames != NULL) {
        // Just free the returned array to prevent leaks
        free_string_array(&SampleNames, count);
    }

    // 5. Test cmsIT8GetPatchName
    if (idx < Size) {
        int nPatch = Data[idx] % 100;
        idx++;
        // Try with buffer
        memset(buffer, 0, sizeof(buffer));
        const char *name1 = cmsIT8GetPatchName(hIT8, nPatch, buffer);
        (void)name1; // Suppress unused warning
        // Try without buffer
        const char *name2 = cmsIT8GetPatchName(hIT8, nPatch, NULL);
        (void)name2; // Suppress unused warning
    }

    // 6. Test cmsIT8GetPatchByName
    if (idx < Size) {
        size_t len = (Size - idx) < 64 ? (Size - idx) : 64;
        memcpy(buffer, Data + idx, len);
        buffer[len] = '\0';
        int patchIdx = cmsIT8GetPatchByName(hIT8, buffer);
        (void)patchIdx; // Suppress unused warning
    }

    cleanup_it8(hIT8, ctx);
    return 0;
}