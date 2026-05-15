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
//<ID> 575
//<Prompt> []
/*<Combination>: [
*/
//<score> 195, nr_unique_branch: 78
//<Quality> {"density":13,"unique_branches":{"_TIFFReadEncodedTileAndAllocBuffer":[[1102,13,1102,47,0,0,4,0],[1103,13,1103,66,0,0,4,0]],"LogLuv32fromXYZ":[[1214,9,1214,12,0,0,4,1],[1214,16,1214,23,0,0,4,1],[1224,9,1224,16,0,0,4,1],[1228,9,1228,17,0,0,4,1],[1230,9,1230,16,0,0,4,1],[1234,9,1234,17,0,0,4,1]],"LogL16fromY":[[818,9,818,26,0,0,4,1],[820,9,820,27,0,0,4,1],[822,9,822,26,0,0,4,0]],"XYZtoRGB24":[[874,24,874,33,0,0,4,1],[874,40,874,49,0,0,4,1],[875,24,875,33,0,0,4,1],[875,40,875,49,0,0,4,1],[876,24,876,33,0,0,4,1],[876,40,876,49,0,0,4,1]],"TIFFReadRGBATileExt":[[3512,9,3512,26,0,0,4,1],[3521,9,3521,24,0,0,4,1],[3521,28,3521,43,0,0,4,1],[3528,9,3528,32,0,0,4,1],[3528,36,3528,59,0,0,4,1],[3540,9,3540,60,0,0,4,0],[3540,9,3540,60,0,0,4,1],[3546,9,3546,25,0,0,4,1],[3546,29,3546,46,0,0,4,1],[3561,9,3561,38,0,0,4,0],[3561,9,3561,38,0,0,4,1],[3566,9,3566,37,0,0,4,0],[3566,9,3566,37,0,0,4,1],[3590,9,3590,33,0,0,4,0],[3590,9,3590,33,0,0,4,1],[3590,37,3590,61,0,0,4,0],[3590,37,3590,61,0,0,4,1],[3593,21,3593,39,0,0,4,0],[3593,21,3593,39,0,0,4,1],[3603,30,3603,48,0,0,4,0],[3603,30,3603,48,0,0,4,1]],"TIFFRGBAImageOK":[[110,9,110,51,0,0,4,0]],"setorientation":[[683,9,683,34,0,0,4,0],[696,9,696,34,0,0,4,0]],"TIFFCheckpointDirectory":[[296,9,296,46,0,0,4,0],[296,9,296,46,0,0,4,1]],"Fax3TryG3WithoutEOL":[[282,22,282,62,1,0,4,0],[282,22,282,62,2,0,4,0]],"TIFFSetClientInfo":[[87,12,87,26,0,0,4,1],[90,9,90,23,0,0,4,1]],"Fax4Decode":[[1685,17,1685,34,0,0,4,1],[572,33,572,40,3,0,4,0]],"TIFFCleanup":[[58,12,58,31,0,0,4,0]],"gtTileSeparate":[[930,9,930,29,0,0,4,0],[930,33,930,71,0,0,4,0],[945,33,945,38,0,0,4,0],[946,9,946,21,0,0,4,1],[955,9,955,31,0,0,4,0],[957,13,957,40,0,0,4,1],[980,9,980,36,0,0,4,0],[981,9,981,36,0,0,4,1],[982,9,982,33,0,0,4,1],[986,9,986,16,0,0,4,1],[991,9,991,16,0,0,4,1],[991,20,991,27,0,0,4,1],[1003,9,1003,27,0,0,4,1],[1003,31,1003,49,0,0,4,1],[1010,19,1010,27,0,0,4,0],[1010,19,1010,27,0,0,4,1],[1010,31,1010,38,0,0,4,0],[1013,17,1013,37,0,0,4,0],[1020,16,1020,28,0,0,4,0],[1022,17,1022,28,0,0,4,0],[1024,21,1027,45,0,0,4,0],[1028,22,1028,33,0,0,4,0],[1111,25,1111,49,0,0,4,0],[1115,9,1115,33,0,0,4,1],[339,24,339,33,1,0,4,0],[339,24,339,33,1,0,4,1]],"gtTileContig":[[843,17,843,39,0,0,4,0],[339,24,339,33,1,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFSetClientInfo","TIFFGetField","TIFFGetField","TIFFIsTiled","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFCheckTile","TIFFReadRGBATile","LogLuv32fromXYZ","XYZtoRGB24","TIFFCheckpointDirectory","_TIFFfree","LogLuv32fromXYZ","XYZtoRGB24","TIFFCheckpointDirectory","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFSetClientInfo","TIFFGetField","TIFFGetField","TIFFIsTiled","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFCheckTile","TIFFReadRGBATile","LogLuv32fromXYZ","XYZtoRGB24","TIFFCheckpointDirectory","_TIFFfree","TIFFClose"],"visited":0}
/**/


// Include necessary headers for libtiff

