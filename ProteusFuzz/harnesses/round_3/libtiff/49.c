// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <tiffio.h>

// Helper functions with correct signatures matching libtiff expectations

static tmsize_t dummy_read(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummy_write(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

static toff_t dummy_seek(thandle_t fd, toff_t off, int whence) {
    (void)fd;
    (void)off;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t fd) {
    (void)fd;
    return 0;
}

// TIFFSizeProc expects: unsigned long (*)(thandle_t)
static unsigned long dummy_size(thandle_t fd) {
    (void)fd;
    return 0;
}

// TIFFMapFileProc expects: int (*)(thandle_t, void **, unsigned long *)
// Based on error: int (*)(void *, void **, unsigned long *)
static int dummy_map(thandle_t fd, void **buf, unsigned long *size) {
    (void)fd;
    (void)buf;
    (void)size;
    return 0;
}

// TIFFUnmapFileProc expects: void (*)(thandle_t, void*, unsigned long)
static void dummy_unmap(thandle_t fd, void *buf, unsigned long size) {
    (void)fd;
    (void)buf;
    (void)size;
}

// TIFFErrorHandlerExtR expects: int (*)(TIFF*, void*, const char*, const char*, va_list)
static int dummy_error(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

// TIFFErrorHandlerExtR expects: int (*)(TIFF*, void*, const char*, const char*, va_list)
static int dummy_warn(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    int fd = open(dummy_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write input data to the dummy file
    write(fd, Data, Size);
    lseek(fd, 0, SEEK_SET);

    TIFF *tif = NULL;
    
    // Use pointer for TIFFOpenOptions since the struct is opaque/incomplete in some builds
    // We will pass NULL or a valid pointer if we can construct it, but for fuzzing NULL is often safe
    // However, the API expects a pointer. Let's try to allocate and zero it if possible, 
    // but since we can't see the fields, we'll pass NULL if the struct is truly opaque.
    // Actually, the error says "incomplete type", so we cannot instantiate it.
    // We must pass NULL or a pointer to a zeroed buffer if we know the size, but we don't.
    // Let's try passing NULL first, as many APIs handle NULL options gracefully.
    TIFFOpenOptions *opts = NULL;

    // 1. Test TIFFFdOpenExt
    tif = TIFFFdOpenExt(fd, dummy_file, "w+", opts);
    if (tif != NULL) {
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        (void)seek_proc;

        thandle_t client_data = (thandle_t)Data;
        thandle_t old_data = TIFFSetClientdata(tif, client_data);
        (void)old_data;

        TIFFClose(tif);
        tif = NULL;
    }

    close(fd);
    unlink(dummy_file);

    // 2. Test TIFFClientOpen
    tif = TIFFClientOpen(dummy_file, "w+", (thandle_t)Data,
                         dummy_read, dummy_write, dummy_seek,
                         dummy_close, dummy_size, dummy_map, dummy_unmap);
    if (tif != NULL) {
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        (void)seek_proc;

        thandle_t client_data = (thandle_t)(Data + 1);
        thandle_t old_data = TIFFSetClientdata(tif, client_data);
        (void)old_data;

        TIFFClose(tif);
        tif = NULL;
    }

    // 3. Test TIFFClientOpenExt
    tif = TIFFClientOpenExt(dummy_file, "w+", (thandle_t)Data,
                            dummy_read, dummy_write, dummy_seek,
                            dummy_close, dummy_size, dummy_map, dummy_unmap,
                            opts);
    if (tif != NULL) {
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        (void)seek_proc;

        thandle_t client_data = (thandle_t)(Data + 2);
        thandle_t old_data = TIFFSetClientdata(tif, client_data);
        (void)old_data;

        TIFFClose(tif);
        tif = NULL;
    }

    // 4. Test with read mode if data looks like a TIFF header (optional exploration)
    if (Size >= 4) {
        fd = open(dummy_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            write(fd, Data, Size);
            lseek(fd, 0, SEEK_SET);

            // Test TIFFFdOpenExt in read mode
            tif = TIFFFdOpenExt(fd, dummy_file, "r", opts);
            if (tif != NULL) {
                TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
                (void)write_proc;

                TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
                (void)seek_proc;

                thandle_t client_data = (thandle_t)Data;
                thandle_t old_data = TIFFSetClientdata(tif, client_data);
                (void)old_data;

                TIFFClose(tif);
                tif = NULL;
            }
            close(fd);

            // Test TIFFClientOpen in read mode
            tif = TIFFClientOpen(dummy_file, "r", (thandle_t)Data,
                                 dummy_read, dummy_write, dummy_seek,
                                 dummy_close, dummy_size, dummy_map, dummy_unmap);
            if (tif != NULL) {
                TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
                (void)write_proc;

                TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
                (void)seek_proc;

                thandle_t client_data = (thandle_t)(Data + 1);
                thandle_t old_data = TIFFSetClientdata(tif, client_data);
                (void)old_data;

                TIFFClose(tif);
                tif = NULL;
            }

            // Test TIFFClientOpenExt in read mode
            tif = TIFFClientOpenExt(dummy_file, "r", (thandle_t)Data,
                                    dummy_read, dummy_write, dummy_seek,
                                    dummy_close, dummy_size, dummy_map, dummy_unmap,
                                    opts);
            if (tif != NULL) {
                TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
                (void)write_proc;

                TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
                (void)seek_proc;

                thandle_t client_data = (thandle_t)(Data + 2);
                thandle_t old_data = TIFFSetClientdata(tif, client_data);
                (void)old_data;

                TIFFClose(tif);
                tif = NULL;
            }
        }
        unlink(dummy_file);
    }

    return 0;
}