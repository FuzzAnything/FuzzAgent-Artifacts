#include <cstddef>      // for size_t
#include <cstdio>       // for FILE
#include <csetjmp>      // for setjmp/longjmp
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <unistd.h>     // for close()
#include <fcntl.h>      // for mkstemp()
#include "jpeglib.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_016 {
/*
 * Fuzzing harness for libjpeg-turbo targeting JPEG decompression with colormap operations
 * This harness specifically targets APIs with 0% coverage:
 * - jpeg_new_colormap: 0% covered, 8 undiscovered branches
 * - jpeg_input_complete: 0% covered, 4 undiscovered branches  
 * - jpeg_stdio_src: 0% covered, 4 undiscovered branches
 * 
 * Target APIs from coverage guidance:
 * Primary: jpeg_new_colormap, jpeg_input_complete, jpeg_stdio_src
 * Required Helper APIs: jpeg_CreateDecompress, jpeg_read_header, jpeg_start_decompress,
 *                      jpeg_read_scanlines, jpeg_finish_decompress, jpeg_destroy_decompress
 * 
 * Invocation sequence based on coverage guidance:
 * 1. Call jpeg_CreateDecompress to initialize decompression object.
 * 2. Call jpeg_stdio_src to set input source from file or memory buffer.
 * 3. Call jpeg_read_header to read JPEG header.
 * 4. Call jpeg_start_decompress to begin decompression.
 * 5. Call jpeg_new_colormap to create/modify color map (target operation).
 * 6. Call jpeg_read_scanlines to read image data with new colormap.
 * 7. Call jpeg_input_complete to mark input completion (target operation).
 * 8. Call jpeg_finish_decompress to finish decompression.
 * 9. Call jpeg_destroy_decompress to clean up.
 * 
 * Optional compression side for round-trip testing:
 * - jpeg_CreateCompress, jpeg_stdio_dest, jpeg_set_defaults,
 *   jpeg_start_compress, jpeg_write_scanlines, jpeg_finish_compress,
 *   jpeg_destroy_compress
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000.cpp: TurboJPEG compression/decompression
 * - harness_001.cpp: 12-bit JPEG handling
 * - harness_002.cpp: JPEG compression and transformations
 * - harness_003.cpp: ICC profile handling
 * - harness_004.cpp: Memory management APIs
 * - harness_005.cpp: Error handling functions
 * - harness_006.cpp: DCT coefficient manipulation
 * - harness_007.cpp: Color space conversions
 * - harness_008.cpp: Sampling and scaling
 * - harness_009.cpp: Progressive JPEG
 * - harness_010.cpp: Lossless JPEG
 * - harness_011.cpp: TurboJPEG scaling
 * - harness_012.cpp: JPEG buffered image mode
 * - harness_013.cpp: Arithmetic coding
 * - harness_014.cpp: TurboJPEG transformation operations
 * - harness_015.cpp: Marker processing
 * - This harness: Focuses exclusively on colormap operations for indexed color images
 * 
 * Coverage strategy:
 * 1. Test colormap creation and modification during decompression
 * 2. Test jpeg_input_complete to check input completion status
 * 3. Test jpeg_stdio_src with different input sources
 * 4. Test quantized color mode with external colormap support
 * 5. Test error handling for invalid colormap operations
 * 
 * Expected Coverage Gain:
 * - Direct coverage of 3 critical decompression APIs with 16+ undiscovered branches
 * - Exploration of colormap-specific code paths in decompression
 * - Testing of indexed color image handling with external colormaps
 * - Round-trip testing of colormap creation and usage
 */

// Include standard headers first as required by jpeglib.h
// Now include jpeglib.h (which depends on size_t and FILE being defined)

