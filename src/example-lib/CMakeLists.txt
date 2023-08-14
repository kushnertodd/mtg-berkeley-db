add_library(example-lib STATIC
        example_keys.cpp
        example_keys.hpp
        inventory.cpp
        inventory.hpp
        vendor.cpp
        vendor.hpp
        )
target_include_directories(example-lib PUBLIC .)
