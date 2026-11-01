// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
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
#include "tiffio.h"

static int write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    
    // Write a minimal TIFF header (Classic TIFF, Little Endian)
    // 2 bytes: II (Little Endian)
    // 2 bytes: 42 (Magic number)
    // 4 bytes: Offset to first IFD (usually 8)
    uint8_t header[8];
    header[0] = 'I';
    header[1] = 'I';
    header[2] = 42;
    header[3] = 0;
    header[4] = 8; // Offset to first IFD
    header[5] = 0;
    header[6] = 0;
    header[7] = 0;
    
    ssize_t written = write(fd, header, sizeof(header));
    if (written != sizeof(header)) {
        close(fd);
        return -1;
    }
    
    // Write a minimal IFD (0 entries, next IFD offset 0)
    uint8_t ifd[8];
    // 2 bytes: Number of entries (0)
    ifd[0] = 0;
    ifd[1] = 0;
    // 4 bytes: Next IFD offset (0)
    ifd[2] = 0;
    ifd[3] = 0;
    ifd[4] = 0;
    ifd[5] = 0;
    // Padding to align to 4 bytes if needed (already aligned)
    ifd[6] = 0;
    ifd[7] = 0;
    
    written = write(fd, ifd, sizeof(ifd));
    if (written != sizeof(ifd)) {
        close(fd);
        return -1;
    }
    
    // Append fuzzing data to the file
    if (Size > 0) {
        written = write(fd, Data, Size);
        if (written < 0) {
            close(fd);
            return -1;
        }
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment: create a dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Open the file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        unlink("./dummy_file");
        return 0;
    }

    // 1. TIFFCurrentDirectory
    tdir_t dir1 = TIFFCurrentDirectory(tif);
    (void)dir1;

    // 2. TIFFSetDirectory (try index 0)
    int ret1 = TIFFSetDirectory(tif, 0);
    (void)ret1;

    // 3. TIFFSetSubDirectory (try offset 0 or a value derived from input)
    uint64_t sub_offset = 0;
    if (Size >= 8) {
        sub_offset = (uint64_t)Data[0] | ((uint64_t)Data[1] << 8) | 
                     ((uint64_t)Data[2] << 16) | ((uint64_t)Data[3] << 24) |
                     ((uint64_t)Data[4] << 32) | ((uint64_t)Data[5] << 40) |
                     ((uint64_t)Data[6] << 48) | ((uint64_t)Data[7] << 56);
    }
    int ret2 = TIFFSetSubDirectory(tif, sub_offset);
    (void)ret2;

    // 4. TIFFReadDirectory
    int ret3 = TIFFReadDirectory(tif);
    (void)ret3;

    // 5. TIFFSetDirectory (try index 1)
    int ret4 = TIFFSetDirectory(tif, 1);
    (void)ret4;

    // 6. TIFFReadDirectory
    int ret5 = TIFFReadDirectory(tif);
    (void)ret5;

    // 7. TIFFCurrentDirectory
    tdir_t dir2 = TIFFCurrentDirectory(tif);
    (void)dir2;

    // 8. TIFFSetSubDirectory (try another offset)
    uint64_t sub_offset2 = 0;
    if (Size >= 8) {
        sub_offset2 = (uint64_t)Data[Size-1] | ((uint64_t)Data[Size-2] << 8) | 
                      ((uint64_t)Data[Size-3] << 16) | ((uint64_t)Data[Size-4] << 24) |
                      ((uint64_t)Data[Size-5] << 32) | ((uint64_t)Data[Size-6] << 40) |
                      ((uint64_t)Data[Size-7] << 48) | ((uint64_t)Data[Size-8] << 56);
    }
    int ret6 = TIFFSetSubDirectory(tif, sub_offset2);
    (void)ret6;

    // 9. TIFFNumberOfDirectories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    (void)num_dirs;

    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");

    return 0;
}