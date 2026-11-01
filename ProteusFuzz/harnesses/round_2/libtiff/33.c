// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        } else {
            // Write a minimal valid TIFF header if input is empty
            // Little endian, version 42, first IFD offset 8
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, header, sizeof(header));
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    write_dummy_file(Data, Size);

    // Allocate options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // Fuzz options configuration
    // Use data to determine boolean flags and limits
    int warn_flag = (Data[0] % 2);
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_flag);

    // Set memory limit based on input data, ensuring it's not too small to crash immediately
    // but large enough to trigger logic.
    tmsize_t mem_limit = 0;
    if (Size >= 4) {
        mem_limit = *(const uint32_t *)(Data + Size - 4);
        // Cap at a reasonable limit for fuzzing to avoid OOM
        if (mem_limit > 1024 * 1024 * 100) { 
            mem_limit = 1024 * 1024 * 100; 
        }
    }
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, mem_limit);

    // Test TIFFOpenExt
    TIFF *tif1 = NULL;
    // Try reading
    tif1 = TIFFOpenExt("./dummy_file", "r", opts);
    if (tif1 != NULL) {
        TIFFClose(tif1);
    }

    // Try writing (might fail if file exists and is not empty/valid for overwrite, which is fine for fuzzing)
    TIFF *tif2 = NULL;
    tif2 = TIFFOpenExt("./dummy_file", "w", opts);
    if (tif2 != NULL) {
        TIFFClose(tif2);
    }

    // Test TIFFFdOpenExt
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        TIFF *tif3 = TIFFFdOpenExt(fd, "./dummy_file", "r", opts);
        if (tif3 != NULL) {
            TIFFClose(tif3);
        }
        close(fd);
    }

    // Try opening with write mode via FD
    fd = open("./dummy_file", O_RDWR);
    if (fd >= 0) {
        TIFF *tif4 = TIFFFdOpenExt(fd, "./dummy_file", "w", opts);
        if (tif4 != NULL) {
            TIFFClose(tif4);
        }
        close(fd);
    }

    // Cleanup
    TIFFOpenOptionsFree(opts);
    
    return 0;
}