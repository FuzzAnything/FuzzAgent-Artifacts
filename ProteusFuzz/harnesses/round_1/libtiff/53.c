// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <fcntl.h>
#include <unistd.h>

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to use with file descriptors
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write some data to the file to ensure it's not empty
    if (Size > 0) {
        write(fd, Data, Size);
    }
    lseek(fd, 0, SEEK_SET);

    TIFF *tif = NULL;
    TIFFOpenOptions *opts = NULL;

    // Fuzz TIFFFdOpenExt with different modes
    // Mode "r" for read, "w" for write
    const char *mode = (Data[0] & 0x01) ? "w" : "r";
    
    // Try opening with options (passing NULL for opts to avoid incomplete type issues)
    tif = TIFFFdOpenExt(fd, filename, mode, opts);
    
    if (tif != NULL) {
        // Fuzz TIFFSetMode
        int prev_mode = TIFFSetMode(tif, (Data[1] & 0x01) ? O_RDONLY : O_RDWR);
        (void)prev_mode; // Suppress unused warning

        // Fuzz TIFFGetMode
        int current_mode = TIFFGetMode(tif);
        (void)current_mode;

        // Fuzz TIFFFileno
        int fd_retrieved = TIFFFileno(tif);
        if (fd_retrieved >= 0) {
            // Try to seek to ensure FD is valid
            lseek(fd_retrieved, 0, SEEK_SET);
        }

        // Fuzz TIFFSetFileno
        // Create another FD to swap with
        int fd2 = open(filename, O_RDWR);
        if (fd2 >= 0) {
            int old_fd = TIFFSetFileno(tif, fd2);
            if (old_fd >= 0) {
                close(old_fd);
            }
            close(fd2);
        }

        // Fuzz TIFFFlush
        int flush_result = TIFFFlush(tif);
        (void)flush_result;

        // Cleanup TIFF handle
        TIFFClose(tif);
    } else {
        // If opening failed, we still own the file descriptor
        close(fd);
    }

    // Cleanup temporary file
    unlink(filename);

    return 0;
}