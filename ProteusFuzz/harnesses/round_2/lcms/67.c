// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void safe_fuzz_string(const uint8_t *Data, size_t Size, char *out, size_t outSize) {
    if (Size == 0 || outSize == 0) {
        out[0] = '\0';
        return;
    }
    size_t copy_len = Size < outSize - 1 ? Size : outSize - 1;
    memcpy(out, Data, copy_len);
    out[copy_len] = '\0';
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(NULL);
    if (!hProfile) {
        return 0;
    }

    cmsHANDLE hIT8 = cmsIT8Alloc(hProfile);
    if (!hIT8) {
        cmsCloseProfile(hProfile);
        return 0;
    }

    // Fuzz cmsIT8SetTable
    if (Size > 0) {
        cmsUInt32Number nTable = Data[0] % 10; // Limit table index to reasonable range
        cmsIT8SetTable(hIT8, nTable);
    }

    // Fuzz cmsIT8SetPropertyHex
    // Ensure we have at least 3 bytes for Data[1], Data[2] and the start of the string
    if (Size > 3) {
        cmsUInt32Number Val = ((cmsUInt32Number)Data[1] << 8) | (Data[2] % 256);
        char prop[64];
        safe_fuzz_string(Data + 3, Size - 3, prop, sizeof(prop));
        cmsIT8SetPropertyHex(hIT8, prop, Val);
    }

    // Fuzz cmsIT8SetDataFormat
    // Ensure we have at least 3 bytes for index and start of string
    if (Size > 3) {
        int n = Data[2] % 5;
        char sample[64];
        safe_fuzz_string(Data + 3, Size - 3, sample, sizeof(sample));
        cmsIT8SetDataFormat(hIT8, n, sample);
    }

    // Fuzz cmsIT8SetDataRowCol
    // Ensure we have enough bytes for row, col, and value string
    if (Size > 5) {
        int row = (int)(Data[3] % 10) - 5; // Allow negative indices to test bounds
        int col = (int)(Data[4] % 10) - 5;
        char val[64];
        safe_fuzz_string(Data + 5, Size - 5, val, sizeof(val));
        cmsIT8SetDataRowCol(hIT8, row, col, val);
    }

    // Fuzz cmsIT8GetPatchByName
    // Ensure we have enough bytes for the patch name string
    if (Size > 5) {
        char patchName[64];
        safe_fuzz_string(Data + 5, Size - 5, patchName, sizeof(patchName));
        cmsIT8GetPatchByName(hIT8, patchName);
    }

    // Fuzz cmsIT8SaveToMem (Two-pass: query size, then write)
    cmsUInt32Number BytesNeeded = 0;
    cmsBool res = cmsIT8SaveToMem(hIT8, NULL, &BytesNeeded);
    (void)res; // Ignore return of query phase

    if (BytesNeeded > 0) {
        void *MemPtr = malloc(BytesNeeded);
        if (MemPtr) {
            res = cmsIT8SaveToMem(hIT8, MemPtr, &BytesNeeded);
            (void)res;
            free(MemPtr);
        }
    }

    // Cleanup
    cmsIT8Free(hIT8);
    cmsCloseProfile(hProfile);

    return 0;
}