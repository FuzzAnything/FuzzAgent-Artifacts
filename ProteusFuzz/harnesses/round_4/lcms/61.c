// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsWriteUInt32Number at cmsplugin.c:295:20 in lcms2_plugin.h
// _cmsWriteUInt8Number at cmsplugin.c:258:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt32Number at cmsplugin.c:156:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadUInt16Array at cmsplugin.c:137:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadUInt8Number at cmsplugin.c:111:20 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2_plugin.h"

static cmsContext g_context = NULL;

static void cleanup(void) {
    if (g_context != NULL) {
        cmsDeleteContext(g_context);
        g_context = NULL;
    }
}

static cmsIOHANDLER* create_io_handler(const uint8_t *data, size_t size, int is_write) {
    cmsIOHANDLER *io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    io->ContextID = g_context;
    io->ReportedSize = size;
    io->UsedSpace = 0;

    if (is_write) {
        // For write mode, we simulate writing to a buffer
        // We will use a simple in-memory buffer approach for fuzzing
        // Since we don't have a real file, we'll just track position
        // This is a simplified handler for fuzzing purposes
        // In real scenario, this would write to a file or buffer
        // For fuzzing, we'll just accept writes and return success
        // We need to implement the Write function pointer
        // But since we can't easily store data, we'll just return success
        // This is acceptable for fuzzing to trigger the logic paths
        // We'll use a static buffer for write operations
        static uint8_t write_buffer[1024 * 1024];
        static size_t write_pos = 0;
        
        // Reset write position for each new handler
        write_pos = 0;
        
        // We can't easily pass context to the function pointer without a struct
        // So we'll use a simple approach: just return success for writes
        // This allows the fuzzer to explore the write logic
        // In a real implementation, we'd need a more sophisticated handler
        // For now, we'll just return TRUE for all writes
        // This is safe for fuzzing as it doesn't corrupt memory
        // The actual write logic is what we want to test
        // We'll implement a dummy write that just advances position
        // But we need to store the position somewhere
        // Let's use the stream pointer to store our state
        typedef struct {
            size_t pos;
            uint8_t *buffer;
            size_t size;
        } write_state;
        
        write_state *state = (write_state*)malloc(sizeof(write_state));
        if (state == NULL) {
            free(io);
            return NULL;
        }
        state->pos = 0;
        state->buffer = write_buffer;
        state->size = sizeof(write_buffer);
        io->stream = state;
        
        // We can't set the function pointer here because we need to define it
        // So we'll use a different approach: just return success for all operations
        // This is acceptable for fuzzing to test the logic paths
        // The actual write logic is what we want to test
        // We'll implement the handlers in the main function
    } else {
        // For read mode, we use the provided data
        typedef struct {
            const uint8_t *data;
            size_t pos;
            size_t size;
        } read_state;
        
        read_state *state = (read_state*)malloc(sizeof(read_state));
        if (state == NULL) {
            free(io);
            return NULL;
        }
        state->data = data;
        state->pos = 0;
        state->size = size;
        io->stream = state;
    }
    
    return io;
}

static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    if (iohandler == NULL || iohandler->stream == NULL) return 0;
    
    typedef struct {
        const uint8_t *data;
        size_t pos;
        size_t size;
    } read_state;
    
    read_state *state = (read_state*)iohandler->stream;
    cmsUInt32Number total_size = size * count;
    cmsUInt32Number remaining = state->size - state->pos;
    
    if (total_size > remaining) {
        total_size = remaining;
    }
    
    if (total_size > 0) {
        memcpy(Buffer, state->data + state->pos, total_size);
        state->pos += total_size;
    }
    
    return total_size / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    if (iohandler == NULL || iohandler->stream == NULL) return FALSE;
    
    typedef struct {
        const uint8_t *data;
        size_t pos;
        size_t size;
    } read_state;
    
    read_state *state = (read_state*)iohandler->stream;
    
    if (offset <= state->size) {
        state->pos = offset;
        return TRUE;
    }
    return FALSE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    if (iohandler == NULL) return FALSE;
    if (iohandler->stream != NULL) {
        free(iohandler->stream);
        iohandler->stream = NULL;
    }
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    if (iohandler == NULL || iohandler->stream == NULL) return 0;
    
    typedef struct {
        const uint8_t *data;
        size_t pos;
        size_t size;
    } read_state;
    
    read_state *state = (read_state*)iohandler->stream;
    return state->pos;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler == NULL || iohandler->stream == NULL) return FALSE;
    
    // For fuzzing, we just return success
    // The actual write logic is what we want to test
    return TRUE;
}