// Custom memory buffer structure for TIFF client info
struct ClientInfo {
    uint8_t* buffer;
    size_t size;
    size_t position;
};

// TIFF I/O functions for memory buffer
static tmsize_t  id_000018_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    ClientInfo* info = static_cast<ClientInfo*>(handle);
    tmsize_t to_read = size;
    
    if (info->position + to_read > info->size) {
        to_read = info->size - info->position;
    }
    
    if (to_read > 0) {
        memcpy(buf, info->buffer + info->position, to_read);
        info->position += to_read;
    }
    
    return to_read;
}

static tmsize_t  id_000018_mem_write(thandle_t handle, void* buf, tmsize_t size) {
    // Not used for reading
    return 0;
}

static toff_t  id_000018_mem_seek(thandle_t handle, toff_t offset, int whence) {
    ClientInfo* info = static_cast<ClientInfo*>(handle);
    
    switch (whence) {
        case SEEK_SET:
            info->position = (offset < info->size) ? offset : info->size;
            break;
        case SEEK_CUR:
            info->position = (info->position + offset < info->size) ? 
                            info->position + offset : info->size;
            break;
        case SEEK_END:
            info->position = (info->size + offset < info->size) ? 
                            info->size + offset : info->size;
            break;
    }
    
    return info->position;
}

static int  id_000018_mem_close(thandle_t handle) {
    // Don't free the buffer here - it's managed by the fuzzer
    return 0;
}

static toff_t  id_000018_mem_size(thandle_t handle) {
    ClientInfo* info = static_cast<ClientInfo*>(handle);
    return info->size;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    // Early exit if we don't have enough data
    if (size < 100) {
        return 0;
    }
    
    TIFF* tif = nullptr;
    ClientInfo client_info;
    
    // Initialize client info with input data
    client_info.buffer = const_cast<uint8_t*>(data);
    client_info.size = size;
    client_info.position = 0;
    
    // Create TIFF handle from memory buffer
    tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                         reinterpret_cast<thandle_t>(&client_info),
                          id_000018_mem_read,  id_000018_mem_write,  id_000018_mem_seek,  id_000018_mem_close,
                          id_000018_mem_size, nullptr, nullptr);
    
    if (!tif) {
        return 0;
    }
    
    // Set client info for the TIFF handle
    const char* client_name = "fuzz_client";
    TIFFSetClientInfo(tif, &client_info, fuzz_str_3);
    
    // Get image dimensions
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Only proceed if we have valid dimensions
    if (width > 0 && height > 0) {
        // Check if the image is tiled
        int is_tiled = TIFFIsTiled(tif);
        
        if (is_tiled) {
            // Get tile dimensions
            uint32_t tile_width = 0, tile_height = 0;
            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
            
            if (tile_width > 0 && tile_height > 0) {
                // Allocate buffer for RGBA tile
                uint32_t* rgba_buffer = static_cast<uint32_t*>(
                    _TIFFmalloc(tile_width * tile_height * sizeof(uint32_t)));
                
                if (rgba_buffer) {
                    // Try to read the first tile
                    uint32_t tile_x = 0;
                    uint32_t tile_y = 0;
                    
                    // Check if tile coordinates are valid
                    if (TIFFCheckTile(tif, fuzz_uint32_t_4, fuzz_uint32_t_5, fuzz_uint32_t_6, fuzz_uint16_t_7)) {
                        // Read RGBA tile
                        int result = TIFFReadRGBATile(tif, fuzz_uint32_t_8, fuzz_uint32_t_9, rgba_buffer);
                        
                        // If we successfully read a tile, perform color conversions
                        if (result == 1) {
                            // Create sample XYZ color values
                            float xyz[3];
                            xyz[0] = 0.412453f;  // X
                            xyz[1] = 0.357580f;  // Y  
                            xyz[2] = 0.180423f;  // Z
                            
                            // Convert XYZ to LogLuv32
                            uint32_t logluv = LogLuv32fromXYZ(xyz, fuzz_int32_t_10);
                            
                            // Convert XYZ to RGB24
                            uint8_t rgb24[3];
                            XYZtoRGB24(xyz, rgb24);
                            
                            // Checkpoint the directory
                            TIFFCheckpointDirectory(tif);
                        }
                    }
                    
                    _TIFFfree(rgba_buffer);
                }
            }
        } else {
            // For non-tiled images, we can still use other APIs
            // Create sample XYZ color values
            float xyz[3];
            xyz[0] = 0.412453f;
            xyz[1] = 0.357580f;
            xyz[2] = 0.180423f;
            
            // Convert XYZ to LogLuv32
            uint32_t logluv = LogLuv32fromXYZ(xyz, fuzz_int32_t_11);
            
            // Convert XYZ to RGB24
            uint8_t rgb24[3];
            XYZtoRGB24(xyz, rgb24);
            
            // Checkpoint the directory
            TIFFCheckpointDirectory(tif);
        }
    }
    
    // Clean up
    TIFFClose(tif);
    
    return 0;
}