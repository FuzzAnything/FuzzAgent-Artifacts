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
//<ID> 581
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 3
//<Quality> {"density":19,"unique_branches":{"TIFFWriteBufferSetup":[[692,9,692,31,0,0,4,1],[706,9,706,19,0,0,4,1]],"TIFFUnlinkDirectory":[[2237,9,2237,34,0,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFGetMode","TIFFSetMode","TIFFIsTiled","TIFFVTileSize","TIFFCreateDirectory","TIFFGetStrileOffsetWithErr","TIFFNumberOfDirectories","TIFFUnlinkDirectory","TIFFSetFileName","_TIFFmalloc","TIFFWriteBufferSetup","TIFFGetField","TIFFGetField","TIFFIsTiled","_TIFFmalloc","TIFFReadRGBATileExt","_TIFFfree","_TIFFfree","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFGetMode","TIFFSetMode","TIFFIsTiled","TIFFVTileSize","TIFFCreateDirectory","TIFFGetStrileOffsetWithErr","TIFFNumberOfDirectories","TIFFUnlinkDirectory","TIFFSetFileName","_TIFFmalloc","TIFFWriteBufferSetup","TIFFGetField","TIFFGetField","TIFFIsTiled","_TIFFmalloc","TIFFReadRGBATileExt","_TIFFfree","_TIFFfree","TIFFClose"],"visited":0}
/**/


// Include libtiff headers

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
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
    
    // Open the input file for reading using libtiff
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    TIFF *tif = NULL;
    
    // Define custom I/O functions for TIFFClientOpen
    auto readProc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, (FILE*)handle);
    };
    
    auto writeProc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fwrite(buf, 1, size, (FILE*)handle);
    };
    
    auto seekProc = [](thandle_t handle, toff_t off, int whence) -> toff_t {
        return fseek((FILE*)handle, off, whence) == 0 ? ftell((FILE*)handle) : (toff_t)-1;
    };
    
    auto closeProc = [](thandle_t handle) -> int {
        return fclose((FILE*)handle);
    };
    
    auto sizeProc = [](thandle_t handle) -> toff_t {
        long cur = ftell((FILE*)handle);
        fseek((FILE*)handle, 0, SEEK_END);
        long end = ftell((FILE*)handle);
        fseek((FILE*)handle, cur, SEEK_SET);
        return end;
    };
    
    // Open TIFF file using client functions
    tif = TIFFClientOpen("input_file", "r", (thandle_t)in_file,
                        readProc, writeProc, seekProc,
                        closeProc, sizeProc, NULL, NULL);
    
    if (!tif) {
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Set mode
    int current_mode = TIFFGetMode(tif);
    TIFFSetMode(tif, current_mode);
    
    // 2. Calculate VTile size
    uint32_t nrows = 1;
    if (TIFFIsTiled(tif)) {
        tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
        (void)vtile_size; // Use the result
    }
    
    // 3. Create EXIF directory (if supported)
    // Note: TIFFCreateEXIFDirectory is not in the provided list, but we'll attempt
    // to use it as per the instruction
    int exif_result = 0;
    // We'll use TIFFCreateDirectory as an alternative since TIFFCreateEXIFDirectory
    // might not be available
    exif_result = TIFFCreateDirectory(tif);
    
    // 4. Get strile offset with error checking
    uint32_t strile = 0;
    int pbErr = 0;
    uint64_t strile_offset = TIFFGetStrileOffsetWithErr(tif, strile, &pbErr);
    
    // 5. Unlink directory (if we have multiple directories)
    tdir_t dir_count = TIFFNumberOfDirectories(tif);
    if (dir_count > 1) {
        TIFFUnlinkDirectory(tif, 1); // Try to unlink second directory
    }
    
    // 6. Set file name
    const char *new_filename = TIFFSetFileName(tif, "output_file");
    
    // 7. Setup write buffer
    tmsize_t buffer_size = 4096;
    void *buffer = _TIFFmalloc(buffer_size);
    if (buffer) {
        int buffer_setup = TIFFWriteBufferSetup(tif, buffer, buffer_size);
        (void)buffer_setup;
    }
    
    // 8. Read RGBA tile (if image is tiled)
    uint32_t tile_width = 0, tile_height = 0;
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
    
    if (TIFFIsTiled(tif) && tile_width > 0 && tile_height > 0) {
        uint32_t *raster = (uint32_t*)_TIFFmalloc(tile_width * tile_height * sizeof(uint32_t));
        if (raster) {
            int stop_on_error = 1;
            int tile_result = TIFFReadRGBATileExt(tif, 0, 0, raster, stop_on_error);
            (void)tile_result;
            _TIFFfree(raster);
        }
    }
    
    // Cleanup
    if (buffer) {
        _TIFFfree(buffer);
    }
    
    TIFFClose(tif);
    
    // Note: The file is closed by TIFFClose via our closeProc
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}