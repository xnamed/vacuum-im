#include "iconstorage.h"
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QVariant>
#include <QApplication>

QHash<QString, IconStorage*> IconStorage::FStaticStorages;
QHash<QObject*, IconStorage*> IconStorage::FObjectStorage;
QHash<QString, QHash<QString,QIcon> > IconStorage::FIconCache;
QHash<QString, QHash<QString, QList<IconStorage::IconAnimateFrame> > > IconStorage::FAnimateCache;

struct IconStorage::IconAnimateFrame {
	int delay;
	QIcon icon;
};

struct IconStorage::IconAnimateParams {
	IconAnimateParams() { 
		frameIndex = 0;
		frameCount = 0;
		timer = new QTimer;
		reader = NULL;
	}
	~IconAnimateParams() { 
		timer->stop();
		timer->deleteLater();
		delete reader; 
	}
	int frameIndex;
	int frameCount;
	QTimer *timer;
	QImageReader *reader;
	QList<IconAnimateFrame> frames;
};

struct IconStorage::IconUpdateParams {
	IconUpdateParams() { 
		animation = NULL; 
	}
	~IconUpdateParams() { 
		delete animation; 
	}
	QString key;
	int index;
	int animate;
	QString prop;
	IconAnimateParams *animation;
};

//!--- *** <<< eyeCU <<< *** ------
#define SCALEBASE	8.0
#define SCALEMIN	3.0
#define SCALESTEP	1.0
#define DELIM		#

qreal IconStorage::FScale(2.0);
float IconStorage::FFontPointSize(8.0);
int IconStorage::FBaseSize(8);
//!--- *** >>> eyeCU >>> *** -------

IconStorage::IconStorage(const QString &AStorage, const QString &ASubStorage, QObject *AParent) : FileStorage(AStorage,ASubStorage,AParent)
{
	connect(this,SIGNAL(storageChanged()),SLOT(onStorageChanged()));
}

IconStorage::~IconStorage()
{
	QList<QObject*> objects = FUpdateParams.keys();
	foreach(QObject *object, objects)
		removeObject(object);
}

// *** <<< eyeCU <<< ***
QPixmap IconStorage::getStoragePixmap(const QString AFileName)
{
    if(AFileName.isEmpty() || AFileName.isNull())
        return QPixmap();   //! Later need return pixmap about not picture...

	QStringList sizes;
	sizes <<"1"<<"8"<<"16"<<"24"<<"32"<<"40"<<"48"<<"64"<<"80"<<"96"<<"128"<<"160"<<"192"<<"256"<<"320"<<"384"<<"448"<<"512";
    QStringList partsName = AFileName.split(".");
    QString     extName   = partsName[1];   // or [partsName.size()-1]
    QString     nameName  = partsName[0];
    if(nameName.split("#").size()>1)        // example file name= AABBCC#32
    {
        QFile file(AFileName);
        if(file.exists())
            return QPixmap::fromImage(QImageReader(AFileName).read());
    }
    else
    {
		if(FScale>SCALEMIN)
        {
            //!-build new name ---
			QString suffix		= QString("%1").arg(FScale*SCALEBASE);
			QString newFileName	= QString(nameName).append("#%1.%2").arg(suffix).arg(extName);
            QFile file(newFileName);
            if(file.exists())
            {
                return QPixmap::fromImage(QImageReader(newFileName).read());
            }
            else
            {
                //! SEARCH NEAR NAME
                bool result=false;
				qreal step =FScale;
                QPixmap pixmap;
                do{
					step		-= SCALESTEP;
					suffix		= QString("%1").arg(SCALEBASE*step);
					newFileName	= QString(nameName).append("#%1.%2").arg(suffix).arg(extName);
                    file.setFileName(newFileName);
                    if(file.exists())
                    {
                        result=true;
                        break;
                     }
				} while(step>SCALEMIN);

                if(result) 	// Scale icon according to FScale
                    pixmap = QPixmap::fromImage(QImageReader(newFileName).read());
                else{
                    QFile file(AFileName);
                    if(file.exists()){
                        pixmap = QPixmap::fromImage(QImageReader(AFileName).read());
                    }
                }
                if (pixmap.isNull())
                    return pixmap;      //! not scaled if pixmap== NULL
				return (pixmap.width()==pixmap.height() && pixmap.width()<FScale*SCALEBASE)?pixmap.scaled(FScale*SCALEBASE,FScale*SCALEBASE,Qt::IgnoreAspectRatio,Qt::SmoothTransformation):pixmap;
            }
        }
        else
        {
            QFile file(AFileName);
            if(file.exists())
                return QPixmap::fromImage(QImageReader(AFileName).read());
        }
    }

    return QPixmap();
}

