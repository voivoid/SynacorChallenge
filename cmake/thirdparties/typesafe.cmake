include(ExternalProject)

find_package(Git REQUIRED)

set(TypesafeVer master)

ExternalProject_Add(
  get_typesafe
  PREFIX thirdparties
  GIT_REPOSITORY https://github.com/foonathan/type_safe
  GIT_TAG ${TypesafeVer}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  UPDATE_COMMAND ""
  )

add_library(typesafe INTERFACE)

ExternalProject_Get_Property(get_typesafe SOURCE_DIR)
set_target_properties(typesafe PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)
set_target_properties(typesafe PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES ${SOURCE_DIR}/include)

add_dependencies(typesafe get_typesafe)



