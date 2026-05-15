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
//<ID> 644
//<Prompt> []
/*<Combination>: [
*/
//<score> 50, nr_unique_branch: 15
//<Quality> {"density":15,"unique_branches":{"TIFFCheckTile":[[75,9,75,31,0,0,4,0]],"TIFFReadTile":[[961,9,961,31,0,0,4,1],[961,35,961,66,0,0,4,1]],"TIFFWriteTile":[[392,9,392,40,0,0,4,0]],"TIFFCIELab16ToXYZ":[[61,9,61,19,0,0,4,1],[73,9,73,22,0,0,4,1],[79,9,79,22,0,0,4,1]],"TIFFReadAndRealloc":[[98,13,98,75,0,0,4,1]],"TIFFReadEncodedTile":[[1007,9,1007,33,0,0,4,1],[1014,14,1015,78,0,0,4,0],[1014,14,1015,78,0,0,4,1]],"TIFFSwabArrayOfDouble":[[228,12,228,19,0,0,4,0],[228,12,228,19,0,0,4,1]],"TIFFFillTile":[[1331,17,1331,50,0,0,4,1]],"TIFFStartTile":[[1493,9,1493,48,0,0,4,1]]},"library_calls":["TIFFClientOpen","TIFFClientOpen","TIFFClose","TIFFGetWriteProc","TIFFGetStrileByteCountWithErr","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFTileSize64","TIFFTileSize","TIFFReadTile","TIFFWriteTile","TIFFSwabArrayOfDouble","TIFFCIELabToXYZ","TIFFReadDirectory","TIFFClose","TIFFClose"],"critical_calls":["TIFFClientOpen","TIFFClientOpen","TIFFGetWriteProc","TIFFGetStrileByteCountWithErr","TIFFIsTiled","TIFFGetField","TIFFGetField","TIFFTileSize64","TIFFTileSize","TIFFReadTile","TIFFWriteTile","TIFFSwabArrayOfDouble","TIFFCIELabToXYZ","TIFFReadDirectory","TIFFClose","TIFFClose"],"visited":2}
/*Here's a C++ fuzz driver that uses the specified libtiff APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<69) return 0;

	
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
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_9, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_10, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Write input data to file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open input file for reading
    FILE *input_fp = fopen("input_file", "rb");
    if (!input_fp) {
	assert_file_closed(&input_fp);
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *output_fp = fopen("output_file", "wb");
    if (!output_fp) {
        assert_file_closed(&input_fp);;
        assert_file_closed(&output_fp);
	assert_file_closed(&input_fp);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int input_fd = fuzz_fileno(input_fp);
    int output_fd = fuzz_fileno(output_fp);
    
    // Create custom I/O handlers
    auto readproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fread(buf, 1, size, reinterpret_cast<FILE*>(handle));
    };
    
    auto writeproc = [](thandle_t handle, void* buf, tmsize_t size) -> tmsize_t {
        return fwrite(buf, 1, size, reinterpret_cast<FILE*>(handle));
    };
    
    auto seekproc = [](thandle_t handle, toff_t off, int whence) -> toff_t {
        fseek(reinterpret_cast<FILE*>(handle), off, whence);
        return ftell(reinterpret_cast<FILE*>(handle));
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
    
    // Open input TIFF
    TIFF* tif_in = TIFFClientOpen(fuzz_str_1, fuzz_str_2, 
        reinterpret_cast<thandle_t>(input_fp),
        readproc, writeproc, seekproc, closeproc,
        sizeproc, nullptr, nullptr);
    
    if (!tif_in) {
        assert_file_closed(&input_fp);;
        assert_file_closed(&output_fp);;
        assert_file_closed(&output_fp);
	assert_file_closed(&input_fp);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
    }
    
    // Open output TIFF
    TIFF* tif_out = TIFFClientOpen(fuzz_str_3, fuzz_str_4,
        reinterpret_cast<thandle_t>(output_fp),
        readproc, writeproc, seekproc, closeproc,
        sizeproc, nullptr, nullptr);
    
    if (!tif_out) {
        TIFFClose(tif_in);
        assert_file_closed(&output_fp);;
        assert_file_closed(&output_fp);
	assert_file_closed(&input_fp);
	assert_file_closed(&in_file);
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
    }
    
    // Get close procedure (though not in provided list, we'll use TIFFGetWriteProc as alternative)
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif_in);
    
    // Try to read directories
    int dir_count = 0;
    do {
        dir_count++;
        
        // Get strile byte count for first strip/tile
        int err = 0;
        uint64_t strile_size = TIFFGetStrileByteCountWithErr(tif_in, fuzz_uint32_t_5, &err);
        
        // Check if tiled
        int is_tiled = TIFFIsTiled(tif_in);
        
        if (is_tiled) {
            // Get tile dimensions
            uint32_t tile_width = 0, tile_height = 0;
            TIFFGetField(tif_in, TIFFTAG_TILEWIDTH, &tile_width);
            TIFFGetField(tif_in, TIFFTAG_TILELENGTH, &tile_height);
            
            if (tile_width > 0 && tile_height > 0) {
                // Calculate tile size
                uint64_t tile_size64 = TIFFTileSize64(tif_in);
                tmsize_t tile_size = TIFFTileSize(tif_in);
                
                if (tile_size > 0) {
                    std::vector<uint8_t> tile_buffer(tile_size);
                    
                    // Read first tile
                    tmsize_t read_size = TIFFReadTile(tif_in, tile_buffer.data(), 
                        fuzz_uint32_t_6, fuzz_uint32_t_7, fuzz_uint32_t_8, fuzz_uint16_t_9);
                    
                    if (read_size > 0) {
                        // Write tile to output
                        tmsize_t write_size = TIFFWriteTile(tif_out, tile_buffer.data(),
                            fuzz_uint32_t_10, fuzz_uint32_t_11, fuzz_uint32_t_12, fuzz_uint16_t_13);
                    }
                }
            }
        }
        
        // Test byte swapping on double array
        double double_array[4] = {1.0, 2.0, 3.0, 4.0};
        TIFFSwabArrayOfDouble(double_array, 4);
        
        // Setup CIELab conversion structure
        TIFFCIELabToRGB cielab;
        memset(&cielab, 0, sizeof(cielab));
        cielab.range = 100;
        
        // Convert Lab to XYZ (dummy values)
        float X, Y, Z;
        TIFFCIELabToXYZ(&cielab, fuzz_uint32_t_14, fuzz_int32_t_15, fuzz_int32_t_16, &X, &Y, &Z);
        
    } while (TIFFReadDirectory(tif_in));
    
    // Cleanup
    TIFFClose(tif_in);
    TIFFClose(tif_out);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&output_fp);
	assert_file_closed(&input_fp);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
}