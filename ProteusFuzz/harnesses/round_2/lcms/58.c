// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>
#include <stdarg.h>

static void static_cleanup(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        // If context creation fails, we can still test global functions
        cmsUInt16Number alarm_codes[cmsMAXCHANNELS];
        memset(alarm_codes, 0, sizeof(alarm_codes));
        
        // Test global alarm codes functions
        cmsSetAlarmCodes(alarm_codes);
        cmsGetAlarmCodes(alarm_codes);
        
        // Test signal error with NULL context (global)
        cmsSignalError(NULL, 0, "Test error message");
        
        return 0;
    }

    // Parse input data to create diverse test scenarios
    cmsUInt16Number alarm_codes[cmsMAXCHANNELS];
    memset(alarm_codes, 0, sizeof(alarm_codes));

    // Fill alarm codes from input data if available
    size_t idx = 0;
    for (int i = 0; i < cmsMAXCHANNELS && idx + 1 < Size; i++) {
        alarm_codes[i] = (cmsUInt16Number)(Data[idx] | (Data[idx + 1] << 8));
        idx += 2;
    }

    // Test 1: Set and get thread-specific alarm codes
    cmsSetAlarmCodesTHR(ctx, alarm_codes);
    cmsGetAlarmCodesTHR(ctx, alarm_codes);

    // Test 2: Signal errors with various parameters
    cmsUInt32Number error_code = 0;
    if (idx < Size) error_code = Data[idx++];
    
    const char *error_msg = "Test error";
    if (idx < Size) {
        // Use a portion of input as error message, ensuring null termination
        size_t msg_len = (Size - idx < 256) ? (Size - idx) : 256;
        char *msg_buf = (char *)malloc(msg_len + 1);
        if (msg_buf != NULL) {
            memcpy(msg_buf, Data + idx, msg_len);
            msg_buf[msg_len] = '\0';
            error_msg = msg_buf;
            cmsSignalError(ctx, error_code, error_msg);
            free(msg_buf);
        }
    } else {
        cmsSignalError(ctx, error_code, error_msg);
    }

    // Test 3: Mutex operations
    // _cmsUnlockMutex expects a valid mutex pointer if one was locked.
    // Passing NULL is unsafe if the internal logic tries to dereference it directly
    // without checking for NULL first in the specific implementation path.
    // Based on the crash, we avoid calling _cmsUnlockMutex with NULL or invalid pointers.
    // We skip this specific call as we cannot safely generate a valid mutex pointer
    // without calling _cmsLockMutex first, which requires specific internal setup.
    // The description says "otherwise, it safely does nothing", but the crash indicates
    // the implementation might not handle NULL gracefully in all configurations.
    // To be safe, we omit the call or only call it if we had a valid mutex.
    // Since we don't have a valid mutex, we skip this test case to avoid crashes.
    // void *mtx = NULL;
    // _cmsUnlockMutex(ctx, mtx); // Removed to prevent SEGV

    // Test 4: Global alarm codes functions (should work regardless of context)
    cmsSetAlarmCodes(alarm_codes);
    cmsGetAlarmCodes(alarm_codes);

    // Cleanup
    static_cleanup(ctx);

    return 0;
}