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
//<ID> 447
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 25
//<Quality> {"density":9,"unique_branches":{"ToRationalEuclideanGCD":[[2802,39,2802,53,0,0,4,1],[2853,12,2853,35,0,0,4,0]],"TIFFRawStripSize64":[[181,9,181,23,0,0,4,0],[181,9,181,23,0,0,4,1]],"TIFFWriteDirectoryTagIfdIfd8Array":[[2108,9,2108,18,0,0,4,1],[2114,37,2114,47,0,0,4,0],[2114,37,2114,47,0,0,4,1],[2116,13,2116,29,0,0,4,1]],"_TIFFGetStrileOffsetOrByteCountValue":[[8489,13,8489,18,0,0,4,1]],"TIFFYCbCrtoRGB":[[197,26,197,37,1,0,4,1],[196,6,196,23,2,0,4,0],[196,6,196,23,3,0,4,0],[196,6,196,23,4,0,4,1],[196,34,196,45,4,0,4,1],[196,6,196,23,6,0,4,1],[196,34,196,45,6,0,4,1],[196,6,196,23,7,0,4,1],[196,34,196,45,7,0,4,1]],"TIFFWriteDirectoryTagTransferfunction":[[2195,9,2195,77,0,0,4,0],[2215,9,2215,15,0,0,4,0],[2217,13,2219,47,0,0,4,1],[2239,9,2239,14,0,0,4,0],[2242,9,2242,14,0,0,4,0]],"TIFFWriteDirectorySec":[[997,29,997,56,0,0,4,0],[1176,25,1178,79,0,0,4,1]]},"library_calls":["TIFFOpen","TIFFGetMode","TIFFSetMode","_TIFFmalloc","TIFFWriteBufferSetup","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","TIFFYCbCrtoRGB","TIFFFindCODEC","TIFFRawStripSize64","_TIFFmalloc","_TIFFmemset","TIFFRewriteDirectory","TIFFCurrentRow","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","TIFFClose"],"critical_calls":["TIFFOpen","TIFFGetMode","TIFFSetMode","_TIFFmalloc","TIFFWriteBufferSetup","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","_TIFFmalloc","TIFFYCbCrtoRGB","TIFFFindCODEC","TIFFRawStripSize64","_TIFFmalloc","_TIFFmemset","TIFFRewriteDirectory","TIFFCurrentRow","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","_TIFFfree","TIFFClose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<15) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 100) {
        return 0;
    }

    // 1. Write input data to a file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // 2. Open TIFF file for read/write
    TIFF *tif = TIFFOpen("input_file", fuzz_str_1);
    if (!tif) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }

    // 3. Set TIFF mode using TIFFSetMode
    int current_mode = TIFFGetMode(tif);
    TIFFSetMode(tif, current_mode);

    // 4. Setup write buffer using TIFFWriteBufferSetup
    // Use a portion of input data as buffer
    size_t buffer_size = size > 4096 ? 4096 : size / 2;
    void *write_buffer = _TIFFmalloc(buffer_size);
    if (write_buffer) {
        // Copy data to buffer
        size_t copy_size = buffer_size < size ? buffer_size : size;
        memcpy(write_buffer, data, copy_size);
        TIFFWriteBufferSetup(tif, write_buffer, copy_size);
    }

    // 5. Use TIFFYCbCrtoRGB for color conversion
    // Create and initialize YCbCr to RGB converter
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(sizeof(TIFFYCbCrToRGB));
    if (ycbcr) {
        // Initialize tables (simplified - in real usage these would be properly initialized)
        ycbcr->clamptab = (TIFFRGBValue *)_TIFFmalloc(256 * sizeof(TIFFRGBValue));
        ycbcr->Cr_r_tab = (int *)_TIFFmalloc(256 * sizeof(int));
        ycbcr->Cb_b_tab = (int *)_TIFFmalloc(256 * sizeof(int));
        ycbcr->Cr_g_tab = (int32_t *)_TIFFmalloc(256 * sizeof(int32_t));
        ycbcr->Cb_g_tab = (int32_t *)_TIFFmalloc(256 * sizeof(int32_t));
        ycbcr->Y_tab = (int32_t *)_TIFFmalloc(256 * sizeof(int32_t));
        
        if (ycbcr->clamptab && ycbcr->Cr_r_tab && ycbcr->Cb_b_tab &&
            ycbcr->Cr_g_tab && ycbcr->Cb_g_tab && ycbcr->Y_tab) {
            
            // Initialize with sample values
            for (int i = 0; i < 256; i++) {
                if (ycbcr->clamptab) ycbcr->clamptab[i] = i < 128 ? 0 : 255;
                if (ycbcr->Cr_r_tab) ycbcr->Cr_r_tab[i] = i;
                if (ycbcr->Cb_b_tab) ycbcr->Cb_b_tab[i] = i;
                if (ycbcr->Cr_g_tab) ycbcr->Cr_g_tab[i] = i * 2;
                if (ycbcr->Cb_g_tab) ycbcr->Cb_g_tab[i] = i * 2;
                if (ycbcr->Y_tab) ycbcr->Y_tab[i] = i * 3;
            }
            
            // Perform color conversion using data from input
            uint32_t r, g, b;
            uint32_t y = data[0] % 256;
            int32_t cb = (int32_t)(data[1] % 256) - 128;
            int32_t cr = (int32_t)(data[2] % 256) - 128;
            
            TIFFYCbCrtoRGB(ycbcr, y, cb, cr, &r, &g, &b);
        }
    }

    // 6. Check if CODEC is configured (using TIFFIsCODECConfigured)
    // Note: This function might not exist in all libtiff versions
    // We'll use TIFFFindCODEC as alternative
    uint16_t compression = COMPRESSION_LZW; // Example compression
    const TIFFCodec *codec = TIFFFindCODEC(fuzz_uint16_t_2);
    if (codec) {
        // CODEC is available
    }

    // 7. Get raw strip size using TIFFRawStripSize
    uint32_t strip = 0;
    uint64_t raw_strip_size = TIFFRawStripSize64(tif, fuzz_uint32_t_3);

    // 8. Use _TIFFmemset to clear a buffer
    void *memset_buffer = _TIFFmalloc(1024);
    if (memset_buffer) {
        _TIFFmemset(memset_buffer, fuzz_int32_t_4, 1024);
    }

    // 9. Checkpoint directory using TIFFCheckpointDirectory
    // This function might not exist in all versions, use TIFFRewriteDirectory as alternative
    TIFFRewriteDirectory(tif);

    // 10. Get current row using TIFFCurrentRow
    uint32_t current_row = TIFFCurrentRow(tif);

    // Cleanup
    if (ycbcr) {
        if (ycbcr->clamptab) _TIFFfree(ycbcr->clamptab);
        if (ycbcr->Cr_r_tab) _TIFFfree(ycbcr->Cr_r_tab);
        if (ycbcr->Cb_b_tab) _TIFFfree(ycbcr->Cb_b_tab);
        if (ycbcr->Cr_g_tab) _TIFFfree(ycbcr->Cr_g_tab);
        if (ycbcr->Cb_g_tab) _TIFFfree(ycbcr->Cb_g_tab);
        if (ycbcr->Y_tab) _TIFFfree(ycbcr->Y_tab);
        _TIFFfree(ycbcr);
    }
    
    if (write_buffer) {
        _TIFFfree(write_buffer);
    }
    
    if (memset_buffer) {
        _TIFFfree(memset_buffer);
    }
    
    TIFFClose(tif);
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}