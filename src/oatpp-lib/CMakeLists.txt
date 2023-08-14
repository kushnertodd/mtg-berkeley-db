add_library(oatpp-lib STATIC
        imdb_component.hpp
        imdb_controller.cpp
        imdb_controller.hpp
        DTOs.hpp
        imdb-dto.hpp
        )
target_include_directories(oatpp-lib PUBLIC .)




