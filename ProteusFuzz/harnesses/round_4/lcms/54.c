// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8SetPropertyUncooked at cmscgats.c:1563:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_handle(cmsHANDLE hIT8) {
    if (hIT8) {
        cmsIT8Free(hIT8);
    }
}

static cmsHANDLE create_it8_handle(const uint8_t *Data, size_t Size) {
    cmsHANDLE hIT8 = NULL;
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    
    if (ContextID == NULL) {
        return NULL;
    }

    // Try to create a temporary file to load IT8 data
    if (Data && Size > 0) {
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            
            // Load from file using the context
            hIT8 = cmsIT8LoadFromFile(ContextID, "./dummy_file");
        }
    }
    
    // Clean up context if handle creation failed or succeeded (context is not owned by handle)
    cmsDeleteContext(ContextID);
    
    return hIT8;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHANDLE hIT8 = create_it8_handle(Data, Size);
    if (!hIT8) {
        return 0;
    }

    // Extract strings from fuzz data for parameters
    const char *cSet = (Size > 0) ? (const char *)Data : "Set1";
    const char *cField = (Size > 10) ? (const char *)(Data + 10) : "LABEL";
    const char *ExpectedType = (Size > 20) ? (const char *)(Data + 20) : NULL;
    const char *cSample = (Size > 30) ? (const char *)(Data + 30) : "Sample1";
    const char *cFileName = "./dummy_file";
    const char *cPatch = (Size > 40) ? (const char *)(Data + 40) : "Patch1";
    const char *Key = (Size > 50) ? (const char *)(Data + 50) : "Key1";
    const char *Buffer = (Size > 60) ? (const char *)(Data + 60) : "Buffer1";

    // Ensure null termination for safety
    char safeSet[256] = {0};
    char safeField[256] = {0};
    char safeExpectedType[256] = {0};
    char safeSample[256] = {0};
    char safePatch[256] = {0};
    char safeKey[256] = {0};
    char safeBuffer[256] = {0};

    strncpy(safeSet, cSet, sizeof(safeSet) - 1);
    strncpy(safeField, cField, sizeof(safeField) - 1);
    strncpy(safeExpectedType, ExpectedType ? ExpectedType : "", sizeof(safeExpectedType) - 1);
    strncpy(safeSample, cSample, sizeof(safeSample) - 1);
    strncpy(safePatch, cPatch, sizeof(safePatch) - 1);
    strncpy(safeKey, Key, sizeof(safeKey) - 1);
    strncpy(safeBuffer, Buffer, sizeof(safeBuffer) - 1);

    // 1. cmsIT8SetTableByLabel
    cmsIT8SetTableByLabel(hIT8, safeSet, safeField, safeExpectedType);

    // 2. cmsIT8SetIndexColumn
    cmsIT8SetIndexColumn(hIT8, safeSample);

    // 3. cmsIT8GetPatchName
    char patchNameBuffer[256] = {0};
    cmsIT8GetPatchName(hIT8, 0, patchNameBuffer);
    cmsIT8GetPatchName(hIT8, 0, NULL);

    // 4. cmsIT8SaveToFile
    cmsIT8SaveToFile(hIT8, cFileName);

    // 5. cmsIT8GetPatchByName
    cmsIT8GetPatchByName(hIT8, safePatch);

    // 6. cmsIT8SetPropertyUncooked
    cmsIT8SetPropertyUncooked(hIT8, safeKey, safeBuffer);

    // Cleanup
    cleanup_handle(hIT8);

    return 0;
}