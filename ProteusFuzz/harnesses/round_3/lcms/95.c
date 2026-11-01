// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void safe_fuzz_string(const uint8_t *Data, size_t Size, char *out, size_t out_size) {
    if (Size == 0) {
        out[0] = '\0';
        return;
    }
    size_t copy_len = Size < out_size - 1 ? Size : out_size - 1;
    memcpy(out, Data, copy_len);
    out[copy_len] = '\0';
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = cmsCreateNULLProfile();
    if (hProfile == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(hProfile);
    if (hIT8 == NULL) {
        cmsCloseProfile(hProfile);
        return 0;
    }

    char key[256];
    char buffer[256];
    char comment[256];
    char sample[256];
    char patch[256];
    char val[256];

    safe_fuzz_string(Data, Size, key, sizeof(key));
    safe_fuzz_string(Data, Size, buffer, sizeof(buffer));
    safe_fuzz_string(Data, Size, comment, sizeof(comment));
    safe_fuzz_string(Data, Size, sample, sizeof(sample));
    safe_fuzz_string(Data, Size, patch, sizeof(patch));
    safe_fuzz_string(Data, Size, val, sizeof(val));

    // Fuzz cmsIT8SetPropertyUncooked
    cmsIT8SetPropertyUncooked(hIT8, key, buffer);

    // Fuzz cmsIT8SetComment
    cmsIT8SetComment(hIT8, comment);

    // Fuzz cmsIT8FindDataFormat
    int format_id = cmsIT8FindDataFormat(hIT8, sample);
    (void)format_id;

    // Fuzz cmsIT8SetIndexColumn
    cmsIT8SetIndexColumn(hIT8, sample);

    // Fuzz cmsIT8SetPropertyStr
    cmsIT8SetPropertyStr(hIT8, key, buffer);

    // Fuzz cmsIT8SetData
    cmsIT8SetData(hIT8, patch, sample, val);

    cmsIT8Free(hIT8);
    cmsCloseProfile(hProfile);
    return 0;
}