// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsDoubleTo8Fixed8 at cmsplugin.c:370:27 in lcms2_plugin.h
// _cms8Fixed8toDouble at cmsplugin.c:365:28 in lcms2_plugin.h
// _cmsDoubleTo15Fixed16 at cmsplugin.c:383:31 in lcms2_plugin.h
// _cms15Fixed16toDouble at cmsplugin.c:377:28 in lcms2_plugin.h
// _cmsWrite15Fixed16Number at cmsplugin.c:337:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
// _cmsRead15Fixed16Number at cmsplugin.c:224:20 in lcms2_plugin.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <lcms2_plugin.h>

// Helper function pointers
static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count);
static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset);
static cmsBool dummy_close(struct _cms_io_handler* iohandler);
static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler);
static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer);

static cmsUInt32Number dummy_read(struct _cms_io_handler* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    char *buf = (char*)iohandler->stream;
    cmsUInt32Number total = size * count;
    if (iohandler->UsedSpace + total > iohandler->ReportedSize) {
        total = iohandler->ReportedSize - iohandler->UsedSpace;
    }
    if (total == 0) return 0;
    memcpy(Buffer, buf + iohandler->UsedSpace, total);
    iohandler->UsedSpace += total;
    return total / size;
}

static cmsBool dummy_seek(struct _cms_io_handler* iohandler, cmsUInt32Number offset) {
    if (offset > iohandler->ReportedSize) return FALSE;
    iohandler->UsedSpace = offset;
    return TRUE;
}

static cmsBool dummy_close(struct _cms_io_handler* iohandler) {
    if (iohandler->stream) free(iohandler->stream);
    iohandler->stream = NULL;
    return TRUE;
}

static cmsUInt32Number dummy_tell(struct _cms_io_handler* iohandler) {
    return iohandler->UsedSpace;
}

static cmsBool dummy_write(struct _cms_io_handler* iohandler, cmsUInt32Number size, const void* Buffer) {
    if (iohandler->stream == NULL) return FALSE;
    if (iohandler->UsedSpace + size > iohandler->ReportedSize) return FALSE;
    memcpy((char*)iohandler->stream + iohandler->UsedSpace, Buffer, size);
    iohandler->UsedSpace += size;
    return TRUE;
}

// Helper to create a dummy IO handler backed by a memory buffer
static cmsIOHANDLER* create_dummy_io_handler(const uint8_t *data, size_t size) {
    cmsIOHANDLER *io = (cmsIOHANDLER*)calloc(1, sizeof(cmsIOHANDLER));
    if (!io) return NULL;

    // Allocate a buffer to hold the data for reading/writing
    char *buffer = (char*)malloc(size);
    if (!buffer) {
        free(io);
        return NULL;
    }

    if (size > 0 && data != NULL) {
        memcpy(buffer, data, size);
    }

    io->stream = buffer;
    io->ContextID = NULL; // Use global context or NULL
    io->UsedSpace = 0;
    io->ReportedSize = size;
    io->PhysicalFile[0] = '\0';

    io->Read = dummy_read;
    io->Seek = dummy_seek;
    io->Close = dummy_close;
    io->Tell = dummy_tell;
    io->Write = dummy_write;

    return io;
}

// Helper to cleanup IO handler
static void cleanup_io_handler(cmsIOHANDLER *io) {
    if (io) {
        if (io->Close) io->Close(io);
        free(io);
    }
}

// Helper to extract double from input
static cmsFloat64Number extract_double(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(double) > Size) {
        // Fallback to a deterministic value if not enough data
        return (cmsFloat64Number)(*offset);
    }
    double val;
    memcpy(&val, Data + *offset, sizeof(double));
    *offset += sizeof(double);
    return (cmsFloat64Number)val;
}

// Helper to extract uint16 from input
static cmsUInt16Number extract_uint16(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(uint16_t) > Size) {
        return (cmsUInt16Number)(*offset);
    }
    uint16_t val;
    memcpy(&val, Data + *offset, sizeof(uint16_t));
    *offset += sizeof(uint16_t);
    return (cmsUInt16Number)val;
}

// Helper to extract int32 (S15Fixed16) from input
static cmsS15Fixed16Number extract_int32(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(int32_t) > Size) {
        return (cmsS15Fixed16Number)(*offset);
    }
    int32_t val;
    memcpy(&val, Data + *offset, sizeof(int32_t));
    *offset += sizeof(int32_t);
    return (cmsS15Fixed16Number)val;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize offset for parsing input
    size_t offset = 0;

    // 1. Test _cmsDoubleTo8Fixed8
    if (offset < Size) {
        cmsFloat64Number val1 = extract_double(Data, Size, &offset);
        cmsUInt16Number res1 = _cmsDoubleTo8Fixed8(val1);
        (void)res1; // Use result to avoid unused variable warning
    }

    // 2. Test _cms8Fixed8toDouble
    if (offset < Size) {
        cmsUInt16Number val2 = extract_uint16(Data, Size, &offset);
        cmsFloat64Number res2 = _cms8Fixed8toDouble(val2);
        (void)res2;
    }

    // 3. Test _cmsDoubleTo15Fixed16
    if (offset < Size) {
        cmsFloat64Number val3 = extract_double(Data, Size, &offset);
        cmsS15Fixed16Number res3 = _cmsDoubleTo15Fixed16(val3);
        (void)res3;
    }

    // 4. Test _cms15Fixed16toDouble
    if (offset < Size) {
        cmsS15Fixed16Number val4 = extract_int32(Data, Size, &offset);
        cmsFloat64Number res4 = _cms15Fixed16toDouble(val4);
        (void)res4;
    }

    // 5. Test _cmsWrite15Fixed16Number and _cmsRead15Fixed16Number
    // We need a buffer to act as the IO stream
    // Reserve some space for the IO handler buffer
    size_t io_buffer_size = 64;
    if (Size > io_buffer_size) io_buffer_size = Size;
    
    // Create a dummy buffer for IO
    uint8_t *io_buf = (uint8_t*)calloc(io_buffer_size, 1);
    if (io_buf) {
        // Copy some data into the buffer if available
        size_t copy_len = (Size < io_buffer_size) ? Size : io_buffer_size;
        if (copy_len > 0) {
            memcpy(io_buf, Data, copy_len);
        }

        cmsIOHANDLER *io = create_dummy_io_handler(io_buf, io_buffer_size);
        if (io) {
            // Test Write
            if (offset < Size) {
                cmsFloat64Number val5 = extract_double(Data, Size, &offset);
                cmsBool write_res = _cmsWrite15Fixed16Number(io, val5);
                (void)write_res;
            }

            // Test Read
            cmsFloat64Number read_val = 0.0;
            cmsBool read_res = _cmsRead15Fixed16Number(io, &read_val);
            (void)read_res;
            (void)read_val;

            // Test Read with NULL pointer (should read and discard)
            cmsBool read_res_null = _cmsRead15Fixed16Number(io, NULL);
            (void)read_res_null;

            cleanup_io_handler(io);
        }
        free(io_buf);
    }

    return 0;
}