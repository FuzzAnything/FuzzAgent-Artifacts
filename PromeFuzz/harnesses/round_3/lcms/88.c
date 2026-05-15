// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsIOPrintf at cmsplugin.c:482:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadFloat32Number at cmsplugin.c:169:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "lcms2_plugin.h"

#define MAX_BUFFER_SIZE 4096

static cmsUInt32Number test_read(cmsIOHANDLER* iohandler, void* buffer,
                                 cmsUInt32Number size, cmsUInt32Number count) {
    cmsUInt32Number total = size * count;
    if (total > MAX_BUFFER_SIZE) total = MAX_BUFFER_SIZE;
    
    // Simulate reading from fuzzer data
    static uint8_t test_data[MAX_BUFFER_SIZE];
    static size_t test_pos = 0;
    
    if (test_pos + total > MAX_BUFFER_SIZE) {
        test_pos = 0;
    }
    
    memcpy(buffer, test_data + test_pos, total);
    test_pos += total;
    return count;
}

static cmsBool test_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    // Always succeed for fuzzing
    return TRUE;
}

static cmsBool test_close(cmsIOHANDLER* iohandler) {
    // Always succeed for fuzzing
    return TRUE;
}

static cmsUInt32Number test_tell(cmsIOHANDLER* iohandler) {
    // Return current position
    return 0;
}

static cmsBool test_write(cmsIOHANDLER* iohandler, cmsUInt32Number size,
                          const void* buffer) {
    // Always succeed for fuzzing
    return TRUE;
}

static cmsIOHANDLER* create_test_handler(void) {
    cmsIOHANDLER* handler = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!handler) return NULL;
    
    memset(handler, 0, sizeof(cmsIOHANDLER));
    handler->Read = test_read;
    handler->Seek = test_seek;
    handler->Close = test_close;
    handler->Tell = test_tell;
    handler->Write = test_write;
    handler->UsedSpace = 0;
    handler->ReportedSize = MAX_BUFFER_SIZE;
    
    return handler;
}

static void destroy_test_handler(cmsIOHANDLER* handler) {
    if (handler) {
        free(handler);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize test data for read operations
    static uint8_t test_data[MAX_BUFFER_SIZE];
    size_t copy_size = Size > MAX_BUFFER_SIZE ? MAX_BUFFER_SIZE : Size;
    memcpy(test_data, Data, copy_size);
    
    cmsIOHANDLER* io = create_test_handler();
    if (!io) return 0;
    
    // Test _cmsWriteAlignment
    _cmsWriteAlignment(io);
    
    // Test _cmsIOPrintf with various format strings
    const char* formats[] = {
        "Test: %d",
        "Value: %.2f",
        "String: %s",
        "Hex: %x",
        NULL
    };
    
    for (int i = 0; formats[i] != NULL; i++) {
        _cmsIOPrintf(io, formats[i], 42);
    }
    
    // Test _cmsReadAlignment
    _cmsReadAlignment(io);
    
    // Test _cmsWriteUInt8Number with different values
    for (int i = 0; i < 10 && i < (int)Size; i++) {
        _cmsWriteUInt8Number(io, Data[i]);
    }
    
    // Test _cmsReadFloat32Number
    cmsFloat32Number float_val = 0.0f;
    _cmsReadFloat32Number(io, &float_val);
    
    // Test _cmsReadUInt8Number
    cmsUInt8Number byte_val = 0;
    _cmsReadUInt8Number(io, &byte_val);
    
    // Test with different handler states
    io->UsedSpace = Size % 100;
    io->ReportedSize = Size % 1000 + 100;
    
    // Test alignment functions again with different states
    _cmsWriteAlignment(io);
    _cmsReadAlignment(io);
    
    // Test writing multiple bytes
    for (int i = 0; i < 20 && i < (int)Size; i++) {
        _cmsWriteUInt8Number(io, Data[i] ^ 0xFF);
    }
    
    // Test reading multiple values
    for (int i = 0; i < 5; i++) {
        _cmsReadUInt8Number(io, &byte_val);
        _cmsReadFloat32Number(io, &float_val);
    }
    
    // Test with maximum values
    _cmsWriteUInt8Number(io, 0xFF);
    _cmsWriteUInt8Number(io, 0x00);
    _cmsWriteUInt8Number(io, 0x7F);
    
    destroy_test_handler(io);
    
    return 0;
}