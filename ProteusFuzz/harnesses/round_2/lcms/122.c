// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2_plugin.h"

static cmsContext gContext = NULL;

static void cleanup(void) {
    if (gContext != NULL) {
        cmsDeleteContext(gContext);
        gContext = NULL;
    }
}

// Helper function to read from the simulated stream
static cmsUInt32Number static_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    const uint8_t* data = (const uint8_t*)iohandler->stream;
    cmsUInt32Number total = size * count;
    cmsUInt32Number remaining = iohandler->ReportedSize - iohandler->UsedSpace;
    cmsUInt32Number to_read = (total < remaining) ? total : remaining;
    if (to_read > 0) {
        memcpy(Buffer, data + iohandler->UsedSpace, to_read);
        iohandler->UsedSpace += to_read;
    }
    return to_read / size;
}

// Helper function to seek in the simulated stream
static cmsBool static_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize) return FALSE;
    iohandler->UsedSpace = offset;
    return TRUE;
}

// Helper function to close the simulated stream
static cmsBool static_close(struct _cms_io_handler* iohandler) {
    return TRUE;
}

// Helper function to tell the position in the simulated stream
static cmsUInt32Number static_tell(struct _cms_io_handler* iohandler) {
    return iohandler->UsedSpace;
}

// Helper function to write to the simulated stream
static cmsBool static_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    // In a real scenario, this would write to a file or buffer
    // Here we just return success to allow the function to complete
    (void)iohandler;
    (void)size;
    (void)Buffer;
    return TRUE;
}

static cmsIOHANDLER* create_io_handler(const uint8_t* data, size_t size) {
    cmsIOHANDLER* io = (cmsIOHANDLER*)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));
    
    // Use a simple memory-based implementation for Read/Write/Seek/Tell
    // We simulate the stream with the input data
    io->stream = (void*)data;
    io->ContextID = gContext;
    io->UsedSpace = 0;
    io->ReportedSize = size;
    strcpy(io->PhysicalFile, "./dummy_file");

    // Implement Read
    io->Read = static_read;

    // Implement Seek
    io->Seek = static_seek;

    // Implement Close (no-op for memory)
    io->Close = static_close;

    // Implement Tell
    io->Tell = static_tell;

    // Implement Write (simulate writing to a buffer or dummy file)
    // For fuzzing, we just track that a write was attempted and succeed
    io->Write = static_write;

    return io;
}

static void free_io_handler(cmsIOHANDLER* io) {
    if (io) {
        free(io);
    }
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    cleanup();
    
    // Create a context
    gContext = cmsCreateContext(NULL, NULL);
    if (gContext == NULL) {
        return 0;
    }

    // Create IO handler from input data
    cmsIOHANDLER* io = create_io_handler(Data, Size);
    if (!io) {
        cleanup();
        return 0;
    }

    cmsBool result;
    cmsFloat64Number double_val;
    cmsS15Fixed16Number fixed_val;
    cmsCIEXYZ xyz;

    // Test _cmsRead15Fixed16Number
    // Ensure we have enough data (4 bytes for one number)
    if (Size >= 4) {
        result = _cmsRead15Fixed16Number(io, &double_val);
        (void)result; // Ignore result for fuzzing exploration
    }

    // Test _cmsReadXYZNumber
    // Requires 12 bytes (3 * 4 bytes)
    if (Size >= 12) {
        // Reset position
        io->Seek(io, 0);
        result = _cmsReadXYZNumber(io, &xyz);
        (void)result;
    }

    // Test _cms15Fixed16toDouble with various inputs
    // Use input data to generate fixed point values
    if (Size >= 4) {
        fixed_val = *(const cmsS15Fixed16Number*)(Data);
        double_val = _cms15Fixed16toDouble(fixed_val);
        (void)double_val;
    }

    // Test _cmsDoubleTo15Fixed16
    // Use input data to generate double values if possible
    if (Size >= 8) {
        double_val = *(const double*)(Data);
        // Clamp to avoid overflow
        if (double_val > 32767.5) double_val = 32767.5;
        if (double_val < -32767.5) double_val = -32767.5;
        fixed_val = _cmsDoubleTo15Fixed16(double_val);
        (void)fixed_val;
    }

    // Test _cmsWriteXYZNumber
    // Prepare some XYZ data
    xyz.X = (Size > 0) ? (double)Data[0] / 255.0 : 0.0;
    xyz.Y = (Size > 1) ? (double)Data[1] / 255.0 : 0.0;
    xyz.Z = (Size > 2) ? (double)Data[2] / 255.0 : 0.0;
    result = _cmsWriteXYZNumber(io, &xyz);
    (void)result;

    // Test _cmsWrite15Fixed16Number
    if (Size >= 8) {
        double_val = *(const double*)(Data);
        result = _cmsWrite15Fixed16Number(io, double_val);
        (void)result;
    }

    // Cleanup
    free_io_handler(io);
    cleanup();

    return 0;
}