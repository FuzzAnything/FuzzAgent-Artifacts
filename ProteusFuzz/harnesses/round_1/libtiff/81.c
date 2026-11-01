// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadGPSDirectory at tif_dirread.c:5470:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
#include "tiffio.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        } else {
            // Write a minimal valid TIFF header if input is empty
            uint8_t minimal_header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, minimal_header, 8);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: create a dummy file with fuzz input
    write_dummy_file(Data, Size);

    // Open the file in read-only mode
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Extract values from input for diversification
    uint64_t diroff = 0;
    if (Size >= 8) {
        diroff = *(const uint64_t *)(Data);
    } else if (Size >= 4) {
        diroff = *(const uint32_t *)(Data);
    } else if (Size > 0) {
        diroff = Data[0];
    }

    tdir_t dir_index = 0;
    if (Size >= 12) {
        dir_index = *(const tdir_t *)(Data + 8);
    } else if (Size > 8) {
        dir_index = Data[8];
    }

    // 1. TIFFReadDirectory: Parse the next IFD
    (void)TIFFReadDirectory(tif);

    // 2. TIFFSetSubDirectory: Manually set current IFD offset
    (void)TIFFSetSubDirectory(tif, diroff);

    // 3. TIFFReadEXIFDirectory: Read EXIF tags
    (void)TIFFReadEXIFDirectory(tif, diroff);

    // 4. TIFFReadGPSDirectory: Read GPS tags
    (void)TIFFReadGPSDirectory(tif, diroff);

    // 5. TIFFSetDirectory: Select specific directory
    (void)TIFFSetDirectory(tif, dir_index);

    // 6. TIFFReadCustomDirectory: Read custom tags
    // We pass NULL for infoarray to test error handling or default behavior
    // Note: Passing NULL might cause a crash in some versions, which is valid for fuzzing
    (void)TIFFReadCustomDirectory(tif, diroff, NULL);

    // Cleanup
    TIFFClose(tif);

    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}