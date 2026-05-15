// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cmsMallocZero at cmserr.c:272:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_cmsRealloc(const uint8_t *data, size_t size, cmsContext context) {
    if (size < 8) return;
    
    cmsUInt32Number initial_size = *(const cmsUInt32Number *)data % 1024;
    cmsUInt32Number new_size = *(const cmsUInt32Number *)(data + 4) % 2048;
    
    void *ptr = _cmsMalloc(context, initial_size);
    if (ptr != NULL) {
        void *new_ptr = _cmsRealloc(context, ptr, new_size);
        if (new_ptr == NULL && new_size > 0) {
            _cmsFree(context, ptr);
        } else {
            _cmsFree(context, new_ptr);
        }
    }
}

static void fuzz_cmsMalloc(const uint8_t *data, size_t size, cmsContext context) {
    if (size < 4) return;
    
    cmsUInt32Number alloc_size = *(const cmsUInt32Number *)data % 4096;
    void *ptr = _cmsMalloc(context, alloc_size);
    if (ptr != NULL) {
        _cmsFree(context, ptr);
    }
}

static void fuzz_cmsCreateNULLProfile(void) {
    cmsHPROFILE profile = cmsCreateNULLProfile();
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static void fuzz_cmsMallocZero(const uint8_t *data, size_t size, cmsContext context) {
    if (size < 4) return;
    
    cmsUInt32Number alloc_size = *(const cmsUInt32Number *)data % 4096;
    void *ptr = _cmsMallocZero(context, alloc_size);
    if (ptr != NULL) {
        _cmsFree(context, ptr);
    }
}

static void fuzz_cmsDupMem(const uint8_t *data, size_t size, cmsContext context) {
    if (size < 8) return;
    
    cmsUInt32Number dup_size = *(const cmsUInt32Number *)data % 1024;
    if (dup_size > size - 4) {
        dup_size = size - 4;
    }
    
    if (dup_size > 0) {
        const void *src = data + 4;
        void *dst = _cmsDupMem(context, src, dup_size);
        if (dst != NULL) {
            _cmsFree(context, dst);
        }
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t operation = Data[0] % 5;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    switch (operation) {
        case 0:
            fuzz_cmsRealloc(remaining_data, remaining_size, context);
            break;
        case 1:
            fuzz_cmsMalloc(remaining_data, remaining_size, context);
            break;
        case 2:
            fuzz_cmsCreateNULLProfile();
            break;
        case 3:
            fuzz_cmsMallocZero(remaining_data, remaining_size, context);
            break;
        case 4:
            fuzz_cmsDupMem(remaining_data, remaining_size, context);
            break;
    }
    
    cmsDeleteContext(context);
    return 0;
}