/*! ----OLD VARIANT --
QPixmap IconStorage::getStoragePixmap(const QString AFileName)
{
	QStringList partsName = AFileName.split(".");
	QString     extName   = partsName[1];   // or [partsName.size()-1]
	QString     nameName  = partsName[0];
	if(nameName.split("#").size()>1)        // example file name= AABBCC#32
	{
		QFile file(AFileName);
		if(file.exists())
			return QPixmap::fromImage(QImageReader(AFileName).read());
	}
	else
	{
		if(FScale>SCALEMIN)
		{
			//!-build new name ---
			QString suffix=QString("%1").arg(FScale*SCALEBASE);
			QString newFileName=QString(nameName).append("#%1.%2").arg(suffix).arg(extName);
			QFile file(newFileName);
			if(file.exists())
			{
				return QPixmap::fromImage(QImageReader(newFileName).read());
			}
			else
			{
				//! SEARCH NEAR NAME
				bool result=false;
				qreal step =FScale;
				QPixmap pixmap;
				do{
					step-=SCALESTEP;
					suffix=QString("%1").arg(SCALEBASE*step);
					newFileName=QString(nameName).append("#%1.%2").arg(suffix).arg(extName);
					file.setFileName(newFileName);
					if(file.exists())
					{
						result=true;
						break;
					 }
				} while(step>SCALEMIN);
				if(result) 	// Scale icon according to FScale
					pixmap = QPixmap::fromImage(QImageReader(newFileName).read());
				else{
					QFile file(AFileName);
					if(file.exists())
						pixmap = QPixmap::fromImage(QImageReader(AFileName).read());
				}
				if (pixmap.isNull())
					return pixmap;      //! not scaled if pixmap== NULL
				return (pixmap.width()==pixmap.height() && pixmap.width()<FScale*SCALEBASE)?pixmap.scaled(FScale*SCALEBASE,FScale*SCALEBASE,Qt::IgnoreAspectRatio,Qt::SmoothTransformation):pixmap;
			}
		}
		else
		{
			QFile file(AFileName);
			if(file.exists())
				return QPixmap::fromImage(QImageReader(AFileName).read());
		}
	}
	return QPixmap();
}

--------------------------------------------------------------- */


// *** >>> eyeCU >>> ***

QIcon IconStorage::getIcon(const QString &AKey, int AIndex) const
{
	QIcon icon;
	QString key = fileCacheKey(AKey,AIndex);
	if (!key.isEmpty())
	{
		icon = FIconCache[storage()].value(key);
		if (icon.isNull())
		{
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
            icon.addPixmap(getStoragePixmap(fileFullName(AKey,AIndex)));
#else
// *** >>> eyeCU >>> ***
			icon.addFile(fileFullName(AKey,AIndex));
#endif
			FIconCache[storage()].insert(key,icon);
		}
	}
	return icon;
}

void IconStorage::clearIconCache()
{
	FIconCache.clear();
	FAnimateCache.clear();
}

IconStorage *IconStorage::staticStorage(const QString &AStorage)
{
	IconStorage *iconStorage = FStaticStorages.value(AStorage,NULL);
	if (!iconStorage)
	{
		iconStorage = new IconStorage(AStorage,FILE_STORAGE_SHARED_DIR,qApp);
		FStaticStorages.insert(AStorage,iconStorage);
	}
    return iconStorage;
}

void IconStorage::insertAutoIcon(QObject *AObject, const QString &AKey, int AIndex, int AAnimate, const QString &AProperty)
{
	IconStorage *oldStorage = FObjectStorage.value(AObject);
	if (oldStorage!=NULL && oldStorage!=this)
		oldStorage->removeAutoIcon(AObject);

	if (AObject!=NULL && !AKey.isEmpty())
	{
		IconUpdateParams *params;
		if (oldStorage!=this)
		{
			params = new IconUpdateParams;
			FObjectStorage.insert(AObject,this);
			FUpdateParams.insert(AObject,params);
		}
		else
		{
			params = FUpdateParams.value(AObject);
		}
		params->key = AKey;
		params->index = AIndex;
		params->prop = AProperty;
		params->animate = AAnimate;
		initAnimation(AObject,params);
		updateObject(AObject);
		connect(AObject,SIGNAL(destroyed(QObject *)),SLOT(onObjectDestroyed(QObject *)));
	}
	else if (AObject!=NULL)
	{
		removeAutoIcon(AObject);
	}
}

void IconStorage::removeAutoIcon(QObject *AObject)
{
	if (FUpdateParams.contains(AObject))
	{
		removeObject(AObject);
		disconnect(AObject,SIGNAL(destroyed(QObject *)),this,SLOT(onObjectDestroyed(QObject *)));
	}
}

