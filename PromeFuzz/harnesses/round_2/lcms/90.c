// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8SetSheetType at cmscgats.c:1513:19 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataFormat at cmscgats.c:1694:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lcms2.h"

#define MAXSTR 256

static int split_strings(const uint8_t *data, size_t size, const char **strings, int max_strings) {
    int count = 0;
    const uint8_t *ptr = data;
    const uint8_t *end = data + size;
    
    while (ptr < end && count < max_strings) {
        const uint8_t *start = ptr;
        while (ptr < end && *ptr != 0) {
            ptr++;
        }
        if (ptr < end) {
            strings[count] = (const char*)start;
            count++;
            ptr++;
        } else {
            break;
        }
    }
    return count;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(NULL, Data, Size);
    
    if (hIT8 == NULL) {
        hIT8 = cmsIT8Alloc(NULL);
        if (hIT8 == NULL) {
            return 0;
        }
        
        cmsIT8SetSheetType(hIT8, "CGATS.17");
        cmsIT8DefineDblFormat(hIT8, "%.4f");
        cmsIT8SetPropertyStr(hIT8, "ORIGINATOR", "Fuzzer");
        
        cmsIT8SetDataFormat(hIT8, 0, "RGB_R");
        cmsIT8SetDataFormat(hIT8, 1, "RGB_G");
        cmsIT8SetDataFormat(hIT8, 2, "RGB_B");
        
        cmsIT8SetDataRowCol(hIT8, 0, 0, "1.0");
        cmsIT8SetDataRowCol(hIT8, 0, 1, "0.0");
        cmsIT8SetDataRowCol(hIT8, 0, 2, "0.0");
        cmsIT8SetData(hIT8, 0, 0, "Patch1");
    }
    
    const char *strings[10];
    int num_strings = split_strings(Data, Size, strings, 10);
    
    int nPatch = 0;
    if (Size > 0) {
        nPatch = Data[0];
    }
    
    char buffer[MAXSTR];
    cmsIT8GetPatchName(hIT8, nPatch, buffer);
    cmsIT8GetPatchName(hIT8, nPatch, NULL);
    
    if (num_strings > 0) {
        cmsIT8GetProperty(hIT8, strings[0]);
    }
    
    if (num_strings > 1) {
        cmsIT8DefineDblFormat(hIT8, strings[1]);
    }
    
    if (num_strings > 2) {
        const char *patch = strings[2];
        const char *sample = (num_strings > 3) ? strings[3] : NULL;
        cmsIT8GetData(hIT8, patch, sample);
    }
    
    cmsIT8GetSheetType(hIT8);
    
    if (num_strings > 4) {
        const char *key = strings[4];
        const char *subkey = (num_strings > 5) ? strings[5] : NULL;
        cmsIT8GetPropertyMulti(hIT8, key, subkey);
    }
    
    cmsIT8Free(hIT8);
    return 0;
}