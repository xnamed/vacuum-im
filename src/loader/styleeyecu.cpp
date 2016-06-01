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

#include "styleeyecu.h"

StyleEyecu::StyleEyecu(QApplication *APpl): QObject(APpl)
{
#ifdef EYECU_MOBILE
	QScreen *screen = qApp->primaryScreen();
	qreal logicalDotsPerInch= screen->logicalDotsPerInch();
	qreal physicalDotsPerInch= screen->physicalDotsPerInch();
	qreal midleDotsPerInch=(logicalDotsPerInch+physicalDotsPerInch)/2;
	//! PointSizeF=96
	float newPointSizeF;
		 if(midleDotsPerInch<=110)                          {FScale=1.0; newPointSizeF= 8.0;}
	else if(midleDotsPerInch>110 && midleDotsPerInch<=160)	{FScale=1.5; newPointSizeF=12.0;}
	else if(midleDotsPerInch>160 && midleDotsPerInch<=240)	{FScale=2.0; newPointSizeF=18.0;}
	else if(midleDotsPerInch>240 && midleDotsPerInch<=320)	{FScale=2.5; newPointSizeF=20.0;}
	else if(midleDotsPerInch>320 && midleDotsPerInch<=480)	{FScale=3.0; newPointSizeF=24.0;}
	else if(midleDotsPerInch>480 && midleDotsPerInch<=640)	{FScale=3.5; newPointSizeF=28.0;}
	else if(midleDotsPerInch>640)							{FScale=4.5; newPointSizeF=36.0;}

//!---- delete Later-------
#ifdef Q_OS_WIN		//! *** To DEBUG ****
	 FScale=2.0;
	 newPointSizeF=18;
#endif
//!---- delete Later-------

	// -- select style for APP ---
	QStringList styles=QStyleFactory::keys();	// "Android","Windows", "Fusion"
	if(styles.contains("Fusion"))//Qt::CaseInsensitive
		APpl->setStyle(QStyleFactory::create("Fusion"));
	APpl->setStyleSheet(saveStyle());
	APpl->style()->setObjectName("Fusion");

	//!----------------
	QFont font = APpl->font();
	font.setPointSizeF(newPointSizeF);
	APpl->setFont(font);

	IconStorage::setScale(FScale);
	IconStorage::setFontPointSize(newPointSizeF);
#else
	IconStorage::setScale(1.0);         //!---For Q_OS_WIN OR DECKTOP---
	IconStorage::setFontPointSize(8.0); //!---For Q_OS_WIN OR DECKTOP---
#endif
}

void StyleEyecu::init()
{

}

QString StyleEyecu::saveStyle()
{
	//! Style modify
	int newSize=16*FScale;
	QString all=QString().append(checkBox(newSize))
						 .append(radioBot(newSize))
						 .append(slider(newSize))
						 .append(spinBox(newSize));
	return all;
}

QString StyleEyecu::spinBox(int ASize)
{
	Q_UNUSED(ASize)
	QString spinBoxSyle=QString("QSpinBox {max-width: 152px; }");//padding-right: 35px;max-height: 48px;
	return spinBoxSyle;
}

QString StyleEyecu::checkBox(int ASize)
{
	QString chBoxStyle=QString("QCheckBox::indicator {width: %1px; height: %2px; spacing: %3px;}")
			.arg(ASize).arg(ASize).arg(4);
	return chBoxStyle;
}

QString StyleEyecu::radioBot(int ASize)
{
	QString radBotStyle=QString("QRadioButton::indicator {width: %1px; height: %2px; spacing: %3px;}")
			.arg(ASize).arg(ASize).arg(4);
	return radBotStyle;
}

QString StyleEyecu::slider(int ASize)
{
	Q_UNUSED(ASize)
	QString sliderStyle=QString(""
		"QSlider::groove:horizontal {border: 1px 1px solid #999999;;height: 8px; background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4); margin: 2px 0;}"
		"QSlider::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #05BB04, stop:1 #07D606); border: 1px solid #039702; width: 18px; margin: -4px 0;border-radius: 3px;}"
//        "QSlider::add-page:horizontal {background: white;}"     //after
//        "QSlider::sub-page:horizontal {background: #039302;}"   //befor
	 );
	return sliderStyle;
}
