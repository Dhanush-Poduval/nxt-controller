cmake_minimum_required(VERSION 3.8)
project(nxt_hardware)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(controller_interface REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
add_library(
  nxt_system SHARED src/nxt_system.cpp nxt/nxt.c nxt/bluetooth.c
)
target_include_directories(nxt_system PUBLIC include ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(nxt_system ${LIBUSB_LIBRARIES})
ament_target_dependencies(nxt_system
  rclcpp
  rclcpp_lifecycle
  hardware_interface
  pluginlib
)
pluginlib_export_plugin_description_file(hardware_interface plugin.xml)

install(TARGETS nxt_system
  LIBRARY DESTINATION lib

)
install(
  DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)
install(DIRECTORY
  models
  DESTINATION share/${PROJECT_NAME}
)
install(DIRECTORY include/
  DESTINATION include
)
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)
install(FILES plugin.xml
  DESTINATION share/${PROJECT_NAME}
)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
