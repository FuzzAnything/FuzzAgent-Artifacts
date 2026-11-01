// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIT8Alloc at cmscgats.c:1454:22 in lcms2.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsIT8SaveToFile at cmscgats.c:2007:19 in lcms2.h
// cmsIT8Free at cmscgats.c:1170:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static void cleanup_iohandler(cmsIOHANDLER *io) {
    if (io != NULL && io->Close != NULL) {
        io->Close(io);
        // Do not call free(io) here. The Close callback (FileClose) 
        // internally frees the iohandler structure via _cmsFree.
        // Calling free again causes a double-free.
    }
}

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void destroy_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static cmsHPROFILE create_dummy_profile(cmsContext ctx) {
    (void)ctx;
    return cmsCreate_sRGBProfile();
}

static cmsHANDLE create_dummy_it8(cmsContext ctx) {
    return cmsIT8Alloc(ctx);
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context();
    if (ctx == NULL) return 0;

    const char *filename = "./dummy_file";
    
    // Write some data to the dummy file to ensure it exists for "r" mode
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Test 1: cmsOpenIOhandlerFromFile
    cmsIOHANDLER *io = cmsOpenIOhandlerFromFile(ctx, filename, "r");
    if (io != NULL) {
        // Test _cmsIOPrintf
        cmsBool res1 = _cmsIOPrintf(io, "Test: %d", (int)(Data[0] & 0xFF));
        (void)res1;

        // Test _cmsWriteAlignment
        cmsBool res2 = _cmsWriteAlignment(io);
        (void)res2;

        // Test _cmsWriteUInt16Array
        cmsUInt16Number arr[10];
        for (int i = 0; i < 10; i++) {
            arr[i] = (cmsUInt16Number)(Data[i % Size] & 0xFFFF);
        }
        cmsBool res3 = _cmsWriteUInt16Array(io, 10, arr);
        (void)res3;

        cleanup_iohandler(io);
    }

    // Test write mode for cmsOpenIOhandlerFromFile
    cmsIOHANDLER *io_write = cmsOpenIOhandlerFromFile(ctx, filename, "w");
    if (io_write != NULL) {
        _cmsWriteAlignment(io_write);
        cmsUInt16Number arr2[5] = {1, 2, 3, 4, 5};
        _cmsWriteUInt16Array(io_write, 5, arr2);
        cleanup_iohandler(io_write);
    }

    // Test 2: cmsSaveProfileToFile
    cmsHPROFILE hProfile = create_dummy_profile(ctx);
    if (hProfile != NULL) {
        cmsBool res4 = cmsSaveProfileToFile(hProfile, filename);
        (void)res4;
        cmsCloseProfile(hProfile);
    }

    // Test 3: cmsIT8SaveToFile
    cmsHANDLE hIT8 = create_dummy_it8(ctx);
    if (hIT8 != NULL) {
        cmsBool res5 = cmsIT8SaveToFile(hIT8, filename);
        (void)res5;
        cmsIT8Free(hIT8);
    }

    destroy_context(ctx);
    return 0;
}