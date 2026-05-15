// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetProfileIOhandler at cmsio0.c:517:25 in lcms2.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsWriteTypeBase at cmsplugin.c:434:20 in lcms2_plugin.h
// _cmsReadTypeBase at cmsplugin.c:421:31 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
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
#include <stdio.h>

// Custom memory-based I/O handler implementation
typedef struct {
    uint8_t* data;
    size_t size;
    size_t pos;
    cmsBool is_readable;
    cmsBool is_writable;
} MemoryStream;

static cmsUInt32Number MemoryRead(cmsIOHANDLER* iohandler, void* buffer, cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    if (!stream || !stream->is_readable) return 0;
    
    size_t total_bytes = (size_t)size * count;
    if (stream->pos + total_bytes > stream->size) {
        total_bytes = stream->size - stream->pos;
        if (total_bytes == 0) return 0;
    }
    
    memcpy(buffer, stream->data + stream->pos, total_bytes);
    stream->pos += total_bytes;
    return (cmsUInt32Number)(total_bytes / size);
}

static cmsBool MemorySeek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    if (!stream) return FALSE;
    
    if (offset <= stream->size) {
        stream->pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool MemoryClose(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    if (stream) {
        if (stream->data) free(stream->data);
        free(stream);
        iohandler->stream = NULL;
    }
    return TRUE;
}

static cmsUInt32Number MemoryTell(cmsIOHANDLER* iohandler) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    return stream ? (cmsUInt32Number)stream->pos : 0;
}

static cmsBool MemoryWrite(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* buffer) {
    MemoryStream* stream = (MemoryStream*)iohandler->stream;
    if (!stream || !stream->is_writable) return FALSE;
    
    // Simple implementation - just track position without actual writing
    // since we're fuzzing and don't need to preserve output
    stream->pos += size;
    if (stream->pos > stream->size) {
        stream->pos = stream->size;
    }
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryIOHandler(const uint8_t* data, size_t size, cmsBool for_reading) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;
    
    MemoryStream* stream = (MemoryStream*)calloc(1, sizeof(MemoryStream));
    if (!stream) {
        free(io);
        return NULL;
    }
    
    if (for_reading && size > 0) {
        stream->data = (uint8_t*)malloc(size);
        if (!stream->data) {
            free(stream);
            free(io);
            return NULL;
        }
        memcpy(stream->data, data, size);
        stream->size = size;
        stream->is_readable = TRUE;
    } else {
        stream->data = NULL;
        stream->size = 1024; // Arbitrary buffer size for writing
        stream->is_writable = TRUE;
    }
    stream->pos = 0;
    
    io->stream = stream;
    io->Read = MemoryRead;
    io->Seek = MemorySeek;
    io->Close = MemoryClose;
    io->Tell = MemoryTell;
    io->Write = MemoryWrite;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)stream->size;
    
    return io;
}

static void CleanupMemoryIOHandler(cmsIOHANDLER* io) {
    if (io) {
        if (io->Close) io->Close(io);
        free(io);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Create a dummy profile for testing cmsGetProfileIOhandler
    cmsHPROFILE dummy_profile = NULL;
    
    // Try to create profile from input data
    if (Size > 0) {
        dummy_profile = cmsOpenProfileFromMem(Data, (cmsUInt32Number)(Size > 0xFFFFFFFF ? 0xFFFFFFFF : Size));
    }
    
    // If profile creation failed, create a placeholder
    if (!dummy_profile) {
        dummy_profile = cmsCreate_sRGBProfile();
    }
    
    // Test cmsGetProfileIOhandler
    cmsIOHANDLER* profile_io = cmsGetProfileIOhandler(dummy_profile);
    
    // Create memory-based I/O handlers for testing other functions
    cmsIOHANDLER* read_io = CreateMemoryIOHandler(Data, Size, TRUE);
    cmsIOHANDLER* write_io = CreateMemoryIOHandler(NULL, 0, FALSE);
    
    if (read_io) {
        // Test _cmsReadAlignment
        _cmsReadAlignment(read_io);
        
        // Test _cmsReadTypeBase
        _cmsReadTypeBase(read_io);
        
        // Reset position for more testing
        if (read_io->Seek) read_io->Seek(read_io, 0);
        
        // Test reading alignment again from different positions
        if (Size > 4) {
            read_io->Seek(read_io, (cmsUInt32Number)(Size % 4));
            _cmsReadAlignment(read_io);
        }
    }
    
    if (write_io) {
        // Test _cmsWriteAlignment
        _cmsWriteAlignment(write_io);
        
        // Test _cmsWriteUInt64Number
        cmsUInt64Number test_num = 0;
        if (Size >= sizeof(cmsUInt64Number)) {
            memcpy(&test_num, Data, sizeof(cmsUInt64Number));
        } else if (Size > 0) {
            memcpy(&test_num, Data, Size);
        }
        _cmsWriteUInt64Number(write_io, &test_num);
        
        // Test _cmsWriteTypeBase
        cmsTagTypeSignature test_sig = 0;
        if (Size >= sizeof(cmsTagTypeSignature)) {
            memcpy(&test_sig, Data, sizeof(cmsTagTypeSignature));
        } else if (Size > 0) {
            memcpy(&test_sig, Data, Size);
        }
        _cmsWriteTypeBase(write_io, test_sig);
        
        // Write alignment after writing data
        _cmsWriteAlignment(write_io);
        
        // Test with different signatures
        if (Size > 4) {
            memcpy(&test_sig, Data + 4, sizeof(cmsTagTypeSignature));
            _cmsWriteTypeBase(write_io, test_sig);
        }
    }
    
    // Test with profile I/O handler if available
    if (profile_io) {
        // These operations might fail depending on the profile state
        _cmsReadAlignment(profile_io);
        _cmsWriteAlignment(profile_io);
        
        cmsUInt64Number test_num = 0x123456789ABCDEF0;
        _cmsWriteUInt64Number(profile_io, &test_num);
        
        cmsTagTypeSignature test_sig = 0x6D6E7472; // 'mntr' signature
        _cmsWriteTypeBase(profile_io, test_sig);
        
        _cmsReadTypeBase(profile_io);
    }
    
    // Cleanup
    if (dummy_profile) cmsCloseProfile(dummy_profile);
    if (read_io) CleanupMemoryIOHandler(read_io);
    if (write_io) CleanupMemoryIOHandler(write_io);
    
    return 0;
}