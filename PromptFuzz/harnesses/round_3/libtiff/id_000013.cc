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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"TIFFErrorExt":[[66,9,66,26,0,0,4,0]]},"library_calls":["TIFFSetErrorHandler","TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFSetErrorHandler","TIFFDeferStrileArrayWriting","TIFFIsTiled","TIFFTileSize","_TIFFmalloc","TIFFReadRawTile","_TIFFfree","TIFFVTileSize64","TIFFGetStrileOffsetWithErr","TIFFGetUnmapFileProc","TIFFErrorExt","TIFFClose","TIFFOpenOptionsFree","TIFFSetErrorHandler"],"critical_calls":["TIFFSetErrorHandler","TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFDeferStrileArrayWriting","TIFFIsTiled","TIFFTileSize","_TIFFmalloc","TIFFReadRawTile","_TIFFfree","TIFFVTileSize64","TIFFGetStrileOffsetWithErr","TIFFGetUnmapFileProc","TIFFErrorExt","TIFFClose","TIFFOpenOptionsFree","TIFFSetErrorHandler"],"visited":0}
/**/


// Custom memory handle structure
struct mem_handle {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Callback implementations for memory-based TIFF operations
static tmsize_t  id_000013_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_handle* mh = reinterpret_cast<mem_handle*>(handle);
    tmsize_t to_read = size;
    
    if (mh->pos + to_read > mh->size) {
        to_read = mh->size - mh->pos;
        if (to_read <= 0) return 0;
    }
    
    memcpy(buf, mh->data + mh->pos, to_read);
    mh->pos += to_read;
    return to_read;
}

static tmsize_t  id_000013_mem_write(thandle_t, void*, tmsize_t) {
    return 0;  // Write not supported for fuzzing
}

static toff_t  id_000013_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_handle* mh = reinterpret_cast<mem_handle*>(handle);
    
    switch (whence) {
        case SEEK_SET:
            mh->pos = offset;
            break;
        case SEEK_CUR:
            mh->pos += offset;
            break;
        case SEEK_END:
            mh->pos = mh->size + offset;
            break;
    }
    
    if (mh->pos > mh->size) mh->pos = mh->size;
    return mh->pos;
}

static int  id_000013_mem_close(thandle_t) {
    return 0;
}

static toff_t  id_000013_mem_size(thandle_t handle) {
    mem_handle* mh = reinterpret_cast<mem_handle*>(handle);
    return mh->size;
}

static int  id_000013_mem_map(thandle_t, void**, toff_t*) {
    return 0;  // Not implemented
}

static void  id_000013_mem_unmap(thandle_t, void*, toff_t) {
    // Nothing to do
}

// Custom error handler
static void  id_000013_error_handler(const char* module, const char* fmt, va_list args) {
    // Empty handler to suppress errors during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data
    
    // Initialize memory handle
    mem_handle mh = {data, size, 0};
    
    // Set error handler
    TIFFErrorHandler old_handler = TIFFSetErrorHandler( id_000013_error_handler);
    
    // Create TIFFOpenOptions
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
	return 0;
	}
	
    
    // Open TIFF file using memory-based callbacks
    TIFF* tif = TIFFClientOpenExt(
        fuzz_str_1, 
        fuzz_str_2, 
        reinterpret_cast<thandle_t>(&mh),
         id_000013_mem_read,
         id_000013_mem_write,
         id_000013_mem_seek,
         id_000013_mem_close,
         id_000013_mem_size,
         id_000013_mem_map,
         id_000013_mem_unmap,
        opts
    );
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        TIFFSetErrorHandler(old_handler);
        return 0;
    }
    
    // Use various TIFF APIs as required
    int defer_result = TIFFDeferStrileArrayWriting(tif);
    
    // Check if TIFF is tiled
    int is_tiled = TIFFIsTiled(tif);
    
    if (is_tiled) {
        // Get tile size
        tmsize_t tile_size = TIFFTileSize(tif);
        
        if (tile_size > 0) {
            // Allocate buffer for tile reading
            void* tile_buf = _TIFFmalloc(tile_size);
            if (tile_buf) {
                // Try to read first tile
                tmsize_t read_size = TIFFReadRawTile(
                    tif, 
                    fuzz_uint32_t_3,  // First tile
                    tile_buf, 
                    tile_size
                );
                _TIFFfree(tile_buf);
            }
            
            // Get tile size for specific number of rows
            uint64_t v_tile_size = TIFFVTileSize64(tif, fuzz_uint32_t_4);
            
            // Get strile offset with error checking
            int err = 0;
            uint64_t strile_offset = TIFFGetStrileOffsetWithErr(tif, fuzz_uint32_t_5, &err);
        }
    }
    
    // Get unmap file procedure
    TIFFUnmapFileProc unmap_proc = TIFFGetUnmapFileProc(tif);
    
    // Trigger error for testing
    TIFFErrorExt(reinterpret_cast<thandle_t>(&mh), fuzz_str_6, fuzz_str_7);
    
    // Cleanup
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    TIFFSetErrorHandler(old_handler);
    
    return 0;
}