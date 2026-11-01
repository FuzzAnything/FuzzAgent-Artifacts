// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static char temp_buf[256];

static const char* get_string(const uint8_t *Data, size_t Size, size_t offset, size_t max_len) {
    if (offset >= Size) return "";
    size_t len = 0;
    while (len < max_len && offset + len < Size && Data[offset + len] != 0) {
        len++;
    }
    if (len == 0) return "";
    memcpy(temp_buf, Data + offset, len);
    temp_buf[len] = '\0';
    return temp_buf;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize LCMS context
    cmsContext ctx = cmsCreateContext(0, 0);
    if (ctx == NULL) return 0;

    // Allocate IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // 1. Fuzz cmsIT8SetComment
    for (size_t i = 0; i < Size && i < 10; i++) {
        const char *comment = get_string(Data, Size, i, 100);
        cmsIT8SetComment(hIT8, comment);
    }
    // Skip NULL and empty checks for comment if they cause issues, 
    // but usually the API handles them. If not, we rely on the loop above.
    // The error was specifically about SetPropertyUncooked, so we focus there.

    // 2. Fuzz cmsIT8SetData
    for (size_t i = 0; i < Size && i < 10; i++) {
        for (size_t j = 0; j < Size && j < 10; j++) {
            for (size_t k = 0; k < Size && k < 10; k++) {
                const char *patch = get_string(Data, Size, i, 50);
                const char *sample = get_string(Data, Size, j, 50);
                const char *val = get_string(Data, Size, k, 50);
                cmsIT8SetData(hIT8, patch, sample, val);
            }
        }
    }
    // Avoid passing NULL to parameters that might be dereferenced immediately without check
    // cmsIT8SetData(hIT8, NULL, "SAMPLE", "VAL"); 
    // cmsIT8SetData(hIT8, "PATCH", NULL, "VAL"); 
    // cmsIT8SetData(hIT8, "PATCH", "SAMPLE", NULL); 

    // 3. Fuzz cmsIT8SaveToFile
    const char *dummy_file = "./dummy_file";
    cmsIT8SaveToFile(hIT8, dummy_file);
    const char *fname = get_string(Data, Size, 0, 20);
    if (fname[0] != '\0') {
        cmsIT8SaveToFile(hIT8, fname);
    }

    // 4. Fuzz cmsIT8SetPropertyUncooked
    // The ASAN error occurred when passing NULL as 'Key'. 
    // The library code dereferences 'Key' immediately. 
    // We must ensure 'Key' is never NULL.
    for (size_t i = 0; i < Size && i < 10; i++) {
        for (size_t j = 0; j < Size && j < 10; j++) {
            const char *key = get_string(Data, Size, i, 50);
            const char *buffer = get_string(Data, Size, j, 100);
            // Ensure key is not empty or NULL before passing
            if (key != NULL && key[0] != '\0') {
                cmsIT8SetPropertyUncooked(hIT8, key, buffer);
            }
        }
    }
    // Removed: cmsIT8SetPropertyUncooked(hIT8, NULL, "BUFFER");
    // Removed: cmsIT8SetPropertyUncooked(hIT8, "KEY", NULL); // Buffer can likely be NULL, but Key cannot.
    // Let's test with a valid key and NULL buffer if the API allows, but safe to skip if unsure.
    cmsIT8SetPropertyUncooked(hIT8, "TEST_KEY", NULL); 

    // 5. Fuzz cmsIT8SetDataFormat
    int n = (Size > 0) ? (int)Data[0] : 0;
    if (n > 100) n = 100;
    if (n < -10) n = -10;
    
    for (int offset = 0; offset < Size && offset < 10; offset++) {
        const char *sample_fmt = get_string(Data, Size, offset, 50);
        cmsIT8SetDataFormat(hIT8, n, sample_fmt);
        // Avoid NULL sample if it causes issues, but API might handle it.
        // cmsIT8SetDataFormat(hIT8, n, NULL); 
        // cmsIT8SetDataFormat(hIT8, n, "");
    }
    for (int test_n = -5; test_n <= 5; test_n++) {
        cmsIT8SetDataFormat(hIT8, test_n, "FORMAT");
    }

    // 6. Fuzz cmsIT8SetTableByLabel
    for (size_t i = 0; i < Size && i < 10; i++) {
        for (size_t j = 0; j < Size && j < 10; j++) {
            for (size_t k = 0; k < Size && k < 10; k++) {
                const char *cSet = get_string(Data, Size, i, 50);
                const char *cField = get_string(Data, Size, j, 50);
                const char *expType = get_string(Data, Size, k, 50);
                cmsIT8SetTableByLabel(hIT8, cSet, cField, expType);
            }
        }
    }
    // Avoid passing NULL if the function dereferences immediately. 
    // The function signature suggests it might handle defaults, but let's be safe.
    // cmsIT8SetTableByLabel(hIT8, NULL, "FIELD", "TYPE");
    // cmsIT8SetTableByLabel(hIT8, "SET", NULL, "TYPE");
    // cmsIT8SetTableByLabel(hIT8, "SET", "FIELD", NULL);
    // cmsIT8SetTableByLabel(hIT8, NULL, NULL, NULL);

    // Cleanup
    cmsDeleteContext(ctx);

    return 0;
}