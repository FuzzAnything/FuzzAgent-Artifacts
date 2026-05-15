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
//<ID> 705
//<Prompt> []
/*<Combination>: [
*/
//<score> 323, nr_unique_branch: 111
//<Quality> {"density":19,"unique_branches":{"tiff_itrunc":[[792,9,792,35,0,0,4,1]],"LZWPreDecode":[[262,9,262,36,0,0,4,1]],"TIFFErrorExtR":[[110,16,110,37,0,0,4,0],[116,13,116,17,0,0,4,0]],"TIFFFetchDirectory":[[6091,9,6091,18,0,0,4,1],[6181,13,6181,50,0,0,4,0],[6181,13,6181,50,0,0,4,1],[6197,13,6197,45,0,0,4,0],[6197,13,6197,45,0,0,4,1],[6200,17,6200,26,0,0,4,1],[6200,30,6200,62,0,0,4,1],[6201,17,6201,56,0,0,4,0],[6201,17,6201,56,0,0,4,1],[6211,17,6211,43,0,0,4,0],[6211,17,6211,43,0,0,4,1],[6213,17,6213,34,0,0,4,0],[6213,17,6213,34,0,0,4,1],[6226,17,6226,26,0,0,4,1],[6226,30,6226,62,0,0,4,1],[6227,17,6227,56,0,0,4,0],[6227,17,6227,56,0,0,4,1],[6237,17,6237,43,0,0,4,0],[6239,17,6239,34,0,0,4,0],[6239,17,6239,34,0,0,4,1],[6249,13,6249,28,0,0,4,0],[6249,13,6249,28,0,0,4,1],[6260,13,6260,33,0,0,4,0],[6260,13,6260,33,0,0,4,1],[6272,13,6272,28,0,0,4,1],[6275,13,6275,22,0,0,4,1],[6275,26,6275,64,0,0,4,1],[6276,13,6276,32,0,0,4,0],[6276,13,6276,32,0,0,4,1],[6286,13,6286,23,0,0,4,0],[6289,17,6289,49,0,0,4,0],[6289,17,6289,49,0,0,4,1],[6293,21,6293,30,0,0,4,1],[6293,34,6293,66,0,0,4,1],[6294,21,6294,60,0,0,4,0],[6294,21,6294,60,0,0,4,1],[6299,21,6299,47,0,0,4,0],[6299,21,6299,47,0,0,4,1],[6306,21,6306,30,0,0,4,1],[6306,34,6306,66,0,0,4,1],[6307,21,6307,60,0,0,4,0],[6307,21,6307,60,0,0,4,1],[6312,21,6312,47,0,0,4,0]],"TIFFFillTile":[[1279,17,1279,52,0,0,4,0],[1279,17,1279,52,0,0,4,1],[1280,17,1281,56,0,0,4,0],[1280,17,1281,56,0,0,4,1],[1289,31,1289,63,0,0,4,1],[1302,17,1302,49,0,0,4,0],[1302,53,1302,69,0,0,4,1],[1352,21,1352,54,0,0,4,0],[1353,21,1353,61,0,0,4,1],[1357,21,1358,59,0,0,4,1],[283,23,283,62,3,0,4,0],[283,23,283,62,5,0,4,0],[284,29,284,60,6,0,4,0],[284,29,284,60,6,0,4,1],[283,23,283,62,16,0,4,0]],"TIFFReadBufferSetup":[[1400,9,1400,25,0,0,4,1],[1407,9,1407,11,0,0,4,1],[1416,13,1416,38,0,0,4,1],[1427,9,1427,33,0,0,4,1]],"TIFFReadDirEntryArrayWithLimit":[[1318,26,1318,60,0,0,4,0],[1318,26,1318,60,0,0,4,1],[1321,9,1321,18,0,0,4,1],[283,23,283,62,4,0,4,0],[283,23,283,62,10,0,4,0]],"LogL16toY":[[806,9,806,12,0,0,4,1],[809,13,809,28,0,0,4,0]],"TIFFReadDirEntryData":[[3981,9,3981,18,0,0,4,1],[3992,13,3992,35,0,0,4,1],[3992,39,3992,71,0,0,4,1],[3997,13,3997,41,0,0,4,0],[3997,13,3997,41,0,0,4,1]],"_TIFFRand":[[781,9,781,18,0,0,4,0]],"TIFFReadRawTile1":[[1141,9,1141,18,0,0,4,1],[1170,13,1170,75,0,0,4,1],[1171,13,1171,33,0,0,4,1],[1173,18,1173,27,0,0,4,1],[1173,31,1173,42,0,0,4,1],[1173,46,1173,66,0,0,4,1],[1177,13,1177,22,0,0,4,1]],"LogLuv32toXYZ":[[1185,9,1185,16,0,0,4,1]],"TIFFClientOpenExt":[[295,5,295,69,50,0,4,0]],"LZWDecode":[[534,9,534,17,0,0,4,0],[547,13,547,31,0,0,4,0],[607,13,607,21,0,0,4,0],[609,17,609,25,0,0,4,1],[681,14,681,32,0,0,4,0],[684,9,684,24,0,0,4,0]],"TIFFAdvanceDirectory":[[1850,13,1850,45,0,0,4,0],[1857,17,1857,42,0,0,4,1],[1857,46,1857,61,0,0,4,1],[1858,17,1858,53,0,0,4,1],[1858,57,1858,80,0,0,4,0],[1858,57,1858,80,0,0,4,1],[1867,17,1867,43,0,0,4,1],[1871,17,1871,32,0,0,4,1],[1871,36,1871,59,0,0,4,1],[1871,63,1871,78,0,0,4,1],[1872,17,1872,53,0,0,4,1],[1872,57,1872,80,0,0,4,0],[1872,57,1872,80,0,0,4,1],[1877,17,1877,28,0,0,4,1],[1880,17,1880,43,0,0,4,1],[283,23,283,62,1,0,4,0]],"PackBitsDecode":[[283,17,283,40,0,0,4,0]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetErrorHandlerExtR","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFIsTiled","TIFFGetField","TIFFFieldWithTag","TIFFFieldName","TIFFGetMapFileProc","TIFFTileSize","_TIFFmalloc","TIFFReadTile","LogL10fromY","LogLuv24toXYZ","LogLuv32toXYZ","TIFFSwabArrayOfLong8","_TIFFfree","TIFFGetField","TIFFFindCODEC","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetErrorHandlerExtR","TIFFClientOpenExt","TIFFNumberOfDirectories","TIFFSetDirectory","TIFFIsTiled","TIFFGetField","TIFFFieldWithTag","TIFFFieldName","TIFFGetMapFileProc","TIFFTileSize","_TIFFmalloc","TIFFReadTile","LogL10fromY","LogLuv24toXYZ","LogLuv32toXYZ","TIFFSwabArrayOfLong8","_TIFFfree","TIFFGetField","TIFFFindCODEC","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libtiff APIs to process TIFF data from memory:

