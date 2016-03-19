#include "notifications.h"

#include <QSpinBox>
#include <QProcess>
#include <QSystemTrayIcon>
#include <definitions/notificationdataroles.h>
#include <definitions/actiongroups.h>
#include <definitions/toolbargroups.h>
#include <definitions/optionvalues.h>
#include <definitions/optionnodes.h>
#include <definitions/optionnodeorders.h>
#include <definitions/optionwidgetorders.h>
#include <definitions/rosterindexroles.h>
#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <definitions/shortcuts.h>
#include <utils/widgetmanager.h>
#include <utils/shortcuts.h>
#include <utils/options.h>
#include <utils/action.h>
#include <utils/logger.h>
#include "notifykindoptionswidget.h"

#define FIRST_KIND         0x0001
#define LAST_KIND          0x8000
#define UNDEFINED_KINDS    0xFFFF
#define ADR_NOTIFYID       Action::DR_Parametr1

Notifications::Notifications()
{
	FAvatars = NULL;
	FRosterManager = NULL;
	FStatusIcons = NULL;
	FStatusChanger = NULL;
	FTrayManager = NULL;
	FRostersModel = NULL;
	FRostersViewPlugin = NULL;
	FOptionsManager = NULL;
	FMainWindowPlugin = NULL;
	FUrlProcessor = NULL;

	FSoundOnOff = NULL;
	FActivateLast = NULL;
	FRemoveAll = NULL;
	FNotifyMenu = NULL;
	FNetworkAccessManager = NULL;

	FNotifyId = 0;
	FSound = NULL;
}

Notifications::~Notifications()
{
	delete FActivateLast;
	delete FRemoveAll;
	delete FNotifyMenu;
	delete FSound;
}

void Notifications::pluginInfo(IPluginInfo *APluginInfo)
{
	APluginInfo->name = tr("Notifications Manager");
	APluginInfo->description = tr("Allows other modules to notify the user of the events");
	APluginInfo->version = "1.0";
	APluginInfo->author = "Potapov S.A. aka Lion";
	APluginInfo->homePage = "http://www.vacuum-im.org";
}

bool Notifications::initConnections(IPluginManager *APluginManager, int &AInitOrder)
{
	Q_UNUSED(AInitOrder);

	IPlugin *plugin = APluginManager->pluginInterface("ITrayManager").value(0,NULL);
	if (plugin)
	{
		FTrayManager = qobject_cast<ITrayManager *>(plugin->instance());
		if (FTrayManager)
		{
			connect(FTrayManager->instance(),SIGNAL(notifyActivated(int, QSystemTrayIcon::ActivationReason)),
				SLOT(onTrayNotifyActivated(int, QSystemTrayIcon::ActivationReason)));
			connect(FTrayManager->instance(),SIGNAL(notifyRemoved(int)),SLOT(onTrayNotifyRemoved(int)));
		}
	}

	plugin = APluginManager->pluginInterface("IRostersViewPlugin").value(0,NULL);
	if (plugin)
	{
		FRostersViewPlugin = qobject_cast<IRostersViewPlugin *>(plugin->instance());
		if (FRostersViewPlugin)
		{
			connect(FRostersViewPlugin->rostersView()->instance(),SIGNAL(notifyActivated(int)),SLOT(onRosterNotifyActivated(int)));
			connect(FRostersViewPlugin->rostersView()->instance(),SIGNAL(notifyRemoved(int)),SLOT(onRosterNotifyRemoved(int)));
		}
	}

	plugin = APluginManager->pluginInterface("IRostersModel").value(0,NULL);
	if (plugin)
	{
		FRostersModel = qobject_cast<IRostersModel *>(plugin->instance());
	}

	plugin = APluginManager->pluginInterface("IAvatars").value(0,NULL);
	if (plugin)
	{
		FAvatars = qobject_cast<IAvatars *>(plugin->instance());
	}

	plugin = APluginManager->pluginInterface("IRosterManager").value(0,NULL);
	if (plugin)
	{
		FRosterManager = qobject_cast<IRosterManager *>(plugin->instance());
	}

	plugin = APluginManager->pluginInterface("IStatusIcons").value(0,NULL);
	if (plugin)
	{
		FStatusIcons = qobject_cast<IStatusIcons *>(plugin->instance());
	}

	plugin = APluginManager->pluginInterface("IStatusChanger").value(0,NULL);
	if (plugin)
	{
		FStatusChanger = qobject_cast<IStatusChanger *>(plugin->instance());
	}

	plugin = APluginManager->pluginInterface("IMainWindowPlugin").value(0,NULL);
	if (plugin)
	{
		FMainWindowPlugin = qobject_cast<IMainWindowPlugin *>(plugin->instance());
	}

	plugin = APluginManager->pluginInterface("IOptionsManager").value(0,NULL);
	if (plugin)
	{
		FOptionsManager = qobject_cast<IOptionsManager *>(plugin->instance());
	}

	plugin = APluginManager->pluginInterface("IUrlProcessor").value(0);
	if (plugin)
	{
		FUrlProcessor = qobject_cast<IUrlProcessor *>(plugin->instance());
	}

	connect(Options::instance(),SIGNAL(optionsOpened()),SLOT(onOptionsOpened()));
	connect(Options::instance(),SIGNAL(optionsChanged(const OptionsNode &)),SLOT(onOptionsChanged(const OptionsNode &)));

	connect(Shortcuts::instance(),SIGNAL(shortcutActivated(const QString, QWidget *)),SLOT(onShortcutActivated(const QString, QWidget *)));

	return true;
}

