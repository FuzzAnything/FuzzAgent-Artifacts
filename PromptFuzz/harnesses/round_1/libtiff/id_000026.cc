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
//<ID> 1607
//<Prompt> []
/*<Combination>: [
*/
//<score> 37, nr_unique_branch: 0
//<Quality> {"density":37,"unique_branches":{},"library_calls":["TIFFGetVersion","TIFFOpenOptionsAlloc","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFOpenOptionsFree","TIFFClientOpen","TIFFClose","TIFFOpenOptionsFree","TIFFOpenOptionsFree","TIFFReadDirectory","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFDeferStrileArrayWriting","TIFFDefaultStripSize","TIFFSetField","TIFFNumberOfStrips","TIFFStripSize","_TIFFcalloc","TIFFReadEncodedStrip","TIFFWriteEncodedStrip","TIFFIsTiled","TIFFComputeTile","TIFFReadScanline","TIFFCIELabToRGBInit","LogL10fromY","TIFFFlushData","_TIFFfree","TIFFWarningExt","TIFFClose","TIFFClose","TIFFOpenOptionsFree","TIFFOpenOptionsFree"],"critical_calls":["TIFFGetVersion","TIFFOpenOptionsAlloc","TIFFOpenOptionsAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFOpenOptionsSetMaxCumulatedMemAlloc","TIFFClientOpenExt","TIFFClientOpen","TIFFReadDirectory","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFFieldWithTag","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFDeferStrileArrayWriting","TIFFDefaultStripSize","TIFFSetField","TIFFNumberOfStrips","TIFFStripSize","_TIFFcalloc","TIFFReadEncodedStrip","TIFFWriteEncodedStrip","TIFFIsTiled","TIFFComputeTile","TIFFReadScanline","TIFFCIELabToRGBInit","LogL10fromY","TIFFFlushData","_TIFFfree","TIFFWarningExt","TIFFClose","TIFFClose","TIFFOpenOptionsFree","TIFFOpenOptionsFree"],"visited":0}
/**/


// Custom I/O handlers for memory buffer
struct MemBuffer {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

static tmsize_t  id_000026_memReadProc(thandle_t fd, void* buf, tmsize_t size) {
    MemBuffer* buffer = reinterpret_cast<MemBuffer*>(fd);
    tmsize_t remaining = buffer->size - buffer->pos;
    tmsize_t to_read = size < remaining ? size : remaining;
    if (to_read > 0) {
        memcpy(buf, buffer->data + buffer->pos, to_read);
        buffer->pos += to_read;
    }
    return to_read;
}

static tmsize_t  id_000026_memWriteProc(thandle_t fd, void* buf, tmsize_t size) {
    // Not used for reading
    return 0;
}

static toff_t  id_000026_memSeekProc(thandle_t fd, toff_t offset, int whence) {
    MemBuffer* buffer = reinterpret_cast<MemBuffer*>(fd);
    switch (whence) {
        case SEEK_SET:
            buffer->pos = offset < buffer->size ? offset : buffer->size;
            break;
        case SEEK_CUR:
            buffer->pos = (buffer->pos + offset) < buffer->size ? 
                         (buffer->pos + offset) : buffer->size;
            break;
        case SEEK_END:
            buffer->pos = buffer->size;
            break;
    }
    return buffer->pos;
}

static int  id_000026_memCloseProc(thandle_t fd) {
    // Nothing to cleanup
    return 0;
}

static toff_t  id_000026_memSizeProc(thandle_t fd) {
    MemBuffer* buffer = reinterpret_cast<MemBuffer*>(fd);
    return buffer->size;
}

static int  id_000026_memMapProc(thandle_t fd, void** base, toff_t* size) {
    MemBuffer* buffer = reinterpret_cast<MemBuffer*>(fd);
    *base = const_cast<void*>(reinterpret_cast<const void*>(buffer->data));
    *size = buffer->size;
    return 0;
}

static void  id_000026_memUnmapProc(thandle_t fd, void* base, toff_t size) {
    // Nothing to do
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<190) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
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
		FDPConsumeIntegral(int64_t, fuzz_int64_t_21, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_22, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_23, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_24, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_25, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_26, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_27, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_28, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_29, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_30, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_31, fuzz_str_sz_31, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_32, fuzz_str_sz_32, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
	return 0;
	}
	 // Need minimum data
    
    // 1. Get version
    const char* version = TIFFGetVersion();
    
    // 2. Create input/output files
    FILE* in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Allocate and configure open options
    TIFFOpenOptions* in_opts = TIFFOpenOptionsAlloc();
    TIFFOpenOptions* out_opts = TIFFOpenOptionsAlloc();
    
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(in_opts, 100 * 1024 * 1024); // 100MB
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(out_opts, 100 * 1024 * 1024);
    
    // 4. Open input TIFF using client open
    MemBuffer mem_buffer = {data, size, 0};
    TIFF* tif_in = TIFFClientOpenExt(fuzz_str_1, fuzz_str_2, 
                                     reinterpret_cast<thandle_t>(&mem_buffer),
                                      id_000026_memReadProc,  id_000026_memWriteProc,
                                      id_000026_memSeekProc,  id_000026_memCloseProc,
                                      id_000026_memSizeProc,  id_000026_memMapProc,  id_000026_memUnmapProc,
                                     in_opts);
    
