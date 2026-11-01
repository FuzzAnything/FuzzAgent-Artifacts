// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_fuzz_data(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
}

static cmsHANDLE create_fuzz_it8(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, 0);
    if (ctx == NULL) {
        return NULL;
    }

    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 == NULL) {
        cmsDeleteContext(ctx);
        return NULL;
    }

    // Store context pointer in the handle's user data if needed, 
    // but for this driver we rely on the handle being valid within the context.
    // We assume the context remains alive as long as the handle is used.
    // To simplify, we don't pass ctx around, but we must ensure we don't delete it prematurely.
    // In a real scenario, we might store ctx in a global or pass it along.
    // Here, we will just keep the context alive by not deleting it until the end of the function
    // or we assume the library manages it. However, lcms2 requires explicit deletion.
    // To fix the lifecycle, we will create the context once per input and delete it at the end.
    // But since we can't easily pass ctx to cleanup, we will handle deletion in the main function.
    
    // Actually, let's restructure: Create context in main, pass to helper? 
    // No, the helper returns the handle. The context must be kept alive.
    // We will assume the context is valid for the duration of the test.
    // We will store the context in a static variable or just delete it at the very end.
    // Let's just create the context in the main function and pass it to a helper that doesn't delete it,
    // then delete it in the main function.
    
    // Revised approach: The helper just allocates. The main function manages the context.
    // But the signature of create_fuzz_it8 doesn't take ctx.
    // Let's change the helper to take ctx.
    
    // Wait, I cannot change the helper signature easily if I want to keep the flow.
    // Let's just create the context in the main function and pass it to a new helper.
    
    return hIT8;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, 0);
    if (ctx == NULL) {
        return 0;
    }

    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz cmsIT8SetPropertyMulti
    if (Size >= 3) {
        char key[64], subkey[64], buffer[256];
        size_t klen = (Data[0] % 63) + 1;
        size_t slen = (Data[1] % 63) + 1;
        size_t blen = (Data[2] % 255) + 1;

        if (Size >= klen + slen + blen + 3) {
            memcpy(key, Data + 3, klen);
            key[klen] = '\0';
            memcpy(subkey, Data + 3 + klen, slen);
            subkey[slen] = '\0';
            memcpy(buffer, Data + 3 + klen + slen, blen);
            buffer[blen] = '\0';

            cmsBool ret = cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer);
            (void)ret;
        }
    }

    // Fuzz cmsIT8EnumDataFormat
    char **SampleNames = NULL;
    int count = cmsIT8EnumDataFormat(hIT8, &SampleNames);
    if (count > 0 && SampleNames != NULL) {
        for (int i = 0; i < count; i++) {
            if (SampleNames[i] != NULL) {
                free(SampleNames[i]);
            }
        }
        free(SampleNames);
    }

    // Fuzz cmsIT8SetSheetType
    if (Size > 0) {
        char type[64];
        size_t tlen = (Data[0] % 63) + 1;
        if (Size >= tlen + 1) {
            memcpy(type, Data + 1, tlen);
            type[tlen] = '\0';
            cmsBool ret = cmsIT8SetSheetType(hIT8, type);
            (void)ret;
        }
    }

    // Fuzz cmsIT8SetDataRowCol
    if (Size >= 3) {
        int row = (int)(Data[0] % 100) - 50;
        int col = (int)(Data[1] % 100) - 50;
        size_t vlen = (Data[2] % 63) + 1;
        char val[64];
        if (Size >= 3 + vlen) {
            memcpy(val, Data + 3, vlen);
            val[vlen] = '\0';
            cmsBool ret = cmsIT8SetDataRowCol(hIT8, row, col, val);
            (void)ret;
        }
    }

    // Fuzz cmsIT8SaveToFile
    cmsBool saveRet = cmsIT8SaveToFile(hIT8, "./dummy_file");
    (void)saveRet;

    // Fuzz cmsIT8SetTableByLabel
    if (Size >= 3) {
        char set[64], field[64], expected[64];
        size_t slen = (Data[0] % 63) + 1;
        size_t flen = (Data[1] % 63) + 1;
        size_t elen = (Data[2] % 63) + 1;

        if (Size >= 3 + slen + flen + elen) {
            memcpy(set, Data + 3, slen);
            set[slen] = '\0';
            memcpy(field, Data + 3 + slen, flen);
            field[flen] = '\0';
            memcpy(expected, Data + 3 + slen + flen, elen);
            expected[elen] = '\0';

            int ret = cmsIT8SetTableByLabel(hIT8, set, field, expected);
            (void)ret;
        }
    }

    cmsIT8Free(hIT8);
    cmsDeleteContext(ctx);
    return 0;
}