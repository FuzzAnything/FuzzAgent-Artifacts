#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "jpeglib.h"
#include "jerror.h"

namespace harness_011 {
/*
 * Fuzzing harness for libjpeg-turbo targeting 12-bit JPEG compression and decompression operations
 * This harness specifically targets 12-bit JPEG operations which have 0% coverage but high branch complexity
 * 
 * Target 12-bit APIs:
 * - jpeg12_write_raw_data (Target: completely uncovered, 18 branches)
 * - jpeg12_read_raw_data (Target: completely uncovered, 16 branches)
 * - jpeg12_skip_scanlines (Target: 0% hit despite static reachability)
 * - jpeg12_crop_scanline (Target: 0% hit despite static reachability)
 * - jpeg12_write_scanlines (Target: completely uncovered, 20 branches)
 * - jpeg12_read_scanlines (Target: completely uncovered, 16 branches)
 * 
 * Required Helper APIs for lifecycle:
 * - jpeg_CreateCompress (Initialization)
 * - jpeg_CreateDecompress (Initialization)
 * - jpeg_mem_src (Input setup)
 * - jpeg_mem_dest (Output setup)
 * - jpeg_set_defaults (Configuration)
 * - jpeg_set_colorspace (Configuration)
 * - jpeg_set_quality (Configuration)
 * - jpeg_destroy_compress (Cleanup)
 * - jpeg_destroy_decompress (Cleanup)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000-010: All focus on 8-bit JPEG, TurboJPEG, or PNG operations
 * - This harness: Focuses EXCLUSIVELY on 12-bit JPEG operations using the classic libjpeg API
 * 
 * Coverage strategy:
 * 1. Comprehensive 12-bit JPEG encode/decode cycles with various formats
 * 2. Both scanline-based and raw data writing/reading paths
 * 3. Advanced operations: skip_scanlines and crop_scanline
 * 4. Different color spaces and sampling factors
 * 5. Error handling paths in 12-bit API
 */

struct memory_source_mgr {
    struct jpeg_source_mgr pub;   // public fields
    const JOCTET* data;           // pointer to fuzzer input
    size_t size;                  // size of input data
    boolean start_of_file;        // have we started reading?
};

// Note: memory_dest_mgr is no longer used since we're using standard jpeg_mem_dest
// The standard jpeg_mem_dest function uses unsigned char** and unsigned long* parameters

// Memory source manager functions
static void init_source(j_decompress_ptr cinfo) {
    memory_source_mgr* src = (memory_source_mgr*)cinfo->src;
    src->start_of_file = TRUE;
}

static boolean fill_input_buffer(j_decompress_ptr cinfo) {
    memory_source_mgr* src = (memory_source_mgr*)cinfo->src;
    if (src->size == 0) {
        // Insert a fake EOI marker
        static const JOCTET eoi_buffer[2] = {0xFF, JPEG_EOI};
        src->pub.next_input_byte = eoi_buffer;
        src->pub.bytes_in_buffer = 2;
        src->start_of_file = FALSE;
    } else {
        src->pub.next_input_byte = src->data;
        src->pub.bytes_in_buffer = src->size;
        src->data += src->size;
        src->size = 0;
    }
    return TRUE;
}

static void skip_input_data(j_decompress_ptr cinfo, long num_bytes) {
    memory_source_mgr* src = (memory_source_mgr*)cinfo->src;
    if (num_bytes > 0) {
        while (num_bytes > (long)src->pub.bytes_in_buffer) {
            num_bytes -= (long)src->pub.bytes_in_buffer;
            (void)fill_input_buffer(cinfo);
        }
        src->pub.next_input_byte += (size_t)num_bytes;
        src->pub.bytes_in_buffer -= (size_t)num_bytes;
    }
}

static void term_source(j_decompress_ptr cinfo) {
    // Nothing to do
}

static void jpeg_memory_src(j_decompress_ptr cinfo, const uint8_t* data, size_t size) {
    memory_source_mgr* src;
    
    if (cinfo->src == NULL) {
        cinfo->src = (struct jpeg_source_mgr*)
            (*cinfo->mem->alloc_small)((j_common_ptr)cinfo, JPOOL_PERMANENT,
                                      sizeof(memory_source_mgr));
    }
    
    src = (memory_source_mgr*)cinfo->src;
    src->pub.init_source = init_source;
    src->pub.fill_input_buffer = fill_input_buffer;
    src->pub.skip_input_data = skip_input_data;
    src->pub.resync_to_restart = jpeg_resync_to_restart;
    src->pub.term_source = term_source;
    src->data = data;
    src->size = size;
    src->pub.bytes_in_buffer = 0;
    src->pub.next_input_byte = NULL;
}

// Note: Removed custom jpeg_memory_dest implementation to use standard jpeg_mem_dest instead
// The standard jpeg_mem_dest function will be used directly

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful 12-bit JPEG operations
    // We need parameters + at least some image data
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    JDIMENSION width = fdp.ConsumeIntegralInRange<JDIMENSION>(8, 256);
    JDIMENSION height = fdp.ConsumeIntegralInRange<JDIMENSION>(8, 256);
    int quality = fdp.ConsumeIntegralInRange<int>(1, 100);
    J_COLOR_SPACE color_space = static_cast<J_COLOR_SPACE>(
        fdp.ConsumeIntegralInRange<int>(JCS_UNKNOWN, JCS_YCCK));
    int in_color_space = fdp.ConsumeIntegralInRange<int>(JCS_UNKNOWN, JCS_YCCK);
    int num_components = fdp.ConsumeIntegralInRange<int>(1, 4);
    int data_precision = 12; // Force 12-bit for this harness
    boolean write_all_tables = fdp.ConsumeBool();
    boolean raw_data_in = fdp.ConsumeBool();
    boolean raw_data_out = fdp.ConsumeBool();
    int operation_type = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Validate color space
    if (color_space < JCS_UNKNOWN || color_space > JCS_YCCK) {
        color_space = JCS_RGB;
    }
    if (in_color_space < JCS_UNKNOWN || in_color_space > JCS_YCCK) {
        in_color_space = JCS_RGB;
    }
    
