// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2.h>

static cmsHANDLE create_it8_handle(void) {
    // Create a new IT8 handle with NULL context (default)
    cmsHANDLE hIT8 = cmsIT8Alloc(NULL);
    return hIT8;
}

static void cleanup_it8_handle(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHANDLE hIT8 = create_it8_handle();
    if (hIT8 == NULL) {
        return 0;
    }

    // Use input data to drive fuzzing
    const char *input_str = (const char *)Data;
    size_t input_len = Size;

    // Ensure null-termination for string operations
    char *safe_str = (char *)malloc(input_len + 1);
    if (safe_str == NULL) {
        cleanup_it8_handle(hIT8);
        return 0;
    }
    memcpy(safe_str, Data, input_len);
    safe_str[input_len] = '\0';

    // Fuzz cmsIT8SetSheetType
    cmsIT8SetSheetType(hIT8, safe_str);

    // Fuzz cmsIT8SetPropertyMulti
    // Split input into Key, SubKey, and Buffer if possible
    char key[64], subkey[64], buffer[256];
    memset(key, 0, sizeof(key));
    memset(subkey, 0, sizeof(subkey));
    memset(buffer, 0, sizeof(buffer));

    if (input_len >= 3) {
        size_t key_len = (input_len / 3) > 63 ? 63 : input_len / 3;
        size_t subkey_len = (input_len / 3) > 63 ? 63 : input_len / 3;
        size_t buffer_len = input_len - key_len - subkey_len;
        if (buffer_len > 255) buffer_len = 255;

        memcpy(key, safe_str, key_len);
        memcpy(subkey, safe_str + key_len, subkey_len);
        memcpy(buffer, safe_str + key_len + subkey_len, buffer_len);

        cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer);
    }

    // Fuzz cmsIT8SetPropertyDbl
    cmsFloat64Number val_dbl = 0.0;
    if (input_len >= 8) {
        memcpy(&val_dbl, safe_str, 8);
        // Ensure valid double by checking for NaN/Inf if desired, but let fuzzer explore
        cmsIT8SetPropertyDbl(hIT8, "FuzzProp", val_dbl);
    } else if (input_len > 0) {
        // Fallback: use first byte as integer part
        val_dbl = (cmsFloat64Number)(safe_str[0] & 0xFF);
        cmsIT8SetPropertyDbl(hIT8, "FuzzProp", val_dbl);
    }

    // Fuzz cmsIT8SetDataDbl
    char patch[32], sample[32];
    memset(patch, 0, sizeof(patch));
    memset(sample, 0, sizeof(sample));
    
    if (input_len >= 2) {
        size_t patch_len = (input_len / 2) > 31 ? 31 : input_len / 2;
        size_t sample_len = input_len - patch_len;
        if (sample_len > 31) sample_len = 31;

        memcpy(patch, safe_str, patch_len);
        memcpy(sample, safe_str + patch_len, sample_len);

        cmsFloat64Number data_val = 0.0;
        if (input_len >= 8) {
            memcpy(&data_val, safe_str + patch_len + sample_len, 8);
        } else {
            data_val = (cmsFloat64Number)(safe_str[0] & 0xFF);
        }
        cmsIT8SetDataDbl(hIT8, patch, sample, data_val);
    }

    // Fuzz cmsIT8GetDataRowCol
    int row = 0, col = 0;
    if (input_len >= 2) {
        row = (int)(safe_str[0] & 0xFF);
        col = (int)(safe_str[1] & 0xFF);
    }
    // Try a few variations
    const char *result = cmsIT8GetDataRowCol(hIT8, row, col);
    (void)result; // Suppress unused variable warning

    // Try negative indices
    if (input_len >= 2) {
        row = -(int)(safe_str[0] & 0xFF);
        col = -(int)(safe_str[1] & 0xFF);
        result = cmsIT8GetDataRowCol(hIT8, row, col);
        (void)result;
    }

    // Fuzz cmsIT8SaveToFile
    // Write some data to dummy_file to ensure it exists
    FILE *f = fopen("./dummy_file", "w");
    if (f != NULL) {
        fwrite(safe_str, 1, input_len, f);
        fclose(f);
    }
    
    cmsIT8SaveToFile(hIT8, "./dummy_file");

    // Cleanup
    free(safe_str);
    cleanup_it8_handle(hIT8);

    return 0;
}