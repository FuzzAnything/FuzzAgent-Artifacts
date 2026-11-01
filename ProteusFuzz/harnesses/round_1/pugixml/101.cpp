// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_variable_set::add at pugixml.cpp:13149:51 in pugixml.hpp
// pugi::xpath_variable::type at pugixml.cpp:12945:48 in pugixml.hpp
// pugi::xpath_variable::name at pugixml.cpp:12923:45 in pugixml.hpp
// pugi::xpath_variable::get_string at pugixml.cpp:12960:45 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13204:57 in pugixml.hpp
// pugi::xpath_variable_set::get at pugixml.cpp:13199:51 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
// pugi::xpath_query::return_type at pugixml.cpp:13282:45 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>

extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert input data to a C-string, ensuring null termination
    static char buffer[4096];
    size_t copySize = (Size < sizeof(buffer) - 1) ? Size : sizeof(buffer) - 1;
    std::memcpy(buffer, Data, copySize);
    buffer[copySize] = '\0';

    // 1. Test xpath_variable_set::add and xpath_variable_set::get
    pugi::xpath_variable_set variables;
    
    // Add variables of different types based on input data to explore states
    // We use the first byte to determine the type to add
    pugi::xpath_value_type typeToTest = static_cast<pugi::xpath_value_type>(Data[0] % 5);
    
    // Ensure we have a valid type name for the add function
    const char* varName = "test_var";
    if (Size > 10) {
        varName = buffer; // Use fuzzed data as name if long enough
    }

    pugi::xpath_variable* var = variables.add(varName, typeToTest);
    
    if (var) {
        // 2. Test xpath_variable::type
        pugi::xpath_value_type varType = var->type();
        
        // 3. Test xpath_variable::name
        const pugi::char_t* varNameResult = var->name();
        
        // 4. Test xpath_variable::get_string
        // This is safe to call regardless of type, returns empty string if not string type
        const pugi::char_t* strVal = var->get_string();
        
        // 5. Test xpath_variable_set::get (const version)
        // Avoid copying the variable set to prevent assertion failures in debug builds
        // when internal string data is null or invalid. Instead, use a const reference.
        const pugi::xpath_variable_set& variables_const_ref = variables;
        const pugi::xpath_variable* var_const = variables_const_ref.get(varName);
        
        // 6. Test xpath_variable_set::get (non-const version)
        pugi::xpath_variable* var_non_const = variables.get(varName);
    }

    // 7. Test xpath_query::return_type
    pugi::xpath_query query;
    // Default constructed query is invalid, return_type should be xpath_type_none
    pugi::xpath_value_type queryType = query.return_type();
    
    // Try to compile a query from the fuzzed input to test valid/invalid states
    // We construct a query from the buffer. If it fails, return_type will be none.
    // Note: We don't evaluate it, just check the type.
    // Wrap in try-catch to handle exceptions thrown by invalid XPath syntax
    try {
        pugi::xpath_query queryFuzzed(buffer);
        pugi::xpath_value_type queryFuzzedType = queryFuzzed.return_type();
    } catch (...) {
        // Ignore exceptions from invalid XPath queries
    }

    // Cleanup is implicit as objects are on stack and destructors handle memory
    
    return 0;
}