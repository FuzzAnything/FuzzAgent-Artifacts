#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <algorithm>

namespace harness_008 {
/*
 * Fuzzing harness for tinygltf library image processing functions
 * 
 * This harness specifically targets critical coverage gaps in image processing:
 * 1. WriteImageData - 42 branches, 0 hits (highest undiscovered complexity)
 * 2. LoadImageData - 48 branches, only 10.42% function lines hit
 * 3. stb_image loading functions: stbi_load, stbi_load_16, stbi_loadf, stbi_info (all at 0% coverage)
 * 
 * The stb_image.h module has only 4.95% region coverage - the lowest of all modules.
 * This harness aims to cover 100+ currently undiscovered branches.
 * 
 * Strategy:
 * 1. Direct stb_image function testing (bypassing tinygltf wrappers)
 * 2. Custom image loader/writer callback implementation
 * 3. Comprehensive image format testing (PNG, JPEG, BMP, HDR, etc.)
 * 4. Different pixel formats and bit depths (8-bit, 16-bit, float)
 * 5. Complete image processing lifecycle testing
 */


// Include stb_image headers - implementations are already in the tinygltf library


// Mock filesystem for fuzzing - stores files in memory
class MockImageProcessingFileSystem {
public:
    struct FileInfo {
        std::vector<unsigned char> data;
        bool exists;
    };
    
    std::map<std::string, FileInfo> files;
    void* user_data;
    
    MockImageProcessingFileSystem() : user_data(nullptr) {}
    
    // FileExists callback implementation
    static bool MockFileExists(const std::string& abs_filename, void* user_data) {
        MockImageProcessingFileSystem* fs = static_cast<MockImageProcessingFileSystem*>(user_data);
        if (!fs) return false;
        
        auto it = fs->files.find(abs_filename);
        return it != fs->files.end() && it->second.exists;
    }
    
    // ExpandFilePath callback implementation (no-op for mocking)
    static std::string MockExpandFilePath(const std::string& filepath, void* user_data) {
        return filepath;
    }
    
    // ReadWholeFile callback implementation
    static bool MockReadWholeFile(std::vector<unsigned char>* out, 
                                 std::string* err, 
                                 const std::string& filepath, 
                                 void* user_data) {
        MockImageProcessingFileSystem* fs = static_cast<MockImageProcessingFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        auto it = fs->files.find(filepath);
        if (it == fs->files.end() || !it->second.exists) {
            if (err) *err = "File not found: " + filepath;
            return false;
        }
        
        *out = it->second.data;
        return true;
    }
    
    // WriteWholeFile callback implementation  
    static bool MockWriteWholeFile(std::string* err, 
                                   const std::string& filepath,
                                   const std::vector<unsigned char>& contents,
                                   void* user_data) {
        MockImageProcessingFileSystem* fs = static_cast<MockImageProcessingFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        FileInfo info;
        info.data = contents;
        info.exists = true;
        fs->files[filepath] = info;
        return true;
    }
    
    // GetFileSizeInBytes callback implementation
    static bool MockGetFileSizeInBytes(size_t* filesize_out,
                                       std::string* err,
                                       const std::string& filepath,
                                       void* user_data) {
        MockImageProcessingFileSystem* fs = static_cast<MockImageProcessingFileSystem*>(user_data);
        if (!fs) {
            if (err) *err = "No filesystem context";
            return false;
        }
        
        auto it = fs->files.find(filepath);
        if (it == fs->files.end() || !it->second.exists) {
            if (err) *err = "File not found: " + filepath;
            return false;
        }
        
        *filesize_out = it->second.data.size();
        return true;
    }
    
    // Get filesystem callbacks
    tinygltf::FsCallbacks GetFsCallbacks() const {
        tinygltf::FsCallbacks fs;
        fs.FileExists = &MockFileExists;
        fs.ExpandFilePath = &MockExpandFilePath;
        fs.ReadWholeFile = &MockReadWholeFile;
        fs.WriteWholeFile = &MockWriteWholeFile;
        fs.user_data = const_cast<void*>(reinterpret_cast<const void*>(this));
        return fs;
    }
};

// Mock URI callbacks
class MockImageURICallbacks {
public:
    static bool MockURIDecode(const std::string& in_uri, std::string* out_uri, void* user_data) {
        *out_uri = in_uri;
        return true;
    }
    
    static bool MockURIEncode(const std::string& in_uri, const std::string& object_type, 
                              std::string* out_uri, void* user_data) {
        *out_uri = in_uri;
        return true;
    }
    
