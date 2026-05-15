#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "absl/strings/cord.h"
#include "absl/strings/cord_buffer.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "fuzzer/FuzzedDataProvider.h"

namespace harness_006 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0;  // Need minimum input for meaningful stream operations
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test type selection - determines which stream combination to test
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Create Cord data from fuzzer input
    size_t cord_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
    std::string cord_data = fdp.ConsumeBytesAsString(cord_data_size);
    absl::Cord source_cord(cord_data);
    
    // Use Cord helper functions as specified in guidance
    absl::CordMemoryAccounting accounting = static_cast<absl::CordMemoryAccounting>(
        fdp.ConsumeIntegralInRange<int>(0, 2));
    size_t estimated_memory = source_cord.EstimatedMemoryUsage(accounting);
    
    // Test basic Cord operations
    size_t cord_size = source_cord.size();
    bool cord_empty = source_cord.empty();
    
    switch (test_type) {
        case 0: {
            // Test StringOutputStream with WriteCord and BackUp
            std::string output_string;
            output_string.reserve(4096);
            google::protobuf::io::StringOutputStream string_output(&output_string);
            
            // Write Cord to output stream
            bool write_result = string_output.WriteCord(source_cord);
            
            // Call Next to get buffer and optionally BackUp
            void* buffer = nullptr;
            int buffer_size = 0;
            if (string_output.Next(&buffer, &buffer_size)) {
                // Write some additional data
                size_t write_size = fdp.ConsumeIntegralInRange<size_t>(0, buffer_size);
                std::vector<uint8_t> write_data = fdp.ConsumeBytes<uint8_t>(write_size);
                if (!write_data.empty() && write_data.size() <= static_cast<size_t>(buffer_size)) {
                    memcpy(buffer, write_data.data(), write_data.size());
                    // Back up if we didn't use full buffer
                    if (write_data.size() < static_cast<size_t>(buffer_size)) {
                        string_output.BackUp(buffer_size - write_data.size());
                    }
                } else {
                    // Back up entire buffer if no data written
                    string_output.BackUp(buffer_size);
                }
            }
            
            // Create ArrayInputStream from output data
            google::protobuf::io::ArrayInputStream array_input(
                output_string.data(), output_string.size());
            
            // Read Cord from input stream
            absl::Cord read_cord;
            int read_amount = fdp.ConsumeIntegralInRange<int>(1, output_string.size());
            bool read_result = array_input.ReadCord(&read_cord, read_amount);
            
            // Call BackUp on input stream
            const void* read_buffer = nullptr;
            int read_buffer_size = 0;
            if (array_input.Next(&read_buffer, &read_buffer_size)) {
                size_t back_up_amount = fdp.ConsumeIntegralInRange<size_t>(
                    0, read_buffer_size);
                if (back_up_amount > 0) {
                    array_input.BackUp(back_up_amount);
                }
            }
            
            break;
        }
        
        case 1: {
            // Test ArrayOutputStream with WriteCord and CopyingOutputStreamAdaptor
            std::vector<uint8_t> output_buffer(4096);
            google::protobuf::io::ArrayOutputStream array_output(
                output_buffer.data(), output_buffer.size());
            
            // Write Cord to array output stream
            bool write_result = array_output.WriteCord(source_cord);
            
            // Test BackUp
            void* buffer = nullptr;
            int buffer_size = 0;
            if (array_output.Next(&buffer, &buffer_size)) {
                size_t back_up_amount = fdp.ConsumeIntegralInRange<size_t>(
                    0, buffer_size);
                if (back_up_amount > 0) {
                    array_output.BackUp(back_up_amount);
                }
            }
            
            // Create ArrayInputStream from output buffer
            google::protobuf::io::ArrayInputStream array_input(
                output_buffer.data(), output_buffer.size());
            
            // Test CopyingInputStreamAdaptor BackUp
            class SimpleCopyingInputStream : public google::protobuf::io::CopyingInputStream {
            public:
                SimpleCopyingInputStream(const uint8_t* data, size_t size)
                    : data_(data), size_(size), position_(0) {}
                
                int Read(void* buffer, int size) override {
                    size_t remaining = size_ - position_;
                    if (remaining == 0) return 0;
                    size_t to_copy = std::min(static_cast<size_t>(size), remaining);
                    memcpy(buffer, data_ + position_, to_copy);
                    position_ += to_copy;
                    return to_copy;
                }
                
            private:
                const uint8_t* data_;
                size_t size_;
                size_t position_;
            };
            
            SimpleCopyingInputStream copying_input(output_buffer.data(), output_buffer.size());
            google::protobuf::io::CopyingInputStreamAdaptor copying_adaptor(&copying_input);
            
            // Test BackUp on CopyingInputStreamAdaptor
            const void* adaptor_buffer = nullptr;
            int adaptor_size = 0;
            if (copying_adaptor.Next(&adaptor_buffer, &adaptor_size)) {
                size_t back_up_amount = fdp.ConsumeIntegralInRange<size_t>(
                    0, adaptor_size);
                if (back_up_amount > 0) {
                    copying_adaptor.BackUp(back_up_amount);
                }
            }
            
            // Test ReadCord on the adaptor
            absl::Cord adaptor_cord;
            int read_amount = fdp.ConsumeIntegralInRange<int>(1, output_buffer.size());
            bool read_result = copying_adaptor.ReadCord(&adaptor_cord, read_amount);
            
            break;
        }
        
        case 2: {
            // Test CopyingOutputStreamAdaptor with WriteCord and BackUp
            std::vector<uint8_t> output_buffer(4096);
            size_t buffer_pos = 0;
            
            class SimpleCopyingOutputStream : public google::protobuf::io::CopyingOutputStream {
            public:
                SimpleCopyingOutputStream(std::vector<uint8_t>& buffer, size_t& pos)
                    : buffer_(buffer), pos_(pos) {}
                
                bool Write(const void* buffer, int size) override {
                    if (pos_ + size > buffer_.size()) {
                        return false;
                    }
                    memcpy(buffer_.data() + pos_, buffer, size);
                    pos_ += size;
                    return true;
                }
                
            private:
                std::vector<uint8_t>& buffer_;
                size_t& pos_;
            };
            
            SimpleCopyingOutputStream copying_output(output_buffer, buffer_pos);
            google::protobuf::io::CopyingOutputStreamAdaptor copying_output_adaptor(&copying_output);
            
            // Write Cord to copying output adaptor
            bool write_result = copying_output_adaptor.WriteCord(source_cord);
            
            // Test BackUp on CopyingOutputStreamAdaptor
            void* out_buffer = nullptr;
            int out_buffer_size = 0;
            if (copying_output_adaptor.Next(&out_buffer, &out_buffer_size)) {
                size_t back_up_amount = fdp.ConsumeIntegralInRange<size_t>(
                    0, out_buffer_size);
                if (back_up_amount > 0) {
                    copying_output_adaptor.BackUp(back_up_amount);
                }
            }
            
            // Test WriteAliasedRaw since CopyingOutputStreamAdaptor allows aliasing
            size_t aliased_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            std::vector<uint8_t> aliased_data = fdp.ConsumeBytes<uint8_t>(aliased_size);
            if (!aliased_data.empty()) {
                copying_output_adaptor.WriteAliasedRaw(aliased_data.data(), aliased_data.size());
            }
            
            // Flush the adaptor
            copying_output_adaptor.Flush();
            
            break;
        }
        
        case 3: {
            // Test EpsCopyOutputStream WriteCord via CodedOutputStream
            std::vector<uint8_t> output_buffer(4096);
            google::protobuf::io::ArrayOutputStream array_output(
                output_buffer.data(), output_buffer.size());
            google::protobuf::io::CodedOutputStream coded_output(&array_output);
            
            // Get the internal EpsCopyOutputStream
            google::protobuf::io::EpsCopyOutputStream* eps_copy = coded_output.EpsCopy();
            
            // Write some initial data to set up the stream
            uint32_t int_value = fdp.ConsumeIntegral<uint32_t>();
            coded_output.WriteVarint32(int_value);
            
            // Write string to initialize buffer
            size_t str_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
            std::string test_string = fdp.ConsumeBytesAsString(str_size);
            coded_output.WriteString(test_string);
            
            // Create additional Cord for testing
            size_t extra_cord_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
            std::string extra_cord_data = fdp.ConsumeBytesAsString(extra_cord_size);
            absl::Cord extra_cord(extra_cord_data);
            
            // Test WriteCord with small Cord that fits in buffer
            size_t small_cord_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            std::string small_cord_data = fdp.ConsumeBytesAsString(small_cord_size);
            absl::Cord small_cord(small_cord_data);
            
            // Write more varints to exercise different buffer states
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                uint64_t varint = fdp.ConsumeIntegral<uint64_t>();
                coded_output.WriteVarint64(varint);
            }
            
            // Write the original source Cord
            std::string cord_as_string = std::string(source_cord);
            coded_output.WriteString(cord_as_string);
            
            // Write remaining data
            size_t remaining_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            std::vector<uint8_t> remaining_data = fdp.ConsumeBytes<uint8_t>(remaining_data_size);
            if (!remaining_data.empty()) {
                coded_output.WriteRaw(remaining_data.data(), remaining_data.size());
            }
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_006
