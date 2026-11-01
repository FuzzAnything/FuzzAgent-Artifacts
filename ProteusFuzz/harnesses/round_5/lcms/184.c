// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
// cmsIT8SetData at cmscgats.c:2894:19 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8GetPropertyMulti at cmscgats.c:1600:23 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8GetPatchName at cmscgats.c:2955:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void create_dummy_it8_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal valid IT8 header structure
    const char *header = "IT8.7/0";
    fwrite(header, 1, 7, f);
    
    // Pad to a reasonable size to simulate a real file
    char padding[256] = {0};
    fwrite(padding, 1, sizeof(padding), f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    const char *dummy_file = "./dummy_file";
    create_dummy_it8_file(dummy_file);
    
    // Create a context for IT8 operations
    // cmsCreateContext requires two arguments: Plugin and UserData
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) {
        return 0;
    }
    
    // Use cmsIT8Alloc with the context
    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (!hIT8) {
        cmsDeleteContext(ContextID);
        return 0;
    }
    
    // Extract strings from input for API calls
    // Use safe bounds to prevent out-of-bounds access
    size_t max_len = Size > 100 ? 100 : Size;
    char key[101] = {0};
    char subkey[101] = {0};
    char sample[101] = {0};
    char patch[101] = {0};
    char value[101] = {0};
    char prop[101] = {0};
    
    if (Size > 0) {
        size_t copy_len = Size < 100 ? Size : 100;
        memcpy(key, Data, copy_len);
        key[copy_len] = '\0';
    }
    
    if (Size > 1) {
        size_t copy_len = (Size - 1) < 100 ? (Size - 1) : 100;
        memcpy(subkey, Data + 1, copy_len);
        subkey[copy_len] = '\0';
    }
    
    if (Size > 2) {
        size_t copy_len = (Size - 2) < 100 ? (Size - 2) : 100;
        memcpy(sample, Data + 2, copy_len);
        sample[copy_len] = '\0';
    }
    
    if (Size > 3) {
        size_t copy_len = (Size - 3) < 100 ? (Size - 3) : 100;
        memcpy(patch, Data + 3, copy_len);
        patch[copy_len] = '\0';
    }
    
    if (Size > 4) {
        size_t copy_len = (Size - 4) < 100 ? (Size - 4) : 100;
        memcpy(value, Data + 4, copy_len);
        value[copy_len] = '\0';
    }
    
    if (Size > 5) {
        size_t copy_len = (Size - 5) < 100 ? (Size - 5) : 100;
        memcpy(prop, Data + 5, copy_len);
        prop[copy_len] = '\0';
    }
    
    // Test cmsIT8GetPropertyMulti
    const char *prop_result = cmsIT8GetPropertyMulti(hIT8, key, subkey);
    (void)prop_result;
    
    // Test cmsIT8FindDataFormat
    int format_id = cmsIT8FindDataFormat(hIT8, sample);
    (void)format_id;
    
    // Test cmsIT8GetPatchName
    char buffer[256] = {0};
    const char *patch_name = cmsIT8GetPatchName(hIT8, (int)(Data[0] % 10), buffer);
    if (!patch_name) {
        patch_name = cmsIT8GetPatchName(hIT8, (int)(Data[0] % 10), NULL);
    }
    (void)patch_name;
    
    // Test cmsIT8SetData
    cmsBool set_data_result = cmsIT8SetData(hIT8, patch, sample, value);
    (void)set_data_result;
    
    // Test cmsstrcasecmp
    int str_result = cmsstrcasecmp(key, subkey);
    (void)str_result;
    
    // Test cmsIT8SetPropertyDbl
    cmsFloat64Number dbl_val = (Size > 0) ? (cmsFloat64Number)Data[0] : 0.0;
    cmsBool set_prop_result = cmsIT8SetPropertyDbl(hIT8, prop, dbl_val);
    (void)set_prop_result;
    
    // Cleanup
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);
    
    return 0;
}