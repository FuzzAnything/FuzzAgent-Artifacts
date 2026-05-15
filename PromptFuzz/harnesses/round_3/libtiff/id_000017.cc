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
//<ID> 529
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"TIFFFlush":[[32,9,32,34,0,0,4,0]],"TIFFClientOpenExt":[[516,21,516,34,0,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFReadDirectory","TIFFIsTiled","TIFFXYZToRGB","TIFFWriteDirectory","TIFFFlush","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFComputeTile","TIFFNumberOfTiles","TIFFGetStrileByteCount","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFReadDirectory","TIFFIsTiled","TIFFXYZToRGB","TIFFWriteDirectory","TIFFFlush","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFComputeTile","TIFFNumberOfTiles","TIFFGetStrileByteCount","TIFFClose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs to process TIFF data:

*/


// Include libtiff headers

// Custom memory I/O structure
struct mem_handle {
    const uint8_t* data;
    size_t size;
    size_t pos;
    bool writable;
    std::vector<uint8_t> output_buffer;
};

// I/O callbacks for TIFFClientOpen
static tmsize_t  id_000017_mem_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_handle* mh = reinterpret_cast<mem_handle*>(handle);
    tmsize_t bytes_to_read = size;
    
    if (mh->pos + bytes_to_read > mh->size) {
        bytes_to_read = mh->size - mh->pos;
    }
    
    if (bytes_to_read > 0) {
        memcpy(buf, mh->data + mh->pos, bytes_to_read);
        mh->pos += bytes_to_read;
    }
    
    return bytes_to_read;
}

static tmsize_t  id_000017_mem_write(thandle_t handle, void* buf, tmsize_t size) {
    mem_handle* mh = reinterpret_cast<mem_handle*>(handle);
    
    if (!mh->writable) {
        return 0;
    }
    
    size_t new_pos = mh->pos + size;
    if (new_pos > mh->output_buffer.size()) {
        mh->output_buffer.resize(new_pos);
    }
    
    memcpy(mh->output_buffer.data() + mh->pos, buf, size);
    mh->pos = new_pos;
    
    return size;
}

static toff_t  id_000017_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_handle* mh = reinterpret_cast<mem_handle*>(handle);
    
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

static int  id_000017_mem_close(thandle_t handle) {
    // Nothing to do for memory handle cleanup
    return 0;
}

static toff_t  id_000017_mem_size(thandle_t handle) {
    mem_handle* mh = reinterpret_cast<mem_handle*>(handle);
    return mh->size;
}

static int  id_000017_mem_map(thandle_t handle, void** base, toff_t* size) {
    mem_handle* mh = reinterpret_cast<mem_handle*>(handle);
    *base = const_cast<void*>(reinterpret_cast<const void*>(mh->data));
    *size = mh->size;
    return 1;
}

static void  id_000017_mem_unmap(thandle_t handle, void* base, toff_t size) {
    // Nothing to do for memory unmapping
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0;  // Need minimum data
    }
    
    // 1. Open TIFF using memory buffer
    mem_handle mh;
    mh.data = data;
    mh.size = size;
    mh.pos = 0;
    mh.writable = true;
    mh.output_buffer.resize(size);
    
    TIFF* tif = TIFFClientOpen(
        fuzz_str_1, 
        fuzz_str_2,  // read/write mode
        reinterpret_cast<thandle_t>(&mh),
         id_000017_mem_read,
         id_000017_mem_write,
         id_000017_mem_seek,
         id_000017_mem_close,
         id_000017_mem_size,
         id_000017_mem_map,
         id_000017_mem_unmap
    );
    
    if (!tif) {
        return 0;
    }
    
    // 2. Read directory
    int read_dir_result = TIFFReadDirectory(tif);
    
    // 3. Check if TIFF is tiled
    int is_tiled = TIFFIsTiled(tif);
    
    // 4. Convert XYZ to RGB (using dummy values)
    TIFFCIELabToRGB cielab;
    memset(&cielab, 0, sizeof(cielab));
    float X = 0.5f, Y = 0.5f, Z = 0.5f;
    uint32_t R, G, B;
    TIFFXYZToRGB(&cielab, X, Y, Z, &R, &G, &B);
    
    // 5. Write directory
    int write_dir_result = 0;
    if (read_dir_result) {
        write_dir_result = TIFFWriteDirectory(tif);
    }
    
    // 6. Flush data
    int flush_result = TIFFFlush(tif);
    
    // 7. Check tile if image is tiled
    if (is_tiled) {
        // Use safe coordinates based on image dimensions
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        if (width > 0 && height > 0) {
            // Calculate tile coordinates
            uint32_t tile_width = 0, tile_height = 0;
            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
            
            if (tile_width > 0 && tile_height > 0) {
                // Check a tile at a valid position
                uint32_t x = 0, y = 0, z = 0;
                uint16_t s = 0;
                
                // Ensure coordinates are within bounds
                if (x < width && y < height) {
                    // Note: TIFFCheckTile is not in the provided API list,
                    // using TIFFComputeTile instead
                    uint32_t tile = TIFFComputeTile(tif, fuzz_uint32_t_3, fuzz_uint32_t_4, fuzz_uint32_t_5, fuzz_uint16_t_6);
                    
                    // Get strile byte count if tile is valid
                    if (tile < TIFFNumberOfTiles(tif)) {
                        uint64_t byte_count = TIFFGetStrileByteCount(tif, tile);
                    }
                }
            }
        }
    }
    
    // 8. Close TIFF
    TIFFClose(tif);
    
    // 9. Also test with file-based I/O
    FILE* in_file = fopen("input_file", "wb");
    if (in_file) {
        fwrite(data, 1, size, in_file);
        assert_file_closed(&in_file);;
    }
    
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write processed data if available
        if (!mh.output_buffer.empty()) {
            fwrite(mh.output_buffer.data(), 1, mh.output_buffer.size(), out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // 10. Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}