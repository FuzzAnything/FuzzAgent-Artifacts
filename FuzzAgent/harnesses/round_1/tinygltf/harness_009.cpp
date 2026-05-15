#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>

namespace harness_009 {
/*
 * Fuzzing harness for tinygltf library file I/O and image data manipulation APIs
 * 
 * This harness targets critical coverage gaps in file I/O and image processing:
 * 1. WriteImageData - 43 undiscovered branches (0% coverage)
 * 2. ReadWholeFile - 20 undiscovered branches (0% coverage)
 * 3. GetFileSizeInBytes - 20 undiscovered branches (0% coverage)
 * 4. WriteWholeFile - 8 undiscovered branches (0% coverage)
 * 5. FileExists - 6 undiscovered branches (0% coverage)
 * 6. LoadImageData - 48 undiscovered branches (only 12.5% covered)
 * 
 * Total: 145+ undiscovered branches across logically related file I/O operations
 * 
 * Implementation follows the exact invocation sequence from coverage guidance:
 * 1. Create TinyGLTF instance
 * 2. Set up FsCallbacks and URICallbacks
 * 3. Load a simple GLTF model containing image data
 * 4. Extract Image object from the loaded Model
 * 5. Call WriteImageData to write image to temporary file
 * 6. Call FileExists to verify file was created
 * 7. Call GetFileSizeInBytes to check file size
 * 8. Call ReadWholeFile to read the written image data back
 * 9. Call LoadImageData to process the image data
 * 10. Call WriteWholeFile to write raw data
 * 11. Clean up temporary files and resources
 */



// Mock filesystem for fuzzing - stores files in memory
class MockFileIOFileSystem {
public:
    struct FileInfo {
        std::vector<unsigned char> data;
        bool exists;
    };
    
    std::map<std::string, FileInfo> files;
    void* user_data;
    
    MockFileIOFileSystem() : user_data(nullptr) {}
    
    // FileExists callback implementation
    static bool MockFileExists(const std::string& abs_filename, void* user_data) {
        MockFileIOFileSystem* fs = static_cast<MockFileIOFileSystem*>(user_data);
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
        MockFileIOFileSystem* fs = static_cast<MockFileIOFileSystem*>(user_data);
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
        MockFileIOFileSystem* fs = static_cast<MockFileIOFileSystem*>(user_data);
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
        MockFileIOFileSystem* fs = static_cast<MockFileIOFileSystem*>(user_data);
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
    
    // Get FsCallbacks structure
    tinygltf::FsCallbacks GetFsCallbacks() {
        tinygltf::FsCallbacks callbacks;
        callbacks.FileExists = MockFileExists;
        callbacks.ExpandFilePath = MockExpandFilePath;
        callbacks.ReadWholeFile = MockReadWholeFile;
        callbacks.WriteWholeFile = MockWriteWholeFile;
        callbacks.GetFileSizeInBytes = MockGetFileSizeInBytes;
        callbacks.user_data = this;
        return callbacks;
    }
};
// Mock URI callbacks for fuzzing
class MockURISystem {
public:
    void* user_data;
    
    MockURISystem() : user_data(nullptr) {}
    
    // Decode callback implementation
    static bool MockURIDecode(const std::string& in_uri, std::string* out_uri, 
                              void* user_data) {
        // Simple implementation: pass through
        *out_uri = in_uri;
        return true;
    }
    
    // Encode callback implementation
    static bool MockURIEncode(const std::string& in_uri, const std::string& asset_type,
                              std::string* out_uri, void* user_data) {
        // Simple implementation: pass through
        *out_uri = in_uri;
        return true;
    }
    
