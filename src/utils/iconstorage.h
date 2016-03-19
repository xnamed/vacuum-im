#ifndef ICONSTORAGE_H
#define ICONSTORAGE_H

#include <QPair>
#include <QIcon>
#include <QTimer>
#include <QImageReader>
#include "filestorage.h"

#define ICON_STORAGE_ANIMATE_INTERVAL "animate"

class UTILS_EXPORT IconStorage :
	public FileStorage
{
	Q_OBJECT;
	struct IconAnimateFrame;
	struct IconAnimateParams;
	struct IconUpdateParams;
public:
	IconStorage(const QString &AStorage, const QString &ASubStorage = QString::null, QObject *AParent = NULL);
	~IconStorage();
	QIcon getIcon(const QString &AKey, int AIndex = 0) const;
	void insertAutoIcon(QObject *AObject, const QString &AKey, int AIndex = 0, int AAnimate = 0, const QString &AProperty = "icon");
	void removeAutoIcon(QObject *AObject);
public:
	static void clearIconCache();
	static IconStorage *staticStorage(const QString &AStorage);
	static void setScale(int AValue) {FScale = AValue;}  // *** <<< eyeCU <<< ***
	static int scale() {return FScale;}					 // *** <<< eyeCU <<< ***
protected:
	void initAnimation(QObject *AObject, IconUpdateParams *AParams);
	void removeAnimation(IconUpdateParams *AParams);
	void updateObject(QObject *AObject);
	void removeObject(QObject *AObject);
protected slots:
	void onStorageChanged();
	void onAnimateTimer();
	void onObjectDestroyed(QObject *AObject);
private:
	QHash<QTimer *, QObject *> FTimerObject;
	QHash<QObject *, IconUpdateParams *> FUpdateParams;
private:
	static QHash<QString, IconStorage *> FStaticStorages;
	static QHash<QObject *, IconStorage *> FObjectStorage;
	static QHash<QString, QHash<QString, QIcon> > FIconCache;
	static QHash<QString, QHash<QString, QList<IconAnimateFrame> > > FAnimateCache;
	static int FScale;		 // *** <<< eyeCU <<< ***
};

#endif // ICONSTORAGE_H