    tinygltf::URICallbacks GetURICallbacks() const {
        tinygltf::URICallbacks uri;
        uri.decode = &MockURIDecode;
        uri.encode = &MockURIEncode;
        uri.user_data = nullptr;
        return uri;
    }
};

// Custom image loader callback for testing
bool CustomImageLoader(tinygltf::Image* image, int image_idx, std::string* err,
                      std::string* warn, int req_width, int req_height,
                      const unsigned char* bytes, int size, void* user_data) {
    // Forward to default LoadImageData
    return tinygltf::LoadImageData(image, image_idx, err, warn, req_width, req_height, bytes, size, user_data);
}

// Custom image writer callback for testing
bool CustomImageWriter(const std::string* basepath, const std::string* filename,
                      const tinygltf::Image* image, bool embedImages,
                      const tinygltf::FsCallbacks* fs_cb, 
                      const tinygltf::URICallbacks* uri_cb,
                      std::string* out_uri, void* user_data) {
    // Forward to default WriteImageData
    return tinygltf::WriteImageData(basepath, filename, image, embedImages, fs_cb, uri_cb, out_uri, user_data);
}

// Generate mock image data for different formats
std::vector<unsigned char> GenerateMockImageData(FuzzedDataProvider& fdp, int width, int height, int channels, int bits) {
    std::vector<unsigned char> image_data;
    size_t pixel_count = width * height * channels;
    
    if (bits == 16) {
        // 16-bit data
        pixel_count *= 2; // 2 bytes per channel
    } else if (bits == 32) {
        // Float data (32-bit)
        pixel_count *= 4; // 4 bytes per channel (float)
    }
    
    // Generate random pixel data from fuzzer input
    for (size_t i = 0; i < pixel_count && fdp.remaining_bytes() > 0; i++) {
        image_data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    // Fill remaining with pattern if fuzzer input exhausted
    while (image_data.size() < pixel_count) {
        image_data.push_back(static_cast<uint8_t>(image_data.size() % 256));
    }
    
    return image_data;
}

// Create mock image file data (PNG, JPEG, BMP headers)
std::vector<unsigned char> CreateMockImageFile(FuzzedDataProvider& fdp, uint8_t format_type) {
    std::vector<unsigned char> image_data;
    
    switch (format_type % 5) {
        case 0: { // PNG
            // PNG signature (8 bytes)
            image_data.push_back(0x89); image_data.push_back(0x50); image_data.push_back(0x4E); image_data.push_back(0x47);
            image_data.push_back(0x0D); image_data.push_back(0x0A); image_data.push_back(0x1A); image_data.push_back(0x0A);
            break;
        }
        case 1: { // JPEG
            // JPEG SOI marker
            image_data.push_back(0xFF); image_data.push_back(0xD8); // Start of Image
            // Add some JFIF header data
            image_data.push_back(0xFF); image_data.push_back(0xE0); // APP0 marker
            break;
        }
        case 2: { // BMP
            // BMP header
            image_data.push_back(0x42); image_data.push_back(0x4D); // "BM"
            break;
        }
        case 3: { // HDR
            // Radiance HDR header start
            image_data.push_back(0x23); image_data.push_back(0x3F); // "#?"
            image_data.push_back(0x52); image_data.push_back(0x41); // "RA"
            image_data.push_back(0x44); image_data.push_back(0x49); // "DI"
            image_data.push_back(0x41); image_data.push_back(0x4E); // "AN"
            image_data.push_back(0x43); image_data.push_back(0x45); // "CE"
            break;
        }
        case 4: { // GIF
            // GIF header
            image_data.push_back(0x47); image_data.push_back(0x49); image_data.push_back(0x46); // "GIF"
            image_data.push_back(0x38); image_data.push_back(0x39); image_data.push_back(0x61); // "89a"
            break;
        }
    }
    
    // Add some mock image data from fuzzer input
    size_t data_size = fdp.ConsumeIntegralInRange<size_t>(10, 500);
    for (size_t i = 0; i < data_size && fdp.remaining_bytes() > 0; i++) {
        image_data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    return image_data;
}

// Direct stb_image function testing
void TestStbImageFunctions(FuzzedDataProvider& fdp, const std::vector<unsigned char>& image_data) {
    if (image_data.empty()) return;
    
    // Test stbi_info first (should work without decoding)
    int width = 0, height = 0, comp = 0;
    bool info_result = stbi_info_from_memory(image_data.data(), static_cast<int>(image_data.size()), &width, &height, &comp);
    (void)info_result; // Result may be false for invalid data
    
    // Test different loading functions based on selector
    uint8_t loader_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (loader_type) {
        case 0: {
            // Test stbi_load (8-bit)
            unsigned char* data = stbi_load_from_memory(image_data.data(), 
                                                       static_cast<int>(image_data.size()),
                                                       &width, &height, &comp, 0);
            if (data) {
                stbi_image_free(data);
            }
            break;
        }
        case 1: {
            // Test stbi_load_16 (16-bit)
            uint16_t* data16 = stbi_load_16_from_memory(image_data.data(),
                                                       static_cast<int>(image_data.size()),
                                                       &width, &height, &comp, 0);
            if (data16) {
                stbi_image_free(data16);
            }
            break;
        }
        case 2: {
            // Test stbi_loadf (float)
            float* dataf = stbi_loadf_from_memory(image_data.data(),
                                                 static_cast<int>(image_data.size()),
                                                 &width, &height, &comp, 0);
            if (dataf) {
                stbi_image_free(dataf);
            }
            break;
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful testing
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test configuration
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    bool embed_images = fdp.ConsumeBool();
    bool pretty_print = fdp.ConsumeBool();
    bool write_binary = fdp.ConsumeBool();
    
    // Create TinyGLTF instance
    tinygltf::TinyGLTF ctx;
    tinygltf::Model model;
    std::string err, warn;
    
    // Configure filesystem callbacks
    MockImageProcessingFileSystem mock_fs;
    tinygltf::FsCallbacks fs_callbacks = mock_fs.GetFsCallbacks();
    
    // Configure URI callbacks
    MockImageURICallbacks mock_uri;
    tinygltf::URICallbacks uri_callbacks = mock_uri.GetURICallbacks();
    
    // Set filesystem callbacks
    ctx.SetFsCallbacks(fs_callbacks);
    
    // Set custom image loader and writer callbacks
    ctx.SetImageLoader(&CustomImageLoader, nullptr);
    ctx.SetImageWriter(&CustomImageWriter, nullptr);
    
    // Create some mock image files in the filesystem
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 50; i++) {
        std::string filename = "image_" + std::to_string(i);
        if (i == 0) filename += ".png";
        else if (i == 1) filename += ".jpg";
        else filename += ".bmp";
        
        std::vector<unsigned char> mock_file = CreateMockImageFile(fdp, static_cast<uint8_t>(i));
        if (!mock_file.empty()) {
            MockImageProcessingFileSystem::FileInfo info;
            info.data = mock_file;
            info.exists = true;
            mock_fs.files[filename] = info;
        }
    }
    
    // Test different modes based on coverage guidance
    switch (test_mode) {
        case 0: {
            // Direct stb_image function testing (targeting 0% coverage functions)
            for (const auto& file_pair : mock_fs.files) {
                TestStbImageFunctions(fdp, file_pair.second.data);
            }
            
            // Also test with fuzzer-generated image data
            size_t mock_data_size = fdp.ConsumeIntegralInRange<size_t>(10, 500);
            std::vector<unsigned char> mock_data = fdp.ConsumeBytes<uint8_t>(mock_data_size);
            if (!mock_data.empty()) {
                TestStbImageFunctions(fdp, mock_data);
            }
            break;
        }
        
        case 1: {
            // LoadImageData testing with various image configurations
            tinygltf::Image test_image;
            test_image.name = "test_image";
            test_image.width = fdp.ConsumeIntegralInRange<int>(1, 512);
            test_image.height = fdp.ConsumeIntegralInRange<int>(1, 512);
            test_image.component = fdp.ConsumeIntegralInRange<int>(1, 4);
            test_image.bits = (fdp.ConsumeIntegral<uint8_t>() % 3 == 0) ? 8 : 
                             (fdp.ConsumeIntegral<uint8_t>() % 3 == 1) ? 16 : 32;
            test_image.pixel_type = (test_image.bits == 8) ? TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE :
                                   (test_image.bits == 16) ? TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT :
                                   TINYGLTF_COMPONENT_TYPE_FLOAT;
            
            // Generate mock image data
            std::vector<unsigned char> img_data = GenerateMockImageData(fdp, test_image.width, 
                                                                       test_image.height, 
                                                                       test_image.component,
                                                                       test_image.bits);
            
            if (!img_data.empty()) {
                // Test LoadImageData directly
                std::string load_err, load_warn;
                bool load_success = tinygltf::LoadImageData(&test_image, 0, &load_err, &load_warn,
                                                           0, 0, img_data.data(), 
                                                           static_cast<int>(img_data.size()), nullptr);
                (void)load_success;
                
                // If loading succeeded, test writing it back
                if (!test_image.image.empty()) {
                    std::string out_uri;
                    std::string basepath = ".";
                    std::string filename = "output_" + test_image.name + ".png";
                    
                    // Test WriteImageData (primary target with 42 branches)
                    bool write_success = tinygltf::WriteImageData(&basepath, &filename, &test_image,
                                                                 embed_images, &fs_callbacks,
                                                                 &uri_callbacks, &out_uri, nullptr);
                    (void)write_success;
                }
            }
            break;
        }
        
        case 2: {
            // Complete image processing pipeline with Model
            // Add images to model
            for (int i = 0; i < 2 && fdp.remaining_bytes() > 100; i++) {
                tinygltf::Image img;
                img.name = "model_image_" + std::to_string(i);
                img.width = fdp.ConsumeIntegralInRange<int>(1, 256);
                img.height = fdp.ConsumeIntegralInRange<int>(1, 256);
                img.component = fdp.ConsumeIntegralInRange<int>(1, 4);
                img.bits = 8; // Simple 8-bit for model testing
                img.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
                img.uri = "image_" + std::to_string(i) + ".png";
                
                model.images.push_back(img);
            }
            
            // Try to load images using the configured loader
            if (!model.images.empty()) {
                // Create LoadImageDataOption
                struct LocalLoadImageDataOption {
                    bool preserve_channels{false};
                    bool as_is{false};
                };
                LocalLoadImageDataOption load_option;
                load_option.as_is = false;
                load_option.preserve_channels = true;
                
                for (size_t i = 0; i < model.images.size(); i++) {
                    tinygltf::Image& image = model.images[i];
                    std::string load_err, load_warn;
                    
                    // Get image data from mock filesystem
                    auto it = mock_fs.files.find(image.uri);
                    if (it != mock_fs.files.end() && !it->second.data.empty()) {
                        const auto& img_data = it->second.data;
                        
                        // Use LoadImageData through the configured loader
                        bool load_success = tinygltf::LoadImageData(&image, static_cast<int>(i),
                                                                   &load_err, &load_warn, 0, 0,
                                                                   img_data.data(), 
                                                                   static_cast<int>(img_data.size()),
                                                                   &load_option);
                        (void)load_success;
                    }
                }
                
                // Write processed images using WriteImageData
                for (const auto& image : model.images) {
                    if (!image.image.empty()) {
                        std::string out_uri;
                        std::string basepath = "output";
                        std::string filename = "processed_" + image.name + ".png";
                        
                        tinygltf::WriteImageData(&basepath, &filename, &image,
                                                embed_images, &fs_callbacks,
                                                &uri_callbacks, &out_uri, nullptr);
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test different image formats and pixel formats comprehensively
            std::vector<std::string> test_formats = {"png", "jpg", "bmp", "hdr", "gif"};
            std::vector<int> test_bits = {8, 16, 32};
            std::vector<int> test_channels = {1, 2, 3, 4};
            
            for (int format_idx = 0; format_idx < 2 && fdp.remaining_bytes() > 200; format_idx++) {
                for (int bits_idx = 0; bits_idx < 2 && fdp.remaining_bytes() > 100; bits_idx++) {
                    for (int channels_idx = 0; channels_idx < 2 && fdp.remaining_bytes() > 50; channels_idx++) {
                        tinygltf::Image img;
                        img.name = "comprehensive_test_" + std::to_string(format_idx) + "_" + 
                                  std::to_string(bits_idx) + "_" + std::to_string(channels_idx);
                        img.width = fdp.ConsumeIntegralInRange<int>(1, 128);
                        img.height = fdp.ConsumeIntegralInRange<int>(1, 128);
                        img.component = test_channels[channels_idx % test_channels.size()];
                        img.bits = test_bits[bits_idx % test_bits.size()];
                        img.pixel_type = (img.bits == 8) ? TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE :
                                       (img.bits == 16) ? TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT :
                                       TINYGLTF_COMPONENT_TYPE_FLOAT;
                        
                        // Generate appropriate image data
                        std::vector<unsigned char> img_data = GenerateMockImageData(fdp, img.width, 
                                                                                   img.height, 
                                                                                   img.component,
                                                                                   img.bits);
                        
                        if (!img_data.empty()) {
                            // Test loading
                            std::string load_err, load_warn;
                            bool load_success = tinygltf::LoadImageData(&img, 0, &load_err, &load_warn,
                                                                       0, 0, img_data.data(), 
                                                                       static_cast<int>(img_data.size()), nullptr);
                            (void)load_success;
                            
                            // Test writing if loading produced image data
                            if (!img.image.empty()) {
                                std::string out_uri;
                                std::string basepath = "test_output";
                                std::string filename = img.name + "." + test_formats[format_idx % test_formats.size()];
                                
                                tinygltf::WriteImageData(&basepath, &filename, &img,
                                                        embed_images, &fs_callbacks,
                                                        &uri_callbacks, &out_uri, nullptr);
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_008
