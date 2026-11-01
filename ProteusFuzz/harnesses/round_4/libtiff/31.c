// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFGetClientInfo at tif_extension.c:64:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to act as the TIFF source
    const char *filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }

    // Write input data to the file
    ssize_t written = write(fd, Data, Size);
    if (written < 0) {
        close(fd);
        return 0;
    }

    // Prepare options for TIFFFdOpenExt
    // Since TIFFOpenOptions is an opaque type in older libtiff versions or forward declared,
    // we cannot instantiate it directly. We pass NULL to use default options.
    TIFFOpenOptions *opts = NULL;

    // Duplicate file descriptor for TIFFFdOpenExt
    int fd_for_open = dup(fd);
    if (fd_for_open < 0) {
        close(fd);
        return 0;
    }

    // Open TIFF using the file descriptor
    TIFF *tif = TIFFFdOpenExt(fd_for_open, filename, "r", opts);
    close(fd_for_open); // Close our copy, libtiff manages the handle internally

    if (tif != NULL) {
        // 1. Test TIFFSetMode
        // Try different modes based on fuzzing data
        int mode = 0;
        if (Size > 3) {
            mode = Data[3] & 0x03; // O_RDONLY, O_WRONLY, O_RDWR
        }
        TIFFSetMode(tif, mode);

        // 2. Test TIFFSetFileName
        // Create a dummy name from input data if available
        char dummy_name[256];
        size_t name_len = Size > 255 ? 255 : Size;
        if (name_len > 0) {
            memcpy(dummy_name, Data, name_len);
            dummy_name[name_len] = '\0';
            TIFFSetFileName(tif, dummy_name);
        }
        // Also test with NULL
        TIFFSetFileName(tif, NULL);

        // 3. Test TIFFSetClientdata
        void *client_data = malloc(Size > 0 ? 1 : 0);
        void *prev_data = TIFFSetClientdata(tif, client_data);
        (void)prev_data; // Suppress unused variable warning
        // Test setting NULL
        TIFFSetClientdata(tif, NULL);

        // 4. Test TIFFGetClientInfo
        // We haven't set any client info via TIFFSetClientInfo, so this should return NULL
        // but we test the call path.
        const char *key = "fuzz_key";
        if (Size > 4) {
            // Use input data to form a key
            char key_buf[64];
            size_t k_len = Size > 64 ? 64 : Size;
            memcpy(key_buf, Data, k_len);
            key_buf[k_len] = '\0';
            key = key_buf;
        }
        void *info = TIFFGetClientInfo(tif, key);
        (void)info; // Suppress unused variable warning

        // Close the TIFF handle first
        TIFFClose(tif);
        
        // 5. Test TIFFCleanup
        // Call it after TIFFClose as per documentation
        TIFFCleanup(tif);
    }

    // Cleanup file descriptor and file
    close(fd);
    unlink(filename);

    return 0;
}