INCLUDE_DIRECTORIES(.)

LINK_DIRECTORIES(${BUILD_DIR}/src)

SET(ExtraCalendar_SRCS
    ExtraCalendar.hpp
    ExtraCalendar.cpp
    )

ADD_LIBRARY(ExtraCalendar-static STATIC ${ExtraCalendar_SRCS})
# The library target "ExtraCalendar-static" has a default OUTPUT_NAME of "ExtraCalendar-static", so change it.
SET_TARGET_PROPERTIES(ExtraCalendar-static PROPERTIES OUTPUT_NAME "ExtraCalendar")
TARGET_LINK_LIBRARIES(ExtraCalendar-static Qt5::Core Qt5::Gui)
SET_TARGET_PROPERTIES(ExtraCalendar-static PROPERTIES COMPILE_FLAGS "-DQT_STATICPLUGIN")
ADD_DEPENDENCIES(AllStaticPlugins ExtraCalendar-static)

SET_TARGET_PROPERTIES(ExtraCalendar-static PROPERTIES FOLDER "plugins/ExtraCalendar")
