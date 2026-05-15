#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

namespace harness_006 {
/*
 * Fuzzing harness for tinygltf library stb_image.h module integration
 * 
 * This harness specifically targets stb_image.h module with 4.38% coverage
 * and 150+ undiscovered branches identified in coverage analysis:
 * - tinygltf::LoadImageData (54 undiscovered branches)
 * - tinygltf::WriteImageData (43 undiscovered branches)
 * - stbi_load (12 undiscovered branches)
 * - stbi_load_16 (12 undiscovered branches)
 * - stbi_loadf (10 undiscovered branches)
 * - stbi_info (8 undiscovered branches)
 * - stbi_write_png (4 undiscovered branches)
 * - stbi_write_jpg (2 undiscovered branches)
 * - stbi_write_bmp (2 undiscovered branches)
 * - stbi_write_tga (2 undiscovered branches)
 * - stbi_write_hdr (2 undiscovered branches)
 * 
 * The harness implements a complete image processing lifecycle targeting
 * the exact low-coverage stb_image functions through direct integration:
 * 1. Initialize TinyGLTF context with filesystem and URI callbacks
 * 2. Read image files using GetFileSizeInBytes and ReadWholeFile
 * 3. Use stbi_info to get image metadata
 * 4. Call stbi_load/stbi_load_16/stbi_loadf to load image data
 * 5. Create tinygltf::Image object with loaded data
 * 6. Call tinygltf::LoadImageData via image loader callback
 * 7. Process/modify image data
 * 8. Call tinygltf::WriteImageData via image writer callback
 * 9. Use stbi_write_* functions to write processed images
 * 10. Call WriteWholeFile to save output
 */



// Forward declarations for stb_image functions
extern "C" {
    // stb_image types
    typedef unsigned short stbi_us;
    
    // stb_image functions
    extern int stbi_info_from_memory(const unsigned char* buffer, int len, int* x, int* y, int* comp);
    extern unsigned char* stbi_load_from_memory(const unsigned char* buffer, int len, int* x, int* y, int* comp, int req_comp);
    extern float* stbi_loadf_from_memory(const unsigned char* buffer, int len, int* x, int* y, int* comp, int req_comp);
    extern stbi_us* stbi_load_16_from_memory(const unsigned char* buffer, int len, int* x, int* y, int* comp, int req_comp);
    extern void stbi_image_free(void* retval_from_stbi_load);
    
    // stb_image_write functions  
    extern int stbi_write_png_to_func(void (*func)(void*, void*, int), void* context,
                                     int w, int h, int comp, const void* data, int stride_in_bytes);
    extern int stbi_write_bmp_to_func(void (*func)(void*, void*, int), void* context,
                                     int w, int h, int comp, const void* data);
    extern int stbi_write_tga_to_func(void (*func)(void*, void*, int), void* context,
                                     int w, int h, int comp, const void* data);
    extern int stbi_write_hdr_to_func(void (*func)(void*, void*, int), void* context,
                                     int w, int h, int comp, const float* data);
    extern int stbi_write_jpg_to_func(void (*func)(void*, void*, int), void* context,
                                     int w, int h, int comp, const void* data, int quality);
}

// Mock filesystem for fuzzing
class MockStbImageFileSystem {
public:
    struct FileInfo {
        std::vector<unsigned char> data;
        bool exists;
    };
    
    std::map<std::string, FileInfo> files;
    void* user_data;
    
    MockStbImageFileSystem() : user_data(nullptr) {}
    
