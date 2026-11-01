// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

/* Helper to replicate TIFFroundup_32 behavior if not exported or to ensure compatibility */
static inline size_t my_tiff_roundup_32(size_t val, size_t align) {
    return (val + align - 1) & ~(align - 1);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) == 0 ||
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) == 0) {
        TIFFClose(tif);
        return 0;
    }

    // Limit dimensions to prevent OOM
    if (width > 4096 || height > 4096) {
        TIFFClose(tif);
        return 0;
    }

    uint32_t *rgba_buf = NULL;
    size_t buf_size = (size_t)width * (size_t)height * sizeof(uint32_t);
    if (buf_size > 0 && buf_size < (1024 * 1024 * 1024)) {
        rgba_buf = (uint32_t *)malloc(buf_size);
        if (!rgba_buf) {
            TIFFClose(tif);
            return 0;
        }
        memset(rgba_buf, 0, buf_size);
    }

    // Fuzz TIFFReadRGBAImage
    if (rgba_buf) {
        TIFFReadRGBAImage(tif, width, height, rgba_buf, 0);
        TIFFReadRGBAImage(tif, width, height, rgba_buf, 1);
    }

    // Fuzz TIFFRGBAImageBegin and related
    TIFFRGBAImage img;
    char err[1024];
    memset(&img, 0, sizeof(img));
    TIFFRGBAImageBegin(&img, tif, 0, err);
    if (rgba_buf && img.width > 0 && img.height > 0) {
        // Attempt to read a row if initialized successfully
        // Note: TIFFRGBAImageGetRow is not in the target list, but we can try to trigger state
        // by just ensuring the struct is populated.
        (void)img; 
    }

    // Fuzz TIFFReadRGBAStrip and TIFFReadRGBAStripExt
    uint32_t rowsperstrip = 0;
    if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip) == 0) {
        rowsperstrip = 1;
    }
    if (rgba_buf && rowsperstrip > 0) {
        // Read first strip
        TIFFReadRGBAStrip(tif, 0, rgba_buf);
        TIFFReadRGBAStripExt(tif, 0, rgba_buf, 0);
        TIFFReadRGBAStripExt(tif, 0, rgba_buf, 1);
        
        // Try reading a strip that might not exist or is misaligned
        if (height > rowsperstrip) {
            TIFFReadRGBAStrip(tif, 1, rgba_buf);
            TIFFReadRGBAStripExt(tif, 1, rgba_buf, 0);
        }
    }

    // Fuzz TIFFReadRGBATile
    uint32_t tilewidth = 0, tilelength = 0;
    if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth) == 0) {
        tilewidth = 64;
    }
    if (TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength) == 0) {
        tilelength = 64;
    }
    if (rgba_buf && tilewidth > 0 && tilelength > 0) {
        size_t tile_size = (size_t)tilewidth * tilelength * sizeof(uint32_t);
        if (tile_size < buf_size) {
            TIFFReadRGBATile(tif, 0, 0, rgba_buf);
            TIFFReadRGBATile(tif, 1, 0, rgba_buf);
        }
    }

    // Fuzz TIFFYCbCrtoRGB
    // Allocate buffer for YCbCrToRGB structure as per documentation
    // Using manual calculation to avoid dependency on internal TIFFroundup_32
    size_t ycbcr_struct_size = sizeof(TIFFYCbCrToRGB);
    size_t align_size = sizeof(long);
    size_t aligned_struct_size = my_tiff_roundup_32(ycbcr_struct_size, align_size);
    
    size_t ycbcr_size = aligned_struct_size +
                        4 * 256 * sizeof(TIFFRGBValue) +
                        2 * 256 * sizeof(int) +
                        3 * 256 * sizeof(int32_t);
    
    TIFFYCbCrToRGB *ycbcr = NULL;
    if (ycbcr_size > 0 && ycbcr_size < (1024 * 1024)) {
        ycbcr = (TIFFYCbCrToRGB *)malloc(ycbcr_size);
        if (ycbcr) {
            memset(ycbcr, 0, ycbcr_size);
            // Initialize the YCbCr conversion
            TIFFYCbCrToRGBInit(ycbcr, 0, 0);
            
            uint32_t r, g, b;
            // Fuzz with various Y, Cb, Cr values
            TIFFYCbCrtoRGB(ycbcr, 0, 0, 0, &r, &g, &b);
            TIFFYCbCrtoRGB(ycbcr, 255, 128, 128, &r, &g, &b);
            TIFFYCbCrtoRGB(ycbcr, 128, 0, 255, &r, &g, &b);
            TIFFYCbCrtoRGB(ycbcr, 255, 255, 0, &r, &g, &b);
            // Out of bounds inputs (should be clamped)
            TIFFYCbCrtoRGB(ycbcr, 500, 300, 300, &r, &g, &b);
            TIFFYCbCrtoRGB(ycbcr, 100, -100, -100, &r, &g, &b);
            
            free(ycbcr);
        }
    }

    if (rgba_buf) {
        free(rgba_buf);
    }
    TIFFClose(tif);
    return 0;
}