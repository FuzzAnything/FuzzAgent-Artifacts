// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }
    
    // Write a minimal valid TIFF header (Little Endian)
    // Bytes 0-1: Magic number (II = 0x4949)
    // Bytes 2-3: Version (42 = 0x002A)
    // Bytes 4-7: Offset to first IFD
    uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    
    if (write(fd, header, 8) != 8) {
        close(fd);
        return;
    }

    // Write a minimal IFD at offset 8
    // 1 entry (16 bits)
    uint16_t num_entries = 1;
    if (write(fd, &num_entries, 2) != 2) {
        close(fd);
        return;
    }

    // Entry: Tag (2), Type (2), Count (4), Value/Offset (4)
    // Tag: ImageWidth (256), Type: SHORT (3), Count: 1, Value: 100
    uint16_t tag = 256;
    uint16_t type = 3;
    uint32_t count = 1;
    uint32_t value = 100;
    
    if (write(fd, &tag, 2) != 2 ||
        write(fd, &type, 2) != 2 ||
        write(fd, &count, 4) != 4 ||
        write(fd, &value, 4) != 4) {
        close(fd);
        return;
    }

    // Next IFD offset (0 for end of chain)
    uint32_t next_ifd = 0;
    if (write(fd, &next_ifd, 4) != 4) {
        close(fd);
        return;
    }

    // Write the fuzzing data as raw image data (strips)
    // We need at least one strip. Let's write the fuzz data.
    if (Size > 0) {
        write(fd, Data, Size);
    }

    close(fd);
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a dummy TIFF file
    write_dummy_tiff(Data, Size);

    // Open the file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // 1. TIFFReadDirectory
    // Read the first directory
    int read_dir_result = TIFFReadDirectory(tif);
    (void)read_dir_result; // Ignore result for fuzzing state exploration

    // 2. TIFFCurrentDirectory
    tdir_t current_dir = TIFFCurrentDirectory(tif);
    (void)current_dir;

    // 3. TIFFSetDirectory
    // Try to set directory to 0 (should be valid if we read one)
    int set_dir_result = TIFFSetDirectory(tif, 0);
    (void)set_dir_result;

    // 4. TIFFDefaultStripSize
    // Use a portion of fuzz data to determine request size
    uint32_t request_size = 0;
    if (Size >= 4) {
        request_size = *(const uint32_t *)Data;
    } else if (Size > 0) {
        request_size = Data[0];
    }
    // Ensure request is not zero to avoid potential issues, though API handles it
    if (request_size == 0) request_size = 1;

    uint32_t strip_size = TIFFDefaultStripSize(tif, request_size);
    (void)strip_size;

    // 5. TIFFDefaultTileSize
    uint32_t tile_width = 0;
    uint32_t tile_height = 0;
    TIFFDefaultTileSize(tif, &tile_width, &tile_height);
    (void)tile_width;
    (void)tile_height;

    // 6. TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    // Cleanup
    TIFFClose(tif);
    
    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}