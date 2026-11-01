// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsReadUInt64Number at cmsplugin.c:206:21 in lcms2_plugin.h
// _cmsReadUInt16Number at cmsplugin.c:124:20 in lcms2_plugin.h
// _cmsWriteUInt64Number at cmsplugin.c:324:20 in lcms2_plugin.h
// _cmsWriteAlignment at cmsplugin.c:462:19 in lcms2_plugin.h
// _cmsReadAlignment at cmsplugin.c:445:19 in lcms2_plugin.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
// cmsCloseIOhandler at cmsio0.c:510:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lcms2.h>
#include <lcms2_plugin.h>

static cmsIOHANDLER* create_dummy_iohandler(const uint8_t *Data, size_t Size, int is_write) {
    cmsIOHANDLER *io = (cmsIOHANDLER *)malloc(sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    memset(io, 0, sizeof(cmsIOHANDLER));

    // Use a simple buffer as the stream
    if (is_write) {
        // For write operations, allocate a buffer to hold data
        io->stream = malloc(Size > 0 ? Size : 1);
        if (!io->stream) {
            free(io);
            return NULL;
        }
    } else {
        // For read operations, use the fuzz input directly
        io->stream = (void *)Data;
    }

    // Set up dummy callbacks
    // Read callback
    io->Read = (cmsUInt32Number (*)(struct _cms_io_handler*, void*, cmsUInt32Number, cmsUInt32Number))
        (void (*)(void))0x12345678; // Placeholder, will be overridden or handled via logic if needed

    // Seek callback
    io->Seek = (cmsBool (*)(struct _cms_io_handler*, cmsUInt32Number))
        (void (*)(void))0x12345678;

    // Close callback
    io->Close = (cmsBool (*)(struct _cms_io_handler*))
        (void (*)(void))0x12345678;

    // Tell callback
    io->Tell = (cmsUInt32Number (*)(struct _cms_io_handler*))
        (void (*)(void))0x12345678;

    // Write callback
    io->Write = (cmsBool (*)(struct _cms_io_handler*, cmsUInt32Number, const void*))
        (void (*)(void))0x12345678;

    io->ContextID = NULL;
    io->UsedSpace = 0;
    io->ReportedSize = Size;
    memset(io->PhysicalFile, 0, sizeof(io->PhysicalFile));

    return io;
}

// Dummy implementations for callbacks to make the fuzzer work without full lcms internals
static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    // This is a simplified dummy read that just returns success
    // In a real scenario, this would read from the stream
    return 0;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    if (iohandler && iohandler->stream) {
        // Only free if we allocated it (write mode)
        // In a real scenario, we'd track this better
        // For fuzzing, we'll just return success
    }
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    return 0;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    return TRUE;
}

static void setup_iohandler_callbacks(cmsIOHANDLER *io) {
    if (!io) return;
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;
}

int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy IO handler for reading
    cmsIOHANDLER *io_read = create_dummy_iohandler(Data, Size, 0);
    if (!io_read) return 0;
    setup_iohandler_callbacks(io_read);

    // Create a dummy IO handler for writing
    cmsIOHANDLER *io_write = create_dummy_iohandler(Data, Size, 1);
    if (!io_write) {
        free(io_read);
        return 0;
    }
    setup_iohandler_callbacks(io_write);

    cmsUInt64Number n64 = 0;
    cmsUInt16Number n16 = 0;
    cmsBool result;

    // Fuzz _cmsReadUInt64Number
    result = _cmsReadUInt64Number(io_read, &n64);
    (void)result; // Suppress unused variable warning

    // Fuzz _cmsReadUInt16Number
    result = _cmsReadUInt16Number(io_read, &n16);
    (void)result;

    // Fuzz _cmsWriteUInt64Number
    n64 = 0x123456789ABCDEF0ULL;
    result = _cmsWriteUInt64Number(io_write, &n64);
    (void)result;

    // Fuzz _cmsWriteAlignment
    result = _cmsWriteAlignment(io_write);
    (void)result;

    // Fuzz _cmsReadAlignment
    result = _cmsReadAlignment(io_read);
    (void)result;

    // Fuzz cmsCloseIOhandler
    // Only close if we haven't passed it to a function that takes ownership
    result = cmsCloseIOhandler(io_read);
    (void)result;

    result = cmsCloseIOhandler(io_write);
    (void)result;

    // Clean up
    if (io_read && io_read->stream && io_read->stream != (void*)Data) {
        free(io_read->stream);
    }
    free(io_read);

    if (io_write && io_write->stream && io_write->stream != (void*)Data) {
        free(io_write->stream);
    }
    free(io_write);

    return 0;
}