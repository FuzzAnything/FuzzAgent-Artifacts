// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "tiffio.h"

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        } else {
            // Write some minimal valid TIFF header if input is empty to avoid immediate parse errors
            // Minimal TIFF header: 4 bytes (II or MM), 2 bytes (magic 42), 2 bytes (offset)
            uint8_t header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, header, sizeof(header));
        }
        close(fd);
    }
}

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    int fd = open("./dummy_file", O_RDWR);
    if (fd < 0) {
        return 0;
    }

    // TIFFOpenOptions is an opaque type in some versions or requires specific initialization.
    // We pass NULL to use defaults, or rely on the library's internal handling if the struct is opaque.
    // Since the struct definition might be hidden or incomplete in the header, we cannot initialize it directly.
    // Passing NULL is a valid way to invoke TIFFFdOpenExt with default options.
    TIFF *tif = TIFFFdOpenExt(fd, "./dummy_file", "r+", NULL);
    
    if (tif == NULL) {
        close(fd);
        return 0;
    }

    // Fuzz TIFFSetMode
    if (Size > 1) {
        int mode_flag = Data[1] & 0x03; // Limit to reasonable values (0, 1, 2, 3)
        // O_RDONLY=0, O_WRONLY=1, O_RDWR=2, O_APPEND=4
        // Map fuzzed value to valid modes roughly
        int mapped_mode = 0;
        switch (mode_flag) {
            case 0: mapped_mode = O_RDONLY; break;
            case 1: mapped_mode = O_WRONLY; break;
            case 2: mapped_mode = O_RDWR; break;
            case 3: mapped_mode = O_RDWR | O_APPEND; break;
        }
        TIFFSetMode(tif, mapped_mode);
    }

    // Fuzz TIFFGetMode
    if (Size > 2) {
        (void)TIFFGetMode(tif);
    }

    // Fuzz TIFFFileno
    if (Size > 3) {
        int fd_check = TIFFFileno(tif);
        if (fd_check >= 0) {
            // Optional: do something with fd_check if needed, but just calling is enough for coverage
        }
    }

    // Fuzz TIFFSetFileno
    if (Size > 4) {
        // Create a new dummy file to switch to
        int new_fd = open("./dummy_file", O_RDWR);
        if (new_fd >= 0) {
            int old_fd = TIFFSetFileno(tif, new_fd);
            if (old_fd >= 0 && old_fd != new_fd) {
                close(old_fd);
            }
            // Note: We don't close new_fd here immediately as it's now owned by tif
            // But we must ensure we don't leak if we switch again or close tif
        }
    }

    // Fuzz TIFFFlush
    if (Size > 5) {
        (void)TIFFFlush(tif);
    }

    cleanup_tiff(tif);
    
    // Clean up dummy file
    unlink("./dummy_file");

    return 0;
}