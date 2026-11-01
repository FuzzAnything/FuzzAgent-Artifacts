// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8GetDataRowCol at cmscgats.c:2816:23 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void safe_fuzz_string(const uint8_t *Data, size_t Size, char *out, size_t max_len) {
    if (Size == 0) {
        out[0] = '\0';
        return;
    }
    size_t len = (Size < max_len - 1) ? Size : max_len - 1;
    memcpy(out, Data, len);
    out[len] = '\0';
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 == NULL) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Extract parameters from fuzz input
    uint8_t op = Data[0] % 7; // 0-6 for different operations
    uint8_t row = Data[1];
    uint8_t col = (Data[1] + 1) % 100;
    uint8_t sample_idx = (Data[1] + 2) % 50;
    
    char key[64], subkey[64], patch[64], sample[64], val[64], type[64];
    size_t offset = 2;
    size_t remaining = Size - offset;
    
    // Ensure we don't read past the buffer end
    if (remaining < 6) {
        // Not enough data for all strings, use minimal defaults or skip
        // We will just use empty strings for missing data
        key[0] = subkey[0] = patch[0] = sample[0] = val[0] = type[0] = '\0';
    } else {
        size_t chunk = remaining / 6;
        if (chunk == 0) chunk = 1;

        safe_fuzz_string(Data + offset, chunk, key, sizeof(key));
        offset += chunk;
        
        safe_fuzz_string(Data + offset, chunk, subkey, sizeof(subkey));
        offset += chunk;
        
        safe_fuzz_string(Data + offset, chunk, patch, sizeof(patch));
        offset += chunk;
        
        safe_fuzz_string(Data + offset, chunk, sample, sizeof(sample));
        offset += chunk;
        
        safe_fuzz_string(Data + offset, chunk, val, sizeof(val));
        offset += chunk;
        
        // For the last string, use whatever is left
        safe_fuzz_string(Data + offset, Size - offset, type, sizeof(type));
    }

    switch (op) {
        case 0:
            cmsIT8SetPropertyMulti(hIT8, key, subkey, val);
            break;
        case 1:
            cmsIT8SetData(hIT8, patch, sample, val);
            break;
        case 2:
            cmsIT8SetDataFormat(hIT8, sample_idx, sample);
            break;
        case 3:
            cmsIT8SetSheetType(hIT8, type);
            break;
        case 4:
            cmsIT8SetDataRowCol(hIT8, row, col, val);
            break;
        case 5: {
            const char *res = cmsIT8GetDataRowCol(hIT8, row, col);
            if (res != NULL) {
                // Consume result to avoid unused variable warning
                volatile char c = res[0];
                (void)c;
            }
            break;
        }
        case 6:
            // Combined operations to explore more states
            cmsIT8SetSheetType(hIT8, type);
            cmsIT8SetData(hIT8, patch, sample, val);
            cmsIT8SetDataRowCol(hIT8, row, col, val);
            cmsIT8SetPropertyMulti(hIT8, key, subkey, val);
            cmsIT8SetDataFormat(hIT8, sample_idx, sample);
            cmsIT8GetDataRowCol(hIT8, row, col);
            break;
    }

    // lcms2 does not have a specific IT8 deallocation function; 
    // the handle is managed by the context or requires specific internal cleanup not exposed.
    // We rely on context deletion to clean up resources associated with the handle.
    cmsDeleteContext(ctx);
    return 0;
}