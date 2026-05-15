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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 4
//<Quality> {"density":1,"unique_branches":{"TIFFReadDirEntryLong8":[[883,9,883,25,0,0,4,0],[887,17,887,45,0,0,4,0]],"OJPEGReadHeaderInfoSec":[[1559,13,1559,39,0,0,4,0],[1560,13,1560,39,0,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFStreamOpen","TIFFWriteCheck","TIFFReadDirectory","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFDefaultStripSize","TIFFIsTiled","TIFFGetWriteProc","TIFFTileSize64","TIFFTileSize","TIFFGetField","TIFFGetField","TIFFSetField","TIFFSetField","_TIFFmalloc","TIFFReadTile","TIFFWriteTile","TIFFWriteEncodedTile","TIFFComputeTile","_TIFFmalloc","TIFFReadRGBATileExt","_TIFFfree","TIFFGetField","TIFFStripSize","_TIFFmalloc","TIFFNumberOfStrips","TIFFReadEncodedStrip","TIFFWriteEncodedStrip","TIFFGetStrileOffsetWithErr","LogL16fromY","TIFFCIELabToXYZ","TIFFSwabArrayOfDouble","TIFFFlush","_TIFFfree","_TIFFfree","TIFFClose","TIFFClose"],"critical_calls":["TIFFFlush","TIFFSwabArrayOfDouble","TIFFCIELabToXYZ","LogL16fromY","TIFFGetStrileOffsetWithErr","TIFFWriteEncodedStrip","TIFFReadEncodedStrip","TIFFNumberOfStrips","_TIFFmalloc","TIFFStripSize","TIFFGetField","_TIFFfree","TIFFReadRGBATileExt","_TIFFmalloc","TIFFWriteEncodedTile","TIFFComputeTile","TIFFWriteTile","TIFFReadTile","_TIFFmalloc","TIFFSetField","TIFFSetField","TIFFGetField","TIFFGetField","TIFFTileSize","TIFFTileSize64","TIFFGetWriteProc","TIFFIsTiled","TIFFSetField","TIFFDefaultStripSize","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFReadDirectory","TIFFWriteCheck","TIFFStreamOpen","TIFFClientOpen","_TIFFfree","_TIFFfree","TIFFClose","TIFFClose"],"visited":0}
/**/


// Memory buffer structure for TIFFClientOpen
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

// I/O functions for memory buffer
static tmsize_t  id_000022_mem_read(thandle_t fd, void* buf, tmsize_t size) {
    mem_buffer* mb = (mem_buffer*)fd;
    tmsize_t n = size;
    if (mb->pos + n > mb->size) {
        n = mb->size - mb->pos;
    }
    if (n > 0) {
        memcpy(buf, mb->data + mb->pos, n);
        mb->pos += n;
    }
    return n;
}

static tmsize_t  id_000022_mem_write(thandle_t fd, void* buf, tmsize_t size) {
    // Not used for read-only memory buffer
    return 0;
}

static toff_t  id_000022_mem_seek(thandle_t fd, toff_t offset, int whence) {
    mem_buffer* mb = (mem_buffer*)fd;
    switch (whence) {
        case SEEK_SET:
            mb->pos = offset;
            break;
        case SEEK_CUR:
            mb->pos += offset;
            break;
        case SEEK_END:
            mb->pos = mb->size + offset;
            break;
    }
    if (mb->pos > mb->size) {
        mb->pos = mb->size;
    }
    return mb->pos;
}

static int  id_000022_mem_close(thandle_t fd) {
    // Nothing to do for memory buffer
    return 0;
}

static toff_t  id_000022_mem_size(thandle_t fd) {
    mem_buffer* mb = (mem_buffer*)fd;
    return mb->size;
}

static int  id_000022_mem_map(thandle_t fd, void** base, toff_t* size) {
    // Not implemented
    return 0;
}

