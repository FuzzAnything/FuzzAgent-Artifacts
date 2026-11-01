// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        } else {
            // Write minimal valid-ish TIFF header if input is empty to avoid immediate crash on open
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, header, sizeof(header));
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Prepare environment
    setup_dummy_file(Data, Size);

    // Open file descriptor for the dummy file
    int fd = open("./dummy_file", O_RDONLY);
    if (fd < 0) {
        return 0;
    }

    // Allocate options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        close(fd);
        return 0;
    }

    // Step 2: Configure options based on fuzz input to explore states
    // Use data to set memory limits (ensure non-negative logic by casting)
    if (Size >= 4) {
        uint32_t val1 = *(const uint32_t *)(Data);
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, (tmsize_t)val1);
    }
    
    if (Size >= 8) {
        uint32_t val2 = *(const uint32_t *)(Data + 4);
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, (tmsize_t)val2);
    }

    // Use data to toggle warning flag
    if (Size >= 12) {
        int warn_flag = (Data[11] % 2);
        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_flag);
    }

    // Step 3: Invoke target function
    TIFF *tif = TIFFFdOpenExt(fd, "./dummy_file", "r", opts);

    // Cleanup logic
    if (tif != NULL) {
        TIFFClose(tif);
    }

    // Free options
    TIFFOpenOptionsFree(opts);
    
    // Close file descriptor
    close(fd);

    return 0;
}