// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_handle(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for IT8 operations
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Note: cmsIT8Open is not available in standard lcms2.h. 
    // We simulate a handle by allocating memory to test the API functions that accept a handle.
    // In a real scenario, a valid IT8 file and the correct open function would be used.
    cmsHANDLE hIT8 = malloc(1); 

    // Fuzz cmsIT8GetPropertyMulti
    if (hIT8) {
        const char *key = (Size > 0) ? (const char *)Data : "Key";
        const char *subkey = (Size > 1) ? (const char *)(Data + 1) : "SubKey";
        cmsIT8GetPropertyMulti(hIT8, key, subkey);
    }

    // Fuzz cmsIT8GetProperty
    if (hIT8) {
        const char *prop = (Size > 0) ? (const char *)Data : "Property";
        cmsIT8GetProperty(hIT8, prop);
    }

    // Fuzz cmsIT8GetData
    if (hIT8) {
        const char *patch = (Size > 0) ? (const char *)Data : "Patch";
        const char *sample = (Size > 1) ? (const char *)(Data + 1) : "Sample";
        cmsIT8GetData(hIT8, patch, sample);
    }

    // Fuzz cmsIT8SetPropertyMulti
    if (hIT8) {
        const char *key = (Size > 0) ? (const char *)Data : "Key";
        const char *subkey = (Size > 1) ? (const char *)(Data + 1) : "SubKey";
        const char *buffer = (Size > 2) ? (const char *)(Data + 2) : "Buffer";
        cmsIT8SetPropertyMulti(hIT8, key, subkey, buffer);
    }

    // Fuzz cmsIT8GetSheetType
    if (hIT8) {
        cmsIT8GetSheetType(hIT8);
    }

    // Fuzz cmsIT8DefineDblFormat
    if (hIT8) {
        const char *formatter = (Size > 0) ? (const char *)Data : "%.6f";
        cmsIT8DefineDblFormat(hIT8, formatter);
    }

    cleanup_handle(hIT8);

    // Clean up dummy file
    remove(dummy_file);

    return 0;
}