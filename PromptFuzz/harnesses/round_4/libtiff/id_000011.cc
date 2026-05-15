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
//<ID> 352
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","_TIFFmalloc","TIFFOpenOptionsFree","TIFFClientOpenExt","TIFFIsBigTIFF","TIFFNumberOfStrips","TIFFStripSize64","TIFFStripSize","TIFFRasterScanlineSize64","TIFFCurrentRow","TIFFSwabLong","TIFFSwabFloat","TIFFSwabArrayOfLong8","TIFFClose","_TIFFfree","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","_TIFFmalloc","TIFFClientOpenExt","TIFFIsBigTIFF","TIFFNumberOfStrips","TIFFStripSize64","TIFFStripSize","TIFFRasterScanlineSize64","TIFFCurrentRow","TIFFSwabLong","TIFFSwabFloat","TIFFSwabArrayOfLong8","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


// Custom memory I/O callbacks for TIFFClientOpenExt
struct mem_io_handle {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

static tmsize_t  id_000011_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_io_handle* h = reinterpret_cast<mem_io_handle*>(handle);
    tmsize_t to_read = size;
    if (h->pos + to_read > h->size) {
        to_read = h->size - h->pos;
    }
    if (to_read > 0) {
        memcpy(buf, h->data + h->pos, to_read);
        h->pos += to_read;
    }
    return to_read;
}

static tmsize_t  id_000011_mem_write(thandle_t, void*, tmsize_t) {
    return 0; // Read-only
}

static toff_t  id_000011_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_io_handle* h = reinterpret_cast<mem_io_handle*>(handle);
    switch (whence) {
        case SEEK_SET:
            h->pos = offset;
            break;
        case SEEK_CUR:
            h->pos += offset;
            break;
        case SEEK_END:
            h->pos = h->size + offset;
            break;
    }
    return h->pos;
}

static int  id_000011_mem_close(thandle_t handle) {
    _TIFFfree(handle);
    return 0;
}

static toff_t  id_000011_mem_size(thandle_t handle) {
    mem_io_handle* h = reinterpret_cast<mem_io_handle*>(handle);
    return h->size;
}

static int  id_000011_mem_map(thandle_t handle, void** base, toff_t* size) {
    mem_io_handle* h = reinterpret_cast<mem_io_handle*>(handle);
    *base = const_cast<void*>(reinterpret_cast<const void*>(h->data));
    *size = h->size;
    return 0;
}

static void  id_000011_mem_unmap(thandle_t, void*, toff_t) {}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Minimal valid TIFF size

    // 1. Allocate and configure TIFFOpenOptions
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
	return 0;
	}
	
    
    // Set memory allocation limits
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, 100 * 1024 * 1024); // 100MB
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, 10 * 1024 * 1024); // 10MB

    // 2. Set up memory I/O handle
    mem_io_handle* handle = reinterpret_cast<mem_io_handle*>(_TIFFmalloc(sizeof(mem_io_handle)));
    if (!handle) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }
    handle->data = data;
    handle->size = size;
    handle->pos = 0;

    // 3. Open TIFF from memory
    TIFF* tif = TIFFClientOpenExt(
        "memory", fuzz_str_1,
        reinterpret_cast<thandle_t>(handle),
         id_000011_mem_read,  id_000011_mem_write,
         id_000011_mem_seek,  id_000011_mem_close,
         id_000011_mem_size,  id_000011_mem_map,  id_000011_mem_unmap,
        opts
    );
    
    if (tif) {
        // 4. Check if it's BigTIFF
        int is_big = TIFFIsBigTIFF(tif);
        (void)is_big; // Prevent unused variable warning
        
        // 5. Get strip information
        uint32_t strip_count = TIFFNumberOfStrips(tif);
        if (strip_count > 0) {
            // Use TIFFStripSize64
            uint64_t strip_size64 = TIFFStripSize64(tif);
            (void)strip_size64;
            
            // Use TIFFRawStripSize (if available - using similar function)
            // Note: TIFFRawStripSize not in provided list, using TIFFStripSize instead
            tmsize_t strip_size = TIFFStripSize(tif);
            (void)strip_size;
        }
        
        // 6. Get raster scanline size
        uint64_t raster_size = TIFFRasterScanlineSize64(tif);
        (void)raster_size;
        
        // 7. Get current row
        uint32_t current_row = TIFFCurrentRow(tif);
        (void)current_row;
        
        // 8. Test byte swapping functions
        uint32_t long_val = 0x12345678;
        TIFFSwabLong(&long_val);
        
        float float_val = 3.14159f;
        TIFFSwabFloat(&float_val);
        
        uint64_t long8_arr[4] = {0x1234567890ABCDEF, 0xFEDCBA0987654321};
        TIFFSwabArrayOfLong8(long8_arr, 2);
        
        // 9. Clean up TIFF
        TIFFClose(tif);
    } else {
        // Free handle if TIFF opening failed
        _TIFFfree(handle);
    }
    
    // 10. Free options
    TIFFOpenOptionsFree(opts);
    
    return 0;
}