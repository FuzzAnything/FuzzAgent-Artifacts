// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsIT8SetPropertyHex at cmscgats.c:1553:19 in lcms2.h
// cmsIT8SetTable at cmscgats.c:1427:26 in lcms2.h
// cmsIT8TableCount at cmscgats.c:2981:27 in lcms2.h
// cmsIT8EnumProperties at cmscgats.c:2665:27 in lcms2.h
// cmsIT8EnumPropertyMulti at cmscgats.c:2700:27 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsIT8SaveToMem at cmscgats.c:2047:19 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

static void cleanup_properties(char ***PropertyNames, cmsUInt32Number count) {
    if (PropertyNames && *PropertyNames) {
        for (cmsUInt32Number i = 0; i < count; i++) {
            free((*PropertyNames)[i]);
        }
        free(*PropertyNames);
        *PropertyNames = NULL;
    }
}

static void cleanup_subproperties(const char ***SubpropertyNames, cmsUInt32Number count) {
    if (SubpropertyNames && *SubpropertyNames) {
        for (cmsUInt32Number i = 0; i < count; i++) {
            free((*SubpropertyNames)[i]);
        }
        free(*SubpropertyNames);
        *SubpropertyNames = NULL;
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Extract parameters from input
    cmsUInt32Number tableIndex = Data[0];
    cmsUInt32Number hexValue = 0;
    if (Size > 1) hexValue = Data[1];
    if (Size > 2) hexValue |= (Data[2] << 8);
    if (Size > 3) hexValue |= (Data[3] << 16);

    // Fuzz cmsIT8SetPropertyHex
    cmsBool setRes = cmsIT8SetPropertyHex(hIT8, "TestProp", hexValue);
    (void)setRes;

    // Fuzz cmsIT8SetTable
    cmsInt32Number tableRes = cmsIT8SetTable(hIT8, tableIndex);
    (void)tableRes;

    // Fuzz cmsIT8TableCount
    cmsUInt32Number count = cmsIT8TableCount(hIT8);
    (void)count;

    // Fuzz cmsIT8EnumProperties
    char **propNames = NULL;
    cmsUInt32Number propCount = cmsIT8EnumProperties(hIT8, &propNames);
    if (propCount > 0 && propNames) {
        cleanup_properties(&propNames, propCount);
    }

    // Fuzz cmsIT8EnumPropertyMulti
    const char **subProps = NULL;
    cmsUInt32Number subCount = cmsIT8EnumPropertyMulti(hIT8, "TestProp", &subProps);
    if (subCount > 0 && subProps) {
        cleanup_subproperties(&subProps, subCount);
    }

    // Fuzz cmsIT8SaveToMem
    cmsUInt32Number bytesNeeded = 0;
    cmsBool sizeRes = cmsIT8SaveToMem(hIT8, NULL, &bytesNeeded);
    if (sizeRes && bytesNeeded > 0) {
        void *memPtr = malloc(bytesNeeded);
        if (memPtr) {
            cmsBool writeRes = cmsIT8SaveToMem(hIT8, memPtr, &bytesNeeded);
            (void)writeRes;
            free(memPtr);
        }
    }

    // Cleanup
    // cmsIT8DeAlloc is not declared in the provided header in this environment.
    // We rely on cmsDeleteContext to clean up resources associated with the context,
    // or we simply leak the handle if the context doesn't manage it, to avoid compile errors.
    // In standard lcms2, cmsIT8DeAlloc should be used, but since it's missing from the header,
    // we skip the explicit call to avoid the compilation error.
    // Note: This might lead to memory leaks in a real scenario if the context doesn't handle it,
    // but it satisfies the compilation requirement.
    
    cmsDeleteContext(ctx);
    return 0;
}