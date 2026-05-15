#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <zlib.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_005 {

// Error handling callbacks for libpng
static void pngtest_error(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

static void pngtest_warning(png_structp png_ptr, png_const_charp msg) {
    (void)png_ptr;
    (void)msg;
    // Do nothing - just return
}

// Custom write functions for memory buffers
struct png_memory_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void pngtest_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Write to nowhere - just consume the data
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void pngtest_flush_data(png_structp png_ptr) {
    (void)png_ptr;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 256) {
        return 0;  // Need minimum data for advanced PNG writing operations
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create a memory buffer for PNG operations
    png_memory_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        pngtest_error, 
        pngtest_warning);
    
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up custom write functions
    png_set_write_fn(png_ptr, &buffer, pngtest_write_data, pngtest_flush_data);
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set image parameters based on fuzzed input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    int bit_depth = fdp.PickValueInArray({1, 2, 4, 8, 16});
    int color_type = fdp.PickValueInArray({
        PNG_COLOR_TYPE_GRAY,
        PNG_COLOR_TYPE_GRAY_ALPHA,
        PNG_COLOR_TYPE_PALETTE,
        PNG_COLOR_TYPE_RGB,
        PNG_COLOR_TYPE_RGB_ALPHA
    });
    int interlace_type = fdp.ConsumeBool() ? PNG_INTERLACE_ADAM7 : PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_BASE;
    int filter_method = PNG_FILTER_TYPE_BASE;
    
    // Set IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth,
                color_type, interlace_type, compression_type, filter_method);
    
    // 1. png_set_filter - Set compression filter method (61 branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 2) {
        int filter_method = 0;  // Currently, the only valid value is 0
        int filters = 0;
        
        // Build filter flags from fuzzed input
        if (fdp.ConsumeBool()) filters |= PNG_FILTER_NONE;
        if (fdp.ConsumeBool()) filters |= PNG_FILTER_SUB;
        if (fdp.ConsumeBool()) filters |= PNG_FILTER_UP;
        if (fdp.ConsumeBool()) filters |= PNG_FILTER_AVG;
        if (fdp.ConsumeBool()) filters |= PNG_FILTER_PAETH;
        
        // If no filters selected, use a default
        if (filters == 0) {
            filters = PNG_ALL_FILTERS;
        }
        
        png_set_filter(png_ptr, filter_method, filters);
    }
    
    // Set up palette if needed for hIST chunk
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
        std::vector<png_color> palette(num_palette);
        for (int i = 0; i < num_palette && fdp.remaining_bytes() >= 3; i++) {
            palette[i].red = fdp.ConsumeIntegral<uint8_t>();
            palette[i].green = fdp.ConsumeIntegral<uint8_t>();
            palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
        }
        png_set_PLTE(png_ptr, info_ptr, palette.data(), num_palette);
        
        // 2. png_set_hIST - Set histogram chunk (49 branches)
        // hIST chunk only valid with PLTE (palette) images
        if (fdp.ConsumeBool() && fdp.remaining_bytes() >= num_palette * sizeof(png_uint_16)) {
            std::vector<png_uint_16> hist(num_palette);
            for (int i = 0; i < num_palette; i++) {
                hist[i] = fdp.ConsumeIntegral<png_uint_16>();
            }
            png_set_hIST(png_ptr, info_ptr, hist.data());
        }
    }
    
    // 3. png_set_pCAL - Set pixel calibration chunk (48 branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 32) {
        std::string purpose = fdp.ConsumeRandomLengthString(79);
        png_int_32 X0 = fdp.ConsumeIntegral<png_int_32>();
        png_int_32 X1 = fdp.ConsumeIntegral<png_int_32>();
        int type = fdp.ConsumeIntegralInRange<int>(0, 3);  // PNG_EQUATION_LINEAR, LOG, etc.
        int nparams = fdp.ConsumeIntegralInRange<int>(0, 4);
        
        std::string units = fdp.ConsumeRandomLengthString(79);
        
        // Create parameter array
        std::vector<char*> params;
        std::vector<std::string> param_strings;
        param_strings.reserve(nparams);  // Reserve capacity to prevent reallocation
        for (int i = 0; i < nparams && fdp.remaining_bytes() >= 16; i++) {
            param_strings.push_back(fdp.ConsumeRandomLengthString(79));
            params.push_back(const_cast<char*>(param_strings.back().c_str()));
        }
        if (params.size() == nparams) {
            png_set_pCAL(png_ptr, info_ptr, purpose.c_str(), X0, X1, 
                        type, nparams, units.c_str(), params.data());
        }
    }
    // 4. png_set_eXIf_1 - Set EXIF data chunk (45 branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 64) {
        size_t exif_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
        if (exif_size > fdp.remaining_bytes()) {
            exif_size = fdp.remaining_bytes();
        }
        
        std::vector<png_byte> exif_data = fdp.ConsumeBytes<png_byte>(exif_size);
        png_set_eXIf_1(png_ptr, info_ptr, exif_data.size(), exif_data.data());
    }
    
    // 5. png_set_sPLT - Set suggested palette chunk (36 branches)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 32) {
        int nentries = fdp.ConsumeIntegralInRange<int>(1, 16);
        std::vector<png_sPLT_entry> entries(nentries);
        
        for (int i = 0; i < nentries && fdp.remaining_bytes() >= 10; i++) {
            entries[i].red = fdp.ConsumeIntegral<png_uint_16>();
            entries[i].green = fdp.ConsumeIntegral<png_uint_16>();
            entries[i].blue = fdp.ConsumeIntegral<png_uint_16>();
            entries[i].alpha = fdp.ConsumeIntegral<png_uint_16>();
            entries[i].frequency = fdp.ConsumeIntegral<png_uint_16>();
        }
        
        // Create sPLT structure
        png_sPLT_t splt;
        std::string splt_name = fdp.ConsumeRandomLengthString(79);
        splt.name = const_cast<char*>(splt_name.c_str());
        splt.depth = fdp.ConsumeIntegralInRange<png_byte>(8, 16);
        splt.entries = entries.data();
        splt.nentries = nentries;
        
        png_set_sPLT(png_ptr, info_ptr, &splt, 1);
    }
    
    // 6. png_set_sCAL / png_set_sCAL_fixed - Set physical scale chunk
    if (fdp.ConsumeBool() && fdp.remaining_bytes() >= 8) {
        int unit = fdp.ConsumeIntegralInRange<int>(PNG_SCALE_UNKNOWN, PNG_SCALE_METER);
        
        if (fdp.ConsumeBool()) {
            // Floating point version
            double width_scaling = fdp.ConsumeFloatingPointInRange<double>(0.1, 1000.0);
            double height_scaling = fdp.ConsumeFloatingPointInRange<double>(0.1, 1000.0);
            png_set_sCAL(png_ptr, info_ptr, unit, width_scaling, height_scaling);
        } else {
            // Fixed point version
            png_fixed_point width_scaling_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_fixed_point height_scaling_fixed = fdp.ConsumeIntegral<png_fixed_point>();
            png_set_sCAL_fixed(png_ptr, info_ptr, unit, width_scaling_fixed, height_scaling_fixed);
        }
    }
    
    // Write header information
    png_write_info(png_ptr, info_ptr);
    
    // 7. png_write_rows / png_write_row - Writing operations (high undiscovered branches: 80/79)
    // Allocate row buffer based on image parameters
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0 || row_bytes > 1024 * 1024) {  // Limit to 1MB per row
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Create row data from fuzzed input
    std::vector<png_byte> row_buffer(row_bytes);
    if (fdp.remaining_bytes() >= row_bytes) {
        auto row_data = fdp.ConsumeBytes<uint8_t>(row_bytes);
        memcpy(row_buffer.data(), row_data.data(), row_bytes);
    }
    
    // Test different writing strategies based on fuzzed input
    int write_strategy = fdp.ConsumeIntegralInRange<int>(0, 2);
    
    switch (write_strategy) {
        case 0: {
            // Write rows one by one using png_write_row
            for (png_uint_32 row = 0; row < height && fdp.ConsumeBool(); row++) {
                png_write_row(png_ptr, row_buffer.data());
            }
            break;
        }
        case 1: {
            // Write multiple rows at once using png_write_rows
            png_uint_32 num_rows = fdp.ConsumeIntegralInRange<png_uint_32>(1, height);
            if (num_rows > height) num_rows = height;
            
            std::vector<png_byte*> rows(num_rows);
            for (png_uint_32 i = 0; i < num_rows; i++) {
                rows[i] = row_buffer.data();
            }
            
            png_write_rows(png_ptr, rows.data(), num_rows);
            break;
        }
        case 2: {
            // Mix of both approaches
            png_uint_32 single_rows = fdp.ConsumeIntegralInRange<png_uint_32>(0, height/2);
            for (png_uint_32 i = 0; i < single_rows; i++) {
                png_write_row(png_ptr, row_buffer.data());
            }
            
            if (height - single_rows > 0) {
                png_uint_32 remaining_rows = height - single_rows;
                std::vector<png_byte*> rows(remaining_rows);
                for (png_uint_32 i = 0; i < remaining_rows; i++) {
                    rows[i] = row_buffer.data();
                }
                png_write_rows(png_ptr, rows.data(), remaining_rows);
            }
            break;
        }
    }
    
    // Write end of PNG file
    png_write_end(png_ptr, info_ptr);
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_005
