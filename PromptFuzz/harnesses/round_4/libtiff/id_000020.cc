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
//<ID> 848
//<Prompt> []
/*<Combination>: [
*/
//<score> 56, nr_unique_branch: 8
//<Quality> {"density":14,"unique_branches":{"TIFFReadCustomDirectory":[[5386,9,5386,18,0,0,4,0]],"TIFFFetchDirectory":[[6089,9,6089,19,0,0,4,1],[6159,17,6159,49,0,0,4,1],[6170,21,6170,28,0,0,4,1],[6172,21,6172,47,0,0,4,1],[6339,13,6339,45,0,0,4,1],[6351,17,6351,43,0,0,4,1]],"TIFFReadDirEntryIfd8":[[1172,9,1172,23,0,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFCurrentDirOffset","TIFFReadEXIFDirectory","TIFFReadGPSDirectory","TIFFFlushData","TIFFTileSize64","TIFFVTileSize64","TIFFGetField","TIFFGetField","TIFFIsTiled","TIFFScanlineSize","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFCurrentDirOffset","TIFFReadEXIFDirectory","TIFFReadGPSDirectory","TIFFFlushData","TIFFTileSize64","TIFFVTileSize64","TIFFGetField","TIFFGetField","TIFFIsTiled","TIFFScanlineSize","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFClose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to read and process TIFF files containing EXIF and GPS directories:

*/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file
    FILE *tmp_file = fopen("input_file", "wb");
    if (!tmp_file) {
        assert_file_closed(&tmp_file);
	return 0;
    }
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;
    
    TIFF *tif = nullptr;
    
    // Open the TIFF file using TIFFClientOpen
    FILE *in_file = fopen("input_file", "rb");
    if (!in_file) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Define TIFF client callbacks
    auto read_proc = [](thandle_t fd, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, reinterpret_cast<FILE*>(fd));
    };
    
    auto write_proc = [](thandle_t fd, void* buf, tmsize_t size) -> tmsize_t {
        return fwrite(buf, 1, size, reinterpret_cast<FILE*>(fd));
    };
    
    auto seek_proc = [](thandle_t fd, toff_t off, int whence) -> toff_t {
        fseek(reinterpret_cast<FILE*>(fd), off, whence);
        return ftell(reinterpret_cast<FILE*>(fd));
    };
    
    auto close_proc = [](thandle_t fd) -> int {
        return fclose(reinterpret_cast<FILE*>(fd));
    };
    
    auto size_proc = [](thandle_t fd) -> toff_t {
        long curr = ftell(reinterpret_cast<FILE*>(fd));
        fseek(reinterpret_cast<FILE*>(fd), 0, SEEK_END);
        long size = ftell(reinterpret_cast<FILE*>(fd));
        fseek(reinterpret_cast<FILE*>(fd), curr, SEEK_SET);
        return size;
    };
    
    // Open TIFF file using client callbacks
    tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                         reinterpret_cast<thandle_t>(in_file),
                         read_proc, write_proc,
                         seek_proc, close_proc,
                         size_proc,
                         nullptr, nullptr);
    
    if (!tif) {
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Get the current directory offset
    uint64_t dir_offset = TIFFCurrentDirOffset(tif);
    
    // Try to read EXIF directory (if present)
    // Note: TIFFReadEXIFDirectory reads EXIF data from specified offset
    // We'll try to read from the current directory offset
    int exif_result = TIFFReadEXIFDirectory(tif, dir_offset);
    
    // Try to read GPS directory (if present)
    int gps_result = TIFFReadGPSDirectory(tif, dir_offset);
    
    // Flush any pending data
    TIFFFlushData(tif);
    
    // Get tile size information
    uint64_t tile_size = TIFFTileSize64(tif);
    
    // Get vertical tile size with 1 row
    uint64_t v_tile_size = TIFFVTileSize64(tif, fuzz_uint32_t_3);
    
    // Try to get basic TIFF information
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, fuzz_uint32_t_4, &width)) {
        // Image width obtained
    }
    if (TIFFGetField(tif, fuzz_uint32_t_5, &height)) {
        // Image height obtained
    }
    
    // Try to read scanlines if image is not tiled
    if (!TIFFIsTiled(tif) && height > 0) {
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0) {
            uint8_t *buffer = static_cast<uint8_t*>(_TIFFmalloc(scanline_size));
            if (buffer) {
                // Try to read first scanline
                TIFFReadScanline(tif, buffer, fuzz_uint32_t_6, fuzz_uint16_t_7);
                _TIFFfree(buffer);
            }
        }
    }
    
    // Create an output file to test writing
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        assert_file_closed(&out_file);;
        remove("output_file");
    }
    
    // Cleanup
    TIFFClose(tif);
    
    // Remove temporary input file
    remove("input_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	return 0;
}

// Note: TIFFReadEXIFDirectory and TIFFReadGPSDirectory are not in the provided API list,
// but they are mentioned in the instructions. If they don't exist in your libtiff version,
// you might need to use TIFFReadCustomDirectory with appropriate directory offsets
// or create the directories first with TIFFCreateEXIFDirectory/TIFFCreateGPSDirectory

// For compilation, you might need to define these if they don't exist:
#ifndef TIFFReadEXIFDirectory
#endif

#ifndef TIFFReadGPSDirectory
#endif