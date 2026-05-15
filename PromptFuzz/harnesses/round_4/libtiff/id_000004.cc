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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 7
//<Quality> {"density":18,"unique_branches":{"TIFFWriteDirectorySec":[[659,21,661,77,0,0,4,1],[236,34,236,71,33,0,4,0]],"TIFFWriteDirectoryTagCheckedLong":[[2443,9,2443,20,0,0,4,0],[2443,9,2443,20,0,0,4,1],[2450,9,2450,35,0,0,4,1]],"TIFFWriteDirectoryTagShortLong":[[1908,9,1908,24,0,0,4,1]],"TIFFComputeTile":[[52,9,52,16,0,0,4,1]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFGetField","TIFFGetField","TIFFClientOpenExt","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFGetSizeProc","TIFFGetWriteProc","TIFFComputeTile","TIFFReverseBits","TIFFClose","TIFFClose"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFGetField","TIFFGetField","TIFFClientOpenExt","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFSetField","TIFFGetSizeProc","TIFFGetWriteProc","TIFFComputeTile","TIFFReverseBits","TIFFClose","TIFFClose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
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





    // Write input data to a file for TIFF to read
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Reopen for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Define TIFF I/O callbacks
    auto readproc = [](thandle_t fd, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, (FILE*)fd);
    };
    
    auto writeproc = [](thandle_t fd, void* buf, tmsize_t size) -> tmsize_t {
        return fwrite(buf, 1, size, (FILE*)fd);
    };
    
    auto seekproc = [](thandle_t fd, toff_t off, int whence) -> toff_t {
        fseek((FILE*)fd, off, whence);
        return ftell((FILE*)fd);
    };
    
    auto closeproc = [](thandle_t fd) -> int {
        return fclose((FILE*)fd);
    };
    
    auto sizeproc = [](thandle_t fd) -> toff_t {
        FILE* f = (FILE*)fd;
        long cur = ftell(f);
        fseek(f, 0, SEEK_END);
        long end = ftell(f);
        fseek(f, cur, SEEK_SET);
        return end;
    };
    
    // Create TIFF handle using client open
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    TIFF* tif = TIFFClientOpenExt("input_file", fuzz_str_1, (thandle_t)in_file,
                                 readproc, writeproc, seekproc, closeproc,
                                 sizeproc, nullptr, nullptr, opts);
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 1. Use TIFFSetField - set some basic fields if we're writing
    // First try to read existing fields
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        
        // Create output file for writing
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Create output TIFF
            TIFF* out_tif = TIFFClientOpenExt("output_file", fuzz_str_2, (thandle_t)out_file,
                                            readproc, writeproc, seekproc, closeproc,
                                            sizeproc, nullptr, nullptr, nullptr);
            
            if (out_tif) {
                // Use TIFFSetField to set basic image parameters
                TIFFSetField(out_tif, TIFFTAG_IMAGEWIDTH, width);
                TIFFSetField(out_tif, TIFFTAG_IMAGELENGTH, height);
                TIFFSetField(out_tif, TIFFTAG_BITSPERSAMPLE, 8);
                TIFFSetField(out_tif, TIFFTAG_SAMPLESPERPIXEL, 1);
                TIFFSetField(out_tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
                TIFFSetField(out_tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
                TIFFSetField(out_tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
                
                // 2. Use TIFFGetSizeProc
                TIFFSizeProc size_func = TIFFGetSizeProc(out_tif);
                if (size_func) {
                    toff_t file_size = size_func((thandle_t)out_file);
                    (void)file_size; // Use variable to avoid unused warning
                }
                
                // 3. Use TIFFGetWriteProc
                TIFFReadWriteProc write_func = TIFFGetWriteProc(out_tif);
                if (write_func) {
                    // Write some dummy data
                    uint8_t buffer[100];
                    memset(buffer, 0, sizeof(buffer));
                    write_func((thandle_t)out_file, buffer, sizeof(buffer));
                }
                
                // 4. Use TIFFComputeTile
                // Compute tile index for a specific coordinate
                uint32_t tile = TIFFComputeTile(out_tif, fuzz_uint32_t_3, fuzz_uint32_t_4, fuzz_uint32_t_5, fuzz_uint16_t_6);
                (void)tile; // Use variable to avoid unused warning
                
                // 5. Use TIFFReverseBits
                // Prepare some data and reverse its bits
                uint8_t bits_data[50];
                for (size_t i = 0; i < sizeof(bits_data); i++) {
                    bits_data[i] = (uint8_t)(i % 256);
                }
                TIFFReverseBits(bits_data, sizeof(bits_data));
                
                TIFFClose(out_tif);
            }
            assert_file_closed(&out_file);;
        }
    }
    
    // Clean up
    TIFFClose(tif);
    // Note: TIFFClose will close the file via closeproc
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}