bool Notifications::initObjects()
{
	Shortcuts::declareShortcut(SCT_GLOBAL_TOGGLESOUND, tr("Enable/Disable notifications sound"), QKeySequence::UnknownKey, Shortcuts::GlobalShortcut);
	Shortcuts::declareShortcut(SCT_GLOBAL_ACTIVATELASTNOTIFICATION, tr("Activate notification"), QKeySequence::UnknownKey, Shortcuts::GlobalShortcut);

	FSoundOnOff = new Action(this);
	FSoundOnOff->setToolTip(tr("Enable/Disable notifications sound"));
    FSoundOnOff->setText(tr("Notifications sound"));
	FSoundOnOff->setIcon(RSR_STORAGE_MENUICONS, MNI_NOTIFICATIONS_SOUND_ON);
#ifndef EYECU_MOBILE // *** <<< eyeCU <<< ***
	FSoundOnOff->setShortcutId(SCT_GLOBAL_TOGGLESOUND);
#endif
	connect(FSoundOnOff,SIGNAL(triggered(bool)),SLOT(onSoundOnOffActionTriggered(bool)));

	FActivateLast = new Action(this);
	FActivateLast->setVisible(false);
	FActivateLast->setText(tr("Activate Notification"));
	FActivateLast->setShortcutId(SCT_GLOBAL_ACTIVATELASTNOTIFICATION);
	connect(FActivateLast,SIGNAL(triggered(bool)),SLOT(onTrayActionTriggered(bool)));

	FRemoveAll = new Action(this);
	FRemoveAll->setVisible(false);
	FRemoveAll->setText(tr("Remove All Notifications"));
	FRemoveAll->setIcon(RSR_STORAGE_MENUICONS,MNI_NOTIFICATIONS_REMOVE_ALL);
	connect(FRemoveAll,SIGNAL(triggered(bool)),SLOT(onTrayActionTriggered(bool)));

	FNotifyMenu = new Menu;
	FNotifyMenu->setTitle(tr("Pending Notifications"));
	FNotifyMenu->setIcon(RSR_STORAGE_MENUICONS,MNI_NOTIFICATIONS);
	FNotifyMenu->menuAction()->setVisible(false);

	if (FTrayManager)
	{
		FTrayManager->contextMenu()->addAction(FActivateLast,AG_TMTM_NOTIFICATIONS_LAST,false);
		FTrayManager->contextMenu()->addAction(FRemoveAll,AG_TMTM_NOTIFICATIONS_MENU,false);
		FTrayManager->contextMenu()->addAction(FNotifyMenu->menuAction(),AG_TMTM_NOTIFICATIONS_MENU,false);
	}

	if (FMainWindowPlugin)
	{
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
        FMainWindowPlugin->mainWindow()->mainMenuRight()->addAction(FSoundOnOff,AG_MMENU_RI_SOUND,true);
#else
		FMainWindowPlugin->mainWindow()->topToolBarChanger()->insertAction(FSoundOnOff,TBG_MWTTB_NOTIFICATIONS_SOUND);
#endif
// *** >>> eyeCU >>> ***
	}
	FNetworkAccessManager = FUrlProcessor!=NULL ? FUrlProcessor->networkAccessManager() : new QNetworkAccessManager(this);
	
	NotifyWidget::setNetworkManager(FNetworkAccessManager);
	NotifyWidget::setMainWindow(FMainWindowPlugin!=NULL ? FMainWindowPlugin->mainWindow() : NULL);
	
	return true;
}

