// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// _cmsReadXYZNumber at cmsplugin.c:241:20 in lcms2_plugin.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsXYZ2Lab at cmspcs.c:143:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// _cmsMAT3eval at cmsmtrx.c:169:16 in lcms2_plugin.h
// cmsAdaptToIlluminant at cmswtpnt.c:328:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

static void cleanup_context(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static cmsContext create_context(void) {
    return cmsCreateContext(NULL, NULL);
}

// Helper to define dummy IO handler functions outside of the main function
static cmsUInt32Number dummy_read(cmsIOHANDLER* iohandler, void* Buffer, cmsUInt32Number size, cmsUInt32Number count) {
    FILE *f = fopen(iohandler->PhysicalFile, "rb");
    if (!f) return 0;
    cmsUInt32Number total = size * count;
    cmsUInt32Number read_bytes = fread(Buffer, 1, total, f);
    fclose(f);
    return read_bytes / size;
}

static cmsBool dummy_seek(cmsIOHANDLER* iohandler, cmsUInt32Number offset) {
    return TRUE;
}

static cmsBool dummy_close(cmsIOHANDLER* iohandler) {
    return TRUE;
}

static cmsUInt32Number dummy_tell(cmsIOHANDLER* iohandler) {
    return 0;
}

static cmsBool dummy_write(cmsIOHANDLER* iohandler, cmsUInt32Number size, const void* Buffer) {
    return TRUE;
}

static void fuzz_mat3eval(cmsContext ctx, const uint8_t *Data, size_t Size) {
    cmsMAT3 a;
    cmsVEC3 v;
    cmsVEC3 r;

    memset(&a, 0, sizeof(a));
    memset(&v, 0, sizeof(v));
    memset(&r, 0, sizeof(r));

    // cmsMAT3 is typically a 3x3 array of doubles. Size is 9 * 8 = 72 bytes.
    // We copy as much as possible into the matrix structure.
    size_t mat_size = sizeof(a);
    size_t copy_len = Size < mat_size ? Size : mat_size;
    if (copy_len > 0) {
        memcpy(&a, Data, copy_len);
    }

    // cmsVEC3 is typically 3 doubles. Size is 24 bytes.
    size_t vec_size = sizeof(v);
    size_t offset = (Size >= mat_size) ? mat_size : 0;
    size_t remaining = (Size > offset) ? (Size - offset) : 0;
    size_t vec_copy_len = remaining < vec_size ? remaining : vec_size;
    if (vec_copy_len > 0) {
        memcpy(&v, Data + offset, vec_copy_len);
    }

    _cmsMAT3eval(&r, &a, &v);
}

static void fuzz_adapt_to_illuminant(cmsContext ctx, const uint8_t *Data, size_t Size) {
    cmsCIEXYZ source_white, illuminant, value, result;

    memset(&source_white, 0, sizeof(source_white));
    memset(&illuminant, 0, sizeof(illuminant));
    memset(&value, 0, sizeof(value));
    memset(&result, 0, sizeof(result));

    size_t xyz_size = sizeof(cmsCIEXYZ);
    
    if (Size >= xyz_size) {
        memcpy(&source_white, Data, xyz_size);
    }
    if (Size >= xyz_size * 2) {
        memcpy(&illuminant, Data + xyz_size, xyz_size);
    }
    if (Size >= xyz_size * 3) {
        memcpy(&value, Data + xyz_size * 2, xyz_size);
    }

    cmsAdaptToIlluminant(&result, &source_white, &illuminant, &value);
}

static void fuzz_read_xyz_number(cmsContext ctx, const uint8_t *Data, size_t Size) {
    cmsIOHANDLER io;
    cmsCIEXYZ xyz;
    uint8_t buffer[256];
    size_t copy_size = Size < 256 ? Size : 256;

    memset(&io, 0, sizeof(io));
    memset(&xyz, 0, sizeof(xyz));
    memset(buffer, 0, sizeof(buffer));

    if (copy_size > 0) {
        memcpy(buffer, Data, copy_size);
    }

    // Create a dummy file for the IO handler
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(buffer, 1, copy_size, f);
        fclose(f);
    }

    // Initialize IO handler with file
    io.stream = NULL;
    io.ContextID = ctx;
    io.UsedSpace = 0;
    io.ReportedSize = copy_size;
    memset(io.PhysicalFile, 0, cmsMAX_PATH);
    strcpy(io.PhysicalFile, "./dummy_file");

    // Set up minimal Read function
    io.Read = dummy_read;
    io.Seek = dummy_seek;
    io.Close = dummy_close;
    io.Tell = dummy_tell;
    io.Write = dummy_write;

    _cmsReadXYZNumber(&io, &xyz);
}

static void fuzz_d50_xyz(cmsContext ctx, const uint8_t *Data, size_t Size) {
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50) {
        // Use the data to potentially trigger different code paths if any
        volatile double dummy = d50->X + d50->Y + d50->Z;
        (void)dummy;
    }
}

static void fuzz_xyz2lab(cmsContext ctx, const uint8_t *Data, size_t Size) {
    cmsCIEXYZ white_point, xyz;
    cmsCIELab lab;

    memset(&white_point, 0, sizeof(white_point));
    memset(&xyz, 0, sizeof(xyz));
    memset(&lab, 0, sizeof(lab));

    size_t xyz_size = sizeof(cmsCIEXYZ);

    if (Size >= xyz_size) {
        memcpy(&white_point, Data, xyz_size);
    }
    if (Size >= xyz_size * 2) {
        memcpy(&xyz, Data + xyz_size, xyz_size);
    }

    cmsXYZ2Lab(&white_point, &lab, &xyz);
}

static void fuzz_xyz_encoded2float(cmsContext ctx, const uint8_t *Data, size_t Size) {
    cmsCIEXYZ fxyz;
    cmsUInt16Number xyz[3];

    memset(&fxyz, 0, sizeof(fxyz));
    memset(xyz, 0, sizeof(xyz));

    size_t encoded_size = sizeof(cmsUInt16Number) * 3;
    if (Size >= encoded_size) {
        memcpy(xyz, Data, encoded_size);
    } else if (Size > 0) {
        memcpy(xyz, Data, Size);
    }

    cmsXYZEncoded2Float(&fxyz, xyz);
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    cmsContext ctx = create_context();
    if (!ctx) {
        return 0;
    }

    // Fuzz each API function
    fuzz_mat3eval(ctx, Data, Size);
    fuzz_adapt_to_illuminant(ctx, Data, Size);
    fuzz_read_xyz_number(ctx, Data, Size);
    fuzz_d50_xyz(ctx, Data, Size);
    fuzz_xyz2lab(ctx, Data, Size);
    fuzz_xyz_encoded2float(ctx, Data, Size);

    cleanup_context(ctx);
    return 0;
}