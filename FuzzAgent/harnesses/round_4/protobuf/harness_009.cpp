#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/coded_stream.h"

namespace harness_009 {
// Fuzzing harness for Google Protocol Buffers I/O stream backup and manipulation operations
// Targets uncovered APIs: CopyingInputStreamAdaptor::BackUp, FileInputStream::BackUp, 
// IstreamInputStream::BackUp, CopyingInputStreamAdaptor::Skip, FileInputStream::Close,
// FileInputStream::CopyingFileInputStream::Read, and FileInputStream::Next
// Based on coverage guidance analysis - focuses on stream backup/skip operations




using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::FileInputStream;
using google::protobuf::io::IstreamInputStream;
using google::protobuf::io::CopyingInputStream;
using google::protobuf::io::CopyingInputStreamAdaptor;
using google::protobuf::io::ArrayInputStream;
using google::protobuf::io::CodedInputStream;

// Custom CopyingInputStream implementation for testing
class TestCopyingInputStream : public CopyingInputStream {
public:
    explicit TestCopyingInputStream(const uint8_t* data, size_t size) 
        : data_(data), size_(size), position_(0) {}
    
    ~TestCopyingInputStream() override = default;
    
    int Read(void* buffer, int size) override {
        if (position_ >= size_) {
            return 0;  // EOF
        }
        
        int bytes_to_read = std::min(static_cast<int>(size_ - position_), size);
        if (bytes_to_read <= 0) {
            return 0;
        }
        
        memcpy(buffer, data_ + position_, bytes_to_read);
        position_ += bytes_to_read;
        return bytes_to_read;
    }
    
    int Skip(int count) override {
        if (count <= 0) {
            return 0;
        }
        
        int bytes_to_skip = std::min(static_cast<int>(size_ - position_), count);
        position_ += bytes_to_skip;
        return bytes_to_skip;
    }
    
    void Reset() {
        position_ = 0;
    }

private:
    const uint8_t* data_;
    size_t size_;
    size_t position_;
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration parameters
    uint8_t stream_type = fdp.ConsumeIntegral<uint8_t>() % 3;
    int block_size = fdp.ConsumeIntegralInRange<int>(1, 4096);  // Fix: block_size must be positive
    bool owns_copying_stream = fdp.ConsumeBool();
    bool close_on_delete = fdp.ConsumeBool();
    
    // Consume operation parameters
    int backup_amount = fdp.ConsumeIntegralInRange<int>(0, 1024);
    int skip_amount = fdp.ConsumeIntegralInRange<int>(0, 1024);
    int read_amount = fdp.ConsumeIntegralInRange<int>(1, 2048);
    
