// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2_plugin.h"

static cmsUInt32Number dummyRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return 0;
    size_t total = size * count;
    size_t read = fread(Buffer, 1, total, f);
    return (cmsUInt32Number)read;
}

static cmsBool dummySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return FALSE;
    if (fseek(f, offset, SEEK_SET) != 0) return FALSE;
    return TRUE;
}

static cmsBool dummyClose(cmsIOHANDLER* iohandler) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return FALSE;
    fclose(f);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number dummyTell(cmsIOHANDLER* iohandler) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return 0;
    long pos = ftell(f);
    if (pos < 0) return 0;
    return (cmsUInt32Number)pos;
}

static cmsBool dummyWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    FILE* f = (FILE*)iohandler->stream;
    if (!f) return FALSE;
    size_t written = fwrite(Buffer, 1, size, f);
    return (written == size) ? TRUE : FALSE;
}

static cmsContext createContext(void) {
    return cmsCreateContext(NULL, NULL);
}

static void destroyContext(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = createContext();
    if (!ctx) return 0;

    cmsIOHANDLER io;
    memset(&io, 0, sizeof(io));
    io.ContextID = ctx;
    io.Read = dummyRead;
    io.Seek = dummySeek;
    io.Close = dummyClose;
    io.Tell = dummyTell;
    io.Write = dummyWrite;
    io.UsedSpace = 0;
    io.ReportedSize = 0;

    // Create a temporary file for testing
    FILE* f = fopen("./dummy_file", "r+b");
    if (!f) {
        // Create file if it doesn't exist
        f = fopen("./dummy_file", "w+b");
        if (!f) {
            destroyContext(ctx);
            return 0;
        }
    }

    // Write fuzz data to file if needed for read tests
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
        rewind(f);
    }

    io.stream = f;

    cmsCIEXYZ xyz = {0.0, 0.0, 0.0};
    cmsUInt16Number u16 = 0;
    cmsUInt8Number u8 = 0;

    // Extract values from fuzz data if available
    if (Size >= 12) {
        memcpy(&xyz, Data, sizeof(cmsCIEXYZ));
    }
    if (Size >= 14) {
        u16 = (Data[12] << 8) | Data[13];
    }
    if (Size >= 15) {
        u8 = Data[14];
    }

    // Test _cmsWriteXYZNumber
    _cmsWriteXYZNumber(&io, &xyz);

    // Test _cmsWriteAlignment
    _cmsWriteAlignment(&io);

    // Test _cmsWriteUInt16Number
    _cmsWriteUInt16Number(&io, u16);

    // Test _cmsWriteUInt8Number
    _cmsWriteUInt8Number(&io, u8);

    // Seek back for read tests
    if (io.Seek(&io, 0)) {
        // Test _cmsReadAlignment
        _cmsReadAlignment(&io);

        // Test _cmsReadXYZNumber
        cmsCIEXYZ readXyz = {0.0, 0.0, 0.0};
        _cmsReadXYZNumber(&io, &readXyz);
    }

    // Cleanup
    io.Close(&io);
    destroyContext(ctx);

    return 0;
}