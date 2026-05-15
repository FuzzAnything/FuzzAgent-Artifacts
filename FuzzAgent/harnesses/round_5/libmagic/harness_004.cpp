#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <algorithm>
#include "magic.h"

namespace harness_004 {
/*
 * Fuzzing harness for libmagic targeting file descriptor and file path-based detection
 * with focus on filesystem magic logic and format-specific detection modules
 * 
 * Primary Targets: magic_descriptor (4 undiscovered branches), magic_file (2 undiscovered branches)
 * Secondary Targets: file_fsmagic (4004+ undiscovered branches), ELF parsing, CDF format detection
 * 
 * Semantic differentiation from previous harnesses:
 * - harness_000: Basic buffer detection with random operations
 * - harness_001: Configuration management and parameter tuning  
 * - harness_002: Database operations (compile, check, list)
 * - harness_003: Basic file operations and filesystem objects
 * - harness_004: Deep filesystem magic logic with specific file formats (ELF, CDF, compressed)
 */



// Maximum size for file operations
#define MAX_FILE_SIZE 1024 * 1024  // 1MB

// ELF file signatures (64-bit ELF)
static const uint8_t elf64_header[] = {
    0x7f, 'E', 'L', 'F',  // ELF magic
    0x02,                 // 64-bit
    0x01,                 // Little endian
    0x01,                 // ELF version
    0x00,                 // OS ABI
    0x00,                 // ABI version
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Padding
    0x02, 0x00,           // ET_EXEC
    0x3e, 0x00,           // x86-64
    0x01, 0x00, 0x00, 0x00  // ELF version
};

// CDF file signature (Compound Document Format)
static const uint8_t cdf_header[] = {
    0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1
};

// Compressed file signatures
static const uint8_t gzip_header[] = {0x1f, 0x8b};
static const uint8_t zip_header[] = {0x50, 0x4b, 0x03, 0x04};

// Helper class for creating test files with specific formats
class FormatTestFile {
public:
    FormatTestFile(FuzzedDataProvider& fdp, uint8_t format_type) {
        char filename_buffer[] = "/tmp/libmagic_fmt_XXXXXX";
        fd = mkstemp(filename_buffer);
        if (fd >= 0) {
            strncpy(filename, filename_buffer, sizeof(filename) - 1);
            filename[sizeof(filename) - 1] = '\0';
            
            size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
            std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(data_size);
            
            // Write format-specific header followed by fuzzer data
            switch (format_type) {
                case 0:  // ELF format
                    write(fd, elf64_header, sizeof(elf64_header));
                    break;
                case 1:  // CDF format
                    write(fd, cdf_header, sizeof(cdf_header));
                    break;
                case 2:  // GZIP format
                    write(fd, gzip_header, sizeof(gzip_header));
                    break;
                case 3:  // ZIP format
                    write(fd, zip_header, sizeof(zip_header));
                    break;
                default: // Raw data only
                    break;
            }
            
            if (test_data.size() > 0) {
                write(fd, test_data.data(), test_data.size());
            }
            
            fsync(fd);
            lseek(fd, 0, SEEK_SET); // Rewind for reading
        }
    }
    
    ~FormatTestFile() {
        if (fd >= 0) {
            close(fd);
            unlink(filename);
        }
    }
    
