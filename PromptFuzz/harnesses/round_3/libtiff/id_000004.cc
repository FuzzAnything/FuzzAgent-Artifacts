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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 140, nr_unique_branch: 38
//<Quality> {"density":10,"unique_branches":{"TIFFReadDirEntryCheckedRational":[[3493,13,3493,39,0,0,4,0]],"TIFFReadDirEntryCheckedDouble":[[3616,13,3616,39,0,0,4,0]],"TIFFReadDirEntryByteArray":[[1503,21,1503,47,0,0,4,0]],"TIFFReadDirEntryFloatArray":[[2876,21,2876,47,0,0,4,0],[2891,21,2891,47,0,0,4,0],[2906,21,2906,47,0,0,4,0],[2921,21,2921,47,0,0,4,0]],"TIFFReadDirEntryLongArray":[[2126,9,2126,24,0,0,4,0],[2129,9,2129,22,0,0,4,0],[2130,9,2130,23,0,0,4,0],[2149,9,2149,24,0,0,4,0],[2154,25,2154,34,0,0,4,0],[2154,25,2154,34,0,0,4,1],[2156,21,2156,47,0,0,4,0],[2159,21,2159,49,0,0,4,0],[2159,21,2159,49,0,0,4,1],[2216,21,2216,47,0,0,4,0],[2231,21,2231,47,0,0,4,0]],"TIFFReadDirEntryCheckedSrational":[[3526,13,3526,39,0,0,4,0]],"_TIFFSetDefaultPostDecode":[[157,18,157,44,0,0,4,0]],"TIFFReadDirEntryCheckedFloat":[[3602,9,3602,35,0,0,4,0]],"TIFFReadDirEntryDoubleArray":[[3116,21,3116,47,0,0,4,0],[3131,21,3131,47,0,0,4,0],[3161,21,3161,47,0,0,4,0],[3251,17,3251,43,0,0,4,0]],"TIFFFieldSetGetSize":[[764,9,764,35,0,0,4,0]],"TIFFFetchNormalTag":[[6661,17,6661,65,0,0,4,1],[6673,21,6673,49,0,0,4,1],[6731,17,6731,65,0,0,4,1],[6743,21,6743,49,0,0,4,0],[6743,21,6743,49,0,0,4,1],[6745,25,6745,61,0,0,4,1],[6753,25,6753,34,0,0,4,0],[6755,25,6755,27,0,0,4,1]],"TIFFReadDirEntryLong8ArrayWithLimit":[[2562,21,2562,47,0,0,4,0]],"TIFFReadDirEntryByte":[[331,9,331,25,0,0,4,0],[336,17,336,45,0,0,4,0],[336,17,336,45,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFClientOpen","TIFFOpenOptionsFree","TIFFCreateEXIFDirectory","TIFFClose","TIFFOpenOptionsFree","TIFFClientOpen","TIFFClose","TIFFOpenOptionsFree","_TIFFmalloc","TIFFCurrentTile","TIFFGetField","TIFFGetField","TIFFReadRGBATileExt","TIFFTileSize","TIFFWriteTile","_TIFFfree","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxSingleMemAlloc","TIFFClientOpen","TIFFCreateEXIFDirectory","TIFFClientOpen","_TIFFmalloc","TIFFCurrentTile","TIFFGetField","TIFFGetField","TIFFReadRGBATileExt","TIFFTileSize","TIFFWriteTile","_TIFFfree","TIFFClose","TIFFClose","TIFFOpenOptionsFree"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs:

*/


// Include libtiff headers
extern "C" {
}

// Define custom I/O functions for memory-based TIFF handling
static tmsize_t  id_000004_memReadProc(thandle_t handle, void* buf, tmsize_t size) {
    auto* mem = reinterpret_cast<std::vector<uint8_t>*>(handle);
    tmsize_t to_read = std::min(size, static_cast<tmsize_t>(mem->size()));
    if (to_read > 0) {
        memcpy(buf, mem->data(), to_read);
        mem->erase(mem->begin(), mem->begin() + to_read);
    }
    return to_read;
}

static tmsize_t  id_000004_memWriteProc(thandle_t handle, void* buf, tmsize_t size) {
    auto* mem = reinterpret_cast<std::vector<uint8_t>*>(handle);
    size_t old_size = mem->size();
    mem->resize(old_size + size);
    memcpy(mem->data() + old_size, buf, size);
    return size;
}

static toff_t  id_000004_memSeekProc(thandle_t handle, toff_t off, int whence) {
    auto* mem = reinterpret_cast<std::vector<uint8_t>*>(handle);
    switch (whence) {
        case SEEK_SET:
            if (off < 0 || off > static_cast<toff_t>(mem->size())) {
                return static_cast<toff_t>(-1);
            }
            mem->erase(mem->begin(), mem->begin() + off);
            break;
        case SEEK_CUR:
            if (off < 0 || off > static_cast<toff_t>(mem->size())) {
                return static_cast<toff_t>(-1);
            }
            mem->erase(mem->begin(), mem->begin() + off);
            break;
        case SEEK_END:
            mem->clear();
            break;
    }
    return static_cast<toff_t>(mem->size());
}

