#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace harness_002 {
/*
 * Fuzzing harness for tinygltf library image processing operations
 * 
 * This harness targets image loading, decoding, and writing functions:
 * 1. LoadImageData - Image data loading via stb_image
 * 2. WriteImageData - Image data writing via stb_image_write
 * 3. DecodeDataURI - Data URI decoding for embedded images
 * 4. IsDataURI - Data URI detection
 * 5. Image struct manipulation and processing
 * 
 * The harness implements a complete image processing lifecycle:
 * - Load GLTF model (or create mock one with images)
 * - Process images: check URIs, decode data URIs, load image data
 * - Manipulate image data (pixel buffer access)
 * - Write image data back (various formats)
 * - Save modified model
 */



// Mock filesystem for fuzzing - similar to harness_001 but focused on images
class MockImageFileSystem {
public:
    struct FileInfo {
        std::vector<unsigned char> data;
        bool exists;
    };
    
    std::map<std::string, FileInfo> files;
    void* user_data;
    
    MockImageFileSystem() : user_data(nullptr) {}
    
    // FileExists callback implementation
    static bool MockFileExists(const std::string& abs_filename, void* user_data) {
        MockImageFileSystem* fs = static_cast<MockImageFileSystem*>(user_data);
        if (!fs) return false;
        
        auto it = fs->files.find(abs_filename);
        return it != fs->files.end() && it->second.exists;
    }
    
    // ExpandFilePath callback implementation (no-op for mocking)
    static std::string MockExpandFilePath(const std::string& filepath, void* user_data) {
        // Return path as-is for mocking
        return filepath;
    }
    
