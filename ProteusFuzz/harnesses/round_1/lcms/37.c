// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void dummy_cleanup(cmsHPROFILE hProfile) {
    if (hProfile) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsIOHANDLER *io = NULL;
    cmsBool result = TRUE;
    cmsUInt16Number test_array[10];
    cmsUInt16Number test_val;
    char dummy_file[] = "./dummy_file";

    // Initialize test data from fuzz input
    memset(test_array, 0, sizeof(test_array));
    if (Size >= sizeof(cmsUInt16Number)) {
        test_val = Data[0] << 8 | Data[1];
    } else {
        test_val = 0;
    }

    // Fill array with data if available
    size_t array_len = (Size > sizeof(test_array)) ? sizeof(test_array) / sizeof(cmsUInt16Number) : Size / sizeof(cmsUInt16Number);
    for (size_t i = 0; i < array_len; i++) {
        if (i + 1 < Size) {
            test_array[i] = Data[i * 2] << 8 | Data[i * 2 + 1];
        } else {
            test_array[i] = 0;
        }
    }

    // Test 1: cmsOpenIOhandlerFromFile
    // Try writing to dummy file to create it for read operations later
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Test read mode
    io = cmsOpenIOhandlerFromFile(ctx, dummy_file, "r");
    if (io) {
        // Test _cmsWriteUInt16Number (Note: writing to read-only file might fail, which is expected)
        result = _cmsWriteUInt16Number(io, test_val);
        
        // Test _cmsWriteUInt16Array
        result = _cmsWriteUInt16Array(io, array_len, test_array);

        // Test _cmsIOPrintf
        result = _cmsIOPrintf(io, "Test data: %d", test_val);
        
        cmsCloseIOhandler(io);
        io = NULL;
    }

    // Test write mode
    io = cmsOpenIOhandlerFromFile(ctx, dummy_file, "w");
    if (io) {
        result = _cmsWriteUInt16Number(io, test_val);
        result = _cmsWriteUInt16Array(io, array_len, test_array);
        result = _cmsIOPrintf(io, "Test data: %d", test_val);
        cmsCloseIOhandler(io);
        io = NULL;
    }

    // Test 2: cmsSaveProfileToFile
    // Create a dummy profile (sRGB)
    hProfile = cmsCreate_sRGBProfile();
    if (hProfile) {
        result = cmsSaveProfileToFile(hProfile, dummy_file);
        dummy_cleanup(hProfile);
        hProfile = NULL;
    }

    // Test 3: cmsIT8SaveToFile
    // Skip calling cmsIT8SaveToFile with NULL as it causes a SEGV in the library implementation.
    // The library does not perform a NULL check before dereferencing the handle.
    // We cannot safely test this function without a valid handle, which requires cmsOpenIT8 (not in target list).

    // Cleanup context
    cmsDeleteContext(ctx);
    ctx = NULL;

    return 0;
}