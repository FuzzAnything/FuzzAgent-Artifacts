#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "google/protobuf/json/json.h"
#include "google/protobuf/util/json_util.h"
#include "google/protobuf/any.pb.h"
#include "google/protobuf/struct.pb.h"
#include "google/protobuf/timestamp.pb.h"
#include "google/protobuf/duration.pb.h"
#include "google/protobuf/wrappers.pb.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/util/type_resolver_util.h"

namespace harness_004 {
// Harness for Google Protocol Buffers JSON parsing module
// Targets JSON parsing and serialization operations as identified by coverage analysis
// Semantic differentiation from previous harnesses:
// - harness_000: binary parsing and serialization
// - harness_001: text format parsing and printing
// - harness_002: Cord-based I/O streaming with compression
// - harness_003: SymbolChecker visibility rules validation
// - harness_004: JSON parsing/serialization for web/REST APIs (completely uncovered module)


// Include protobuf headers for JSON parsing
using google::protobuf::json::JsonStringToMessage;
using google::protobuf::json::MessageToJsonString;
using google::protobuf::json::JsonStreamToMessage;
using google::protobuf::json::MessageToJsonStream;
using google::protobuf::json::ParseOptions;
using google::protobuf::json::PrintOptions;
using google::protobuf::Any;
using google::protobuf::Struct;
using google::protobuf::Timestamp;
using google::protobuf::Duration;
using google::protobuf::DoubleValue;
using google::protobuf::Int32Value;
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for meaningful JSON testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type to determine which JSON API to test
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume JSON parsing options
    ParseOptions parse_options;
    parse_options.allow_legacy_nonconformant_behavior = fdp.ConsumeBool();
    parse_options.ignore_unknown_fields = fdp.ConsumeBool();
    parse_options.case_insensitive_enum_parsing = fdp.ConsumeBool();
    
    // Consume JSON printing options
    PrintOptions print_options;
    print_options.allow_legacy_nonconformant_behavior = fdp.ConsumeBool();
    print_options.add_whitespace = fdp.ConsumeBool();
    print_options.always_print_fields_with_no_presence = fdp.ConsumeBool();
    print_options.always_print_enums_as_ints = fdp.ConsumeBool();
    print_options.preserve_proto_field_names = fdp.ConsumeBool();
    print_options.unquote_int64_if_possible = fdp.ConsumeBool();
    
    // Consume JSON data for parsing
    std::string json_data;
    if (fdp.remaining_bytes() > 0) {
        // Use remaining bytes as JSON input
        json_data = fdp.ConsumeRemainingBytesAsString();
    } else {
        return 0;
    }
    