    // ReadWholeFile callback implementation
    static bool MockReadWholeFile(std::vector<unsigned char>* out, 
                                  std::string* err, 
                                  const std::string& filepath, 
                                  void* user_data) {
        MockImageFileSystem* fs = static_cast<MockImageFileSystem*>(user_data);
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
        MockImageFileSystem* fs = static_cast<MockImageFileSystem*>(user_data);
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
        MockImageFileSystem* fs = static_cast<MockImageFileSystem*>(user_data);
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
class MockURICallbacks {
public:
    static bool MockURIDecode(const std::string& in_uri, std::string* out_uri, void* user_data) {
        // Simple decode: just return the input
        *out_uri = in_uri;
        return true;
    }
    
    static bool MockURIEncode(const std::string& in_uri, const std::string& object_type, 
                              std::string* out_uri, void* user_data) {
        // Simple encode: just return the input
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

// Generate mock image data for testing
std::vector<unsigned char> GenerateMockImageData(FuzzedDataProvider& fdp, int width, int height, int channels) {
    std::vector<unsigned char> image_data;
    size_t pixel_count = width * height * channels;
    
    // Generate random pixel data
    for (size_t i = 0; i < pixel_count; i++) {
        if (fdp.remaining_bytes() > 0) {
            image_data.push_back(fdp.ConsumeIntegral<uint8_t>());
        } else {
            image_data.push_back(static_cast<uint8_t>(i % 256));
        }
    }
    
    return image_data;
}

// Create a simple PNG header (8 bytes) followed by mock data
std::vector<unsigned char> CreateMockPNG(FuzzedDataProvider& fdp) {
    std::vector<unsigned char> png_data;
    
    // PNG signature (8 bytes)
    png_data.push_back(0x89); png_data.push_back(0x50); png_data.push_back(0x4E); png_data.push_back(0x47);
    png_data.push_back(0x0D); png_data.push_back(0x0A); png_data.push_back(0x1A); png_data.push_back(0x0A);
    
    // Add some mock PNG chunk data
    size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
    for (size_t i = 0; i < chunk_size && fdp.remaining_bytes() > 0; i++) {
        png_data.push_back(fdp.ConsumeIntegral<uint8_t>());
    }
    
    return png_data;
}

// Create a data URI from raw bytes
std::string CreateDataURI(const std::vector<unsigned char>& data, const std::string& mime_type) {
    // Simple base64-like encoding (not real base64 for simplicity)
    std::string encoded;
    for (unsigned char byte : data) {
        encoded.push_back(static_cast<char>((byte % 26) + 'A'));
    }
    
    return "data:" + mime_type + ";base64," + encoded;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different image processing paths
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>();
    
    // Consume image parameters
    int image_width = fdp.ConsumeIntegralInRange<int>(1, 64);
    int image_height = fdp.ConsumeIntegralInRange<int>(1, 64);
    int image_channels = fdp.ConsumeIntegralInRange<int>(1, 4);
    int image_bits = fdp.ConsumeBool() ? 8 : 16;
    bool embed_images = fdp.ConsumeBool();
    bool pretty_print = fdp.ConsumeBool();
    bool write_binary = fdp.ConsumeBool();
    
    // Consume remaining bytes for image data and filenames
    std::string image_filename = fdp.ConsumeRandomLengthString(64);
    std::string output_filename = fdp.ConsumeRandomLengthString(64);
    
    // Create mock filesystem
    MockImageFileSystem mock_fs;
    MockURICallbacks mock_uri;
    
    // Set up tinygltf context
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Set filesystem and URI callbacks
    tinygltf::FsCallbacks fs_callbacks = mock_fs.GetFsCallbacks();
    tinygltf::URICallbacks uri_callbacks = mock_uri.GetURICallbacks();
    
    if (!ctx.SetFsCallbacks(fs_callbacks, &err)) {
        return 0; // Failed to set filesystem callbacks
    }
    
    if (!ctx.SetURICallbacks(uri_callbacks, &err)) {
        return 0; // Failed to set URI callbacks
    }
    
    // Create a model with mock images
    tinygltf::Model model;
    
    // Add some images to the model
    int num_images = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < num_images; i++) {
        tinygltf::Image image;
        image.name = "image_" + std::to_string(i);
        image.width = image_width;
        image.height = image_height;
        image.component = image_channels;
        image.bits = image_bits;
        image.pixel_type = (image_bits == 8) ? 
            TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE : 
            TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
        
        // Decide whether to use URI or embedded data
        bool use_data_uri = fdp.ConsumeBool();
        if (use_data_uri && fdp.remaining_bytes() > 10) {
            // Create a data URI
            size_t data_size = fdp.ConsumeIntegralInRange<size_t>(10, 100);
            std::vector<unsigned char> uri_data = fdp.ConsumeBytes<unsigned char>(data_size);
            std::string mime_type = "image/png";
            image.uri = CreateDataURI(uri_data, mime_type);
            image.mimeType = mime_type;
        } else {
            // Use external file reference
            image.uri = "images/texture_" + std::to_string(i) + ".png";
            
            // Create mock image file in filesystem
            std::vector<unsigned char> mock_png = CreateMockPNG(fdp);
            MockImageFileSystem::FileInfo file_info;
            file_info.data = mock_png;
            file_info.exists = true;
            mock_fs.files[image.uri] = file_info;
        }
        
        model.images.push_back(image);
    }
    
    // Test different image processing operations based on selector
    switch (operation % 4) {
        case 0: {
            // Test IsDataURI and DecodeDataURI functions
            for (const auto& image : model.images) {
                if (!image.uri.empty()) {
                    // Test IsDataURI
                    bool is_data_uri = tinygltf::IsDataURI(image.uri);
                    (void)is_data_uri;
                    
                    // Test DecodeDataURI if it's a data URI
                    if (is_data_uri) {
                        std::vector<unsigned char> decoded_data;
                        std::string mime_type;
                        bool decode_success = tinygltf::DecodeDataURI(&decoded_data, mime_type, 
                                                                      image.uri, 0, false);
                        (void)decode_success;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test LoadImageData function directly
            // Create a simple struct matching LoadImageDataOption layout
            struct LocalLoadImageDataOption {
                bool preserve_channels{false};
                bool as_is{false};
            };
            LocalLoadImageDataOption load_option;
            load_option.as_is = fdp.ConsumeBool();
            load_option.preserve_channels = fdp.ConsumeBool();
            
            // Try to load image data for each image
            for (size_t i = 0; i < model.images.size(); i++) {
                tinygltf::Image& image = model.images[i];
                std::string load_err, load_warn;
                
                // Prepare image data for loading
                std::vector<unsigned char> img_data;
                std::string mime_type;
                
                if (!image.uri.empty() && tinygltf::IsDataURI(image.uri)) {
                    if (tinygltf::DecodeDataURI(&img_data, mime_type, image.uri, 0, false)) {
                        // Try loading the decoded data
                        bool load_success = tinygltf::LoadImageData(&image, static_cast<int>(i),
                                                                    &load_err, &load_warn, 0, 0,
                                                                    img_data.data(), 
                                                                    static_cast<int>(img_data.size()),
                                                                    &load_option);
                        (void)load_success;
                    }
                } else if (!image.uri.empty()) {
                    // Try loading from mock filesystem
                    auto it = mock_fs.files.find(image.uri);
                    if (it != mock_fs.files.end() && !it->second.data.empty()) {
                        const auto& file_data = it->second.data;
                        bool load_success = tinygltf::LoadImageData(&image, static_cast<int>(i),
                                                                    &load_err, &load_warn, 0, 0,
                                                                    file_data.data(), 
                                                                    static_cast<int>(file_data.size()),
                                                                    &load_option);
                        (void)load_success;
                    }
                }
            }
            break;
        }
        
        case 2: {
            // Test WriteImageData function
            // First, ensure we have some image data to write
            for (auto& image : model.images) {
                // Generate mock image data if needed
                if (image.image.empty() && image.width > 0 && image.height > 0 && image.component > 0) {
                    image.image = GenerateMockImageData(fdp, image.width, image.height, image.component);
                }
            }
            
            // Test WriteImageData for each image
            for (const auto& image : model.images) {
                if (!image.image.empty()) {
                    std::string out_uri;
                    std::string basepath = ".";
                    std::string filename = "output_" + image.name + ".png";
                    
                    // Test WriteImageData with different formats
                    bool write_success = tinygltf::WriteImageData(&basepath, &filename, &image,
                                                                  embed_images, &fs_callbacks,
                                                                  &uri_callbacks, &out_uri, nullptr);
                    (void)write_success;
                }
            }
            break;
        }
        
        case 3: {
            // Test complete image processing pipeline
            // 1. Check data URIs
            for (const auto& image : model.images) {
                if (!image.uri.empty()) {
                    bool is_data_uri = tinygltf::IsDataURI(image.uri);
                    (void)is_data_uri;
                }
            }
            
            // 2. Try to load image data using local struct
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
                
                if (!image.uri.empty()) {
                    // Get image data (either from file or data URI)
                    std::vector<unsigned char> img_data;
                    
                    if (tinygltf::IsDataURI(image.uri)) {
                        std::string mime_type;
                        tinygltf::DecodeDataURI(&img_data, mime_type, image.uri, 0, false);
                    } else {
                        // Read from mock filesystem
                        auto it = mock_fs.files.find(image.uri);
                        if (it != mock_fs.files.end()) {
                            img_data = it->second.data;
                        }
                    }
                    
                    if (!img_data.empty()) {
                        bool load_success = tinygltf::LoadImageData(&image, static_cast<int>(i),
                                                                    &load_err, &load_warn, 0, 0,
                                                                    img_data.data(), 
                                                                    static_cast<int>(img_data.size()),
                                                                    &load_option);
                        (void)load_success;
                    }
                }
            }
            
            // 3. Write processed images
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
            
            // 4. Write the entire model to file
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

} // namespace harness_002
