// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_property_array(char ***PropertyNames, cmsUInt32Number Count) {
    if (PropertyNames && *PropertyNames) {
        for (cmsUInt32Number i = 0; i < Count; i++) {
            if ((*PropertyNames)[i]) {
                free((*PropertyNames)[i]);
            }
        }
        free(*PropertyNames);
        *PropertyNames = NULL;
    }
}

static void cleanup_subproperty_array(const char ***SubpropertyNames, cmsUInt32Number Count) {
    if (SubpropertyNames && *SubpropertyNames) {
        for (cmsUInt32Number i = 0; i < Count; i++) {
            if ((*SubpropertyNames)[i]) {
                free((void*)(*SubpropertyNames)[i]);
            }
        }
        free((void*)*SubpropertyNames);
        *SubpropertyNames = NULL;
    }
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize context
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    // Create IT8 handle with context
    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (!hIT8) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Use input data to drive fuzzing logic
    cmsUInt32Number val = 0;
    if (Size >= 4) {
        val = *(const cmsUInt32Number*)(Data);
    } else if (Size > 0) {
        val = Data[0];
    }

    // Fuzz cmsIT8SetPropertyHex
    const char *propName = "TestProp";
    if (Size >= 10 && Data[0] == 0xFF) {
        propName = "CustomProp";
    }
    cmsIT8SetPropertyHex(hIT8, propName, val);

    // Fuzz cmsIT8SetTable
    cmsUInt32Number tableIdx = 0;
    if (Size >= 4) {
        tableIdx = *(const cmsUInt32Number*)(Data + 4) % 10;
    } else if (Size > 4) {
        tableIdx = Data[4] % 10;
    }
    cmsInt32Number retTable = cmsIT8SetTable(hIT8, tableIdx);
    (void)retTable;

    // Try setting another table to trigger allocation logic
    cmsIT8SetTable(hIT8, 1);
    cmsIT8SetTable(hIT8, 2);

    // Fuzz cmsIT8TableCount
    cmsUInt32Number tableCount = cmsIT8TableCount(hIT8);
    (void)tableCount;

    // Fuzz cmsIT8EnumProperties
    char **PropertyNames = NULL;
    cmsUInt32Number propCount = cmsIT8EnumProperties(hIT8, &PropertyNames);
    if (propCount > 0 && PropertyNames) {
        cleanup_property_array(&PropertyNames, propCount);
    }

    // Fuzz cmsIT8EnumPropertyMulti
    const char **SubpropertyNames = NULL;
    cmsUInt32Number subPropCount = cmsIT8EnumPropertyMulti(hIT8, propName, &SubpropertyNames);
    if (subPropCount > 0 && SubpropertyNames) {
        cleanup_subproperty_array(&SubpropertyNames, subPropCount);
    }

    // Fuzz cmsIT8SaveToMem
    cmsUInt32Number BytesNeeded = 0;
    cmsBool success = cmsIT8SaveToMem(hIT8, NULL, &BytesNeeded);
    if (success && BytesNeeded > 0) {
        void *MemPtr = malloc(BytesNeeded);
        if (MemPtr) {
            success = cmsIT8SaveToMem(hIT8, MemPtr, &BytesNeeded);
            (void)success;
            free(MemPtr);
        }
    }

    // Cleanup: Use cmsIT8Free to release the IT8 handle
    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);

    return 0;
}