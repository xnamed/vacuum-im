#ifndef DEF_TOOLBARGROUPS_H
#define DEF_TOOLBARGROUPS_H

//MainWindow - TopToolBar
#define TBG_MWTTB_DISCOVERY                                     200
#define TBG_MWTTB_MULTIUSERCHAT                                 500
#define TBG_MWTTB_BOOKMARKS                                     700
#define TBG_MWTTB_ROSTERSEARCH                                  900
#define TBG_MWTTB_ROSTERSVIEW                                   10200
#define TBG_MWTTB_NOTIFICATIONS_SOUND                           10500
// *** <<< eyeCU <<< ***
#define TBG_MWTTB_MAINMENU                                      100
#define TBG_MWTTB_TITLE                                         2000
#define TBG_MWTTB_ROSTERSEARCH_EDITOR                           2100
#define TBG_MWTTB_ROSTERSEARCH_ANDROID_ACT                      10100
#define TBG_MWTTB_RIGHTMENU                                     15000

#ifdef EYECU_MOBILE
#define TBG_MWTTB_MAPS                                          14000
#else
#define TBG_MWTTB_MAPS                                          1100
#endif

#define TBG_MWTTB_POI                                           1700
#define TBG_MWTTB_MMPLAYER                                      1800

//MainWindow - BottomToolBar
#define TBG_MWBTB_ROSTER                                        12000
#define TBG_MWBTB_MAP                                           13000
#define TBG_MWBTB_CHAT                                          14000
#define TBG_MWBTB_OPTIONS                                       15000
// *** >>> eyeCU >>> ***

//MessageWidgets - ToolBarWidget
#define TBG_MWTBW_EMOTICONS                                     200
#define TBG_MWTBW_MESSAGEWIDGETS_QUOTE                          300
#define TBG_MWTBW_FILETRANSFER                                  500
#define TBG_MWTBW_ARCHIVE_VIEW                                  700
#define TBG_MWTBW_VCARD_VIEW                                    900
// *** <<< eyeCU <<< ***
#define TBG_MWTBW_EMOJI                                         100
#define TBG_MWEWTB_ATTENTION_SEND                               1100
#define TBG_MWTBW_POI_VIEW                                      1200
#define TBG_MWTBW_OOB_VIEW                                      1250
#define TBG_MWTBW_JINGLE_RTP                                    1280
#define TBG_MWTBW_MOOD_VIEW                                     1290
//Wessage Widgets - SelectIconMenu
#define TBG_MWSIM_RECENT                                        1000
#define TBG_MWSIM_SKINCOLOR                                     10100
// *** >>> eyeCU >>> ***
#define TBG_MWTBW_CLEAR_WINDOW                                  1800
#define TBG_MWTBW_CHATSTATES                                    10100

//MessageWidgets - InfowWidgetToolBar
#define TBG_MWIWTB_ADDRESSMENU                                  500

//MessageWidgets - EditWidgetToolBar
#define TBG_MWEWTB_SENDMESSAGE                                  500

//MessageWidgets - NormalWindowToolBar
#define TBG_MWNWTB_WINDOWMENU                                   15000

//MultiChatWindow - ToolBarWidget
#define TBG_MCWTBW_BOOKMARKS                                    10200
#define TBG_MCWTBW_ROOM_ENTER                                   10990
#define TBG_MCWTBW_ROOM_EXIT                                    11000

//SeriveDiscovery - DiscoItemsWindow
#define TBG_DIWT_DISCOVERY_NAVIGATE                             100
#define TBG_DIWT_DISCOVERY_DEFACTIONS                           300
#define TBG_DIWT_DISCOVERY_ACTIONS                              500
#define TBG_DIWT_DISCOVERY_FEATURE_ACTIONS                      700

//RosterChanger - AddContactDialog
#define TBG_RCACD_ROSTERCHANGER                                 500

//RosterChanger - SubscriptionRequestDialog
#define TBG_RCSRD_ROSTERCHANGER                                 500

#endif // DEF_TOOLBARGROUPS_H
