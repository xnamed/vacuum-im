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
     FPointSizeF=16;
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
	return QString().append(checkBox()).append(radioBottom()).append(slider())
			.append(spinBox().append(doubleSpinBox()));
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

QString StyleEyecu::checkBox()
{
    QString style=QString("QCheckBox::indicator {width: %1px; height: %1px; spacing: %2px;}")
            .arg(FSize).arg(FSize/8);
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
        "QSlider::groove:horizontal {border: 1px 1px solid #999999;;height: %1px;"
                "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4); margin: 2px 0;}"
        "QSlider::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #05BB04, stop:1 #07D606);"
                "border: 1px solid #039702; width: 18px; margin: -4px 0;border-radius: 3px;}")
        .arg(FSize/4);
    return style;
}
//        "QSlider::add-page:horizontal {background: white;}"     //after
//        "QSlider::sub-page:horizontal {background: #039302;}"   //befor

QString StyleEyecu::treeView()
{
    QString style;
    return style;
}
