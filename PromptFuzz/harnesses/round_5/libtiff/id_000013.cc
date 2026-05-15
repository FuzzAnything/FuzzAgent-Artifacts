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
//<ID> 690
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["_TIFFmalloc","TIFFClientOpen","_TIFFfree","TIFFIsTiled","TIFFNumberOfDirectories","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFFieldName","TIFFGetMapFileProc","_TIFFmalloc","_TIFFfree","_TIFFmalloc","TIFFSwabArrayOfLong8","_TIFFfree","LogLuv24toXYZ","_TIFFmalloc","TIFFUnRegisterCODEC","_TIFFfree","TIFFClose"],"critical_calls":["_TIFFmalloc","TIFFClientOpen","TIFFIsTiled","TIFFNumberOfDirectories","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFFieldName","TIFFGetMapFileProc","_TIFFmalloc","_TIFFfree","_TIFFmalloc","TIFFSwabArrayOfLong8","_TIFFfree","LogLuv24toXYZ","_TIFFmalloc","TIFFUnRegisterCODEC","_TIFFfree","TIFFClose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to achieve reading and processing a TIFF file from memory:

*/


// Forward declarations for memory callbacks
struct mem_data {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

static tmsize_t  id_000013_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_data* md = (mem_data*)handle;
    tmsize_t to_read = size;
    
    if (md->pos + to_read > md->size) {
        to_read = md->size - md->pos;
    }
    
    if (to_read > 0) {
        memcpy(buf, md->data + md->pos, to_read);
        md->pos += to_read;
    }
    
    return to_read;
}

static tmsize_t  id_000013_mem_write(thandle_t, void*, tmsize_t) {
    // Not implemented for read-only
    return 0;
}

static toff_t  id_000013_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_data* md = (mem_data*)handle;
    
    switch (whence) {
        case SEEK_SET:
            md->pos = (offset < md->size) ? offset : md->size;
            break;
        case SEEK_CUR:
            md->pos = (md->pos + offset < md->size) ? md->pos + offset : md->size;
            break;
        case SEEK_END:
            md->pos = (md->size + offset < md->size) ? md->size + offset : md->size;
            break;
    }
    
    return md->pos;
}

static int  id_000013_mem_close(thandle_t handle) {
    mem_data* md = (mem_data*)handle;
    _TIFFfree(md);
    return 0;
}

static toff_t  id_000013_mem_size(thandle_t handle) {
    mem_data* md = (mem_data*)handle;
    return md->size;
}

static int  id_000013_mem_map(thandle_t, void**, toff_t*) {
    return 0;
}

static void  id_000013_mem_unmap(thandle_t, void*, toff_t) {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need minimum data
    }
    
    // Allocate memory for our memory buffer structure
    mem_data* md = (mem_data*)_TIFFmalloc(sizeof(mem_data));
    if (!md) {
        return 0;
    }
    
    md->data = data;
    md->size = size;
    md->pos = 0;
    
    // Open TIFF from memory using client functions
    TIFF* tif = TIFFClientOpen("memory", "r", 
                               (thandle_t)md,
                                id_000013_mem_read,
                                id_000013_mem_write,
                                id_000013_mem_seek,
                                id_000013_mem_close,
                                id_000013_mem_size,
                                id_000013_mem_map,
                                id_000013_mem_unmap);
    
    if (!tif) {
        _TIFFfree(md);
        return 0;
    }
    
    // Use TIFFIsTiled to check if the image is tiled
    int is_tiled = TIFFIsTiled(tif);
    
    // Get number of directories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    // Try to get some common fields using TIFFGetField
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, 256, &width)) { // TIFFTAG_IMAGEWIDTH
        // Field retrieved
    }
    if (TIFFGetField(tif, 257, &height)) { // TIFFTAG_IMAGELENGTH
        // Field retrieved
    }
    
    // Get field information for a specific tag
    const TIFFField* field = TIFFFieldWithTag(tif, 256); // TIFFTAG_IMAGEWIDTH
    if (field) {
        const char* field_name = TIFFFieldName(field);
        // field_name could be used, but we don't need to do anything with it
    }
    
    // Get the map file procedure (if any)
    TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
    
    // Allocate some memory using _TIFFmalloc and free it
    void* temp_buffer = _TIFFmalloc(1024);
    if (temp_buffer) {
        // Fill with some data
        memset(temp_buffer, 0, 1024);
        _TIFFfree(temp_buffer);
    }
    
    // Test TIFFSwabArrayOfLong8 on some data
    if (size >= 32) {
        uint64_t* long8_data = (uint64_t*)_TIFFmalloc(4 * sizeof(uint64_t));
        if (long8_data) {
            // Copy some input data
            size_t copy_size = (size > 4 * sizeof(uint64_t)) ? 4 * sizeof(uint64_t) : size;
            memcpy(long8_data, data, copy_size);
            
            // Test byte swapping
            TIFFSwabArrayOfLong8(long8_data, 4);
            
            _TIFFfree(long8_data);
        }
    }
    
    // Test LogLuv24toXYZ with a sample value
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    uint32_t logluv_value = 0x123456; // Sample LogLuv24 value
    LogLuv24toXYZ(logluv_value, xyz);
    
    // Try to unregister a CODEC (this might fail if the CODEC isn't registered)
    // We'll create a dummy codec structure
    TIFFCodec* dummy_codec = (TIFFCodec*)_TIFFmalloc(sizeof(TIFFCodec));
    if (dummy_codec) {
        memset(dummy_codec, 0, sizeof(TIFFCodec));
        // Note: This will likely fail since dummy_codec isn't a real registered codec
        TIFFUnRegisterCODEC(dummy_codec);
        _TIFFfree(dummy_codec);
    }
    
    // Close the TIFF file
    TIFFClose(tif);
    
    // Note:  id_000013_mem_close will free md, so we don't need to free it here
    
    return 0;
}