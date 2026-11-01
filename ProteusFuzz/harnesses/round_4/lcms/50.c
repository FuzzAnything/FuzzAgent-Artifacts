// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
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

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Use input data to generate strings for fuzzing
    // Ensure null termination by limiting length to Size-1
    size_t max_len = Size > 1 ? Size - 1 : 0;
    char *prop_key = (char *)malloc(max_len + 1);
    char *prop_val = (char *)malloc(max_len + 1);
    char *sample_str = (char *)malloc(max_len + 1);
    char *sheet_type = (char *)malloc(max_len + 1);
    char *patch_name = (char *)malloc(max_len + 1);
    char *sample_name = (char *)malloc(max_len + 1);
    char *sample_val = (char *)malloc(max_len + 1);

    if (!prop_key || !prop_val || !sample_str || !sheet_type || 
        !patch_name || !sample_name || !sample_val) {
        cleanup_it8(hIT8);
        free(prop_key); free(prop_val); free(sample_str); free(sheet_type);
        free(patch_name); free(sample_name); free(sample_val);
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Initialize strings with input data, ensuring null termination
    memcpy(prop_key, Data, max_len);
    prop_key[max_len] = '\0';
    
    // Create variations for different parameters
    if (Size > 1) {
        memcpy(prop_val, Data + 1, max_len - 1);
        prop_val[max_len - 1] = '\0';
    } else {
        prop_val[0] = '\0';
    }

    if (Size > 2) {
        memcpy(sample_str, Data + 2, max_len - 2);
        sample_str[max_len - 2] = '\0';
    } else {
        sample_str[0] = '\0';
    }

    if (Size > 3) {
        memcpy(sheet_type, Data + 3, max_len - 3);
        sheet_type[max_len - 3] = '\0';
    } else {
        sheet_type[0] = '\0';
    }

    if (Size > 4) {
        memcpy(sample_name, Data + 4, max_len - 4);
        sample_name[max_len - 4] = '\0';
    } else {
        sample_name[0] = '\0';
    }

    if (Size > 5) {
        memcpy(sample_val, Data + 5, max_len - 5);
        sample_val[max_len - 5] = '\0';
    } else {
        sample_val[0] = '\0';
    }

    // Fuzz cmsIT8SetPropertyStr
    cmsIT8SetPropertyStr(hIT8, prop_key, prop_val);
    cmsIT8SetPropertyStr(hIT8, prop_key, NULL);
    cmsIT8SetPropertyStr(hIT8, prop_key, "");

    // Fuzz cmsIT8SetDataFormat
    int n = (Size > 0) ? (int)(Data[0] % 100) : 0;
    cmsIT8SetDataFormat(hIT8, n, sample_str);
    cmsIT8SetDataFormat(hIT8, n, NULL);
    cmsIT8SetDataFormat(hIT8, n, "");

    // Fuzz cmsIT8SetSheetType
    cmsIT8SetSheetType(hIT8, sheet_type);
    cmsIT8SetSheetType(hIT8, NULL);
    cmsIT8SetSheetType(hIT8, "");

    // Fuzz cmsIT8EnumDataFormat
    char **SampleNames = NULL;
    int count = cmsIT8EnumDataFormat(hIT8, &SampleNames);
    if (SampleNames) {
        // Just access to trigger potential issues, do not free as it's likely internal
        for (int i = 0; i < count && i < 10; i++) {
            if (SampleNames[i]) {
                // Access
            }
        }
    }
    cmsIT8EnumDataFormat(hIT8, NULL);

    // Fuzz cmsIT8SetData
    cmsIT8SetData(hIT8, sample_name, sample_name, sample_val);
    cmsIT8SetData(hIT8, NULL, sample_name, sample_val);
    cmsIT8SetData(hIT8, sample_name, NULL, sample_val);
    cmsIT8SetData(hIT8, sample_name, sample_name, NULL);

    // Fuzz cmsIT8GetPatchName
    int nPatch = (Size > 0) ? (int)(Data[0] % 50) : 0;
    cmsIT8GetPatchName(hIT8, nPatch, patch_name);
    cmsIT8GetPatchName(hIT8, nPatch, NULL);
    cmsIT8GetPatchName(hIT8, -1, patch_name);
    cmsIT8GetPatchName(hIT8, 1000, patch_name);

    // Cleanup
    cleanup_it8(hIT8);
    free(prop_key);
    free(prop_val);
    free(sample_str);
    free(sheet_type);
    free(patch_name);
    free(sample_name);
    free(sample_val);
    cmsDeleteContext(ContextID);

    return 0;
}