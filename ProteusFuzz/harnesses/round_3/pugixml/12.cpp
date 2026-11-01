// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::set_memory_management_functions at pugixml.cpp:7867:37 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

// Custom memory management functions for fuzzing
static void* fuzz_allocate(size_t size) {
    if (size == 0) return nullptr;
    void* ptr = malloc(size);
    return ptr;
}

static void fuzz_deallocate(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

// Helper to safely parse input data into an xml_document
static pugi::xml_document parse_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    
    // Ensure the input is null-terminated for parsing
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    
    // Try to parse the XML string
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    // If parsing fails, try to create a minimal valid XML structure
    if (!result) {
        doc.append_child("root");
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Step 1: Prepare the environment
    pugi::xml_document doc = parse_input(Data, Size);
    
    // Get the root node
    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        root = doc.first_child();
    }
    
    // If no root, create one
    if (root.empty()) {
        root = doc.append_child("root");
    }
    
    // Step 2: Invoke the target functions in the specified order
    
    // 1. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();
    
    // 2. pugi::xml_node::next_sibling
    pugi::xml_node next = first.next_sibling();
    
    // 3. pugi::xml_node::value
    const pugi::char_t* value = first.value();
    (void)value; // Suppress unused variable warning
    
    // 4. pugi::xml_node::set_value
    // Try to set a value, but only if the node is not an element
    if (first.type() != pugi::node_element) {
        first.set_value("fuzzed_value");
    }
    
    // 5. pugi::xml_node::first_child (again)
    pugi::xml_node first_again = root.first_child();
    
    // 6. pugi::xml_node::next_sibling (again)
    pugi::xml_node next_again = first_again.next_sibling();
    
    // 7. pugi::xml_node::next_sibling (third time)
    pugi::xml_node next_third = next_again.next_sibling();
    
    // 8. pugi::xml_node::parent
    pugi::xml_node parent = next_third.parent();
    
    // 9. pugi::xml_node::remove_child
    // Try to remove a child if it exists
    // Note: is_null() was removed as it's not a member of xml_node in pugixml
    // Using empty() check instead
    if (!first_again.empty()) {
        parent.remove_child(first_again);
    }
    
    // 10. pugi::set_memory_management_functions
    // Set custom memory management functions
    pugi::set_memory_management_functions(fuzz_allocate, fuzz_deallocate);
    
    // Step 3: Cleanup
    // The xml_document will automatically clean up when it goes out of scope
    
    return 0;
}