bool Notifications::initSettings()
{
	Options::setDefaultValue(OPV_NOTIFICATIONS_EXPANDGROUPS,true);
	Options::setDefaultValue(OPV_NOTIFICATIONS_SILENTIFAWAY,false);
	Options::setDefaultValue(OPV_NOTIFICATIONS_SILENTIFDND,true);
	Options::setDefaultValue(OPV_NOTIFICATIONS_NATIVEPOPUPS,false);
	Options::setDefaultValue(OPV_NOTIFICATIONS_FORCESOUND,false);
	Options::setDefaultValue(OPV_NOTIFICATIONS_HIDEMESSAGE,false);
	Options::setDefaultValue(OPV_NOTIFICATIONS_POPUPTIMEOUT,8);
	Options::setDefaultValue(OPV_NOTIFICATIONS_SOUNDCOMMAND,QString("aplay"));
	Options::setDefaultValue(OPV_NOTIFICATIONS_TYPEKINDS_ITEM,0);
	Options::setDefaultValue(OPV_NOTIFICATIONS_KINDENABLED_ITEM,true);
// *** <<< eyeCU <<< ***
	Options::setDefaultValue(OPV_NOTIFICATIONS_ANIMATIONENABLE,true);
// *** >>> eyeCU >>> ***
	if (FOptionsManager)
	{
		IOptionsDialogNode notificationsNode = { ONO_NOTIFICATIONS, OPN_NOTIFICATIONS, MNI_NOTIFICATIONS, tr("Notifications") };
		FOptionsManager->insertOptionsDialogNode(notificationsNode);
		FOptionsManager->insertOptionsDialogHolder(this);
	}

	return true;
}

bool Notifications::startPlugin()
{
	Shortcuts::setGlobalShortcut(SCT_GLOBAL_TOGGLESOUND,true);
	Shortcuts::setGlobalShortcut(SCT_GLOBAL_ACTIVATELASTNOTIFICATION,true);
	return true;
}

QMultiMap<int, IOptionsDialogWidget *> Notifications::optionsDialogWidgets(const QString &ANodeId, QWidget *AParent)
{
	QMultiMap<int, IOptionsDialogWidget *> widgets;
	if (FOptionsManager && ANodeId == OPN_NOTIFICATIONS)
	{
		widgets.insertMulti(OHO_NOTIFICATIONS,FOptionsManager->newOptionsDialogHeader(tr("Notifications"),AParent));
		widgets.insertMulti(OWO_NOTIFICATIONS_DISABLEIFAWAY,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_SILENTIFAWAY),tr("Disable sounds and popup windows if status is 'Away'"),AParent));
		widgets.insertMulti(OWO_NOTIFICATIONS_DISABLEIFDND,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_SILENTIFDND),tr("Disable sounds and popup windows if status is 'Do not disturb'"),AParent));
		widgets.insertMulti(OWO_NOTIFICATIONS_NATIVEPOPUPS,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_NATIVEPOPUPS),tr("Use native popup notifications if available"),AParent));
		widgets.insertMulti(OWO_NOTIFICATIONS_FORCESOUND,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_FORCESOUND),tr("Play notification sound when received a message in the active window"),AParent));
		widgets.insertMulti(OWO_NOTIFICATIONS_HIDEMESSAGE,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_HIDEMESSAGE),tr("Do not show the message body in the popup window"),AParent));		
		widgets.insertMulti(OWO_NOTIFICATIONS_EXPANDGROUPS,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_EXPANDGROUPS),tr("Expand contact groups in roster"),AParent));
// *** <<< eyeCU <<< ***
		if (Options::node(OPV_COMMON_ADVANCED).value().toBool())
			widgets.insertMulti(OWO_NOTIFICATIONS_ANIMATIONENABLE,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_ANIMATIONENABLE),tr("Enable animation"),AParent));
// *** >>> eyeCU >>> ***
		QSpinBox *spbPopupTimeout = new QSpinBox(AParent);
		spbPopupTimeout->setRange(0,120);
// *** <<< eyeCU <<< ***
//		spbPopupTimeout->setSuffix(tr(" seconds"));
		connect(spbPopupTimeout, SIGNAL(valueChanged(int)), SLOT(onSpinBoxValueChanged(int)));
// *** >>> eyeCU >>> ***
		widgets.insertMulti(OWO_NOTIFICATIONS_POPUPTIMEOUT,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_POPUPTIMEOUT),tr("Time to display a pop-up window (0 - always visible):"),spbPopupTimeout,AParent));

#if defined Q_OS_X11 && 0
		widgets.insertMulti(OWO_NOTIFICATIONS_SOUNDCOMMAND,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_SOUNDCOMMAND),tr("System command to play sound:"),AParent));
#endif

		widgets.insertMulti(OHO_NOTIFICATIONS_KINDS,FOptionsManager->newOptionsDialogHeader(tr("Notification kinds"),AParent));
		widgets.insertMulti(OWO_NOTIFICATIONS_ALERTWINDOW,FOptionsManager->newOptionsDialogWidget(Options::node(OPV_NOTIFICATIONS_KINDENABLED_ITEM,QString::number(INotification::AlertWidget)),tr("Highlight the corresponding window in the taskbar"),AParent));
		widgets.insertMulti(OWO_NOTIFICATIONS_KINDS, new NotifyKindOptionsWidget(this,AParent));
	}
	return widgets;
}