    const char* get_filename() const { return fd >= 0 ? filename : nullptr; }
    int get_fd() const { return fd; }
    
private:
    char filename[64];
    int fd;
};

// Test special device files
static bool test_device_file(magic_t magic, const char* device_path, bool use_descriptor) {
    int fd = -1;
    bool result = false;
    
    if (use_descriptor) {
        fd = open(device_path, O_RDONLY);
        if (fd >= 0) {
            const char* desc_result = magic_descriptor(magic, fd);
            (void)desc_result;
            result = true;
        }
    } else {
        const char* file_result = magic_file(magic, device_path);
        (void)file_result;
        result = true;
    }
    
    // Check for errors
    const char* error_msg = magic_error(magic);
    (void)error_msg;
    
    if (fd >= 0) {
        close(fd);
    }
    
    return result;
}

// Test pipe/FIFO
static bool test_pipe(magic_t magic, FuzzedDataProvider& fdp) {
    char pipe_name[] = "/tmp/libmagic_pipe_XXXXXX";
    mktemp(pipe_name);
    
    if (mkfifo(pipe_name, 0644) != 0) {
        return false;
    }
    
    // Fork to test pipe (one end writes, other reads)
    pid_t pid = fork();
    if (pid == 0) {
        // Child process - write to pipe
        int write_fd = open(pipe_name, O_WRONLY);
        if (write_fd >= 0) {
            size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
            std::vector<uint8_t> pipe_data = fdp.ConsumeBytes<uint8_t>(data_size);
            write(write_fd, pipe_data.data(), pipe_data.size());
            close(write_fd);
        }
        _exit(0);
    } else if (pid > 0) {
        // Parent process - test pipe detection
        int read_fd = open(pipe_name, O_RDONLY);
        if (read_fd >= 0) {
            const char* desc_result = magic_descriptor(magic, read_fd);
            (void)desc_result;
            
            const char* error_msg = magic_error(magic);
            (void)error_msg;
            
            close(read_fd);
        }
        
        // Wait for child
        waitpid(pid, NULL, 0);
        
        // Clean up pipe
        unlink(pipe_name);
        return true;
    }
    
    unlink(pipe_name);
    return false;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for basic parameters and test data
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume flags from input, focusing on filesystem detection flags
    int flags = fdp.ConsumeIntegral<int>();
    
    // Prioritize flags important for filesystem detection
    flags &= (MAGIC_DEBUG | MAGIC_SYMLINK | MAGIC_COMPRESS | MAGIC_DEVICES |
              MAGIC_MIME_TYPE | MAGIC_CONTINUE | MAGIC_CHECK | MAGIC_PRESERVE_ATIME |
              MAGIC_RAW | MAGIC_ERROR | MAGIC_MIME_ENCODING | MAGIC_APPLE |
              MAGIC_EXTENSION | MAGIC_COMPRESS_TRANSP | MAGIC_NO_COMPRESS_FORK);
    
    // Ensure device flag is often set for device file testing
    if (fdp.ConsumeBool()) {
        flags |= MAGIC_DEVICES;
    }
    
    // Ensure symlink flag is often set for symlink testing
    if (fdp.ConsumeBool()) {
        flags |= MAGIC_SYMLINK;
    }
    
    // Step 1: magic_open() - Initialize magic context
    magic_t magic = magic_open(flags);
    if (magic == nullptr) {
        return 0;  // Failed to open magic handle
    }
    
    // Step 2: magic_load() - Load magic database
    int load_result = magic_load(magic, nullptr);
    if (load_result == -1) {
        // Try common magic database paths
        const char* magic_paths[] = {"/etc/magic", "/usr/share/misc/magic"};
        for (const char* path : magic_paths) {
            load_result = magic_load(magic, path);
            if (load_result != -1) {
                break;
            }
        }
    }
    
    if (load_result == -1) {
        magic_close(magic);
        return 0;
    }
    
    // Consume test scenario from input
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Step 3-4: Test different scenarios targeting filesystem magic logic
    switch (scenario) {
        case 0: {
            // Test specific file formats (ELF, CDF, compressed) with magic_file()
            uint8_t format_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            FormatTestFile fmt_file(fdp, format_type);
            
            if (fmt_file.get_filename() != nullptr) {
                const char* file_result = magic_file(magic, fmt_file.get_filename());
                (void)file_result;
                
                // Also test with descriptor
                const char* desc_result = magic_descriptor(magic, fmt_file.get_fd());
                (void)desc_result;
                
                // Compare results
                const char* error_msg = magic_error(magic);
                (void)error_msg;
            }
            break;
        }
        
        case 1: {
            // Test device files (critical for filesystem magic detection)
            uint8_t device_type = fdp.ConsumeIntegral<uint8_t>() % 4;
            bool use_descriptor = fdp.ConsumeBool();
            
            const char* device_paths[] = {
                "/dev/null",
                "/dev/zero", 
                "/dev/random",
                "/dev/urandom"
            };
            
            if (device_type < 4) {
                test_device_file(magic, device_paths[device_type], use_descriptor);
            }
            break;
        }
        
        case 2: {
            // Test pipes/FIFOs (filesystem objects)
            test_pipe(magic, fdp);
            break;
        }
        
        case 3: {
            // Test symlinks and hardlinks
            FormatTestFile orig_file(fdp, 0); // Raw format
            
            if (orig_file.get_filename() != nullptr) {
                char symlink_path[] = "/tmp/libmagic_symlink_XXXXXX";
                mktemp(symlink_path);
                
                if (symlink(orig_file.get_filename(), symlink_path) == 0) {
                    const char* symlink_result = magic_file(magic, symlink_path);
                    (void)symlink_result;
                    
                    unlink(symlink_path);
                }
                
                // Also test hardlink
                char hardlink_path[] = "/tmp/libmagic_hardlink_XXXXXX";
                mktemp(hardlink_path);
                
                if (link(orig_file.get_filename(), hardlink_path) == 0) {
                    const char* hardlink_result = magic_file(magic, hardlink_path);
                    (void)hardlink_result;
                    
                    unlink(hardlink_path);
                }
            }
            break;
        }
        
        case 4: {
            // Test empty files and very small files
            char empty_filename[] = "/tmp/libmagic_empty_XXXXXX";
            int empty_fd = mkstemp(empty_filename);
            
            if (empty_fd >= 0) {
                // Test empty file
                const char* empty_file_result = magic_file(magic, empty_filename);
                (void)empty_file_result;
                
                const char* empty_desc_result = magic_descriptor(magic, empty_fd);
                (void)empty_desc_result;
                
                close(empty_fd);
                unlink(empty_filename);
            }
            break;
        }
        
        case 5: {
            // Test large files (boundary conditions)
            size_t large_size = fdp.ConsumeIntegralInRange<size_t>(MAX_FILE_SIZE/2, MAX_FILE_SIZE);
            // Use fuzzer data for large file content
            std::vector<uint8_t> large_data = fdp.ConsumeBytes<uint8_t>(std::min(large_size, fdp.remaining_bytes()));
            if (large_data.empty()) {
                large_data.resize(1, 0); // Minimum content if no data available
            }
            
            char large_filename[] = "/tmp/libmagic_large_XXXXXX";
            int large_fd = mkstemp(large_filename);
            
            if (large_fd >= 0) {
                write(large_fd, large_data.data(), large_data.size());
                fsync(large_fd);
                lseek(large_fd, 0, SEEK_SET);
                
                const char* large_file_result = magic_file(magic, large_filename);
                (void)large_file_result;
                
                const char* large_desc_result = magic_descriptor(magic, large_fd);
                (void)large_desc_result;
                
                close(large_fd);
                unlink(large_filename);
            }
            break;
        }
        
        case 6: {
            // Test comparison between buffer, file, and descriptor on same data
            size_t compare_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
            std::vector<uint8_t> compare_data = fdp.ConsumeBytes<uint8_t>(compare_size);
            
            if (compare_data.size() > 0) {
                // Test magic_buffer
                const char* buffer_result = magic_buffer(magic, compare_data.data(), compare_data.size());
                (void)buffer_result;
                
                // Create temp file with same data
                char compare_filename[] = "/tmp/libmagic_compare_XXXXXX";
                int compare_fd = mkstemp(compare_filename);
                
                if (compare_fd >= 0) {
                    write(compare_fd, compare_data.data(), compare_data.size());
                    fsync(compare_fd);
                    lseek(compare_fd, 0, SEEK_SET);
                    
                    // Test magic_file
                    const char* file_result = magic_file(magic, compare_filename);
                    (void)file_result;
                    
                    // Test magic_descriptor
                    const char* desc_result = magic_descriptor(magic, compare_fd);
                    (void)desc_result;
                    
                    close(compare_fd);
                    unlink(compare_filename);
                }
            }
            break;
        }
        
        case 7: {
            // Test directory and special filesystem objects
            char dirname[] = "/tmp/libmagic_dir_XXXXXX";
            if (mkdtemp(dirname) != nullptr) {
                const char* dir_result = magic_file(magic, dirname);
                (void)dir_result;
                
                // Create a file inside directory
                char subfile_path[128];
                snprintf(subfile_path, sizeof(subfile_path), "%s/subfile", dirname);
                
                int sub_fd = open(subfile_path, O_CREAT | O_WRONLY, 0644);
                if (sub_fd >= 0) {
                    size_t sub_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                    std::vector<uint8_t> sub_data = fdp.ConsumeBytes<uint8_t>(sub_size);
                    
                    write(sub_fd, sub_data.data(), sub_data.size());
                    close(sub_fd);
                    
                    // Test the file inside directory
                    const char* subfile_result = magic_file(magic, subfile_path);
                    (void)subfile_result;
                    
                    unlink(subfile_path);
                }
                
                rmdir(dirname);
            }
            break;
        }
    }
    
    // Additional error checking
    const char* final_error = magic_error(magic);
    (void)final_error;
    
    int final_flags = magic_getflags(magic);
    (void)final_flags;
    
    // Test magic_version() if we have enough input
    if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
        int version = magic_version();
        (void)version;
    }
    
    // Step 5: magic_close() - Clean up
    magic_close(magic);
    
    return 0;
}


} // namespace harness_004
