/*!
densities:--dpi--icon--koef--font-comments--------------------------
desktop  : [96]  [16]  1.0   8	  (base)					~96dpi
ldpi	 : [120] [24]  1,5   12   (low)						~120dpi
mdpi	 : [160] [32]  2.0   16   (medium)(standard size)	~160dpi
hdpi	 : [240] [40]  2.5   20   (high)					~240dpi
xhdpi	 : [320] [48]  3.0   24   (extra-high)				~320dpi
xxhdpi	 : [480] [56]  3.5   28   (extra-extra-high)		~480dpi
xxxhdpi	 : [640] [72]  4.5   36   (extra-extra-extra-high)	~640dpi
--------------------------------------------------------------------
							   [height x width]
1. xlarge screens are at least [960dp  x 720dp]
2. large  screens are at least [640dp  x 480dp]
3. normal screens are at least [470dp  x 320dp]
4. small  screens are at least [426dp  x 320dp]
--------------------------------------------------------------------
*/

#include <QDebug>
#include <QDir>

#include "styleeyecu.h"

StyleEyecu::StyleEyecu(QApplication *APpl): FAPpl(APpl)
{
#ifdef EYECU_MOBILE
	QScreen *screen = qApp->primaryScreen();
	qreal logicalDotsPerInch= screen->logicalDotsPerInch();
	qreal physicalDotsPerInch= screen->physicalDotsPerInch();
	qreal midleDotsPerInch=(logicalDotsPerInch+physicalDotsPerInch)/2;
	//! PointSizeF=96
    if(midleDotsPerInch<=110)                               {FScale=1.0; FPointSizeF= 8.0;}
    else if(midleDotsPerInch>110 && midleDotsPerInch<=160)	{FScale=1.5; FPointSizeF=12.0;}
    else if(midleDotsPerInch>160 && midleDotsPerInch<=240)	{FScale=2.0; FPointSizeF=18.0;}
    else if(midleDotsPerInch>240 && midleDotsPerInch<=320)	{FScale=2.5; FPointSizeF=20.0;}
    else if(midleDotsPerInch>320 && midleDotsPerInch<=480)	{FScale=3.0; FPointSizeF=24.0;}
    else if(midleDotsPerInch>480 && midleDotsPerInch<=640)	{FScale=3.5; FPointSizeF=28.0;}
    else if(midleDotsPerInch>640)							{FScale=4.5; FPointSizeF=36.0;}

//!---- delete Later-------
#ifdef Q_OS_WIN		//! *** To DEBUG ****
     FScale=2.0;
     FPointSizeF=18;
#endif
//!---- delete Later-------

    FSize=16*FScale;
	IconStorage::setScale(FScale);
    IconStorage::setFontPointSize(FPointSizeF);
#else
	IconStorage::setScale(1.0);         //!---For Q_OS_WIN OR DECKTOP---
	IconStorage::setFontPointSize(8.0); //!---For Q_OS_WIN OR DECKTOP---
#endif
}

void StyleEyecu::init()
{
#ifdef EYECU_MOBILE
    QStringList styles=QStyleFactory::keys();	// "Android","Windows", "Fusion"
    if(styles.contains("Fusion"))               //Qt::CaseInsensitive
        FAPpl->setStyle(QStyleFactory::create("Fusion"));
    FAPpl->setStyleSheet(saveStyle());
    FAPpl->style()->setObjectName("Fusion");

    QFont font = FAPpl->font();
    font.setPointSizeF(FPointSizeF);
    FAPpl->setFont(font);
#endif
}

//! Style modify
QString StyleEyecu::saveStyle()
{
    return QString().append(checkBox()).append(radioBottom()).append(slider()).append(scrollBar())
            .append(spinBox()).append(doubleSpinBox()).append(treeWidget()).append(treeView());
}

QString StyleEyecu::spinBox()
{
    QString style=QString(""
        "QSpinBox {padding-right: %1px; padding-left: %2px; border-image: url(:spinframe.png) 3; border-width: 3;}"
        "QSpinBox::up-button {subcontrol-origin: border; subcontrol-position: right; width: %1px; height: %1px;"
                             "border-image: url(:spinup.png) 1; border-width: 1px;}"
        "QSpinBox::up-button:hover {border-image: url(:spinup_hover.png) 1;}"
        "QSpinBox::up-button:pressed {border-image: url(:spinup_pressed.png) 1;}"
        "QSpinBox::up-arrow {image: url(:spinup_arrow.png) 1;width: %2px; height: %2px;}"
        "QSpinBox::up-arrow:disabled, QSpinBox::up-arrow:off {image: url(:spinup_arrow_disabled.png);}"
        "QSpinBox::down-button {subcontrol-origin: border; subcontrol-position: left; width: %1px; height: %1px;"
                              "border-image: url(:spindown.png) 1; border-width: 1px;}"
        "QSpinBox::down-button:hover {border-image: url(:spindown_hover.png) 1;}"
        "QSpinBox::down-button:pressed {border-image: url(:spindown_pressed.png) 1;}"
        "QSpinBox::down-arrow {image: url(:spindown_arrow.png) 1;width: %2px; height: %2px;}"
        "QSpinBox::down-arrow:disabled,QSpinBox::down-arrow:off {image: url(:spindown_arrow_disabled.png);}")
        .arg(FSize).arg(FSize/2);
	return style;
}