static int  id_000004_memCloseProc(thandle_t handle) {
    // Nothing to do for memory buffer
    return 0;
}

static toff_t  id_000004_memSizeProc(thandle_t handle) {
    auto* mem = reinterpret_cast<std::vector<uint8_t>*>(handle);
    return static_cast<toff_t>(mem->size());
}

static int  id_000004_memMapProc(thandle_t, void** base, toff_t* size) {
    return 0; // Not implemented
}

static void  id_000004_memUnmapProc(thandle_t, void* base, toff_t size) {
    // Not implemented
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<49) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need minimum data
    }

    // Create input memory buffer
    std::vector<uint8_t> input_buffer(data, data + size);
    std::vector<uint8_t> output_buffer;
    
    // 1. Create TIFFOpenOptions and set max single memory allocation
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }
    
    // Use a portion of input data to determine max memory allocation
    tmsize_t max_alloc = static_cast<tmsize_t>(data[0] * 1024);
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_alloc);
    
    // 2. Create input TIFF from memory buffer
    TIFF* tif_in = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
        reinterpret_cast<thandle_t>(&input_buffer),
         id_000004_memReadProc,  id_000004_memWriteProc,
         id_000004_memSeekProc,  id_000004_memCloseProc,
         id_000004_memSizeProc,  id_000004_memMapProc,  id_000004_memUnmapProc);
    
    if (!tif_in) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }
    
    // 3. Create EXIF directory
    int exif_result = TIFFCreateEXIFDirectory(tif_in);
    
    // 4. Create output TIFF file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        TIFFClose(tif_in);
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&out_file);
	return 0;
    }
    
    int fd_out = fuzz_fileno(out_file);
    TIFF* tif_out = TIFFClientOpen(fuzz_str_3, fuzz_str_4, 
        reinterpret_cast<thandle_t>(fd_out),
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    
    if (!tif_out) {
        assert_file_closed(&out_file);;
        TIFFClose(tif_in);
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&out_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    
    // 5. Set up write buffer using TIFFWriteBufferSetup
    // Allocate buffer from input data
    size_t buffer_size = std::min(size / 4, static_cast<size_t>(1024));
    void* write_buffer = _TIFFmalloc(static_cast<tmsize_t>(buffer_size));
    if (write_buffer) {
        // Copy some input data to the buffer
        size_t copy_size = std::min(buffer_size, size);
        memcpy(write_buffer, data, copy_size);
        
        // Note: TIFFWriteBufferSetup is not in the provided API list,
        // so we'll simulate buffer setup by directly manipulating TIFF
        // For this example, we'll just use the buffer for writing
    }
    
    // 6. Try to read and process tiles from input TIFF
    uint32_t current_tile = TIFFCurrentTile(tif_in);
    
    // Try to read an RGBA tile if the image supports it
    uint32_t tile_width = 0, tile_height = 0;
    if (TIFFGetField(tif_in, fuzz_uint32_t_5, &tile_width) &&
        TIFFGetField(tif_in, fuzz_uint32_t_6, &tile_height)) {
        
        // Allocate buffer for RGBA tile
        std::vector<uint32_t> rgba_buffer(tile_width * tile_height);
        
        // Try to read RGBA tile (use data from input for coordinates)
        uint32_t x = static_cast<uint32_t>(data[1]) % 100;
        uint32_t y = static_cast<uint32_t>(data[2]) % 100;
        int rgba_result = TIFFReadRGBATileExt(tif_in, x, y, rgba_buffer.data(), fuzz_int32_t_7);
        
        // 7. Write a tile to output TIFF
        if (write_buffer && tile_width > 0 && tile_height > 0) {
            // Calculate tile size
            tmsize_t tile_size = TIFFTileSize(tif_in);
            if (tile_size > 0 && tile_size <= buffer_size) {
                // Write tile using coordinates from input data
                uint32_t write_x = static_cast<uint32_t>(data[3]) % 100;
                uint32_t write_y = static_cast<uint32_t>(data[4]) % 100;
                uint32_t write_z = static_cast<uint32_t>(data[5]) % 10;
                uint16_t write_sample = static_cast<uint16_t>(data[6]) % 4;
                
                tmsize_t write_result = TIFFWriteTile(tif_out, write_buffer, 
                    write_x, write_y, write_z, write_sample);
            }
        }
    }
    
    // 8. Clean up
    if (write_buffer) {
        _TIFFfree(write_buffer);
    }
    
    TIFFClose(tif_out);
    TIFFClose(tif_in);
    TIFFOpenOptionsFree(opts);
    
    // Close and remove output file
    assert_file_closed(&out_file);;
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	return 0;
}