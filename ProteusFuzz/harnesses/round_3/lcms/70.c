// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static cmsContext gCtx = NULL;

// Custom memory-based IO handler implementation for fuzzing
static cmsUInt32Number mem_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler == NULL || Buffer == NULL || size == 0 || count == 0) return 0;
    // Simplified stub: returns 0 to indicate EOF or error, forcing the target functions to handle failures
    return 0;
}

static cmsBool mem_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (iohandler == NULL) return FALSE;
    return TRUE;
}

static cmsBool mem_close(struct _cms_io_handler* iohandler) {
    if (iohandler == NULL) return FALSE;
    return TRUE;
}

static cmsUInt32Number mem_tell(struct _cms_io_handler* iohandler) {
    if (iohandler == NULL) return 0;
    return 0;
}

static cmsBool mem_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler == NULL || Buffer == NULL) return FALSE;
    return TRUE;
}

static cmsIOHANDLER* create_io_handler_from_data(const uint8_t *Data, size_t Size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;
    
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = gCtx;
    io->stream = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = Size;
    io->PhysicalFile[0] = '\0';
    
    io->Read = mem_read;
    io->Seek = mem_seek;
    io->Close = mem_close;
    io->Tell = mem_tell;
    io->Write = mem_write;
    
    return io;
}

static cmsIOHANDLER* create_write_io_handler(void) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;
    
    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = gCtx;
    io->stream = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = 0;
    io->PhysicalFile[0] = '\0';
    
    io->Read = mem_read;
    io->Seek = mem_seek;
    io->Close = mem_close;
    io->Tell = mem_tell;
    io->Write = mem_write;
    
    // Allocate a dummy buffer for writing
    void* buf = malloc(65536);
    if (buf) {
        memset(buf, 0, 65536);
        io->stream = buf;
    }
    
    return io;
}

static void destroy_io_handler(cmsIOHANDLER* io) {
    if (io == NULL) return;
    if (io->Close != NULL) {
        io->Close(io);
    }
    // Free the stream if it was allocated by us
    if (io->stream != NULL) {
        free(io->stream);
    }
    free(io);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    if (gCtx == NULL) {
        gCtx = cmsCreateContext(NULL, NULL);
        if (gCtx == NULL) return 0;
    }

    cmsUInt32Number count = 0;
    cmsUInt32Number value = 0;
    cmsUInt16Number* array = NULL;
    cmsUInt16Number* read_array = NULL;
    cmsUInt32Number read_value = 0;
    
    if (Size >= 4) {
        count = Data[0] * 256 + Data[1]; 
        if (Size >= 6) {
            value = (Data[2] << 24) | (Data[3] << 16) | (Data[4] << 8) | Data[5];
        }
        if (count > 10000) count = 10000;
    }
    
    if (count > 0) {
        array = (cmsUInt16Number*)malloc(count * sizeof(cmsUInt16Number));
        read_array = (cmsUInt16Number*)malloc(count * sizeof(cmsUInt16Number));
        if (array == NULL || read_array == NULL) {
            free(array);
            free(read_array);
            return 0;
        }
        
        for (cmsUInt32Number i = 0; i < count; i++) {
            if (i < Size / 2) {
                array[i] = Data[i * 2] | (Data[i * 2 + 1] << 8);
            } else {
                array[i] = 0;
            }
        }
    }

    // Test Read Operations
    {
        cmsIOHANDLER* io = create_io_handler_from_data(Data, Size);
        if (io != NULL) {
            cmsUInt32Number r_val;
            _cmsReadUInt32Number(io, &r_val);
            
            if (count > 0) {
                _cmsReadUInt16Array(io, count, read_array);
            } else {
                // Pass NULL array only if count is 0, or ensure array is valid if count > 0
                // The API documentation says: "If Array is NULL, it skips data without storing."
                // But the assert in _cmsWriteUInt16Array suggests strict checks.
                // Let's try passing a valid array even for count 0 if possible, or just skip if count is 0.
                // Actually, if count is 0, we can pass NULL safely based on the description.
                _cmsReadUInt16Array(io, 0, NULL);
            }
            
            _cmsReadAlignment(io);
            
            destroy_io_handler(io);
        }
    }

    // Test Write Operations
    {
        cmsIOHANDLER* io = create_write_io_handler();
        if (io != NULL) {
            _cmsWriteUInt32Number(io, value);
            
            if (count > 0 && array != NULL) {
                _cmsWriteUInt16Array(io, count, array);
            } else {
                // The assert _cmsAssert(Array != NULL) failed when passing NULL.
                // We must provide a valid array pointer even if count is 0.
                // Allocate a small dummy array to satisfy the assertion.
                cmsUInt16Number dummy_array[1] = {0};
                _cmsWriteUInt16Array(io, 0, dummy_array);
            }
            
            _cmsWriteAlignment(io);
            
            destroy_io_handler(io);
        }
    }

    free(array);
    free(read_array);
    
    return 0;
}