    // Get URICallbacks structure
    tinygltf::URICallbacks GetURICallbacks() {
        tinygltf::URICallbacks callbacks;
        callbacks.decode = MockURIDecode;
        callbacks.encode = MockURIEncode;
        callbacks.user_data = this;
        return callbacks;
    }
};

// Helper function to generate mock image data
std::vector<unsigned char> GenerateMockImageData(FuzzedDataProvider& fdp, 
                                                 int& width, int& height, int& components) {
    size_t data_size = width * height * components;
    if (data_size == 0) return {};
    
    // Ensure we have enough data
    if (fdp.remaining_bytes() < data_size) {
        // If not enough data, reduce dimensions to match available bytes
        // Try to keep aspect ratio roughly similar
        size_t available_bytes = fdp.remaining_bytes();
        if (available_bytes < static_cast<size_t>(components)) {
            // Not even enough for one pixel
            width = 0;
            height = 0;
            return {};
        }
        
        // Calculate max pixels we can have with available bytes
        size_t max_pixels = available_bytes / components;
        
        // Adjust dimensions to fit within max_pixels
        if (max_pixels < static_cast<size_t>(width * height)) {
            // Reduce dimensions while trying to maintain aspect ratio
            double aspect_ratio = static_cast<double>(width) / height;
            height = static_cast<int>(sqrt(max_pixels / aspect_ratio));
            width = static_cast<int>(height * aspect_ratio);
            
            // Ensure minimum dimensions of 1
            if (width < 1) width = 1;
            if (height < 1) height = 1;
            
            // Recalculate data_size with adjusted dimensions
            data_size = width * height * components;
            
            // Double-check we have enough bytes
            if (data_size > available_bytes) {
                // Last resort: use 1x1 image
                width = 1;
                height = 1;
                data_size = components;
            }
        }
    }
    
    if (data_size == 0) return {};
    
    // Consume bytes for image data
    std::vector<unsigned char> image_data = fdp.ConsumeBytes<unsigned char>(data_size);
    
    // Ensure we have the right size (in case fdp.ConsumeBytes returned less)
    if (image_data.size() < data_size) {
        // Pad with zeros
        image_data.resize(data_size, 0);
    }
    
    return image_data;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful testing
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume parsing options
    uint8_t check_sections = fdp.ConsumeIntegral<uint8_t>();
    bool embed_images = fdp.ConsumeBool();
    bool as_is = fdp.ConsumeBool();
    
    // Consume image parameters
    int image_width = fdp.ConsumeIntegralInRange<int>(1, 256);
    int image_height = fdp.ConsumeIntegralInRange<int>(1, 256);
    int image_components = fdp.ConsumeIntegralInRange<int>(1, 4);
    
    // Consume remaining bytes for file content
    std::vector<unsigned char> file_content = fdp.ConsumeRemainingBytes<unsigned char>();
    if (file_content.empty()) return 0;
    
    // Create mock filenames
    std::string gltf_filename = "test.gltf";
    std::string image_filename = "test_image.png";
    std::string output_filename = "output_image.png";
    std::string raw_filename = "raw_data.bin";
    
    // Initialize mock filesystem and URI system
    MockFileIOFileSystem mock_fs;
    MockURISystem mock_uri;
    
    // Create mock GLTF file with image data
    MockFileIOFileSystem::FileInfo gltf_file;
    gltf_file.data = file_content;
    gltf_file.exists = true;
    mock_fs.files[gltf_filename] = gltf_file;
    
    // Create TinyGLTF context
    tinygltf::TinyGLTF ctx;
    std::string err;
    std::string warn;
    
    // Setup filesystem and URI callbacks
    tinygltf::FsCallbacks fs_callbacks = mock_fs.GetFsCallbacks();
    tinygltf::URICallbacks uri_callbacks = mock_uri.GetURICallbacks();
    
    if (!ctx.SetFsCallbacks(fs_callbacks, &err)) {
        return 0; // Failed to set filesystem callbacks
    }
    
    if (!ctx.SetURICallbacks(uri_callbacks, &err)) {
        return 0; // Failed to set URI callbacks
    }
    
    // Create model instance
    tinygltf::Model model;
    
    // Test different file I/O and image operations based on operation selector
    switch (operation) {
        case 0: {
            // Test 1: Load GLTF model and test file I/O operations
            
            // Load model from mock file
            bool load_ret = ctx.LoadASCIIFromFile(&model, &err, &warn,
                                                gltf_filename, check_sections);
            (void)load_ret;
            
            // If model has no images, create a mock one
            if (model.images.empty()) {
                tinygltf::Image mock_image;
                mock_image.name = "mock_image";
                mock_image.width = image_width;
                mock_image.height = image_height;
                mock_image.component = image_components;
                mock_image.bits = 8;
                mock_image.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
                mock_image.image = GenerateMockImageData(fdp, image_width, image_height, image_components);
                // Update dimensions in case GenerateMockImageData adjusted them
                mock_image.width = image_width;
                mock_image.height = image_height;
                mock_image.component = image_components;
                // Add validation: ensure image data size matches dimensions
                if (mock_image.image.size() != static_cast<size_t>(image_width * image_height * image_components)) {
                    return 0;
                }
                model.images.push_back(mock_image);
            }
            
            // Extract first image object
            if (!model.images.empty()) {
                const tinygltf::Image& image = model.images[0];
                
                // Test 5: Call WriteImageData to write image to temporary file
                if (!image.image.empty()) {
                    std::string out_uri;
                    std::string basepath = ".";
                    
                    // Test WriteImageData with PNG format
                    bool write_image_success = tinygltf::WriteImageData(&basepath, &image_filename, &image,
                                                                        embed_images, &fs_callbacks,
                                                                        &uri_callbacks, &out_uri, nullptr);
                    (void)write_image_success;
                    
                    // Test 6: Call FileExists to verify file was created
                    if (write_image_success) {
                        bool file_exists = tinygltf::FileExists(image_filename, nullptr);
                        (void)file_exists;
                        
                        // Also test through mock filesystem
                        bool mock_file_exists = mock_fs.MockFileExists(image_filename, &mock_fs);
                        (void)mock_file_exists;
                    }
                }
            }
            break;
        }
        
        case 1: {
            // Test 2: File size operations and reading
            
            // Create a test file in mock filesystem
            MockFileIOFileSystem::FileInfo test_file;
            test_file.data = file_content;
            test_file.exists = true;
            mock_fs.files["test_file.bin"] = test_file;
            
            // Test 7: Call GetFileSizeInBytes to check file size
            size_t file_size = 0;
            std::string size_err;
            bool get_size_success = tinygltf::GetFileSizeInBytes(&file_size, &size_err, 
                                                                 "test_file.bin", nullptr);
            (void)get_size_success;
            (void)file_size;
            
            // Also test through mock filesystem
            size_t mock_file_size = 0;
            std::string mock_size_err;
            bool mock_get_size_success = mock_fs.MockGetFileSizeInBytes(&mock_file_size, 
                                                                        &mock_size_err,
                                                                        "test_file.bin", &mock_fs);
            (void)mock_get_size_success;
            (void)mock_file_size;
            
            // Test 8: Call ReadWholeFile to read the written image data back
            std::vector<unsigned char> read_data;
            std::string read_err;
            bool read_success = tinygltf::ReadWholeFile(&read_data, &read_err,
                                                       "test_file.bin", nullptr);
            (void)read_success;
            
            // Also test through mock filesystem
            std::vector<unsigned char> mock_read_data;
            std::string mock_read_err;
            bool mock_read_success = mock_fs.MockReadWholeFile(&mock_read_data, &mock_read_err,
                                                              "test_file.bin", &mock_fs);
            (void)mock_read_success;
            
            break;
        }
        
        case 2: {
            // Test 3: Image data loading and writing
            
            // Create a mock image for testing
            tinygltf::Image test_image;
            test_image.name = "test_load_image";
            test_image.width = image_width;
            test_image.height = image_height;
            test_image.component = image_components;
            test_image.bits = 8;
            test_image.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
            
            // Generate or consume image data
            if (fdp.remaining_bytes() > 0) {
                test_image.image = GenerateMockImageData(fdp, image_width, image_height, image_components);
                // Update dimensions in case GenerateMockImageData adjusted them
                test_image.width = image_width;
                test_image.height = image_height;
                test_image.component = image_components;
                // Add validation: ensure image data size matches dimensions
                if (test_image.image.size() != static_cast<size_t>(image_width * image_height * image_components)) {
                    return 0;
                }
            }
            
            // Test 9: Call LoadImageData to process the image data
            if (!test_image.image.empty()) {
                std::string load_err, load_warn;
                
                // Create load options struct
                struct LocalLoadImageDataOption {
                    bool preserve_channels{false};
                    bool as_is{false};
                };
                LocalLoadImageDataOption load_option;
                load_option.as_is = as_is;
                load_option.preserve_channels = true;
                
                // Try to load the image data
                bool load_data_success = tinygltf::LoadImageData(&test_image, 0, &load_err, &load_warn,
                                                                 0, 0, test_image.image.data(),
                                                                 static_cast<int>(test_image.image.size()),
                                                                 &load_option);
                (void)load_data_success;
            }
            
            // Test 10: Call WriteWholeFile to write raw data
            if (!test_image.image.empty()) {
                std::string write_err;
                bool write_success = tinygltf::WriteWholeFile(&write_err, raw_filename,
                                                             test_image.image, nullptr);
                (void)write_success;
                
                // Also test through mock filesystem
                std::string mock_write_err;
                bool mock_write_success = mock_fs.MockWriteWholeFile(&mock_write_err, raw_filename,
                                                                    test_image.image, &mock_fs);
                (void)mock_write_success;
            }
            
            break;
        }
        
        case 3: {
            // Test 4: Complete integrated workflow
            
            // Load model
            bool load_ret = ctx.LoadASCIIFromFile(&model, &err, &warn,
                                                gltf_filename, check_sections);
            (void)load_ret;
            
            // Ensure we have an image
            if (model.images.empty()) {
                tinygltf::Image mock_image;
                mock_image.name = "integrated_test_image";
                mock_image.width = image_width;
                mock_image.height = image_height;
                mock_image.component = image_components;
                mock_image.bits = 8;
                mock_image.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
                
                if (fdp.remaining_bytes() > 0) {
                    mock_image.image = GenerateMockImageData(fdp, image_width, image_height, image_components);
                }
                
                model.images.push_back(mock_image);
            }
            
            // Test the complete sequence
            for (size_t i = 0; i < model.images.size() && i < 1; i++) {
                const tinygltf::Image& image = model.images[i];
                
                if (!image.image.empty()) {
                    // Step 1: Write image data
                    std::string out_uri;
                    std::string basepath = ".";
                    bool write_image_success = tinygltf::WriteImageData(&basepath, &output_filename, &image,
                                                                        embed_images, &fs_callbacks,
                                                                        &uri_callbacks, &out_uri, nullptr);
                    (void)write_image_success;
                    
                    // Step 2: Check if file exists
                    if (write_image_success) {
                        bool file_exists = mock_fs.MockFileExists(output_filename, &mock_fs);
                        (void)file_exists;
                        
                        // Step 3: Get file size
                        size_t file_size = 0;
                        std::string size_err;
                        bool get_size_success = mock_fs.MockGetFileSizeInBytes(&file_size, &size_err,
                                                                              output_filename, &mock_fs);
                        (void)get_size_success;
                        
                        // Step 4: Read file back
                        std::vector<unsigned char> read_data;
                        std::string read_err;
                        bool read_success = mock_fs.MockReadWholeFile(&read_data, &read_err,
                                                                     output_filename, &mock_fs);
                        (void)read_success;
                        
                        // Step 5: Load image data from read data
                        if (read_success && !read_data.empty()) {
                            tinygltf::Image loaded_image = image;
                            std::string load_err, load_warn;
                            
                            struct LocalLoadImageDataOption {
                                bool preserve_channels{false};
                                bool as_is{false};
                            };
                            LocalLoadImageDataOption load_option;
                            load_option.as_is = false;
                            load_option.preserve_channels = true;
                            
                            bool load_data_success = tinygltf::LoadImageData(&loaded_image, 
                                                                             static_cast<int>(i),
                                                                             &load_err, &load_warn,
                                                                             0, 0, read_data.data(),
                                                                             static_cast<int>(read_data.size()),
                                                                             &load_option);
                            (void)load_data_success;
                        }
                    }
                }
            }
            break;
        }
    }
    
    // Cleanup: Resources are automatically cleaned up when objects go out of scope
    
    return 0;
}

} // namespace harness_009
