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
//<ID> 1020
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":32,"unique_branches":{},"library_calls":["TIFFClientOpen","TIFFClose","TIFFClientOpen","TIFFClose","TIFFWriteCheck","TIFFClose","TIFFClose","TIFFReadDirectory","TIFFClose","TIFFClose","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFGetWriteProc","TIFFTileSize64","TIFFTileSize","TIFFClose","TIFFClose","TIFFReadTile","TIFFWriteTile","TIFFReadRGBATileExt","TIFFSetField","TIFFDefaultStripSize","TIFFCIELabToXYZ","TIFFSwabArrayOfDouble","TIFFClose","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFClientOpen","TIFFWriteCheck","TIFFReadDirectory","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFGetWriteProc","TIFFTileSize64","TIFFTileSize","TIFFReadTile","TIFFWriteTile","TIFFReadRGBATileExt","TIFFCIELabToXYZ","TIFFSwabArrayOfDouble","TIFFClose","TIFFClose"],"visited":1}
/*Here's a fuzz driver that uses the specified libtiff APIs to process TIFF data:

*/


// Custom structures for memory I/O
struct mem_handle {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// Memory read/write callbacks
static tmsize_t  id_000021_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_handle* mh = (mem_handle*)handle;
    if (mh->pos >= mh->size) return 0;
    
    tmsize_t to_read = size;
    if (mh->pos + to_read > mh->size) {
        to_read = mh->size - mh->pos;
    }
    
    memcpy(buf, mh->data + mh->pos, to_read);
    mh->pos += to_read;
    return to_read;
}

static tmsize_t  id_000021_mem_write(thandle_t handle, void* buf, tmsize_t size) {
    mem_handle* mh = (mem_handle*)handle;
    // Just track position without actual writing for memory output
    mh->pos += size;
    return size;
}

static toff_t  id_000021_mem_seek(thandle_t handle, toff_t offset, int whence) {
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
    
    if (mh->pos > mh->size) mh->pos = mh->size;
    return mh->pos;
}

static int  id_000021_mem_close(thandle_t handle) {
    delete (mem_handle*)handle;
    return 0;
}

static toff_t  id_000021_mem_size(thandle_t handle) {
    mem_handle* mh = (mem_handle*)handle;
    return mh->size;
}

// Dummy map/unmap functions
static int  id_000021_mem_map(thandle_t, void** base, toff_t* size) {
    *base = nullptr;
    *size = 0;
    return 0;
}

static void  id_000021_mem_unmap(thandle_t, void* base, toff_t size) {
    // Nothing to do
}

// File-based callbacks for output
static tmsize_t  id_000021_file_read(thandle_t fd, void* buf, tmsize_t size) {
    return fread(buf, 1, size, (FILE*)fd);
}

static tmsize_t  id_000021_file_write(thandle_t fd, void* buf, tmsize_t size) {
    return fwrite(buf, 1, size, (FILE*)fd);
}

static toff_t  id_000021_file_seek(thandle_t fd, toff_t offset, int whence) {
    fseek((FILE*)fd, offset, whence);
    return ftell((FILE*)fd);
}

static int  id_000021_file_close(thandle_t fd) {
    fclose((FILE*)fd);
    return 0;
}

static toff_t  id_000021_file_size(thandle_t fd) {
    FILE* f = (FILE*)fd;
    long current = ftell(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, current, SEEK_SET);
    return size;
}

static int  id_000021_file_map(thandle_t, void** base, toff_t* size) {
    *base = nullptr;
    *size = 0;
    return 0;
}

