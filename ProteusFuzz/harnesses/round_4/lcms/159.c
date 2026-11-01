// Source: Generated Fuzz Driver for cmsPluginTHR
// Target: cmsPluginTHR at cmsplugin.c:546
// Goal: Cover lines 551, 554, 559, 567-612, 615

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Define cmsPluginBase if not fully defined in headers, based on usage
// The header lcms2_plugin.h should define it, but we provide a fallback structure
// to ensure compilation if the header is incomplete or internal.
// Based on cmsplugin.c usage: Magic, ExpectedVersion, Type, Next
struct cmsPluginBase {
    uint32_t Magic;
    uint32_t ExpectedVersion;
    uint32_t Type;
    struct cmsPluginBase *Next;
    // Other fields might exist, but we only need these for the switch and checks
};

static struct cmsPluginBase* create_plugin(uint32_t magic, uint32_t version, uint32_t type, struct cmsPluginBase* next) {
    struct cmsPluginBase* p = (struct cmsPluginBase*)malloc(sizeof(struct cmsPluginBase));
    if (!p) return NULL;
    memset(p, 0, sizeof(struct cmsPluginBase));
    p->Magic = magic;
    p->ExpectedVersion = version;
    p->Type = type;
    p->Next = next;
    return p;
}

static void free_plugins(struct cmsPluginBase* head) {
    while (head) {
        struct cmsPluginBase* next = head->Next;
        free(head);
        head = next;
    }
}

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Constants
    uint32_t valid_magic = cmsPluginMagicNumber;
    uint32_t current_version = LCMS_VERSION;

    struct cmsPluginBase* plugin_list = NULL;
    struct cmsPluginBase* current = NULL;

    // 1. Test Invalid Magic (Line 554)
    plugin_list = create_plugin(0x00000000, current_version, 0, NULL);
    current = plugin_list;

    // 2. Test Invalid Version (Line 559)
    current->Next = create_plugin(valid_magic, current_version + 100, 0, NULL);
    current = current->Next;

    // 3. Test Invalid Type (Line 615)
    current->Next = create_plugin(valid_magic, current_version, 0xFFFFFFFF, NULL);
    current = current->Next;

    // 4. Test Valid Types (Lines 567-612)
    // We try to hit as many cases as possible.
    // Note: The exact values of the enum types are defined in lcms2_plugin.h.
    // We assume the constants are available. If not, we might need to use raw values.
    
    // Case: cmsPluginMemHandlerSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginMemHandlerSig, NULL);
    current = current->Next;

    // Case: cmsPluginInterpolationSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginInterpolationSig, NULL);
    current = current->Next;

    // Case: cmsPluginTagTypeSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginTagTypeSig, NULL);
    current = current->Next;

    // Case: cmsPluginTagSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginTagSig, NULL);
    current = current->Next;

    // Case: cmsPluginFormattersSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginFormattersSig, NULL);
    current = current->Next;

    // Case: cmsPluginRenderingIntentSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginRenderingIntentSig, NULL);
    current = current->Next;

    // Case: cmsPluginParametricCurveSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginParametricCurveSig, NULL);
    current = current->Next;

    // Case: cmsPluginMultiProcessElementSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginMultiProcessElementSig, NULL);
    current = current->Next;

    // Case: cmsPluginOptimizationSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginOptimizationSig, NULL);
    current = current->Next;

    // Case: cmsPluginTransformSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginTransformSig, NULL);
    current = current->Next;

    // Case: cmsPluginMutexSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginMutexSig, NULL);
    current = current->Next;

    // Case: cmsPluginParalellizationSig
    current->Next = create_plugin(valid_magic, current_version, cmsPluginParalellizationSig, NULL);
    current = current->Next;

    // Execute the target function
    cmsBool result = cmsPluginTHR(ctx, plugin_list);
    (void)result;

    // Cleanup
    free_plugins(plugin_list);
    cmsDeleteContext(ctx);

    return 0;
}