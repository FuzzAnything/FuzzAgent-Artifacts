#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_008 {

// Structure to hold memory reading callback data
struct png_read_memory_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Read callback function for memory-based input
static void png_read_memory_callback(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
    png_read_memory_data* read_data = (png_read_memory_data*)png_get_io_ptr(png_ptr);
    
    if (read_data->offset + byteCountToRead > read_data->size) {
        png_error(png_ptr, "Insufficient data in read callback");
        return;
    }
    
    memcpy(outBytes, read_data->data + read_data->offset, byteCountToRead);
    read_data->offset += byteCountToRead;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for meaningful metadata testing
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // PHASE 1: INITIALIZATION
    
    // Create PNG read structure
    png_struct *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Create PNG info structure
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Set up custom read data structure
    png_read_memory_data read_data;
    read_data.data = data;
    read_data.size = size;
    read_data.offset = 0;
    
    // Set up the read function
    png_set_read_fn(png_ptr, &read_data, png_read_memory_callback);

    // Set the number of bytes already read (0 since we start from beginning)
    png_set_sig_bytes(png_ptr, 0);

    // Read PNG info to populate info structure
    png_read_info(png_ptr, info_ptr);

    // PHASE 2: SETUP & DATA POPULATION WITH SETTER APIS
    
    // Consume parameters for setter APIs from fuzzer input
    int operation = fdp.ConsumeIntegralInRange<int>(0, 3);
    bool use_fixed_point = fdp.ConsumeBool();
    
    // Test png_set_iCCP (set ICC profile chunk)
    if (fdp.remaining_bytes() > 50) {
        std::string icc_name = fdp.ConsumeRandomLengthString(50);
        int icc_compression = fdp.ConsumeIntegralInRange<int>(0, 2);
        size_t icc_profile_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        std::vector<png_byte> icc_profile = fdp.ConsumeBytes<png_byte>(icc_profile_size);
        
        if (icc_profile.size() > 0) {
            png_set_iCCP(png_ptr, info_ptr, 
                        (png_charp)icc_name.c_str(), 
                        icc_compression,
                        icc_profile.data(), 
                        icc_profile.size());
        }
    }
    
    // Test png_set_crc_action (set CRC handling)
    if (fdp.remaining_bytes() > 2) {
        int crc_action = fdp.ConsumeIntegralInRange<int>(0, 2);
        png_set_crc_action(png_ptr, crc_action, crc_action);
    }
    
    // Test png_set_sCAL or png_set_sCAL_fixed (set physical scale)
    if (fdp.remaining_bytes() > 10) {
        int unit = fdp.ConsumeIntegralInRange<int>(1, 2);
        double width = fdp.ConsumeFloatingPoint<double>();
        double height = fdp.ConsumeFloatingPoint<double>();
        
        if (use_fixed_point) {
            // Check if values are within representable range for png_fixed_point
            // PNG_FP_MAX = 0x7fffffffL = 2147483647
            // PNG_FP_MIN = -PNG_FP_MAX = -2147483647
            // After multiplying by 100000.0, valid range is [-21474.83647, 21474.83647]
            const double MAX_VALID_VALUE = 21474.83647;
            const double MIN_VALID_VALUE = -21474.83647;
            
            if (width >= MIN_VALID_VALUE && width <= MAX_VALID_VALUE &&
                height >= MIN_VALID_VALUE && height <= MAX_VALID_VALUE) {
                png_fixed_point width_fixed = (png_fixed_point)(width * 100000.0);
                png_fixed_point height_fixed = (png_fixed_point)(height * 100000.0);
                png_set_sCAL_fixed(png_ptr, info_ptr, unit, width_fixed, height_fixed);
            }
            // If values are out of range, skip the fixed-point call
        } else {
            png_set_sCAL(png_ptr, info_ptr, unit, width, height);
        }
    }
    
    // PHASE 3: METADATA RETRIEVAL WITH GETTER APIS
    
    // Variables to store retrieved metadata (declared here to avoid scope issues)
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    double max_luminance, min_luminance;
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z;
    png_uint_32 int_max_luminance, int_min_luminance;
    
    // Consume operation choice to decide which getter APIs to test
    switch (operation) {
        case 0:
            // Test chromaticity functions (cHRM) - floating point versions
            png_get_cHRM(png_ptr, info_ptr, 
                        &white_x, &white_y, 
                        &red_x, &red_y, 
                        &green_x, &green_y, 
                        &blue_x, &blue_y);
            
            png_get_cHRM_XYZ(png_ptr, info_ptr,
                           &red_X, &red_Y, &red_Z,
                           &green_X, &green_Y, &green_Z,
                           &blue_X, &blue_Y, &blue_Z);
            break;
            
        case 1:
            // Test fixed-point chromaticity functions
            png_get_cHRM_fixed(png_ptr, info_ptr,
                             &int_white_x, &int_white_y,
                             &int_red_x, &int_red_y,
                             &int_green_x, &int_green_y,
                             &int_blue_x, &int_blue_y);
            
            png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                 &int_red_X, &int_red_Y, &int_red_Z,
                                 &int_green_X, &int_green_Y, &int_green_Z,
                                 &int_blue_X, &int_blue_Y, &int_blue_Z);
            break;
            
        case 2:
            // Test mastering display color volume functions (mDCV)
            png_get_mDCV(png_ptr, info_ptr,
                        &white_x, &white_y,
                        &red_x, &red_y,
                        &green_x, &green_y,
                        &blue_x, &blue_y,
                        &max_luminance, &min_luminance);
            
            png_get_mDCV_fixed(png_ptr, info_ptr,
                             &int_white_x, &int_white_y,
                             &int_red_x, &int_red_y,
                             &int_green_x, &int_green_y,
                             &int_blue_x, &int_blue_y,
                             &int_max_luminance, &int_min_luminance);
            break;
            
        case 3:
            // Test pCAL and frame control functions
            {
                png_charp purpose = NULL;
                png_int_32 X0, X1;
                int type, nparams;
                png_charp units = NULL;
                png_charpp params = NULL;
                
                png_get_pCAL(png_ptr, info_ptr,
                            &purpose, &X0, &X1, &type, &nparams,
                            &units, &params);
                
                // Test APNG frame control if available
                png_uint_32 width, height, x_offset, y_offset;
                png_uint_16 delay_num, delay_den;
                png_byte dispose_op, blend_op;
                
                png_get_next_frame_fcTL(png_ptr, info_ptr,
                                       &width, &height,
                                       &x_offset, &y_offset,
                                       &delay_num, &delay_den,
                                       &dispose_op, &blend_op);
            }
            break;
    }
    
    // PHASE 4: CLEANUP
    
    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}

} // namespace harness_008
