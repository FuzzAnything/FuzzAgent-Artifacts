// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext createContext(void) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    return ctx;
}

static void destroyContext(cmsContext ctx) {
    if (ctx) cmsDeleteContext(ctx);
}

static cmsHPROFILE createDummyProfile(cmsContext ctx) {
    if (!ctx) return NULL;

    cmsCIExyY WhitePoint;
    WhitePoint.x = 0.3457 * 65536.0;
    WhitePoint.y = 0.3585 * 65536.0;
    WhitePoint.Y = 1.0 * 65536.0;

    cmsCIExyYTRIPLE Primaries;
    Primaries.Red.x = 0.6400 * 65536.0; Primaries.Red.y = 0.3300 * 65536.0;
    Primaries.Green.x = 0.3000 * 65536.0; Primaries.Green.y = 0.6000 * 65536.0;
    Primaries.Blue.x = 0.1500 * 65536.0; Primaries.Blue.y = 0.0600 * 65536.0;

    cmsToneCurve* Curves[3];
    Curves[0] = cmsBuildGamma(ctx, 2.2);
    Curves[1] = cmsBuildGamma(ctx, 2.2);
    Curves[2] = cmsBuildGamma(ctx, 2.2);

    if (!Curves[0] || !Curves[1] || !Curves[2]) {
        if (Curves[0]) cmsFreeToneCurve(Curves[0]);
        if (Curves[1]) cmsFreeToneCurve(Curves[1]);
        if (Curves[2]) cmsFreeToneCurve(Curves[2]);
        return NULL;
    }

    cmsHPROFILE h = cmsCreateRGBProfile(&WhitePoint, &Primaries, Curves);

    cmsFreeToneCurve(Curves[0]);
    cmsFreeToneCurve(Curves[1]);
    cmsFreeToneCurve(Curves[2]);

    return h;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsContext ctx = createContext();
    if (!ctx) return 0;

    char filename[256];
    size_t fn_len = (Size > 255) ? 255 : Size - 1;
    memcpy(filename, Data, fn_len);
    filename[fn_len] = '\0';
    
    if (strchr(filename, '/') || strchr(filename, '\\')) {
        strcpy(filename, "./dummy_file");
    } else if (strlen(filename) == 0) {
        strcpy(filename, "./dummy_file");
    }

    char mode[4];
    if (Data[0] == 'r') {
        strcpy(mode, "r");
    } else if (Data[0] == 'w') {
        strcpy(mode, "w");
    } else {
        mode[0] = Data[0];
        mode[1] = Data[1];
        mode[2] = '\0';
    }

    cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(ctx, filename, mode);
    
    if (io) {
        cmsUInt16Number arr[10];
        memset(arr, 0, sizeof(arr));
        for (int i = 0; i < 10 && i < Size; i++) {
            arr[i] = Data[i] + (Data[i+1] << 8);
        }
        cmsBool writeRes = _cmsWriteUInt16Array(io, 10, arr);
        (void)writeRes;

        char formatStr[64];
        size_t fmt_len = (Size > 63) ? 63 : Size;
        memcpy(formatStr, Data, fmt_len);
        formatStr[fmt_len] = '\0';
        cmsBool printRes = _cmsIOPrintf(io, formatStr, 123, 456, "test");
        (void)printRes;

        cmsBool closeRes = cmsCloseIOhandler(io);
        (void)closeRes;
    }

    cmsHPROFILE profile = createDummyProfile(ctx);
    if (profile) {
        cmsBool saveRes = cmsSaveProfileToFile(profile, filename);
        (void)saveRes;
        cmsCloseProfile(profile);
    }

    // Avoid passing NULL to cmsIT8SaveToFile as it causes a crash inside the library
    // The function expects a valid IT8 handle. Since creating a valid one requires
    // complex setup not suitable for simple fuzzing, we skip this specific call
    // to prevent the SEGV observed in the sanitizer report.
    // cmsBool it8Res = cmsIT8SaveToFile(NULL, filename); 

    destroyContext(ctx);
    return 0;
}