void IconStorage::initAnimation(QObject *AObject, IconUpdateParams *AParams)
{
	static const QList<QString> movieMimes = QList<QString>() << "image/gif" << "image/mng";

	removeAnimation(AParams);
	if (AParams->animate >= 0)
	{
		int iconCount = filesCount(AParams->key);
		QString file = fileFullName(AParams->key,AParams->index);
		if (iconCount > 1)
		{
			int interval = AParams->animate > 0 ? AParams->animate : fileProperty(AParams->key,ICON_STORAGE_ANIMATE_INTERVAL).toInt();
			if (interval > 0)
			{
				AParams->animation = new IconAnimateParams;
				AParams->animation->frameIndex = 0;
				AParams->animation->frameCount = iconCount;
				AParams->animation->timer->setSingleShot(false);
				AParams->animation->timer->setInterval(interval);
			}
		}
		else if (!file.isEmpty() && movieMimes.contains(fileMime(AParams->key,AParams->index)))
		{
			AParams->animation = new IconAnimateParams;
			QString key = fileCacheKey(AParams->key,AParams->index);
			AParams->animation->frames = FAnimateCache[storage()].value(key);
			if (!AParams->animation->frames.isEmpty())
				AParams->animation->frameCount = AParams->animation->frames.count();
			else
				AParams->animation->reader = new QImageReader(file);
			AParams->animation->timer->setSingleShot(true);
		}
		if (AParams->animation)
		{
			AParams->animation->timer->start(0);
			FTimerObject.insert(AParams->animation->timer,AObject);
			connect(AParams->animation->timer,SIGNAL(timeout()),SLOT(onAnimateTimer()));
		}
	}
}

void IconStorage::removeAnimation(IconUpdateParams *AParams)
{
	if (AParams && AParams->animation)
	{
		FTimerObject.remove(AParams->animation->timer);
		delete AParams->animation;
		AParams->animation = NULL;
	}
}

void IconStorage::updateObject(QObject *AObject)
{
	QIcon icon;
	IconUpdateParams *params = FUpdateParams[AObject];

	if (params->animation!=NULL && params->animation->reader!=NULL)
	{
		if (params->animation->reader->canRead())
		{
			QImage image = params->animation->reader->read();
			if (!image.isNull())
			{
				icon.addPixmap(QPixmap::fromImage(image));

				IconAnimateFrame frame;
				frame.icon = icon;
				frame.delay = params->animation->reader->nextImageDelay();
				params->animation->frames.append(frame);
				
				params->animation->frameCount = params->animation->frames.count();
				params->animation->frameIndex = params->animation->frameCount-1;
			}
			else
			{
				removeAnimation(params);
			}
		}
		else
		{
			QString key = fileCacheKey(params->key,params->index);
			if (!key.isEmpty())
				FAnimateCache[storage()].insert(key,params->animation->frames);

			delete params->animation->reader;
			params->animation->reader = NULL;
			params->animation->frameIndex = 0;
		}
	}

	if (params->animation)
	{
		if (!params->animation->frames.isEmpty())
		{
			const IconAnimateFrame &frame = params->animation->frames.at(params->animation->frameIndex);
			icon = frame.icon;
			params->animation->timer->start(frame.delay);
		}
		else
		{
			icon = getIcon(params->key,params->animation->frameIndex);
		}
	}
	else
	{
		icon = getIcon(params->key,params->index);
	}
	
	if (params->prop == "pixmap")
		AObject->setProperty(params->prop.toLatin1(),icon.pixmap(icon.availableSizes().value(0)));
	else
		AObject->setProperty(params->prop.toLatin1(),icon);
}

void IconStorage::removeObject(QObject *AObject)
{
	FObjectStorage.remove(AObject);
	IconUpdateParams *params = FUpdateParams.take(AObject);
	removeAnimation(params);
	delete params;
}

void IconStorage::onStorageChanged()
{
	FTimerObject.clear();
	for (QHash<QObject*,IconUpdateParams*>::iterator it=FUpdateParams.begin(); it!=FUpdateParams.end(); ++it)
	{
		initAnimation(it.key(),it.value());
		updateObject(it.key());
	}
}

void IconStorage::onAnimateTimer()
{
	QObject *object = FTimerObject.value(qobject_cast<QTimer *>(sender()));
	IconUpdateParams *params = FUpdateParams.value(object);
	if (params)
	{
		if (!params->animation->reader)
			params->animation->frameIndex = params->animation->frameCount>0 ? (params->animation->frameIndex + 1) % params->animation->frameCount : 0;
		updateObject(object);
	}
}

void IconStorage::onObjectDestroyed(QObject *AObject)
{
	removeObject(AObject);
}