QList<int> Notifications::notifications() const
{
	return FNotifyRecords.keys();
}

INotification Notifications::notificationById(int ANotifyId) const
{
	return FNotifyRecords.value(ANotifyId).notification;
}

int Notifications::appendNotification(const INotification &ANotification)
{
	int notifyId = ++FNotifyId;
	LOG_INFO(QString("Appending notification, id=%1, type=%2, kinds=%3, flags=%4").arg(notifyId).arg(ANotification.typeId).arg(ANotification.kinds).arg(ANotification.flags));

	NotifyRecord record;
	record.notification = ANotification;
	emit notificationAppend(notifyId, record.notification);

	bool isAway = FStatusChanger!=NULL ? FStatusChanger->statusItemShow(STATUS_MAIN_ID)==IPresence::Away : false;
	bool isDND = FStatusChanger!=NULL ? FStatusChanger->statusItemShow(STATUS_MAIN_ID)==IPresence::DoNotDisturb : false;
	bool isSilent = isDND && Options::node(OPV_NOTIFICATIONS_SILENTIFDND).value().toBool();
	isSilent = isSilent || (isAway && Options::node(OPV_NOTIFICATIONS_SILENTIFAWAY).value().toBool());

	QIcon icon = qvariant_cast<QIcon>(record.notification.data.value(NDR_ICON));
	QString toolTip = record.notification.data.value(NDR_TOOLTIP).toString();

	if (FRostersModel && FRostersViewPlugin && (record.notification.kinds & INotification::RosterNotify)>0)
	{
		if (!showNotifyByHandler(INotification::RosterNotify,notifyId,record.notification))
		{
			QList<IRosterIndex *> indexes;
			QMap<QString,QVariant> searchData = record.notification.data.value(NDR_ROSTER_SEARCH_DATA).toMap();
			if (searchData.isEmpty())
			{
				Jid streamJid = record.notification.data.value(NDR_STREAM_JID).toString();
				Jid contactJid = record.notification.data.value(NDR_CONTACT_JID).toString();
				bool createIndex = record.notification.data.value(NDR_ROSTER_CREATE_INDEX).toBool();
				indexes = createIndex ? FRostersModel->getContactIndexes(streamJid,contactJid) : FRostersModel->findContactIndexes(streamJid,contactJid);
			}
			else
			{
				QMultiMap<int,QVariant> findData;
				for (QMap<QString,QVariant>::const_iterator it=searchData.constBegin(); it!=searchData.constEnd(); ++it)
					findData.insertMulti(it.key().toInt(),it.value());
				indexes = FRostersModel->rootIndex()->findChilds(findData,true);
			}

			if (!indexes.isEmpty())
			{
				IRostersNotify rnotify;
				rnotify.icon = icon;
				rnotify.order = record.notification.data.value(NDR_ROSTER_ORDER).toInt();
				rnotify.flags = record.notification.data.value(NDR_ROSTER_FLAGS).toInt();
				if (Options::node(OPV_NOTIFICATIONS_EXPANDGROUPS).value().toBool())
					rnotify.flags |= IRostersNotify::ExpandParents;
				rnotify.timeout = record.notification.data.value(NDR_ROSTER_TIMEOUT).toInt();
				rnotify.footer = record.notification.data.value(NDR_ROSTER_FOOTER).toString();
				rnotify.background = record.notification.data.value(NDR_ROSTER_BACKGROUND).value<QBrush>();
				record.rosterId = FRostersViewPlugin->rostersView()->insertNotify(rnotify,indexes);
			}
		}
	}

	if (!isSilent && (record.notification.kinds & INotification::PopupWindow)>0)
	{
		if (!showNotifyByHandler(INotification::PopupWindow,notifyId,record.notification))
		{
			if (Options::node(OPV_NOTIFICATIONS_NATIVEPOPUPS).value().toBool() && FTrayManager && FTrayManager->isMessagesSupported())
			{
				QString title = record.notification.data.value(NDR_POPUP_TITLE).toString();
				QString caption = record.notification.data.value(NDR_POPUP_CAPTION).toString();
				QString text = record.notification.data.value(NDR_POPUP_TEXT).toString();
				int timeout = Options::node(OPV_NOTIFICATIONS_POPUPTIMEOUT).value().toInt()*1000;
				FTrayManager->showMessage(QString("%1 - %2").arg(title,caption),text,QSystemTrayIcon::Information,timeout);
			}
			else
			{
				record.popupWidget = new NotifyWidget(record.notification);
				connect(record.popupWidget,SIGNAL(notifyActivated()),SLOT(onWindowNotifyActivated()));
				connect(record.popupWidget,SIGNAL(notifyRemoved()),SLOT(onWindowNotifyRemoved()));
				connect(record.popupWidget,SIGNAL(windowDestroyed()),SLOT(onWindowNotifyDestroyed()));
// *** <<< eyeCU <<< ***
            	connect(this, SIGNAL(enableAnimation(bool)), record.popupWidget, SLOT(setAnimated(bool)));
				record.popupWidget->setAnimated(Options::node(OPV_NOTIFICATIONS_ANIMATIONENABLE).value().toBool());
            	record.popupWidget->setNetworkManager(FNetworkAccessManager);
// *** >>> eyeCU >>> ***
				record.popupWidget->appear();
			}
		}
	}

	if (FTrayManager && (record.notification.kinds & INotification::TrayNotify)>0)
	{
		if (!showNotifyByHandler(INotification::TrayNotify,notifyId,record.notification))
		{
			ITrayNotify notify;
			notify.blink = true;
			notify.icon = icon;
			notify.toolTip = toolTip;
			record.trayId = FTrayManager->appendNotify(notify);
		}
		FTrayNotifies.append(notifyId);
		FActivateLast->setIcon(icon);
		FActivateLast->setVisible(true);
	}

	if (FTrayManager && !toolTip.isEmpty() && (record.notification.kinds & INotification::TrayAction)>0)
	{
		if (!showNotifyByHandler(INotification::TrayAction,notifyId,record.notification))
		{
			record.trayAction = new Action(FNotifyMenu);
			record.trayAction->setIcon(icon);
			record.trayAction->setText(toolTip);
			record.trayAction->setData(ADR_NOTIFYID,notifyId);
			connect(record.trayAction,SIGNAL(triggered(bool)),SLOT(onActionNotifyActivated(bool)));
			FNotifyMenu->addAction(record.trayAction);
		}
	}

	if (!isSilent && (record.notification.kinds & INotification::SoundPlay)>0)
	{
		if (!showNotifyByHandler(INotification::SoundPlay,notifyId,record.notification))
		{
			QString soundName = record.notification.data.value(NDR_SOUND_FILE).toString();
			QString soundFile = FileStorage::staticStorage(RSR_STORAGE_SOUNDS)->fileFullName(soundName);
			if (!soundFile.isEmpty())
			{
#ifdef Q_OS_X11
				QProcess::startDetached(Options::node(OPV_NOTIFICATIONS_SOUNDCOMMAND).value().toString(),QStringList()<<soundFile);
#else
				delete FSound;
				FSound = new QSound(soundFile);
				FSound->play();
#endif
			}
		}
	}

	if ((record.notification.kinds & INotification::ShowMinimized)>0)
	{
		if (!showNotifyByHandler(INotification::ShowMinimized,notifyId,record.notification))
		{
			QWidget *widget = qobject_cast<QWidget *>((QWidget *)record.notification.data.value(NDR_SHOWMINIMIZED_WIDGET).toLongLong());
			if (widget)
			{
				FDelayedShowMinimized.append(widget);
				QTimer::singleShot(0,this,SLOT(onDelayedShowMinimized()));
			}
		}
	}

	if ((record.notification.kinds & INotification::AlertWidget)>0)
	{
		if (!showNotifyByHandler(INotification::AlertWidget,notifyId,record.notification))
		{
			QWidget *widget = qobject_cast<QWidget *>((QWidget *)record.notification.data.value(NDR_ALERT_WIDGET).toLongLong());
			if (widget)
				WidgetManager::alertWidget(widget);
		}
	}

	if ((record.notification.kinds & INotification::TabPageNotify)>0)
	{
		if (!showNotifyByHandler(INotification::TabPageNotify,notifyId,record.notification))
		{
			IMessageTabPage *page = qobject_cast<IMessageTabPage *>((QWidget *)record.notification.data.value(NDR_TABPAGE_WIDGET).toLongLong());
			if (page && page->tabPageNotifier())
			{
				IMessageTabPageNotify notify;
				notify.icon = icon;
				notify.toolTip = toolTip;
				notify.priority = record.notification.data.value(NDR_TABPAGE_PRIORITY).toInt();
				notify.blink = record.notification.data.value(NDR_TABPAGE_ICONBLINK).toBool();
				record.tabPageId = page->tabPageNotifier()->insertNotify(notify);
				record.tabPageNotifier = page->tabPageNotifier()->instance();
			}
		}
	}
		
	if ((record.notification.kinds & INotification::AutoActivate)>0)
	{
		FDelayedActivations.append(notifyId);
		QTimer::singleShot(0,this,SLOT(onDelayedActivations()));
	}

	FRemoveAll->setVisible(!FNotifyMenu->isEmpty());
	FNotifyMenu->menuAction()->setVisible(!FNotifyMenu->isEmpty());

	FDelayedRemovals.append(notifyId);
	QTimer::singleShot(0,this,SLOT(onDelayedRemovals()));

	FNotifyRecords.insert(notifyId,record);
	emit notificationAppended(notifyId, record.notification);

	return notifyId;
}

