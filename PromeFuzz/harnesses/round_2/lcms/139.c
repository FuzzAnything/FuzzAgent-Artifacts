// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
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

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler->stream == NULL) return 0;
    
    size_t total_bytes = size * count;
    uint8_t* stream_data = (uint8_t*)iohandler->stream;
    size_t* stream_pos = (size_t*)(stream_data + total_bytes);
    
    if (*stream_pos >= total_bytes) return 0;
    
    size_t bytes_to_read = total_bytes - *stream_pos;
    if (bytes_to_read > size * count) bytes_to_read = size * count;
    
    memcpy(buffer, stream_data + *stream_pos, bytes_to_read);
    *stream_pos += bytes_to_read;
    
    return (cmsUInt32Number)(bytes_to_read / size);
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (iohandler->stream == NULL) return FALSE;
    
    uint8_t* stream_data = (uint8_t*)iohandler->stream;
    size_t total_bytes = iohandler->ReportedSize;
    size_t* stream_pos = (size_t*)(stream_data + total_bytes);
    
    if (offset > total_bytes) return FALSE;
    *stream_pos = offset;
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    if (iohandler->stream != NULL) {
        free(iohandler->stream);
        iohandler->stream = NULL;
    }
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    if (iohandler->stream == NULL) return 0;
    
    uint8_t* stream_data = (uint8_t*)iohandler->stream;
    size_t total_bytes = iohandler->ReportedSize;
    size_t* stream_pos = (size_t*)(stream_data + total_bytes);
    
    return (cmsUInt32Number)*stream_pos;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* buffer) {
    if (iohandler->stream == NULL) return FALSE;
    
    uint8_t* stream_data = (uint8_t*)iohandler->stream;
    size_t total_bytes = iohandler->ReportedSize;
    size_t* stream_pos = (size_t*)(stream_data + total_bytes);
    
    if (*stream_pos + size > total_bytes) return FALSE;
    
    memcpy(stream_data + *stream_pos, buffer, size);
    *stream_pos += size;
    
    if (*stream_pos > iohandler->UsedSpace) {
        iohandler->UsedSpace = (cmsUInt32Number)*stream_pos;
    }
    
    return TRUE;
}

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t* data, size_t size, int for_writing) {
    size_t total_size = size + sizeof(size_t);
    uint8_t* stream_data = (uint8_t*)malloc(total_size);
    if (!stream_data) return NULL;
    
    if (for_writing) {
        memset(stream_data, 0, total_size);
    } else {
        memcpy(stream_data, data, size);
    }
    
    size_t* stream_pos = (size_t*)(stream_data + size);
    *stream_pos = 0;
    
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) {
        free(stream_data);
        return NULL;
    }
    
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->stream = stream_data;
    io->ContextID = 0;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)size;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
    
    return io;
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test _cms15Fixed16toDouble
    if (Size >= sizeof(cmsS15Fixed16Number)) {
        cmsS15Fixed16Number fix32;
        memcpy(&fix32, Data, sizeof(fix32));
        cmsFloat64Number result = _cms15Fixed16toDouble(fix32);
        (void)result;
    }
    
    // Test cmsGetProfileVersion
    if (Size > 128) {
        cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, (cmsUInt32Number)(Size > 65536 ? 65536 : Size));
        if (hProfile != NULL) {
            cmsFloat64Number version = cmsGetProfileVersion(hProfile);
            (void)version;
            cmsCloseProfile(hProfile);
        }
    }
    
    // Test cmsGetEncodedCMMversion
    int cmm_version = cmsGetEncodedCMMversion();
    (void)cmm_version;
    
    // Test _cmsDoubleTo15Fixed16
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number v;
        memcpy(&v, Data, sizeof(v));
        cmsS15Fixed16Number fixed = _cmsDoubleTo15Fixed16(v);
        (void)fixed;
    }
    
    // Test _cmsRead15Fixed16Number
    if (Size > 0) {
        cmsIOHANDLER* io = create_dummy_iohandler(Data, Size > 1024 ? 1024 : Size, 0);
        if (io != NULL) {
            cmsFloat64Number n;
            cmsBool read_result = _cmsRead15Fixed16Number(io, &n);
            (void)read_result;
            io->Close(io);
            free(io);
        }
    }
    
    // Test _cmsWrite15Fixed16Number
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsIOHANDLER* io = create_dummy_iohandler(Data, 1024, 1);
        if (io != NULL) {
            cmsFloat64Number v;
            memcpy(&v, Data, sizeof(v));
            cmsBool write_result = _cmsWrite15Fixed16Number(io, v);
            (void)write_result;
            io->Close(io);
            free(io);
        }
    }
    
    return 0;
}