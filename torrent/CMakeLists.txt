



########### next target ###############

set(kfile_torrent_PART_SRCS 
   bytetape.cpp 
   bint.cpp 
   bstring.cpp 
   blist.cpp 
   bdict.cpp 
   kfile_torrent.cpp )

kde4_add_plugin(kfile_torrent ${kfile_torrent_PART_SRCS})


target_link_libraries(kfile_torrent  ${KDE4_KIO_LIBS}  )

install(TARGETS kfile_torrent  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES kfile_torrent.desktop  DESTINATION ${SERVICES_INSTALL_DIR})




