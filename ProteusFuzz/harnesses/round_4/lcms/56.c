// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// _cmsUnlockMutex at cmserr.c:655:16 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void* dummy_mutex = NULL;

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ContextID = NULL;
    cmsUInt16Number alarms[cmsMAXCHANNELS];
    cmsUInt16Number new_alarms[cmsMAXCHANNELS];
    cmsUInt32Number error_code;
    const char *error_text;

    // Initialize alarms with input data if available
    for (int i = 0; i < cmsMAXCHANNELS; i++) {
        if (Size > (size_t)(i * 2)) {
            new_alarms[i] = Data[i * 2] | (Data[i * 2 + 1] << 8);
        } else {
            new_alarms[i] = 0;
        }
    }

    // Create a context for thread-safe operations
    ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) {
        // Fallback to global context if creation fails
        ContextID = NULL;
    }

    // Test 1: cmsSetAlarmCodes (Global)
    cmsSetAlarmCodes(new_alarms);

    // Test 2: cmsSetAlarmCodesTHR (Thread-specific)
    if (ContextID != NULL) {
        cmsSetAlarmCodesTHR(ContextID, new_alarms);
    }

    // Test 3: cmsGetAlarmCodes (Global)
    cmsGetAlarmCodes(alarms);

    // Test 4: cmsGetAlarmCodesTHR (Thread-specific)
    if (ContextID != NULL) {
        cmsGetAlarmCodesTHR(ContextID, alarms);
    }

    // Test 5: cmsSignalError
    // Prepare error code and text from input
    if (Size >= 4) {
        error_code = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    } else {
        error_code = 0;
    }

    // Construct a safe error string from input (limit length)
    size_t text_len = Size > 100 ? 100 : Size;
    char safe_text[101];
    memcpy(safe_text, Data, text_len);
    safe_text[text_len] = '\0';
    // Ensure no null bytes in the middle for printf safety if needed, though %s handles it
    for (size_t i = 0; i < text_len; i++) {
        if (safe_text[i] == '\0') safe_text[i] = ' ';
    }
    error_text = safe_text;

    if (ContextID != NULL) {
        cmsSignalError(ContextID, error_code, "%s", error_text);
    } else {
        cmsSignalError(NULL, error_code, "%s", error_text);
    }

    // Test 6: _cmsUnlockMutex
    // We don't have a valid mutex from _cmsLockMutex, so we pass NULL or a dummy.
    // The function is designed to safely do nothing if the plugin is not registered or mtx is invalid.
    // We pass a dummy pointer to test the path, but ensure we don't crash on invalid unlock.
    // Note: In a real scenario, we would lock first. Here we just test the unlock call.
    if (ContextID != NULL) {
        _cmsUnlockMutex(ContextID, dummy_mutex);
    } else {
        _cmsUnlockMutex(NULL, dummy_mutex);
    }

    // Cleanup
    if (ContextID != NULL) {
        cmsDeleteContext(ContextID);
    }

    return 0;
}