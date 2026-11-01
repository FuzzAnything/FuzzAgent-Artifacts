// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8DefineDblFormat at cmscgats.c:3041:16 in lcms2.h
// cmsIT8GetSheetType at cmscgats.c:1508:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void write_dummy_it8_file(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    
    // Write a minimal valid IT8 header structure
    // Magic number for IT8.7 (0x49543837)
    uint32_t magic = 0x49543837;
    fwrite(&magic, sizeof(uint32_t), 1, f);
    
    // Version (usually 1.4 or 2.0) - using 1.4
    uint16_t version = 0x0104;
    fwrite(&version, sizeof(uint16_t), 1, f);
    
    // Padding or additional header fields to make it look plausible
    char dummy_data[100];
    memset(dummy_data, 'A', sizeof(dummy_data));
    fwrite(dummy_data, sizeof(dummy_data), 1, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_179(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Prepare environment: Create a dummy IT8 file
    const char *dummy_file = "./dummy_file";
    write_dummy_it8_file(dummy_file);

    // Open the IT8 file using cmsIT8LoadFromFile with NULL context
    cmsHANDLE hIT8 = cmsIT8LoadFromFile(NULL, dummy_file);
    
    // If opening fails, test error handling with NULL handle
    if (!hIT8) {
        cmsIT8GetPropertyMulti(NULL, "Key", "SubKey");
        cmsIT8GetData(NULL, "Patch", "Sample");
        cmsIT8GetPatchName(NULL, 0, NULL);
        cmsIT8DefineDblFormat(NULL, "Format");
        cmsIT8GetSheetType(NULL);
        cmsIT8GetProperty(NULL, "Prop");
        return 0;
    }

    // Helper to extract string from fuzz input safely
    static char buffer[256];
    
    // Extract potential strings from input safely
    int len = (Size > 255) ? 255 : Size;
    memcpy(buffer, Data, len);
    buffer[len] = '\0';

    // Fuzz cmsIT8GetPropertyMulti
    cmsIT8GetPropertyMulti(hIT8, buffer, buffer);
    cmsIT8GetPropertyMulti(hIT8, buffer, "");
    cmsIT8GetPropertyMulti(hIT8, "", buffer);
    cmsIT8GetPropertyMulti(hIT8, "", "");

    // Fuzz cmsIT8GetData
    cmsIT8GetData(hIT8, buffer, buffer);
    cmsIT8GetData(hIT8, buffer, "");
    cmsIT8GetData(hIT8, "", buffer);
    cmsIT8GetData(hIT8, "", "");

    // Fuzz cmsIT8GetPatchName
    int patch_idx = (Size > 0) ? Data[0] : 0;
    int patch_idx_neg = -1;
    int patch_idx_large = 999999;

    cmsIT8GetPatchName(hIT8, patch_idx, buffer);
    cmsIT8GetPatchName(hIT8, patch_idx_neg, buffer);
    cmsIT8GetPatchName(hIT8, patch_idx_large, buffer);
    cmsIT8GetPatchName(hIT8, patch_idx, NULL);
    cmsIT8GetPatchName(hIT8, 0, buffer);

    // Fuzz cmsIT8DefineDblFormat
    cmsIT8DefineDblFormat(hIT8, buffer);
    cmsIT8DefineDblFormat(hIT8, "");
    cmsIT8DefineDblFormat(hIT8, NULL);
    cmsIT8DefineDblFormat(hIT8, "%.2f");
    cmsIT8DefineDblFormat(hIT8, "%.10f");

    // Fuzz cmsIT8GetSheetType
    const char *sheet_type = cmsIT8GetSheetType(hIT8);
    if (sheet_type) {
        (void)sheet_type;
    }

    // Fuzz cmsIT8GetProperty
    cmsIT8GetProperty(hIT8, buffer);
    cmsIT8GetProperty(hIT8, "");
    cmsIT8GetProperty(hIT8, "NonExistentProperty");

    // Cleanup using cmsIT8Free
    cmsIT8Free(hIT8);

    return 0;
}