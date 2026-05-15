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
//<ID> 691
//<Prompt> []
/*<Combination>: [
*/
//<score> 42.5, nr_unique_branch: 10
//<Quality> {"density":17,"unique_branches":{"TIFFReadAndRealloc":[[139,17,139,25,0,0,4,1]],"TIFFStartTile":[[1493,9,1493,48,0,0,4,1]],"TIFFFillTile":[[1365,21,1366,31,0,0,4,0],[1365,21,1366,31,0,0,4,1],[1376,17,1376,29,0,0,4,1],[283,23,283,62,3,0,4,1],[283,23,283,62,5,0,4,1],[283,23,283,62,16,0,4,1]],"TIFFReadRawStripOrTile2":[[706,9,706,16,0,0,4,0],[708,13,708,21,0,0,4,1]]},"library_calls":["TIFFClientOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFFieldName","TIFFGetMapFileProc","TIFFTileSize","_TIFFmalloc","TIFFReadTile","_TIFFfree","TIFFSwabArrayOfLong8","LogLuv24toXYZ","TIFFFindCODEC","TIFFUnRegisterCODEC","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFFieldName","TIFFGetMapFileProc","TIFFTileSize","_TIFFmalloc","TIFFReadTile","_TIFFfree","TIFFSwabArrayOfLong8","LogLuv24toXYZ","TIFFFindCODEC","TIFFUnRegisterCODEC","TIFFClose"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to process TIFF data from memory:

*/


// Include necessary libtiff headers
extern "C" {
}

// Custom memory buffer structure for TIFFClientOpen
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Custom I/O functions for memory buffer
static tmsize_t  id_000014_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_buffer* buffer = (mem_buffer*)handle;
    
    if (buffer->pos >= buffer->size) {
        return 0;
    }
    
    tmsize_t to_read = size;
    if (buffer->pos + to_read > buffer->size) {
        to_read = buffer->size - buffer->pos;
    }
    
    memcpy(buf, buffer->data + buffer->pos, to_read);
    buffer->pos += to_read;
    
    return to_read;
}

static tmsize_t  id_000014_mem_write(thandle_t, void*, tmsize_t) {
    return 0; // Read-only
}

static toff_t  id_000014_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_buffer* buffer = (mem_buffer*)handle;
    
    switch (whence) {
        case SEEK_SET:
            buffer->pos = offset;
            break;
        case SEEK_CUR:
            buffer->pos += offset;
            break;
        case SEEK_END:
            buffer->pos = buffer->size + offset;
            break;
        default:
            return -1;
    }
    
    if (buffer->pos > buffer->size) {
        buffer->pos = buffer->size;
    }
    
    return buffer->pos;
}

static int  id_000014_mem_close(thandle_t handle) {
    // Nothing to clean up since we don't allocate memory for the buffer
    return 0;
}

static toff_t  id_000014_mem_size(thandle_t handle) {
    mem_buffer* buffer = (mem_buffer*)handle;
    return buffer->size;
}

static int  id_000014_mem_map(thandle_t, void**, toff_t*) {
    return 0; // Not implemented
}

static void  id_000014_mem_unmap(thandle_t, void*, toff_t) {
    // Nothing to do
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Initialize memory buffer
    mem_buffer buffer = {data, size, 0};
    
    // Open TIFF from memory using TIFFClientOpen
    TIFF* tif = TIFFClientOpen("memory", "r", 
                               (thandle_t)&buffer,
                                id_000014_mem_read,  id_000014_mem_write,
                                id_000014_mem_seek,  id_000014_mem_close,
                                id_000014_mem_size,  id_000014_mem_map,  id_000014_mem_unmap);
    
    if (!tif) {
        return 0;
    }

    // Get number of directories using TIFFNumberOfDirectories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    // Process each directory
    for (tdir_t dir = 0; dir < num_dirs; dir++) {
        TIFFSetDirectory(tif, dir);
        
        // Check if image is tiled using TIFFIsTiled
        int is_tiled = TIFFIsTiled(tif);
        
        // Try to get common fields using TIFFGetField
        uint32_t width = 0, height = 0;
        uint16_t samples_per_pixel = 0, bits_per_sample = 0;
        uint16_t photometric = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
        TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
        
        // Look up field information using TIFFFieldWithTag and TIFFFieldName
        const TIFFField* field = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
        if (field) {
            const char* field_name = TIFFFieldName(field);
            // field_name could be used for logging/debugging
        }
        
        // Get map file proc using TIFFGetMapFileProc
        TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
        
        // Allocate buffer using _TIFFmalloc
        uint32_t* tile_buffer = nullptr;
        if (is_tiled && width > 0 && height > 0) {
            tmsize_t tile_size = TIFFTileSize(tif);
            if (tile_size > 0) {
                tile_buffer = (uint32_t*)_TIFFmalloc(tile_size);
                if (tile_buffer) {
                    // Try to read a tile
                    TIFFReadTile(tif, tile_buffer, 0, 0, 0, 0);
                    _TIFFfree(tile_buffer);
                }
            }
        }
        
        // Test TIFFSwabArrayOfLong8 if we have enough data
        if (size >= 16) {
            uint64_t long8_array[2];
            memcpy(long8_array, data, 16);
            TIFFSwabArrayOfLong8(long8_array, 2);
        }
        
        // Test LogLuv24toXYZ if photometric indicates LogL
        if (photometric == PHOTOMETRIC_LOGL) {
            float xyz[3];
            // Use a sample LogLuv24 value
            uint32_t logluv_value = 0x123456;
            LogLuv24toXYZ(logluv_value, xyz);
        }
    }
    
    // Try to unregister a codec using TIFFUnRegisterCODEC
    TIFFCodec* codec = (TIFFCodec*)TIFFFindCODEC(COMPRESSION_NONE);
    if (codec) {
        TIFFUnRegisterCODEC(codec);
    }
    
    // Close TIFF using TIFFClose
    TIFFClose(tif);
    
    return 0;
}