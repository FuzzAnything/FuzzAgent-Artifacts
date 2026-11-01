// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Custom I/O callbacks for fuzzing */

static tmsize_t
fuzz_read(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    const uint8_t *data = (const uint8_t *)clientdata;
    size_t available = *(size_t *)(data + 8); 
    size_t offset = *(size_t *)(data + 16);   
    
    if (offset > available) {
        return 0;
    }
    if (offset + size > available) {
        size = available - offset;
    }
    if (size <= 0) {
        return 0;
    }
    
    memcpy(buf, data + 24 + offset, size);
    return size;
}

static tmsize_t
fuzz_write(thandle_t clientdata, const tdata_t buf, tmsize_t size) {
    return size;
}

static toff_t
fuzz_seek(thandle_t clientdata, toff_t off, int whence) {
    return off;
}

static int
fuzz_close(thandle_t clientdata) {
    return 0;
}

/* TIFFSizeProc expects unsigned long return type on some platforms */
static unsigned long
fuzz_size(thandle_t clientdata) {
    const uint8_t *data = (const uint8_t *)clientdata;
    size_t sz = *(size_t *)(data + 8);
    return (unsigned long)sz;
}

static int
fuzz_map(thandle_t clientdata, void **pbase, toff_t *psize) {
    const uint8_t *data = (const uint8_t *)clientdata;
    size_t sz = *(size_t *)(data + 8);
    *pbase = (void *)(data + 24);
    *psize = (toff_t)sz;
    return 1;
}

static void
fuzz_unmap(thandle_t clientdata, void *base, toff_t size) {
    /* No-op for fuzzing */
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 32) {
        return 0;
    }

    size_t data_size = 0;
    size_t data_offset = 0;
    
    if (Size >= 8) {
        data_size = *(const size_t *)(Data);
        if (data_size > Size - 24) {
            data_size = Size - 24;
        }
    }
    if (Size >= 16) {
        data_offset = *(const size_t *)(Data + 8);
        if (data_offset > data_size) {
            data_offset = 0;
        }
    }

    size_t client_data_size = 24 + data_size;
    uint8_t *client_data = (uint8_t *)malloc(client_data_size);
    if (!client_data) {
        return 0;
    }

    memcpy(client_data, &data_size, sizeof(size_t));
    memcpy(client_data + 8, &data_offset, sizeof(size_t));
    memcpy(client_data + 24, Data + 16, data_size);

    /* Test TIFFClientOpenExt with NULL options (default behavior) */
    TIFF *tif_ext = TIFFClientOpenExt(
        "fuzz_tiff", "r",
        client_data,
        fuzz_read, fuzz_write,
        fuzz_seek, fuzz_close,
        fuzz_size,
        fuzz_map, fuzz_unmap,
        NULL
    );

    if (tif_ext) {
        TIFFMapFileProc mapproc = TIFFGetMapFileProc(tif_ext);
        (void)mapproc;

        TIFFUnmapFileProc unmapproc = TIFFGetUnmapFileProc(tif_ext);
        (void)unmapproc;

        TIFFReadWriteProc readproc = TIFFGetReadProc(tif_ext);
        (void)readproc;

        TIFFReadWriteProc writeproc = TIFFGetWriteProc(tif_ext);
        (void)writeproc;

        TIFFClose(tif_ext);
    }

    /* Test TIFFClientOpen */
    TIFF *tif = TIFFClientOpen(
        "fuzz_tiff", "r",
        client_data,
        fuzz_read, fuzz_write,
        fuzz_seek, fuzz_close,
        fuzz_size,
        fuzz_map, fuzz_unmap
    );

    if (tif) {
        TIFFMapFileProc mapproc = TIFFGetMapFileProc(tif);
        (void)mapproc;

        TIFFUnmapFileProc unmapproc = TIFFGetUnmapFileProc(tif);
        (void)unmapproc;

        TIFFReadWriteProc readproc = TIFFGetReadProc(tif);
        (void)readproc;

        TIFFReadWriteProc writeproc = TIFFGetWriteProc(tif);
        (void)writeproc;

        TIFFClose(tif);
    }

    free(client_data);
    return 0;
}