QString StyleEyecu::doubleSpinBox()
{
	QString style=QString(""
		"QDoubleSpinBox {padding-right: %1px; padding-left: %2px; border-image: url(:spinframe.png) 3; border-width: 3;}"
		"QDoubleSpinBox::up-button {subcontrol-origin: border; subcontrol-position: right; width: %1px; height: %1px;"
							 "border-image: url(:spinup.png) 1; border-width: 1px;}"
		"QDoubleSpinBox::up-button:hover {border-image: url(:spinup_hover.png) 1;}"
		"QDoubleSpinBox::up-button:pressed {border-image: url(:spinup_pressed.png) 1;}"
		"QDoubleSpinBox::up-arrow {image: url(:spinup_arrow.png) 1;width: %2px; height: %2px;}"
		"QDoubleSpinBox::up-arrow:disabled, QDoubleSpinBox::up-arrow:off {image: url(:spinup_arrow_disabled.png);}"
		"QDoubleSpinBox::down-button {subcontrol-origin: border; subcontrol-position: left; width: %1px; height: %1px;"
							  "border-image: url(:spindown.png) 1; border-width: 1px;}"
		"QDoubleSpinBox::down-button:hover {border-image: url(:spindown_hover.png) 1;}"
		"QDoubleSpinBox::down-button:pressed {border-image: url(:spindown_pressed.png) 1;}"
		"QDoubleSpinBox::down-arrow {image: url(:spindown_arrow.png) 1;width: %2px; height: %2px;}"
		"QDoubleSpinBox::down-arrow:disabled,QDoubleSpinBox::down-arrow:off {image: url(:spindown_arrow_disabled.png);}")
		.arg(FSize).arg(FSize/2);
	return style;
}

/*! Base size = 16px*/
QString StyleEyecu::checkBox()
{
    QString style=QString(""
        "QCheckBox::indicator {width: %1px; height: %1px; spacing: %2px;}"
        "QCheckBox::indicator:unchecked {border-image: url(:checkbox_unchecked.png);}"
        "QCheckBox::indicator:unchecked:hover {border-image: url(:checkbox_unchecked_hover.png);}"
        "QCheckBox::indicator:unchecked:pressed {border-image: url(:checkbox_unchecked_pressed.png);}"
        "QCheckBox::indicator:checked {border-image: url(:checkbox_checked.png);}"
        "QCheckBox::indicator:checked:hover {border-image: url(:checkbox_checked_hover.png);}"
        "QCheckBox::indicator:checked:pressed {border-image: url(:checkbox_checked_pressed.png);}"
        "QCheckBox::indicator:unchecked:disabled {border-image: url(:checkbox_unchecked_disabled.png);}"
        "QCheckBox::indicator:checked:disabled {border-image: url(:checkbox_checked_disabled.png);}"
         ).arg(FSize).arg(FSize/8);
    return style;
}

QString StyleEyecu::radioBottom()
{
    QString style=QString("QRadioButton::indicator {width: %1px; height: %1px; spacing: %2px;}")
            .arg(FSize).arg(FSize/8);
    return style;
}

QString StyleEyecu::slider()
{
    QString style=QString(""
        "QSlider::groove:horizontal {border: 1px 1px solid #999999;height: %1px;"
                "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4); margin: 2px 0;}"
        "QSlider::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #05BB04, stop:1 #07D606);"
                "border: 1px solid #039702; width: 18px; margin: -4px 0;border-radius: 3px;}"
        ).arg(FSize/4);
    return style;
}
//        "QSlider::add-page:horizontal {background: white;}"     //after
//        "QSlider::sub-page:horizontal {background: #039302;}"   //befor

QString StyleEyecu::treeWidget()
{
	QString style=QString(""
        "QTreeWidget::branch:has-siblings:!adjoins-item {border-image: url(:vline.png) 1;}"
        "QTreeWidget::branch:has-siblings:adjoins-item {border-image: url(:branch-more.png) 1;}"
        "QTreeWidget::branch:!has-children:!has-siblings:adjoins-item {border-image: url(:branch-end.png) 1;}"
		"QTreeWidget::branch:has-children:!has-siblings:closed,"
        "QTreeWidget::branch:closed:has-children:has-siblings {border-image: none;image: url(:branch-closed.png) 1;}"
		"QTreeWidget::branch:open:has-children:!has-siblings,"
        "QTreeWidget::branch:open:has-children:has-siblings{border-image: none;image: url(:branch-open.png) 1;}")
        .arg(FSize);
	return style;
}

