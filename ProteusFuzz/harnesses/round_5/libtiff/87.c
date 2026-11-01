// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Dummy I/O callbacks for fuzzing with correct signatures */
static tmsize_t dummy_read(thandle_t fd, void *buf, tmsize_t size) {
    return 0;
}

static tmsize_t dummy_write(thandle_t fd, void *buf, tmsize_t size) {
    return 0;
}

static toff_t dummy_seek(thandle_t fd, toff_t off, int whence) {
    return 0;
}

static int dummy_close(thandle_t fd) {
    return 0;
}

/* TIFFSizeProc expects unsigned long return type (tmsize_t is signed) */
static unsigned long dummy_size(thandle_t fd) {
    return 0;
}

/* TIFFMapFileProc expects int return type and void** for size */
static int dummy_map(thandle_t fd, void **buf, toff_t *size) {
    return 0;
}

/* TIFFUnmapFileProc expects void return type */
static void dummy_unmap(thandle_t fd, void *buf, toff_t size) {
    return;
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    const char *filename = "./dummy_file";
    const char *mode = "r";
    thandle_t clientdata = NULL;
    
    /* Write dummy data to file if needed */
    FILE *f = fopen(filename, "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }

    /* Try opening with TIFFClientOpen */
    tif = TIFFClientOpen(filename, mode, clientdata,
                         dummy_read, dummy_write,
                         dummy_seek, dummy_close,
                         dummy_size, dummy_map, dummy_unmap);

    if (tif != NULL) {
        /* Fuzz TIFFGetUnmapFileProc */
        TIFFUnmapFileProc unmapproc = TIFFGetUnmapFileProc(tif);
        (void)unmapproc;

        /* Fuzz TIFFGetReadProc */
        TIFFReadWriteProc readproc = TIFFGetReadProc(tif);
        (void)readproc;

        /* Fuzz TIFFGetSeekProc */
        TIFFSeekProc seekproc = TIFFGetSeekProc(tif);
        (void)seekproc;

        /* Fuzz TIFFGetMapFileProc */
        TIFFMapFileProc mapproc = TIFFGetMapFileProc(tif);
        (void)mapproc;

        TIFFClose(tif);
        tif = NULL;
    }

    /* Try opening with TIFFClientOpenExt using NULL options for now */
    tif = TIFFClientOpenExt(filename, mode, clientdata,
                            dummy_read, dummy_write,
                            dummy_seek, dummy_close,
                            dummy_size, dummy_map, dummy_unmap,
                            NULL);

    if (tif != NULL) {
        /* Fuzz TIFFGetUnmapFileProc */
        TIFFUnmapFileProc unmapproc = TIFFGetUnmapFileProc(tif);
        (void)unmapproc;

        /* Fuzz TIFFGetReadProc */
        TIFFReadWriteProc readproc = TIFFGetReadProc(tif);
        (void)readproc;

        /* Fuzz TIFFGetSeekProc */
        TIFFSeekProc seekproc = TIFFGetSeekProc(tif);
        (void)seekproc;

        /* Fuzz TIFFGetMapFileProc */
        TIFFMapFileProc mapproc = TIFFGetMapFileProc(tif);
        (void)mapproc;

        TIFFClose(tif);
        tif = NULL;
    }

    /* Try with different modes */
    mode = "w";
    tif = TIFFClientOpen(filename, mode, clientdata,
                         dummy_read, dummy_write,
                         dummy_seek, dummy_close,
                         dummy_size, dummy_map, dummy_unmap);

    if (tif != NULL) {
        TIFFGetUnmapFileProc(tif);
        TIFFGetReadProc(tif);
        TIFFGetSeekProc(tif);
        TIFFGetMapFileProc(tif);
        TIFFClose(tif);
        tif = NULL;
    }

    tif = TIFFClientOpenExt(filename, mode, clientdata,
                            dummy_read, dummy_write,
                            dummy_seek, dummy_close,
                            dummy_size, dummy_map, dummy_unmap,
                            NULL);

    if (tif != NULL) {
        TIFFGetUnmapFileProc(tif);
        TIFFGetReadProc(tif);
        TIFFGetSeekProc(tif);
        TIFFGetMapFileProc(tif);
        TIFFClose(tif);
        tif = NULL;
    }

    /* Cleanup dummy file */
    remove(filename);

    return 0;
}