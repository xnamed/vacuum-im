#ifndef EMOJI_H
#define EMOJI_H

#include <QHash>
#include <QStringList>
#include <QDir>
#include <interfaces/ipluginmanager.h>
#include <interfaces/iemoji.h>
#include <interfaces/imessageprocessor.h>
#include <interfaces/imessagewidgets.h>
#include <interfaces/ioptionsmanager.h>
#include "selecticonmenu.h"

struct EmojiTreeItem {
	QUrl url;
	QMap<QChar, EmojiTreeItem *> childs;
};

class Emoji :
	public QObject,
	public IPlugin,
	public IEmoji,
	public IMessageWriter,
	public IOptionsDialogHolder,
	public IMessageEditContentsHandler
{
	Q_OBJECT
	Q_INTERFACES(IPlugin IEmoji IMessageWriter IOptionsDialogHolder IMessageEditContentsHandler)
#if QT_VERSION >= 0x050000
	Q_PLUGIN_METADATA(IID "ru.rwsoftware.eyecu.IEmoji")
#endif
public:
	Emoji();
	~Emoji();
	//IPlugin
	virtual QObject *instance() { return this; }
	virtual QUuid pluginUuid() const { return EMOJI_UUID; }
	virtual void pluginInfo(IPluginInfo *APluginInfo);
	virtual bool initConnections(IPluginManager *APluginManager, int &AInitOrder);
	virtual bool initObjects();
	virtual bool initSettings();
	virtual bool startPlugin() { return true; }
	//IMessageWriter
	virtual void writeTextToMessage(int AOrder, Message &AMessage, QTextDocument *ADocument, const QString &ALang);
	virtual void writeMessageToText(int AOrder, Message &AMessage, QTextDocument *ADocument, const QString &ALang);
	//IOptionsHolder
	virtual QMultiMap<int, IOptionsDialogWidget *> optionsDialogWidgets(const QString &ANodeId, QWidget *AParent);
	//IMessageEditContentsHandler
	virtual bool messageEditContentsCreate(int AOrder, IMessageEditWidget *AWidget, QMimeData *AData);
	virtual bool messageEditContentsCanInsert(int AOrder, IMessageEditWidget *AWidget, const QMimeData *AData);
	virtual bool messageEditContentsInsert(int AOrder, IMessageEditWidget *AWidget, const QMimeData *AData, QTextDocument *ADocument);
	virtual bool messageEditContentsChanged(int AOrder, IMessageEditWidget *AWidget, int &APosition, int &ARemoved, int &AAdded);
	//IEmoticons
	virtual QList<QString> activeIconsets() const;
	virtual QUrl urlByKey(const QString &AKey) const;
	virtual QString keyByUrl(const QUrl &AUrl) const;
	virtual QMap<int, QString> findTextEmoticons(const QTextDocument *ADocument, int AStartPos=0, int ALength=-1) const;
	virtual QMap<int, QString> findImageEmoticons(const QTextDocument *ADocument, int AStartPos=0, int ALength=-1) const;
	virtual QStringList recentIcons(const QString &ASetName) const {Q_UNUSED(ASetName) return FRecent;}
	virtual QMap<uint, QString> setEmoji(const QString &AEmojiSet) const;
	//IEmoji
	virtual QString categoryName(Category ACategory) const {return FCategoryNames.value(ACategory);}
	virtual QIcon categoryIcon(Category ACategory) const {return FCategoryIcons.value(ACategory);}
	virtual QIcon getIcon(const QString &AEmojiCode, const QSize &ASize=QSize()) const;
	virtual QIcon getIconForSet(const QString &AEmojiSet, const QString &AEmojiText, const QSize &ASize=QSize()) const;
	virtual QMap<uint, EmojiData> emojiData(Category ACategory) const;
	virtual EmojiData findData(const QString &AEmojiCode) const;
	virtual bool isColored(const QString &AEmojiCode) const;
	virtual const QStringList &colorSuffixes() const {return FColorSuffixes;}
	virtual int categoryCount(Category ACategory) const {return FCategoryCount[ACategory];}
	virtual QStringList emojiSets() const {return FEmojiSets.keys();}
	virtual QList<int> availableSizes(const QString &ASetName) const {return FAvailableSizes.value(ASetName);}
protected:
	void findEmojiSets();
	void loadEmojiSet(const QString &AEmojiSet);
	void createTreeItem(const QString &AKey, const QUrl &AUrl);
	void clearTreeItem(EmojiTreeItem *AItem) const;
	bool isWordBoundary(const QString &AText) const;
	int replaceTextToImage(QTextDocument *ADocument, int AStartPos=0, int ALength=-1) const;
	int replaceImageToText(QTextDocument *ADocument, int AStartPos=0, int ALength=-1) const;
	SelectIconMenu *createSelectIconMenu(const QString &AIconSet, QWidget *AParent);
	void updateSelectIconMenu(const QString &AIconSet);
	QString getFileName(const EmojiData &AEmojiData, const QDir &ADir) const;
protected slots:
	void onToolBarWindowLayoutChanged();
	void onToolBarWidgetCreated(IMessageToolBarWidget *AWidget);
	void onToolBarWidgetDestroyed(QObject *AObject);
	void onSelectIconMenuSelected(QString AIconKey, const QString &AIconText);
	void onSelectIconMenuDestroyed(QObject *AObject);
	void onOptionsOpened();
	void onOptionsChanged(const OptionsNode &ANode);
private:
	IMessageWidgets *FMessageWidgets;
	IMessageProcessor *FMessageProcessor;
	IOptionsManager *FOptionsManager;
private:
	EmojiTreeItem FRootTreeItem;
	QMap<int, QHash<QString, QUrl> > FUrlByKey;
	QHash<QString, QString> FKeyByUrl;
	QHash<Category, QMap<uint, EmojiData> > FCategories;
	QHash<QString, EmojiData> FEmojiData;
	QList<IMessageToolBarWidget *> FToolBarsWidgets;
	QMap<SelectIconMenu *, IMessageToolBarWidget *> FToolBarWidgetByMenu;

	QStringList FColorSuffixes;
	QStringList FRecent;
//	QStringList FEmojiSets;
	QHash<QString, QMap<uint, QString> > FEmojiSets;
	QHash<QString, QList<int> >	FAvailableSizes;
	mutable QHash<QString, QIcon> FIconHash;
	QMap<int, QString> FCategoryNames;
	QMap<int, QString> FCategoryIDs;
	QMap<int, QIcon> FCategoryIcons;	
	int FCategoryCount[8];
	QDir		FResourceDir;
};

#endif // EMOJI_H
