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
//<ID> 1046
//<Prompt> []
/*<Combination>: [
*/
//<score> 380, nr_unique_branch: 89
//<Quality> {"density":20,"unique_branches":{"TIFFReadTile":[[961,9,961,31,0,0,4,1],[961,35,961,66,0,0,4,1]],"TIFFStartTile":[[1493,9,1493,48,0,0,4,0],[1493,9,1493,48,0,0,4,1],[1495,13,1495,42,0,0,4,0],[1495,13,1495,42,0,0,4,1],[1500,9,1500,30,0,0,4,1],[1506,9,1506,23,0,0,4,1],[1513,9,1513,23,0,0,4,1],[1520,9,1520,40,0,0,4,0],[1520,9,1520,40,0,0,4,1],[1528,13,1528,39,0,0,4,0],[314,6,314,54,3,0,4,0],[314,6,314,54,4,0,4,0]],"OJPEGSubsamplingCorrect":[[1094,10,1094,60,0,0,4,0],[1095,10,1095,61,0,0,4,0]],"TIFFCheckTile":[[75,9,75,31,0,0,4,1],[81,9,81,32,0,0,4,1],[88,9,88,31,0,0,4,1],[94,9,94,53,0,0,4,0],[94,9,94,53,0,0,4,1],[95,9,95,36,0,0,4,1]],"_TIFFFindFieldByName":[[909,9,909,28,0,0,4,0],[915,9,915,25,0,0,4,1],[927,35,927,38,0,0,4,0],[66,21,66,40,1,0,4,1]],"DumpModeDecode":[[77,9,77,28,0,0,4,0]],"TIFFReadRawTile1":[[1141,9,1141,18,0,0,4,0],[1145,13,1145,20,0,0,4,0],[1145,13,1145,20,0,0,4,1],[1154,13,1154,23,0,0,4,0]],"TIFFFillTile":[[1238,9,1238,47,0,0,4,0],[1238,9,1238,47,0,0,4,1],[1241,13,1241,27,0,0,4,0],[1241,13,1241,27,0,0,4,1],[1241,31,1241,67,0,0,4,1],[1252,13,1252,36,0,0,4,0],[1252,13,1252,36,0,0,4,1],[1257,17,1257,31,0,0,4,0],[1257,35,1257,80,0,0,4,0],[1326,17,1326,50,0,0,4,1],[1331,17,1331,50,0,0,4,0],[1331,17,1331,50,0,0,4,1],[1334,21,1334,58,0,0,4,1],[1342,17,1342,49,0,0,4,1],[1365,21,1366,31,0,0,4,0],[1365,21,1366,31,0,0,4,1],[1375,17,1375,41,0,0,4,0],[1376,17,1376,29,0,0,4,0],[1376,17,1376,29,0,0,4,1],[1377,17,1377,54,0,0,4,0],[283,23,283,62,3,0,4,1],[283,23,283,62,5,0,4,1],[283,23,283,62,16,0,4,1]],"TIFFFieldWithName":[[944,9,944,13,0,0,4,1]],"OJPEGReadHeaderInfo":[[282,22,282,62,1,0,4,0]],"TIFFReadEncodedTile":[[977,9,977,31,0,0,4,1],[979,9,979,31,0,0,4,1],[988,9,988,47,0,0,4,0],[988,9,988,47,0,0,4,1],[988,51,988,73,0,0,4,0],[988,51,988,73,0,0,4,1],[989,9,989,25,0,0,4,0],[989,29,989,38,0,0,4,0],[990,9,990,49,0,0,4,0],[992,13,992,75,0,0,4,0],[1003,9,1003,31,0,0,4,0],[1003,9,1003,31,0,0,4,1],[1005,14,1005,29,0,0,4,1],[1007,9,1007,33,0,0,4,0],[1007,9,1007,33,0,0,4,1],[1010,13,1010,16,0,0,4,0],[1014,14,1015,78,0,0,4,0],[1014,14,1015,78,0,0,4,1]],"td_lfind":[[554,34,554,47,0,0,4,0],[555,13,555,34,0,0,4,0],[555,13,555,34,0,0,4,1]],"LZWPreDecode":[[265,13,265,28,0,0,4,1]],"TIFFWarningExtR":[[83,16,83,36,0,0,4,0],[89,13,89,17,0,0,4,0]],"TIFFReadAndRealloc":[[98,13,98,75,0,0,4,1],[139,17,139,25,0,0,4,1]],"tagNameCompare":[[619,9,619,12,0,0,4,0],[619,9,619,12,0,0,4,1],[622,16,622,44,0,0,4,0]],"LZWDecodeCompat":[[874,12,874,19,0,0,4,1],[893,17,893,34,0,0,4,0],[1010,9,1010,16,0,0,4,1]],"TIFFReadRawStripOrTile2":[[708,13,708,21,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFIsTiled","TIFFDefaultTileSize","TIFFNumberOfTiles","TIFFTileSize","TIFFCurrentTile","_TIFFmalloc","TIFFReadTile","TIFFReadEncodedTile","_TIFFfree","TIFFRGBAImageBegin","TIFFRGBAImageEnd","LogLuv32toXYZ","TIFFFieldWithName","TIFFFieldSetGetSize","TIFFWarningExtR","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetWarningHandlerExtR","TIFFClientOpenExt","TIFFIsTiled","TIFFDefaultTileSize","TIFFNumberOfTiles","TIFFTileSize","TIFFCurrentTile","_TIFFmalloc","TIFFReadTile","TIFFReadEncodedTile","_TIFFfree","TIFFRGBAImageBegin","TIFFRGBAImageEnd","LogLuv32toXYZ","TIFFFieldWithName","TIFFFieldSetGetSize","TIFFWarningExtR","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/**/


