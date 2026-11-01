// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tiffio.h"

static void cleanup_file(const char *filename) {
    if (filename && filename[0] != '\0') {
        unlink(filename);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    cleanup_file(filename);

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write input data to the file
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        cleanup_file(filename);
        return 0;
    }
    lseek(fd, 0, SEEK_SET);

    TIFF *tif = NULL;
    int old_fd = -1;
    int old_mode = -1;
    int current_mode = -1;
    int flush_result = -1;

    // Fuzz TIFFFdOpenExt
    // Try different modes: O_RDONLY, O_RDWR, O_WRONLY
    int mode_choice = Data[0] % 3;
    const char *mode_str = NULL;
    switch (mode_choice) {
        case 0: mode_str = "r"; break;
        case 1: mode_str = "w"; break;
        case 2: mode_str = "a"; break;
    }

    tif = TIFFFdOpenExt(fd, filename, mode_str, NULL);
    if (tif) {
        // Fuzz TIFFFileno
        old_fd = TIFFFileno(tif);

        // Fuzz TIFFGetMode
        current_mode = TIFFGetMode(tif);

        // Fuzz TIFFSetMode
        // Check if we have enough data to access Data[1]
        if (Size > 1) {
            int new_mode_choice = Data[1] % 3;
            int new_mode_flag = 0;
            switch (new_mode_choice) {
                case 0: new_mode_flag = O_RDONLY; break;
                case 1: new_mode_flag = O_RDWR; break;
                case 2: new_mode_flag = O_WRONLY; break;
            }
            old_mode = TIFFSetMode(tif, new_mode_flag);
        }

        // Fuzz TIFFFlush
        flush_result = TIFFFlush(tif);

        // Fuzz TIFFSetFileno
        // Create a new dummy file descriptor to switch to
        int new_fd = open(filename, O_RDWR);
        if (new_fd >= 0) {
            int prev_fd = TIFFSetFileno(tif, new_fd);
            // Try to flush again after switching FD
            flush_result = TIFFFlush(tif);
            close(new_fd);
            // Note: prev_fd is the old fd, which we might have already closed or is still valid
            // libtiff expects the new fd to be valid. We closed new_fd immediately, 
            // which might cause issues on next I/O, but that's the point of fuzzing.
            // We should close the original fd if it was returned and we are done with it.
            if (prev_fd >= 0 && prev_fd != fd) {
                close(prev_fd);
            }
        }
    }

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }
    
    if (fd >= 0) {
        close(fd);
    }

    cleanup_file(filename);

    return 0;
}