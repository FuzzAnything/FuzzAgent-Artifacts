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
//<ID> 694
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["TIFFClientOpen","TIFFNumberOfDirectories","TIFFIsTiled","TIFFFieldWithTag","TIFFFieldName","TIFFGetField","_TIFFmalloc","TIFFSwabArrayOfLong8","_TIFFfree","TIFFGetField","LogLuv24toXYZ","TIFFGetMapFileProc","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFNumberOfDirectories","TIFFIsTiled","TIFFFieldWithTag","TIFFFieldName","TIFFGetField","_TIFFmalloc","TIFFSwabArrayOfLong8","_TIFFfree","TIFFGetField","LogLuv24toXYZ","TIFFGetMapFileProc","TIFFClose"],"visited":0}
/**/


// Define missing types and functions from the provided list
typedef int64_t tmsize_t;
typedef void *thandle_t;
typedef uint64_t toff_t;

// Custom memory handle structure
struct mem_handle {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Custom I/O functions for memory-based TIFF
static tmsize_t  id_000015_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_handle* mh = (mem_handle*)handle;
    tmsize_t available = mh->size - mh->pos;
    tmsize_t to_copy = size < available ? size : available;
    if (to_copy > 0) {
        memcpy(buf, mh->data + mh->pos, to_copy);
        mh->pos += to_copy;
    }
    return to_copy;
}

static tmsize_t  id_000015_mem_write(thandle_t handle, void* buf, tmsize_t size) {
    // Read-only for fuzzing
    return 0;
}

static toff_t  id_000015_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_handle* mh = (mem_handle*)handle;
    switch (whence) {
        case SEEK_SET:
            mh->pos = offset < mh->size ? offset : mh->size;
            break;
        case SEEK_CUR:
            mh->pos += offset;
            if (mh->pos > mh->size) mh->pos = mh->size;
            break;
        case SEEK_END:
            mh->pos = mh->size;
            break;
    }
    return mh->pos;
}

static int  id_000015_mem_close(thandle_t handle) {
    // Nothing to clean up
    return 0;
}

static toff_t  id_000015_mem_size(thandle_t handle) {
    mem_handle* mh = (mem_handle*)handle;
    return mh->size;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data

    // 1. Create memory handle for TIFFClientOpen
    mem_handle mh = {data, size, 0};
    
    // 2. Open TIFF from memory using TIFFClientOpen
    TIFF* tif = TIFFClientOpen("memory", "r", (thandle_t)&mh,
                                id_000015_mem_read,  id_000015_mem_write,
                                id_000015_mem_seek,  id_000015_mem_close,
                                id_000015_mem_size, NULL, NULL);
    if (!tif) {
	return 0;
	}
	

    // 3. Get number of directories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    // 4. Check if image is tiled
    int is_tiled = TIFFIsTiled(tif);
    
    // 5. Try to get field information
    const TIFFField* field = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
    if (field) {
        const char* field_name = TIFFFieldName(field);
        // field_name could be used but we just need to call the function
    }
    
    // 6. Get a field value
    uint32_t width = 0;
    int got_field = TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    
    // 7. Allocate and free memory using libtiff functions
    void* allocated_mem = _TIFFmalloc(100);
    if (allocated_mem) {
        // 8. Use TIFFSwabArrayOfLong8 if we have 64-bit data
        if (size >= sizeof(uint64_t) * 2) {
            uint64_t* long8_data = (uint64_t*)allocated_mem;
            // Copy some data to process
            size_t copy_size = size < 100 ? size : 100;
            memcpy(long8_data, data, copy_size);
            TIFFSwabArrayOfLong8(long8_data, copy_size / sizeof(uint64_t));
        }
        _TIFFfree(allocated_mem);
    }
    
    // 9. LogLuv conversion (if image uses LogLuv encoding)
    uint16_t photometric = 0;
    if (TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric)) {
        if (photometric == PHOTOMETRIC_LOGLUV) {
            float xyz[3];
            // Use some data for LogLuv conversion
            uint32_t logluv_pixel = 0;
            if (size >= sizeof(uint32_t)) {
                memcpy(&logluv_pixel, data, sizeof(uint32_t));
                LogLuv24toXYZ(logluv_pixel, xyz);
            }
        }
    }
    
    // 10. Get map file procedure (though we use memory)
    TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
    
    // 11. Try to unregister a codec (use with caution)
    // We'll try to find and unregister a dummy codec
    // This is just to demonstrate the API call
    TIFFCodec* codec = NULL;
    // We can't actually unregister without a valid codec
    
    // 12. Clean up
    TIFFClose(tif);
    
    return 0;
}