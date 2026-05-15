// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenIOhandlerFromFile at cmsio0.c:378:25 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// cmsOpenIOhandlerFromMem at cmsio0.c:238:25 in lcms2.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2_plugin.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t* data, size_t size, const char* mode) {
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    return cmsOpenIOhandlerFromFile(NULL, "./dummy_file", mode);
}

static void cleanup_iohandler(cmsIOHANDLER* io) {
    if (io) {
        cmsCloseIOhandler(io);
    }
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsIOHANDLER* io = create_dummy_iohandler(Data, Size, "rb");
    if (!io) return 0;
    
    cmsUInt16Number u16_val = 0;
    cmsUInt32Number u32_val = 0;
    cmsBool result = FALSE;
    
    // Test _cmsReadUInt16Number
    result = _cmsReadUInt16Number(io, &u16_val);
    
    // Test _cmsReadUInt32Number
    result = _cmsReadUInt32Number(io, &u32_val);
    
    // Reset to beginning for array tests
    io->Seek(io, 0);
    
    // Test _cmsReadUInt16Array
    if (Size >= 8) {
        cmsUInt32Number array_size = (Size / 2) < 100 ? (Size / 2) : 100;
        cmsUInt16Number* array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
        if (array) {
            result = _cmsReadUInt16Array(io, array_size, array);
            free(array);
        }
    }
    
    cleanup_iohandler(io);
    
    // Now test write functions with a new I/O handler
    io = cmsOpenIOhandlerFromMem(NULL, NULL, 0, "wb");
    if (!io) return 0;
    
    // Test _cmsWriteUInt16Number
    if (Size >= 2) {
        u16_val = (Data[0] << 8) | Data[1];
        result = _cmsWriteUInt16Number(io, u16_val);
    }
    
    // Test _cmsWriteUInt32Number
    if (Size >= 4) {
        u32_val = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        result = _cmsWriteUInt32Number(io, u32_val);
    }
    
    // Test _cmsWriteUInt16Array
    if (Size >= 8) {
        cmsUInt32Number array_size = (Size / 2) < 50 ? (Size / 2) : 50;
        cmsUInt16Number* write_array = (cmsUInt16Number*)malloc(array_size * sizeof(cmsUInt16Number));
        if (write_array) {
            for (cmsUInt32Number i = 0; i < array_size && (i*2+1) < Size; i++) {
                write_array[i] = (Data[i*2] << 8) | Data[i*2+1];
            }
            result = _cmsWriteUInt16Array(io, array_size, write_array);
            free(write_array);
        }
    }
    
    cmsCloseIOhandler(io);
    
    return 0;
}