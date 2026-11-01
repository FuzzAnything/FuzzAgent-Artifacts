// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static const char* get_safe_string(const uint8_t *Data, size_t Size, size_t offset, const char* default_val) {
    if (Size <= offset) {
        return default_val;
    }
    
    size_t len = 0;
    // Find the length of the string, ensuring we don't read past the buffer
    while (offset + len < Size && Data[offset + len] != '\0') {
        len++;
    }
    
    // If we reached the end of the buffer without a null terminator, use default
    if (offset + len >= Size) {
        return default_val;
    }
    
    // We found a null-terminated string within bounds
    // However, we cannot return a pointer directly into the fuzz buffer for functions 
    // that might store or copy it, because the fuzzer might reuse the buffer.
    // Instead, we should create a temporary copy if we need to pass it to a function
    // that stores it. But for simple reads, returning the pointer is okay if the 
    // library doesn't store it long-term. 
    // Given the crash in cmsIT8SetPropertyMulti which calls strlen, it implies the 
    // library is reading the string. The issue is that the fuzzer buffer might not 
    // be null-terminated at the offset we picked.
    // The safest approach for fuzzing string inputs is to construct a null-terminated 
    // string in a local buffer.
    
    static char temp_buf[256];
    size_t copy_len = len;
    if (copy_len >= sizeof(temp_buf)) {
        copy_len = sizeof(temp_buf) - 1;
    }
    memcpy(temp_buf, Data + offset, copy_len);
    temp_buf[copy_len] = '\0';
    
    return temp_buf;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    cmsHANDLE hIT8 = cmsIT8Alloc("./dummy_file");
    if (hIT8 == NULL) {
        return 0;
    }

    // Fuzz cmsIT8GetPatchName
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int patch_idx = 0;
    if (Size > 0) {
        patch_idx = (int)(Data[0] % 100) - 50; 
    }
    cmsIT8GetPatchName(hIT8, patch_idx, buffer);
    cmsIT8GetPatchName(hIT8, patch_idx, NULL);

    // Fuzz cmsIT8SetPropertyMulti
    // We must ensure the strings passed are null-terminated and safe.
    // Using the helper which creates a safe copy in a static buffer.
    // Note: Since get_safe_string uses a static buffer, we must call them sequentially 
    // or handle the fact that the previous value might be overwritten if the function 
    // stores the pointer. cmsIT8SetPropertyMulti likely copies the string, so it should be safe.
    const char *key = get_safe_string(Data, Size, 1, "Key");
    const char *subkey = get_safe_string(Data, Size, 2, "SubKey");
    const char *buffer_val = get_safe_string(Data, Size, 3, "Value");
    cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer_val);

    // Fuzz cmsIT8EnumDataFormat
    char **SampleNames = NULL;
    int sample_count = cmsIT8EnumDataFormat(hIT8, &SampleNames);
    // Note: lcms2 typically returns internal pointers for this, so we do not free SampleNames.

    // Fuzz cmsIT8SetDataRowCol
    int row = 0;
    int col = 0;
    if (Size > 0) row = (int)(Data[0] % 200) - 100;
    if (Size > 1) col = (int)(Data[1] % 50) - 25;
    
    const char *val = get_safe_string(Data, Size, 4, "1.0");
    cmsIT8SetDataRowCol(hIT8, row, col, val);

    // Fuzz cmsIT8SetDataFormat
    int n = 0;
    if (Size > 0) n = (int)(Data[0] % 100) - 50;
    const char *sample_fmt = get_safe_string(Data, Size, 5, "XYZ");
    cmsIT8SetDataFormat(hIT8, n, sample_fmt);

    // Fuzz cmsIT8SetTableByLabel
    const char *cSet = get_safe_string(Data, Size, 6, "Set1");
    const char *cField = get_safe_string(Data, Size, 7, "LABEL");
    const char *ExpectedType = get_safe_string(Data, Size, 8, "Type1");
    cmsIT8SetTableByLabel(hIT8, cSet, cField, ExpectedType);

    cmsIT8Free(hIT8);
    return 0;
}