// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <tiffio.h>

// Ensure TIFFOpenOptions is defined if not fully exposed in tiffio.h for the specific build
// This matches the provided type definition to ensure compilation
#ifndef TIFFOpenOptions_defined
typedef struct TIFFOpenOptions {
    TIFFErrorHandlerExtR errorhandler;
    void *errorhandler_user_data;
    TIFFErrorHandlerExtR warnhandler;
    void *warnhandler_user_data;
    tmsize_t max_single_mem_alloc;
    tmsize_t max_cumulated_mem_alloc;
    int warn_about_unknown_tags;
} TIFFOpenOptions;
#define TIFFOpenOptions_defined
#endif

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write the fuzzed input to the file
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        unlink(filename);
        return 0;
    }

    TIFFOpenOptions opts;
    memset(&opts, 0, sizeof(opts));
    
    // Use input data to seed options safely
    opts.max_single_mem_alloc = (Size > 0) ? (tmsize_t)Size : 0;
    opts.max_cumulated_mem_alloc = (Size > 0) ? (tmsize_t)Size : 0;
    opts.warn_about_unknown_tags = (Size % 2);

    TIFF *tif = NULL;

    // Test 1: TIFFFdOpen (read mode)
    lseek(fd, 0, SEEK_SET);
    tif = TIFFFdOpen(fd, filename, "r");
    if (tif) {
        // Test TIFFGetReadProc
        TIFFReadWriteProc readProc = TIFFGetReadProc(tif);
        (void)readProc;

        // Test TIFFFileno
        int fileno = TIFFFileno(tif);
        if (fileno >= 0) {
            (void)fileno;
        }

        // Test TIFFSetWriteOffset
        toff_t offset = (Size > 0) ? (toff_t)(Data[0] & 0xFF) : 0;
        TIFFSetWriteOffset(tif, offset);

        TIFFClose(tif);
        tif = NULL;
    }

    // Test 2: TIFFFdOpenExt (write mode)
    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, 0);
    tif = TIFFFdOpenExt(fd, filename, "w", &opts);
    if (tif) {
        // Test TIFFGetWriteProc
        TIFFReadWriteProc writeProc = TIFFGetWriteProc(tif);
        (void)writeProc;

        // Test TIFFSetWriteOffset
        toff_t offset2 = (Size > 1) ? (toff_t)((Data[0] << 8) | Data[1]) : 0;
        TIFFSetWriteOffset(tif, offset2);

        // Test TIFFFileno
        int fileno2 = TIFFFileno(tif);
        (void)fileno2;

        // Test TIFFGetReadProc on write mode
        TIFFReadWriteProc readProc2 = TIFFGetReadProc(tif);
        (void)readProc2;

        TIFFClose(tif);
        tif = NULL;
    }

    // Test 3: TIFFFdOpenExt with "rw" mode
    lseek(fd, 0, SEEK_SET);
    tif = TIFFFdOpenExt(fd, filename, "rw", &opts);
    if (tif) {
        TIFFReadWriteProc wp = TIFFGetWriteProc(tif);
        (void)wp;
        TIFFReadWriteProc rp = TIFFGetReadProc(tif);
        (void)rp;
        TIFFSetWriteOffset(tif, 1024);
        TIFFClose(tif);
    }

    close(fd);
    unlink(filename);

    return 0;
}