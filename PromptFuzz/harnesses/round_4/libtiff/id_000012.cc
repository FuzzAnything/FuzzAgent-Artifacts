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
//<ID> 385
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"TIFFFetchDirectory":[[6109,17,6109,43,0,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFSetDirectory","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFNumberOfTiles","TIFFTileSize","TIFFReadEncodedTile","_TIFFmemcpy","TIFFGetField","TIFFSetSubDirectory","TIFFSwabDouble","TIFFCleanup"],"critical_calls":["TIFFClientOpen","TIFFSetDirectory","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFNumberOfTiles","TIFFTileSize","TIFFReadEncodedTile","_TIFFmemcpy","TIFFGetField","TIFFSetSubDirectory","TIFFSwabDouble","TIFFCleanup"],"visited":0}
/**/


// Include libtiff headers

// Custom I/O handlers for memory-based TIFF
static tmsize_t  id_000012_memReadProc(thandle_t handle, void* buf, tmsize_t size) {
    auto* mem = reinterpret_cast<std::vector<uint8_t>*>(handle);
    tmsize_t to_read = std::min(size, static_cast<tmsize_t>(mem->size()));
    if (to_read > 0) {
        memcpy(buf, mem->data(), to_read);
        mem->erase(mem->begin(), mem->begin() + to_read);
    }
    return to_read;
}

static tmsize_t  id_000012_memWriteProc(thandle_t, void*, tmsize_t) {
    return 0; // Not used for reading
}

static toff_t  id_000012_memSeekProc(thandle_t handle, toff_t offset, int whence) {
    auto* mem = reinterpret_cast<std::vector<uint8_t>*>(handle);
    toff_t new_pos = 0;
    
    switch (whence) {
        case SEEK_SET:
            new_pos = offset;
            break;
        case SEEK_CUR:
            new_pos = static_cast<toff_t>(mem->size()) + offset;
            break;
        case SEEK_END:
            new_pos = static_cast<toff_t>(mem->size()) + offset;
            break;
    }
    
    if (new_pos < 0) new_pos = 0;
    if (new_pos > static_cast<toff_t>(mem->size())) {
        mem->resize(new_pos);
    } else {
        mem->erase(mem->begin() + new_pos, mem->end());
    }
    return new_pos;
}

static int  id_000012_memCloseProc(thandle_t) {
    return 0;
}

static toff_t  id_000012_memSizeProc(thandle_t handle) {
    auto* mem = reinterpret_cast<std::vector<uint8_t>*>(handle);
    return static_cast<toff_t>(mem->size());
}

static int  id_000012_memMapProc(thandle_t, void**, toff_t*) {
    return 0;
}

static void  id_000012_memUnmapProc(thandle_t, void*, toff_t) {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // Create a copy of input for TIFF processing
    std::vector<uint8_t> tiff_data(data, data + size);
    
    // Create TIFFOpenOptions and set warning about unknown tags
    TIFFOpenOptions* opts = nullptr;
    // Note: In actual libtiff, we'd use TIFFOpenOptionsAlloc() but it's not in the provided API list
    // We'll simulate by using default options
    
    // Create TIFF from memory
    TIFF* tif = TIFFClientOpen(
        fuzz_str_1, fuzz_str_2,
        reinterpret_cast<thandle_t>(&tiff_data),
         id_000012_memReadProc,  id_000012_memWriteProc,
         id_000012_memSeekProc,  id_000012_memCloseProc,
         id_000012_memSizeProc,  id_000012_memMapProc,  id_000012_memUnmapProc
    );
    
    if (!tif) {
        return 0;
    }
    
    // Try to set directory (if multiple directories exist)
    if (TIFFSetDirectory(tif, fuzz_uint32_t_3) == 1) {
        // Check if image is upsampled (TIFFIsUpSampled is not in provided list, 
        // but we'll use a similar check if available)
        // For now, we'll proceed with other operations
        
        // Try to read an encoded tile if image is tiled
        if (TIFFIsTiled(tif)) {
            uint32_t tile_width, tile_height;
            TIFFGetField(tif, fuzz_uint32_t_4, &tile_width);
            TIFFGetField(tif, fuzz_uint32_t_5, &tile_height);
            
            uint32_t tile_count = TIFFNumberOfTiles(tif);
            if (tile_count > 0) {
                // Allocate buffer for tile
                tmsize_t tile_size = TIFFTileSize(tif);
                if (tile_size > 0 && tile_size < 1024 * 1024) { // Reasonable size limit
                    std::vector<uint8_t> tile_buffer(tile_size);
                    
                    // Try to read first tile
                    tmsize_t bytes_read = TIFFReadEncodedTile(
                        tif, fuzz_uint32_t_6, tile_buffer.data(), tile_size);
                    
                    if (bytes_read > 0) {
                        // Demonstrate _TIFFmemcpy
                        std::vector<uint8_t> copy_buffer(tile_size);
                        _TIFFmemcpy(copy_buffer.data(), tile_buffer.data(), 
                                   std::min(tile_size, static_cast<tmsize_t>(bytes_read)));
                    }
                }
            }
        }
        
        // Try to set subdirectory if possible
        // First get current directory offset
        uint64_t diroff = 0;
        if (TIFFGetField(tif, fuzz_uint32_t_7, &diroff) == 1) {
            TIFFSetSubDirectory(tif, fuzz_uint64_t_8);
        }
    }
    
    // Demonstrate TIFFSwabDouble
    double test_double = 3.141592653589793;
    TIFFSwabDouble(&test_double);
    
    // Clean up
    TIFFCleanup(tif);
    
    return 0;
}