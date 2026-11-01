// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to hold the fuzzed data
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write the fuzzed data to the file
    ssize_t written = write(fd, Data, Size);
    if (written < 0) {
        close(fd);
        return 0;
    }
    lseek(fd, 0, SEEK_SET);

    TIFF *tif = NULL;
    // TIFFOpenOptions is an opaque type in tiffio.h, so we cannot instantiate it directly.
    // We pass NULL to use default options.
    TIFFOpenOptions *opts = NULL;

    // Fuzzing logic
    int mode_choice = Data[0] % 3; // 0: read, 1: write, 2: append (if supported)
    const char *mode = NULL;
    if (mode_choice == 0) {
        mode = "r";
    } else if (mode_choice == 1) {
        mode = "w";
    } else {
        mode = "a";
    }

    // Test TIFFFdOpenExt
    tif = TIFFFdOpenExt(fd, filename, mode, opts);
    if (tif != NULL) {
        // Test TIFFCurrentDirectory
        tdir_t dir = TIFFCurrentDirectory(tif);
        (void)dir; // Use the value to avoid unused variable warning

        // Test TIFFFileName
        const char *fname = TIFFFileName(tif);
        (void)fname; // Use the value to avoid unused variable warning

        // Test TIFFSetFileName
        const char *new_name = "new_dummy_file";
        const char *old_name = TIFFSetFileName(tif, new_name);
        (void)old_name; // Use the value to avoid unused variable warning

        // Test TIFFSetFileno
        int new_fd = open(filename, O_RDWR);
        if (new_fd >= 0) {
            int old_fd = TIFFSetFileno(tif, new_fd);
            (void)old_fd; // Use the value to avoid unused variable warning
            close(new_fd);
        }

        // Close the TIFF handle
        cleanup_tiff(tif);
    }

    // Test TIFFFdOpen
    lseek(fd, 0, SEEK_SET);
    tif = TIFFFdOpen(fd, filename, mode);
    if (tif != NULL) {
        // Perform similar operations as above
        tdir_t dir = TIFFCurrentDirectory(tif);
        (void)dir;

        const char *fname = TIFFFileName(tif);
        (void)fname;

        const char *old_name = TIFFSetFileName(tif, "another_dummy_file");
        (void)old_name;

        int new_fd = open(filename, O_RDWR);
        if (new_fd >= 0) {
            int old_fd = TIFFSetFileno(tif, new_fd);
            (void)old_fd;
            close(new_fd);
        }

        cleanup_tiff(tif);
    }

    // Clean up the file descriptor and temporary file
    close(fd);
    unlink(filename);

    return 0;
}