// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

// Dummy IO handler implementation for fuzzing
static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    (void)iohandler; (void)Buffer; (void)size; (void)count;
    return 0;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    (void)iohandler; (void)offset;
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    (void)iohandler;
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    (void)iohandler;
    return 0;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    (void)iohandler; (void)size; (void)Buffer;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_iohandler(cmsContext ctx) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)_cmsCalloc(ctx, 1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->ContextID = ctx;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    io->stream = NULL;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    return io;
}

int LLVMFuzzerTestOneInput_202(const uint8_t *Data, size_t Size) {
    (void)Data;
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    // Create a dummy IO handler
    cmsIOHANDLER* io = create_dummy_iohandler(ctx);
    if (!io) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Fuzz _cmsReadTypeBase
    if (Size >= 4) {
        cmsTagTypeSignature sig = _cmsReadTypeBase(io);
        (void)sig;
    }

    // Fuzz _cmsWriteTypeBase
    cmsTagTypeSignature test_sig = 0x41424344; // 'ABCD'
    cmsBool write_result = _cmsWriteTypeBase(io, test_sig);
    (void)write_result;

    // Fuzz _cmsWriteXYZNumber
    cmsCIEXYZ xyz_val = {0.0, 0.0, 0.0};
    cmsBool xyz_result = _cmsWriteXYZNumber(io, &xyz_val);
    (void)xyz_result;

    // Fuzz _cmsReadAlignment
    cmsBool align_result = _cmsReadAlignment(io);
    (void)align_result;

    // Fuzz cmsCloseIOhandler
    cmsBool close_result = cmsCloseIOhandler(io);
    (void)close_result;
    // Note: cmsCloseIOhandler calls the Close callback but does not free the struct itself.
    // We must free it manually using the context allocator.
    _cmsFree(ctx, io);
    io = NULL;

    // Fuzz cmsGetProfileIOhandler with NULL profile (safe)
    cmsIOHANDLER* null_io = cmsGetProfileIOhandler(NULL);
    (void)null_io;

    // Create a dummy profile to test cmsGetProfileIOhandler
    // cmsCreateRGBProfile requires WhitePoint, Primaries, and TransferFunction
    // We pass NULLs to test error handling or minimal creation paths if supported
    cmsHPROFILE hProfile = cmsCreateRGBProfile(NULL, NULL, NULL);
    if (hProfile) {
        cmsIOHANDLER* profile_io = cmsGetProfileIOhandler(hProfile);
        (void)profile_io;
        cmsCloseProfile(hProfile);
    }

    cmsDeleteContext(ctx);
    return 0;
}