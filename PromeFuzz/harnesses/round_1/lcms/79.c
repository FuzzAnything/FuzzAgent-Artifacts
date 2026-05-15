// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

// Memory stream implementation for cmsIOHANDLER
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total = size * count;
    
    if (stream->pos + total > stream->size) {
        total = stream->size - stream->pos;
        if (total < size) return 0;
        count = total / size;
        total = count * size;
    }
    
    if (total > 0) {
        memcpy(Buffer, stream->data + stream->pos, total);
        stream->pos += total;
    }
    
    return count;
}

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    if (offset > stream->size) return FALSE;
    stream->pos = offset;
    return TRUE;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    // Nothing to clean up for memory stream
    return TRUE;
}

static cmsUInt32Number MemoryTell(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                          const void* Buffer) {
    // For fuzzing, we don't actually need to write anywhere
    // Just simulate successful write
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t* data, size_t size) {
    static cmsIOHANDLER handler;
    static MemoryStream stream;
    
    stream.data = data;
    stream.size = size;
    stream.pos = 0;
    
    handler.stream = &stream;
    handler.Read = MemoryRead;
    handler.Seek = MemorySeek;
    handler.Close = MemoryClose;
    handler.Tell = MemoryTell;
    handler.Write = MemoryWrite;
    handler.ContextID = NULL;
    handler.UsedSpace = 0;
    handler.ReportedSize = (cmsUInt32Number)size;
    memset(handler.PhysicalFile, 0, cmsMAX_PATH);
    
    return &handler;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;
    
    // Test _cms15Fixed16toDouble
    cmsS15Fixed16Number fix32;
    memcpy(&fix32, Data, sizeof(fix32));
    cmsFloat64Number result1 = _cms15Fixed16toDouble(fix32);
    (void)result1;
    
    // Test _cms8Fixed8toDouble
    cmsUInt16Number fixed8;
    memcpy(&fixed8, Data + 4, sizeof(fixed8));
    cmsFloat64Number result2 = _cms8Fixed8toDouble(fixed8);
    (void)result2;
    
    // Test _cmsDoubleTo8Fixed8
    cmsFloat64Number val1;
    memcpy(&val1, Data + 6, sizeof(val1));
    cmsUInt16Number result3 = _cmsDoubleTo8Fixed8(val1);
    (void)result3;
    
    // Test _cmsDoubleTo15Fixed16
    cmsFloat64Number val2;
    memcpy(&val2, Data + 14, sizeof(val2));
    cmsS15Fixed16Number result4 = _cmsDoubleTo15Fixed16(val2);
    (void)result4;
    
    // Test _cmsRead15Fixed16Number with memory stream
    if (Size >= 24) {
        cmsIOHANDLER* io = CreateMemoryHandler(Data + 22, Size - 22);
        cmsFloat64Number readNum;
        cmsBool readResult = _cmsRead15Fixed16Number(io, &readNum);
        (void)readResult;
        (void)readNum;
    }
    
    // Test _cmsWrite15Fixed16Number with memory stream
    if (Size >= 30) {
        cmsIOHANDLER* io = CreateMemoryHandler(Data + 22, Size - 22);
        cmsFloat64Number writeNum;
        memcpy(&writeNum, Data + 14, sizeof(writeNum));
        cmsBool writeResult = _cmsWrite15Fixed16Number(io, writeNum);
        (void)writeResult;
    }
    
    return 0;
}