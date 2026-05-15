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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 98, nr_unique_branch: 56
//<Quality> {"density":28,"unique_branches":{"TIFFClientOpenExt":[[295,5,295,69,50,0,4,0]],"_tiffWriteProc":[[116,13,116,34,0,0,4,0]],"TIFFFlush":[[43,9,43,43,0,0,4,0],[44,9,44,45,0,0,4,1]],"TIFFReadDirEntryArrayWithLimit":[[1318,26,1318,60,0,0,4,0],[1318,26,1318,60,0,0,4,1],[1321,9,1321,18,0,0,4,1],[283,23,283,62,4,0,4,0],[283,23,283,62,10,0,4,0]],"TIFFReadDirEntryData":[[3981,9,3981,18,0,0,4,1],[3992,13,3992,35,0,0,4,1],[3992,39,3992,71,0,0,4,1],[3997,13,3997,41,0,0,4,0],[3997,13,3997,41,0,0,4,1]],"TIFFFetchDirectory":[[6091,9,6091,18,0,0,4,1],[6181,13,6181,50,0,0,4,0],[6181,13,6181,50,0,0,4,1],[6197,13,6197,45,0,0,4,0],[6197,13,6197,45,0,0,4,1],[6200,17,6200,26,0,0,4,1],[6200,30,6200,62,0,0,4,1],[6201,17,6201,56,0,0,4,0],[6201,17,6201,56,0,0,4,1],[6211,17,6211,43,0,0,4,0],[6211,17,6211,43,0,0,4,1],[6213,17,6213,34,0,0,4,0],[6213,17,6213,34,0,0,4,1],[6226,17,6226,26,0,0,4,1],[6226,30,6226,62,0,0,4,1],[6227,17,6227,56,0,0,4,0],[6227,17,6227,56,0,0,4,1],[6237,17,6237,43,0,0,4,0],[6239,17,6239,34,0,0,4,0],[6239,17,6239,34,0,0,4,1],[6249,13,6249,28,0,0,4,0],[6249,13,6249,28,0,0,4,1],[6260,13,6260,33,0,0,4,0],[6260,13,6260,33,0,0,4,1],[6272,13,6272,28,0,0,4,1],[6275,13,6275,22,0,0,4,1],[6275,26,6275,64,0,0,4,1],[6276,13,6276,32,0,0,4,0],[6276,13,6276,32,0,0,4,1],[6286,13,6286,23,0,0,4,0],[6289,17,6289,49,0,0,4,0],[6289,17,6289,49,0,0,4,1],[6293,21,6293,30,0,0,4,1],[6293,34,6293,66,0,0,4,1],[6294,21,6294,60,0,0,4,0],[6294,21,6294,60,0,0,4,1],[6299,21,6299,47,0,0,4,0],[6299,21,6299,47,0,0,4,1],[6306,21,6306,30,0,0,4,1],[6306,34,6306,66,0,0,4,1],[6307,21,6307,60,0,0,4,1],[6312,21,6312,47,0,0,4,0]]},"library_calls":["TIFFClientOpen","TIFFGetUnmapFileProc","TIFFGetSizeProc","TIFFSetClientdata","TIFFRasterScanlineSize","TIFFReadDirectory","TIFFClose","TIFFOpen","TIFFClientOpen","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFScanlineSize","_TIFFmalloc","TIFFWriteScanline","_TIFFfree","TIFFClose","TIFFFlush","TIFFClose","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFGetUnmapFileProc","TIFFGetSizeProc","TIFFSetClientdata","TIFFRasterScanlineSize","TIFFReadDirectory","TIFFClose","TIFFOpen","TIFFClientOpen","TIFFGetField","TIFFGetField","TIFFGetField","TIFFGetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFScanlineSize","_TIFFmalloc","TIFFWriteScanline","_TIFFfree","TIFFClose","TIFFFlush","TIFFClose"],"visited":1}
/**/


// Custom I/O callbacks for memory-based TIFF handling
static tmsize_t  id_000009_memRead(thandle_t handle, void* buf, tmsize_t size) {
    uint8_t* data = (uint8_t*)handle;
    tmsize_t* offset = (tmsize_t*)(data - sizeof(tmsize_t));
    
    if (*offset + size > *(offset - 1)) {  // Check against size
        size = *(offset - 1) - *offset;
    }
    if (size > 0) {
        memcpy(buf, data + *offset, size);
        *offset += size;
    }
    return size;
}

static tmsize_t  id_000009_memWrite(thandle_t handle, void* buf, tmsize_t size) {
    // Write not supported for this fuzzing scenario
    return 0;
}

static toff_t  id_000009_memSeek(thandle_t handle, toff_t offset, int whence) {
    uint8_t* data = (uint8_t*)handle;
    tmsize_t* mem_offset = (tmsize_t*)(data - sizeof(tmsize_t));
    tmsize_t* mem_size = (tmsize_t*)(mem_offset - 1);
    
    switch (whence) {
        case SEEK_SET:
            *mem_offset = (offset < *mem_size) ? offset : *mem_size;
            break;
        case SEEK_CUR:
            *mem_offset = (*mem_offset + offset < *mem_size) ? 
                         *mem_offset + offset : *mem_size;
            break;
        case SEEK_END:
            *mem_offset = *mem_size;
            break;
    }
    return *mem_offset;
}

