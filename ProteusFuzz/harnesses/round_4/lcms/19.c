// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// _cmsWriteFloat32Number at cmsplugin.c:309:20 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

// Helper to create a context
static cmsContext CreateDummyContext(void) {
    return cmsCreateContext(NULL, NULL);
}

// Helper to destroy a context
static void DestroyDummyContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

// Helper to cleanup IO handler
static void CleanupIOHandler(cmsIOHANDLER* io) {
    if (io) {
        cmsCloseIOhandler(io);
    }
}

// We need to define our own IO handler implementation since cmsOpenIOhandler 
// might be internal or not exported in all builds, and we need to ensure
// the functions we are fuzzing (_cmsWriteAlignment, etc.) have a valid handler.
// We will create a simple in-memory or file-based handler manually.

static cmsUInt32Number DummyRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return 0;
    return fread(Buffer, size, count, f);
}

static cmsBool DummySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return FALSE;
    return (fseek(f, offset, SEEK_SET) == 0) ? TRUE : FALSE;
}

static cmsBool DummyClose(cmsIOHANDLER* iohandler) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return FALSE;
    fclose(f);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number DummyTell(cmsIOHANDLER* iohandler) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return 0;
    long pos = ftell(f);
    return (pos < 0) ? 0 : (cmsUInt32Number)pos;
}

static cmsBool DummyWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return FALSE;
    size_t written = fwrite(Buffer, 1, size, f);
    return (written == size) ? TRUE : FALSE;
}

static cmsIOHANDLER* CreateDummyIOHandler(const char* filename, const char* mode) {
    FILE* f = fopen(filename, mode);
    if (!f) return NULL;

    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) {
        fclose(f);
        return NULL;
    }

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = f;
    io->ContextID = NULL; // Will be set by caller or left NULL if not used by internal functions
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    io->Read = DummyRead;
    io->Seek = DummySeek;
    io->Close = DummyClose;
    io->Tell = DummyTell;
    io->Write = DummyWrite;
    
    // Get file size for ReportedSize if possible
    long current = ftell(f);
    fseek(f, 0, SEEK_END);
    io->ReportedSize = (cmsUInt32Number)ftell(f);
    fseek(f, current, SEEK_SET);

    return io;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateDummyContext();
    if (!ctx) return 0;

    // Create a dummy file with the fuzz data
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) {
        DestroyDummyContext(ctx);
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Test Read Operations
    cmsIOHANDLER* ioRead = CreateDummyIOHandler("./dummy_file", "rb");
    
    if (ioRead) {
        cmsFloat32Number floatVal;
        cmsBool res;

        // Fuzz _cmsReadAlignment
        res = _cmsReadAlignment(ioRead);
        
        // Fuzz _cmsReadFloat32Number
        res = _cmsReadFloat32Number(ioRead, &floatVal);

        // Try reading again to explore different states
        res = _cmsReadAlignment(ioRead);
        res = _cmsReadFloat32Number(ioRead, &floatVal);

        CleanupIOHandler(ioRead);
    }

    // Test Write Operations
    // Truncate the file
    f = fopen("./dummy_file", "wb");
    if (f) fclose(f); 

    cmsIOHANDLER* ioWrite = CreateDummyIOHandler("./dummy_file", "wb");
    
    if (ioWrite) {
        cmsBool res;
        cmsUInt32Number uintVal = 0;
        cmsFloat32Number floatVal = 0.0f;

        // Extract values from fuzz data if possible
        if (Size >= 4) {
            uintVal = *(const cmsUInt32Number*)(Data);
        }
        
        if (Size >= 4) {
            floatVal = *(const cmsFloat32Number*)(Data);
        }

        // Fuzz _cmsWriteAlignment
        res = _cmsWriteAlignment(ioWrite);

        // Fuzz _cmsWriteUInt32Number
        res = _cmsWriteUInt32Number(ioWrite, uintVal);

        // Fuzz _cmsWriteFloat32Number
        res = _cmsWriteFloat32Number(ioWrite, floatVal);

        // Try multiple writes to explore state
        res = _cmsWriteAlignment(ioWrite);
        res = _cmsWriteUInt32Number(ioWrite, 0xFFFFFFFF);
        res = _cmsWriteFloat32Number(ioWrite, 1.0f);
        res = _cmsWriteFloat32Number(ioWrite, -1.0f);
        res = _cmsWriteFloat32Number(ioWrite, 0.0f);

        CleanupIOHandler(ioWrite);
    }

    DestroyDummyContext(ctx);
    return 0;
}