static void  id_000021_file_unmap(thandle_t, void* base, toff_t size) {
    // Nothing to do
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<143) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_15, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_16, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_17, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_18, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_19, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_20, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_21, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_22, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_23, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_24, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_25, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_26, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_27, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_28, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_29, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_30, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_31, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Create input TIFF from memory
    mem_handle* mh = new mem_handle{data, size, 0};
    
    TIFF* tif_in = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
        (thandle_t)mh,
         id_000021_mem_read,
         id_000021_mem_write,
         id_000021_mem_seek,
         id_000021_mem_close,
         id_000021_mem_size,
         id_000021_mem_map,
         id_000021_mem_unmap);
    
    if (!tif_in) {
        delete mh;
        return 0;
    }
    
    // 2. Create output TIFF to file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        TIFFClose(tif_in);
        assert_file_closed(&out_file);
	return 0;
    }
    
    TIFF* tif_out = TIFFClientOpen(fuzz_str_3, fuzz_str_4,
        (thandle_t)out_file,
         id_000021_file_read,
         id_000021_file_write,
         id_000021_file_seek,
         id_000021_file_close,
         id_000021_file_size,
         id_000021_file_map,
         id_000021_file_unmap);
    
    if (!tif_out) {
        assert_file_closed(&out_file);;
        TIFFClose(tif_in);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // 3. Use TIFFWriteCheck to validate output TIFF
    if (!TIFFWriteCheck(tif_out, fuzz_int32_t_6, fuzz_str_5)) {
        TIFFClose(tif_out);
        TIFFClose(tif_in);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // 4. Read directory from input TIFF
    if (!TIFFReadDirectory(tif_in)) {
        TIFFClose(tif_out);
        TIFFClose(tif_in);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // 5. Check if input is tiled
    int is_tiled = TIFFIsTiled(tif_in);
    
    // 6. Get some fields from input
    uint32_t width = 0, height = 0;
    TIFFGetField(tif_in, fuzz_uint32_t_7, &width);
    TIFFGetField(tif_in, fuzz_uint32_t_8, &height);
    
    // 7. Set fields on output using TIFFVSetField (via TIFFSetField)
    TIFFSetField(tif_out, fuzz_uint32_t_9, width);
    TIFFSetField(tif_out, fuzz_uint32_t_10, height);
    TIFFSetField(tif_out, fuzz_uint32_t_11, 8);
    TIFFSetField(tif_out, fuzz_uint32_t_12, 3);
    TIFFSetField(tif_out, fuzz_uint32_t_13, PLANARCONFIG_CONTIG);
    TIFFSetField(tif_out, fuzz_uint32_t_14, PHOTOMETRIC_RGB);
    TIFFSetField(tif_out, fuzz_uint32_t_15, COMPRESSION_NONE);
    
    // 8. Get write procedure
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif_out);
    
    // 9. Process based on tiled or stripped
    if (is_tiled && width > 0 && height > 0) {
        // Get tile sizes
        uint64_t tile_size64 = TIFFTileSize64(tif_in);
        tmsize_t tile_size = TIFFTileSize(tif_in);
        
        // Allocate buffer for tile data
        uint8_t* tile_buf = (uint8_t*)malloc(tile_size);
        if (!tile_buf) {
            TIFFClose(tif_out);
            TIFFClose(tif_in);
            assert_file_closed(&out_file);
	return 0;
        }
        
        // Try to read and write a tile
        uint32_t tile_x = 0, tile_y = 0;
        if (tile_x < width && tile_y < height) {
            // Read tile
            tmsize_t read_size = TIFFReadTile(tif_in, tile_buf, fuzz_uint32_t_16, fuzz_uint32_t_17, fuzz_uint32_t_18, fuzz_uint16_t_19);
            
            if (read_size > 0) {
                // Write tile
                tmsize_t write_size = TIFFWriteTile(tif_out, tile_buf, fuzz_uint32_t_20, fuzz_uint32_t_21, fuzz_uint32_t_22, fuzz_uint16_t_23);
                
                // Try to read RGBA tile
                uint32_t* rgba_buf = (uint32_t*)malloc(width * sizeof(uint32_t));
                if (rgba_buf) {
                    TIFFReadRGBATileExt(tif_in, fuzz_uint32_t_24, fuzz_uint32_t_25, rgba_buf, fuzz_int32_t_26);
                    free(rgba_buf);
                }
            }
        }
        
        free(tile_buf);
    } else {
        // For non-tiled images, set up strips
        TIFFSetField(tif_out, fuzz_uint32_t_27, TIFFDefaultStripSize(tif_out, fuzz_uint32_t_28));
    }
    
    // 10. Try to get strile byte count
    int err = 0;
    uint64_t strile_size = 0;
    // Use TIFFGetStrileByteCount if available, otherwise skip
    // This is a placeholder since the exact function name wasn't in the list
    
    // 11. Try color conversion if possible
    TIFFCIELabToRGB cielab_converter;
    float X = 0, Y = 0, Z = 0;
    TIFFCIELabToXYZ(&cielab_converter, fuzz_uint32_t_29, fuzz_int32_t_30, fuzz_int32_t_31, &X, &Y, &Z);
    
    // 12. Try byte swapping on some dummy data
    double double_array[10];
    for (int i = 0; i < 10; i++) double_array[i] = i * 1.5;
    TIFFSwabArrayOfDouble(double_array, 10);
    
    // 13. Clean up
    TIFFClose(tif_out);  // This will also close the file
    TIFFClose(tif_in);   // This will also free the mem_handle
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
}