static int  id_000009_memClose(thandle_t handle) {
    // Nothing to close for memory buffer
    return 0;
}

static toff_t  id_000009_memSize(thandle_t handle) {
    uint8_t* data = (uint8_t*)handle;
    tmsize_t* mem_size = (tmsize_t*)(data - 2 * sizeof(tmsize_t));
    return *mem_size;
}

static int  id_000009_memMap(thandle_t handle, void** base, toff_t* size) {
    uint8_t* data = (uint8_t*)handle;
    tmsize_t* mem_size = (tmsize_t*)(data - 2 * sizeof(tmsize_t));
    
    *base = data;
    *size = *mem_size;
    return 1;
}

static void  id_000009_memUnmap(thandle_t handle, void* base, toff_t size) {
    // Nothing to unmap
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<36) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_7, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for TIFF operations
    
    // Create a memory buffer with size and offset tracking
    tmsize_t* mem_buf = (tmsize_t*)malloc(size + 2 * sizeof(tmsize_t));
    if (!mem_buf) {
	return 0;
	}
	
    
    // Setup memory buffer structure
    mem_buf[0] = size;  // Total size
    mem_buf[1] = 0;     // Current offset
    uint8_t* tiff_data = (uint8_t*)(mem_buf + 2);
    memcpy(tiff_data, data, size);
    
    // Open TIFF from memory buffer
    TIFF* tif = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
                              (thandle_t)tiff_data,
                               id_000009_memRead,  id_000009_memWrite,
                               id_000009_memSeek,  id_000009_memClose,
                               id_000009_memSize,  id_000009_memMap,  id_000009_memUnmap);
    
    if (tif) {
        // Get the unmap file procedure
        TIFFUnmapFileProc unmapProc = TIFFGetUnmapFileProc(tif);
        
        // Get the size procedure
        TIFFSizeProc sizeProc = TIFFGetSizeProc(tif);
        
        // Set client data (using the memory buffer as client data)
        thandle_t old_clientdata = TIFFSetClientdata(tif, (thandle_t)tiff_data);
        
        // Get scanline size (if image has appropriate dimensions)
        tmsize_t scanline_size = TIFFRasterScanlineSize(tif);
        
        // Try to read and rewrite directory
        // First read the current directory
        if (TIFFReadDirectory(tif)) {
            // Create a temporary file for output
            FILE* out_file = fopen("output_file", "wb");
            if (out_file) {
                int fd = fuzz_fileno(out_file);
                
                // Close the memory-based TIFF
                TIFFClose(tif);
                
                // Now open a file-based TIFF for writing
                TIFF* tif_out = TIFFOpen("output_file", fuzz_str_3);
                if (tif_out) {
                    // Copy basic tags from input
                    uint32_t width = 0, height = 0;
                    uint16_t samples = 1, bitspersample = 8;
                    
                    // Reset memory offset for reading again
                    mem_buf[1] = 0;
                    TIFF* tif_in = TIFFClientOpen(fuzz_str_4, fuzz_str_5, 
                                                  (thandle_t)tiff_data,
                                                   id_000009_memRead,  id_000009_memWrite,
                                                   id_000009_memSeek,  id_000009_memClose,
                                                   id_000009_memSize,  id_000009_memMap,  id_000009_memUnmap);
                    
                    if (tif_in) {
                        TIFFGetField(tif_in, TIFFTAG_IMAGEWIDTH, &width);
                        TIFFGetField(tif_in, fuzz_uint32_t_6, &height);
                        TIFFGetField(tif_in, TIFFTAG_SAMPLESPERPIXEL, &samples);
                        TIFFGetField(tif_in, TIFFTAG_BITSPERSAMPLE, &bitspersample);
                        
                        // Set basic tags for output
                        TIFFSetField(tif_out, TIFFTAG_IMAGEWIDTH, width);
                        TIFFSetField(tif_out, TIFFTAG_IMAGELENGTH, height);
                        TIFFSetField(tif_out, TIFFTAG_SAMPLESPERPIXEL, samples);
                        TIFFSetField(tif_out, TIFFTAG_BITSPERSAMPLE, bitspersample);
                        TIFFSetField(tif_out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
                        TIFFSetField(tif_out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
                        TIFFSetField(tif_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
                        TIFFSetField(tif_out, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
                        
                        // Allocate buffer for one scanline
                        tmsize_t out_scanline = TIFFScanlineSize(tif_out);
                        uint8_t* buf = (uint8_t*)_TIFFmalloc(out_scanline);
                        
                        if (buf && width > 0 && height > 0) {
                            // Write dummy scanlines
                            for (uint32_t row = 0; row < height; row++) {
                                memset(buf, row % 256, out_scanline);
                                TIFFWriteScanline(tif_out, buf, row, fuzz_uint16_t_7);
                            }
                        }
                        
                        if (buf) _TIFFfree(buf);
                        TIFFClose(tif_in);
                    }
                    
                    // Force data to be written
                    TIFFFlush(tif_out);
                    TIFFClose(tif_out);
                }
                
                assert_file_closed(&out_file);;
                remove("output_file");
            }
        } else {
            TIFFClose(tif);
        }
    }
    
    // Clean up memory buffer
    free(mem_buf);
    
    return 0;
}