*/


extern "C" {
}

// Memory buffer structure for TIFF I/O callbacks
struct MemBuffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// TIFF I/O callback functions
static tmsize_t  id_000016_mem_read(thandle_t handle, void* buffer, tmsize_t size) {
    MemBuffer* buf = reinterpret_cast<MemBuffer*>(handle);
    tmsize_t remaining = static_cast<tmsize_t>(buf->size - buf->pos);
    tmsize_t to_read = size < remaining ? size : remaining;
    
    if (to_read > 0) {
        memcpy(buffer, buf->data + buf->pos, to_read);
        buf->pos += to_read;
    }
    return to_read;
}

static tmsize_t  id_000016_mem_write(thandle_t handle, void* buffer, tmsize_t size) {
    // Not implemented for read-only fuzzing
    return 0;
}

static toff_t  id_000016_mem_seek(thandle_t handle, toff_t offset, int whence) {
    MemBuffer* buf = reinterpret_cast<MemBuffer*>(handle);
    
    switch (whence) {
        case SEEK_SET:
            buf->pos = (offset < buf->size) ? offset : buf->size;
            break;
        case SEEK_CUR:
            buf->pos = ((buf->pos + offset) < buf->size) ? (buf->pos + offset) : buf->size;
            break;
        case SEEK_END:
            buf->pos = buf->size;
            break;
    }
    return buf->pos;
}

static int  id_000016_mem_close(thandle_t handle) {
    // Memory buffer cleanup happens elsewhere
    return 0;
}