// Custom error handler structure for libjpeg
struct fuzzer_error_mgr {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

static void fuzzer_error_exit(j_common_ptr cinfo) {
  struct fuzzer_error_mgr *fuzz_err = (struct fuzzer_error_mgr *)cinfo->err;
  longjmp(fuzz_err->setjmp_buffer, 1);
}

static void fuzzer_emit_message(j_common_ptr cinfo, int msg_level) {
  // Suppress warning messages during fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum input size: need enough for JPEG header + minimal image + colormap params
  if (size < 128) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Step 1: Consume fixed-size parameters from fuzzer input
  // Image dimensions (small for fuzzing efficiency)
  int width = fdp.ConsumeIntegralInRange<int>(1, 64);
  int height = fdp.ConsumeIntegralInRange<int>(1, 64);
  
  // Color space - focus on indexed color for colormap operations
  int color_space_idx = fdp.ConsumeIntegralInRange<int>(0, 3);
  J_COLOR_SPACE color_space;
  switch (color_space_idx) {
    case 0: color_space = JCS_GRAYSCALE; break;  // Can still use colormap
    case 1: color_space = JCS_RGB; break;        // RGB can use colormap when quantized
    case 2: color_space = JCS_YCbCr; break;      // YCbCr can use colormap when quantized
    default: color_space = JCS_UNKNOWN; break;   // Unknown for testing edge cases
  }
  
  // Number of color components
  int components = fdp.ConsumeIntegralInRange<int>(1, 4);
  
  // Colormap size and quantization parameters
  int desired_colors = fdp.ConsumeIntegralInRange<int>(2, 256);  // Colormap size
  boolean enable_dithering = fdp.ConsumeBool();
  boolean two_pass_quantize = fdp.ConsumeBool();
  
  // Buffered image mode for colormap operations (required for jpeg_new_colormap)
  boolean buffered_image = fdp.ConsumeBool();
  
  // Step 2: Consume JPEG image data (remaining bytes)
  // Calculate required image buffer size
  size_t image_buffer_size = width * height * components;
  
  // Ensure we have enough data for the image
  if (fdp.remaining_bytes() < image_buffer_size) {
    return 0; // Not enough data for image
  }
  
  std::vector<uint8_t> image_data = fdp.ConsumeBytes<uint8_t>(image_buffer_size);
  
  // Create a temporary file to hold JPEG data for jpeg_stdio_src
  char temp_filename[] = "/tmp/libjpeg_fuzz_XXXXXX";
  int fd = mkstemp(temp_filename);
  if (fd < 0) {
    return 0; // Failed to create temp file
  }
  
  FILE* temp_file = fdopen(fd, "w+b");
  if (!temp_file) {
    close(fd);
    return 0;
  }
  
  // =============================================
  // COMPRESSION PATH: Create JPEG (without quantization settings)
  // =============================================
  
  struct jpeg_compress_struct cinfo_compress;
  struct fuzzer_error_mgr jerr_compress;
  
  // Initialize compression object with custom error handler
  cinfo_compress.err = jpeg_std_error(&jerr_compress.pub);
  jerr_compress.pub.error_exit = fuzzer_error_exit;
  jerr_compress.pub.emit_message = fuzzer_emit_message;
  
  if (setjmp(jerr_compress.setjmp_buffer)) {
    // Error handling: clean up and return
    fclose(temp_file);
    remove(temp_filename);
    jpeg_destroy_compress(&cinfo_compress);
    return 0;
  }
  
  jpeg_CreateCompress(&cinfo_compress, JPEG_LIB_VERSION, sizeof(struct jpeg_compress_struct));
  
  // Set up stdio destination
  jpeg_stdio_dest(&cinfo_compress, temp_file);
  
  // Set compression parameters
  cinfo_compress.image_width = width;
  cinfo_compress.image_height = height;
  cinfo_compress.input_components = components;
  cinfo_compress.in_color_space = color_space;
  
  jpeg_set_defaults(&cinfo_compress);
  
  // Start compression
  jpeg_start_compress(&cinfo_compress, TRUE);
  
  // Write scanlines
  JSAMPROW row_pointer[1];
  size_t row_stride = width * components;
  
  for (int row = 0; row < height; row++) {
    row_pointer[0] = &image_data[row * row_stride];
    jpeg_write_scanlines(&cinfo_compress, row_pointer, 1);
  }
  
  // Finish compression
  jpeg_finish_compress(&cinfo_compress);
  jpeg_destroy_compress(&cinfo_compress);
  
  // Rewind file for decompression
  rewind(temp_file);
  
  // =============================================
  // DECOMPRESSION PATH: Test colormap operations
  // =============================================
  
  struct jpeg_decompress_struct cinfo_decompress;
  struct fuzzer_error_mgr jerr_decompress;
  
  // Initialize decompression object with custom error handler
  cinfo_decompress.err = jpeg_std_error(&jerr_decompress.pub);
  jerr_decompress.pub.error_exit = fuzzer_error_exit;
  jerr_decompress.pub.emit_message = fuzzer_emit_message;
  
  if (setjmp(jerr_decompress.setjmp_buffer)) {
    // Error handling: clean up and return
    fclose(temp_file);
    remove(temp_filename);
    jpeg_destroy_decompress(&cinfo_decompress);
    return 0;
  }
  
  // Step 1: Create decompression object (Target: jpeg_CreateDecompress helper)
  jpeg_CreateDecompress(&cinfo_decompress, JPEG_LIB_VERSION, sizeof(struct jpeg_decompress_struct));
  
  // Step 2: Set up stdio source (Target: jpeg_stdio_src)
  jpeg_stdio_src(&cinfo_decompress, temp_file);
  
  // Step 3: Read header
  jpeg_read_header(&cinfo_decompress, TRUE);
  
  // Configure for colormap operations
  cinfo_decompress.quantize_colors = TRUE;
  cinfo_decompress.desired_number_of_colors = desired_colors;
  cinfo_decompress.two_pass_quantize = two_pass_quantize;
  cinfo_decompress.dither_mode = enable_dithering ? JDITHER_FS : JDITHER_NONE;
  
  // Enable external colormap support (required for jpeg_new_colormap)
  cinfo_decompress.enable_external_quant = TRUE;
  
  // Enable buffered image mode if requested (allows colormap changes between passes)
  cinfo_decompress.buffered_image = buffered_image;
  
  // Step 4: Start decompression
  jpeg_start_decompress(&cinfo_decompress);
  
  // Allocate output buffer
  std::vector<uint8_t> output_buffer(cinfo_decompress.output_width * 
                                     cinfo_decompress.output_height * 
                                     cinfo_decompress.output_components);
  
  JSAMPROW output_row[1];
  
  // Read some scanlines first
  int scanlines_read = 0;
  int max_scanlines = std::min(height, 10);  // Read a few scanlines
  
  while (scanlines_read < max_scanlines && cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
    output_row[0] = &output_buffer[scanlines_read * cinfo_decompress.output_width * cinfo_decompress.output_components];
    jpeg_read_scanlines(&cinfo_decompress, output_row, 1);
    scanlines_read++;
  }
  
  // Step 5: Test jpeg_new_colormap if conditions are right
  // jpeg_new_colormap requires: DSTATE_BUFIMAGE state, quantize_colors TRUE, 
  // enable_external_quant TRUE, and colormap != NULL
  if (cinfo_decompress.buffered_image && 
      cinfo_decompress.quantize_colors && 
      cinfo_decompress.enable_external_quant &&
      cinfo_decompress.colormap != NULL) {
    
    // We're in buffered image mode, so we can try to change colormap
    // Note: This would normally be called between output passes in buffered mode
    // For testing purposes, we'll call it to exercise the API
    // In real usage, this would be called between jpeg_start_decompress and 
    // jpeg_finish_decompress when in buffered image mode
    
    // The actual call to jpeg_new_colormap requires proper state management
    // For fuzzing, we'll attempt to call it to trigger error paths if state is wrong
    // and potentially succeed if state happens to be correct
    
    // Try calling jpeg_new_colormap - this may fail if not in right state
    // but will exercise the function's error checking logic
    jpeg_new_colormap(&cinfo_decompress);
  }
  
  // Step 6: Test jpeg_input_complete (Target API)
  // Check if input is complete
  boolean input_complete = jpeg_input_complete(&cinfo_decompress);
  
  // Continue reading remaining scanlines
  while (cinfo_decompress.output_scanline < cinfo_decompress.output_height) {
    output_row[0] = &output_buffer[cinfo_decompress.output_scanline * 
                                   cinfo_decompress.output_width * 
                                   cinfo_decompress.output_components];
    jpeg_read_scanlines(&cinfo_decompress, output_row, 1);
  }
  
  // Step 7: Finish decompression
  jpeg_finish_decompress(&cinfo_decompress);
  
  // Step 8: Clean up
  jpeg_destroy_decompress(&cinfo_decompress);
  
  // Clean up temp file
  fclose(temp_file);
  remove(temp_filename);
  
  return 0;
}


} // namespace harness_016
