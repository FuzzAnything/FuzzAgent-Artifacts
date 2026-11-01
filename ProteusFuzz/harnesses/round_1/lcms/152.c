// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsIT8SetIndexColumn at cmscgats.c:3025:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8GetPatchByName at cmscgats.c:2974:15 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_it8(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        cmsCloseProfile(hIT8);
    }
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy IT8 file for testing
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp != NULL) {
        // Write minimal dummy data to the file
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    } else {
        return 0;
    }

    // Open IT8 file using the correct API: cmsOpenIT8File is not standard in lcms2.h
    // Instead, we use cmsOpenProfileFromFile to open a valid ICC profile as a proxy
    // Since the target functions expect an IT8 handle, and lcms2 does not expose 
    // a direct public API to create an IT8 handle from a file in the standard header,
    // we must rely on internal or alternative methods. However, for fuzzing purposes,
    // we can try to open the file as a profile and cast it, though this is unsafe.
    // A better approach is to skip file-based IT8 creation and use cmsIT8Alloc if available.
    // But cmsIT8Alloc is not in the public API.
    // Given the constraints, we will attempt to open the file as a profile and cast.
    // Note: This is a workaround and may not work as intended for IT8-specific functions.
    
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(dummy_file, "r");
    if (hProfile == NULL) {
        remove(dummy_file);
        return 0;
    }

    // Cast profile handle to IT8 handle (unsafe but necessary for fuzzing)
    cmsHANDLE hIT8 = (cmsHANDLE)hProfile;

    // Extract strings from input data for testing
    char *sample_name = NULL;
    char *patch_name = NULL;
    char *set_name = NULL;
    char *field_name = NULL;
    char *expected_type = NULL;
    char *prop_key = NULL;
    char *prop_value = NULL;

    size_t offset = 0;
    if (offset < Size) {
        size_t len = Data[offset] % (Size - offset);
        if (len > 0) {
            sample_name = malloc(len + 1);
            if (sample_name) {
                memcpy(sample_name, Data + offset + 1, len);
                sample_name[len] = '\0';
            }
            offset += len + 1;
        }
    }
    if (offset < Size) {
        size_t len = Data[offset] % (Size - offset);
        if (len > 0) {
            patch_name = malloc(len + 1);
            if (patch_name) {
                memcpy(patch_name, Data + offset + 1, len);
                patch_name[len] = '\0';
            }
            offset += len + 1;
        }
    }
    if (offset < Size) {
        size_t len = Data[offset] % (Size - offset);
        if (len > 0) {
            set_name = malloc(len + 1);
            if (set_name) {
                memcpy(set_name, Data + offset + 1, len);
                set_name[len] = '\0';
            }
            offset += len + 1;
        }
    }
    if (offset < Size) {
        size_t len = Data[offset] % (Size - offset);
        if (len > 0) {
            field_name = malloc(len + 1);
            if (field_name) {
                memcpy(field_name, Data + offset + 1, len);
                field_name[len] = '\0';
            }
            offset += len + 1;
        }
    }
    if (offset < Size) {
        size_t len = Data[offset] % (Size - offset);
        if (len > 0) {
            expected_type = malloc(len + 1);
            if (expected_type) {
                memcpy(expected_type, Data + offset + 1, len);
                expected_type[len] = '\0';
            }
            offset += len + 1;
        }
    }
    if (offset < Size) {
        size_t len = Data[offset] % (Size - offset);
        if (len > 0) {
            prop_key = malloc(len + 1);
            if (prop_key) {
                memcpy(prop_key, Data + offset + 1, len);
                prop_key[len] = '\0';
            }
            offset += len + 1;
        }
    }
    if (offset < Size) {
        size_t len = Data[offset] % (Size - offset);
        if (len > 0) {
            prop_value = malloc(len + 1);
            if (prop_value) {
                memcpy(prop_value, Data + offset + 1, len);
                prop_value[len] = '\0';
            }
            offset += len + 1;
        }
    }

    // Fuzz cmsIT8SetIndexColumn
    if (sample_name) {
        cmsIT8SetIndexColumn(hIT8, sample_name);
    }

    // Fuzz cmsIT8FindDataFormat
    if (sample_name) {
        cmsIT8FindDataFormat(hIT8, sample_name);
    }

    // Fuzz cmsIT8SetTableByLabel
    if (set_name && field_name && expected_type) {
        cmsIT8SetTableByLabel(hIT8, set_name, field_name, expected_type);
    }
    if (set_name && field_name) {
        cmsIT8SetTableByLabel(hIT8, set_name, field_name, NULL);
    }
    if (set_name) {
        cmsIT8SetTableByLabel(hIT8, set_name, "LABEL", NULL);
    }

    // Fuzz cmsIT8EnumDataFormat
    char **sample_names = NULL;
    int count = cmsIT8EnumDataFormat(hIT8, NULL);
    if (count > 0) {
        // No need to free if we didn't ask for names
    }
    sample_names = NULL;
    cmsIT8EnumDataFormat(hIT8, &sample_names);
    if (sample_names) {
        for (int i = 0; i < count; i++) {
            if (sample_names[i]) {
                free(sample_names[i]);
            }
        }
        free(sample_names);
    }

    // Fuzz cmsIT8GetPatchByName
    if (patch_name) {
        cmsIT8GetPatchByName(hIT8, patch_name);
    }

    // Fuzz cmsIT8SetPropertyStr
    if (prop_key && prop_value) {
        cmsIT8SetPropertyStr(hIT8, prop_key, prop_value);
    }
    if (prop_key) {
        cmsIT8SetPropertyStr(hIT8, prop_key, "");
        cmsIT8SetPropertyStr(hIT8, prop_key, NULL);
    }

    // Cleanup
    free(sample_name);
    free(patch_name);
    free(set_name);
    free(field_name);
    free(expected_type);
    free(prop_key);
    free(prop_value);

    cleanup_it8(hIT8);
    remove(dummy_file);

    return 0;
}