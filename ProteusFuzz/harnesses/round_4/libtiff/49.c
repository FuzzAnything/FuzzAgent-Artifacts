// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "tiffio.h"

/*
 * Helper function to create a dummy file with fuzzed data
 */
static int create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    if (size > 0) {
        ssize_t written = write(fd, data, size);
        if (written != (ssize_t)size) {
            close(fd);
            unlink("./dummy_file");
            return -1;
        }
    }
    lseek(fd, 0, SEEK_SET);
    return fd;
}

/*
 * Dummy I/O callbacks for TIFFClientOpen matching libtiff signatures
 */
static tmsize_t dummy_read(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd; (void)buf; (void)size;
    return 0;
}

static tmsize_t dummy_write(thandle_t fd, void *buf, tmsize_t size) {
    (void)fd; (void)buf; (void)size;
    return size;
}

static toff_t dummy_seek(thandle_t fd, toff_t off, int whence) {
    (void)fd; (void)off; (void)whence;
    return 0;
}

static int dummy_close(thandle_t fd) {
    (void)fd;
    return 0;
}

/* TIFFSizeProc expects unsigned long return type */
static unsigned long dummy_size(thandle_t fd) {
    (void)fd;
    return 0;
}

/* TIFFMapFileProc expects int return type and void** for address, unsigned long* for size */
static int dummy_mmap(thandle_t fd, void **maddr, unsigned long *size) {
    (void)fd; (void)maddr; (void)size;
    return 0;
}

/* TIFFUnmapFileProc expects void return type, void* for address, unsigned long for size */
static void dummy_munmap(thandle_t fd, void *maddr, unsigned long size) {
    (void)fd; (void)maddr; (void)size;
    return;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    TIFF *tif = NULL;
    TIFFOpenOptions *opts = NULL;

    /* Test 1: TIFFFdOpenExt */
    int fd = create_dummy_file(Data, Size);
    if (fd >= 0) {
        tif = TIFFFdOpenExt(fd, "test_fd.tiff", "w+", opts);
        if (tif) {
            /* Test TIFFGetWriteProc */
            TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
            (void)write_proc;

            /* Test TIFFGetSeekProc */
            TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
            (void)seek_proc;

            /* Test TIFFSetClientdata */
            thandle_t old_data = TIFFSetClientdata(tif, (thandle_t)Data);
            (void)old_data;

            TIFFClose(tif);
        }
        close(fd);
        unlink("./dummy_file");
    }

    /* Test 2: TIFFClientOpen */
    tif = TIFFClientOpen("test_client.tiff", "w+", 
                         (thandle_t)Data,
                         dummy_read, dummy_write,
                         dummy_seek, dummy_close,
                         dummy_size, dummy_mmap, dummy_munmap);
    if (tif) {
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        (void)seek_proc;

        thandle_t old_data = TIFFSetClientdata(tif, (thandle_t)0x12345678);
        (void)old_data;

        TIFFClose(tif);
    }

    /* Test 3: TIFFClientOpenExt */
    tif = TIFFClientOpenExt("test_client_ext.tiff", "w+", 
                            (thandle_t)Data,
                            dummy_read, dummy_write,
                            dummy_seek, dummy_close,
                            dummy_size, dummy_mmap, dummy_munmap,
                            opts);
    if (tif) {
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        (void)write_proc;

        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        (void)seek_proc;

        thandle_t old_data = TIFFSetClientdata(tif, (thandle_t)0x87654321);
        (void)old_data;

        TIFFClose(tif);
    }

    return 0;
}