    // Validate number of components based on color space
    if (color_space == JCS_GRAYSCALE && num_components != 1) {
        num_components = 1;
    } else if ((color_space == JCS_RGB || color_space == JCS_YCbCr) && num_components != 3) {
        num_components = 3;
    } else if ((color_space == JCS_CMYK || color_space == JCS_YCCK) && num_components != 4) {
        num_components = 4;
    }
    
    // Prepare 12-bit sample data for compression
    // Calculate required buffer size for 12-bit samples
    size_t row_stride = width * num_components;
    size_t image_size = height * row_stride;
    
    // Declare sample_buffer here so it's in scope for the whole function
    std::vector<J12SAMPLE> sample_buffer;
    
    // Check if we have enough input data
    if (fdp.remaining_bytes() < image_size * sizeof(J12SAMPLE)) {
        // Not enough data for full image, but we can still test with partial data
        // Allocate buffer and fill with zeros
        sample_buffer.resize(image_size, 0);
        
        // Fill with available data
        size_t bytes_to_copy = fdp.ConsumeData(sample_buffer.data(), 
                                             std::min(fdp.remaining_bytes(), 
                                                     image_size * sizeof(J12SAMPLE)));
        if (bytes_to_copy == 0) {
            return 0; // Not enough data even for partial testing
        }
    } else {
        // We have enough data for full image
        sample_buffer.resize(image_size);
        size_t bytes_to_copy = image_size * sizeof(J12SAMPLE);
        fdp.ConsumeData(sample_buffer.data(), bytes_to_copy);
    }
    
    // Create scanline array for 12-bit data
    J12SAMPARRAY scanlines = (J12SAMPARRAY)malloc(height * sizeof(J12SAMPROW));
    if (!scanlines) return 0;
    
    for (JDIMENSION row = 0; row < height; row++) {
        scanlines[row] = &sample_buffer[row * row_stride];
    }
    
    // Create raw data array for 12-bit data (3D array)
    J12SAMPIMAGE raw_data = (J12SAMPIMAGE)malloc(num_components * sizeof(J12SAMPARRAY));
    if (!raw_data) {
        free(scanlines);
        return 0;
    }
    