    // Test different JSON operations
    switch (operation) {
        case 0: {
            // Test JsonStringToMessage with basic parsing
            Any message;
            absl::Status status = JsonStringToMessage(json_data, &message, parse_options);
            
            // Even if parsing fails, continue testing - invalid JSON testing is important
            // Try to serialize back if parsing succeeded
            if (status.ok()) {
                std::string serialized_json;
                absl::Status print_status = MessageToJsonString(message, &serialized_json, print_options);
                // Ignore print status for fuzzing purposes
            }
            break;
        }
        
        case 1: {
            // Test JsonStringToMessage with default options
            Struct message;
            absl::Status status = JsonStringToMessage(json_data, &message);
            
            if (status.ok()) {
                std::string serialized_json;
                absl::Status print_status = MessageToJsonString(message, &serialized_json);
            }
            break;
        }
        
        case 2: {
            // Test JsonStreamToMessage with ArrayInputStream
            Timestamp message;
            google::protobuf::io::ArrayInputStream input_stream(
                json_data.data(), static_cast<int>(json_data.size()));
            
            absl::Status status = JsonStreamToMessage(&input_stream, &message, parse_options);
            
            if (status.ok()) {
                // Test serialization to stream
                std::string output_buffer;
                google::protobuf::io::StringOutputStream output_stream(&output_buffer);
                absl::Status stream_status = MessageToJsonStream(message, &output_stream, print_options);
            }
            break;
        }
        
        case 3: {
            // Test round-trip: JSON -> Proto -> JSON
            Duration message1;
            absl::Status parse_status = JsonStringToMessage(json_data, &message1, parse_options);
            
            if (parse_status.ok()) {
                std::string round_trip_json;
                absl::Status print_status = MessageToJsonString(message1, &round_trip_json, print_options);
                
                // Try parsing the round-trip JSON again
                if (print_status.ok() && !round_trip_json.empty()) {
                    Duration message2;
                    absl::Status round_trip_status = JsonStringToMessage(round_trip_json, &message2, parse_options);
                }
            }
            break;
        }
        
        case 4: {
            // Test with binary input as JSON (edge case testing)
            DoubleValue message;
            // Create a modified JSON string that might include binary data
            std::string mixed_json = json_data;
            
            // Try parsing with different option combinations
            ParseOptions strict_options;
            strict_options.allow_legacy_nonconformant_behavior = false;
            strict_options.ignore_unknown_fields = false;
            strict_options.case_insensitive_enum_parsing = false;
            
            absl::Status strict_status = JsonStringToMessage(mixed_json, &message, strict_options);
            absl::Status lenient_status = JsonStringToMessage(mixed_json, &message, parse_options);
            
            // Test both parsing approaches
            break;
        }
        
        case 5: {
            // Test with TypeResolver for more complex scenarios
            google::protobuf::util::TypeResolver* resolver = google::protobuf::util::NewTypeResolverForDescriptorPool(
                "type.googleapis.com", google::protobuf::DescriptorPool::generated_pool());
            
            // Convert message to binary first
            Struct temp_message;
            std::string binary_data;
            
            // Try to parse JSON first
            absl::Status parse_status = JsonStringToMessage(json_data, &temp_message, parse_options);
            
            if (parse_status.ok()) {
                binary_data = temp_message.SerializeAsString();
                
                // Test BinaryToJsonString with TypeResolver
                std::string json_output;
                absl::Status binary_to_json_status = google::protobuf::json::BinaryToJsonString(
                    resolver,
                    "type.googleapis.com/google.protobuf.Struct",
                    binary_data, &json_output, print_options);
                
                // Test JsonToBinaryString with TypeResolver
                if (!json_data.empty()) {
                    std::string binary_output;
                    absl::Status json_to_binary_status = google::protobuf::json::JsonToBinaryString(
                        resolver,
                        "type.googleapis.com/google.protobuf.Struct",
                        json_data, &binary_output, parse_options);
                }
            }
            
            // Clean up resolver
            delete resolver;
            break;
        }
        case 6: {
            // Test error handling with malformed JSON
            Int32Value message;
            
            // Try parsing with empty string
            if (!json_data.empty()) {
                absl::Status empty_status = JsonStringToMessage("", &message, parse_options);
            }
            
            // Try parsing with invalid JSON fragments
            std::vector<std::string> test_fragments = {
                "{",
                "[",
                "null",
                "true",
                "false",
                "123",
                "\"string\"",
                "{}",
                "[]"
            };
            
            for (const auto& fragment : test_fragments) {
                absl::Status fragment_status = JsonStringToMessage(fragment, &message, parse_options);
            }
            
            // Test with JSON that has missing braces
            std::string malformed_json = json_data;
            if (malformed_json.size() > 10) {
                // Remove some characters to create malformed JSON
                malformed_json.erase(malformed_json.size() / 2, 5);
                absl::Status malformed_status = JsonStringToMessage(malformed_json, &message, parse_options);
            }
            break;
        }
        
        case 7: {
            // Test nested message parsing and complex JSON structures
            Struct message;
            
            // Create a nested JSON structure using fuzzer data
            std::string nested_json = "{";
            
            // Add some fields based on fuzzer input
            if (fdp.ConsumeBool() && json_data.size() > 10) {
                nested_json += "\"field1\": " + std::to_string(fdp.ConsumeIntegral<int32_t>()) + ",";
            }
            if (fdp.ConsumeBool() && json_data.size() > 20) {
                nested_json += "\"field2\": \"" + json_data.substr(0, std::min<size_t>(10, json_data.size())) + "\",";
            }
            if (fdp.ConsumeBool()) {
                nested_json += "\"nested\": {\"value\": " + std::to_string(fdp.ConsumeFloatingPoint<double>()) + "},";
            }
            
            // Remove trailing comma if present and close JSON
            if (nested_json.back() == ',') {
                nested_json.pop_back();
            }
            nested_json += "}";
            
            // Try parsing the generated JSON
            absl::Status nested_status = JsonStringToMessage(nested_json, &message, parse_options);
            
            if (nested_status.ok()) {
                // Test serialization with different print options
                PrintOptions compact_options;
                compact_options.add_whitespace = false;
                
                PrintOptions pretty_options;
                pretty_options.add_whitespace = true;
                
                std::string compact_json;
                std::string pretty_json;
                
                absl::Status compact_status = MessageToJsonString(message, &compact_json, compact_options);
                absl::Status pretty_status = MessageToJsonString(message, &pretty_json, pretty_options);
                
                // Try parsing the serialized JSON back
                if (compact_status.ok() && !compact_json.empty()) {
                    Struct round_trip_message;
                    absl::Status round_trip_status = JsonStringToMessage(compact_json, &round_trip_message, parse_options);
                }
            }
            
            // Also test with fuzzer-provided JSON
            absl::Status fuzzer_status = JsonStringToMessage(json_data, &message, parse_options);
            break;
        }
    }
    
    return 0;
}

} // namespace harness_004