static toff_t  id_000016_mem_size(thandle_t handle) {
    MemBuffer* buf = reinterpret_cast<MemBuffer*>(handle);
    return static_cast<toff_t>(buf->size);
}

static int  id_000016_mem_map(thandle_t handle, void** base, toff_t* size) {
    MemBuffer* buf = reinterpret_cast<MemBuffer*>(handle);
    *base = const_cast<void*>(reinterpret_cast<const void*>(buf->data));
    *size = static_cast<toff_t>(buf->size);
    return 1;
}

static void  id_000016_mem_unmap(thandle_t handle, void* base, toff_t size) {
    // Nothing to do for memory buffer
}

// Custom error handler
static int  id_000016_error_handler(TIFF* tif, void* user_data, const char* module, 
                         const char* fmt, va_list ap) {
    // Consume errors without printing to avoid noise during fuzzing
    return 1;
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Skip very small inputs
    }
    
    // Setup memory buffer for TIFF I/O
    MemBuffer mem_buffer = {data, size, 0};
    
    // Create TIFFOpenOptions
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }
    
    // Set custom error handler
    TIFFOpenOptionsSetErrorHandlerExtR(opts,  id_000016_error_handler, nullptr);
    
    // Open TIFF from memory
    TIFF* tif = TIFFClientOpenExt("memory", "r", 
                                   reinterpret_cast<thandle_t>(&mem_buffer),
                                    id_000016_mem_read,  id_000016_mem_write,
                                    id_000016_mem_seek,  id_000016_mem_close,
                                    id_000016_mem_size,  id_000016_mem_map,  id_000016_mem_unmap,
                                   opts);
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }
    
    // Process TIFF directories
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    
    for (tdir_t dir = 0; dir < num_dirs && dir < 10; dir++) {
        if (!TIFFSetDirectory(tif, dir)) {
            continue;
        }
        
        // Check if image is tiled
        int is_tiled = TIFFIsTiled(tif);
        
        // Get image width using TIFFGetField
        uint32_t width = 0;
        if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width)) {
            width = 0;
        }
        
        // Get field by tag
        const TIFFField* field = TIFFFieldWithTag(tif, TIFFTAG_IMAGEWIDTH);
        if (field) {
            const char* field_name = TIFFFieldName(field);
            // field_name is used for lookup
        }
        
        // Get map file proc
        TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
        
        if (is_tiled && width > 0) {
            // Calculate tile size
            tmsize_t tile_size = TIFFTileSize(tif);
            
            if (tile_size > 0 && tile_size < 1024 * 1024) { // Limit allocation
                // Allocate buffer for tile data
                void* tile_buffer = _TIFFmalloc(tile_size);
                if (tile_buffer) {
                    // Read a tile
                    tmsize_t read_bytes = TIFFReadTile(tif, tile_buffer, 0, 0, 0, 0);
                    
                    // Process tile data
                    if (read_bytes > 0) {
                        // Test LogL10fromY with sample data
                        if (width > 10) {
                            double y_value = static_cast<double>(data[0]) / 255.0;
                            int log_result = LogL10fromY(y_value, 1);
                            (void)log_result; // Use result to avoid warning
                        }
                        
                        // Test LogLuv conversions if data available
                        if (size > 100) {
                            float xyz[3];
                            // Test with some sample values
                            uint32_t logluv24 = *reinterpret_cast<const uint32_t*>(data) & 0x00FFFFFF;
                            LogLuv24toXYZ(logluv24, xyz);
                            
                            uint32_t logluv32 = *reinterpret_cast<const uint32_t*>(data);
                            LogLuv32toXYZ(logluv32, xyz);
                            
                            // Test byte swapping
                            uint64_t long8_data[2] = {0x0123456789ABCDEF, 0xFEDCBA9876543210};
                            TIFFSwabArrayOfLong8(long8_data, 2);
                        }
                    }
                    
                    _TIFFfree(tile_buffer);
                }
            }
        }
    }
    
    // Test codec functions
    uint16_t compression = COMPRESSION_NONE;
    if (TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression)) {
        const TIFFCodec* codec = TIFFFindCODEC(compression);
        if (codec) {
            // Note: We won't actually unregister as it would affect subsequent tests
            // TIFFUnRegisterCODEC(const_cast<TIFFCodec*>(codec));
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    return 0;
}