    for (int comp = 0; comp < num_components; comp++) {
        raw_data[comp] = (J12SAMPARRAY)malloc(height * sizeof(J12SAMPROW));
        if (!raw_data[comp]) {
            // Cleanup
            for (int c = 0; c < comp; c++) {
                free(raw_data[c]);
            }
            free(raw_data);
            free(scanlines);
            return 0;
        }
        
        for (JDIMENSION row = 0; row < height; row++) {
            raw_data[comp][row] = &sample_buffer[row * row_stride + comp];
        }
    }
    
    // Test 1: Compression path with 12-bit data
    {
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_CreateCompress(&cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
        
        // Set up memory destination using standard jpeg_mem_dest
        unsigned char* jpeg_buffer = nullptr;
        unsigned long jpeg_size = 0;
        jpeg_mem_dest(&cinfo, &jpeg_buffer, &jpeg_size);
        
        // Set compression parameters
        cinfo.image_width = width;
        cinfo.image_height = height;
        cinfo.input_components = num_components;
        cinfo.in_color_space = (J_COLOR_SPACE)in_color_space;
        cinfo.data_precision = data_precision;
        cinfo.raw_data_in = raw_data_in;
        
        // Choose operation based on operation_type
        switch (operation_type % 2) {
            case 0:
                // Write scanlines
                while (cinfo.next_scanline < cinfo.image_height) {
                    JDIMENSION lines_to_write = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 4);
                    lines_to_write = std::min(lines_to_write, cinfo.image_height - cinfo.next_scanline);
                    
                    J12SAMPARRAY scanlines_ptr = &scanlines[cinfo.next_scanline];
                    jpeg12_write_scanlines(&cinfo, scanlines_ptr, lines_to_write);
                }
                break;
                
            case 1:
                // Write raw data
                if (cinfo.raw_data_in) {
                    while (cinfo.next_scanline < cinfo.image_height) {
                        JDIMENSION lines_to_write = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 4);
                        lines_to_write = std::min(lines_to_write, cinfo.image_height - cinfo.next_scanline);
                        
                        jpeg12_write_raw_data(&cinfo, raw_data, lines_to_write);
                    }
                }
                break;
        }
        
        // Finish compression
        jpeg_finish_compress(&cinfo);
        
