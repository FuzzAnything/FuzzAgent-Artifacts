#include <fuzzer/FuzzedDataProvider.h>
#include "magic.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>

namespace harness_002 {
/*
 * Specialized fuzzing harness for libmagic file and descriptor operations
 * Primary targets: magic_descriptor (0% coverage, 4 undiscovered branches)
 *                  magic_file (0% coverage, 2 undiscovered branches)
 * Secondary targets: magic_open, magic_load/magic_load_buffers, magic_close
 * Focus: Comprehensive testing of file type identification via file descriptors and file paths
 * Differentiator: Tests edge cases, special files, and diverse invocation patterns
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size for meaningful testing
    // Need: operation selection + flags + buffer size + some file data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size data first
    int flags = fdp.ConsumeIntegral<int>();
    int operation = fdp.ConsumeIntegralInRange<int>(0, 5); // 0-5 different testing strategies
    bool use_mime = fdp.ConsumeBool();
    bool use_symlink = fdp.ConsumeBool();
    bool use_raw = fdp.ConsumeBool();
    bool use_continue = fdp.ConsumeBool();
    
    // Build actual flags from consumed booleans
    int actual_flags = 0;
    if (use_mime) actual_flags |= MAGIC_MIME;
    if (use_symlink) actual_flags |= MAGIC_SYMLINK;
    if (use_raw) actual_flags |= MAGIC_RAW;
    if (use_continue) actual_flags |= MAGIC_CONTINUE;
    actual_flags |= (flags & 0xFFFFF); // Add random flags from input

    // Consume variable-size data
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    std::string filename_template = "/tmp/fuzz_magic_fd_" + std::to_string(getpid());
    std::string temp_filename = filename_template + "_data";

    // Create magic instance
    magic_t magic = magic_open(actual_flags);
    if (magic == nullptr) {
        return 0;
    }

    // Try to load magic database - use different strategies
    int load_result = -1;
    if (fdp.ConsumeBool()) {
        // Try default location first
        const char* magic_path = "/usr/share/misc/magic";
        if (access(magic_path, R_OK) != 0) {
            magic_path = "magic"; // Try local magic file
        }
        load_result = magic_load(magic, magic_path);
    } else {
        // Try magic_load_buffers with dummy data
        const size_t num_buffers = 2;
        void* buffers[num_buffers] = {nullptr};
        size_t sizes[num_buffers] = {0};
        
        // Create dummy buffers from remaining fuzzer input
        for (size_t i = 0; i < num_buffers && fdp.remaining_bytes() > 0; i++) {
            size_t buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            if (fdp.remaining_bytes() >= buf_size) {
                std::vector<uint8_t> magic_buffer = fdp.ConsumeBytes<uint8_t>(buf_size);
                buffers[i] = malloc(buf_size);
                if (buffers[i]) {
                    memcpy(buffers[i], magic_buffer.data(), buf_size);
                    sizes[i] = buf_size;
                }
            }
        }
        
        load_result = magic_load_buffers(magic, buffers, sizes, num_buffers);
        
        // Clean up dummy buffers
        for (size_t i = 0; i < num_buffers; i++) {
            if (buffers[i]) free(buffers[i]);
        }
    }
    // Don't fail if load fails - continue testing with whatever state we have

    // Choose testing strategy based on operation
    switch (operation) {
        case 0: {
            // Strategy 1: Test magic_file with various file scenarios
            if (!buffer.empty()) {
                // Create a regular file with fuzzer data
                int fd = open(temp_filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd >= 0) {
                    write(fd, buffer.data(), buffer.size());
                    close(fd);
                    
                    // Test magic_file on the created file
                    const char* result = magic_file(magic, temp_filename.c_str());
                    (void)result;
                    
                    // Clean up
                    unlink(temp_filename.c_str());
                }
            }
            break;
        }
        
        case 1: {
            // Strategy 2: Test magic_descriptor with various file descriptors
            if (!buffer.empty()) {
                // Create a file and get its descriptor
                int fd = open(temp_filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd >= 0) {
                    write(fd, buffer.data(), buffer.size());
                    close(fd);
                    
                    // Reopen for reading
                    fd = open(temp_filename.c_str(), O_RDONLY);
                    if (fd >= 0) {
                        // Test magic_descriptor on the file descriptor
                        const char* result = magic_descriptor(magic, fd);
                        (void)result;
                        
                        // Test with different seek positions
                        lseek(fd, 0, SEEK_SET);
                        result = magic_descriptor(magic, fd);
                        (void)result;
                        
                        close(fd);
                    }
                    
                    // Clean up
                    unlink(temp_filename.c_str());
                }
            }
            break;
        }
        
        case 2: {
            // Strategy 3: Test edge cases for magic_descriptor
            // Test with various file descriptors including special ones
            const int test_fds[] = {
                STDIN_FILENO,   // Standard input (might be pipe or terminal)
                STDOUT_FILENO,  // Standard output
                STDERR_FILENO,  // Standard error
                -1,             // Invalid descriptor
                9999,           // Non-existent descriptor
            };
            
            for (int test_fd : test_fds) {
                const char* result = magic_descriptor(magic, test_fd);
                (void)result;
            }
            
            // Also test with a pipe
            int pipe_fds[2];
            if (pipe(pipe_fds) == 0) {
                // Write some data to the pipe
                if (!buffer.empty() && buffer.size() > 0) {
                    write(pipe_fds[1], buffer.data(), std::min(buffer.size(), (size_t)1024));
                }
                
                const char* result = magic_descriptor(magic, pipe_fds[0]);
                (void)result;
                
                close(pipe_fds[0]);
                close(pipe_fds[1]);
            }
            break;
        }
        
        case 3: {
            // Strategy 4: Test magic_file with various path scenarios
            std::vector<std::string> test_paths;
            
            // Generate various test paths from fuzzer input
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                std::string path = fdp.ConsumeRandomLengthString(128);
                test_paths.push_back(path);
            }
            
            // Add some special paths
            test_paths.push_back("/dev/null");
            test_paths.push_back("/dev/zero");
            test_paths.push_back("/dev/random");
            test_paths.push_back("/proc/self/exe");  // Current executable
            test_paths.push_back("");                // Empty path
            test_paths.push_back(".");               // Current directory
            
            // Test all paths
            for (const auto& path : test_paths) {
                const char* result = magic_file(magic, path.c_str());
                (void)result;
            }
            
            // Also test with NULL pointer
            const char* result = magic_file(magic, nullptr);
            (void)result;
            break;
        }
        
        case 4: {
            // Strategy 5: Test combined file and descriptor operations
            if (!buffer.empty()) {
                // Create multiple files with different content
                for (int i = 0; i < 2; i++) {
                    std::string multi_filename = filename_template + "_multi_" + std::to_string(i);
                    
                    int fd = open(multi_filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if (fd >= 0) {
                        // Write different portions of buffer to each file
                        size_t write_size = std::min(buffer.size(), (size_t)(i * 100 + 100));
                        if (write_size > 0) {
                            write(fd, buffer.data(), write_size);
                        }
                        close(fd);
                        
                        // Test magic_file
                        const char* result = magic_file(magic, multi_filename.c_str());
                        (void)result;
                        
                        // Reopen and test magic_descriptor
                        fd = open(multi_filename.c_str(), O_RDONLY);
                        if (fd >= 0) {
                            result = magic_descriptor(magic, fd);
                            (void)result;
                            close(fd);
                        }
                        
                        // Clean up
                        unlink(multi_filename.c_str());
                    }
                }
            }
            break;
        }
        
        case 5: {
            // Strategy 6: Test with different flag combinations
            int flag_combinations[] = {
                MAGIC_NONE,
                MAGIC_MIME,
                MAGIC_SYMLINK,
                MAGIC_RAW,
                MAGIC_CONTINUE,
                MAGIC_MIME | MAGIC_SYMLINK,
                MAGIC_RAW | MAGIC_CONTINUE,
            };
            
            if (!buffer.empty()) {
                // Create a test file
                int fd = open(temp_filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd >= 0) {
                    write(fd, buffer.data(), buffer.size());
                    close(fd);
                    
                    // Test with each flag combination
                    for (int test_flags : flag_combinations) {
                        magic_setflags(magic, test_flags);
                        
                        // Test magic_file
                        const char* result = magic_file(magic, temp_filename.c_str());
                        (void)result;
                        
                        // Test magic_descriptor
                        fd = open(temp_filename.c_str(), O_RDONLY);
                        if (fd >= 0) {
                            result = magic_descriptor(magic, fd);
                            (void)result;
                            close(fd);
                        }
                    }
                    
                    // Clean up
                    unlink(temp_filename.c_str());
                }
            }
            break;
        }
    }

    // Test error retrieval
    const char* error_msg = magic_error(magic);
    (void)error_msg;
    
    // Clean up
    magic_close(magic);

    return 0;
}

} // namespace harness_002