static void  id_000022_mem_unmap(thandle_t fd, void* base, toff_t size) {
    // Not implemented
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<217) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
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
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_19, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_20, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_21, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_22, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_23, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_24, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_25, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_26, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_27, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_28, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_29, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_30, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_31, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_32, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_33, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_34, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_35, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_36, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_37, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_38, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_39, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_40, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_41, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_42, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_43, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_44, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_45, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_46, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_47, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_48, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0;
    }

    TIFF* tif_in = nullptr;
    TIFF* tif_out = nullptr;
    FILE* out_file = nullptr;
    std::ofstream* out_stream = nullptr;
    void* tile_buf = nullptr;
    void* rgba_buf = nullptr;
    uint32_t* rgba_tile_buf = nullptr;
    
    try {
        // 1. Open input TIFF from memory buffer using TIFFClientOpen
        mem_buffer mb = {data, size, 0};
        tif_in = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                               (thandle_t)&mb,
                                id_000022_mem_read,  id_000022_mem_write,
                                id_000022_mem_seek,  id_000022_mem_close,
                                id_000022_mem_size,  id_000022_mem_map,  id_000022_mem_unmap);
        
        if (!tif_in) {
            throw std::runtime_error("Failed to open input TIFF");
        }
        
        // 2. Check if we can write to output
        out_file = fopen("output_file", "wb");
        if (!out_file) {
            throw std::runtime_error("Failed to open output file");
        }
        
        out_stream = new std::ofstream("output_file", std::ios::binary);
        tif_out = TIFFStreamOpen(fuzz_str_3, out_stream);
        
        if (!tif_out) {
            throw std::runtime_error("Failed to open output TIFF");
        }
        
        // 3. Check write capability for output TIFF
        if (!TIFFWriteCheck(tif_out, fuzz_int32_t_5, fuzz_str_4)) {
            throw std::runtime_error("TIFFWriteCheck failed");
        }
        
        // 4. Read directory from input TIFF
        if (!TIFFReadDirectory(tif_in)) {
            throw std::runtime_error("Failed to read directory");
        }
        
        // 5. Get image parameters
        uint32_t width = 0, height = 0;
        uint16_t samplesperpixel = 1, bitspersample = 8;
        uint16_t photometric = 0;
        uint16_t planarconfig = 0;
        
        TIFFGetField(tif_in, fuzz_uint32_t_6, &width);
        TIFFGetField(tif_in, fuzz_uint32_t_7, &height);
        TIFFGetField(tif_in, fuzz_uint32_t_8, &samplesperpixel);
        TIFFGetField(tif_in, fuzz_uint32_t_9, &bitspersample);
        TIFFGetField(tif_in, fuzz_uint32_t_10, &photometric);
        TIFFGetField(tif_in, fuzz_uint32_t_11, &planarconfig);
        
        // 6. Set basic parameters for output TIFF
        TIFFSetField(tif_out, fuzz_uint32_t_12, width);
        TIFFSetField(tif_out, fuzz_uint32_t_13, height);
        TIFFSetField(tif_out, fuzz_uint32_t_14, samplesperpixel);
        TIFFSetField(tif_out, fuzz_uint32_t_15, bitspersample);
        TIFFSetField(tif_out, fuzz_uint32_t_16, photometric);
        TIFFSetField(tif_out, fuzz_uint32_t_17, planarconfig);
        TIFFSetField(tif_out, fuzz_uint32_t_18, ORIENTATION_TOPLEFT);
        TIFFSetField(tif_out, fuzz_uint32_t_19, COMPRESSION_NONE);
        TIFFSetField(tif_out, fuzz_uint32_t_20, 
                     TIFFDefaultStripSize(tif_out, fuzz_uint32_t_21));
        
        // 7. Check if input is tiled
        int is_tiled = TIFFIsTiled(tif_in);
        
        // 8. Get write procedure
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif_out);
        
        // 9. Get tile sizes
        uint64_t tile_size64 = TIFFTileSize64(tif_in);
        tmsize_t tile_size = TIFFTileSize(tif_in);
        
        // 10. Process based on tiling
        if (is_tiled) {
            uint32_t tile_width = 0, tile_height = 0;
            TIFFGetField(tif_in, fuzz_uint32_t_22, &tile_width);
            TIFFGetField(tif_in, fuzz_uint32_t_23, &tile_height);
            
            TIFFSetField(tif_out, fuzz_uint32_t_24, tile_width);
            TIFFSetField(tif_out, fuzz_uint32_t_25, tile_height);
            
            // Allocate tile buffer
            tile_buf = _TIFFmalloc(tile_size);
            if (!tile_buf) {
                throw std::runtime_error("Failed to allocate tile buffer");
            }
            
            // Read and write tiles
            for (uint32_t row = 0; row < height; row += tile_height) {
                for (uint32_t col = 0; col < width; col += tile_width) {
                    // Read tile from input
                    tmsize_t bytes_read = TIFFReadTile(tif_in, tile_buf, 
                                                      fuzz_uint32_t_26, fuzz_uint32_t_27, fuzz_uint32_t_28, fuzz_uint16_t_29);
                    if (bytes_read > 0) {
                        // Write tile to output
                        TIFFWriteTile(tif_out, tile_buf, fuzz_uint32_t_30, fuzz_uint32_t_31, fuzz_uint32_t_32, fuzz_uint16_t_33);
                        
                        // Also write encoded tile
                        TIFFWriteEncodedTile(tif_out, 
                                            TIFFComputeTile(tif_out, fuzz_uint32_t_34, fuzz_uint32_t_35, fuzz_uint32_t_36, fuzz_uint16_t_37),
                                            tile_buf, bytes_read);
                    }
                }
            }
            
            // Try to read RGBA tile
            rgba_tile_buf = (uint32_t*)_TIFFmalloc(tile_width * tile_height * sizeof(uint32_t));
            if (rgba_tile_buf) {
                TIFFReadRGBATileExt(tif_in, fuzz_uint32_t_38, fuzz_uint32_t_39, rgba_tile_buf, fuzz_int32_t_40);
                _TIFFfree(rgba_tile_buf);
                rgba_tile_buf = nullptr;
            }
            
        } else {
            // Process as stripped image
            uint32_t rows_per_strip = 0;
            TIFFGetField(tif_in, fuzz_uint32_t_41, &rows_per_strip);
            
            tmsize_t strip_size = TIFFStripSize(tif_in);
            tile_buf = _TIFFmalloc(strip_size);
            if (!tile_buf) {
                throw std::runtime_error("Failed to allocate strip buffer");
            }
            
            for (uint32_t strip = 0; strip < TIFFNumberOfStrips(tif_in); strip++) {
                tmsize_t bytes_read = TIFFReadEncodedStrip(tif_in, fuzz_uint32_t_42, 
                                                          tile_buf, strip_size);
                if (bytes_read > 0) {
                    TIFFWriteEncodedStrip(tif_out, fuzz_uint32_t_43, tile_buf, bytes_read);
                }
            }
        }
        
        // 11. Get strile offset
        int err = 0;
        uint64_t strile_offset = TIFFGetStrileOffsetWithErr(tif_in, fuzz_uint32_t_44, &err);
        
        // 12. Use LogL16fromY for LogL conversion if applicable
        if (photometric == PHOTOMETRIC_LOGL) {
            double y = 0.5;
            int em = 1;
            int logl16 = LogL16fromY(y, fuzz_int32_t_45);
        }
        
        // 13. Color conversion for CIELab if applicable
        if (photometric == PHOTOMETRIC_CIELAB) {
            TIFFCIELabToRGB cielab;
            uint32_t l = 50000, a = 0, b = 0;
            float X, Y, Z;
            TIFFCIELabToXYZ(&cielab, fuzz_uint32_t_46, fuzz_int32_t_47, fuzz_int32_t_48, &X, &Y, &Z);
        }
        
        // 14. Byte swapping example
        double double_array[2] = {1.234, 5.678};
        TIFFSwabArrayOfDouble(double_array, 2);
        
        // 15. Flush and close
        TIFFFlush(tif_out);
        
    } catch (const std::exception& e) {
        // Error handling - just continue to cleanup
    }
    
    // Cleanup
    if (tile_buf) {
        _TIFFfree(tile_buf);
    }
    if (rgba_buf) {
        _TIFFfree(rgba_buf);
    }
    if (tif_in) {
        TIFFClose(tif_in);
    }
    if (tif_out) {
        TIFFClose(tif_out);
    }
    if (out_stream) {
        out_stream->close();
        delete out_stream;
    }
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_name_closed("output_file");
	return 0;
}