    if (!tif_in) {
        TIFFOpenOptionsFree(in_opts);
        TIFFOpenOptionsFree(out_opts);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Open output TIFF using file descriptor
    int out_fd = fuzz_fileno(out_file);
    TIFF* tif_out = TIFFClientOpen(fuzz_str_3, fuzz_str_4,
                                   reinterpret_cast<thandle_t>(out_fd),
                                   NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    
    if (!tif_out) {
        TIFFClose(tif_in);
        TIFFOpenOptionsFree(in_opts);
        TIFFOpenOptionsFree(out_opts);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
    }
    
    // 6. Read input directory
    int dir_read = TIFFReadDirectory(tif_in);
    
    // 7. Get basic image information
    uint32_t width = 0, height = 0;
    uint16_t samples_per_pixel = 1, bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    
    TIFFGetField(tif_in, fuzz_uint32_t_5, &width);
    TIFFGetField(tif_in, fuzz_uint32_t_6, &height);
    TIFFGetField(tif_in, fuzz_uint32_t_7, &samples_per_pixel);
    TIFFGetField(tif_in, fuzz_uint32_t_8, &bits_per_sample);
    TIFFGetField(tif_in, fuzz_uint32_t_9, &photometric);
    
    // 8. Use TIFFFieldWithTag for checking available fields
    const TIFFField* field = TIFFFieldWithTag(tif_in, fuzz_uint32_t_10);
    
    // 9. Set output fields
    TIFFSetField(tif_out, fuzz_uint32_t_11, width);
    TIFFSetField(tif_out, fuzz_uint32_t_12, height);
    TIFFSetField(tif_out, fuzz_uint32_t_13, samples_per_pixel);
    TIFFSetField(tif_out, fuzz_uint32_t_14, bits_per_sample);
    TIFFSetField(tif_out, fuzz_uint32_t_15, photometric);
    TIFFSetField(tif_out, fuzz_uint32_t_16, PLANARCONFIG_CONTIG);
    TIFFSetField(tif_out, fuzz_uint32_t_17, ORIENTATION_TOPLEFT);
    TIFFSetField(tif_out, fuzz_uint32_t_18, COMPRESSION_NONE);
    
    // 10. Defer strile array writing
    TIFFDeferStrileArrayWriting(tif_out);
    
    // 11. Get strip/tile information
    uint32_t strip_size = TIFFDefaultStripSize(tif_in, fuzz_uint32_t_19);
    TIFFSetField(tif_out, fuzz_uint32_t_20, strip_size);
    
    uint32_t num_strips = TIFFNumberOfStrips(tif_in);
    tmsize_t input_strip_size = TIFFStripSize(tif_in);
    
    // 12. Allocate buffer for reading
    tmsize_t alloc_size = input_strip_size > 0 ? input_strip_size : 8192;
    void* buffer = _TIFFcalloc(fuzz_int64_t_21, alloc_size);
    
    if (buffer && num_strips > 0 && width > 0 && height > 0) {
        // 13. Process strips
        for (uint32_t strip = 0; strip < num_strips; strip++) {
            if (strip * strip_size >= height) break;
            
            tmsize_t bytes_read = TIFFReadEncodedStrip(tif_in, fuzz_uint32_t_22, buffer, alloc_size);
            if (bytes_read > 0) {
                TIFFWriteEncodedStrip(tif_out, fuzz_uint32_t_23, buffer, bytes_read);
            }
        }
        
        // 14. Check if tiled
        int is_tiled = TIFFIsTiled(tif_in);
        if (is_tiled && width > 0 && height > 0) {
            uint32_t tile = TIFFComputeTile(tif_in, fuzz_uint32_t_24, fuzz_uint32_t_25, fuzz_uint32_t_26, fuzz_uint16_t_27);
        }
        
        // 15. Try reading a scanline
        if (height > 0) {
            uint32_t row = 0;
            if (row < height) {
                TIFFReadScanline(tif_in, buffer, fuzz_uint32_t_28, fuzz_uint16_t_29);
            }
        }
        
        // 16. CIELab conversion if applicable
        if (photometric == PHOTOMETRIC_CIELAB) {
            TIFFDisplay display;
            TIFFCIELabToRGB cielab;
            float whitePoint[3] = {1.0f, 1.0f, 1.0f};
            TIFFCIELabToRGBInit(&cielab, &display, whitePoint);
            
            // Test LogL conversion
            double y = 0.5;
            int logl = LogL10fromY(y, fuzz_int32_t_30);
        }
        
        // 17. Flush data
        TIFFFlushData(tif_out);
    }
    
    // 18. Cleanup
    if (buffer) _TIFFfree(buffer);
    
    // 19. Warning example
    if (width == 0 || height == 0) {
        TIFFWarningExt(reinterpret_cast<thandle_t>(tif_in), fuzz_str_31, 
                      fuzz_str_32);
    }
    
    TIFFClose(tif_in);
    TIFFClose(tif_out);
    TIFFOpenOptionsFree(in_opts);
    TIFFOpenOptionsFree(out_opts);
    
    assert_file_closed(&out_file);;
    
    // Clean up temp files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	return 0;
}