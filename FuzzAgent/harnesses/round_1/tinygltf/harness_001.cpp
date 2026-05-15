#include <fuzzer/FuzzedDataProvider.h>
#include "tiny_gltf.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace harness_001 {
/*
 * Fuzzing harness for tinygltf library file-based GLTF operations
 * 
 * This harness targets file-based loading and writing functions:
 * 1. LoadBinaryFromFile - Binary GLTF file loading
 * 2. LoadASCIIFromFile - ASCII GLTF file loading  
 * 3. Model equality comparison
 * 4. WriteGltfSceneToFile - GLTF file writing
 * 
 * The harness implements mock filesystem callbacks to simulate file I/O
 * using fuzzer-provided data, enabling comprehensive testing of file
 * processing code paths without actual disk access.
 */



// Mock filesystem for fuzzing - stores files in memory
class MockFileSystem {
public:
    struct FileInfo {
        std::vector<unsigned char> data;
        bool exists;
    };
    
    std::map<std::string, FileInfo> files;
    void* user_data;
    
    MockFileSystem() : user_data(nullptr) {}
    
    // FileExists callback implementation
    static bool MockFileExists(const std::string& abs_filename, void* user_data) {
        MockFileSystem* fs = static_cast<MockFileSystem*>(user_data);
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
        MockFileSystem* fs = static_cast<MockFileSystem*>(user_data);
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
        MockFileSystem* fs = static_cast<MockFileSystem*>(user_data);
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
        MockFileSystem* fs = static_cast<MockFileSystem*>(user_data);
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
    
    // Setup filesystem callbacks
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
    
    // URI encode callback (no-op for mocking)
    static bool MockURIEncode(const std::string& in_uri,
                              const std::string& object_type,
                              std::string* out_uri,
                              void* user_data) {
        *out_uri = in_uri;
        return true;
    }
    
    // URI decode callback (no-op for mocking)
    static bool MockURIDecode(const std::string& in_uri,
                              std::string* out_uri,
                              void* user_data) {
        *out_uri = in_uri;
        return true;
    }
    
    // Setup URI callbacks
    tinygltf::URICallbacks GetURICallbacks() {
        tinygltf::URICallbacks callbacks;
        callbacks.encode = MockURIEncode;
        callbacks.decode = MockURIDecode;
        callbacks.user_data = this;
        return callbacks;
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for filenames and file content
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume parameters for file operations
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t check_sections = fdp.ConsumeIntegral<uint8_t>();
    bool embed_images = fdp.ConsumeBool();
    bool embed_buffers = fdp.ConsumeBool();
    bool pretty_print = fdp.ConsumeBool();
    bool write_binary = fdp.ConsumeBool();
    
    // Consume filenames
    std::string binary_filename = fdp.ConsumeRandomLengthString(64);
    std::string ascii_filename = fdp.ConsumeRandomLengthString(64);
    std::string output_filename = fdp.ConsumeRandomLengthString(64);
    
    // Consume file content (remaining bytes)
    std::vector<uint8_t> file_content = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (file_content.empty()) return 0;
    
    // Setup mock filesystem with test files
    MockFileSystem mock_fs;
    MockURISystem mock_uri;
    
    // Create mock files with the consumed content
    MockFileSystem::FileInfo binary_file;
    binary_file.data = file_content;
    binary_file.exists = true;
    mock_fs.files[binary_filename] = binary_file;
    
    MockFileSystem::FileInfo ascii_file;
    ascii_file.data = file_content;
    ascii_file.exists = true;
    mock_fs.files[ascii_filename] = ascii_file;
    
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
    
    // Create model instances
    tinygltf::Model binary_model;
    tinygltf::Model ascii_model;
    tinygltf::Model output_model;
    
    // Test different file operations based on operation selector
    switch (operation) {
        case 0: {
            // Test LoadBinaryFromFile
            bool binary_ret = ctx.LoadBinaryFromFile(&binary_model, &err, &warn,
                                                    binary_filename,
                                                    check_sections);
            (void)binary_ret; // Result intentionally ignored
            break;
        }
        
        case 1: {
            // Test LoadASCIIFromFile  
            bool ascii_ret = ctx.LoadASCIIFromFile(&ascii_model, &err, &warn,
                                                  ascii_filename,
                                                  check_sections);
            (void)ascii_ret; // Result intentionally ignored
            break;
        }
        
        case 2: {
            // Test both file loading and model comparison
            bool binary_ret = ctx.LoadBinaryFromFile(&binary_model, &err, &warn,
                                                    binary_filename,
                                                    check_sections);
            (void)binary_ret;
            
            // Clear errors for second operation
            err.clear();
            warn.clear();
            
            bool ascii_ret = ctx.LoadASCIIFromFile(&ascii_model, &err, &warn,
                                                  ascii_filename,
                                                  check_sections);
            (void)ascii_ret;
            
            // Compare models using equality operator (if both loaded successfully)
            bool models_equal = (binary_model == ascii_model);
            (void)models_equal;
            break;
        }
        
        case 3: {
            // Test full file lifecycle: load, compare, write
            
            // First load a model
            bool load_ret = ctx.LoadBinaryFromFile(&binary_model, &err, &warn,
                                                  binary_filename,
                                                  check_sections);
            (void)load_ret;
            
            // Clear model for comparison test
            ascii_model = tinygltf::Model();
            
            // Try loading ASCII version
            err.clear();
            warn.clear();
            bool ascii_ret = ctx.LoadASCIIFromFile(&ascii_model, &err, &warn,
                                                  ascii_filename,
                                                  check_sections);
            (void)ascii_ret;
            
            // Compare models
            bool models_equal = (binary_model == ascii_model);
            (void)models_equal;
            
            // Write model to file (use binary_model as source)
            err.clear();
            warn.clear();
            
            // Create output file in mock filesystem
            MockFileSystem::FileInfo output_file;
            output_file.exists = false; // File doesn't exist yet
            mock_fs.files[output_filename] = output_file;
            
            bool write_ret = ctx.WriteGltfSceneToFile(&binary_model, output_filename,
                                                     embed_images, embed_buffers,
                                                     pretty_print, write_binary);
            (void)write_ret;
            break;
        }
    }
    
    return 0;
}

} // namespace harness_001
