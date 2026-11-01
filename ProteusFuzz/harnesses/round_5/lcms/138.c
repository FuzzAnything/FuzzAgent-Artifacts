// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetComment at cmscgats.c:1522:19 in lcms2.h
// cmsIT8SetPropertyStr at cmscgats.c:1533:19 in lcms2.h
// cmsIT8SetPropertyMulti at cmscgats.c:1570:19 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8EnumDataFormat at cmscgats.c:2650:15 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_fuzz_data(cmsHANDLE hIT8, char **SampleNames, int count) {
    if (hIT8 != NULL) {
        cmsIT8Free(hIT8);
    }
    if (SampleNames != NULL) {
        for (int i = 0; i < count; i++) {
            if (SampleNames[i] != NULL) {
                free(SampleNames[i]);
            }
        }
        free(SampleNames);
    }
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (hIT8 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Ensure the input data is null-terminated for string operations
    char *input_str = (char *)malloc(Size + 1);
    if (input_str == NULL) {
        cleanup_fuzz_data(hIT8, NULL, 0);
        cmsDeleteContext(ContextID);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Fuzz cmsIT8SetComment
    cmsIT8SetComment(hIT8, input_str);

    // Fuzz cmsIT8SetPropertyStr
    cmsIT8SetPropertyStr(hIT8, "Key", input_str);

    // Fuzz cmsIT8SetPropertyMulti
    cmsIT8SetPropertyMulti(hIT8, "Key", "SubKey", input_str);

    // Fuzz cmsIT8SetTableByLabel
    // Using parts of input for different parameters if possible, or fixed strings
    int label_result = cmsIT8SetTableByLabel(hIT8, "Set", "LABEL", "Type");
    (void)label_result;

    // Fuzz cmsIT8EnumDataFormat
    char **SampleNames = NULL;
    int count = cmsIT8EnumDataFormat(hIT8, &SampleNames);
    
    // Cleanup SampleNames if allocated
    if (SampleNames != NULL) {
        for (int i = 0; i < count; i++) {
            if (SampleNames[i] != NULL) {
                free(SampleNames[i]);
            }
        }
        free(SampleNames);
        SampleNames = NULL;
    }

    // Fuzz cmsIT8SaveToFile
    // Use a dummy file path as per instructions
    cmsIT8SaveToFile(hIT8, "./dummy_file");

    // Cleanup
    free(input_str);
    cleanup_fuzz_data(hIT8, NULL, 0);
    cmsDeleteContext(ContextID);

    return 0;
}