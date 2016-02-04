TEMPLATE = subdirs

include(pkgtest.inc)
########### List of plugins that SHOULD NOT be disabled ############
SUBDIRS += accountmanager
SUBDIRS += chatmessagehandler
SUBDIRS += connectionmanager
SUBDIRS += defaultconnection
SUBDIRS += mainwindow
SUBDIRS += messageprocessor
SUBDIRS += messagewidgets
SUBDIRS += messagestyles
SUBDIRS += normalmessagehandler
SUBDIRS += notifications
SUBDIRS += optionsmanager
SUBDIRS += roster
SUBDIRS += presence
SUBDIRS += rosterchanger
SUBDIRS += rostersmodel
SUBDIRS += rostersview
SUBDIRS += saslauth
SUBDIRS += simplemessagestyle
SUBDIRS += stanzaprocessor
SUBDIRS += starttls
SUBDIRS += statuschanger
SUBDIRS += statusicons
!symbian: SUBDIRS += traymanager
SUBDIRS += xmppstreams
#############################################
SUBDIRS += iqauth
SUBDIRS += privatestorage
SUBDIRS += compress
SUBDIRS += emoticons
SUBDIRS += clientinfo
SUBDIRS += vcard
SUBDIRS += multiuserchat
SUBDIRS += dataforms
SUBDIRS += servicediscovery
SUBDIRS += bookmarks
SUBDIRS += registration
SUBDIRS += commands
SUBDIRS += jabbersearch
SUBDIRS += gateways
SUBDIRS += messagearchiver
SUBDIRS += privacylists
SUBDIRS += sessionnegotiation
SUBDIRS += avatars
SUBDIRS += autostatus
SUBDIRS += rostersearch
SUBDIRS += console
SUBDIRS += annotations
SUBDIRS += chatstates
SUBDIRS += datastreamsmanager
SUBDIRS += filestreamsmanager
SUBDIRS += filetransfer
SUBDIRS += inbandstreams
SUBDIRS += socksstreams
SUBDIRS += captchaforms
SUBDIRS += bitsofbinary
SUBDIRS += xmppuriqueries
SUBDIRS += remotecontrol
SUBDIRS += pepmanager
!android: SUBDIRS += shortcutmanager
SUBDIRS += birthdayreminder
SUBDIRS += urlprocessor
SUBDIRS += filemessagearchive
SUBDIRS += servermessagearchive
SUBDIRS += rosteritemexchange
SUBDIRS += spellchecker
SUBDIRS += messagecarbons
SUBDIRS += recentcontacts
SUBDIRS += metacontacts
SUBDIRS += statistics
contains(QT_CONFIG, webkit): SUBDIRS += adiummessagestyle
SUBDIRS += activity
SUBDIRS += mood
SUBDIRS += tune
SUBDIRS += nickname
windows: SUBDIRS += tunelistenerwinamp
windows: SUBDIRS += tunelisteneraimp
macx: SUBDIRS += tunelisteneritunes
unix:!android: SUBDIRS += tunelistenermpris
os2: SUBDIRS += tunelistenerpm123
os2: SUBDIRS += tunelistenerquplayer
os2: SUBDIRS += tunelistenerz
SUBDIRS += tunelistenerfile
SUBDIRS += tuneinforequesterlastfm
SUBDIRS += map
SUBDIRS += mapsearch
SUBDIRS += mapsearchprovidergoogle
contains(QT_CONFIG, script): SUBDIRS += mapsearchprovidernavitel
SUBDIRS += mapsearchproviderosm
SUBDIRS += mapsearchprovider2gis
SUBDIRS += mapsearchprovideryandex
SUBDIRS += mapsearchproviderhere
SUBDIRS += mapsource2gis
SUBDIRS += mapsourcebing
SUBDIRS += mapsourceesri
SUBDIRS += mapsourcerosreestr
SUBDIRS += mapsourcerumap
SUBDIRS += mapsourcegoogle
SUBDIRS += mapsourcekosmosnimki
SUBDIRS += mapsourcemegafon
SUBDIRS += mapsourcenavitel
SUBDIRS += mapsourcenavteq
SUBDIRS += mapsourceosm
SUBDIRS += mapsourceovi
SUBDIRS += mapsourceprogorod
SUBDIRS += mapsourcevitel
SUBDIRS += mapsourcewiki
SUBDIRS += mapsourceyahoo
SUBDIRS += mapsourceyandex
SUBDIRS += receipts
SUBDIRS += attention
CONFIG(debug, debug|release): SUBDIRS += eyecubot
SUBDIRS += positioning
SUBDIRS += positioningmethodmanual
!android: contains(QT_CONFIG, serialport): SUBDIRS += positioningmethodserialport
contains(QT_CONFIG, mobility): CONFIG(debug, debug|release): SUBDIRS += positioningmethodlocation
!android: SUBDIRS += positioningmethodip
!android: SUBDIRS += positioningmethodipproviderfreegeoip
SUBDIRS += poi
SUBDIRS += geoloc
CONFIG(debug, debug|release): SUBDIRS += gcompass
CONFIG(debug, debug|release): SUBDIRS += tracker
SUBDIRS += oob
SUBDIRS += mapcontacts
SUBDIRS += contactproximitynotification
SUBDIRS += mapmessage
SUBDIRS += maplocationselector
SUBDIRS += xhtmlim
SUBDIRS += mapmagnifier
SUBDIRS += boburlhandler
SUBDIRS += clienticons
SUBDIRS += emoji
SUBDIRS += abbreviations
!android {
    contains(QT_CONFIG, multimedia)|contains(QT_CONFIG, mobility):exists($(FFMPEGDIR)): SUBDIRS += mmplayer
    contains(QT_CONFIG, multimedia)|contains(QT_CONFIG, mobility):exists($(FFMPEGDIR)): CONFIG(debug, debug|release): SUBDIRS += jingle
    contains(QT_CONFIG, multimedia)|contains(QT_CONFIG, mobility):exists($(FFMPEGDIR)): CONFIG(debug, debug|release): SUBDIRS += jinglertp
    contains(QT_CONFIG, multimedia)|contains(QT_CONFIG, mobility):exists($(FFMPEGDIR)): CONFIG(debug, debug|release): SUBDIRS += jingletransporticeudp
    contains(QT_CONFIG, multimedia)|contains(QT_CONFIG, mobility):exists($(FFMPEGDIR)): CONFIG(debug, debug|release): SUBDIRS += jingletransportrawudp
}
SUBDIRS += streetview
SUBDIRS += streetviewprovidergoogle
contains(QT_CONFIG, webkit) {
    SUBDIRS += placeview
    SUBDIRS += placeviewprovidergoogle
}
CONFIG(debug, debug|release): SUBDIRS += weather
CONFIG(debug, debug|release): SUBDIRS += weatherprovideropenweather
SUBDIRS += wizardaccount
SUBDIRS += wizardtransport
