// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8LoadFromMem at cmscgats.c:2556:22 in lcms2.h
// cmsIT8SetTableByLabel at cmscgats.c:2993:15 in lcms2.h
// cmsIT8SetDataDbl at cmscgats.c:2940:19 in lcms2.h
// cmsIT8FindDataFormat at cmscgats.c:2805:15 in lcms2.h
// cmsIT8SetDataRowCol at cmscgats.c:2838:19 in lcms2.h
// cmsIT8SetPropertyDbl at cmscgats.c:1543:19 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHANDLE create_dummy_it8_handle(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return NULL;
    }

    // Write data to a dummy file
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return NULL;
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Use cmsIT8LoadFromMem to load data directly from memory
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(NULL, Data, Size);
    if (!hIT8) return NULL;

    return hIT8;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHANDLE hIT8 = create_dummy_it8_handle(Data, Size);
    if (!hIT8) return 0;

    // Fuzz cmsIT8SetTableByLabel
    const char *setNames[] = {"SET1", "SET2", "SET3", NULL};
    const char *fieldNames[] = {"LABEL", "FIELD1", "FIELD2", NULL};
    const char *typeNames[] = {"TYPE1", "TYPE2", "TYPE3", NULL};

    for (int i = 0; setNames[i]; i++) {
        for (int j = 0; fieldNames[j]; j++) {
            for (int k = 0; typeNames[k]; k++) {
                cmsIT8SetTableByLabel(hIT8, setNames[i], fieldNames[j], typeNames[k]);
            }
        }
    }

    // Fuzz cmsIT8SetDataDbl
    const char *patchNames[] = {"PATCH1", "PATCH2", "PATCH3", NULL};
    const char *sampleNames[] = {"SAMPLE1", "SAMPLE2", "SAMPLE3", NULL};
    cmsFloat64Number val = (cmsFloat64Number)Size / 100.0;

    for (int i = 0; patchNames[i]; i++) {
        for (int j = 0; sampleNames[j]; j++) {
            cmsIT8SetDataDbl(hIT8, patchNames[i], sampleNames[j], val);
        }
    }

    // Fuzz cmsIT8FindDataFormat
    for (int i = 0; sampleNames[i]; i++) {
        cmsIT8FindDataFormat(hIT8, sampleNames[i]);
    }

    // Fuzz cmsIT8SetDataRowCol
    const char *stringVals[] = {"VAL1", "VAL2", "VAL3", NULL};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; stringVals[k]; k++) {
                cmsIT8SetDataRowCol(hIT8, i, j, stringVals[k]);
            }
        }
    }

    // Fuzz cmsIT8SetPropertyDbl
    const char *propNames[] = {"PROP1", "PROP2", "PROP3", NULL};
    for (int i = 0; propNames[i]; i++) {
        cmsIT8SetPropertyDbl(hIT8, propNames[i], val);
    }

    // Fuzz cmsIT8SaveToFile
    cmsIT8SaveToFile(hIT8, "./dummy_file");

    // Cleanup using cmsIT8Free
    cmsIT8Free(hIT8);
    return 0;
}