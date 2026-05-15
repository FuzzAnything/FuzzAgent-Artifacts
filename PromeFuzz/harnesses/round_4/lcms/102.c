// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsRealloc at cmserr.c:286:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGBDAlloc at cmssm.c:302:22 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsGBDFree at cmssm.c:313:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* custom_malloc(cmsContext ContextID, cmsUInt32Number size) {
    (void)ContextID;
    return malloc(size);
}

static void* custom_calloc(cmsContext ContextID, cmsUInt32Number num, cmsUInt32Number size) {
    (void)ContextID;
    return calloc(num, size);
}

static void* custom_realloc(cmsContext ContextID, void* ptr, cmsUInt32Number size) {
    (void)ContextID;
    return realloc(ptr, size);
}

static void custom_free(cmsContext ContextID, void* ptr) {
    (void)ContextID;
    free(ptr);
}

static void* custom_dup(cmsContext ContextID, const void* ptr, cmsUInt32Number size) {
    (void)ContextID;
    if (!ptr) return NULL;
    void* new_ptr = malloc(size);
    if (new_ptr) {
        memcpy(new_ptr, ptr, size);
    }
    return new_ptr;
}

static void setup_custom_allocator(cmsContext ctx) {
    cmsPluginMemHandler plugin;
    memset(&plugin, 0, sizeof(plugin));
    
    plugin.base.Magic = cmsPluginMagicNumber;
    plugin.base.Type = cmsPluginMemHandlerSig;
    plugin.base.Next = NULL;
    
    plugin.MallocPtr = custom_malloc;
    plugin.FreePtr = custom_free;
    plugin.ReallocPtr = custom_realloc;
    plugin.CallocPtr = custom_calloc;
    plugin.DupPtr = custom_dup;
    
    cmsPluginTHR(ctx, (cmsPluginBase*)&plugin);
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    setup_custom_allocator(ctx);
    
    uint8_t operation = Data[0] % 6;
    size_t data_offset = 1;
    
    switch (operation) {
        case 0: { // _cmsMalloc
            cmsUInt32Number alloc_size = 0;
            if (Size - data_offset >= sizeof(cmsUInt32Number)) {
                memcpy(&alloc_size, Data + data_offset, sizeof(cmsUInt32Number));
            } else if (Size > data_offset) {
                alloc_size = Data[data_offset];
            }
            alloc_size = alloc_size % 65536;
            
            void* ptr = _cmsMalloc(ctx, alloc_size);
            if (ptr) {
                _cmsFree(ctx, ptr);
            }
            break;
        }
        
        case 1: { // _cmsCalloc
            cmsUInt32Number num = 0, elem_size = 0;
            if (Size - data_offset >= 2 * sizeof(cmsUInt32Number)) {
                memcpy(&num, Data + data_offset, sizeof(cmsUInt32Number));
                memcpy(&elem_size, Data + data_offset + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
            } else if (Size - data_offset >= 2) {
                num = Data[data_offset];
                elem_size = Data[data_offset + 1];
            }
            num = num % 256;
            elem_size = elem_size % 256;
            
            void* ptr = _cmsCalloc(ctx, num, elem_size);
            if (ptr) {
                _cmsFree(ctx, ptr);
            }
            break;
        }
        
        case 2: { // _cmsRealloc
            cmsUInt32Number old_size = 0, new_size = 0;
            if (Size - data_offset >= 2 * sizeof(cmsUInt32Number)) {
                memcpy(&old_size, Data + data_offset, sizeof(cmsUInt32Number));
                memcpy(&new_size, Data + data_offset + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
            } else if (Size - data_offset >= 2) {
                old_size = Data[data_offset];
                new_size = Data[data_offset + 1];
            }
            old_size = old_size % 4096;
            new_size = new_size % 4096;
            
            void* ptr = _cmsMalloc(ctx, old_size);
            if (ptr) {
                void* new_ptr = _cmsRealloc(ctx, ptr, new_size);
                if (new_ptr) {
                    _cmsFree(ctx, new_ptr);
                } else {
                    // Only free the original pointer if realloc failed and new_size is not 0
                    // When new_size is 0, realloc acts like free and returns NULL
                    if (new_size != 0) {
                        _cmsFree(ctx, ptr);
                    }
                }
            }
            break;
        }
        
        case 3: { // cmsGBDFree
            cmsHANDLE gbd = cmsGBDAlloc(ctx);
            if (gbd) {
                cmsGBDFree(gbd);
            }
            break;
        }
        
        case 4: { // _cmsFree combinations
            cmsUInt32Number alloc_size = 0;
            if (Size - data_offset >= sizeof(cmsUInt32Number)) {
                memcpy(&alloc_size, Data + data_offset, sizeof(cmsUInt32Number));
            } else if (Size > data_offset) {
                alloc_size = Data[data_offset];
            }
            alloc_size = alloc_size % 4096;
            
            void* ptr1 = _cmsMalloc(ctx, alloc_size);
            void* ptr2 = _cmsCalloc(ctx, 1, alloc_size);
            
            _cmsFree(ctx, ptr1);
            _cmsFree(ctx, ptr2);
            _cmsFree(ctx, NULL);
            break;
        }
        
        case 5: { // cmsGBDAlloc
            cmsHANDLE gbd1 = cmsGBDAlloc(ctx);
            cmsHANDLE gbd2 = cmsGBDAlloc(ctx);
            
            if (gbd1) cmsGBDFree(gbd1);
            if (gbd2) cmsGBDFree(gbd2);
            break;
        }
    }
    
    cmsDeleteContext(ctx);
    return 0;
}