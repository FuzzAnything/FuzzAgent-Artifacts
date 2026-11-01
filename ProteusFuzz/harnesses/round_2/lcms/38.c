// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHANDLE CreateValidIT8Handle(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;

    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return NULL;
    }

    if (Size > 0) {
        // Ensure strings are null-terminated to prevent buffer overflows
        char *safe_str = (char *)malloc(Size + 1);
        if (safe_str) {
            memcpy(safe_str, Data, Size);
            safe_str[Size] = '\0';

            // Set a string property
            cmsIT8SetPropertyStr(hIT8, "TestProp", safe_str);
            
            // Set an uncooked property
            cmsIT8SetPropertyUncooked(hIT8, "UncookedKey", safe_str);
            
            // Set index column
            cmsIT8SetIndexColumn(hIT8, safe_str);
            
            // Find data format
            cmsIT8FindDataFormat(hIT8, safe_str);
            
            // Set data row/col with various indices derived from input
            if (Size >= 2) {
                int row = (int)Data[0];
                int col = (int)Data[1];
                cmsIT8SetDataRowCol(hIT8, row, col, safe_str);
            }
            
            free(safe_str);
        }
    }
    
    return hIT8;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHANDLE hIT8 = CreateValidIT8Handle(Data, Size);
    if (!hIT8) return 0;

    // Ensure strings are null-terminated for diverse invocations
    char *safe_str = (char *)malloc(Size + 1);
    if (!safe_str) {
        cmsIT8Free(hIT8);
        return 0;
    }
    memcpy(safe_str, Data, Size);
    safe_str[Size] = '\0';

    // Diverse invocations to explore program states
    cmsBool res1 = cmsIT8SetPropertyUncooked(hIT8, "Key1", safe_str);
    cmsBool res2 = cmsIT8SetPropertyStr(hIT8, "Prop2", safe_str);
    int formatIdx = cmsIT8FindDataFormat(hIT8, safe_str);
    
    if (Size >= 2) {
        cmsBool res3 = cmsIT8SetDataRowCol(hIT8, (int)Data[0], (int)Data[1], safe_str);
    }
    
    cmsBool res4 = cmsIT8SetIndexColumn(hIT8, safe_str);

    // Save to dummy file
    cmsBool res5 = cmsIT8SaveToFile(hIT8, "./dummy_file");

    free(safe_str);
    // Cleanup
    cmsIT8Free(hIT8);

    return 0;
}