static void setup_io_handler(cmsIOHANDLER* io, int is_write) {
    if (io == NULL) return;
    
    if (!is_write) {
        io->Read = dummy_read;
        io->Seek = dummy_seek;
        io->Close = dummy_close;
        io->Tell = dummy_tell;
        io->Write = dummy_write;
    } else {
        // For write mode, we need to set up the write handler
        io->Write = dummy_write;
        // Other functions are not needed for write mode
        io->Read = NULL;
        io->Seek = NULL;
        io->Close = dummy_close;
        io->Tell = NULL;
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Initialize context if not already done
    if (g_context == NULL) {
        g_context = cmsCreateContext(NULL, NULL);
        if (g_context == NULL) return 0;
    }
    
    // Create IO handler for reading
    cmsIOHANDLER *io_read = create_io_handler(Data, Size, 0);
    if (io_read == NULL) return 0;
    setup_io_handler(io_read, 0);
    
    // Create IO handler for writing
    cmsIOHANDLER *io_write = create_io_handler(Data, Size, 1);
    if (io_write == NULL) {
        free(io_read);
        return 0;
    }
    setup_io_handler(io_write, 1);
    
    // Fuzz _cmsWriteUInt32Number
    cmsUInt32Number write_val32 = 0;
    if (Size >= 4) {
        write_val32 = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }
    _cmsWriteUInt32Number(io_write, write_val32);
    
    // Fuzz _cmsWriteUInt8Number
    cmsUInt8Number write_val8 = 0;
    if (Size >= 1) {
        write_val8 = Data[0];
    }
    _cmsWriteUInt8Number(io_write, write_val8);
    
    // Fuzz _cmsReadUInt32Number
    cmsUInt32Number read_val32 = 0;
    _cmsReadUInt32Number(io_read, &read_val32);
    
    // Fuzz _cmsReadUInt32Number with NULL pointer
    _cmsReadUInt32Number(io_read, NULL);
    
    // Fuzz _cmsReadUInt16Array
    cmsUInt32Number array_count = 0;
    if (Size >= 4) {
        array_count = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        // Limit to prevent excessive allocation
        if (array_count > 10000) array_count = 10000;
    }
    
    cmsUInt16Number *array = NULL;
    if (array_count > 0) {
        array = (cmsUInt16Number*)malloc(array_count * sizeof(cmsUInt16Number));
        if (array != NULL) {
            _cmsReadUInt16Array(io_read, array_count, array);
            free(array);
        }
    }
    
    // Fuzz _cmsReadUInt16Array with NULL pointer
    _cmsReadUInt16Array(io_read, array_count, NULL);
    
    // Fuzz _cmsReadUInt8Number
    cmsUInt8Number read_val8 = 0;
    _cmsReadUInt8Number(io_read, &read_val8);
    
    // Fuzz _cmsReadUInt8Number with NULL pointer
    _cmsReadUInt8Number(io_read, NULL);
    
    // Fuzz _cmsReadAlignment
    _cmsReadAlignment(io_read);
    
    // Cleanup
    if (io_read != NULL) {
        dummy_close(io_read);
        free(io_read);
    }
    if (io_write != NULL) {
        dummy_close(io_write);
        free(io_write);
    }
    
    return 0;
}