## Makefile.am for folder file meta info plugin

INCLUDES         = $(all_includes)

# these are the headers for your project
noinst_HEADERS   = kfile_torrent.h bbase.h bdict.h bytetape.h \
                   bstring.h bint.h blist.h

kde_module_LTLIBRARIES = kfile_torrent.la

kfile_torrent_la_SOURCES = bytetape.cpp bint.cpp bstring.cpp blist.cpp bdict.cpp kfile_torrent.cpp
kfile_torrent_la_LDFLAGS = $(all_libraries) -module $(KDE_PLUGIN)
kfile_torrent_la_LIBADD = $(LIB_KIO)

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

services_DATA = kfile_torrent.desktop
servicesdir = $(kde_servicesdir)

messages:
	$(XGETTEXT) *.cpp -o $(podir)/kfile_torrent.pot
