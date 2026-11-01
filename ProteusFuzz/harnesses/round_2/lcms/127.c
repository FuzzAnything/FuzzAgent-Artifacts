// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_properties(char ***PropertyNames, cmsUInt32Number Count) {
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

static void cleanup_subproperties(const char ***SubpropertyNames, cmsUInt32Number Count) {
    if (SubpropertyNames && *SubpropertyNames) {
        for (cmsUInt32Number i = 0; i < Count; i++) {
            if ((*SubpropertyNames)[i]) {
                free((void *)(*SubpropertyNames)[i]);
            }
        }
        free((void *)*SubpropertyNames);
        *SubpropertyNames = NULL;
    }
}

int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (!ContextID) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ContextID);
    if (!hIT8) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    cmsUInt32Number seed = *(const cmsUInt32Number *)Data;
    cmsUInt32Number op = seed % 6;

    cmsUInt32Number tableIdx = 0;
    cmsUInt32Number propCount = 0;
    char **propNames = NULL;
    const char **subPropNames = NULL;
    cmsUInt32Number bytesNeeded = 0;
    void *memBuf = NULL;

    switch (op) {
        case 0: {
            cmsUInt32Number count = cmsIT8TableCount(hIT8);
            (void)count;
            break;
        }
        case 1: {
            propCount = cmsIT8EnumProperties(hIT8, &propNames);
            cleanup_properties(&propNames, propCount);
            break;
        }
        case 2: {
            const char *propName = "TestProp";
            cmsUInt32Number val = seed;
            cmsBool res = cmsIT8SetPropertyHex(hIT8, propName, val);
            (void)res;
            break;
        }
        case 3: {
            cmsInt32Number res = cmsIT8SetTable(hIT8, tableIdx);
            (void)res;
            break;
        }
        case 4: {
            const char *propName = "TestProp";
            cmsUInt32Number count = cmsIT8EnumPropertyMulti(hIT8, propName, &subPropNames);
            cleanup_subproperties(&subPropNames, count);
            break;
        }
        case 5: {
            cmsBool res = cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
            if (res && bytesNeeded > 0) {
                memBuf = malloc(bytesNeeded);
                if (memBuf) {
                    cmsIT8SaveToMem(hIT8, memBuf, &bytesNeeded);
                    free(memBuf);
                    memBuf = NULL;
                }
            }
            break;
        }
    }

    cmsIT8Free(hIT8);
    cmsDeleteContext(ContextID);
    return 0;
}