// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal valid TIFF header (Classic TIFF, little-endian)
    // Byte 0-1: Magic (0x002A for little-endian)
    uint16_t magic = 0x002A;
    fwrite(&magic, sizeof(uint16_t), 1, f);
    
    // Byte 2-3: Offset to first IFD (set to 8, right after header)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);
    
    // Write a minimal IFD at offset 8
    // Number of directory entries (0 for empty, or 1 for minimal)
    uint16_t num_entries = 0;
    fwrite(&num_entries, sizeof(uint16_t), 1, f);
    
    // Next IFD offset (0 means no more IFDs)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);
    
    // If we have input data, append it as raw data after the IFD
    if (data && size > 0) {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    
    // Prepare environment: Create a dummy file if needed
    write_dummy_tiff(Data, Size);
    
    // Open the dummy file for reading
    tif = TIFFOpen("./dummy_file", "r");
    
    if (tif) {
        // Try to read directory to initialize internal state
        if (TIFFReadDirectory(tif) == 0) {
            // Directory read failed, but we can still test some functions
            // that might return default values or handle errors gracefully
        }
        
        // Fuzz TIFFCurrentTile
        uint32_t current_tile = TIFFCurrentTile(tif);
        (void)current_tile; // Suppress unused warning
        
        // Fuzz TIFFCurrentRow
        uint32_t current_row = TIFFCurrentRow(tif);
        (void)current_row;
        
        // Fuzz TIFFCurrentDirectory
        tdir_t current_dir = TIFFCurrentDirectory(tif);
        (void)current_dir;
        
        // Fuzz TIFFCurrentStrip
        uint32_t current_strip = TIFFCurrentStrip(tif);
        (void)current_strip;
        
        // Fuzz TIFFCheckTile with various inputs derived from fuzz data
        if (Size >= 16) {
            uint32_t x = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
            uint32_t y = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
            uint32_t z = (Data[8] << 24) | (Data[9] << 16) | (Data[10] << 8) | Data[11];
            uint16_t s = (Data[12] << 8) | Data[13];
            
            // Limit coordinates to reasonable ranges to avoid overflow issues
            // but still test boundary conditions
            int check_result = TIFFCheckTile(tif, x % 1000000, y % 1000000, z % 1000000, s % 1000);
            (void)check_result;
            
            // Test with zero coordinates
            check_result = TIFFCheckTile(tif, 0, 0, 0, 0);
            (void)check_result;
            
            // Test with max uint32 values (will likely fail but tests error handling)
            check_result = TIFFCheckTile(tif, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF);
            (void)check_result;
        } else if (Size >= 4) {
            // Fallback for smaller inputs
            uint32_t x = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
            int check_result = TIFFCheckTile(tif, x % 1000, 0, 0, 0);
            (void)check_result;
        } else {
            // Minimal test
            int check_result = TIFFCheckTile(tif, 0, 0, 0, 0);
            (void)check_result;
        }
        
        // Fuzz TIFFNumberOfTiles
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        (void)num_tiles;
        
        // Try to read a tile to change state, then check again
        if (Size > 0) {
            uint8_t *tile_buf = malloc(Size > 65536 ? 65536 : Size);
            if (tile_buf) {
                // Attempt to read a tile (may fail if not tiled or invalid)
                tmsize_t bytes_read = TIFFReadTile(tif, tile_buf, 0, 0, 0, 0);
                (void)bytes_read;
                
                // Check state after read attempt
                current_tile = TIFFCurrentTile(tif);
                (void)current_tile;
                
                free(tile_buf);
            }
        }
        
        // Try to read a strip to change state (using TIFFReadRawStrip)
        if (Size > 0) {
            uint8_t *strip_buf = malloc(Size > 65536 ? 65536 : Size);
            if (strip_buf) {
                tmsize_t bytes_read = TIFFReadRawStrip(tif, 0, strip_buf, Size > 65536 ? 65536 : Size);
                (void)bytes_read;
                
                current_strip = TIFFCurrentStrip(tif);
                (void)current_strip;
                
                free(strip_buf);
            }
        }
        
        // Try to read a row to change state (using TIFFReadScanline with correct args)
        if (Size > 0) {
            uint8_t *row_buf = malloc(Size > 65536 ? 65536 : Size);
            if (row_buf) {
                int bytes_read = TIFFReadScanline(tif, row_buf, 0, 0);
                (void)bytes_read;
                
                current_row = TIFFCurrentRow(tif);
                (void)current_row;
                
                free(row_buf);
            }
        }
        
        // Cleanup
        TIFFClose(tif);
    }
    
    return 0;
}