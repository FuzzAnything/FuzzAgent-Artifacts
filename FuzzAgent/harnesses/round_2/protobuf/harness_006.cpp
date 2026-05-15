#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/gzip_stream.h"
#include "google/protobuf/io/coded_stream.h"

namespace harness_006 {
// Harness for Google Protocol Buffers file-based I/O operations with Gzip compression
// Targets file-based I/O with compression/decompression as identified by coverage analysis
// Semantic differentiation from previous harnesses:
// - harness_000: binary parsing and serialization
// - harness_001: text format parsing and printing
// - harness_002: Cord-based I/O streaming with compression (CordOutputStream/InputStream)
// - harness_003: SymbolChecker visibility rules validation
// - harness_004: JSON parsing/serialization for web/REST APIs
// - harness_005: Cord I/O operations with CodedInputStream/OutputStream and stream backup functions
// - harness_006: File-based I/O operations with Gzip compression (FileInputStream/FileOutputStream with Gzip wrappers)


// Include protobuf headers for file I/O and Gzip compression

using google::protobuf::io::FileInputStream;
using google::protobuf::io::FileOutputStream;
using google::protobuf::io::GzipInputStream;
using google::protobuf::io::GzipOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::CodedOutputStream;

// Helper function to create a temporary file with fuzzer data
static int create_temp_file(const std::vector<uint8_t>& data, std::string& temp_filename) {
    // Create a unique temporary filename
    char temp_template[] = "/tmp/protobuf_fuzz_XXXXXX";
    int fd = mkstemp(temp_template);
    if (fd < 0) {
        return -1;
    }
    
    temp_filename = temp_template;
    
    // Write fuzzer data to the file
    if (!data.empty()) {
        ssize_t written = write(fd, data.data(), data.size());
        if (written < 0 || static_cast<size_t>(written) != data.size()) {
            close(fd);
            unlink(temp_filename.c_str());
            return -1;
        }
    }
    
    // Seek back to beginning for reading
    lseek(fd, 0, SEEK_SET);
    return fd;
}

// Helper function to delete temporary file
static void delete_temp_file(const std::string& filename, int fd) {
    if (fd >= 0) {
        close(fd);
    }
    if (!filename.empty()) {
        unlink(filename.c_str());
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful testing
    if (size < 32) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which file I/O pattern to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Consume configuration options
    bool test_gzip = fdp.ConsumeBool();
    bool test_skip = fdp.ConsumeBool();
    bool test_flush = fdp.ConsumeBool();
    int block_size = fdp.ConsumeIntegralInRange<int>(-1, 65536);
    
    // Consume integral parameters for later use BEFORE consuming bulk data
    size_t skip_amount = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
    size_t read_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    size_t write_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    
    // Consume the remaining data for file content
    std::vector<uint8_t> file_content = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Create temporary input file with fuzzer data
    std::string input_filename;
    int input_fd = create_temp_file(file_content, input_filename);
    if (input_fd < 0) {
        return 0; // Failed to create temp file
    }
    
    // Create temporary output file
    std::string output_filename;
    int output_fd = -1;
    {
        char temp_template[] = "/tmp/protobuf_fuzz_out_XXXXXX";
        output_fd = mkstemp(temp_template);
        if (output_fd < 0) {
            delete_temp_file(input_filename, input_fd);
            return 0;
        }
        output_filename = temp_template;
    }
    
    int result = 0;
    
    try {
        switch (operation) {
            case 0: {
                // Test 1: FileInputStream basic operations with GzipInputStream
                FileInputStream file_input(input_fd, block_size);
                
                if (test_gzip) {
                    GzipInputStream gzip_input(&file_input);
                    
                    // Test Skip on GzipInputStream
                    if (test_skip && skip_amount > 0) {
                        bool skip_result = gzip_input.Skip(skip_amount);
                        // Don't check result - invalid input testing is important
                    }
                    
                    // Test reading from GzipInputStream
                    const void* read_buffer;
                    int read_buffer_size;
                    while (gzip_input.Next(&read_buffer, &read_buffer_size)) {
                        // Just consume the data
                        if (read_buffer_size > 0) {
                            // Could optionally process the data
                        }
                    }
                    
                    // Check error codes
                    int gzip_error = gzip_input.ZlibErrorCode();
                    int file_error = file_input.GetErrno();
                    (void)gzip_error;
                    (void)file_error;
                    
                    // ByteCount should be checked
                    int64_t bytes_read = gzip_input.ByteCount();
                    (void)bytes_read;
                    
                } else {
                    // Test FileInputStream directly
                    if (test_skip && skip_amount > 0) {
                        bool skip_result = file_input.Skip(skip_amount);
                    }
                    
                    const void* read_buffer;
                    int read_buffer_size;
                    while (file_input.Next(&read_buffer, &read_buffer_size)) {
                        // Consume data
                    }
                    
                    int file_error = file_input.GetErrno();
                    (void)file_error;
                    
                    int64_t bytes_read = file_input.ByteCount();
                    (void)bytes_read;
                }
                
                // Close should be tested
                bool close_result = file_input.Close();
                (void)close_result;
                
                break;
            }
            
            case 1: {
                // Test 2: FileOutputStream with GzipOutputStream
                FileOutputStream file_output(output_fd, block_size);
                
                if (test_gzip) {
                    GzipOutputStream gzip_output(&file_output);
                    
                    // Write some data to GzipOutputStream
                    void* write_buffer;
                    int write_buffer_size;
                    size_t bytes_to_write = std::min(write_size, file_content.size());
                    
                    if (bytes_to_write > 0 && gzip_output.Next(&write_buffer, &write_buffer_size)) {
                        int actual_write = std::min(write_buffer_size, static_cast<int>(bytes_to_write));
                        if (actual_write > 0) {
                            memcpy(write_buffer, file_content.data(), actual_write);
                            gzip_output.BackUp(write_buffer_size - actual_write);
                        }
                    }
                    
                    // Test Flush if requested
                    if (test_flush) {
                        bool flush_result = gzip_output.Flush();
                        (void)flush_result;
                    }
                    
                    // Check error codes
                    int gzip_error = gzip_output.ZlibErrorCode();
                    int file_error = file_output.GetErrno();
                    (void)gzip_error;
                    (void)file_error;
                    
                    // ByteCount should be checked
                    int64_t bytes_written = gzip_output.ByteCount();
                    (void)bytes_written;
                    
                    // Close GzipOutputStream
                    bool gzip_close = gzip_output.Close();
                    (void)gzip_close;
                    
                } else {
                    // Test FileOutputStream directly
                    void* write_buffer;
                    int write_buffer_size;
                    size_t bytes_to_write = std::min(write_size, file_content.size());
                    
                    if (bytes_to_write > 0 && file_output.Next(&write_buffer, &write_buffer_size)) {
                        int actual_write = std::min(write_buffer_size, static_cast<int>(bytes_to_write));
                        if (actual_write > 0) {
                            memcpy(write_buffer, file_content.data(), actual_write);
                            file_output.BackUp(write_buffer_size - actual_write);
                        }
                    }
                    
                    int file_error = file_output.GetErrno();
                    (void)file_error;
                    
                    int64_t bytes_written = file_output.ByteCount();
                    (void)bytes_written;
                }
                
                // Close FileOutputStream
                bool file_close = file_output.Close();
                (void)file_close;
                
                break;
            }
            
            case 2: {
                // Test 3: Round-trip compression/decompression
                // First write compressed data
                FileOutputStream file_output(output_fd, block_size);
                GzipOutputStream gzip_output(&file_output);
                
                void* write_buffer;
                int write_buffer_size;
                size_t bytes_to_write = std::min(write_size, file_content.size());
                
                if (bytes_to_write > 0 && gzip_output.Next(&write_buffer, &write_buffer_size)) {
                    int actual_write = std::min(write_buffer_size, static_cast<int>(bytes_to_write));
                        if (actual_write > 0) {
                            memcpy(write_buffer, file_content.data(), actual_write);
                            gzip_output.BackUp(write_buffer_size - actual_write);
                        }
                }
                
                // Flush and close output streams
                if (test_flush) {
                    gzip_output.Flush();
                }
                gzip_output.Close();
                file_output.Close();
                
                // Now reopen output file for reading and decompress
                lseek(output_fd, 0, SEEK_SET);
                FileInputStream file_input(output_fd, block_size);
                GzipInputStream gzip_input(&file_input);
                
                // Read decompressed data
                const void* read_buffer;
                int read_buffer_size;
                size_t total_read = 0;
                
                while (gzip_input.Next(&read_buffer, &read_buffer_size) && total_read < read_size) {
                    total_read += read_buffer_size;
                }
                
                // Check compression/decompression metrics
                int gzip_error = gzip_input.ZlibErrorCode();
                int file_error = file_input.GetErrno();
                (void)gzip_error;
                (void)file_error;
                
                int64_t compressed_bytes = file_input.ByteCount();
                int64_t decompressed_bytes = gzip_input.ByteCount();
                (void)compressed_bytes;
                (void)decompressed_bytes;
                
                // Close input streams
                file_input.Close();
                
                break;
            }
            
            case 3: {
                // Test 4: Direct file copy with backup operations
                FileInputStream file_input(input_fd, block_size);
                FileOutputStream file_output(output_fd, block_size);
                
                const void* read_buffer;
                int read_buffer_size;
                size_t total_read = 0;
                
                while (file_input.Next(&read_buffer, &read_buffer_size) && total_read < read_size) {
                    void* write_buffer;
                    int write_buffer_size;
                    
                    if (file_output.Next(&write_buffer, &write_buffer_size)) {
                        int bytes_to_copy = std::min(read_buffer_size, write_buffer_size);
                        bytes_to_copy = std::min(bytes_to_copy, static_cast<int>(read_size - total_read));
                        
                        if (bytes_to_copy > 0) {
                            memcpy(write_buffer, read_buffer, bytes_to_copy);
                            file_output.BackUp(write_buffer_size - bytes_to_copy);
                            total_read += bytes_to_copy;
                        }
                    }
                    
                    // Also test BackUp on input
                    if (read_buffer_size > 10) {
                        file_input.BackUp(5);
                    }
                }
                
                // Check error states
                int input_error = file_input.GetErrno();
                int output_error = file_output.GetErrno();
                (void)input_error;
                (void)output_error;
                
                // Close both streams
                bool input_close = file_input.Close();
                bool output_close = file_output.Close();
                (void)input_close;
                (void)output_close;
                
                break;
            }
            
            case 4: {
                // Test 5: Error handling scenarios
                // Test with invalid file descriptors
                FileInputStream bad_input(-1);
                FileOutputStream bad_output(-1);
                
                // These should fail and set errno
                const void* buf;
                int buf_size;
                bool next_result = bad_input.Next(&buf, &buf_size);
                (void)next_result;
                
                int input_errno = bad_input.GetErrno();
                (void)input_errno;
                
                void* out_buf;
                int out_size;
                bool out_result = bad_output.Next(&out_buf, &out_size);
                (void)out_result;
                
                int output_errno = bad_output.GetErrno();
                (void)output_errno;
                
                // Test Skip on bad stream
                bool skip_result = bad_input.Skip(10);
                (void)skip_result;
                
                // Test Close on bad streams
                bool close_input = bad_input.Close();
                bool close_output = bad_output.Close();
                (void)close_input;
                (void)close_output;
                
                // Now test with valid streams but trigger errors
                // Write to input fd (which is read-only from our perspective)
                if (file_content.size() > 0) {
                    write(input_fd, file_content.data(), std::min(file_content.size(), static_cast<size_t>(10)));
                }
                
                // Try to read from output fd (write-only)
                FileInputStream wrong_stream(output_fd, block_size);
                wrong_stream.Next(&buf, &buf_size);
                int wrong_errno = wrong_stream.GetErrno();
                (void)wrong_errno;
                wrong_stream.Close();
                
                break;
            }
        }
    } catch (...) {
        // Catch any exceptions to prevent crash
        result = 0;
    }
    
    // Clean up temporary files
    delete_temp_file(input_filename, input_fd);
    delete_temp_file(output_filename, output_fd);
    
    return result;
}

} // namespace harness_006
