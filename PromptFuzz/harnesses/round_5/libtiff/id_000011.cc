#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tiffio.hxx>
//<ID> 569
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 1
//<Quality> {"density":30,"unique_branches":{"TIFFClientOpenExt":[[402,22,402,32,0,0,4,0]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarnAboutUnknownTags","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFClientOpen","TIFFOpenOptionsFree","TIFFGetField","TIFFIsTiled","TIFFTileSize","TIFFScanlineSize","TIFFFieldWithTag","TIFFFieldTag","TIFFGetClientInfo","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFTileSize","_TIFFmalloc","TIFFReadTile","_TIFFfree","TIFFReadEXIFDirectory","TIFFReadGPSDirectory","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFNumberOfStrips","TIFFRawStripSize64","_TIFFmalloc","TIFFReadEncodedStrip","_TIFFfree","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarnAboutUnknownTags","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFClientOpen","TIFFGetField","TIFFIsTiled","TIFFTileSize","TIFFScanlineSize","TIFFFieldWithTag","TIFFFieldTag","TIFFGetClientInfo","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFTileSize","_TIFFmalloc","TIFFReadTile","_TIFFfree","TIFFReadEXIFDirectory","TIFFReadGPSDirectory","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFNumberOfStrips","TIFFRawStripSize64","_TIFFmalloc","TIFFReadEncodedStrip","_TIFFfree","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to handle TIFF files:

*/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Reopen for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    TIFF *tif = nullptr;
    TIFFOpenOptions *opts = nullptr;
    
    // Allocate and configure TIFF options
    opts = TIFFOpenOptionsAlloc();
    if (opts) {
        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, 1);
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 100 * 1024 * 1024); // 100MB limit
        
        // Open TIFF file using client API (simulating file operations)
        tif = TIFFClientOpen(
            "input_file", 
            "r", 
            (thandle_t)in_file,
            (TIFFReadWriteProc)[](thandle_t fd, void* buf, tmsize_t size) -> tmsize_t {
                return fread(buf, 1, size, (FILE*)fd);
            },
            (TIFFReadWriteProc)nullptr, // No writing for read-only mode
            (TIFFSeekProc)[](thandle_t fd, toff_t off, int whence) -> toff_t {
                fseek((FILE*)fd, off, whence);
                return ftell((FILE*)fd);
            },
            (TIFFCloseProc)[](thandle_t fd) -> int {
                return fclose((FILE*)fd);
            },
            (TIFFSizeProc)[](thandle_t fd) -> toff_t {
                FILE* f = (FILE*)fd;
                long cur = ftell(f);
                fseek(f, 0, SEEK_END);
                long size = ftell(f);
                fseek(f, cur, SEEK_SET);
                return size;
            },
            (TIFFMapFileProc)nullptr,
            (TIFFUnmapFileProc)nullptr
        );
    }
    
    if (!tif) {
        // Clean up and exit if TIFF couldn't be opened
        if (opts) TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Use the required APIs at least once
    
    // 1. Get compression codec (TIFFFindCODEC is not in provided list, using available APIs instead)
    // Since TIFFFindCODEC isn't in our list, we'll use TIFFGetField to check compression
    uint16_t compression = 0;
    if (TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression)) {
        // We would use TIFFFindCODEC here if available
        // For now, we'll just note we would process it
    }
    
    // 2. Get tile size (if image is tiled)
    if (TIFFIsTiled(tif)) {
        tmsize_t tile_size = TIFFTileSize(tif);
        // Use tile_size in some way
        (void)tile_size;
    }
    
    // 3. Get scanline size
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    
    // 4. Get field information for a specific tag
    const TIFFField *field = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
    if (field) {
        uint32_t field_tag = TIFFFieldTag(field);
        // Use field_tag in some way
        (void)field_tag;
    }
    
    // 5. Get client information
    const char *client_info_name = "test_info";
    void *client_info = TIFFGetClientInfo(tif, client_info_name);
    // client_info may be null if not set - that's OK
    
    // Try to read some basic image properties
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        
        // Allocate buffer for scanline if size is reasonable
        if (scanline_size > 0 && scanline_size < 100 * 1024 * 1024) {
            void *scanline_buf = _TIFFmalloc(scanline_size);
            if (scanline_buf) {
                // Try to read a scanline
                if (scanline_size > 0) {
                    // Read first scanline
                    TIFFReadScanline(tif, scanline_buf, 0, 0);
                }
                _TIFFfree(scanline_buf);
            }
        }
        
        // Check if image is tiled and try to read a tile
        if (TIFFIsTiled(tif)) {
            uint32_t tile_width = 0, tile_height = 0;
            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
            
            if (tile_width > 0 && tile_height > 0) {
                tmsize_t tile_size = TIFFTileSize(tif);
                if (tile_size > 0 && tile_size < 100 * 1024 * 1024) {
                    void *tile_buf = _TIFFmalloc(tile_size);
                    if (tile_buf) {
                        // Try to read first tile
                        TIFFReadTile(tif, tile_buf, 0, 0, 0, 0);
                        _TIFFfree(tile_buf);
                    }
                }
            }
        }
    }
    
    // Try to read EXIF and GPS directories if they exist
    if (TIFFReadEXIFDirectory(tif, 0)) {
        // EXIF directory read successfully
    }
    
    if (TIFFReadGPSDirectory(tif, 0)) {
        // GPS directory read successfully
    }
    
    // Check number of directories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    // Try to navigate through directories
    for (tdir_t dir = 0; dir < num_dirs && dir < 10; dir++) {
        if (!TIFFSetDirectory(tif, dir)) {
            break;
        }
        
        // Get strip information for this directory
        uint32_t strips_per_image = TIFFNumberOfStrips(tif);
        if (strips_per_image > 0 && strips_per_image < 1000) {
            for (uint32_t strip = 0; strip < strips_per_image && strip < 10; strip++) {
                uint64_t strip_size = TIFFRawStripSize64(tif, strip);
                if (strip_size > 0 && strip_size < 10 * 1024 * 1024) {
                    void *strip_buf = _TIFFmalloc(strip_size);
                    if (strip_buf) {
                        TIFFReadEncodedStrip(tif, strip, strip_buf, strip_size);
                        _TIFFfree(strip_buf);
                    }
                }
            }
        }
    }
    
    // Clean up
    TIFFClose(tif);
    if (opts) TIFFOpenOptionsFree(opts);
    
    // Note: The file was already closed by TIFFClose via our callback
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}