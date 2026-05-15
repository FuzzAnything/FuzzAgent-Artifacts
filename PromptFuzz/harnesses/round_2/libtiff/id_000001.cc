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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 171, nr_unique_branch: 140
//<Quality> {"density":18,"unique_branches":{"TIFFListDestroy":[[264,12,264,21,0,0,4,1]],"TIFFClientOpenExt":[[345,9,345,13,0,0,4,0],[345,17,345,47,0,0,4,1],[356,9,356,13,0,0,4,0],[356,17,356,50,0,0,4,1],[391,9,391,13,0,0,4,0],[402,9,402,18,0,0,4,1],[402,22,402,32,0,0,4,1],[402,36,402,45,0,0,4,1],[402,49,402,59,0,0,4,1],[402,63,402,72,0,0,4,1],[417,9,417,22,0,0,4,0],[421,9,421,22,0,0,4,0],[480,21,480,24,0,0,4,0],[480,21,480,24,0,0,4,1],[483,13,483,21,0,0,4,1],[489,13,489,21,0,0,4,1],[495,13,495,21,0,0,4,1],[499,13,499,21,0,0,4,1],[503,13,503,21,0,0,4,1],[511,13,511,21,0,0,4,1],[515,13,515,21,0,0,4,1],[519,13,519,21,0,0,4,1],[523,13,523,21,0,0,4,1],[527,13,527,21,0,0,4,1],[530,13,530,21,0,0,4,1],[534,13,534,21,0,0,4,1],[537,13,537,21,0,0,4,1],[542,13,542,20,0,0,4,0],[556,9,556,22,0,0,4,1],[557,9,557,16,0,0,4,0],[557,9,557,16,0,0,4,1],[559,13,559,38,0,0,4,0],[633,9,633,60,0,0,4,0],[633,9,633,60,0,0,4,1],[634,9,634,63,0,0,4,0],[634,9,634,63,0,0,4,1],[640,9,640,62,0,0,4,0],[640,9,640,62,0,0,4,1],[658,9,658,60,0,0,4,0],[658,9,658,60,0,0,4,1],[670,9,670,35,0,0,4,0],[670,9,670,35,0,0,4,1],[672,9,672,70,0,0,4,0],[672,9,672,70,0,0,4,1],[673,9,673,66,0,0,4,0],[673,9,673,66,0,0,4,1],[682,9,682,68,0,0,4,0],[682,9,682,68,0,0,4,1],[684,13,684,39,0,0,4,0],[684,13,684,39,0,0,4,1],[690,13,690,20,0,0,4,0],[690,13,690,20,0,0,4,1],[697,13,697,39,0,0,4,1],[702,13,702,53,0,0,4,0],[702,13,702,53,0,0,4,1],[711,13,711,49,0,0,4,0],[711,13,711,49,0,0,4,1],[731,9,731,17,0,0,4,0],[732,17,732,49,0,0,4,0],[732,17,732,49,0,0,4,1],[741,17,741,45,0,0,4,0],[762,17,762,49,0,0,4,1],[772,17,772,39,0,0,4,1],[777,9,777,17,0,0,4,1],[786,9,786,16,0,0,4,1],[295,5,295,69,50,0,4,1]],"TIFFHashSetNew":[[143,9,143,20,0,0,4,1],[145,23,145,33,0,0,4,0],[146,24,146,35,0,0,4,0],[150,9,150,29,0,0,4,1]],"TIFFHashSetGetNewListElt":[[192,9,192,29,0,0,4,1]],"_TIFFCheckDirNumberAndOffset":[[5762,9,5762,20,0,0,4,1],[5765,9,5765,50,0,0,4,0],[5769,13,5769,54,0,0,4,1],[5777,9,5777,50,0,0,4,0],[5783,13,5783,54,0,0,4,1],[5803,9,5803,19,0,0,4,1],[5824,9,5824,19,0,0,4,1],[5879,9,5880,27,0,0,4,1],[5890,9,5890,25,0,0,4,1],[5900,9,5900,72,0,0,4,1],[5906,9,5906,72,0,0,4,1]],"TIFFHashSetFindPtr":[[414,12,414,15,0,0,4,1]],"_TIFFSeekOK":[[416,12,416,37,0,0,4,0],[416,12,416,37,0,0,4,1],[416,41,416,80,0,0,4,1]],"_TIFFCleanupIFDOffsetAndNumberMaps":[[129,9,129,42,0,0,4,0],[129,9,129,42,0,0,4,1],[134,9,134,42,0,0,4,0],[134,9,134,42,0,0,4,1]],"TIFFHashSetInsert":[[445,9,445,13,0,0,4,1],[454,9,454,50,0,0,4,1],[455,10,455,22,0,0,4,1],[476,9,476,24,0,0,4,1]],"TIFFReadDirectory":[[4337,9,4337,33,0,0,4,0],[4337,9,4337,33,0,0,4,1],[4350,9,4355,50,0,0,4,1],[4351,39,4352,75,0,0,4,0],[4361,9,4361,18,0,0,4,0]],"TIFFFetchDirectory":[[6089,9,6089,19,0,0,4,0],[6091,9,6091,18,0,0,4,0],[6093,13,6093,20,0,0,4,0]],"TIFFHashSetDestroy":[[287,9,287,12,0,0,4,0]],"TIFFFreeDirectory":[[1637,17,1637,44,0,0,4,1],[1652,9,1652,52,0,0,4,1],[1601,13,1601,23,1,0,4,1],[1601,13,1601,23,2,0,4,1],[1601,13,1601,23,3,0,4,1],[1601,13,1601,23,4,0,4,1],[1601,13,1601,23,5,0,4,1],[1601,13,1601,23,6,0,4,1],[1601,13,1601,23,7,0,4,1],[1601,13,1601,23,8,0,4,1],[1601,13,1601,23,9,0,4,1],[1601,13,1601,23,10,0,4,1],[1601,13,1601,23,11,0,4,1],[1601,13,1601,23,12,0,4,1],[1601,13,1601,23,13,0,4,1],[1601,13,1601,23,14,0,4,1],[1601,13,1601,23,17,0,4,1]],"TIFFHashSetLookup":[[508,9,508,13,0,0,4,1]],"_TIFFSetDefaultPostDecode":[[152,9,152,35,0,0,4,1]],"TIFFCleanup":[[49,9,49,34,0,0,4,1],[58,12,58,31,0,0,4,1],[67,9,67,25,0,0,4,1],[75,9,75,24,0,0,4,1],[100,9,100,35,0,0,4,1],[112,9,112,46,0,0,4,1],[283,23,283,62,2,0,4,0],[283,23,283,62,2,0,4,1]],"_TIFFcalloc":[[344,9,344,19,0,0,4,1],[344,23,344,31,0,0,4,1]],"TIFFHashSetClearInternal":[[229,21,229,44,0,0,4,0],[229,21,229,44,0,0,4,1],[232,16,232,19,0,0,4,0],[232,16,232,19,0,0,4,1],[234,17,234,35,0,0,4,0],[234,17,234,35,0,0,4,1],[237,17,237,26,0,0,4,0]]},"library_calls":["TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFOpenOptionsFree","TIFFAccessTagMethods","TIFFIsMSB2LSB","TIFFScanlineSize","TIFFNumberOfStrips","TIFFRawStripSize","TIFFReadRawStrip","_TIFFmemcpy","TIFFSwabArrayOfShort","_TIFFmemcpy","TIFFSwabArrayOfFloat","_TIFFmemcpy","TIFFNumberOfDirectories","TIFFUnlinkDirectory","TIFFClose","TIFFOpenOptionsFree"],"critical_calls":["TIFFOpenOptionsAlloc","TIFFClientOpenExt","TIFFAccessTagMethods","TIFFIsMSB2LSB","TIFFScanlineSize","TIFFNumberOfStrips","TIFFRawStripSize","TIFFReadRawStrip","_TIFFmemcpy","TIFFSwabArrayOfShort","_TIFFmemcpy","TIFFSwabArrayOfFloat","_TIFFmemcpy","TIFFNumberOfDirectories","TIFFUnlinkDirectory","TIFFClose","TIFFOpenOptionsFree"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
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
    
    // Reopen file for reading
    in_file = fopen("input_file", "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for TIFF client operations
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Custom I/O handlers for TIFF client interface
    auto readproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, reinterpret_cast<FILE*>(handle));
    };
    
    auto writeproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fwrite(buf, 1, size, reinterpret_cast<FILE*>(handle));
    };
    
    auto seekproc = [](thandle_t handle, toff_t off, int whence) -> toff_t {
        return fseek(reinterpret_cast<FILE*>(handle), off, whence);
    };
    
    auto closeproc = [](thandle_t handle) -> int {
        return fclose(reinterpret_cast<FILE*>(handle));
    };
    
    auto sizeproc = [](thandle_t handle) -> toff_t {
        FILE* f = reinterpret_cast<FILE*>(handle);
        long pos = ftell(f);
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        fseek(f, pos, SEEK_SET);
        return size;
    };
    
    // Create TIFF using client interface
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    TIFF* tif = TIFFClientOpenExt("input_file", "r", (thandle_t)in_file,
                                  readproc, writeproc, seekproc, closeproc,
                                  sizeproc, NULL, NULL, opts);
    if (!tif) {
        TIFFOpenOptionsFree(opts);
        assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Access tag methods
    TIFFTagMethods* tag_methods = TIFFAccessTagMethods(tif);
    if (tag_methods) {
        // Check byte order
        int is_swapped = TIFFIsMSB2LSB(tif);
        
        // Get scanline size
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        
        // Read and process raw strips if available
        uint32_t num_strips = TIFFNumberOfStrips(tif);
        if (num_strips > 0) {
            // Allocate buffer for strip data
            uint32_t strip = 0;
            tmsize_t raw_size = TIFFRawStripSize(tif, strip);
            
            if (raw_size > 0) {
                uint8_t* buffer = new uint8_t[raw_size];
                tmsize_t read_size = TIFFReadRawStrip(tif, strip, buffer, raw_size);
                
                if (read_size > 0) {
                    // Create buffers for byte swapping
                    size_t short_count = read_size / sizeof(uint16_t);
                    size_t float_count = read_size / sizeof(float);
                    
                    if (short_count > 0) {
                        uint16_t* short_buffer = new uint16_t[short_count];
                        _TIFFmemcpy(short_buffer, buffer, short_count * sizeof(uint16_t));
                        TIFFSwabArrayOfShort(short_buffer, short_count);
                        delete[] short_buffer;
                    }
                    
                    if (float_count > 0) {
                        float* float_buffer = new float[float_count];
                        _TIFFmemcpy(float_buffer, buffer, float_count * sizeof(float));
                        TIFFSwabArrayOfFloat(float_buffer, float_count);
                        delete[] float_buffer;
                    }
                    
                    // Create a copy of the buffer
                    uint8_t* buffer_copy = new uint8_t[read_size];
                    _TIFFmemcpy(buffer_copy, buffer, read_size);
                    delete[] buffer_copy;
                }
                
                delete[] buffer;
            }
        }
        
        // Try to unlink first directory if multiple exist
        tdir_t num_dirs = TIFFNumberOfDirectories(tif);
        if (num_dirs > 1) {
            TIFFUnlinkDirectory(tif, 0);
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    TIFFOpenOptionsFree(opts);
    
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}