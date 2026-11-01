// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
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
#include <sys/stat.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        } else {
            // Write minimal valid TIFF header if input is empty
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, header, 8);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open file for reading and writing to test directory manipulation
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (tif == NULL) {
        // If read+write fails, try creating a new file
        TIFFClose(tif);
        tif = TIFFOpen("./dummy_file", "w");
        if (tif == NULL) {
            return 0;
        }
    }

    // Fuzz TIFFCreateDirectory
    TIFFCreateDirectory(tif);

    // Fuzz TIFFSetDirectory with various indices
    tdir_t dir_index = (Size > 0) ? (tdir_t)Data[0] : 0;
    TIFFSetDirectory(tif, dir_index);
    
    // Try setting directory 0 specifically
    TIFFSetDirectory(tif, 0);

    // Fuzz TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // Fuzz TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // Fuzz TIFFFreeDirectory
    TIFFFreeDirectory(tif);

    // Cleanup
    if (tif != NULL) {
        TIFFClose(tif);
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}