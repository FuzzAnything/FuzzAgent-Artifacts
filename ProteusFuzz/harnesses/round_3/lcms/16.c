// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCIECAM02Done at cmscam02.c:427:16 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* safe_alloc(size_t size) {
    void* ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a context for testing
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        return 0;
    }

    // Fuzz cmsUnregisterPluginsTHR
    // This function resets thread-local plugin overrides.
    cmsUnregisterPluginsTHR(ContextID);

    // Fuzz _cmsFree with NULL pointer (safe)
    _cmsFree(ContextID, NULL);

    // Fuzz _cmsFree with valid pointer
    void* test_ptr = safe_alloc(1024);
    if (test_ptr) {
        _cmsFree(ContextID, test_ptr);
    }

    // Fuzz cmsStageFree with NULL (may cause UB, but we test robustness)
    cmsStage* null_stage = NULL;
    // Only call if we can safely assume it handles NULL, otherwise skip
    // Based on description, passing NULL may cause undefined behavior, so we skip direct NULL call
    // Instead, we try to create a minimal stage if possible, but allocation is complex without full API.
    // We skip cmsStageFree for now to avoid crashes unless we can construct a valid stage.

    // Fuzz cmsCIECAM02Done with NULL (safe)
    cmsHANDLE null_model = NULL;
    cmsCIECAM02Done(null_model);

    // Fuzz cmsIT8Free with NULL (safe)
    cmsHANDLE null_it8 = NULL;
    cmsIT8Free(null_it8);

    // Fuzz cmsGBDFree with NULL (safe)
    cmsHANDLE null_gbd = NULL;
    cmsGBDFree(null_gbd);

    // Cleanup context
    cmsDeleteContext(ContextID);

    return 0;
}