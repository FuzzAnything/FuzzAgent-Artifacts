// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
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

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }
    
    // Write a minimal valid TIFF header (BigTIFF or Classic)
    // Classic TIFF header: 8 bytes
    // Magic: 0x002A (little endian) or 0x2A00 (big endian)
    // Offset to first IFD: 8 (little endian)
    
    uint8_t header[8];
    header[0] = 0x2A; // Magic (little endian)
    header[1] = 0x00;
    header[2] = 0x08; // Offset to first IFD
    header[3] = 0x00;
    header[4] = 0x00;
    header[5] = 0x00;
    header[6] = 0x00;
    header[7] = 0x00;
    
    write(fd, header, 8);
    
    // Write a minimal IFD (2 bytes count + 12 bytes per entry + 4 bytes next IFD)
    // Let's write 0 entries and next IFD offset 0
    uint16_t entry_count = 0;
    uint32_t next_ifd = 0;
    
    write(fd, &entry_count, 2);
    write(fd, &next_ifd, 4);
    
    // Write the fuzzing data as raw image data (if any)
    if (size > 0) {
        write(fd, data, size);
    }
    
    close(fd);
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Write dummy file with fuzzing data
    write_dummy_tiff(Data, Size);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }
    
    // Read directory to initialize TIFF structure
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }
    
    // Get image dimensions for fuzzing parameters
    uint32_t width, height, depth;
    uint16_t samplesperpixel;
    
    if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) ||
        !TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) ||
        !TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel)) {
        TIFFClose(tif);
        return 0;
    }
    
    // Default depth to 1 if not available
    depth = 1;
    if (!TIFFGetField(tif, TIFFTAG_IMAGEDEPTH, &depth)) {
        depth = 1;
    }
    
    // Fuzz TIFFComputeTile
    uint32_t x = (Size > 0) ? Data[0] : 0;
    uint32_t y = (Size > 1) ? Data[1] : 0;
    uint32_t z = (Size > 2) ? Data[2] : 0;
    uint16_t s = (Size > 3) ? Data[3] : 0;
    
    // Limit values to reasonable ranges to avoid overflow issues
    x = x % (width > 0 ? width + 10 : 10);
    y = y % (height > 0 ? height + 10 : 10);
    z = z % (depth > 0 ? depth + 10 : 10);
    s = s % (samplesperpixel > 0 ? samplesperpixel + 10 : 10);
    
    TIFFComputeTile(tif, x, y, z, s);
    
    // Fuzz TIFFCheckTile
    TIFFCheckTile(tif, x, y, z, s);
    
    // Fuzz TIFFCurrentTile
    TIFFCurrentTile(tif);
    
    // Fuzz TIFFComputeStrip
    uint32_t row = (Size > 4) ? Data[4] : 0;
    row = row % (height > 0 ? height + 10 : 10);
    TIFFComputeStrip(tif, row, s);
    
    // Fuzz TIFFCurrentRow
    TIFFCurrentRow(tif);
    
    // Fuzz TIFFNumberOfTiles
    TIFFNumberOfTiles(tif);
    
    // Try with different parameter combinations
    for (uint32_t i = 0; i < 10 && Size > 10; i++) {
        uint32_t x2 = (Data[5 + i] % (width > 0 ? width + 1 : 1));
        uint32_t y2 = (Data[6 + i] % (height > 0 ? height + 1 : 1));
        uint32_t z2 = (Data[7 + i] % (depth > 0 ? depth + 1 : 1));
        uint16_t s2 = (Data[8 + i] % (samplesperpixel > 0 ? samplesperpixel + 1 : 1));
        
        TIFFComputeTile(tif, x2, y2, z2, s2);
        TIFFCheckTile(tif, x2, y2, z2, s2);
        
        uint32_t row2 = (Data[9 + i] % (height > 0 ? height + 1 : 1));
        TIFFComputeStrip(tif, row2, s2);
    }
    
    // Cleanup
    TIFFClose(tif);
    
    return 0;
}