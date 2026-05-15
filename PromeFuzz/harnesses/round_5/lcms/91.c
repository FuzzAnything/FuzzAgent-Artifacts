// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsAdjustEndianess64 at cmsplugin.c:79:17 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer,
                                  cmsUInt32Number size, cmsUInt32Number count) {
    cmsUInt32Number total = size * count;
    if (iohandler->UsedSpace + total > iohandler->ReportedSize) {
        total = iohandler->ReportedSize - iohandler->UsedSpace;
    }
    if (total > 0) {
        memcpy(Buffer, (uint8_t*)iohandler->stream + iohandler->UsedSpace, total);
        iohandler->UsedSpace += total;
    }
    return total / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize) {
        return FALSE;
    }
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    free(iohandler->stream);
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
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    io->stream = malloc(size);
    if (!io->stream) {
        free(io);
        return NULL;
    }
    memcpy(io->stream, data, size);
    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    memset(io->PhysicalFile, 0, cmsMAX_PATH);
    
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 32) return 0;
    
    cmsIOHANDLER* io = create_dummy_iohandler(Data, Size);
    if (!io) return 0;
    
    cmsUInt64Number uint64_val = 0;
    cmsFloat32Number float32_val = 0.0f;
    cmsCIEXYZ xyz_val = {0.0, 0.0, 0.0};
    cmsUInt64Number adjust_result = 0;
    
    if (Size >= 8) {
        uint64_val = *((cmsUInt64Number*)Data);
        _cmsAdjustEndianess64(&adjust_result, &uint64_val);
    }
    
    io->UsedSpace = 0;
    if (Size >= 8) {
        _cmsWriteUInt64Number(io, &uint64_val);
    }
    
    io->UsedSpace = 0;
    if (Size >= 4) {
        float32_val = *((cmsFloat32Number*)Data);
        _cmsWriteUInt64Number(io, &uint64_val);
        io->UsedSpace = 0;
        _cmsReadFloat32Number(io, &float32_val);
    }
    
    io->UsedSpace = 0;
    if (Size >= 8) {
        _cmsReadUInt64Number(io, &uint64_val);
    }
    
    io->UsedSpace = 0;
    if (Size >= 24) {
        xyz_val.X = *((cmsFloat32Number*)Data);
        xyz_val.Y = *((cmsFloat32Number*)(Data + 4));
        xyz_val.Z = *((cmsFloat32Number*)(Data + 8));
        _cmsWriteXYZNumber(io, &xyz_val);
    }
    
    io->UsedSpace = 0;
    if (Size >= 12) {
        _cmsReadXYZNumber(io, &xyz_val);
    }
    
    io->Close(io);
    return 0;
}