void Notifications::activateNotification(int ANotifyId)
{
	if (FNotifyRecords.contains(ANotifyId))
	{
		LOG_INFO(QString("Activating notification, id=%1").arg(ANotifyId));
		emit notificationActivated(ANotifyId);
	}
}

void Notifications::removeNotification(int ANotifyId)
{
	if (FNotifyRecords.contains(ANotifyId))
	{
		LOG_INFO(QString("Removing notification, id=%1").arg(ANotifyId));
		NotifyRecord record = FNotifyRecords.take(ANotifyId);
		if (FRostersViewPlugin && record.rosterId!=0)
		{
			FRostersViewPlugin->rostersView()->removeNotify(record.rosterId);
		}
		if (!record.popupWidget.isNull())
		{
			record.popupWidget->deleteLater();
		}
		if (FTrayManager && record.trayId!=0)
		{
			FTrayManager->removeNotify(record.trayId);
		}
		if (!record.trayAction.isNull())
		{
			FNotifyMenu->removeAction(record.trayAction);
			record.trayAction->deleteLater();
		}
		if (!record.tabPageNotifier.isNull())
		{
			IMessageTabPageNotifier *notifier =  qobject_cast<IMessageTabPageNotifier *>(record.tabPageNotifier);
			if (notifier)
				notifier->removeNotify(record.tabPageId);
		}
		if (FTrayNotifies.contains(ANotifyId))
		{
			FTrayNotifies.removeAll(ANotifyId);
			if (!FTrayNotifies.isEmpty())
			{
				NotifyRecord lastRecord = FNotifyRecords.value(FTrayNotifies.last());
				FActivateLast->setIcon(qvariant_cast<QIcon>(lastRecord.notification.data.value(NDR_ICON)));
			}
			else
			{
				FActivateLast->setVisible(false);
			}
		}
		qDeleteAll(record.notification.actions);

		FRemoveAll->setVisible(!FNotifyMenu->isEmpty());
		FNotifyMenu->menuAction()->setVisible(!FNotifyMenu->isEmpty());

		emit notificationRemoved(ANotifyId);
	}
}

