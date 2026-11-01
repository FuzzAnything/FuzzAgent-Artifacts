// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void static_error_handler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *ErrorText) {
    // Silent error handler for fuzzing
    (void)ContextID;
    (void)ErrorCode;
    (void)ErrorText;
}

static cmsContext create_context_from_input(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    
    // Set a custom log error handler to prevent crashes during fuzzing
    cmsSetLogErrorHandler(static_error_handler);
    
    (void)Data;
    (void)Size;
    
    return ctx;
}

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context_from_input(Data, Size);
    if (ctx == NULL) return 0;

    cmsUInt16Number alarmCodes[cmsMAXCHANNELS];
    memset(alarmCodes, 0, sizeof(alarmCodes));

    // 1. Test cmsSignalError
    char safe_format[64];
    size_t format_len = (Size < 63) ? Size : 63;
    memcpy(safe_format, Data, format_len);
    safe_format[format_len] = '\0';
    
    // Ensure format string is safe (no % characters to prevent unexpected behavior)
    for (size_t i = 0; i < format_len; i++) {
        if (safe_format[i] == '%') {
            safe_format[i] = 'x';
        }
    }

    cmsSignalError(ctx, 0, safe_format);

    // 2. Test cmsSetAlarmCodes (global)
    for (int i = 0; i < cmsMAXCHANNELS; i++) {
        if (Size > (size_t)(i + 1)) {
            alarmCodes[i] = Data[i] | (Data[(i + 1) % Size] << 8);
        } else {
            alarmCodes[i] = 0;
        }
    }
    cmsSetAlarmCodes(alarmCodes);

    // 3. Test cmsGetAlarmCodesTHR
    cmsUInt16Number retrievedCodesTHR[cmsMAXCHANNELS];
    memset(retrievedCodesTHR, 0, sizeof(retrievedCodesTHR));
    cmsGetAlarmCodesTHR(ctx, retrievedCodesTHR);

    // 4. Test cmsGetAlarmCodes (global)
    cmsUInt16Number retrievedCodes[cmsMAXCHANNELS];
    memset(retrievedCodes, 0, sizeof(retrievedCodes));
    cmsGetAlarmCodes(retrievedCodes);

    // 5. Test cmsSetAlarmCodesTHR
    cmsSetAlarmCodesTHR(ctx, alarmCodes);

    // 6. Test _cmsUnlockMutex
    // Do NOT call _cmsUnlockMutex with NULL or invalid pointers as it causes SEGV
    // The function expects a valid mutex pointer returned by _cmsLockMutex
    // Since we don't have a valid mutex from _cmsLockMutex, we skip this test
    // to avoid triggering the sanitizer error.
    // _cmsUnlockMutex(ctx, NULL); // Removed to prevent SEGV

    // Cleanup
    cleanup_context(ctx);

    return 0;
}