QString StyleEyecu::treeView()
{
	QString style=QString(""
        "QTreeView::branch:has-siblings:!adjoins-item {border-image: url(:vline.png) 1;}"
        "QTreeView::branch:has-siblings:adjoins-item {border-image: url(:branch-more.png) 1;}"
        "QTreeView::branch:!has-children:!has-siblings:adjoins-item {border-image: url(:branch-end.png) 1;}"
		"QTreeView::branch:has-children:!has-siblings:closed,"
        "QTreeView::branch:closed:has-children:has-siblings {border-image: none;image: url(:branch-closed.png) 1;}"
		"QTreeView::branch:open:has-children:!has-siblings,"
        "QTreeView::branch:open:has-children:has-siblings{border-image: none;image: url(:branch-open.png) 1;}")
        .arg(FSize);
	return style;
}


QString StyleEyecu::progressBar()
{
    //A margin can be included to obtain more visible chunks.
	QString style=QString("QProgressBar::chunk {background-color: #CD96CD; width: 10px; margin: 0.5px;}");
	return style;
}

/*! Base size = 10 px*/
QString StyleEyecu::scrollBar()
{
    QString style=QString(""
        "QScrollBar:vertical {border: %2px solid #%4; background: #%5; width: %1px; margin-top: %1px; margin-bottom: %1px;}"
        "QScrollBar::handle:vertical {background: #%6; min-height: %3px;}"
        "QScrollBar::add-line:vertical {height: %1px; subcontrol-position: bottom; subcontrol-origin: margin;}"
        "QScrollBar::sub-line:vertical {height: %1px; subcontrol-position: top; subcontrol-origin: margin;}"
        "QScrollBar:horizontal {border: %2px solid #%4; background: #%5; height: %1px; margin-right: %1px; margin-left: %1px;}"
        "QScrollBar::handle:horizontal {background: #%6; min-width: %3px;}"
        "QScrollBar::add-line:horizontal {width: %1px; subcontrol-position: right; subcontrol-origin: margin;}"
        "QScrollBar::sub-line:horizontal {width: %1px; subcontrol-position: left; subcontrol-origin: margin;}"
        ).arg(10*FScale).arg(FScale).arg(10*2*FScale).arg("919086").arg("C2C0B5").arg("F6F4E9");
	return style;
}
/*
    "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { border: 2px solid #919086; width: 3px;height: 3px;background:white;}"
    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: none;}"

*/

/*
#define SPINFRAME           "spinframe"
#define SPINUP              "spinup"
#define SPINUPHOVER         "spinup_hover"
#define SPINUPPRESS         "spinup_pressed"
#define SPINUPARROW         "spinup_arrow"
#define SPINUPARROWDIS      "spinup_arrow_dis"
#define SPINUPOFF           "spinup_off"
#define SPINDOWN            "spindown"
#define SPINDOWNHOVER       "spindown_hover"
#define SPINDOWNPRESS       "spindown_pressed"
#define SPINDOWNARROW       "spindown_arrow"
#define SPINDOWNARROWDIS    "spindown_arrow_dis"
#define SPINDOWNOFF         "spindown_off"

#define SPINFRAME           spinframe.png
#define SPINUP              spinup.png
#define SPINUPHOVER         spinup_hover.png
#define SPINUPPRESS         spinup_pressed.png
#define SPINUPARROW         spinup_arrow.png
#define SPINUPARROWDIS      spinup_arrow_dis.png
#define SPINUPOFF           spinup_off.png
#define SPINDOWN            spindown.png
#define SPINDOWNHOVER       spindown_hover.png
#define SPINDOWNPRESS       spindown_pressed.png
#define SPINDOWNARROW       spindown_arrow.png
#define SPINDOWNARROWDIS    spindown_arrow_dis.png
#define SPINDOWNOFF         spindown_off.png

#define CHBOXUNCHECKED		checkbox_unchecked.png
#define CHBOXUNCHHOVER		checkbox_unchecked_hover.png
#define CHBOXUNCHPRESS		checkbox_unchecked_pressed.png
#define CHBOXCHECKED		checkbox_checked.png
#define CHBOXCHECHOVER		checkbox_checked_hover.png
#define CHBOXCHECPRESS		checkbox_checked_pressed.png
#define CHBOXITREMHOVER		checkbox_indeterminate_hover.png
#define CHBOXITERMPRESS		checkbox_indeterminate_pressed.png
*/