void Notifications::registerNotificationType(const QString &ATypeId, const INotificationType &AType)
{
	if (!FTypeRecords.contains(ATypeId))
	{
		TypeRecord typeRecord;
		typeRecord.kinds = UNDEFINED_KINDS;
		typeRecord.type = AType;
		FTypeRecords.insert(ATypeId,typeRecord);
		LOG_DEBUG(QString("Registered notification type, id=%1").arg(ATypeId));
	}
}

QList<QString> Notifications::notificationTypes() const
{
	return FTypeRecords.keys();
}

INotificationType Notifications::notificationType(const QString &ATypeId) const
{
	return FTypeRecords.value(ATypeId).type;
}

void Notifications::removeNotificationType(const QString &ATypeId)
{
	FTypeRecords.remove(ATypeId);
}

ushort Notifications::enabledNotificationKinds() const
{
	ushort kinds = 0;
	for (ushort kind=FIRST_KIND; kind>0; kind=kind<<1)
	{
		if (Options::node(OPV_NOTIFICATIONS_KINDENABLED_ITEM,QString::number(kind)).value().toBool())
			kinds |= kind;
	}
	return kinds;
}

void Notifications::setEnabledNotificationKinds(ushort AKinds)
{
	for (ushort kind=FIRST_KIND; kind>0; kind=kind<<1)
		Options::node(OPV_NOTIFICATIONS_KINDENABLED_ITEM,QString::number(kind)).setValue((AKinds & kind)>0 ? true : false);
}

ushort Notifications::typeNotificationKinds(const QString &ATypeId) const
{
	if (FTypeRecords.contains(ATypeId))
	{
		TypeRecord &rec = FTypeRecords[ATypeId];
		if (rec.kinds == UNDEFINED_KINDS)
			rec.kinds = Options::node(OPV_NOTIFICATIONS_TYPEKINDS_ITEM,ATypeId).value().toInt() ^ rec.type.kindDefs;
		return rec.kinds & rec.type.kindMask;
	}
	return 0;
}

ushort Notifications::enabledTypeNotificationKinds(const QString &ATypeId) const
{
	return typeNotificationKinds(ATypeId) & enabledNotificationKinds();
}

