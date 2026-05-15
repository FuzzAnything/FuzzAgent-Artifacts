#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "re2/re2.h"

namespace harness_016 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check for meaningful fuzzing - need enough for patterns, inputs, and variables
  if (size < 256) return 0;
  
  FuzzedDataProvider fdp(data, size);
  
  // Strategy: Target RE2 argument parsing APIs with semantic differentiation from existing harnesses
  // Focus on RE2::Arg::Parse (completely uncovered) and high-complexity matching functions
  // that internally use argument parsing (FullMatchN, PartialMatchN, ConsumeN, FindAndConsumeN).
  // Create patterns with capture groups, prepare various input strings, declare variables
  // of different types, create RE2::Arg objects, and call all four matching functions.

  // Step 1: Generate regex patterns with capture groups
  std::vector<std::string> patterns;
  size_t num_patterns = fdp.ConsumeIntegralInRange<size_t>(1, 5);
  
  for (size_t i = 0; i < num_patterns && fdp.remaining_bytes() > 100; i++) {
    uint8_t pattern_type = fdp.ConsumeIntegral<uint8_t>() % 6;
    std::string pattern;
    
    switch (pattern_type) {
      case 0:
        // Pattern for integer parsing: "Number: 123" or similar
        pattern = "(\\w+):\\s*(\\d+)";
        break;
      case 1:
        // Pattern for multiple groups with different types
        pattern = "(\\w+)\\s+(\\d+)\\s+([A-Za-z]+)";
        break;
      case 2:
        // Pattern with optional groups
        pattern = "(\\w+)(?:\\s+(\\d+))?(?:\\s+([!@#$%^&*]+))?";
        break;
      case 3:
        // Pattern for hex/decimal/octal numbers
        pattern = "(0x[0-9a-fA-F]+|0[0-7]+|\\d+)";
        break;
      case 4:
        // Pattern with repetition and groups
        pattern = "((\\w+)-\\d+){1,3}";
        break;
      case 5:
        // Complex pattern with nested groups
        pattern = "((\\w+):(\\d+)(?:,(\\w+):(\\d+))*)";
        break;
    }
    
    patterns.push_back(pattern);
  }
  
  if (patterns.empty()) return 0;
  
  // Step 2: Generate input strings that match the patterns
  std::vector<std::string> test_inputs;
  size_t num_inputs = fdp.ConsumeIntegralInRange<size_t>(1, 10);
  
  for (size_t i = 0; i < num_inputs && fdp.remaining_bytes() > 50; i++) {
    uint8_t input_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    std::string input;
    
    switch (input_type) {
      case 0:
        // Simple integer match
        {
          std::string word = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 10));
          int number = fdp.ConsumeIntegralInRange<int>(0, 99999);
          input = word + ": " + std::to_string(number);
        }
        break;
      case 1:
        // Multiple groups
        {
          std::string word1 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 8));
          int number = fdp.ConsumeIntegralInRange<int>(0, 999);
          std::string word2 = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(3, 8));
          input = word1 + " " + std::to_string(number) + " " + word2;
        }
        break;
      case 2:
        // Hex/octal/decimal numbers
        {
          uint8_t num_type = fdp.ConsumeIntegral<uint8_t>() % 3;
          if (num_type == 0) {
            // Hex
            uint32_t hex_val = fdp.ConsumeIntegral<uint32_t>();
            char hex_str[20];
            snprintf(hex_str, sizeof(hex_str), "0x%x", hex_val);
            input = hex_str;
          } else if (num_type == 1) {
            // Octal
            uint32_t oct_val = fdp.ConsumeIntegral<uint32_t>() % 0777777;
            char oct_str[20];
            snprintf(oct_str, sizeof(oct_str), "0%o", oct_val);
            input = oct_str;
          } else {
            // Decimal
            input = std::to_string(fdp.ConsumeIntegralInRange<int>(0, 99999));
          }
        }
        break;
      case 3:
        // Pattern with repetitions
        {
          std::string base = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(2, 6));
          int count = fdp.ConsumeIntegralInRange<int>(1, 3);
          input = base + "-1";
          for (int j = 2; j <= count; j++) {
            input += "," + base + "-" + std::to_string(j);
          }
        }
        break;
    }
    
    test_inputs.push_back(input);
  }
  
  if (test_inputs.empty()) return 0;
  
  // Step 3: Test each pattern with various argument parsing scenarios
  for (const auto& pattern_str : patterns) {
    RE2 re(pattern_str);
    if (!re.ok()) continue;
    
    // Create different types of variables to store parsed values
    for (const auto& input_str : test_inputs) {
      absl::string_view text(input_str);
      
      // Test 1: FullMatchN with various argument types
      {
        // Declare variables of different types
        std::string str_var1, str_var2;
        int int_var1 = 0, int_var2 = 0;
        std::optional<int> opt_int_var;
        std::optional<std::string> opt_str_var;
        
        // Create RE2::Arg objects and store them
        RE2::Arg arg1(&str_var1);
        RE2::Arg arg2(&int_var1);
        RE2::Arg arg3(&str_var2);
        RE2::Arg arg4(&int_var2);
        RE2::Arg arg5(&opt_int_var);
        RE2::Arg arg6(&opt_str_var);
        
        // Create array of pointers to Arg objects
        const RE2::Arg* args[] = {
          &arg1, &arg2, &arg3, &arg4, &arg5, &arg6
        };
        
        int n = sizeof(args) / sizeof(args[0]);
        
        // Call FullMatchN - this will internally call RE2::Arg::Parse for each capture group
        bool match = RE2::FullMatchN(text, re, args, n);
      }
      
      // Test 2: PartialMatchN with different argument configurations
      {
        std::string partial_str;
        int partial_int = 0;
        long partial_long = 0;  // Use long instead of double
        
        RE2::Arg arg1(&partial_str);
        RE2::Arg arg2(&partial_int);
        RE2::Arg arg3(&partial_long);
        
        const RE2::Arg* args[] = {
          &arg1, &arg2, nullptr, &arg3  // nullptr to skip a capture group
        };
        
        int n = sizeof(args) / sizeof(args[0]);
        
        bool match = RE2::PartialMatchN(text, re, args, n);
      }
      
      // Test 3: ConsumeN - modifies the input string_view
      {
        absl::string_view consume_text = text;  // Copy for consumption
        std::string consume_str;
        int consume_int = 0;
        
        RE2::Arg arg1(&consume_str);
        RE2::Arg arg2(&consume_int);
        
        const RE2::Arg* args[] = {
          &arg1, &arg2
        };
        
        int n = sizeof(args) / sizeof(args[0]);
        
        bool consumed = RE2::ConsumeN(&consume_text, re, args, n);
      }
      
      // Test 4: FindAndConsumeN - finds and consumes all matches
      {
        absl::string_view find_text = text;  // Copy for find-and-consume
        std::string find_str;
        int find_int = 0;
        
        RE2::Arg arg1(&find_str);
        RE2::Arg arg2(&find_int);
        
        const RE2::Arg* args[] = {
          &arg1, &arg2
        };
        
        int n = sizeof(args) / sizeof(args[0]);
        
        // Try multiple times to find all occurrences
        while (RE2::FindAndConsumeN(&find_text, re, args, n)) {
          // Continue consuming until no more matches
        }
      }
      
      // Test 5: Specialized Arg constructors (CRadix, Hex, Octal)
      {
        absl::string_view radix_text = text;
        int cradix_int = 0;
        int hex_int = 0;
        int octal_int = 0;
        
        RE2::Arg arg1 = RE2::CRadix(&cradix_int);  // Auto-detect radix
        RE2::Arg arg2 = RE2::Hex(&hex_int);        // Hexadecimal
        RE2::Arg arg3 = RE2::Octal(&octal_int);    // Octal
        
        const RE2::Arg* args[] = {
          &arg1, &arg2, &arg3
        };
        
        int n = sizeof(args) / sizeof(args[0]);
        
        RE2::PartialMatchN(radix_text, re, args, n);
      }
      
      // Test 6: Mixed argument types including optional
      {
        absl::string_view mixed_text = text;
        std::string mixed_str1, mixed_str2;
        std::optional<int> mixed_opt1, mixed_opt2;
        int mixed_int1 = 0, mixed_int2 = 0;
        
        RE2::Arg arg1(&mixed_str1);
        RE2::Arg arg2(&mixed_opt1);
        RE2::Arg arg3(&mixed_int1);
        RE2::Arg arg4(&mixed_str2);
        RE2::Arg arg5(&mixed_opt2);
        RE2::Arg arg6(&mixed_int2);
        
        const RE2::Arg* args[] = {
          &arg1, &arg2, &arg3, &arg4, &arg5, &arg6
        };
        
        int n = sizeof(args) / sizeof(args[0]);
        
        RE2::FullMatchN(mixed_text, re, args, n);
      }
      
      // Test 7: Use the high-level API (which internally uses FullMatchN)
      // This tests the same code paths but through the public interface
      {
        absl::string_view api_text = text;
        std::string api_str;
        int api_int = 0;
        std::optional<int> api_opt;
        
        RE2::FullMatch(api_text, re, &api_str, &api_int, &api_opt);
        RE2::PartialMatch(api_text, re, &api_str, &api_int);
        
        absl::string_view consume_api = text;
        RE2::Consume(&consume_api, re, &api_str, &api_int);
        
        absl::string_view find_api = text;
        RE2::FindAndConsume(&find_api, re, &api_str, &api_int);
      }
    }
  }
  
  return 0;
}

} // namespace harness_016