        // Cleanup
        jpeg_destroy_compress(&cinfo);
        // jpeg_mem_dest allocates memory that needs to be freed
        if (jpeg_buffer) {
            free(jpeg_buffer);
        }
    }
    
    // Test 2: Decompression path with 12-bit data
    // First we need to create some JPEG data to decompress
    // We'll create a simple JPEG buffer using the compression path
    unsigned char* decomp_jpeg_buffer = nullptr;
    unsigned long decomp_jpeg_size = 0;
    {
        struct jpeg_compress_struct cinfo;
        struct jpeg_error_mgr jerr;
        
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_CreateCompress(&cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
        
        // Use standard jpeg_mem_dest
        jpeg_mem_dest(&cinfo, &decomp_jpeg_buffer, &decomp_jpeg_size);
        
        cinfo.image_width = width;
        cinfo.image_height = height;
        cinfo.input_components = num_components;
        cinfo.in_color_space = (J_COLOR_SPACE)in_color_space;
        cinfo.data_precision = data_precision;
        
        jpeg_set_defaults(&cinfo);
        jpeg_set_colorspace(&cinfo, color_space);
        jpeg_set_quality(&cinfo, 75, TRUE); // Use fixed quality for test data
        
        jpeg_start_compress(&cinfo, TRUE);
        
        // Write minimal data
        JDIMENSION lines_to_write = std::min(height, static_cast<JDIMENSION>(4));
        for (JDIMENSION i = 0; i < lines_to_write; i++) {
            J12SAMPARRAY scanline_ptr = &scanlines[i];
            jpeg12_write_scanlines(&cinfo, scanline_ptr, 1);
        }
        
        jpeg_finish_compress(&cinfo);
        jpeg_destroy_compress(&cinfo);
    }
    
    if (decomp_jpeg_buffer && decomp_jpeg_size > 0) {
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
        
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_CreateDecompress(&cinfo, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
        
        // Use standard jpeg_mem_src
        jpeg_mem_src(&cinfo, decomp_jpeg_buffer, decomp_jpeg_size);
        
        // Read header
        if (jpeg_read_header(&cinfo, TRUE) == JPEG_HEADER_OK) {
            // Start decompression
            jpeg_start_decompress(&cinfo);
            // Allocate output buffers for 12-bit data
            JDIMENSION output_width = cinfo.output_width;
            JDIMENSION output_height = cinfo.output_height;
            int output_components = cinfo.output_components;
            
            size_t output_row_stride = output_width * output_components;
            std::vector<J12SAMPLE> output_buffer(output_height * output_row_stride);
            
            J12SAMPARRAY output_scanlines = (J12SAMPARRAY)malloc(output_height * sizeof(J12SAMPROW));
            if (output_scanlines) {
                for (JDIMENSION row = 0; row < output_height; row++) {
                    output_scanlines[row] = &output_buffer[row * output_row_stride];
                }
                
                // Test different decompression operations
                switch ((operation_type + 1) % 4) {
                    case 0:
                        // Read scanlines
                        while (cinfo.output_scanline < cinfo.output_height) {
                            JDIMENSION lines_to_read = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 4);
                            lines_to_read = std::min(lines_to_read, cinfo.output_height - cinfo.output_scanline);
                            
                            J12SAMPARRAY scanlines_ptr = &output_scanlines[cinfo.output_scanline];
                            jpeg12_read_scanlines(&cinfo, scanlines_ptr, lines_to_read);
                        }
                        break;
                        
                    case 1:
                        // Skip scanlines
                        if (cinfo.output_scanline < cinfo.output_height) {
                            JDIMENSION lines_to_skip = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 
                                std::min(cinfo.output_height - cinfo.output_scanline, static_cast<JDIMENSION>(10)));
                            jpeg12_skip_scanlines(&cinfo, lines_to_skip);
                        }
                        break;
                        
                    case 2:
                        // Crop scanline
                        if (cinfo.output_width > 0) {
                            JDIMENSION xoffset = fdp.ConsumeIntegralInRange<JDIMENSION>(0, cinfo.output_width - 1);
                            JDIMENSION crop_width = fdp.ConsumeIntegralInRange<JDIMENSION>(1, cinfo.output_width - xoffset);
                            jpeg12_crop_scanline(&cinfo, &xoffset, &crop_width);
                        }
                        break;
                        
                    case 3:
                        // Read raw data (if supported)
                        if (cinfo.raw_data_out) {
                            // Allocate raw data buffers
                            J12SAMPIMAGE raw_output = (J12SAMPIMAGE)malloc(output_components * sizeof(J12SAMPARRAY));
                            if (raw_output) {
                                for (int comp = 0; comp < output_components; comp++) {
                                    raw_output[comp] = (J12SAMPARRAY)malloc(output_height * sizeof(J12SAMPROW));
                                    if (raw_output[comp]) {
                                        for (JDIMENSION row = 0; row < output_height; row++) {
                                            raw_output[comp][row] = &output_buffer[row * output_row_stride + comp];
                                        }
                                    }
                                }
                                
                                while (cinfo.output_scanline < cinfo.output_height) {
                                    JDIMENSION lines_to_read = fdp.ConsumeIntegralInRange<JDIMENSION>(1, 4);
                                    lines_to_read = std::min(lines_to_read, cinfo.output_height - cinfo.output_scanline);
                                    
                                    jpeg12_read_raw_data(&cinfo, raw_output, lines_to_read);
                                }
                                
                                // Cleanup raw output buffers
                                for (int comp = 0; comp < output_components; comp++) {
                                    if (raw_output[comp]) {
                                        free(raw_output[comp]);
                                    }
                                }
                                free(raw_output);
                            }
                        }
                        break;
                }
                
                free(output_scanlines);
            }
            
            // Finish decompression
            jpeg_finish_decompress(&cinfo);
        }
        
        jpeg_destroy_decompress(&cinfo);
    }
    
    // Cleanup allocated memory
    for (int comp = 0; comp < num_components; comp++) {
        free(raw_data[comp]);
    }
    free(raw_data);
    free(scanlines);
    
    return 0;
}

} // namespace harness_011
