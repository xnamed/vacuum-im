#ifndef IMAP_H
#define IMAP_H

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QVariant>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QRectF>

#include <QtGeo/mercatorcoordinates.h>
#include <QtGeo/mapcoordinates.h>
#include <QtGeo/geomap.h>
#include <interfaces/ioptionsmanager.h>
#include <utils/action.h>
#include <definitions/mapobjecttyperole.h>
#include <definitions/mapobjectdatarole.h>

#define MAP_UUID "{3d5702bc-29b9-40f2-88fe-85887cd6d8d2}"

#define  ICON_SATELLITE 0
#define  ICON_HYBRID    1
#define  ICON_HYBRID1   2
#define  ICON_MAP       3
#define  ICON_MAP1      4
#define  ICON_MAP2      5
#define  ICON_TERRAIN   6

#define  UNCHANGED -1

enum setSearchMenu {
        MENU_MAP,
        MENU_ALL,
        MENU_TOOLBAR
};

class IMapSource
{
public:
	virtual QObject			*instance()=0;
	virtual MapSource		*mapSource() = 0;
	virtual QList<int>      getModeIcons() const = 0;
	virtual QString         getIconId() const = 0;
};

class IMapMouseGrabber
{
public:
	virtual bool mapMouseMoved(QPointF ANewPosition, QPointF AOldPosition, MercatorCoordinates ACoordinates, Qt::MouseButtons AMouseButtons) = 0;
	virtual bool mapMouseClicked(MercatorCoordinates ACoordinates, Qt::MouseButton AButton) = 0;
	virtual bool mapMouseDoubleClicked(MercatorCoordinates ACoordinates, Qt::MouseButton AButton) = 0;
	virtual bool mapMouseWheelMoved(const QPointF &AScenePosition, int ADelta) = 0;
	virtual bool mapContextMenu(MercatorCoordinates ACoordinates, Menu *AMenu) = 0;
};

class IMap
{
public:
	virtual QObject *instance()	= 0;
	virtual GeoMap  *geoMap()	= 0;

	virtual bool showObject(int AType, const QString &AId, bool AFollow=false, bool AShowMap=true) = 0;
	// Stop following map object
	virtual void stopFollowing() = 0;

	virtual Action *addMenuAction(const QString &AText, const QString &AIcon, const QString &AKeyIcon, int ASomething) =0;
	virtual void removeMenuAction(Action *AAction) = 0;
	virtual void insertWindowShortcut(const QString &AId) const = 0;
	virtual void removeWindowShortcut(const QString &AId) const = 0;
	virtual void showMap(bool AShow=true, bool AActivate=true) = 0;
	virtual bool isVisible() const = 0;
	virtual bool isActive() const = 0;
	virtual IMapSource *getMapSource(int AIndex = -1) = 0;
	virtual void insertOptionsDialogNode(const IOptionsDialogNode &ANode) = 0;

	//TODO: Remove
	virtual const MercatorCoordinates &menuPopupPosition() const =0;
	virtual const MercatorCoordinates &mapCenter() const =0;

	virtual bool setMouseGrabber(IMapMouseGrabber *AMouseGrabber, bool AShowCoordinates=false) = 0;
	virtual bool releaseMouseGrabber(IMapMouseGrabber *AMouseGrabber) = 0;
	virtual bool isGrabbingMouseEvents() const = 0;

	// Map scene manipulations
	virtual QRect sceneRect() const = 0;
	virtual void updateScene() = 0;

	// Map graphics view mouse cursor manipulations
	virtual void setCursor(const QCursor &ACursor) = 0;
	virtual QCursor cursor() const = 0;
	virtual void unsetCursor() = 0;
};

Q_DECLARE_INTERFACE(IMapMouseGrabber, "RWS.Plugin.IMapMaouseGrabber/1.0")
Q_DECLARE_INTERFACE(IMapSource, "RWS.Plugin.IMapSource/1.0")
Q_DECLARE_INTERFACE(IMap, "RWS.Plugin.IMap/1.0")

#endif	//IMAP_H
