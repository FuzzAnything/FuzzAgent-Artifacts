// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Memory stream implementation for cmsIOHANDLER
typedef struct {
    const uint8_t *data;
    size_t size;
    size_t pos;
} MemoryStream;

static cmsUInt32Number ReadFromMemory(cmsIOHANDLER* iohandler, void* Buffer, 
                                      cmsUInt32Number size, cmsUInt32Number count) {
    MemoryStream* ms = (MemoryStream*)iohandler->stream;
    cmsUInt32Number total_bytes = size * count;
    cmsUInt32Number bytes_to_read = total_bytes;
    
    if (ms->pos + bytes_to_read > ms->size) {
        bytes_to_read = ms->size - ms->pos;
    }
    
    if (bytes_to_read > 0) {
        memcpy(Buffer, ms->data + ms->pos, bytes_to_read);
        ms->pos += bytes_to_read;
    }
    
    return bytes_to_read / size;
}

static cmsBool SeekInMemory(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    MemoryStream* ms = (MemoryStream*)iohandler->stream;
    if (offset > ms->size) {
        return FALSE;
    }
    ms->pos = offset;
    return TRUE;
}

static cmsBool CloseMemory(cmsIOHANDLER* iohandler) {
    // Nothing to clean up for memory stream
    return TRUE;
}

static cmsUInt32Number TellInMemory(cmsIOHANDLER* iohandler) {
    MemoryStream* ms = (MemoryStream*)iohandler->stream;
    return ms->pos;
}

static cmsBool WriteToMemory(cmsIOHANDLER* iohandler, cmsUInt32Number size, 
                            const void* Buffer) {
    MemoryStream* ms = (MemoryStream*)iohandler->stream;
    
    if (ms->pos + size > ms->size) {
        return FALSE;
    }
    
    memcpy((void*)(ms->data + ms->pos), Buffer, size);
    ms->pos += size;
    return TRUE;
}

static cmsIOHANDLER* CreateMemoryHandler(const uint8_t *data, size_t size) {
    static cmsIOHANDLER handler;
    static MemoryStream stream;
    
    stream.data = data;
    stream.size = size;
    stream.pos = 0;
    
    handler.stream = &stream;
    handler.Read = ReadFromMemory;
    handler.Seek = SeekInMemory;
    handler.Close = CloseMemory;
    handler.Tell = TellInMemory;
    handler.Write = WriteToMemory;
    handler.ContextID = NULL;
    handler.UsedSpace = 0;
    handler.ReportedSize = 0;
    memset(handler.PhysicalFile, 0, sizeof(handler.PhysicalFile));
    
    return &handler;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Test _cms8Fixed8toDouble
    if (Size >= 2) {
        cmsUInt16Number fixed8 = (Data[0] << 8) | Data[1];
        cmsFloat64Number result = _cms8Fixed8toDouble(fixed8);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test _cms15Fixed16toDouble
    if (Size >= 4) {
        cmsS15Fixed16Number fix32 = (Data[0] << 24) | (Data[1] << 16) | 
                                   (Data[2] << 8) | Data[3];
        cmsFloat64Number result = _cms15Fixed16toDouble(fix32);
        (void)result;
    }
    
    // Test _cmsDoubleTo8Fixed8 and _cmsDoubleTo15Fixed16
    if (Size >= 8) {
        // Interpret 8 bytes as double (might be invalid, but that's what fuzzing is for)
        cmsFloat64Number val;
        memcpy(&val, Data, sizeof(cmsFloat64Number));
        
        cmsUInt16Number result8 = _cmsDoubleTo8Fixed8(val);
        (void)result8;
        
        cmsS15Fixed16Number result16 = _cmsDoubleTo15Fixed16(val);
        (void)result16;
    }
    
    // Test _cmsRead15Fixed16Number
    if (Size >= 4) {
        cmsIOHANDLER* io = CreateMemoryHandler(Data, Size);
        cmsFloat64Number n;
        cmsBool read_result = _cmsRead15Fixed16Number(io, &n);
        (void)read_result;
        (void)n;
    }
    
    // Test _cmsWrite15Fixed16Number
    if (Size >= 8) {
        // Create a buffer for writing
        uint8_t write_buffer[16];
        memset(write_buffer, 0, sizeof(write_buffer));
        
        cmsIOHANDLER* io = CreateMemoryHandler(write_buffer, sizeof(write_buffer));
        
        // Interpret 8 bytes as double
        cmsFloat64Number val;
        memcpy(&val, Data, sizeof(cmsFloat64Number));
        
        cmsBool write_result = _cmsWrite15Fixed16Number(io, val);
        (void)write_result;
    }
    
    // Test with file-based operations using "./dummy_file"
    if (Size > 0) {
        FILE* fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
            fclose(fp);
            
            // Re-open for reading if needed (though our API uses cmsIOHANDLER, not FILE*)
            remove("./dummy_file");
        }
    }
    
    return 0;
}