// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xml_node::select_nodes at pugixml.cpp:13464:40 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include <exception>

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Limit input size to prevent excessive memory usage or stack overflow
    const size_t max_size = 1024 * 1024; 
    size_t safe_size = (Size > max_size) ? max_size : Size;
    
    // Create a null-terminated string from input
    std::string xml_content;
    xml_content.reserve(safe_size + 1);
    xml_content.append(reinterpret_cast<const char*>(Data), safe_size);
    xml_content.push_back('\0'); 

    // Load XML document
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    
    if (!result) {
        // Fallback to a simple valid XML structure if parsing fails
        doc.load_string("<root><child/></root>");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc; 
    }

    // 1. Test pugi::xml_object_range::empty
    pugi::xml_object_range<pugi::xml_node_iterator> children_range = root.children();
    pugi::xml_object_range<pugi::xml_node_iterator> range(children_range.begin(), children_range.end());
    bool range_empty = range.empty();
    (void)range_empty;

    pugi::xml_object_range<pugi::xml_node_iterator> empty_range(children_range.end(), children_range.end());
    bool empty_range_check = empty_range.empty();
    (void)empty_range_check;

    // 2, 3, 4, 5, 6. Test pugi::xpath_node_set methods
    pugi::xpath_node_set node_set;
    
    // Prepare a safe XPath query.
    // We create a string for the query, ensuring null termination.
    std::string query_str;
    query_str.reserve(safe_size + 1);
    query_str.append(reinterpret_cast<const char*>(Data), safe_size);
    query_str.push_back('\0'); 
    
    // Limit query length to avoid potential issues
    if (query_str.length() > 1000) {
        query_str.resize(1000);
    }

    // Try to compile the query, catching any exceptions thrown by invalid XPath syntax
    pugi::xpath_query query;
    try {
        query = pugi::xpath_query(query_str.c_str());
    } catch (...) {
        // If the input is not a valid XPath, use a default valid query
        query = pugi::xpath_query(".");
    }
    
    // Wrap the evaluation and subsequent operations in a try-catch block
    // to handle exceptions thrown during XPath evaluation (e.g., from evaluate_node_set_prepare)
    try {
        if (query) {
            node_set = doc.select_nodes(query);
        } else {
            // Fallback if query compilation failed (though try-catch should handle it)
            pugi::xpath_query default_query(".");
            if (default_query) {
                node_set = doc.select_nodes(default_query);
            }
        }

        // 6. Test pugi::xpath_node_set::empty
        bool is_empty = node_set.empty();
        (void)is_empty;

        // 2. Test pugi::xpath_node_set::size
        size_t set_size = node_set.size();
        (void)set_size;

        // 3. Test pugi::xpath_node_set::begin and 5. Test pugi::xpath_node_set::end
        for (auto it = node_set.begin(); it != node_set.end(); ++it) {
            pugi::xpath_node node = *it;
            if (node.node()) {
                const char* name = node.node().name();
                (void)name;
            }
        }

        // 4. Test pugi::xpath_node_set::sort
        node_set.sort(false);
        node_set.sort(true);

        // Test copy constructor
        pugi::xpath_node_set copied_set = node_set;
        (void)copied_set.size();
        (void)copied_set.empty();
    } catch (...) {
        // Silently catch any exceptions thrown during evaluation or manipulation
        // to prevent the fuzzer from crashing due to unhandled exceptions.
        return 0;
    }

    return 0;
}