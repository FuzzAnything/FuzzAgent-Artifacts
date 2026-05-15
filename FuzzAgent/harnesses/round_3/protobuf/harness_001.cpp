#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/json/json.h"
#include "google/protobuf/util/json_util.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "absl/strings/string_view.h"

namespace harness_001 {
// Protocol Buffers fuzzing harness - Text and JSON format parsing
// Targets: TextFormat::ParseFromString, JsonFormat parsing, various text/JSON serialization APIs
// Uses FuzzedDataProvider for structured input splitting


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 4) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create test messages
    google::protobuf::FileDescriptorProto file_desc_proto;
    
    // Test 1: TextFormat parsing
    // TextFormat parses human-readable text format
    std::string text_data = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
    if (!text_data.empty()) {
        // Try parsing with TextFormat::ParseFromString
        bool text_parse_success = google::protobuf::TextFormat::ParseFromString(
            text_data, &file_desc_proto);
        (void)text_parse_success;
        
        // Test TextFormat::Parser with custom options
        google::protobuf::TextFormat::Parser parser;
        // Configure parser options based on fuzzed data
        parser.AllowPartialMessage(fdp.ConsumeBool());
        parser.AllowUnknownField(fdp.ConsumeBool());
        parser.AllowUnknownExtension(fdp.ConsumeBool());
        parser.AllowCaseInsensitiveField(fdp.ConsumeBool());
        bool parser_success = parser.ParseFromString(text_data, &file_desc_proto);
        (void)parser_success;
        
        // If parsing succeeded, test TextFormat printing
        if (text_parse_success || parser_success) {
            std::string printed_text;
            bool print_success = google::protobuf::TextFormat::PrintToString(
                file_desc_proto, &printed_text);
            (void)print_success;
            
            // Test PrintFields
            google::protobuf::TextFormat::Printer printer;
            printer.SetSingleLineMode(fdp.ConsumeBool());
            printer.SetHideUnknownFields(fdp.ConsumeBool());
            
            std::string printed_with_printer;
            bool printer_success = printer.PrintToString(file_desc_proto, &printed_with_printer);
            (void)printer_success;
        }
    }
    
    // Test 2: JSON format parsing and serialization
    if (fdp.remaining_bytes() > 0) {
        std::string json_data = fdp.ConsumeRandomLengthString(fdp.remaining_bytes());
        if (!json_data.empty()) {
            // Create a new message for JSON testing
            google::protobuf::FileDescriptorProto json_proto;
            
            // Try parsing JSON using JsonFormat APIs
            google::protobuf::json::ParseOptions parse_options;
            parse_options.ignore_unknown_fields = fdp.ConsumeBool();
            parse_options.case_insensitive_enum_parsing = fdp.ConsumeBool();
            
            // Note: JsonToBinaryString is in json_util.h
            // We'll test both new json:: and old util:: namespaces
            
            // Test with new json::ParseFromString if available
            // First parse some binary data to have a valid message
            if (size > 20) {
                // Use first 100 bytes as binary proto input
                size_t binary_size = std::min(size, static_cast<size_t>(100));
                bool binary_parse = json_proto.ParseFromArray(data, static_cast<int>(binary_size));
                
                if (binary_parse) {
                    // Test JSON serialization
                    google::protobuf::json::PrintOptions print_options;
                    print_options.add_whitespace = fdp.ConsumeBool();
                    print_options.always_print_enums_as_ints = fdp.ConsumeBool();
                    print_options.preserve_proto_field_names = fdp.ConsumeBool();
                    
                    std::string json_output;
                    absl::Status json_status = google::protobuf::json::MessageToJsonString(
                        json_proto, &json_output, print_options);
                    (void)json_status;
                    
                    // Also test without options
                    std::string json_output_simple;
                    absl::Status json_status_simple = google::protobuf::json::MessageToJsonString(
                        json_proto, &json_output_simple);
                    (void)json_status_simple;
                }
            }
            
            // Test 3: ZeroCopyStream APIs for TextFormat
            if (fdp.remaining_bytes() > 0) {
                std::vector<uint8_t> stream_data = fdp.ConsumeBytes<uint8_t>(
                    std::min(fdp.remaining_bytes(), static_cast<size_t>(100)));
                
                if (!stream_data.empty()) {
                    // Create array input stream
                    google::protobuf::io::ArrayInputStream array_stream(
                        stream_data.data(), static_cast<int>(stream_data.size()));
                    
                    google::protobuf::FileDescriptorProto stream_proto;
                    
                    // Try parsing from stream
                    google::protobuf::io::ZeroCopyInputStream* zc_input = &array_stream;
                    bool stream_parse_success = google::protobuf::TextFormat::Parse(
                        zc_input, &stream_proto);
                    (void)stream_parse_success;
                    
                    // Test with parser
                    google::protobuf::TextFormat::Parser stream_parser;
                    stream_parser.AllowPartialMessage(true);
                    
                    google::protobuf::io::ArrayInputStream array_stream2(
                        stream_data.data(), static_cast<int>(stream_data.size()));
                    google::protobuf::io::ZeroCopyInputStream* zc_input2 = &array_stream2;
                    
                    bool parser_stream_success = stream_parser.Parse(zc_input2, &stream_proto);
                    (void)parser_stream_success;
                }
            }
        }
    }
    
    // Test 4: DebugString and ShortDebugString APIs
    // These convert messages to string representation
    std::string debug_string = file_desc_proto.DebugString();
    std::string short_debug_string = file_desc_proto.ShortDebugString();
    
    // Test Utf8Format and ShortFormat
    std::string utf8_format = google::protobuf::Utf8Format(file_desc_proto);
    std::string short_format = google::protobuf::ShortFormat(file_desc_proto);
    
    // Test 5: Reflection-based field manipulation with text format
    const google::protobuf::Descriptor* descriptor = file_desc_proto.GetDescriptor();
    const google::protobuf::Reflection* reflection = file_desc_proto.GetReflection();
    
    if (descriptor && reflection && fdp.remaining_bytes() > 0) {
        // Try to set some fields using text format
        for (int i = 0; i < std::min(3, descriptor->field_count()); i++) {
            const google::protobuf::FieldDescriptor* field = descriptor->field(i);
            if (field && !field->is_repeated()) {
                // Generate a field assignment in text format
                absl::string_view field_name_view = field->name();
                std::string field_name(field_name_view.data(), field_name_view.size());
                std::string field_value;
                
                switch (field->cpp_type()) {
                    case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                        field_value = std::to_string(fdp.ConsumeIntegral<int32_t>());
                        break;
                    case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                        field_value = std::to_string(fdp.ConsumeIntegral<int64_t>());
                        break;
                    case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                        field_value = std::to_string(fdp.ConsumeIntegral<uint32_t>());
                        break;
                    case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                        field_value = std::to_string(fdp.ConsumeIntegral<uint64_t>());
                        break;
                    case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                        field_value = std::to_string(fdp.ConsumeFloatingPoint<double>());
                        break;
                    case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                        field_value = std::to_string(fdp.ConsumeFloatingPoint<float>());
                        break;
                    case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                        field_value = fdp.ConsumeBool() ? "true" : "false";
                        break;
                    case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
                        const google::protobuf::EnumDescriptor* enum_desc = field->enum_type();
                        if (enum_desc && enum_desc->value_count() > 0) {
                            int enum_index = fdp.ConsumeIntegralInRange<int>(0, enum_desc->value_count() - 1);
                            field_value = enum_desc->value(enum_index)->name();
                        }
                        break;
                    }
                    case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
                        field_value = "\"" + fdp.ConsumeRandomLengthString(50) + "\"";
                        break;
                    case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
                        // Skip nested messages for simplicity
                        continue;
                }
                
                // Create text format assignment
                std::string text_assignment = field_name + ": " + field_value;
                
                // Try to parse this single field assignment
                google::protobuf::FileDescriptorProto field_proto;
                bool field_parse = google::protobuf::TextFormat::ParseFromString(
                    text_assignment, &field_proto);
                (void)field_parse;
            }
        }
    }
    
    // Test 6: MergeFrom with TextFormat
    if (fdp.remaining_bytes() > 0) {
        std::string merge_text = fdp.ConsumeRemainingBytesAsString();
        if (!merge_text.empty()) {
            google::protobuf::FileDescriptorProto merge_proto;
            
            // First parse some valid data
            if (size > 10) {
                merge_proto.ParseFromArray(data, static_cast<int>(std::min(size, static_cast<size_t>(50))));
            }
            
            // Try to merge text format data
            google::protobuf::TextFormat::Parser merge_parser;
            merge_parser.AllowPartialMessage(true);
            merge_parser.MergeFromString(merge_text, &merge_proto);
        }
    }
    
    return 0;
}

} // namespace harness_001
