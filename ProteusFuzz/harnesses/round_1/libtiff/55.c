// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    int fd = -1;
    TIFF *tif = NULL;
    TIFFOpenOptions *opts = NULL;
    int old_fd;
    const char *old_name;
    tdir_t dir_idx;
    const char *file_name;

    // Write fuzzing data to the dummy file
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        return 0;
    }
    lseek(fd, 0, SEEK_SET);

    // Fuzz TIFFFdOpenExt
    // We pass NULL for opts to avoid incomplete type issues and rely on defaults
    tif = TIFFFdOpenExt(fd, filename, "r+", NULL);
    if (tif != NULL) {
        // Fuzz TIFFSetFileName
        old_name = TIFFSetFileName(tif, (const char *)Data);
        (void)old_name;

        // Fuzz TIFFSetFileno (reuse the same fd for safety in this context)
        old_fd = TIFFSetFileno(tif, fd);
        (void)old_fd;

        // Fuzz TIFFCurrentDirectory
        dir_idx = TIFFCurrentDirectory(tif);
        (void)dir_idx;

        // Fuzz TIFFFileName
        file_name = TIFFFileName(tif);
        (void)file_name;

        cleanup_tiff(tif);
    }

    // Fuzz TIFFFdOpen (separate path)
    lseek(fd, 0, SEEK_SET);
    tif = TIFFFdOpen(fd, filename, "r+");
    if (tif != NULL) {
        // Fuzz TIFFSetFileName
        old_name = TIFFSetFileName(tif, (const char *)Data);
        (void)old_name;

        // Fuzz TIFFSetFileno
        old_fd = TIFFSetFileno(tif, fd);
        (void)old_fd;

        // Fuzz TIFFCurrentDirectory
        dir_idx = TIFFCurrentDirectory(tif);
        (void)dir_idx;

        // Fuzz TIFFFileName
        file_name = TIFFFileName(tif);
        (void)file_name;

        cleanup_tiff(tif);
    }

    close(fd);
    unlink(filename);

    return 0;
}