// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <tiffio.h>

static void dummy_error_handler(const char* module, const char* fmt) {
    (void)module;
    (void)fmt;
}

static void dummy_warn_handler(const char* module, const char* fmt) {
    (void)module;
    (void)fmt;
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    int fd = -1;
    TIFF *tif = NULL;

    // Write fuzzing data to a temporary file
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    if (Size > 0) {
        write(fd, Data, Size);
    }

    // Fuzz TIFFFdOpen (wrapper around TIFFFdOpenExt with no opts)
    // We use TIFFFdOpen directly to avoid the incomplete type issue with TIFFOpenOptions
    lseek(fd, 0, SEEK_SET);
    tif = TIFFFdOpen(fd, filename, "r+");
    if (tif) {
        // Fuzz TIFFCurrentDirectory
        (void)TIFFCurrentDirectory(tif);

        // Fuzz TIFFFileName
        const char *name = TIFFFileName(tif);
        (void)name;

        // Fuzz TIFFSetFileName
        const char *old_name = TIFFSetFileName(tif, "new_name");
        (void)old_name;
        TIFFSetFileName(tif, NULL);

        // Fuzz TIFFSetFileno
        int new_fd = open(filename, O_RDWR);
        if (new_fd >= 0) {
            int old_fd = TIFFSetFileno(tif, new_fd);
            (void)old_fd;
            close(new_fd);
        }

        TIFFClose(tif);
        tif = NULL;
    }

    // Attempt to open again with different mode if possible, or just close
    // Since we already closed, we try to re-open for write to test state changes
    lseek(fd, 0, SEEK_SET);
    tif = TIFFFdOpen(fd, filename, "w");
    if (tif) {
        (void)TIFFCurrentDirectory(tif);
        (void)TIFFFileName(tif);
        TIFFClose(tif);
        tif = NULL;
    }

    close(fd);
    unlink(filename);

    return 0;
}