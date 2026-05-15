// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsGetEncodedCMMversion at cmserr.c:30:15 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Define FuzzStream structure for memory-based I/O
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} FuzzStream;

// Memory stream implementations
static cmsUInt32Number FuzzStreamRead(cmsIOHANDLER* iohandler, void* Buffer, 
                                      cmsUInt32Number size, cmsUInt32Number count) {
    FuzzStream* stream = (FuzzStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    cmsUInt32Number available = (cmsUInt32Number)(stream->size - stream->pos);
    
    if (total_bytes > available) {
        total_bytes = available;
        count = total_bytes / size;
    }
    
    if (total_bytes > 0) {
        memcpy(Buffer, stream->data + stream->pos, total_bytes);
        stream->pos += total_bytes;
    }
    
    return count;
}

static cmsBool FuzzStreamSeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    FuzzStream* stream = (FuzzStream*)iohandler->stream;
    if (offset > stream->size) return FALSE;
    stream->pos = offset;
    return TRUE;
}

static cmsUInt32Number FuzzStreamTell(cmsIOHANDLER* iohandler) {
    FuzzStream* stream = (FuzzStream*)iohandler->stream;
    return (cmsUInt32Number)stream->pos;
}

static cmsBool FuzzStreamWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    FuzzStream* stream = (FuzzStream*)iohandler->stream;
    if (stream->pos + size > stream->size) return FALSE;
    
    memcpy((void*)(stream->data + stream->pos), Buffer, size);
    stream->pos += size;
    return TRUE;
}

static cmsBool FuzzStreamClose(cmsIOHANDLER* iohandler) {
    free(iohandler->stream);
    free(iohandler);
    return TRUE;
}

static cmsIOHANDLER* CreateFuzzIOHandler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    FuzzStream* stream = (FuzzStream*)calloc(1, sizeof(FuzzStream));
    if (!stream) {
        free(io);
        return NULL;
    }
    
    stream->data = data;
    stream->size = size;
    stream->pos = 0;
    
    io->stream = stream;
    io->Read = FuzzStreamRead;
    io->Seek = FuzzStreamSeek;
    io->Tell = FuzzStreamTell;
    io->Write = FuzzStreamWrite;
    io->Close = FuzzStreamClose;
    
    return io;
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    // Test 1: _cms15Fixed16toDouble
    if (Size >= sizeof(cmsS15Fixed16Number)) {
        cmsS15Fixed16Number fix32;
        memcpy(&fix32, Data, sizeof(cmsS15Fixed16Number));
        cmsFloat64Number result1 = _cms15Fixed16toDouble(fix32);
        (void)result1; // Use result to avoid optimization
    }
    
    // Test 2: _cmsRead15Fixed16Number
    if (Size >= 4) {
        cmsIOHANDLER* io = CreateFuzzIOHandler(Data, Size);
        if (io) {
            cmsFloat64Number n;
            cmsBool success = _cmsRead15Fixed16Number(io, &n);
            (void)success;
            io->Close(io);
        }
    }
    
    // Test 3: cmsGetProfileVersion
    if (Size > 0) {
        // Create a minimal ICC profile header (128 bytes minimum)
        size_t profile_size = Size > 128 ? 128 : Size;
        uint8_t* profile_data = (uint8_t*)malloc(profile_size);
        if (profile_data) {
            memcpy(profile_data, Data, profile_size);
            
            // Set ICC profile signature
            if (profile_size >= 128) {
                memcpy(profile_data, "acsp", 4); // ICC signature
                profile_data[8] = 0x02; // Version 2.x
                profile_data[9] = 0x10; // Subversion 2.1
            }
            
            cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data, (cmsUInt32Number)profile_size);
            if (hProfile) {
                cmsFloat64Number version = cmsGetProfileVersion(hProfile);
                (void)version;
                cmsCloseProfile(hProfile);
            }
            free(profile_data);
        }
    }
    
    // Test 4: cmsGetEncodedCMMversion
    int version = cmsGetEncodedCMMversion();
    (void)version;
    
    // Test 5: _cmsDoubleTo15Fixed16
    if (Size >= sizeof(cmsFloat64Number)) {
        cmsFloat64Number v;
        memcpy(&v, Data, sizeof(cmsFloat64Number));
        
        // Ensure value is reasonable to avoid overflow
        if (v > 32767.0) v = 32767.0;
        if (v < -32768.0) v = -32768.0;
        
        cmsS15Fixed16Number result5 = _cmsDoubleTo15Fixed16(v);
        (void)result5;
    }
    
    // Test 6: _cmsWrite15Fixed16Number
    if (Size >= sizeof(cmsFloat64Number) + 4) {
        cmsFloat64Number n;
        memcpy(&n, Data, sizeof(cmsFloat64Number));
        
        // Create write buffer
        uint8_t* write_buffer = (uint8_t*)malloc(Size);
        if (write_buffer) {
            cmsIOHANDLER* io = CreateFuzzIOHandler(write_buffer, Size);
            if (io) {
                cmsBool success = _cmsWrite15Fixed16Number(io, n);
                (void)success;
                io->Close(io);
            }
            free(write_buffer);
        }
    }
    
    return 0;
}