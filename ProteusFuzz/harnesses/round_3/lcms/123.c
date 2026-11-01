// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void safe_fuzz_string(const uint8_t *data, size_t size, char *out, size_t out_size) {
    if (size == 0) {
        out[0] = '\0';
        return;
    }
    size_t copy_len = size < out_size - 1 ? size : out_size - 1;
    memcpy(out, data, copy_len);
    out[copy_len] = '\0';
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "w");
    if (f) {
        fprintf(f, "IT8.7\n");
        fclose(f);
    } else {
        return 0;
    }

    cmsContext ContextID = cmsCreateContext(NULL, 0);
    if (ContextID == NULL) {
        remove(dummy_file);
        return 0;
    }

    cmsHANDLE hIT8 = cmsIT8LoadFromFile(ContextID, dummy_file);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        remove(dummy_file);
        return 0;
    }

    char sample_name[256];
    char patch_name[256];
    char prop_key[256];
    char prop_val[256];
    char data_val[256];
    char filename[256];

    safe_fuzz_string(Data, Size, sample_name, sizeof(sample_name));
    safe_fuzz_string(Data, Size, patch_name, sizeof(patch_name));
    safe_fuzz_string(Data, Size, prop_key, sizeof(prop_key));
    safe_fuzz_string(Data, Size, prop_val, sizeof(prop_val));
    safe_fuzz_string(Data, Size, data_val, sizeof(data_val));
    safe_fuzz_string(Data, Size, filename, sizeof(filename));

    // 1. cmsIT8FindDataFormat
    int format_id = cmsIT8FindDataFormat(hIT8, sample_name);
    (void)format_id;

    // 2. cmsIT8SetPropertyStr
    cmsBool prop_result = cmsIT8SetPropertyStr(hIT8, prop_key, prop_val);
    (void)prop_result;

    // 3. cmsIT8SetData
    cmsBool data_result = cmsIT8SetData(hIT8, patch_name, sample_name, data_val);
    (void)data_result;

    // 4. cmsIT8GetPatchByName
    int patch_idx = cmsIT8GetPatchByName(hIT8, patch_name);
    (void)patch_idx;

    // 5. cmsIT8GetDataRowCol
    int row = 0, col = 0;
    if (Size >= 2) {
        row = (int)Data[0];
        col = (int)Data[1];
    }
    const char *cell_data = cmsIT8GetDataRowCol(hIT8, row, col);
    (void)cell_data;

    // 6. cmsIT8SaveToFile
    if (filename[0] == '\0') {
        strcpy(filename, "./dummy_file_out");
    } else {
        if (strlen(filename) > 0 && filename[0] != '.') {
            char temp[512];
            snprintf(temp, sizeof(temp), "./%s", filename);
            strcpy(filename, temp);
        }
    }
    cmsBool save_result = cmsIT8SaveToFile(hIT8, filename);
    (void)save_result;

    // Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);
    remove(dummy_file);
    remove(filename);

    return 0;
}