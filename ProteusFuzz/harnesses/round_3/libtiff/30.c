// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header
    // Byte order (II = little endian)
    uint16_t byte_order = 0x4949;
    fwrite(&byte_order, sizeof(uint16_t), 1, f);
    
    // TIFF magic number
    uint16_t magic = 42;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    
    // Offset to first IFD (will be at offset 8)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    // IFD entry count (0 entries for minimal test)
    uint16_t entry_count = 0;
    fwrite(&entry_count, sizeof(uint16_t), 1, f);
    
    // Next IFD offset (0 = none)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);
    
    // Append fuzzing data as raw payload if any
    if (data && size > 0) {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
}

static void cleanup_tiff(TIFF *tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write the fuzzing input to a dummy file
    write_dummy_tiff(Data, Size);
    
    // Open the file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) return 0;
    
    // Get basic image properties for buffer sizing
    uint32_t width = 1;
    uint32_t height = 1;
    
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width)) {
        if (width == 0) width = 1;
        if (width > 4096) width = 4096;
    }
    if (TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        if (height == 0) height = 1;
        if (height > 4096) height = 4096;
    }
    
    // Allocate buffer for RGBA data (RGBA = 4 bytes per pixel)
    size_t buf_size = width * height * sizeof(uint32_t);
    uint32_t *rgba_buf = (uint32_t *)malloc(buf_size);
    if (!rgba_buf) {
        cleanup_tiff(tif);
        return 0;
    }
    
    // Initialize buffer
    memset(rgba_buf, 0, buf_size);
    
    // Fuzzing Strategy 1: TIFFReadRGBAImage
    int stop_on_error = (Data[0] % 2) != 0;
    TIFFReadRGBAImage(tif, width, height, rgba_buf, stop_on_error);
    
    // Fuzzing Strategy 2: TIFFReadRGBAImageOriented
    // Use only defined orientation constants
    int orientations[] = {ORIENTATION_TOPLEFT, ORIENTATION_TOPRIGHT, 
                          ORIENTATION_BOTLEFT, ORIENTATION_BOTRIGHT,
                          ORIENTATION_LEFTTOP, ORIENTATION_RIGHTTOP};
    int num_orientations = 6;
    int orientation_idx = (Data[1] % num_orientations);
    int orientation = orientations[orientation_idx];
    
    memset(rgba_buf, 0, buf_size);
    TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, orientation, stop_on_error);
    
    // Fuzzing Strategy 3: TIFFReadRGBAStrip
    uint32_t strip_idx = (Data[2] % 10);
    memset(rgba_buf, 0, buf_size);
    TIFFReadRGBAStrip(tif, strip_idx, rgba_buf);
    
    // Fuzzing Strategy 4 & 5: TIFFReadRGBATile and TIFFReadRGBATileExt
    uint32_t tile_w, tile_h;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_w) && 
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_h)) {
        // Calculate tile buffer size
        size_t tile_buf_size = tile_w * tile_h * sizeof(uint32_t);
        uint32_t *tile_buf = (uint32_t *)malloc(tile_buf_size);
        if (tile_buf) {
            memset(tile_buf, 0, tile_buf_size);
            
            uint32_t tile_col = (Data[3] % 10);
            uint32_t tile_row = (Data[4] % 10);
            
            TIFFReadRGBATile(tif, tile_col, tile_row, tile_buf);
            
            memset(tile_buf, 0, tile_buf_size);
            int stop_ext = (Data[5] % 2) != 0;
            TIFFReadRGBATileExt(tif, tile_col, tile_row, tile_buf, stop_ext);
            
            free(tile_buf);
        }
    }
    
    // Fuzzing Strategy 6: TIFFRGBAImageGet
    // Correct usage based on libtiff API: TIFFRGBAImageOK(tif, errbuf)
    // TIFFRGBAImageBegin(&img, tif, stoponerr, errbuf)
    char errbuf[1024];
    TIFFRGBAImage img;
    memset(&img, 0, sizeof(img));
    
    if (TIFFRGBAImageOK(tif, errbuf)) {
        if (TIFFRGBAImageBegin(&img, tif, stop_on_error, errbuf)) {
            uint32_t *region_buf = (uint32_t *)malloc(buf_size);
            if (region_buf) {
                memset(region_buf, 0, buf_size);
                uint32_t region_h = (Data[6] % height) + 1;
                // Ensure region_h doesn't exceed height
                if (region_h > height) region_h = height;
                TIFFRGBAImageGet(&img, region_buf, width, region_h);
                free(region_buf);
            }
            TIFFRGBAImageEnd(&img);
        }
    }
    
    // Cleanup
    free(rgba_buf);
    cleanup_tiff(tif);
    
    return 0;
}