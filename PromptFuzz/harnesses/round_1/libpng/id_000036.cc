#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <png.h>
}
//<ID> 588
//<Prompt> []
/*<Combination>: [
*/
//<score> 37, nr_unique_branch: 1
//<Quality> {"density":37,"unique_branches":{"png_set_crc_action":[[52,7,52,30,0,0,4,0]]},"library_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_destroy_read_struct","png_set_longjmp_fn","png_destroy_read_struct","png_init_io","png_set_sig_bytes","png_set_crc_action","png_read_info","png_set_text_compression_mem_level","png_set_gAMA_fixed","png_get_user_transform_ptr","png_get_next_frame_height","png_set_expand","png_set_gray_to_rgb","png_set_palette_to_rgb","png_set_tRNS_to_alpha","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_destroy_read_struct","png_get_rowbytes","png_destroy_read_struct","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_destroy_write_struct","png_init_io","png_set_IHDR","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_write_struct","png_destroy_read_struct"],"critical_calls":["png_sig_cmp","png_create_read_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_sig_bytes","png_set_crc_action","png_read_info","png_set_text_compression_mem_level","png_set_gAMA_fixed","png_get_user_transform_ptr","png_get_next_frame_height","png_set_expand","png_set_gray_to_rgb","png_set_palette_to_rgb","png_set_tRNS_to_alpha","png_read_update_info","png_get_image_width","png_get_image_height","png_get_color_type","png_get_bit_depth","png_get_rowbytes","png_read_image","png_read_end","png_create_write_struct","png_create_info_struct","png_set_longjmp_fn","png_init_io","png_set_IHDR","png_write_info","png_write_image","png_write_end","png_destroy_write_struct","png_destroy_read_struct"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	FUZZ_FILE_INIT();
	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    // 检查是否有足够的数据进行PNG签名验证
    if (size < 8) {
        return 0;
    }

    // 验证PNG签名
    if (png_sig_cmp(data, fuzz_uint64_t_1, 8) != 0) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
	FUZZ_FILE_PUSH(in_file)
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 创建读结构体
    png_struct *png_ptr = png_create_read_struct(fuzz_str_2, 
                                                  nullptr, nullptr, nullptr);
    if (!png_ptr) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 创建信息结构体
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 设置错误处理
    if (setjmp(png_jmpbuf(png_ptr))) {
	FUZZ_FILE_CLOSE();
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 初始化IO
    png_init_io(png_ptr, in_file);
    
    // 设置我们已读入的签名字节
    png_set_sig_bytes(png_ptr, fuzz_int32_t_3);
    
    // 设置CRC校验行为 - 对于关键块报错退出，对于辅助块警告并忽略
    png_set_crc_action(png_ptr, fuzz_int32_t_4, fuzz_int32_t_5);
    
    // 读取PNG信息
    png_read_info(png_ptr, info_ptr);

    // 使用第一个API: 设置文本压缩内存级别
    png_set_text_compression_mem_level(png_ptr, fuzz_int32_t_6);  // 使用最大内存级别

    // 使用第二个API: 设置伽马值（固定点格式）
    // PNG_FP_1 表示1.0的固定点值
    png_set_gAMA_fixed(png_ptr, info_ptr, fuzz_int32_t_7);  // 设置伽马为1.0

    // 使用第三个API: 获取用户变换指针
    void *user_transform_ptr = png_get_user_transform_ptr(png_ptr);
    // 可以检查是否为空，但这里只是确保调用API

    // 使用第四个API: 获取下一帧高度（对于APNG）
    png_uint_32 next_frame_height = png_get_next_frame_height(png_ptr, info_ptr);
    // 对于非动画PNG，这将返回0

    // 设置变换以便读取
    png_set_expand(png_ptr);
    png_set_gray_to_rgb(png_ptr);
    png_set_palette_to_rgb(png_ptr);
    png_set_tRNS_to_alpha(png_ptr);

    // 更新信息结构体
    png_read_update_info(png_ptr, info_ptr);

    // 获取图像尺寸
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // 为行指针分配内存
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    if (!row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 为每行分配内存
    png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(row_bytes);
        if (!row_pointers[y]) {
            // 释放已分配的内存
            for (png_uint_32 i = 0; i < y; i++) {
                free(row_pointers[i]);
            }
            free(row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	return 0;
        }
    }

    // 读取图像数据
    png_read_image(png_ptr, row_pointers);

    // 读取结束
    png_read_end(png_ptr, nullptr);
	FUZZ_FILENAME_PUSH("output_file")

    // 创建一个输出文件来测试写入
    FILE *out_file = fopen("output_file", "wb");
	FUZZ_FILE_PUSH(out_file)
    if (out_file) {
        // 创建写结构体
        png_struct *write_ptr = png_create_write_struct(fuzz_str_8, 
                                                        nullptr, nullptr, nullptr);
        if (write_ptr) {
            png_info *write_info = png_create_info_struct(write_ptr);
            if (write_info) {
                if (setjmp(png_jmpbuf(write_ptr))) {
	FUZZ_FILE_CLOSE();
                    png_destroy_write_struct(&write_ptr, &write_info);
                } else {
                    png_init_io(write_ptr, out_file);
                    
                    // 设置输出信息
                    png_set_IHDR(write_ptr, write_info, width, height,
                                bit_depth, color_type, fuzz_int32_t_9,
                                fuzz_int32_t_10, fuzz_int32_t_11);
                    
                    // 写入信息
                    png_write_info(write_ptr, write_info);
                    
                    // 写入行数据
                    png_write_image(write_ptr, row_pointers);
                    
                    // 写入结束
                    png_write_end(write_ptr, nullptr);
                    
                    png_destroy_write_struct(&write_ptr, &write_info);
                }
            } else {
                png_destroy_write_struct(&write_ptr, nullptr);
            }
        }
        assert_file_closed(&out_file);;
    }

    // 释放所有分配的资源
    for (png_uint_32 y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}