// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsContext g_context = NULL;

static void cleanup_string_array(char** array, cmsUInt32Number count) {
    if (array) {
        for (cmsUInt32Number i = 0; i < count; i++) {
            if (array[i]) {
                free(array[i]);
            }
        }
        free(array);
    }
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    if (!g_context) {
        g_context = cmsCreateContext(NULL, NULL);
    }
    
    // Create IT8 handle from memory
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(g_context, Data, (cmsUInt32Number)Size);
    if (!hIT8) {
        return 0;
    }
    
    // Test cmsIT8EnumProperties
    char** propNames = NULL;
    cmsUInt32Number propCount = cmsIT8EnumProperties(hIT8, &propNames);
    if (propNames) {
        cleanup_string_array(propNames, propCount);
    }
    
    // Test cmsIT8SaveToMem
    cmsUInt32Number bytesNeeded = 0;
    cmsBool saveResult = cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
    if (saveResult && bytesNeeded > 0 && bytesNeeded < 1024*1024) {
        void* buffer = malloc(bytesNeeded);
        if (buffer) {
            cmsIT8SaveToMem(hIT8, buffer, &bytesNeeded);
            free(buffer);
        }
    }
    
    // Test cmsIT8EnumPropertyMulti
    char** subpropNames = NULL;
    cmsUInt32Number subpropCount = cmsIT8EnumPropertyMulti(hIT8, "SAMPLE_FIELDS", (const char***)&subpropNames);
    if (subpropNames) {
        cleanup_string_array(subpropNames, subpropCount);
    }
    
    // Test cmsIT8SetPropertyMulti with various inputs
    if (Size > 10) {
        char key[10], subkey[10], value[10];
        memcpy(key, Data, 9); key[9] = '\0';
        memcpy(subkey, Data + 1, 9); subkey[9] = '\0';
        memcpy(value, Data + 2, 9); value[9] = '\0';
        
        cmsIT8SetPropertyMulti(hIT8, key, subkey, value);
        cmsIT8SetPropertyMulti(hIT8, "KEY", "SUBKEY", "VALUE");
        cmsIT8SetPropertyMulti(hIT8, "", "", "");
        cmsIT8SetPropertyMulti(hIT8, NULL, NULL, NULL);
    }
    
    // Test cmsIT8TableCount
    cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
    
    // Clean up
    cmsIT8Free(hIT8);
    
    return 0;
}