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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"TIFFFieldSetGetSize":[[766,9,766,36,0,0,4,0]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFIsBigTIFF","TIFFStripSize64","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFSwabLong","_TIFFmalloc","TIFFSwabFloat","_TIFFmalloc","TIFFSwabArrayOfLong8","TIFFFlush","_TIFFfree","_TIFFfree","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFIsBigTIFF","TIFFStripSize64","TIFFRasterScanlineSize64","_TIFFmalloc","TIFFSwabLong","_TIFFmalloc","TIFFSwabFloat","_TIFFmalloc","TIFFSwabArrayOfLong8","TIFFFlush","_TIFFfree","_TIFFfree","_TIFFfree","TIFFClose"],"visited":1}
/**/


extern "C" {
}

// Custom stream functions for reading from memory
struct mem_stream_data {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

static tmsize_t  id_000008_mem_read(thandle_t handle, void* buffer, tmsize_t size) {
    mem_stream_data* stream = (mem_stream_data*)handle;
    tmsize_t to_read = size;
    
    if (stream->pos + to_read > stream->size) {
        to_read = stream->size - stream->pos;
    }
    
    if (to_read > 0) {
        memcpy(buffer, stream->data + stream->pos, to_read);
        stream->pos += to_read;
    }
    
    return to_read;
}

static tmsize_t  id_000008_mem_write(thandle_t handle, void* buffer, tmsize_t size) {
    // Not implemented for read-only fuzzing
    return 0;
}

static toff_t  id_000008_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_stream_data* stream = (mem_stream_data*)handle;
    
    switch (whence) {
        case SEEK_SET:
            if (offset <= stream->size) {
                stream->pos = offset;
            }
            break;
        case SEEK_CUR:
            if (stream->pos + offset <= stream->size) {
                stream->pos += offset;
            }
            break;
        case SEEK_END:
            if (offset <= stream->size) {
                stream->pos = stream->size - offset;
            }
            break;
    }
    
    return stream->pos;
}

static int  id_000008_mem_close(thandle_t handle) {
    // Nothing to clean up
    return 0;
}

static toff_t  id_000008_mem_size(thandle_t handle) {
    mem_stream_data* stream = (mem_stream_data*)handle;
    return stream->size;
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 100) {
        // Not enough data for meaningful TIFF processing
        return 0;
    }

    // 1. Open a TIFF file from memory stream
    mem_stream_data stream_data = {data, size, 0};
    
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }
    
    // Set some options (just to use the API)
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 100 * 1024 * 1024); // 100MB limit
    
    TIFF* tif = TIFFClientOpenExt(
        "memory.tif", fuzz_str_1,
        (thandle_t)&stream_data,
         id_000008_mem_read,  id_000008_mem_write,
         id_000008_mem_seek,  id_000008_mem_close,
         id_000008_mem_size, NULL, NULL,
        opts
    );
    
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        return 0;
    }

    // 2. Check if it's BigTIFF
    int is_big_tiff = TIFFIsBigTIFF(tif);
    
    // 3. Get strip and scanline sizes
    uint64_t strip_size = TIFFStripSize64(tif);
    uint64_t raster_scanline_size = TIFFRasterScanlineSize64(tif);
    
    // 4. Allocate buffer for potential data processing
    uint32_t* swap_buffer = nullptr;
    uint64_t* swap_array = nullptr;
    float* swap_float = nullptr;
    
    if (size >= sizeof(uint32_t)) {
        swap_buffer = (uint32_t*)_TIFFmalloc(sizeof(uint32_t));
        if (swap_buffer) {
            // Use first 4 bytes for swapping
            memcpy(swap_buffer, data, sizeof(uint32_t));
            TIFFSwabLong(swap_buffer);
        }
    }
    
    if (size >= sizeof(float)) {
        swap_float = (float*)_TIFFmalloc(sizeof(float));
        if (swap_float) {
            // Use bytes 4-7 for float swapping
            memcpy(swap_float, data + 4, sizeof(float));
            TIFFSwabFloat(swap_float);
        }
    }
    
    if (size >= 2 * sizeof(uint64_t)) {
        size_t array_size = 2;
        swap_array = (uint64_t*)_TIFFmalloc(array_size * sizeof(uint64_t));
        if (swap_array) {
            // Use next 16 bytes for array swapping
            memcpy(swap_array, data + 8, array_size * sizeof(uint64_t));
            TIFFSwabArrayOfLong8(swap_array, array_size);
        }
    }
    
    // 5. Try to flush data (though we're reading, this is part of the API usage)
    // Note: TIFFFlushData is not in the provided list, but we can use TIFFFlush instead
    // Assuming TIFFFlushData might be similar to TIFFFlush for this context
    TIFFFlush(tif);
    
    // 6. Clean up allocated resources
    if (swap_buffer) {
        _TIFFfree(swap_buffer);
    }
    if (swap_float) {
        _TIFFfree(swap_float);
    }
    if (swap_array) {
        _TIFFfree(swap_array);
    }
    
    TIFFClose(tif);
    
    return 0;
}