if (Steven_LIBRARIES AND Steven_INCLUDE_DIRS)
  set (Steven_FIND_QUIETLY TRUE)
endif (Steven_LIBRARIES AND Steven_INCLUDE_DIRS)

find_path(Steven_INCLUDE_DIRS NAMES Gmpz.h PATH_SUFFIXES CGAL)
find_library(Steven_LIBRARIES NAMES CGAL)

include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Steven DEFAULT_MSG
  Steven_LIBRARIES Steven_INCLUDE_DIRS)

mark_as_advanced(Steven_INCLUDE_DIRS Steven_LIBRARIES)
