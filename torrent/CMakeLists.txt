include_directories(${STRIGI_STREAMS_INCLUDES} ${STRIGI_STREAMANALYZER_INCLUDES})

set(torrent_analyzer_SRCS
   bytestream.cpp
   bint.cpp
   bstring.cpp
   blist.cpp
   bdict.cpp
   torrent_analyzer_factory.cpp
   torrent_analyzer.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")
add_library(torrent_analyzer MODULE ${torrent_analyzer_SRCS})
target_link_libraries(torrent_analyzer ${STRIGI_STREAMANALYZER_LIBRARY}
    ${STRIGI_STREAMS_LIBRARY} ${QT_QTCORE_LIBRARY} ${KDE4_KDECORE_LIBRARY})
set_target_properties(torrent_analyzer PROPERTIES PREFIX strigita_)

install(TARGETS torrent_analyzer LIBRARY DESTINATION ${LIB_INSTALL_DIR}/strigi)


