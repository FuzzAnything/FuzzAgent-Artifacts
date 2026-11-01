// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Helper to safely create a context
static cmsContext create_context_from_fuzz(const uint8_t *data, size_t size) {
    if (size < 1) return NULL;
    // Use first byte to decide if we create a context or not, or just create one
    // We always try to create a valid context to test the target APIs
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    return ctx;
}

// Helper to safely destroy context
static void destroy_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

// Helper to generate alarm codes from fuzz data
static void generate_alarm_codes(const uint8_t *data, size_t size, cmsUInt16Number codes[cmsMAXCHANNELS]) {
    memset(codes, 0, sizeof(cmsUInt16Number) * cmsMAXCHANNELS);
    for (int i = 0; i < cmsMAXCHANNELS && i < size; i++) {
        // Use modulo to keep within reasonable uint16 range, though uint16 is 0-65535
        codes[i] = (cmsUInt16Number)(data[i] * 100); 
    }
    // Fill remaining with pattern
    for (int i = size; i < cmsMAXCHANNELS; i++) {
        codes[i] = (cmsUInt16Number)(i * 10);
    }
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = NULL;
    cmsUInt16Number alarmCodes[cmsMAXCHANNELS];
    cmsUInt16Number alarmCodesGet[cmsMAXCHANNELS];
    
    // 1. Prepare environment
    // Create a context to test THR functions
    ctx = create_context_from_fuzz(Data, Size);
    
    if (ctx != NULL) {
        // Generate alarm codes from input
        generate_alarm_codes(Data, Size, alarmCodes);

        // Test 3: cmsSetAlarmCodes (Global)
        // We pass a non-NULL array. If Data is all zeros, it might be 0s, which is valid.
        // The API asserts if NULL, so we ensure we pass valid pointer.
        cmsSetAlarmCodes(alarmCodes);

        // Test 5: cmsSetAlarmCodesTHR (Thread-specific)
        cmsSetAlarmCodesTHR(ctx, alarmCodes);

        // Test 1: cmsGetAlarmCodesTHR
        memset(alarmCodesGet, 0, sizeof(alarmCodesGet));
        cmsGetAlarmCodesTHR(ctx, alarmCodesGet);

        // Test 6: cmsGetAlarmCodes (Global)
        memset(alarmCodesGet, 0, sizeof(alarmCodesGet));
        cmsGetAlarmCodes(alarmCodesGet);

        // Test 4: cmsSignalError
        // We need a format string. We use a safe static string or derive from data if printable.
        // To avoid crashes from malformed format strings, we use a fixed safe format.
        const char *safe_msg = "Fuzz test error message";
        cmsSignalError(ctx, 0, safe_msg);
        
        // Try with a different error code derived from data
        cmsUInt32Number errCode = (Size > 0) ? (Data[0] & 0xFF) : 0;
        cmsSignalError(ctx, errCode, "Error code: %u", errCode);

        // Test 2: _cmsUnlockMutex
        // This function requires a mutex pointer obtained from _cmsLockMutex.
        // Since we don't have a direct API to get a valid mutex pointer without locking first,
        // and _cmsLockMutex is internal (not in the provided list but implied by _cmsUnlockMutex),
        // we cannot safely call _cmsUnlockMutex with a random pointer without risking undefined behavior
        // or double-unlock if we don't control the lock state.
        // However, the description says: "It invokes the plugin's unlock function if registered; otherwise, it safely does nothing."
        // Passing NULL or an invalid pointer might be safe if the implementation checks, but it's risky.
        // Given the constraints and the need to explore states, we will NOT call _cmsUnlockMutex 
        // with a random pointer because it's an internal synchronization primitive and misuse can cause deadlocks or crashes.
        // Instead, we rely on the other APIs which are more suitable for fuzzing.
        // If we must call it, we would need to call _cmsLockMutex first, but it's not in the target list.
        // We skip this specific call to avoid undefined behavior in the fuzzer unless we can lock first.
        // Note: The prompt asks to fuzz the library using the provided APIs. 
        // Since _cmsLockMutex is not provided, we cannot safely test _cmsUnlockMutex in isolation.
        // We will skip _cmsUnlockMutex to ensure the fuzzer remains stable.
        
        // Cleanup context
        destroy_context(ctx);
    } else {
        // If context creation failed, we can still test global APIs
        generate_alarm_codes(Data, Size, alarmCodes);
        cmsSetAlarmCodes(alarmCodes);
        cmsGetAlarmCodes(alarmCodesGet);
        cmsSignalError(NULL, 0, "Global error message");
    }

    return 0;
}