// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
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

// Memory-based I/O handler implementation
static cmsUInt32Number mem_read(cmsIOHANDLER* iohandler, void* buffer,
                                cmsUInt32Number size, cmsUInt32Number count) {
    if (!iohandler || !buffer || size == 0 || count == 0) return 0;
    
    cmsUInt32Number total = size * count;
    cmsUInt32Number available = iohandler->ReportedSize - iohandler->UsedSpace;
    
    if (total > available) {
        total = available - (available % size);
        if (total == 0) return 0;
    }
    
    memcpy(buffer, (uint8_t*)iohandler->stream + iohandler->UsedSpace, total);
    iohandler->UsedSpace += total;
    return total / size;
}

static cmsBool mem_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (!iohandler) return FALSE;
    
    if (offset <= iohandler->ReportedSize) {
        iohandler->UsedSpace = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool mem_close(cmsIOHANDLER* iohandler) {
    if (!iohandler) return FALSE;
    
    if (iohandler->stream) {
        free(iohandler->stream);
        iohandler->stream = NULL;
    }
    free(iohandler);
    return TRUE;
}

static cmsUInt32Number mem_tell(cmsIOHANDLER* iohandler) {
    return iohandler ? iohandler->UsedSpace : 0;
}

static cmsBool mem_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                         const void* buffer) {
    if (!iohandler || !buffer || size == 0) return FALSE;
    
    if (iohandler->UsedSpace + size > iohandler->ReportedSize) {
        cmsUInt32Number new_size = iohandler->UsedSpace + size;
        void* new_stream = realloc(iohandler->stream, new_size);
        if (!new_stream) return FALSE;
        
        iohandler->stream = new_stream;
        iohandler->ReportedSize = new_size;
    }
    
    memcpy((uint8_t*)iohandler->stream + iohandler->UsedSpace, buffer, size);
    iohandler->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_mem_handler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* handler = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!handler) return NULL;
    
    handler->stream = malloc(size);
    if (!handler->stream) {
        free(handler);
        return NULL;
    }
    
    memcpy(handler->stream, data, size);
    handler->ContextID = 0;
    handler->UsedSpace = 0;
    handler->ReportedSize = (cmsUInt32Number)size;
    strncpy(handler->PhysicalFile, "./dummy_file", cmsMAX_PATH - 1);
    handler->PhysicalFile[cmsMAX_PATH - 1] = '\0';
    
    handler->Read = mem_read;
    handler->Seek = mem_seek;
    handler->Close = mem_close;
    handler->Tell = mem_tell;
    handler->Write = mem_write;
    
    return handler;
}

static void test_functions(cmsIOHANDLER* io, const uint8_t* data, size_t size) {
    if (!io) return;
    
    // Test _cmsReadUInt8Number
    cmsUInt8Number read_val = 0;
    if (size > 0) {
        _cmsReadUInt8Number(io, &read_val);
    }
    
    // Test _cmsWriteUInt8Number
    cmsUInt8Number write_val = size > 0 ? data[0] : 0;
    _cmsWriteUInt8Number(io, write_val);
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(io);
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(io);
    
    // Test _cmsIOPrintf with safe string handling
    // Create a safe string buffer to avoid reading beyond input bounds
    char safe_str[256] = {0};
    if (size > 10) {
        // Copy up to 255 bytes from input, ensuring null termination
        size_t copy_len = size - 10;
        if (copy_len > 255) copy_len = 255;
        memcpy(safe_str, &data[10], copy_len);
        safe_str[copy_len] = '\0';
        
        // Ensure the string doesn't contain null bytes in the middle
        for (size_t i = 0; i < copy_len; i++) {
            if (safe_str[i] == '\0') {
                safe_str[i] = 'X'; // Replace null with safe character
            }
        }
        
        _cmsIOPrintf(io, "Test %d, %f, %s", 
                     (int)data[0], 
                     (double)(data[1] * 0.1),
                     safe_str);
    } else {
        _cmsIOPrintf(io, "Simple test");
    }
    
    // Reset position for more operations
    io->Seek(io, 0);
    
    // Try reading again after reset
    if (size > 1) {
        cmsUInt8Number read_val2 = 0;
        _cmsReadUInt8Number(io, &read_val2);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t* data, size_t size) {
    if (size < 1) return 0;
    
    // Create memory-based I/O handler
    cmsIOHANDLER* io = create_mem_handler(data, size);
    if (!io) return 0;
    
    // Test all target functions
    test_functions(io, data, size);
    
    // Test with different positions
    if (size > 4) {
        io->Seek(io, size % 4);
        test_functions(io, data, size);
    }
    
    // Test closing the handler
    cmsCloseIOhandler(io);
    
    return 0;
}