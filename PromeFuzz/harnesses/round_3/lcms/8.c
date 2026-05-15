// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>

// Dummy I/O handler implementation for memory operations
static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, 
                                   cmsUInt32Number size, cmsUInt32Number count) {
    if (size == 0 || count == 0) return 0;
    
    cmsUInt32Number total = size * count;
    cmsUInt32Number* pos = (cmsUInt32Number*)iohandler->stream;
    
    if (*pos >= iohandler->ReportedSize) return 0;
    
    cmsUInt32Number available = iohandler->ReportedSize - *pos;
    if (total > available) {
        total = available - (available % size);
        if (total == 0) return 0;
    }
    
    memcpy(Buffer, (const uint8_t*)iohandler->ContextID + *pos, total);
    *pos += total;
    
    return total / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize) return FALSE;
    
    cmsUInt32Number* pos = (cmsUInt32Number*)iohandler->stream;
    *pos = offset;
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    // Nothing to clean up for memory-based handler
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    cmsUInt32Number* pos = (cmsUInt32Number*)iohandler->stream;
    return *pos;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, 
                           const void* Buffer) {
    // For fuzzing purposes, we just pretend to write successfully
    return TRUE;
}

static cmsIOHANDLER* create_dummy_handler(const uint8_t* data, size_t size) {
    static cmsIOHANDLER handler;
    static cmsUInt32Number position = 0;
    
    // Initialize handler fields
    handler.ContextID = (cmsContext)data;
    handler.UsedSpace = 0;
    handler.ReportedSize = (cmsUInt32Number)size;
    handler.PhysicalFile[0] = '\0';
    
    // Set function pointers
    handler.Read = dummy_read;
    handler.Seek = dummy_seek;
    handler.Close = dummy_close;
    handler.Tell = dummy_tell;
    handler.Write = dummy_write;
    
    // Use static variable for position tracking
    position = 0;
    handler.stream = &position;
    
    return &handler;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double) + sizeof(cmsUInt16Number) + sizeof(cmsS15Fixed16Number)) {
        return 0;
    }
    
    // Extract test values from fuzzer input
    double double_val;
    cmsUInt16Number uint16_val;
    cmsS15Fixed16Number fixed32_val;
    
    memcpy(&double_val, Data, sizeof(double));
    memcpy(&uint16_val, Data + sizeof(double), sizeof(cmsUInt16Number));
    memcpy(&fixed32_val, Data + sizeof(double) + sizeof(cmsUInt16Number), 
           sizeof(cmsS15Fixed16Number));
    
    // Test conversion functions
    cmsUInt16Number result_8fixed8 = _cmsDoubleTo8Fixed8(double_val);
    cmsS15Fixed16Number result_15fixed16 = _cmsDoubleTo15Fixed16(double_val);
    
    double back_from_8fixed8 = _cms8Fixed8toDouble(uint16_val);
    double back_from_15fixed16 = _cms15Fixed16toDouble(fixed32_val);
    
    // Test I/O functions
    cmsIOHANDLER* io_handler = create_dummy_handler(Data, Size);
    
    cmsFloat64Number read_value;
    cmsBool read_result = _cmsRead15Fixed16Number(io_handler, &read_value);
    
    cmsBool write_result = _cmsWrite15Fixed16Number(io_handler, double_val);
    
    // Clean up
    dummy_close(io_handler);
    
    // Use results to avoid dead code elimination
    (void)result_8fixed8;
    (void)result_15fixed16;
    (void)back_from_8fixed8;
    (void)back_from_15fixed16;
    (void)read_result;
    (void)write_result;
    
    return 0;
}