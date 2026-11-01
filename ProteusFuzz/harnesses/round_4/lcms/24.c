// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsWriteUInt16Array at cmsplugin.c:281:20 in lcms2_plugin.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt16Number at cmsplugin.c:268:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "lcms2_plugin.h"

/*
 * Custom IO Handler Implementation for Fuzzing
 * Uses a memory buffer to simulate file/stream operations.
 */

typedef struct {
    unsigned char *buffer;
    size_t size;
    size_t pos;
    size_t max_size;
    int is_write_mode;
} fuzz_io_data;

static cmsUInt32Number fuzz_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    fuzz_io_data *data = (fuzz_io_data*)iohandler->stream;
    if (!data || !Buffer) return 0;
    
    cmsUInt32Number total_bytes = size * count;
    if (data->pos + total_bytes > data->size) {
        total_bytes = (cmsUInt32Number)(data->size - data->pos);
    }
    
    if (total_bytes == 0) return 0;
    
    memcpy(Buffer, data->buffer + data->pos, total_bytes);
    data->pos += total_bytes;
    
    return total_bytes / size;
}

static cmsBool fuzz_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    fuzz_io_data *data = (fuzz_io_data*)iohandler->stream;
    if (!data) return FALSE;
    if (offset > data->size) return FALSE;
    data->pos = offset;
    return TRUE;
}

static cmsBool fuzz_close(struct _cms_io_handler* iohandler) {
    // No-op for memory buffer
    return TRUE;
}

static cmsUInt32Number fuzz_tell(struct _cms_io_handler* iohandler) {
    fuzz_io_data *data = (fuzz_io_data*)iohandler->stream;
    if (!data) return 0;
    return (cmsUInt32Number)data->pos;
}

static cmsBool fuzz_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    fuzz_io_data *data = (fuzz_io_data*)iohandler->stream;
    if (!data || !Buffer) return FALSE;
    
    cmsUInt32Number total = size;
    if (data->pos + size > data->max_size) {
        // Out of bounds write simulation
        return FALSE;
    }
    
    memcpy(data->buffer + data->pos, Buffer, size);
    data->pos += size;
    return TRUE;
}

static cmsIOHANDLER* create_fuzz_handler(const uint8_t *Data, size_t Size, int write_mode) {
    cmsIOHANDLER *handler = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!handler) return NULL;
    
    fuzz_io_data *data = (fuzz_io_data*)malloc(sizeof(fuzz_io_data));
    if (!data) {
        free(handler);
        return NULL;
    }
    
    // Initialize handler
    memset(handler, 0, sizeof(cmsIOHANDLER));
    memset(data, 0, sizeof(fuzz_io_data));
    
    handler->stream = data;
    handler->ContextID = NULL; // Use global context or NULL
    handler->UsedSpace = 0;
    handler->ReportedSize = 0;
    handler->Read = fuzz_read;
    handler->Seek = fuzz_seek;
    handler->Close = fuzz_close;
    handler->Tell = fuzz_tell;
    handler->Write = fuzz_write;
    
    if (write_mode) {
        // Allocate a buffer for writing
        data->buffer = (unsigned char*)malloc(Size + 1024); // Extra space
        if (!data->buffer) {
            free(data);
            free(handler);
            return NULL;
        }
        data->max_size = Size + 1024;
        data->size = 0;
        data->pos = 0;
        data->is_write_mode = 1;
    } else {
        // Use input data as buffer for reading
        data->buffer = (unsigned char*)malloc(Size);
        if (!data->buffer) {
            free(data);
            free(handler);
            return NULL;
        }
        memcpy(data->buffer, Data, Size);
        data->size = Size;
        data->max_size = Size;
        data->pos = 0;
        data->is_write_mode = 0;
    }
    
    return handler;
}

static void destroy_fuzz_handler(cmsIOHANDLER *handler) {
    if (!handler) return;
    fuzz_io_data *data = (fuzz_io_data*)handler->stream;
    if (data) {
        free(data->buffer);
        free(data);
    }
    free(handler);
}

/*
 * Fuzz Driver Entry Point
 */
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Ensure minimum size for meaningful operations
    if (Size < 2) return 0;
    
    // Parse simple control bytes from input
    int mode = (Data[0] % 2); // 0 for read, 1 for write
    int operation = (Data[1] % 6); // 0-5 for different API calls
    
    cmsIOHANDLER *handler = NULL;
    
    if (mode == 1) {
        // Write mode
        handler = create_fuzz_handler(Data + 2, Size - 2, 1);
        if (!handler) return 0;
    } else {
        // Read mode
        handler = create_fuzz_handler(Data + 2, Size - 2, 0);
        if (!handler) return 0;
    }
    
    cmsBool result = TRUE;
    cmsUInt32Number u32_val;
    cmsUInt16Number u16_val;
    cmsUInt32Number count = 0;
    
    // Extract count from input if available
    if (Size > 4) {
        count = (Data[2] << 8) | Data[3];
        // Limit count to prevent excessive allocation
        if (count > 10000) count = 10000;
    }
    
    cmsUInt16Number *array = NULL;
    if (count > 0 && mode == 0) {
        array = (cmsUInt16Number*)malloc(count * sizeof(cmsUInt16Number));
        if (!array) {
            destroy_fuzz_handler(handler);
            return 0;
        }
    }
    
    // Execute selected operation
    switch (operation) {
        case 0: // _cmsWriteUInt16Array
            if (mode == 1 && count > 0) {
                array = (cmsUInt16Number*)malloc(count * sizeof(cmsUInt16Number));
                if (array) {
                    memset(array, 0xAA, count * sizeof(cmsUInt16Number));
                    result = _cmsWriteUInt16Array(handler, count, array);
                    free(array);
                }
            }
            break;
            
        case 1: // _cmsWriteUInt32Number
            if (mode == 1) {
                cmsUInt32Number val = (Size > 6) ? ((Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7]) : 0x12345678;
                result = _cmsWriteUInt32Number(handler, val);
            }
            break;
            
        case 2: // _cmsWriteUInt16Number
            if (mode == 1) {
                cmsUInt16Number val = (Size > 4) ? ((Data[4] << 8) | Data[5]) : 0x1234;
                result = _cmsWriteUInt16Number(handler, val);
            }
            break;
            
        case 3: // _cmsReadUInt16Array
            if (mode == 0 && count > 0) {
                result = _cmsReadUInt16Array(handler, count, array);
            }
            break;
            
        case 4: // _cmsReadUInt32Number
            if (mode == 0) {
                result = _cmsReadUInt32Number(handler, &u32_val);
            }
            break;
            
        case 5: // _cmsReadUInt16Number
            if (mode == 0) {
                result = _cmsReadUInt16Number(handler, &u16_val);
            }
            break;
            
        default:
            break;
    }
    
    // Cleanup
    if (array) free(array);
    destroy_fuzz_handler(handler);
    
    return 0;
}