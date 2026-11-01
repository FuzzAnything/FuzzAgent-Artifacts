// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void dummy_cleanup(TIFF *tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Create a dummy file with the fuzzing data
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file for reading and writing to test various APIs
    // We use O_RDWR to allow write operations for some tests
    TIFF *tif = TIFFOpen(filename, "r+");
    if (!tif) {
        // If r+ fails, try opening in write mode to create a new file
        // This handles cases where the input data is not a valid TIFF
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            return 0;
        }
        // Set minimal required fields for a valid TIFF in write mode
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)(Data[0] % 100) + 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)(Data[1] % 100) + 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    }

    // Fuzz TIFFSetSubDirectory
    // Use data from input to determine offset
    uint64_t sub_dir_offset = 0;
    if (Size >= 8) {
        memcpy(&sub_dir_offset, Data, 8);
        // Ensure offset is reasonable to avoid immediate crash, but allow 0
        if (sub_dir_offset > 0 && sub_dir_offset < Size) {
            TIFFSetSubDirectory(tif, sub_dir_offset);
        }
    }
    // Also try resetting
    TIFFSetSubDirectory(tif, 0);

    // Fuzz TIFFCurrentDirOffset
    uint64_t current_offset = TIFFCurrentDirOffset(tif);
    (void)current_offset; // Suppress unused variable warning

    // Fuzz TIFFGetStrileOffset
    uint32_t strile_idx = 0;
    if (Size >= 4) {
        memcpy(&strile_idx, Data, 4);
        // Modulo to keep index within reasonable bounds, though API handles errors
        strile_idx = strile_idx % 1000;
    }
    uint64_t strile_off = TIFFGetStrileOffset(tif, strile_idx);
    (void)strile_off;

    // Fuzz TIFFSetWriteOffset
    toff_t write_off = 0;
    if (Size >= 8) {
        memcpy(&write_off, Data, 8);
        // Limit offset to file size to avoid extreme corruption in test env
        if (write_off > Size) {
            write_off = Size;
        }
    }
    TIFFSetWriteOffset(tif, write_off);

    // Fuzz TIFFWriteCheck
    // Try different row/tile indices
    int write_check_status = 0;
    if (Size >= 2) {
        int row = (int)(Data[0]) % 100;
        write_check_status = TIFFWriteCheck(tif, row, "fuzz_row");
        (void)write_check_status;
    }
    // Try with tile index
    if (Size >= 3) {
        int tile = (int)(Data[1]) % 100;
        write_check_status = TIFFWriteCheck(tif, tile, "fuzz_tile");
        (void)write_check_status;
    }

    // Fuzz TIFFRewriteDirectory
    // This requires a valid directory to be set. 
    // If we opened in write mode, we might need to write a directory first.
    // If we opened in read mode, it should have read the first directory.
    int rewrite_status = TIFFRewriteDirectory(tif);
    (void)rewrite_status;

    // Cleanup
    dummy_cleanup(tif);

    // Clean up the dummy file
    remove(filename);

    return 0;
}