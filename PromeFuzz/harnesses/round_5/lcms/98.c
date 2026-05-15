// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Memory-based I/O handler implementation for fuzzing
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total = size * count;
    
    if (stream->pos + total > stream->size) {
        total = (cmsUInt32Number)(stream->size - stream->pos);
        if (total < size) return 0;
        total = total - (total % size);
    }
    
    if (total > 0) {
        memcpy(Buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return total / size;
}

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    if (offset > stream->size) return FALSE;
    stream->pos = offset;
    return TRUE;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    if (iohandler->stream) {
        free(iohandler->stream);
        iohandler->stream = NULL;
    }
    return TRUE;
}

static cmsUInt32Number MemoryTell(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    // For fuzzing, we just pretend to write successfully
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    MemoryStream* stream = (MemoryStream*)malloc(sizeof(MemoryStream));
    if (!stream) {
        free(io);
        return NULL;
    }
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    
    io->stream = stream;
    io->Read = MemoryRead;
    io->Seek = MemorySeek;
    io->Close = MemoryClose;
    io->Tell = MemoryTell;
    io->Write = MemoryWrite;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    
    return io;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    // Test _cmsRead15Fixed16Number
    cmsIOHANDLER* io = CreateMemoryHandler(Data, Size);
    if (io) {
        cmsFloat64Number n;
        cmsBool result = _cmsRead15Fixed16Number(io, &n);
        io->Close(io);
        free(io);
    }
    
    // Test _cmsDoubleTo15Fixed16
    double d;
    memcpy(&d, Data, sizeof(double));
    cmsS15Fixed16Number fixed = _cmsDoubleTo15Fixed16(d);
    
    // Test _cmsWrite15Fixed16Number
    io = CreateMemoryHandler(Data, Size);
    if (io) {
        cmsBool write_result = _cmsWrite15Fixed16Number(io, d);
        io->Close(io);
        free(io);
    }
    
    // Test cmsGetProfileVersion
    if (Size >= 128) {
        // Create a minimal profile from fuzzing data
        cmsHPROFILE profile = cmsOpenProfileFromMem(Data, (cmsUInt32Number)Size);
        if (profile) {
            cmsFloat64Number version = cmsGetProfileVersion(profile);
            cmsCloseProfile(profile);
        }
    }
    
    // Test _cms15Fixed16toDouble
    cmsFloat64Number converted = _cms15Fixed16toDouble(fixed);
    
    // Test cmsGetEncodedCMMversion
    int cmm_version = cmsGetEncodedCMMversion();
    
    // Also test with different values from the input
    if (Size >= 16) {
        // Use different bytes for another double conversion
        double d2;
        memcpy(&d2, Data + 8, sizeof(double));
        cmsS15Fixed16Number fixed2 = _cmsDoubleTo15Fixed16(d2);
        cmsFloat64Number converted2 = _cms15Fixed16toDouble(fixed2);
        
        // Test writing the second value
        io = CreateMemoryHandler(Data, Size);
        if (io) {
            cmsBool write_result2 = _cmsWrite15Fixed16Number(io, d2);
            io->Close(io);
            free(io);
        }
    }
    
    // Test with edge cases for fixed-point conversion
    cmsS15Fixed16Number max_fixed = _cmsDoubleTo15Fixed16(32767.99998474121);
    cmsS15Fixed16Number min_fixed = _cmsDoubleTo15Fixed16(-32768.0);
    cmsS15Fixed16Number zero_fixed = _cmsDoubleTo15Fixed16(0.0);
    
    cmsFloat64Number max_converted = _cms15Fixed16toDouble(max_fixed);
    cmsFloat64Number min_converted = _cms15Fixed16toDouble(min_fixed);
    cmsFloat64Number zero_converted = _cms15Fixed16toDouble(zero_fixed);
    
    return 0;
}