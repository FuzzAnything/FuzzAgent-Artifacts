// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsSetAlarmCodesTHR at cmsxform.c:93:16 in lcms2.h
// cmsGetAlarmCodesTHR at cmsxform.c:104:16 in lcms2.h
// cmsSetAlarmCodes at cmsxform.c:113:16 in lcms2.h
// cmsGetAlarmCodes at cmsxform.c:120:16 in lcms2.h
// cmsSignalError at cmserr.c:510:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0;
    }

    cmsUInt16Number alarmCodes[cmsMAXCHANNELS];
    cmsUInt16Number setAlarmCodes[cmsMAXCHANNELS];
    cmsUInt32Number errorCode;
    char errorText[256];

    // Initialize arrays with zeros
    memset(alarmCodes, 0, sizeof(alarmCodes));
    memset(setAlarmCodes, 0, sizeof(setAlarmCodes));
    memset(errorText, 0, sizeof(errorText));

    // Safely copy fuzz data into alarmCodes
    size_t copyLen = (Size < sizeof(alarmCodes)) ? Size : sizeof(alarmCodes);
    if (copyLen > 0) {
        memcpy(alarmCodes, Data, copyLen);
    }

    // Safely copy fuzz data into setAlarmCodes
    // Ensure we don't read past the buffer end
    size_t remainingSize = (Size > sizeof(alarmCodes)) ? (Size - sizeof(alarmCodes)) : 0;
    copyLen = (remainingSize < sizeof(setAlarmCodes)) ? remainingSize : sizeof(setAlarmCodes);
    if (copyLen > 0) {
        memcpy(setAlarmCodes, Data + sizeof(alarmCodes), copyLen);
    }

    // Fuzz cmsSetAlarmCodesTHR
    cmsSetAlarmCodesTHR(ctx, setAlarmCodes);

    // Fuzz cmsGetAlarmCodesTHR
    cmsGetAlarmCodesTHR(ctx, alarmCodes);

    // Fuzz cmsSetAlarmCodes (global)
    cmsSetAlarmCodes(setAlarmCodes);

    // Fuzz cmsGetAlarmCodes (global)
    cmsGetAlarmCodes(alarmCodes);

    // Fuzz cmsSignalError
    if (Size >= 4) {
        errorCode = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    } else {
        errorCode = 0;
    }

    // Prepare error text safely
    size_t textLen = (Size > 255) ? 255 : Size;
    if (textLen > 0) {
        memcpy(errorText, Data, textLen);
        errorText[textLen] = '\0';
    } else {
        strcpy(errorText, "Fuzz error message");
    }

    cmsSignalError(ctx, errorCode, "%s", errorText);

    cleanup_context(ctx);

    return 0;
}