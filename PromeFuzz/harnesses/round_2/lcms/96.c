// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for potential IT8 loading
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Create IT8 handle from dummy file
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, "./dummy_file");
    
    // If file loading failed, create empty IT8 handle
    if (!hIT8) {
        hIT8 = cmsIT8Alloc(NULL);
        if (!hIT8) {
            cleanup_dummy_file();
            return 0;
        }
    }
    
    // Extract strings from fuzzer input for parameter variation
    char buffer[256];
    const char *str1 = NULL;
    const char *str2 = NULL;
    
    if (Size > 0) {
        // Use first non-null byte sequence as string 1
        size_t len1 = Size > 255 ? 255 : Size;
        memcpy(buffer, Data, len1);
        buffer[len1] = '\0';
        str1 = buffer;
        
        // Use remaining data as string 2 if available
        if (Size > 256) {
            size_t len2 = Size - 256 > 255 ? 255 : Size - 256;
            memcpy(buffer, Data + 256, len2);
            buffer[len2] = '\0';
            str2 = buffer;
        }
    }
    
    // Test cmsIT8GetPatchName
    char patchBuffer[256];
    for (int i = 0; i < 5; i++) {
        const char *result = cmsIT8GetPatchName(hIT8, i, NULL);
        if (result) {
            // Test with buffer
            cmsIT8GetPatchName(hIT8, i, patchBuffer);
        }
        
        // Test negative and out-of-bounds indices
        cmsIT8GetPatchName(hIT8, -1, NULL);
        cmsIT8GetPatchName(hIT8, 1000, NULL);
    }
    
    // Test cmsIT8GetProperty
    if (str1) {
        cmsIT8GetProperty(hIT8, str1);
    }
    // Test with common property names
    cmsIT8GetProperty(hIT8, "DESCRIPTOR");
    cmsIT8GetProperty(hIT8, "ORIGINATOR");
    cmsIT8GetProperty(hIT8, "CREATED");
    cmsIT8GetProperty(hIT8, "");
    cmsIT8GetProperty(hIT8, NULL);
    
    // Test cmsIT8DefineDblFormat
    if (str1) {
        cmsIT8DefineDblFormat(hIT8, str1);
    }
    cmsIT8DefineDblFormat(hIT8, "%.4f");
    cmsIT8DefineDblFormat(hIT8, "%.2e");
    cmsIT8DefineDblFormat(hIT8, NULL);
    
    // Test cmsIT8GetData
    if (str1 && str2) {
        cmsIT8GetData(hIT8, str1, str2);
    }
    // Test with various combinations
    cmsIT8GetData(hIT8, "Patch1", "SAMPLE_1");
    cmsIT8GetData(hIT8, "Patch2", "SAMPLE_2");
    cmsIT8GetData(hIT8, "NonExistent", "SAMPLE_1");
    cmsIT8GetData(hIT8, "Patch1", "NonExistent");
    cmsIT8GetData(hIT8, NULL, NULL);
    
    // Test cmsIT8GetSheetType
    const char *sheetType = cmsIT8GetSheetType(hIT8);
    (void)sheetType; // Use result to avoid unused variable warning
    
    // Test cmsIT8GetPropertyMulti
    if (str1 && str2) {
        cmsIT8GetPropertyMulti(hIT8, str1, str2);
    }
    cmsIT8GetPropertyMulti(hIT8, "COLOR_REP", "RGB");
    cmsIT8GetPropertyMulti(hIT8, "COLOR_REP", "CMYK");
    cmsIT8GetPropertyMulti(hIT8, "NON_EXISTENT", "SUBKEY");
    cmsIT8GetPropertyMulti(hIT8, NULL, NULL);
    
    // Cleanup
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
    
    cleanup_dummy_file();
    return 0;
}