void Notifications::setTypeNotificationKinds(const QString &ATypeId, ushort AKinds)
{
	if (FTypeRecords.contains(ATypeId))
	{
		TypeRecord &rec = FTypeRecords[ATypeId];
		rec.kinds = AKinds & rec.type.kindMask;
		Options::node(OPV_NOTIFICATIONS_TYPEKINDS_ITEM,ATypeId).setValue(rec.kinds ^ rec.type.kindDefs);
	}
}

void Notifications::insertNotificationHandler(int AOrder, INotificationHandler *AHandler)
{
	if (AHandler != NULL)
	{
		LOG_DEBUG(QString("Notification handler inserted, order=%1").arg(AOrder));
		FHandlers.insertMulti(AOrder, AHandler);
		emit notificationHandlerInserted(AOrder,AHandler);
	}
}

void Notifications::removeNotificationHandler(int AOrder, INotificationHandler *AHandler)
{
	if (FHandlers.contains(AOrder,AHandler))
	{
		LOG_DEBUG(QString("Notification handler removed, order=%1").arg(AOrder));
		FHandlers.remove(AOrder,AHandler);
		emit notificationHandlerRemoved(AOrder,AHandler);
	}
}

QImage Notifications::contactAvatar(const Jid &AContactJid) const
{
	return FAvatars!=NULL ? FAvatars->loadAvatarImage(FAvatars->avatarHash(AContactJid), QSize(32,32)) : QImage();
}

QIcon Notifications::contactIcon(const Jid &AStreamJid, const Jid &AContactJid) const
{
	return FStatusIcons!=NULL ? FStatusIcons->iconByJid(AStreamJid,AContactJid) : QIcon();
}

QString Notifications::contactName(const Jid &AStreamJid, const Jid &AContactJid) const
{
	QString name;

	IRosterIndex *index = FRostersModel!=NULL ? FRostersModel->getContactIndexes(AStreamJid, AContactJid).value(0) : NULL; // *** <<< eyeCU >>> ***
	if (index != NULL)
		name = index->data(RDR_NAME).toString();

	if (name.isEmpty())
	{
		IRoster *roster = FRosterManager!=NULL ? FRosterManager->findRoster(AStreamJid) : NULL;
		name = roster!=NULL ? roster->findItem(AContactJid).name : AContactJid.uNode();
	}

	return name.isEmpty() ? AContactJid.uBare() : name;
}

int Notifications::notifyIdByRosterId(int ARosterId) const
{
	QMap<int,NotifyRecord>::const_iterator it = FNotifyRecords.constBegin();
	for (; it!=FNotifyRecords.constEnd(); ++it)
		if (it.value().rosterId == ARosterId)
			return it.key();
	return -1;
}

int Notifications::notifyIdByTrayId(int ATrayId) const
{
	QMap<int,NotifyRecord>::const_iterator it = FNotifyRecords.constBegin();
	for (; it!=FNotifyRecords.constEnd(); ++it)
		if (it.value().trayId == ATrayId)
			return it.key();
	return -1;
}

int Notifications::notifyIdByWidget(NotifyWidget *AWidget) const
{
	QMap<int,NotifyRecord>::const_iterator it = FNotifyRecords.constBegin();
	for (; it!=FNotifyRecords.constEnd(); ++it)
		if (it.value().popupWidget == AWidget)
			return it.key();
	return -1;
}

bool Notifications::showNotifyByHandler(ushort AKind, int ANotifyId, const INotification &ANotification) const
{
	QMultiMap<int, INotificationHandler *>::const_iterator it = FHandlers.constBegin();
	while (it != FHandlers.constEnd())
	{
		if (it.value()->showNotification(it.key(),AKind,ANotifyId,ANotification))
			return true;
		++it;
	}
	return false;
}

void Notifications::removeInvisibleNotification(int ANotifyId)
{
	NotifyRecord record = FNotifyRecords.value(ANotifyId);
	if (record.notification.flags & INotification::RemoveInvisible)
	{
		bool invisible = true;
		if (record.trayId != 0)
			invisible = false;
		if (record.rosterId != 0)
			invisible = false;
		if (record.tabPageId != 0)
			invisible = false;
		if (!record.popupWidget.isNull())
			invisible = false;
		if (invisible)
			removeNotification(ANotifyId);
	}
}

void Notifications::onDelayedRemovals()
{
	foreach(int notifyId, FDelayedRemovals)
		removeInvisibleNotification(notifyId);
	FDelayedRemovals.clear();
}

void Notifications::onDelayedActivations()
{
	foreach(int notifyId, FDelayedActivations)
		activateNotification(notifyId);
	FDelayedActivations.clear();
}

void Notifications::onDelayedShowMinimized()
{
	foreach(QWidget *widget, FDelayedShowMinimized)
	{
		IMessageTabPage *page = qobject_cast<IMessageTabPage *>(widget);
		if (page)
			page->showMinimizedTabPage();
		else if (widget->isWindow() && !widget->isVisible())
			widget->showMinimized();
	}
	FDelayedShowMinimized.clear();
}

