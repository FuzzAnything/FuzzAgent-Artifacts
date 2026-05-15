// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsGetTransformMaxWorkers at cmsxform.c:794:26 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
// _cmsLockMutex at cmserr.c:646:19 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsDestroyMutex at cmserr.c:636:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create context with or without plugin based on input data
    void* plugin = NULL;
    void* user_data = NULL;
    
    if (Data[0] & 0x01) {
        // Use some arbitrary pointer as plugin
        plugin = (void*)(Data);
    }
    
    if (Size > 1 && (Data[0] & 0x02)) {
        // Use some user data
        user_data = (void*)(Data + 1);
    }
    
    cmsContext context = cmsCreateContext(plugin, user_data);
    if (!context) {
        // If context creation fails, try with NULL parameters
        context = cmsCreateContext(NULL, NULL);
        if (!context) return 0;
    }
    
    // Create mutex
    void* mutex = _cmsCreateMutex(context);
    
    // Lock mutex if created
    if (mutex) {
        cmsBool locked = _cmsLockMutex(context, mutex);
        (void)locked; // Use result to avoid unused variable warning
        
        // Unlock mutex
        _cmsUnlockMutex(context, mutex);
    }
    
    // Test with NULL transform
    if (Size > 2 && (Data[0] & 0x04)) {
        cmsInt32Number null_workers = _cmsGetTransformMaxWorkers(NULL);
        (void)null_workers;
    }
    
    // Test with arbitrary pointer as transform
    if (Size > 3) {
        // Create a dummy structure on the stack
        char dummy_buffer[256];
        memset(dummy_buffer, 0, sizeof(dummy_buffer));
        
        // Copy some fuzz data into the buffer
        size_t copy_size = Size < sizeof(dummy_buffer) ? Size : sizeof(dummy_buffer);
        memcpy(dummy_buffer, Data, copy_size);
        
        // Cast to transform pointer
        struct _cmstransform_struct* dummy_transform = 
            (struct _cmstransform_struct*)dummy_buffer;
        
        cmsInt32Number max_workers = _cmsGetTransformMaxWorkers(dummy_transform);
        (void)max_workers;
    }
    
    // Destroy mutex if created
    if (mutex) {
        _cmsDestroyMutex(context, mutex);
    }
    
    // Test destroying NULL mutex
    if (Size > 4 && (Data[0] & 0x08)) {
        _cmsDestroyMutex(context, NULL);
    }
    
    // Clean up context
    cmsDeleteContext(context);
    
    // Test additional context creation patterns
    if (Size > 5) {
        // Create another context with different parameters
        cmsContext context2 = cmsCreateContext(NULL, NULL);
        if (context2) {
            void* mutex2 = _cmsCreateMutex(context2);
            if (mutex2) {
                _cmsLockMutex(context2, mutex2);
                _cmsUnlockMutex(context2, mutex2);
                _cmsDestroyMutex(context2, mutex2);
            }
            
            // Test with another arbitrary pointer
            if (Size > 10) {
                char dummy_buffer2[128];
                memset(dummy_buffer2, 0, sizeof(dummy_buffer2));
                size_t copy_size2 = (Size - 5) < sizeof(dummy_buffer2) ? 
                                   (Size - 5) : sizeof(dummy_buffer2);
                memcpy(dummy_buffer2, Data + 5, copy_size2);
                
                struct _cmstransform_struct* dummy_transform2 = 
                    (struct _cmstransform_struct*)dummy_buffer2;
                
                cmsInt32Number workers2 = _cmsGetTransformMaxWorkers(dummy_transform2);
                (void)workers2;
            }
            
            cmsDeleteContext(context2);
        }
    }
    
    return 0;
}