    // FileExists callback implementation
    static bool MockFileExists(const std::string& abs_filename, void* user_data) {
        MockStbImageFileSystem* fs = static_cast<MockStbImageFileSystem*>(user_data);
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
        MockStbImageFileSystem* fs = static_cast<MockStbImageFileSystem*>(user_data);
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
        MockStbImageFileSystem* fs = static_cast<MockStbImageFileSystem*>(user_data);
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
        MockStbImageFileSystem* fs = static_cast<MockStbImageFileSystem*>(user_data);
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
        fs.GetFileSizeInBytes = &MockGetFileSizeInBytes;
        fs.user_data = const_cast<void*>(reinterpret_cast<const void*>(this));
        return fs;
    }
};

// Mock URI callbacks
class MockStbImageURICallbacks {
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

// Custom LoadImageData function that exercises stb_image functions directly
bool CustomLoadImageData(tinygltf::Image *image, const int image_idx, std::string *err,
                        std::string *warn, int req_width, int req_height,
                        const unsigned char *bytes, int size, void *user_data) {
    (void)image_idx; (void)warn; (void)req_width; (void)req_height; (void)user_data;
    
    if (!bytes || size <= 0) {
        if (err) *err = "Invalid image data";
        return false;
    }
    
    // First use stbi_info to get metadata
    int width = 0, height = 0, comp = 0;
    int info_result = stbi_info_from_memory(bytes, size, &width, &height, &comp);
    (void)info_result;
    
    if (width <= 0 || height <= 0 || comp <= 0) {
        // Try to load anyway to exercise error paths
        width = 64;
        height = 64;
        comp = 4;
    }
    
    // Choose which stbi_load function to use based on image index
    unsigned char* data_8bit = nullptr;
    stbi_us* data_16bit = nullptr;
    float* data_float = nullptr;
    
    switch (image_idx % 3) {
        case 0:
            // Use stbi_load (8-bit)
            data_8bit = stbi_load_from_memory(bytes, size, &width, &height, &comp, 0);
            if (data_8bit) {
                image->width = width;
                image->height = height;
                image->component = comp;
                image->bits = 8;
                image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
                image->image.assign(data_8bit, data_8bit + (width * height * comp));
                stbi_image_free(data_8bit);
                return true;
            }
            break;
            
        case 1:
            // Use stbi_load_16 (16-bit)
            data_16bit = stbi_load_16_from_memory(bytes, size, &width, &height, &comp, 0);
            if (data_16bit) {
                image->width = width;
                image->height = height;
                image->component = comp;
                image->bits = 16;
                image->pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
                // Convert 16-bit data to bytes
                size_t pixel_count = width * height * comp;
                image->image.resize(pixel_count * 2);
                memcpy(image->image.data(), data_16bit, pixel_count * 2);
                stbi_image_free(data_16bit);
                return true;
            }
            break;
            
        case 2:
            // Use stbi_loadf (float)
            data_float = stbi_loadf_from_memory(bytes, size, &width, &height, &comp, 0);
            if (data_float) {
                image->width = width;
                image->height = height;
                image->component = comp;
                image->bits = 32;
                image->pixel_type = TINYGLTF_COMPONENT_TYPE_FLOAT;
                // Convert float data to bytes
                size_t pixel_count = width * height * comp;
                image->image.resize(pixel_count * sizeof(float));
                memcpy(image->image.data(), data_float, pixel_count * sizeof(float));
                stbi_image_free(data_float);
                return true;
            }
            break;
    }
    
    if (err) *err = "Failed to load image with all stb_image methods";
    return false;
}

// Custom WriteImageData function that exercises stb_image_write functions directly
bool CustomWriteImageData(const std::string *basepath, const std::string *filename,
                         const tinygltf::Image *image, bool embedImages,
                         const tinygltf::FsCallbacks* fs_cb, const tinygltf::URICallbacks *uri_cb,
                         std::string *out_uri, void *user_data) {
    (void)basepath; (void)embedImages; (void)fs_cb; (void)uri_cb; (void)user_data;
    
    if (!image || !filename || !out_uri) {
        return false;
    }
    
    if (image->image.empty() || image->width <= 0 || image->height <= 0 || image->component <= 0) {
        return false;
    }
    
    // Simple write callback that collects data
    struct WriteContext {
        std::vector<unsigned char> data;
    };
    
    auto write_func = [](void* context, void* data, int size) {
        WriteContext* ctx = static_cast<WriteContext*>(context);
        unsigned char* bytes = static_cast<unsigned char*>(data);
        ctx->data.insert(ctx->data.end(), bytes, bytes + size);
    };
    
    WriteContext context;
    int write_result = 0;
    
    // Choose which stb_write function to use based on filename hash
    size_t filename_hash = std::hash<std::string>{}(*filename);
    
    switch (filename_hash % 5) {
        case 0:
            // PNG
            write_result = stbi_write_png_to_func(write_func, &context,
                                                 image->width, image->height, image->component,
                                                 image->image.data(), 0);
            break;
        case 1:
            // BMP
            write_result = stbi_write_bmp_to_func(write_func, &context,
                                                 image->width, image->height, image->component,
                                                 image->image.data());
            break;
        case 2:
            // TGA
            write_result = stbi_write_tga_to_func(write_func, &context,
                                                 image->width, image->height, image->component,
                                                 image->image.data());
            break;
        case 3:
            // HDR (requires float data)
            if (image->bits == 32 && image->pixel_type == TINYGLTF_COMPONENT_TYPE_FLOAT) {
                write_result = stbi_write_hdr_to_func(write_func, &context,
                                                     image->width, image->height, image->component,
                                                     reinterpret_cast<const float*>(image->image.data()));
            }
            break;
        case 4:
            // JPG
            write_result = stbi_write_jpg_to_func(write_func, &context,
                                                 image->width, image->height, image->component,
                                                 image->image.data(), 85);
            break;
    }
    
    if (write_result) {
        *out_uri = *filename;
        return true;
    }
    
    return false;
}

// Generate mock image file data
std::vector<unsigned char> GenerateMockImageFile(FuzzedDataProvider& fdp, size_t min_size = 100) {
    std::vector<unsigned char> data;
    
    // Create a simple header to make it look like an image file
    // PNG signature
    data.push_back(0x89); data.push_back(0x50); data.push_back(0x4E); data.push_back(0x47);
    data.push_back(0x0D); data.push_back(0x0A); data.push_back(0x1A); data.push_back(0x0A);
    
    // Add some chunk-like data
    size_t remaining = fdp.ConsumeIntegralInRange<size_t>(min_size, min_size + 500);
    for (size_t i = 0; i < remaining && fdp.remaining_bytes() > 0; i++) {
        data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    return data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size for meaningful testing
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume image parameters
    int num_images = fdp.ConsumeIntegralInRange<int>(1, 5);
    bool embed_images = fdp.ConsumeBool();
    bool pretty_print = fdp.ConsumeBool();
    bool write_binary = fdp.ConsumeBool();
    
    // Consume remaining bytes for filenames and image data
    std::string output_filename = fdp.ConsumeRandomLengthString(64);
    
    // Create mock filesystem and URI callbacks
    MockStbImageFileSystem mock_fs;
    MockStbImageURICallbacks mock_uri;
    
    // Set up tinygltf context
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Set filesystem and URI callbacks
    tinygltf::FsCallbacks fs_callbacks = mock_fs.GetFsCallbacks();
    tinygltf::URICallbacks uri_callbacks = mock_uri.GetURICallbacks();
    
    if (!ctx.SetFsCallbacks(fs_callbacks, &err)) {
        return 0;
    }
    
    if (!ctx.SetURICallbacks(uri_callbacks, &err)) {
        return 0;
    }
    
    // Set custom image loader and writer
    ctx.SetImageLoader(&CustomLoadImageData, nullptr);
    ctx.SetImageWriter(&CustomWriteImageData, nullptr);
    
    // Create a model with images
    tinygltf::Model model;
    
    // Add mock image files to filesystem and create Image objects
    for (int i = 0; i < num_images; i++) {
        // Generate mock image file data
        std::vector<unsigned char> image_data = GenerateMockImageFile(fdp);
        
        // Add to filesystem
        std::string filename = "image_" + std::to_string(i) + ".png";
        MockStbImageFileSystem::FileInfo file_info;
        file_info.data = image_data;
        file_info.exists = true;
        mock_fs.files[filename] = file_info;
        
        // Create Image object
        tinygltf::Image image;
        image.name = "TestImage_" + std::to_string(i);
        image.uri = filename;
        image.mimeType = "image/png";
        
        model.images.push_back(image);
    }
    
    // Test different image processing operations based on selector
    switch (operation % 4) {
        case 0: {
            // Test 1: Direct stbi_info calls on image files
            for (const auto& file_pair : mock_fs.files) {
                int width = 0, height = 0, comp = 0;
                const auto& file_data = file_pair.second.data;
                int info_result = stbi_info_from_memory(file_data.data(), 
                                                       static_cast<int>(file_data.size()),
                                                       &width, &height, &comp);
                (void)info_result;
                (void)width; (void)height; (void)comp;
            }
            break;
        }
        
        case 1: {
            // Test 2: Load images using custom loader (exercises stbi_load functions)
            for (size_t i = 0; i < model.images.size(); i++) {
                auto& image = model.images[i];
                auto it = mock_fs.files.find(image.uri);
                if (it != mock_fs.files.end()) {
                    std::string load_err, load_warn;
                    bool load_success = CustomLoadImageData(&image, static_cast<int>(i),
                                                           &load_err, &load_warn, 0, 0,
                                                           it->second.data.data(),
                                                           static_cast<int>(it->second.data.size()),
                                                           nullptr);
                    (void)load_success;
                }
            }
            break;
        }
        
        case 2: {
            // Test 3: Write images using custom writer (exercises stbi_write functions)
            // First ensure images are loaded
            for (size_t i = 0; i < model.images.size(); i++) {
                auto& image = model.images[i];
                auto it = mock_fs.files.find(image.uri);
                if (it != mock_fs.files.end() && image.image.empty()) {
                    // Load if not already loaded
                    std::string load_err, load_warn;
                    CustomLoadImageData(&image, static_cast<int>(i),
                                       &load_err, &load_warn, 0, 0,
                                       it->second.data.data(),
                                       static_cast<int>(it->second.data.size()),
                                       nullptr);
                }
                
                // Write if we have image data
                if (!image.image.empty()) {
                    std::string out_uri;
                    std::string basepath = ".";
                    std::string filename = "output_" + std::to_string(i);
                    
                    // Try different extensions to trigger different stb_write functions
                    const char* extensions[] = {".png", ".bmp", ".tga", ".hdr", ".jpg"};
                    for (const char* ext : extensions) {
                        std::string full_filename = filename + ext;
                        bool write_success = CustomWriteImageData(&basepath, &full_filename,
                                                                 &image, embed_images,
                                                                 &fs_callbacks, &uri_callbacks,
                                                                 &out_uri, nullptr);
                        (void)write_success;
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test 4: Complete lifecycle with tinygltf functions
            // Load images via tinygltf
            for (size_t i = 0; i < model.images.size(); i++) {
                auto& image = model.images[i];
                auto it = mock_fs.files.find(image.uri);
                if (it != mock_fs.files.end()) {
                    std::string load_err, load_warn;
                    bool load_success = tinygltf::LoadImageData(&image, static_cast<int>(i),
                                                               &load_err, &load_warn, 0, 0,
                                                               it->second.data.data(),
                                                               static_cast<int>(it->second.data.size()),
                                                               nullptr);
                    (void)load_success;
                }
            }
            
            // Write images via tinygltf
            for (const auto& image : model.images) {
                if (!image.image.empty()) {
                    std::string out_uri;
                    std::string basepath = "output";
                    std::string filename = "tinygltf_" + image.name + ".png";
                    
                    bool write_success = tinygltf::WriteImageData(&basepath, &filename, &image,
                                                                 embed_images, &fs_callbacks,
                                                                 &uri_callbacks, &out_uri, nullptr);
                    (void)write_success;
                }
            }
            
            // Write model to file to exercise full pipeline
            if (!model.images.empty()) {
                bool write_success = ctx.WriteGltfSceneToFile(&model, output_filename,
                                                             embed_images, false,
                                                             pretty_print, write_binary);
                (void)write_success;
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_006