void Notifications::onSoundOnOffActionTriggered(bool)
{
	OptionsNode node = Options::node(OPV_NOTIFICATIONS_KINDENABLED_ITEM,QString::number(INotification::SoundPlay));
	node.setValue(!node.value().toBool());
}

void Notifications::onTrayActionTriggered(bool)
{
	Action *action = qobject_cast<Action *>(sender());
	if (action)
	{
		if (action == FActivateLast)
		{
			if (!FTrayNotifies.isEmpty())
				activateNotification(FTrayNotifies.last());
		}
		else if (action == FRemoveAll)
		{
			foreach(int notifyId, FNotifyRecords.keys())
				removeNotification(notifyId);
		}
	}
}

void Notifications::onRosterNotifyActivated(int ANotifyId)
{
	activateNotification(notifyIdByRosterId(ANotifyId));
}

void Notifications::onRosterNotifyRemoved(int ANotifyId)
{
	int notifyId = notifyIdByRosterId(ANotifyId);
	if (FNotifyRecords.contains(notifyId))
	{
		FNotifyRecords[notifyId].rosterId = 0;
		removeInvisibleNotification(notifyId);
	}
}

void Notifications::onTrayNotifyActivated(int ANotifyId, QSystemTrayIcon::ActivationReason AReason)
{
	if (AReason == QSystemTrayIcon::Trigger)
		activateNotification(notifyIdByTrayId(ANotifyId));
}

void Notifications::onTrayNotifyRemoved(int ANotifyId)
{
	int notifyId = notifyIdByTrayId(ANotifyId);
	if (FNotifyRecords.contains(notifyId))
	{
		FNotifyRecords[notifyId].trayId = 0;
		removeInvisibleNotification(notifyId);
	}
}

void Notifications::onWindowNotifyActivated()
{
	activateNotification(notifyIdByWidget(qobject_cast<NotifyWidget*>(sender())));
}

void Notifications::onWindowNotifyRemoved()
{
	removeNotification(notifyIdByWidget(qobject_cast<NotifyWidget*>(sender())));
}

void Notifications::onWindowNotifyDestroyed()
{
	int notifyId = notifyIdByWidget(qobject_cast<NotifyWidget*>(sender()));
	if (FNotifyRecords.contains(notifyId))
	{
		FNotifyRecords[notifyId].popupWidget = NULL;
		removeInvisibleNotification(notifyId);
	}
}

void Notifications::onActionNotifyActivated(bool)
{
	Action *action = qobject_cast<Action *>(sender());
	if (action)
	{
		int notifyId = action->data(ADR_NOTIFYID).toInt();
		activateNotification(notifyId);
	}
}

void Notifications::onOptionsOpened()
{
	onOptionsChanged(Options::node(OPV_NOTIFICATIONS_KINDENABLED_ITEM,QString::number(INotification::SoundPlay)));
	onOptionsChanged(Options::node(OPV_NOTIFICATIONS_KINDENABLED_ITEM,QString::number(INotification::AlertWidget)));
}

void Notifications::onOptionsChanged(const OptionsNode &ANode)
{
	if (Options::cleanNSpaces(ANode.path()) == OPV_NOTIFICATIONS_KINDENABLED_ITEM)
	{
		if (ANode.nspace().toInt() == INotification::SoundPlay)
		{
			FSoundOnOff->setIcon(RSR_STORAGE_MENUICONS, ANode.value().toBool() ? MNI_NOTIFICATIONS_SOUND_ON : MNI_NOTIFICATIONS_SOUND_OFF);
		}
		else if (ANode.nspace().toInt() == INotification::AlertWidget)
		{
			WidgetManager::setWidgetAlertEnabled(ANode.value().toBool());
		}
	}
// *** <<< eyeCU <<< ***
	else if (ANode.path()==OPV_NOTIFICATIONS_ANIMATIONENABLE)
		emit enableAnimation(ANode.value().toBool());
// *** >>> eyeCU >>> ***
}

void Notifications::onShortcutActivated(const QString &AId, QWidget *AWidget)
{
	if (AWidget == NULL)
	{
		if (AId == SCT_GLOBAL_TOGGLESOUND)
		{
			FSoundOnOff->trigger();
		}
		else if (AId == SCT_GLOBAL_ACTIVATELASTNOTIFICATION)
		{
			FActivateLast->trigger();
		}
	}
}
// *** <<< eyeCU <<< ***
void Notifications::onSpinBoxValueChanged(int value)
{
	qobject_cast<QSpinBox *>(sender())->setSuffix(" "+tr("second(s)", "", value));
}
// *** >>> eyeCU >>> ***
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(plg_notifications, Notifications)
#endif
