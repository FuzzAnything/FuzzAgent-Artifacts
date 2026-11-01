// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

static int create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    
    // Write at least a minimal TIFF header if data is too small
    // Minimal TIFF header: 2 bytes magic, 2 bytes offset, 4 bytes reserved
    if (size == 0) {
        uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
        write(fd, header, sizeof(header));
    } else {
        write(fd, data, size);
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    int fd = -1;
    TIFF *tif = NULL;
    int old_fd = -1;
    tdir_t dir_idx;
    const char *fname;

    // Create a dummy file with the fuzzing input
    if (create_dummy_file(filename, Data, Size) != 0) {
        return 0;
    }

    // Open file descriptor for testing TIFFFdOpen and TIFFFdOpenExt
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        return 0;
    }

    // Test 1: TIFFFdOpen
    tif = TIFFFdOpen(fd, filename, "r+");
    if (tif != NULL) {
        // Test TIFFCurrentDirectory
        dir_idx = TIFFCurrentDirectory(tif);
        (void)dir_idx; // Suppress unused warning

        // Test TIFFFileName
        fname = TIFFFileName(tif);
        (void)fname; // Suppress unused warning

        // Test TIFFSetFileName
        const char *new_name = "renamed_file.tiff";
        const char *old_name = TIFFSetFileName(tif, new_name);
        (void)old_name; // Suppress unused warning

        // Test TIFFSetFileno: open another dummy file to swap
        int fd2 = open(filename, O_RDWR);
        if (fd2 >= 0) {
            old_fd = TIFFSetFileno(tif, fd2);
            (void)old_fd; // Suppress unused warning
            close(fd2);
        }

        cleanup_tiff(tif);
        tif = NULL;
    }

    // Test 2: TIFFFdOpenExt with custom options
    if (fd >= 0) {
        // Seek back to beginning for re-opening
        lseek(fd, 0, SEEK_SET);
        
        // Use NULL for options to avoid incomplete type issues
        // TIFFFdOpenExt accepts NULL for opts, falling back to defaults
        tif = TIFFFdOpenExt(fd, filename, "r+", NULL);
        if (tif != NULL) {
            // Test TIFFCurrentDirectory
            dir_idx = TIFFCurrentDirectory(tif);
            (void)dir_idx;

            // Test TIFFFileName
            fname = TIFFFileName(tif);
            (void)fname;

            // Test TIFFSetFileName with NULL
            fname = TIFFSetFileName(tif, NULL);
            (void)fname;

            // Test TIFFSetFileno again
            int fd3 = open(filename, O_RDWR);
            if (fd3 >= 0) {
                old_fd = TIFFSetFileno(tif, fd3);
                (void)old_fd;
                close(fd3);
            }

            cleanup_tiff(tif);
            tif = NULL;
        }
    }

    // Clean up file descriptor
    if (fd >= 0) {
        close(fd);
    }

    // Clean up dummy file
    unlink(filename);

    return 0;
}