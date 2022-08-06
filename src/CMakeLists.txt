include(GNUInstallDirs)

include_directories(./)

add_library(alpha4
  alpha4/common/cli.cpp
  alpha4/common/logger.cpp
  alpha4/common/linescanner.cpp
  alpha4/types/vector.cpp
  alpha4/types/matrix.cpp
)

add_library(alpha4c 
  alpha4c/common/linescanner.cpp
  alpha4c/common/stringbuilder.cpp
)

set_property(TARGET alpha4 PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET alpha4c PROPERTY POSITION_INDEPENDENT_CODE ON)

install(TARGETS
  alpha4
  alpha4c
)

foreach(subdir common types)
  install(
    DIRECTORY alpha4/${subdir}
    DESTINATION include/alpha4
    FILES_MATCHING PATTERN *.hpp
  )
endforeach()

foreach(subdir common types)
  install(
    DIRECTORY alpha4c/${subdir}
    DESTINATION include/alpha4
    FILES_MATCHING PATTERN *.h
  )
endforeach()