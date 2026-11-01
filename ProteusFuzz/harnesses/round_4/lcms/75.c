// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a default context
    cmsContext ContextID = cmsCreateContext(NULL, 0);
    if (ContextID == NULL) return 0;

    // Create an IT8 handle using cmsIT8Alloc with the context
    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Helper to create null-terminated strings from fuzz data
    char key[256], subkey[256], buffer[512], val[256], patch[256], comment[256];
    size_t max_len = sizeof(key) - 1;
    size_t current_size = (Size > max_len) ? max_len : Size;
    
    // Prepare key
    memcpy(key, Data, current_size);
    key[current_size] = '\0';

    // Fuzz cmsIT8SetPropertyMulti
    if (Size >= 2) {
        size_t subkey_len = (Size / 2) > max_len ? max_len : (Size / 2);
        memcpy(subkey, Data, subkey_len);
        subkey[subkey_len] = '\0';
        
        size_t remaining = Size - subkey_len;
        size_t buf_len = remaining > sizeof(buffer) - 1 ? sizeof(buffer) - 1 : remaining;
        memcpy(buffer, Data + subkey_len, buf_len);
        buffer[buf_len] = '\0';
        
        cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer);
    }

    // Fuzz cmsIT8SetDataRowCol
    int row = (Data[0] % 100) - 50;
    int col = (Data[1] % 100) - 50;
    size_t val_len = (Size > 1) ? (Data[1] % (sizeof(val) - 1)) : 0;
    if (val_len > 0) {
        size_t offset = 2;
        if (offset + val_len > Size) val_len = Size - offset;
        memcpy(val, Data + offset, val_len);
        val[val_len] = '\0';
    } else {
        val[0] = '\0';
    }
    cmsIT8SetDataRowCol(hIT8, row, col, val);

    // Fuzz cmsIT8SaveToFile
    cmsIT8SaveToFile(hIT8, "./dummy_file");

    // Fuzz cmsIT8GetPatchByName
    size_t patch_len = (Size > 2) ? (Data[2] % (sizeof(patch) - 1)) : 0;
    if (patch_len > 0) {
        size_t offset = 3;
        if (offset + patch_len > Size) patch_len = Size - offset;
        memcpy(patch, Data + offset, patch_len);
        patch[patch_len] = '\0';
    } else {
        patch[0] = '\0';
    }
    cmsIT8GetPatchByName(hIT8, patch);

    // Fuzz cmsIT8SetPropertyUncooked
    size_t buf_len2 = (Size > 3) ? (Data[3] % (sizeof(buffer) - 1)) : 0;
    if (buf_len2 > 0) {
        size_t offset = 4;
        if (offset + buf_len2 > Size) buf_len2 = Size - offset;
        memcpy(buffer, Data + offset, buf_len2);
        buffer[buf_len2] = '\0';
    } else {
        buffer[0] = '\0';
    }
    cmsIT8SetPropertyUncooked(hIT8, key, buffer);

    // Fuzz cmsIT8SetComment
    size_t comment_len = (Size > 4) ? (Data[4] % (sizeof(comment) - 1)) : 0;
    if (comment_len > 0) {
        size_t offset = 5;
        if (offset + comment_len > Size) comment_len = Size - offset;
        memcpy(comment, Data + offset, comment_len);
        comment[comment_len] = '\0';
    } else {
        comment[0] = '\0';
    }
    cmsIT8SetComment(hIT8, comment);

    // Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);

    return 0;
}