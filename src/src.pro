CONFIG   += ordered
TEMPLATE  = subdirs
SUBDIRS   = thirdparty utils loader plugins tools definitions interfaces make

# TEMPORARY !!!
!android: CONFIG(debug, debug|release): SUBDIRS += qtlive
