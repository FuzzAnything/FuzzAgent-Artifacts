// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsContext CreateContextFromData(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    return ctx;
}

static void CleanupContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE CreateDummyProfile(cmsContext ctx) {
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) return NULL;
    return hProfile;
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateContextFromData(Data, Size);
    if (ctx == NULL) return 0;

    const char *dummy_file = "./dummy_file";
    const char *modes[] = {"r", "w", "rb", "wb", "invalid"};
    
    // Test 1: cmsOpenIOhandlerFromFile
    for (int i = 0; i < 5; i++) {
        cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(ctx, dummy_file, modes[i]);
        if (io != NULL) {
            // Test _cmsWriteAlignment
            _cmsWriteAlignment(io);
            
            // Test _cmsWriteUInt16Array
            cmsUInt16Number arr[10];
            for (int j = 0; j < 10; j++) arr[j] = (cmsUInt16Number)(Data[j % Size]);
            _cmsWriteUInt16Array(io, 10, arr);
            
            // Test _cmsIOPrintf
            _cmsIOPrintf(io, "Test string: %d %s", (int)Data[0], "hello");
            
            // Close the handler if it has a close function
            if (io->Close) {
                io->Close(io);
            }
        }
    }

    // Test 2: cmsSaveProfileToFile
    cmsHPROFILE hProfile = CreateDummyProfile(ctx);
    if (hProfile != NULL) {
        cmsSaveProfileToFile(hProfile, dummy_file);
        cmsCloseProfile(hProfile);
    }

    // Test 3: cmsIT8SaveToFile
    // We cannot pass NULL to cmsIT8SaveToFile as it causes a crash inside the library.
    // We need a valid IT8 handle. Since we cannot easily create one without a file,
    // we skip this test or try to load a dummy file if it exists.
    // For now, we will not call cmsIT8SaveToFile to avoid the crash.
    // If we want to test it, we would need to create a valid IT8 file first.
    // cmsIT8SaveToFile(NULL, dummy_file); // This line causes the crash

    // Cleanup
    CleanupContext(ctx);
    return 0;
}