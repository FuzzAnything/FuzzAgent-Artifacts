// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsWriteXYZNumber at cmsplugin.c:350:20 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsCalloc at cmserr.c:279:17 in lcms2_plugin.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2_plugin.h>

static cmsContext gContext = NULL;

static void cleanup_fuzz(void) {
    if (gContext != NULL) {
        cmsDeleteContext(gContext);
        gContext = NULL;
    }
}

// Helper function to allocate memory using lcms internal allocator
static void* alloc_mem(cmsContext ctx, size_t size) {
    if (ctx == NULL) return NULL;
    return _cmsCalloc(ctx, 1, (cmsUInt32Number)size);
}

// Helper function to free memory using lcms internal allocator
static void free_mem(cmsContext ctx, void* ptr) {
    if (ctx == NULL || ptr == NULL) return;
    _cmsFree(ctx, ptr);
}

// Static function implementations for IO handler callbacks
static cmsUInt32Number dummy_read(struct _cms_io_handler* h, void* buf, cmsUInt32Number size, cmsUInt32Number count) {
    if (h == NULL || buf == NULL) return 0;
    cmsUInt32Number total = size * count;
    if (h->UsedSpace + total > h->ReportedSize) {
        total = h->ReportedSize - h->UsedSpace;
        if (total == 0) return 0;
    }
    memcpy(buf, (uint8_t*)h->stream + h->UsedSpace, total);
    h->UsedSpace += total;
    return total / size;
}

static cmsBool dummy_seek(struct _cms_io_handler* h, cmsUInt32Number offset) {
    if (h == NULL || offset > h->ReportedSize) return FALSE;
    h->UsedSpace = offset;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* h) {
    if (h == NULL) return FALSE;
    if (h->stream != NULL) {
        free_mem(h->ContextID, h->stream);
        h->stream = NULL;
    }
    free_mem(h->ContextID, h);
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* h) {
    if (h == NULL) return 0;
    return h->UsedSpace;
}

static cmsBool dummy_write(struct _cms_io_handler* h, cmsUInt32Number size, const void* buf) {
    if (h == NULL || buf == NULL || size == 0) return FALSE;
    if (h->UsedSpace + size > h->ReportedSize) return FALSE;
    memcpy((uint8_t*)h->stream + h->UsedSpace, buf, size);
    h->UsedSpace += size;
    return TRUE;
}

static cmsIOHANDLER* create_dummy_io_handler(const uint8_t *Data, size_t Size, int is_write_mode) {
    if (gContext == NULL) return NULL;

    cmsIOHANDLER* io = (cmsIOHANDLER*)alloc_mem(gContext, sizeof(cmsIOHANDLER));
    if (io == NULL) return NULL;

    // Allocate a buffer to hold the data
    uint8_t* buffer = (uint8_t*)alloc_mem(gContext, Size);
    if (buffer == NULL) {
        free_mem(gContext, io);
        return NULL;
    }

    if (is_write_mode) {
        // For write mode, we just allocate space, data isn't read from input
        memset(buffer, 0, Size);
    } else {
        // For read mode, copy input data
        memcpy(buffer, Data, Size);
    }

    io->stream = buffer;
    io->ContextID = gContext;
    io->UsedSpace = 0;
    io->ReportedSize = (cmsUInt32Number)Size;
    io->PhysicalFile[0] = '\0';

    // Assign function pointers
    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;

    return io;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure context exists
    if (gContext == NULL) {
        gContext = cmsCreateContext(NULL, NULL);
        if (gContext == NULL) return 0;
    }

    // Cleanup on exit
    atexit(cleanup_fuzz);

    // --- Test _cms15Fixed16toDouble ---
    // Extract a 32-bit value from input to test conversion
    cmsS15Fixed16Number fixed_val = 0;
    if (Size >= 4) {
        memcpy(&fixed_val, Data, 4);
    } else {
        // Pad with zeros if too small
        uint8_t padded[4] = {0};
        memcpy(padded, Data, Size);
        memcpy(&fixed_val, padded, 4);
    }
    cmsFloat64Number double_res = _cms15Fixed16toDouble(fixed_val);
    (void)double_res; // Use result to avoid unused warning

    // --- Test _cmsDoubleTo15Fixed16 ---
    // Use the double result or a derived value
    cmsS15Fixed16Number fixed_back = _cmsDoubleTo15Fixed16(double_res);
    (void)fixed_back;

    // --- Test IO-based functions ---
    // Create a read handler
    cmsIOHANDLER* io_read = create_dummy_io_handler(Data, Size, 0);
    if (io_read != NULL) {
        cmsCIEXYZ xyz_val;
        memset(&xyz_val, 0, sizeof(xyz_val));
        
        // Test _cmsReadXYZNumber (requires 12 bytes)
        if (Size >= 12) {
            _cmsReadXYZNumber(io_read, &xyz_val);
        }

        // Reset position for next read
        io_read->Seek(io_read, 0);

        // Test _cmsRead15Fixed16Number (requires 4 bytes)
        cmsFloat64Number read_double;
        if (Size >= 4) {
            _cmsRead15Fixed16Number(io_read, &read_double);
        }
        
        // Test with NULL pointer for _cmsRead15Fixed16Number
        if (Size >= 4) {
            io_read->Seek(io_read, 0);
            _cmsRead15Fixed16Number(io_read, NULL);
        }

        // Cleanup read handler
        io_read->Close(io_read);
        io_read = NULL;
    }

    // Create a write handler (allocate enough space for XYZ (12 bytes) + 15.16 (4 bytes))
    size_t write_size = (Size > 16) ? Size : 16;
    cmsIOHANDLER* io_write = create_dummy_io_handler(NULL, write_size, 1);
    if (io_write != NULL) {
        cmsCIEXYZ xyz_write;
        xyz_write.X = (double)fixed_val / 65536.0;
        xyz_write.Y = (double)fixed_val / 65536.0;
        xyz_write.Z = (double)fixed_val / 65536.0;

        // Test _cmsWriteXYZNumber
        _cmsWriteXYZNumber(io_write, &xyz_write);

        // Test _cmsWrite15Fixed16Number
        _cmsWrite15Fixed16Number(io_write, double_res);

        // Cleanup write handler
        io_write->Close(io_write);
        io_write = NULL;
    }

    return 0;
}