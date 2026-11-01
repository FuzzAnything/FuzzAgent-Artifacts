// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF **tif) {
    if (tif != NULL && *tif != NULL) {
        TIFFClose(*tif);
        *tif = NULL;
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
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

    // Test TIFFFdOpen with the file descriptor
    tif = TIFFFdOpen(fd, filename, "r");
    
    if (tif != NULL) {
        // Test TIFFIsBigEndian
        int is_big_endian = TIFFIsBigEndian(tif);
        (void)is_big_endian;

        // Test TIFFCurrentDirectory
        tdir_t cur_dir = TIFFCurrentDirectory(tif);
        (void)cur_dir;

        // Test TIFFFileno
        int fileno = TIFFFileno(tif);
        if (fileno >= 0) {
            // Test TIFFSetFileno: try to set it back to itself or a valid one
            int old_fd = TIFFSetFileno(tif, fileno);
            (void)old_fd;
        }

        // Test TIFFRewriteDirectory
        // This might fail if the directory is not properly set up, which is expected in fuzzing
        int rewrite_result = TIFFRewriteDirectory(tif);
        (void)rewrite_result;

        cleanup_tiff(&tif);
    }

    // Close the file descriptor if TIFFFdOpen didn't take ownership (it usually does, but safe to check)
    // Note: TIFFFdOpen typically takes ownership of the fd, so we shouldn't close it again if TIFFClose was called.
    // However, if TIFFFdOpen failed, we still own the fd.
    // Since we called cleanup_tiff which calls TIFFClose, the fd is closed there if tif was valid.
    // If tif was NULL, we need to close fd.
    if (tif == NULL) {
        close(fd);
    }

    // Clean up the temporary file
    unlink(filename);

    return 0;
}