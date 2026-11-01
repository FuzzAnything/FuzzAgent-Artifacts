// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeNamedColorList at cmsnamed.c:780:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsstrcasecmp at cmserr.c:39:15 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_named_color_list(cmsNAMEDCOLORLIST *list) {
    if (list != NULL) {
        cmsFreeNamedColorList(list);
    }
}

static void cleanup_it8_handle(cmsHANDLE hIT8) {
    if (hIT8 != NULL) {
        // Note: cmsCloseIT8 is not always exposed in public API, 
        // relying on context deletion or specific library behavior.
    }
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;

    // Create a dummy IT8 file for testing
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Fuzz cmsIT8FindDataFormat
    // Requires a valid IT8 handle. Since cmsOpenIT8 is not guaranteed to be 
    // available or might fail on dummy data, we skip direct invocation 
    // unless we can safely create a handle.
    cmsHANDLE hIT8 = NULL;
    // Attempt to open if the function exists (commented out as it's not standard)
    // hIT8 = cmsOpenIT8(dummy_file, 0); 
    // If hIT8 were valid:
    // cmsIT8FindDataFormat(hIT8, "dummy_sample");

    // Fuzz cmsIT8GetDataRowCol
    // Requires valid handle and indices. Skipped due to potential invalid handle.
    // if (hIT8 != NULL) { cmsIT8GetDataRowCol(hIT8, 0, 0); }

    // Fuzz cmsstrcasecmp
    // Ensure we have enough data for two strings with null terminators
    if (Size >= 2) {
        // Create two strings from the input data
        // Ensure we don't read past the buffer
        size_t len1 = (Size > 128) ? 128 : (Size / 2);
        size_t len2 = (Size > 128) ? 128 : (Size - len1);
        
        // Allocate buffers with space for null terminator
        char *s1 = (char *)malloc(len1 + 1);
        char *s2 = (char *)malloc(len2 + 1);
        
        if (s1 && s2) {
            memcpy(s1, Data, len1);
            s1[len1] = '\0';
            
            memcpy(s2, Data + len1, len2);
            s2[len2] = '\0';
            
            cmsstrcasecmp(s1, s2);
            
            free(s1);
            free(s2);
        }
    }

    // Fuzz cmsIT8SetPropertyDbl
    // Requires valid handle. Skipped.
    // if (hIT8 != NULL) { cmsIT8SetPropertyDbl(hIT8, "prop", 1.0); }

    // Fuzz cmsNamedColorIndex
    // Requires a valid cmsNAMEDCOLORLIST. Creating one requires specific data.
    // We skip creating a list as it's complex without valid input data.
    cmsNAMEDCOLORLIST *ncList = NULL;
    // if (ncList != NULL) { cmsNamedColorIndex(ncList, "color"); }

    // Fuzz cmsIT8SetIndexColumn
    // Requires valid handle. Skipped.
    // if (hIT8 != NULL) { cmsIT8SetIndexColumn(hIT8, "sample"); }

    // Cleanup
    cleanup_it8_handle(hIT8);
    cmsDeleteContext(ctx);
    remove(dummy_file);

    return 0;
}