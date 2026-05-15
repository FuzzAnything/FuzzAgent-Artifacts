// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsGetTransformContextID at cmsxform.c:1415:22 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDupContext at cmsplugin.c:893:22 in lcms2.h
// _cmsDupMem at cmserr.c:302:17 in lcms2_plugin.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsPluginTHR at cmsplugin.c:546:19 in lcms2.h
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void* create_test_plugin_data(const uint8_t* data, size_t size, size_t min_size) {
    if (size < min_size) {
        return NULL;
    }
    
    void* plugin = malloc(min_size);
    if (!plugin) {
        return NULL;
    }
    
    memcpy(plugin, data, min_size);
    return plugin;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    // Create initial context
    cmsContext context1 = cmsCreateContext(NULL, NULL);
    if (!context1) {
        return 0;
    }
    
    // Test cmsDupContext with various user data
    void* user_data = (Size > 8) ? (void*)(Data + 4) : NULL;
    cmsContext context2 = cmsDupContext(context1, user_data);
    
    // Test _cmsDupMem with different sizes
    cmsUInt32Number copy_size = (cmsUInt32Number)(Size > 1024 ? 1024 : Size);
    if (copy_size > 0) {
        void* duplicated = _cmsDupMem(context1, Data, copy_size);
        if (duplicated) {
            free(duplicated);
        }
    }
    
    // Test cmsCreateNULLProfileTHR with both contexts
    cmsHPROFILE profile1 = cmsCreateNULLProfileTHR(context1);
    if (profile1) {
        cmsCloseProfile(profile1);
    }
    
    if (context2) {
        cmsHPROFILE profile2 = cmsCreateNULLProfileTHR(context2);
        if (profile2) {
            cmsCloseProfile(profile2);
        }
    }
    
    // Test cmsPluginTHR with various plugin data
    // Create a simple plugin base structure
    size_t base_plugin_size = 64; // Arbitrary size for plugin structure
    void* plugin_data = create_test_plugin_data(Data, Size, base_plugin_size);
    if (plugin_data) {
        // Cast to cmsPluginBase to set required fields
        struct _cmsPluginBaseStruct* plugin_base = (struct _cmsPluginBaseStruct*)plugin_data;
        
        // Set some reasonable values for the plugin structure
        // Use values from fuzzer input when possible
        if (Size >= 16) {
            plugin_base->Magic = *(cmsUInt32Number*)Data;
            plugin_base->ExpectedVersion = *(cmsUInt32Number*)(Data + 4);
            plugin_base->Type = *(cmsUInt32Number*)(Data + 8);
            plugin_base->Next = NULL;
        } else {
            // Default values if not enough data
            plugin_base->Magic = cmsPluginMagicNumber;
            plugin_base->ExpectedVersion = LCMS_VERSION;
            plugin_base->Type = 0;
            plugin_base->Next = NULL;
        }
        
        cmsPluginTHR(context1, plugin_data);
        
        // Try with second context if available
        if (context2) {
            // Create another plugin for second context
            void* plugin_data2 = create_test_plugin_data(Data + 16, Size > 16 ? Size - 16 : 0, base_plugin_size);
            if (plugin_data2) {
                struct _cmsPluginBaseStruct* plugin_base2 = (struct _cmsPluginBaseStruct*)plugin_data2;
                if (Size >= 32) {
                    plugin_base2->Magic = *(cmsUInt32Number*)(Data + 16);
                    plugin_base2->ExpectedVersion = *(cmsUInt32Number*)(Data + 20);
                    plugin_base2->Type = *(cmsUInt32Number*)(Data + 24);
                    plugin_base2->Next = NULL;
                } else {
                    plugin_base2->Magic = cmsPluginMagicNumber;
                    plugin_base2->ExpectedVersion = LCMS_VERSION;
                    plugin_base2->Type = 1;
                    plugin_base2->Next = NULL;
                }
                cmsPluginTHR(context2, plugin_data2);
                free(plugin_data2);
            }
        }
        
        free(plugin_data);
    }
    
    // Create profiles and transform to test cmsGetTransformContextID
    cmsHPROFILE src_profile = cmsCreateNULLProfileTHR(context1);
    if (src_profile) {
        cmsHTRANSFORM transform = cmsCreateTransform(src_profile, TYPE_GRAY_8, 
                                                    src_profile, TYPE_GRAY_8, 
                                                    INTENT_PERCEPTUAL, 0);
        if (transform) {
            cmsContext transform_context = cmsGetTransformContextID(transform);
            (void)transform_context; // Use variable to avoid unused warning
            cmsDeleteTransform(transform);
        }
        cmsCloseProfile(src_profile);
    }
    
    // Clean up contexts
    if (context2) {
        cmsDeleteContext(context2);
    }
    if (context1) {
        cmsDeleteContext(context1);
    }
    
    return 0;
}