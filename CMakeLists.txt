cmake_minimum_required(VERSION 2.6)
project(kells)

find_package(Qt5 COMPONENTS Core Gui Widgets)
find_package(Boost COMPONENTS filesystem system)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}) 
include_directories(${Qt5Widgets_INCLUDES} ${Qt5Core_INCLUDES} ${Qt5Gui_INCLUDES} ${Qt5Svg_INCLUDES})

add_definitions(${Qt5Widgets_DEFINITIONS})

add_executable(search.cgi configparser.cpp search.cpp format.cpp renderer.cpp metadatatable.cpp publisher.cpp publishertable.cpp serietable.cpp serie.cpp tagtable.cpp metadata.cpp tag.cpp author.cpp authortable.cpp book.cpp utils.cpp data.cpp booktable.cpp table.cpp config.cpp)
add_executable(oook configdialog.cpp configparser.cpp renderer.cpp utils.cpp config.cpp installer.cpp filesystemutils.cpp booktable.cpp table.cpp data.cpp book.cpp tag.cpp author.cpp metadata.cpp serie.cpp publisher.cpp format.cpp)

target_link_libraries(oook ${Boost_LIBRARIES} Qt5::Core Qt5::Gui Qt5::Widgets)

install(TARGETS search.cgi oook RUNTIME DESTINATION bin)