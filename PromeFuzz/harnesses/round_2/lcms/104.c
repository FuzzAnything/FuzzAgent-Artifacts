// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "lcms2_plugin.h"

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    size_t total_bytes = size * count;
    if (iohandler->UsedSpace + total_bytes > iohandler->ReportedSize) {
        total_bytes = iohandler->ReportedSize - iohandler->UsedSpace;
    }
    if (total_bytes > 0) {
        memcpy(Buffer, (uint8_t*)iohandler->stream + iohandler->UsedSpace, total_bytes);
        iohandler->UsedSpace += total_bytes;
    }
    return total_bytes / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize) {
        return FALSE;
    }
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    if (iohandler->stream) {
        free(iohandler->stream);
        iohandler->stream = NULL;
    }
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    return iohandler->UsedSpace;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                          const void* Buffer) {
    if (iohandler->UsedSpace + size > iohandler->ReportedSize) {
        return FALSE;
    }
    memcpy((uint8_t*)iohandler->stream + iohandler->UsedSpace, Buffer, size);
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->stream = malloc(size);
    if (!io->stream) {
        free(io);
        return NULL;
    }
    
    memcpy(io->stream, data, size);
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = size;
    strcpy(io->PhysicalFile, "./dummy_file");
    
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsIOHANDLER* io = create_dummy_iohandler(Data, Size);
    if (!io) return 0;
    
    cmsUInt16Number value;
    cmsBool result;
    
    result = _cmsReadUInt16Number(io, &value);
    if (result) {
        _cmsWriteUInt16Number(io, value);
    }
    
    io->UsedSpace = 0;
    
    if (Size >= 8) {
        cmsUInt32Number array_size = (cmsUInt32Number)Data[0] | ((cmsUInt32Number)Data[1] << 8);
        if (array_size > 0 && array_size <= 1024) {
            cmsUInt16Number* array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
            if (array) {
                result = _cmsReadUInt16Array(io, array_size, array);
                if (result) {
                    io->UsedSpace = 0;
                    _cmsWriteUInt16Array(io, array_size, array);
                }
                free(array);
            }
        }
    }
    
    io->UsedSpace = 0;
    _cmsReadAlignment(io);
    
    io->UsedSpace = 0;
    _cmsWriteAlignment(io);
    
    dummy_close(io);
    return 0;
}