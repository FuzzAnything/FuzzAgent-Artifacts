// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <tiffio.h>

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return;
    }
    
    // Write a minimal TIFF header (BigTIFF or Classic)
    // Classic TIFF header (8 bytes):
    // Bytes 0-1: Byte order (II = 0x4949 or MM = 0x4D4D)
    // Bytes 2-3: Magic number (42)
    // Bytes 4-7: Offset to first IFD (usually 8)
    
    uint8_t header[8];
    // Use Little Endian
    header[0] = 'I';
    header[1] = 'I';
    header[2] = 42;
    header[3] = 0;
    header[4] = 8; // Offset to first IFD
    header[5] = 0;
    header[6] = 0;
    header[7] = 0;
    
    write(fd, header, 8);
    
    // Write a minimal IFD (Image File Directory)
    // IFD structure:
    // 2 bytes: Number of directory entries
    // N * 12 bytes: Directory entries
    // 4 bytes: Offset to next IFD (0 for last)
    
    uint8_t ifd_header[2];
    ifd_header[0] = 0; // 0 entries for minimal valid file
    ifd_header[1] = 0;
    write(fd, ifd_header, 2);
    
    uint32_t next_ifd = 0;
    write(fd, &next_ifd, 4);
    
    // Write the fuzzing data as raw image data or padding
    if (Size > 0) {
        write(fd, Data, Size);
    }
    
    close(fd);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment
    write_dummy_tiff(Data, Size);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Extract values from input data for testing parameters
    uint32_t strile = 0;
    uint32_t nrows = 0;
    uint32_t strip = 0;
    int pbErr = 0;
    
    if (Size >= 4) {
        strile = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        nrows = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        strip = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }
    
    // Explore diverse program states by invoking target functions
    
    // 1. TIFFGetStrileByteCountWithErr
    int err1 = 0;
    uint64_t byte_count_with_err = TIFFGetStrileByteCountWithErr(tif, strile, &err1);
    
    // 2. TIFFGetStrileOffset
    uint64_t offset_no_err = TIFFGetStrileOffset(tif, strile);
    
    // 3. TIFFVStripSize64
    uint64_t vstrip_size = TIFFVStripSize64(tif, nrows);
    
    // 4. TIFFGetStrileByteCount
    uint64_t byte_count_no_err = TIFFGetStrileByteCount(tif, strile);
    
    // 5. TIFFGetStrileOffsetWithErr
    int err2 = 0;
    uint64_t offset_with_err = TIFFGetStrileOffsetWithErr(tif, strile, &err2);
    
    // 6. TIFFRawStripSize64
    uint64_t raw_strip_size = TIFFRawStripSize64(tif, strip);
    
    // Additional exploration: try with different indices derived from input
    if (Size >= 8) {
        uint32_t strile2 = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
        uint32_t nrows2 = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
        uint32_t strip2 = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
        
        int err3 = 0;
        (void)TIFFGetStrileByteCountWithErr(tif, strile2, &err3);
        (void)TIFFGetStrileOffset(tif, strile2);
        (void)TIFFVStripSize64(tif, nrows2);
        (void)TIFFGetStrileByteCount(tif, strile2);
        int err4 = 0;
        (void)TIFFGetStrileOffsetWithErr(tif, strile2, &err4);
        (void)TIFFRawStripSize64(tif, strip2);
    }
    
    // Try with maximum values
    (void)TIFFVStripSize64(tif, 0xFFFFFFFF);
    (void)TIFFGetStrileByteCount(tif, 0xFFFFFFFF);
    (void)TIFFGetStrileOffset(tif, 0xFFFFFFFF);
    (void)TIFFRawStripSize64(tif, 0xFFFFFFFF);
    
    // Cleanup
    TIFFClose(tif);
    
    return 0;
}