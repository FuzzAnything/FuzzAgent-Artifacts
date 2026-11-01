// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static tmsize_t dummy_read(thandle_t fd, void *buf, tmsize_t size) {
    return 0;
}

static tmsize_t dummy_write(thandle_t fd, void *buf, tmsize_t size) {
    return size;
}

static toff_t dummy_seek(thandle_t fd, toff_t off, int whence) {
    return 0;
}

static int dummy_close(thandle_t fd) {
    return 0;
}

// Fix: TIFFSizeProc expects unsigned long return type and unsigned long* size argument in some versions
// But the typedef in tiffio.h usually defines it as tmsize_t (*)(thandle_t).
// The error says: 'tmsize_t (thandle_t)' vs 'unsigned long (*)(void *)'.
// This implies the typedef in the specific version is: typedef unsigned long (*TIFFSizeProc)(void *);
// We must match the signature exactly.
static unsigned long dummy_size(thandle_t fd) {
    return 0;
}

// Fix: TIFFMapFileProc expects 'unsigned long *' for the size argument, not 'toff_t' (which is uint32_t or uint64_t)
// The error says: 'int (void *, void **, unsigned long)' vs 'int (*)(void *, void **, unsigned long *)'
// We need to change the last argument to a pointer to unsigned long.
static int dummy_map(thandle_t fd, void **mapptr, unsigned long *size) {
    *mapptr = NULL;
    if (size) {
        *size = 0;
    }
    return 0;
}

// TIFFUnmapFileProc signature is: void (*)(void *, void *, unsigned long)
// This matches the standard definition.
static void dummy_unmap(thandle_t fd, void *mapptr, unsigned long size) {
    return;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a dummy file
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    TIFF *tif = NULL;
    TIFFReadWriteProc readProc, writeProc;
    TIFFMapFileProc mapProc;
    TIFFUnmapFileProc unmapProc;

    const char *mode = (Data[0] & 0x01) ? "w" : "r";
    
    thandle_t client_data = (thandle_t)malloc(Size > 0 ? Size : 1);
    if (client_data) {
        memcpy(client_data, Data, Size > 0 ? Size : 1);
    }

    tif = TIFFClientOpen(
        "./dummy_file",
        mode,
        client_data,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_map,
        dummy_unmap
    );

    if (tif) {
        readProc = TIFFGetReadProc(tif);
        writeProc = TIFFGetWriteProc(tif);
        mapProc = TIFFGetMapFileProc(tif);
        unmapProc = TIFFGetUnmapFileProc(tif);
        (void)readProc;
        (void)writeProc;
        (void)mapProc;
        (void)unmapProc;

        // Test TIFFClientOpenExt
        // We allocate a buffer for options. Since we can't define the struct, we just allocate memory.
        // The library will likely only read specific fields if they are valid.
        TIFFOpenOptions *opts = NULL;
        // Heuristic size for TIFFOpenOptions. If it fails, the library returns NULL.
        opts = (TIFFOpenOptions *)calloc(1, 128);
        
        if (opts) {
            TIFF *tif_ext = TIFFClientOpenExt(
                "./dummy_file",
                mode,
                client_data,
                dummy_read,
                dummy_write,
                dummy_seek,
                dummy_close,
                dummy_size,
                dummy_map,
                dummy_unmap,
                opts
            );

            if (tif_ext) {
                readProc = TIFFGetReadProc(tif_ext);
                writeProc = TIFFGetWriteProc(tif_ext);
                mapProc = TIFFGetMapFileProc(tif_ext);
                unmapProc = TIFFGetUnmapFileProc(tif_ext);
                (void)readProc;
                (void)writeProc;
                (void)mapProc;
                (void)unmapProc;
                TIFFClose(tif_ext);
            }
            free(opts);
        }

        TIFFClose(tif);
    }

    if (client_data) {
        free(client_data);
    }

    return 0;
}