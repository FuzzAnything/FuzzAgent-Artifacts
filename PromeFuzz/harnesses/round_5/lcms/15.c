// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsUnregisterPlugins at cmsplugin.c:627:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetSupportedIntentsTHR at cmscnvrt.c:1157:27 in lcms2.h
// cmsPlugin at cmsplugin.c:541:19 in lcms2.h
// cmsUnregisterPluginsTHR at cmsplugin.c:780:16 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// _cmsCreateMutex at cmserr.c:627:17 in lcms2_plugin.h
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

static void* create_dummy_plugin(const uint8_t* data, size_t size) {
    if (size < sizeof(cmsPluginBase)) {
        return NULL;
    }
    
    cmsPluginBase* plugin = (cmsPluginBase*)malloc(size);
    if (!plugin) {
        return NULL;
    }
    
    memcpy(plugin, data, size);
    plugin->Next = NULL;
    return plugin;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;
    
    switch (selector) {
        case 0: {
            if (Size - data_offset >= sizeof(cmsUInt32Number)) {
                cmsUInt32Number nMax = *(cmsUInt32Number*)(Data + data_offset);
                data_offset += sizeof(cmsUInt32Number);
                
                cmsUInt32Number* codes = NULL;
                char** descriptions = NULL;
                
                if (nMax > 0 && nMax < 1024) {
                    codes = (cmsUInt32Number*)calloc(nMax, sizeof(cmsUInt32Number));
                    descriptions = (char**)calloc(nMax, sizeof(char*));
                    
                    if (codes && descriptions) {
                        // Do NOT allocate memory for individual description strings
                        // cmsGetSupportedIntentsTHR will fill these pointers with
                        // static strings from the library
                        cmsGetSupportedIntentsTHR(context, nMax, codes, descriptions);
                        
                        // Only free the arrays, not the individual strings
                        // The strings are owned by the library
                    }
                    
                    free(codes);
                    free(descriptions);
                }
            }
            break;
        }
        
        case 1: {
            void* plugin = create_dummy_plugin(Data + data_offset, Size - data_offset);
            if (plugin) {
                cmsPlugin(plugin);
                free(plugin);
            }
            break;
        }
        
        case 2: {
            cmsUnregisterPluginsTHR(context);
            break;
        }
        
        case 3: {
            void* plugin = create_dummy_plugin(Data + data_offset, Size - data_offset);
            if (plugin) {
                cmsPluginTHR(context, plugin);
                free(plugin);
            }
            break;
        }
        
        case 4: {
            void* mutex = _cmsCreateMutex(context);
            if (mutex) {
                // Mutex should be destroyed by the context cleanup
            }
            break;
        }
        
        case 5: {
            cmsUnregisterPlugins();
            break;
        }
    }
    
    cmsDeleteContext(context);
    return 0;
}