    // Consume test data for streams
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    if (test_data.empty()) {
        return 0;  // Not enough data for testing
    }
        switch (stream_type) {
            case 0: {
                // Test FileInputStream with temporary file
                char temp_filename[] = "/tmp/protobuf_fuzz_XXXXXX";
                int fd = mkstemp(temp_filename);
                if (fd < 0) {
                    return 0;
                }
                
                // Write test data to temp file
                ssize_t written = write(fd, test_data.data(), test_data.size());
                if (written != static_cast<ssize_t>(test_data.size())) {
                    close(fd);
                    unlink(temp_filename);
                    return 0;
                }
                
                if (lseek(fd, 0, SEEK_SET) != 0) {
                    close(fd);
                    unlink(temp_filename);
                    return 0;
                }
                
                // Create FileInputStream
                FileInputStream file_stream(fd, block_size);
                file_stream.SetCloseOnDelete(close_on_delete);
                
                // Test Next() operation
                const void* buffer;
                int buffer_size;
                bool has_next = file_stream.Next(&buffer, &buffer_size);
                
                if (has_next && buffer_size > 0) {
                    // Test BackUp() with various amounts
                    int actual_backup = std::min(backup_amount, buffer_size);
                    file_stream.BackUp(actual_backup);
                    
                    // Test Skip() operation
                    int actual_skip = std::min(skip_amount, buffer_size - actual_backup);
                    if (actual_skip > 0) {
                        file_stream.Skip(actual_skip);
                    }
                    
                    // Try to read more data
                    const void* buffer2;
                    int buffer_size2;
                    if (file_stream.Next(&buffer2, &buffer_size2)) {
                        // Backup again - but only after consuming backed-up bytes via Next()
                        if (buffer_size2 > 0) {
                            file_stream.BackUp(std::min(backup_amount / 2, buffer_size2));
                        }
                    }
                    
                    // Test Skip() again
                    file_stream.Skip(std::max(0, skip_amount / 2));
                } // End of if (has_next && buffer_size > 0)
                
                // Close handling based on close_on_delete flag
                if (!close_on_delete) {
                    // Only call Close() if we're not relying on destructor
                    file_stream.Close();
                }
                // Don't call close(fd) - FileInputStream owns the fd
                // Just unlink the temp file
                unlink(temp_filename);
                break;
            }
            
            case 1: {
                // Test IstreamInputStream with stringstream
                std::string test_string(reinterpret_cast<const char*>(test_data.data()), 
                                       test_data.size());
                std::istringstream input_stream(test_string);
                
                IstreamInputStream istream_stream(&input_stream, block_size);
                
                // Test Next() operation
                const void* buffer;
                int buffer_size;
                bool has_next = istream_stream.Next(&buffer, &buffer_size);
                
                if (has_next && buffer_size > 0) {
                    // Test BackUp() 
                    int actual_backup = std::min(backup_amount, buffer_size);
                    istream_stream.BackUp(actual_backup);
                    
                    // Test Skip() operation
                    int actual_skip = std::min(skip_amount, buffer_size - actual_backup);
                    if (actual_skip > 0) {
                        istream_stream.Skip(actual_skip);
                    }
                    
                    // Read more and backup again - but only after Next()
                    const void* buffer2;
                    int buffer_size2;
                    if (istream_stream.Next(&buffer2, &buffer_size2)) {
                        if (buffer_size2 > 0) {
                            istream_stream.BackUp(std::min(backup_amount / 2, buffer_size2));
                        }
                    }
                }
                
                // Test ByteCount()
                istream_stream.ByteCount();
                break;
            }
            
            case 2: {
                // Test CopyingInputStreamAdaptor with custom CopyingInputStream
                // Memory ownership handling: if owns_copying_stream is true, use heap allocation
                TestCopyingInputStream* copying_stream_ptr = nullptr;
                TestCopyingInputStream stack_stream(test_data.data(), test_data.size());
                
                if (owns_copying_stream) {
                    // Heap allocation when adaptor takes ownership
                    copying_stream_ptr = new TestCopyingInputStream(test_data.data(), test_data.size());
                } else {
                    // Stack allocation when adaptor doesn't take ownership
                    copying_stream_ptr = &stack_stream;
                }
                
                CopyingInputStreamAdaptor adaptor(copying_stream_ptr, block_size);
                adaptor.SetOwnsCopyingStream(owns_copying_stream);
                
                // Test Next() operation
                const void* buffer;
                int buffer_size;
                bool has_next = adaptor.Next(&buffer, &buffer_size);
                
                if (has_next && buffer_size > 0) {
                    // Test BackUp() 
                    int actual_backup = std::min(backup_amount, buffer_size);
                    adaptor.BackUp(actual_backup);
                    
                    // Test Skip() operation
                    int actual_skip = std::min(skip_amount, buffer_size - actual_backup);
                    if (actual_skip > 0) {
                        adaptor.Skip(actual_skip);
                    }
                    
                    // Read more data
                    const void* buffer2;
                    int buffer_size2;
                    if (adaptor.Next(&buffer2, &buffer_size2)) {
                        // Test backup - only one BackUp per Next() call
                        if (buffer_size2 > 0) {
                            adaptor.BackUp(std::min(backup_amount / 3, buffer_size2));
                            
                            // Can't call BackUp again without consuming backed-up bytes
                            // So we need to call Next() again before another BackUp
                            const void* buffer3;
                            int buffer_size3;
                            if (adaptor.Next(&buffer3, &buffer_size3)) {
                                if (buffer_size3 > 0) {
                                    adaptor.BackUp(std::min(backup_amount / 4, buffer_size3 / 2));
                                    
                                    // Skip after backup
                                    adaptor.Skip(std::max(0, skip_amount / 2));
                                }
                            }
                        }
                    }
                    
                    // Test Skip() with larger amount
                    adaptor.Skip(std::max(0, skip_amount * 2));
                }
                
                // Test ByteCount()
                adaptor.ByteCount();
                
                // Note: If owns_copying_stream is true, adaptor will delete copying_stream_ptr
                // If owns_copying_stream is false, copying_stream_ptr points to stack_stream which will be auto-destroyed
                break;
            }
        }
        
        // Additional test: ArrayInputStream for comparison
        ArrayInputStream array_stream(test_data.data(), test_data.size(), block_size);
        const void* array_buffer;
        int array_buffer_size;
        if (array_stream.Next(&array_buffer, &array_buffer_size)) {
            if (array_buffer_size > 0) {
                array_stream.BackUp(std::min(backup_amount, array_buffer_size));
                array_stream.Skip(std::min(skip_amount, array_buffer_size - std::min(backup_amount, array_buffer_size)));
            }
        }
        
        // Test CodedInputStream wrapper
        ArrayInputStream array_stream2(test_data.data(), test_data.size(), block_size);
        CodedInputStream coded_stream(&array_stream2);
        
        // Try reading various data types
        uint32_t tag = 0;
        if (coded_stream.ReadVarint32(&tag)) {
            // Successful read
        }
        
        // Skip some bytes
        coded_stream.Skip(std::max(0, skip_amount));
        
        // Try reading more
        uint64_t value = 0;
        coded_stream.ReadVarint64(&value);
    
    return 0;
}

} // namespace harness_009
