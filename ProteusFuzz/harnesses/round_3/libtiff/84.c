// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "tiffio.h"

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        } else {
            // Write a minimal valid TIFF header if data is empty to ensure open succeeds
            // 0x4949 (II) 0x002A (Magic) 0x00000008 (First IFD offset)
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, header, sizeof(header));
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file to simulate a TIFF file
    write_dummy_file(Data, Size);

    // Attempt to open the file via file descriptor
    int fd = open("./dummy_file", O_RDWR);
    if (fd < 0) {
        return 0;
    }

    TIFF *tif = TIFFFdOpen(fd, "./dummy_file", "r+");
    if (tif == NULL) {
        // Try opening in write mode if read+ failed (common for empty/corrupt files)
        close(fd);
        fd = open("./dummy_file", O_RDWR | O_CREAT, 0644);
        if (fd >= 0) {
            tif = TIFFFdOpen(fd, "./dummy_file", "w");
        }
    }

    if (tif == NULL) {
        close(fd);
        return 0;
    }

    // Fuzz TIFFIsBigEndian
    TIFFIsBigEndian(tif);

    // Fuzz TIFFFileno
    int fileno = TIFFFileno(tif);
    if (fileno >= 0) {
        // Try to seek to a random position if fd is valid
        lseek(fileno, 0, SEEK_SET);
    }

    // Fuzz TIFFCurrentDirectory
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    (void)current_dir; // Suppress unused warning

    // Fuzz TIFFSetFileno
    // We need another valid fd to swap with. Reopen the same file or a temp one.
    int fd2 = open("./dummy_file", O_RDWR);
    if (fd2 >= 0) {
        int old_fd = TIFFSetFileno(tif, fd2);
        if (old_fd >= 0 && old_fd != fd2) {
            close(old_fd);
        }
        // Close the new fd as it's now owned by tif (or was swapped)
        // Note: TIFFSetFileno returns the OLD fd, so fd2 is now in tif.
        // We must not close fd2 here if it's now inside tif, but we don't know for sure
        // if the swap succeeded or if tif took ownership of fd2.
        // Standard behavior: tif now owns fd2. We should not close fd2.
        // However, if we want to be safe, we assume tif owns the new fd.
        // If we need to test swapping back, we would need to keep track.
        // For fuzzing, just leaving it is fine, TIFFClose will handle it.
    } else {
        // If fd2 open failed, do nothing
    }

    // Fuzz TIFFRewriteDirectory
    // This requires a valid directory. If the file was corrupt, this might fail or do nothing.
    TIFFRewriteDirectory(tif);

    // Cleanup
    cleanup_tiff(tif);
    
    // Clean up the dummy file
    unlink("./dummy_file");

    return 0;
}