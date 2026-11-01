// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8LoadFromFile at cmscgats.c:2603:22 in lcms2.h
// cmsIT8GetProperty at cmscgats.c:1578:23 in lcms2.h
// cmsIT8GetData at cmscgats.c:2862:23 in lcms2.h
// cmsIT8GetPropertyDbl at cmscgats.c:1591:28 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8GetDataDbl at cmscgats.c:2883:28 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static const char* extract_string_from_fuzz(const uint8_t *Data, size_t Size, size_t *out_len) {
    if (Size == 0) {
        *out_len = 0;
        return NULL;
    }
    size_t len = 0;
    while (len < Size && Data[len] != '\0') len++;
    if (len == 0) {
        *out_len = 0;
        return NULL;
    }
    *out_len = len;
    return (const char*)Data;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsHANDLE hIT8 = NULL;
    cmsContext ContextID = cmsCreateContext(NULL, NULL);

    // Write fuzz data to a dummy file to attempt IT8 loading
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Try to load IT8 from the dummy file using cmsIT8LoadFromFile
    if (ContextID) {
        hIT8 = cmsIT8LoadFromFile(ContextID, "./dummy_file");
    }

    // Extract potential strings from fuzz input for property, patch, and sample names
    size_t prop_len, patch_len, sample_len;
    const char* prop = extract_string_from_fuzz(Data, Size, &prop_len);
    const char* patch = extract_string_from_fuzz(Data + 1, Size - 1, &patch_len);
    const char* sample = extract_string_from_fuzz(Data + 2, Size - 2, &sample_len);

    // Ensure we have valid C-strings if extraction returned non-NULL but empty
    const char* safe_prop = (prop && prop_len > 0) ? prop : "";
    const char* safe_patch = (patch && patch_len > 0) ? patch : "";
    const char* safe_sample = (sample && sample_len > 0) ? sample : "";

    // Fuzz cmsIT8GetProperty
    if (hIT8) {
        const char* res1 = cmsIT8GetProperty(hIT8, safe_prop);
        (void)res1; 
    }

    // Fuzz cmsIT8GetData
    if (hIT8) {
        const char* res2 = cmsIT8GetData(hIT8, safe_patch, safe_sample);
        (void)res2;
    }

    // Fuzz cmsIT8GetPropertyDbl
    if (hIT8) {
        cmsFloat64Number res3 = cmsIT8GetPropertyDbl(hIT8, safe_prop);
        (void)res3;
    }

    // Fuzz cmsIT8SetDataDbl
    if (hIT8) {
        cmsFloat64Number val = 0.0;
        if (Size > 8) {
            // Read a double from the buffer safely
            cmsFloat64Number temp_val;
            if (memcpy(&temp_val, Data + 4, sizeof(cmsFloat64Number)) == sizeof(cmsFloat64Number)) {
                val = temp_val;
            }
        }
        cmsBool res4 = cmsIT8SetDataDbl(hIT8, safe_patch, safe_sample, val);
        (void)res4;
    }

    // Fuzz cmsIT8GetDataDbl
    if (hIT8) {
        cmsFloat64Number res5 = cmsIT8GetDataDbl(hIT8, safe_patch, safe_sample);
        (void)res5;
    }

    // Fuzz cmsIT8SetPropertyDbl
    if (hIT8) {
        cmsFloat64Number val2 = 0.0;
        if (Size > 12) {
            cmsFloat64Number temp_val;
            if (memcpy(&temp_val, Data + 8, sizeof(cmsFloat64Number)) == sizeof(cmsFloat64Number)) {
                val2 = temp_val;
            }
        }
        cmsBool res6 = cmsIT8SetPropertyDbl(hIT8, safe_prop, val2);
        (void)res6;
    }

    // Cleanup
    if (hIT8) cmsIT8Free(hIT8);
    if (ContextID) cmsDeleteContext(ContextID);

    return 0;
}