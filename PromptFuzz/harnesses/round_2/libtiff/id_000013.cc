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
//<ID> 707
//<Prompt> []
/*<Combination>: [
*/
//<score> 192, nr_unique_branch: 27
//<Quality> {"density":16,"unique_branches":{"tagNameCompare":[[619,9,619,12,0,0,4,0],[619,9,619,12,0,0,4,1],[622,16,622,44,0,0,4,0]],"tiff_itrunc":[[792,9,792,35,0,0,4,1]],"_TIFFFindFieldByName":[[909,9,909,28,0,0,4,0],[915,9,915,25,0,0,4,1],[927,35,927,38,0,0,4,0],[66,21,66,40,1,0,4,1]],"td_lfind":[[554,34,554,47,0,0,4,0],[555,13,555,34,0,0,4,0],[555,13,555,34,0,0,4,1]],"LogL10fromY":[[896,9,896,20,0,0,4,1],[898,14,898,28,0,0,4,1]],"TIFFReadDirEntryLong8":[[839,17,839,45,0,0,4,0]],"uv_encode":[[973,9,973,15,0,0,4,1],[973,19,973,25,0,0,4,1],[979,9,979,22,0,0,4,1],[982,9,982,21,0,0,4,1],[984,9,984,30,0,0,4,1],[987,9,987,29,0,0,4,1]],"OJPEGReadHeaderInfoSec":[[1548,13,1548,38,0,0,4,0]],"_TIFFRand":[[781,9,781,18,0,0,4,0],[781,9,781,18,0,0,4,1]],"TIFFFieldWithName":[[944,9,944,13,0,0,4,1]],"LogLuv24fromXYZ":[[1071,9,1071,12,0,0,4,1],[1071,16,1071,23,0,0,4,1],[1082,9,1082,15,0,0,4,1]]},"library_calls":["TIFFSetErrorHandlerExt","TIFFClientOpen","TIFFGetMapFileProc","TIFFFieldWithName","TIFFVTileSize64","TIFFReadDirectory","LogLuv24fromXYZ","uv_encode","TIFFClientOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteDirectory","TIFFClose","TIFFClose","TIFFSetErrorHandlerExt"],"critical_calls":["TIFFSetErrorHandlerExt","TIFFClientOpen","TIFFGetMapFileProc","TIFFFieldWithName","TIFFVTileSize64","TIFFReadDirectory","LogLuv24fromXYZ","uv_encode","TIFFClientOpen","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFWriteDirectory","TIFFClose","TIFFClose","TIFFSetErrorHandlerExt"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to a temporary file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open the input file with libtiff
    in_file = fopen("input_file", "rb");
    if (!in_file) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Get file descriptor for TIFFClientOpen
    int fd = fuzz_fileno(in_file);
    
    // Custom TIFF I/O handlers
    auto readproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, (FILE*)handle);
    };
    
    auto writeproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fwrite(buf, 1, size, (FILE*)handle);
    };
    
    auto seekproc = [](thandle_t handle, toff_t off, int whence) -> toff_t {
        fseek((FILE*)handle, off, whence);
        return ftell((FILE*)handle);
    };
    
    auto closeproc = [](thandle_t handle) -> int {
        return fclose((FILE*)handle);
    };
    
    auto sizeproc = [](thandle_t handle) -> toff_t {
        long pos = ftell((FILE*)handle);
        fseek((FILE*)handle, 0, SEEK_END);
        long size = ftell((FILE*)handle);
        fseek((FILE*)handle, pos, SEEK_SET);
        return size;
    };
    
    auto mapproc = [](thandle_t handle, void** base, toff_t* size) -> int {
        FILE* f = (FILE*)handle;
        long pos = ftell(f);
        fseek(f, 0, SEEK_END);
        long file_size = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        // For simplicity, just return 0 (can't memory map via this interface)
        *base = nullptr;
        *size = file_size;
        return 0;
    };
    
    auto unmapproc = [](thandle_t handle, void* base, toff_t size) -> void {
        // Nothing to do
    };
    
    // Set error handler
    TIFFErrorHandlerExt prev_handler = TIFFSetErrorHandlerExt(nullptr);
    
    // Open TIFF using client interface
    TIFF* tif = TIFFClientOpen("input_file", "r", 
                               (thandle_t)in_file,
                               readproc, writeproc,
                               seekproc, closeproc,
                               sizeproc, mapproc, unmapproc);
    
    if (tif) {
        // 1. Use TIFFMapFileProc TIFFGetMapFileProc(TIFF *)
        TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
        if (map_proc) {
            void* base = nullptr;
            toff_t map_size = 0;
            map_proc((thandle_t)in_file, &base, &map_size);
        }
        
        // 2. Use const TIFFField *TIFFFieldWithName(TIFF *, const char *)
        const TIFFField* field = TIFFFieldWithName(tif, "ImageWidth");
        
        // 3. Use uint64_t TIFFVStripSize64(TIFF *tif, uint32_t nrows)
        uint32_t nrows = 10;
        uint64_t strip_size64 = TIFFVTileSize64(tif, nrows);
        
        // Read directory if exists
        if (TIFFReadDirectory(tif)) {
            // 4. Use uint32_t LogLuv24fromXYZ(float *, int)
            float xyz[3] = {0.5f, 0.5f, 0.5f};
            uint32_t logluv = LogLuv24fromXYZ(xyz, 1);
            
            // 5. Use int uv_encode(double, double, int)
            double up, vp;
            int result = uv_encode(0.5, 0.5, 1);
            
            // Create output file
            FILE *out_file = fopen("output_file", "wb");
            if (out_file) {
                TIFF* tif_out = TIFFClientOpen("output_file", "w",
                                               (thandle_t)out_file,
                                               readproc, writeproc,
                                               seekproc, closeproc,
                                               sizeproc, mapproc, unmapproc);
                
                if (tif_out) {
                    // Try to write a simple directory
                    TIFFSetField(tif_out, TIFFTAG_IMAGEWIDTH, 100);
                    TIFFSetField(tif_out, TIFFTAG_IMAGELENGTH, 100);
                    TIFFSetField(tif_out, TIFFTAG_BITSPERSAMPLE, 8);
                    TIFFSetField(tif_out, TIFFTAG_SAMPLESPERPIXEL, 3);
                    TIFFSetField(tif_out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
                    TIFFSetField(tif_out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
                    TIFFSetField(tif_out, TIFFTAG_ROWSPERSTRIP, 10);
                    
                    // Write directory
                    TIFFWriteDirectory(tif_out);
                    
                    TIFFClose(tif_out);
                }
                assert_file_closed(&out_file);;
                remove("output_file");
            }
        }
        
        TIFFClose(tif);
    } else {
        assert_file_closed(&in_file);;
    }
    
    // Clean up
    remove("input_file");
    
    // Restore previous error handler
    TIFFSetErrorHandlerExt(prev_handler);
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}