#ifndef DEF_NOTIFICATIONDATAROLES_H
#define DEF_NOTIFICATIONDATAROLES_H

enum NotificationDataRoles {
	// Common roles
	NDR_ICON,
	NDR_TOOLTIP,
	NDR_STREAM_JID,
	NDR_CONTACT_JID,
	// RosterIcon
	NDR_ROSTER_ORDER,
	NDR_ROSTER_FLAGS,
	NDR_ROSTER_FOOTER,
	NDR_ROSTER_TIMEOUT,
	NDR_ROSTER_BACKGROUND,
	NDR_ROSTER_SEARCH_DATA,
	NDR_ROSTER_CREATE_INDEX,
	// PopupWindow
	NDR_POPUP_CAPTION,
	NDR_POPUP_TITLE,
	NDR_POPUP_NOTICE,
	NDR_POPUP_IMAGE,
	NDR_POPUP_HTML,
	NDR_POPUP_TEXT,
	NDR_POPUP_TIMEOUT,
	// SoundPlay
	NDR_SOUND_FILE,
	// AlertWidget
	NDR_ALERT_WIDGET,
	// TabPageNotify
	NDR_TABPAGE_WIDGET,
	NDR_TABPAGE_PRIORITY,
	NDR_TABPAGE_ICONBLINK,
	// ShowMinimized
	NDR_SHOWMINIMIZED_WIDGET,
        // *** <<< eyeCU <<< ***
        // Avatar data for Attention dialog
        NDR_ATTENTION_DIALOG_AVATAR_FILE_NAME,
        // Jingle call type
        NDR_JINGLE_RTP_EVENT_TYPE,
        // Mobile options
        NDR_VIBRATE,
        NDR_LIGHTS
        // *** >>> eyeCU >>> ***
};

#endif // DEF_NOTIFICATIONDATAROLES_H