// Include libtiff headers

// Memory handle structure for TIFF client callbacks
struct mem_handle {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Callback functions for TIFFClientOpenExt
static tmsize_t  id_000022_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_handle* mh = (mem_handle*)handle;
    tmsize_t to_read = size;
    
    if (mh->pos + to_read > mh->size) {
        to_read = mh->size - mh->pos;
    }
    
    if (to_read > 0) {
        memcpy(buf, mh->data + mh->pos, to_read);
        mh->pos += to_read;
    }
    
    return to_read;
}

static tmsize_t  id_000022_mem_write(thandle_t handle, void* buf, tmsize_t size) {
    // Not used for reading
    return 0;
}

static toff_t  id_000022_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_handle* mh = (mem_handle*)handle;
    
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
    
    if (mh->pos > mh->size) {
        mh->pos = mh->size;
    }
    
    return mh->pos;
}

static int  id_000022_mem_close(thandle_t handle) {
    // Nothing to do for memory handle
    return 0;
}

static toff_t  id_000022_mem_size(thandle_t handle) {
    mem_handle* mh = (mem_handle*)handle;
    return mh->size;
}

static int  id_000022_mem_map(thandle_t handle, void** base, toff_t* size) {
    mem_handle* mh = (mem_handle*)handle;
    *base = (void*)mh->data;
    *size = mh->size;
    return 0;
}

static void  id_000022_mem_unmap(thandle_t handle, void* base, toff_t size) {
    // Nothing to do
}

// Warning handler callback
static void  id_000022_warning_handler(thandle_t client_data, const char* module, const char* fmt, va_list ap) {
    // Do nothing - just consume the warning
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need minimum data
    }
    
    // Create memory handle
    mem_handle mh;
    mh.data = data;
    mh.size = size;
    mh.pos = 0;
    
    // Allocate and configure open options
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }
    
    // Set warning handler
    TIFFOpenOptionsSetWarningHandlerExtR(opts, (TIFFErrorHandlerExtR) id_000022_warning_handler, NULL);
    
    // Open TIFF from memory
    TIFF* tif = TIFFClientOpenExt("memory", fuzz_str_1, (thandle_t)&mh,
                                   id_000022_mem_read,  id_000022_mem_write,
                                   id_000022_mem_seek,  id_000022_mem_close,
                                   id_000022_mem_size,  id_000022_mem_map,  id_000022_mem_unmap,
                                  opts);
    
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }
    
    // Check if image is tiled
    int is_tiled = TIFFIsTiled(tif);
    
    if (is_tiled) {
        // Get default tile size
        uint32_t tile_width = 0, tile_height = 0;
        TIFFDefaultTileSize(tif, &tile_width, &tile_height);
        
        // Get number of tiles
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        
        // Get tile size
        tmsize_t tile_size = TIFFTileSize(tif);
        
        // Get current tile
        uint32_t current_tile = TIFFCurrentTile(tif);
        
        if (num_tiles > 0 && tile_size > 0) {
            // Allocate buffer for tile data
            void* tile_buf = _TIFFmalloc(tile_size);
            if (tile_buf) {
                // Try to read first tile
                tmsize_t read_size = TIFFReadTile(tif, tile_buf, fuzz_uint32_t_2, fuzz_uint32_t_3, fuzz_uint32_t_4, fuzz_uint16_t_5);
                
                // Try encoded tile reading
                if (read_size <= 0) {
                    read_size = TIFFReadEncodedTile(tif, fuzz_uint32_t_6, tile_buf, tile_size);
                }
                
                _TIFFfree(tile_buf);
            }
        }
    }
    
    // Try RGBA image processing
    TIFFRGBAImage img;
    char emsg[1024];
    
    if (TIFFRGBAImageBegin(&img, tif, fuzz_int32_t_7, emsg)) {
        // Process would continue here in real usage
        TIFFRGBAImageEnd(&img);
    }
    
    // Test LogLuv conversion
    uint32_t logluv_pixel = 0;
    float xyz[3] = {0};
    LogLuv32toXYZ(fuzz_uint32_t_8, xyz);
    
    // Test field size function
    const TIFFField* field = TIFFFieldWithName(tif, fuzz_str_9);
    if (field) {
        int field_size = TIFFFieldSetGetSize(field);
        (void)field_size; // Use variable
    }
    
    // Emit a warning
    TIFFWarningExtR(tif, fuzz_str_10, "Fuzzing test completed");
    
    // Clean up
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    return 0;
}