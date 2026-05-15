// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test with read mode
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Try to read directory if possible
        int read_result = TIFFReadDirectory(tif);
        
        // Get current directory offset if available
        uint64_t dir_offset = TIFFCurrentDirOffset(tif);
        
        // Try to set subdirectory using offset from data
        uint64_t subdir_offset;
        memcpy(&subdir_offset, Data, sizeof(uint64_t));
        TIFFSetSubDirectory(tif, subdir_offset);
        
        // Try checkpoint if in appropriate state
        if (read_result > 0) {
            TIFFCheckpointDirectory(tif);
        }
        
        TIFFClose(tif);
    }
    
    // Test with write mode
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        // Set up minimal TIFF structure for writing operations
        uint32_t width = 1, height = 1;
        uint16_t samplesperpixel = 1, bitspersample = 8;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
        
        // Write dummy pixel data
        unsigned char pixel = 0;
        TIFFWriteEncodedStrip(tif, 0, &pixel, 1);
        
        // Test write operations
        toff_t write_offset;
        memcpy(&write_offset, Data + (Size > 8 ? 8 : 0), sizeof(toff_t));
        TIFFSetWriteOffset(tif, write_offset);
        
        uint64_t custom_dir_offset = 0;
        TIFFWriteCustomDirectory(tif, &custom_dir_offset);
        
        TIFFCheckpointDirectory(tif);
        
        TIFFClose(tif);
    }
    
    // Test with read/write mode
    tif = TIFFOpen("./dummy_file", "r+");
    if (tif) {
        // Try reading first
        TIFFReadDirectory(tif);
        
        // Get current offset
        uint64_t current_offset = TIFFCurrentDirOffset(tif);
        
        // Try setting subdirectory with various offsets
        for (size_t i = 0; i < Size && i < 64; i += 8) {
            uint64_t test_offset;
            memcpy(&test_offset, Data + i, sizeof(uint64_t) < (Size - i) ? sizeof(uint64_t) : (Size - i));
            TIFFSetSubDirectory(tif, test_offset);
        }
        
        // Try to set back to original if possible
        if (current_offset != 0) {
            TIFFSetSubDirectory(tif, current_offset);
        }
        
        // Try checkpoint
        TIFFCheckpointDirectory(tif);
        
        TIFFClose(tif);
    }
    
    remove_dummy_file();
    return 0;
}