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
//<ID> 1432
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"TIFFSeek":[[336,9,336,34,0,0,4,0]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFRGBAImageOK","TIFFScanlineSize64","TIFFCreateEXIFDirectory","TIFFErrorExt","_TIFFmalloc","TIFFCIELabToXYZ","_TIFFfree","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFErrorExt","TIFFClose"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFRGBAImageOK","TIFFScanlineSize64","TIFFCreateEXIFDirectory","TIFFErrorExt","_TIFFmalloc","TIFFCIELabToXYZ","_TIFFfree","TIFFGetField","TIFFGetField","_TIFFmalloc","TIFFReadScanline","_TIFFfree","TIFFClose"],"visited":0}
/**/



// Custom structure for our memory-based TIFF handling
typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} mem_stream;

// Callback functions for TIFFClientOpenExt
static tmsize_t  id_000022_mem_read(thandle_t handle, void* buffer, tmsize_t size) {
    mem_stream* stream = reinterpret_cast<mem_stream*>(handle);
    
    if (stream->pos >= stream->size) {
        return 0;
    }
    
    tmsize_t to_read = size;
    if (stream->pos + to_read > stream->size) {
        to_read = stream->size - stream->pos;
    }
    
    memcpy(buffer, stream->data + stream->pos, to_read);
    stream->pos += to_read;
    
    return to_read;
}

static tmsize_t  id_000022_mem_write(thandle_t, void*, tmsize_t) {
    // Read-only for fuzzing
    return 0;
}

static toff_t  id_000022_mem_seek(thandle_t handle, toff_t offset, int whence) {
    mem_stream* stream = reinterpret_cast<mem_stream*>(handle);
    
    switch (whence) {
        case SEEK_SET:
            stream->pos = offset;
            break;
        case SEEK_CUR:
            stream->pos += offset;
            break;
        case SEEK_END:
            stream->pos = stream->size + offset;
            break;
    }
    
    if (stream->pos > stream->size) {
        stream->pos = stream->size;
    }
    
    return stream->pos;
}

static int  id_000022_mem_close(thandle_t handle) {
    mem_stream* stream = reinterpret_cast<mem_stream*>(handle);
    delete stream;
    return 0;
}

static toff_t  id_000022_mem_size(thandle_t handle) {
    mem_stream* stream = reinterpret_cast<mem_stream*>(handle);
    return stream->size;
}

static int  id_000022_mem_map(thandle_t, void**, toff_t*) {
    return 0;  // Not implemented
}

static void  id_000022_mem_unmap(thandle_t, void*, toff_t) {
    // Not implemented
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<89) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	//fuzzer shim end}




    // Initialize memory stream for TIFF data
    mem_stream* stream = new mem_stream{data, size, 0};
    
    // Create TIFFOpenOptions for advanced error handling
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        delete stream;
        return 0;
    }
    
    // Open TIFF from memory using client interface
    TIFF* tif = TIFFClientOpenExt(fuzz_str_1, fuzz_str_2,
                                  reinterpret_cast<thandle_t>(stream),
                                   id_000022_mem_read,  id_000022_mem_write,
                                   id_000022_mem_seek,  id_000022_mem_close,
                                   id_000022_mem_size,  id_000022_mem_map,  id_000022_mem_unmap,
                                  opts);
    
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        delete stream;
        return 0;
    }
    
    // Event: Process TIFF image with RGBA conversion and EXIF operations
    // 1. Check if image is suitable for RGBA conversion
    char err_msg[1024] = {0};
    if (TIFFRGBAImageOK(tif, err_msg)) {
        // 2. Get scanline size for potential processing
        uint64_t scanline_size = TIFFScanlineSize64(tif);
        
        // 3. Create EXIF directory for metadata operations
        int exif_result = TIFFCreateEXIFDirectory(tif);
        
        // 4. Report any issues via error callback
        if (exif_result != 1) {
            TIFFErrorExt(reinterpret_cast<thandle_t>(stream), 
                        fuzz_str_3, 
                        "Failed to create EXIF directory");
        }
        
        // 5. Sample color space conversion (CIELab to XYZ)
        // Create a dummy CIELabToRGB structure for demonstration
        TIFFCIELabToRGB* cielab = static_cast<TIFFCIELabToRGB*>(
            _TIFFmalloc(sizeof(TIFFCIELabToRGB)));
        
        if (cielab) {
            // Initialize with reasonable values for fuzzing
            memset(cielab, 0, sizeof(TIFFCIELabToRGB));
            
            // Perform sample conversion
            float X, Y, Z;
            TIFFCIELabToXYZ(cielab, fuzz_uint32_t_4, fuzz_int32_t_5, fuzz_int32_t_6, &X, &Y, &Z);
            
            _TIFFfree(cielab);
        }
        
        // 6. Write processed output to file for analysis
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            // Write basic info about the processed TIFF
            uint32_t width = 0, height = 0;
            TIFFGetField(tif, fuzz_uint32_t_7, &width);
            TIFFGetField(tif, fuzz_uint32_t_8, &height);
            
            fprintf(out_file, "Processed TIFF: %ux%u, Scanline: %lu bytes\n",
                    width, height, (unsigned long)scanline_size);
            
            // Read a small portion of the image data for output
            if (scanline_size > 0 && scanline_size <= 65536) {
                void* buf = _TIFFmalloc(scanline_size);
                if (buf) {
                    // Read first scanline
                    if (TIFFReadScanline(tif, buf, fuzz_uint32_t_9, fuzz_uint16_t_10) >= 0) {
                        fwrite(buf, 1, scanline_size, out_file);
                    }
                    _TIFFfree(buf);
                }
            }
            
            assert_file_closed(&out_file);;
        }
    } else {
        // Report why RGBA conversion is not possible
        TIFFErrorExt(reinterpret_cast<thandle_t>(stream),
                    fuzz_str_11,
                    "RGBA conversion not possible: %s", err_msg);
    }
    
    // 7. Alternative: Try to read from file if memory approach fails
    FILE* in_file = fopen("input_file", "wb");
    if (in_file) {
        // Write the fuzzer input to file for fallback processing
        fwrite(data, 1, size, in_file);
        assert_file_closed(&in_file);;
        
        // Re-open for reading to demonstrate file descriptor usage
        in_file = fopen("input_file", "rb");
        if (in_file) {
            int fd = fuzz_fileno(in_file);  // Get file descriptor
            // Could use fd with other APIs if needed
            assert_file_closed(&in_file);;
        }
    }
    
    // Cleanup
    TIFFClose(tif);  // This will call our  id_000022_mem_close via callback
    
    assert_file_closed(&in_file);
	return 0;
}