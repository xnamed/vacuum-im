/*!
densities:--dpi--icon--koef--font-comments--------------------------
desktop  : [96]  [16]  2.0   8	  (base)					~96dpi
ldpi	 : [120] [24]  3.0   12   (low)						~120dpi
mdpi	 : [160] [32]  4.0   14   (medium)(standard size)	~160dpi
hdpi	 : [240] [40]  5.0   16   (high)					~240dpi
xhdpi	 : [320] [80]  10.0  16   (extra-high)				~320dpi
xxhdpi	 : [480] [112] 14.0  18   (extra-extra-high)		~480dpi
xxxhdpi	 : [640] [160] 20.0  20   (extra-extra-extra-high)	~640dpi
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
#define BASE	8


/*! this file in resource!!
	QFile styleF;
	styleF.setFileName(":/qss/style.css");
	styleF.open(QFile::ReadOnly);
	QString qssStr = styleF.readAll();
	qApp->setStyleSheet(qssStr);
*/


StyleEyecu::StyleEyecu(QApplication *APpl): FAppl(APpl)
{
#ifdef EYECU_MOBILE
	QScreen *screen = qApp->primaryScreen();
	//DPI = 25.4 * resY / mmHeight
	//int DPI = 25.4*screen->physicalDotsPerInchY()/screen->size().height();
//	qreal logicalDotsPerInch= screen->logicalDotsPerInch();
	qreal physicalDotsPerInch= screen->physicalDotsPerInch();
//	qreal midleDotsPerInch=(logicalDotsPerInch+physicalDotsPerInch)/2;
	qreal DPI=physicalDotsPerInch;     // Variant - 2
	//! PointSizeF=96
	if(DPI<=110)                    {FScale=2.0;  FPointSizeF= 8.0;} // desktop
	else if(DPI>110 && DPI<=160)	{FScale=3.0;  FPointSizeF=12.0;} // ldpi
	else if(DPI>160 && DPI<=240)	{FScale=4.0;  FPointSizeF=14.0;} // mdpi
	else if(DPI>240 && DPI<=320)	{FScale=5.0;  FPointSizeF=18.0;} // hdpi
	else if(DPI>320 && DPI<=480)	{FScale=10.0; FPointSizeF=18.0;} // xhdpi
	else if(DPI>480 && DPI<=640)	{FScale=14.0; FPointSizeF=18.0;} // xxhdpi
	else if(DPI>640)				{FScale=20.0; FPointSizeF=20.0;} // xxxhdpi

//!---- delete Later-------
#ifdef Q_OS_WIN		//! *** To DEBUG ****
	 FScale=4.0;
	 FPointSizeF=16;
#endif
//!---- delete Later-------

//!--------------------test---------------
/*
	Dialog *dg=new Dialog(FScale,FPointSizeF);
    if(dg->exec())
    {
        FScale=dg->getScale();
        FPointSizeF=dg->getFontSize();
    }else{

    }
*/
//!--------------------test---------------

	FSize=BASE*FScale;
	IconStorage::setScale(FScale);
    IconStorage::setFontPointSize(FPointSizeF);
#else
	IconStorage::setScale(1.0);         //!---For Q_OS_WIN OR DECKTOP---
	IconStorage::setFontPointSize(8.0); //!---For Q_OS_WIN OR DECKTOP---
#endif
}

//!
//! \brief StyleEyecu::init
//!
void StyleEyecu::init()
{
#ifdef EYECU_MOBILE
    QStringList styles=QStyleFactory::keys();	// "Android","Windows", "Fusion"
    if(styles.contains("Fusion"))               //Qt::CaseInsensitive
		FAppl->setStyle(QStyleFactory::create("Fusion"));
	FAppl->setStyleSheet(saveStyle());
	FAppl->style()->setObjectName("Fusion");

	QFont font = FAppl->font();
	font.setPointSizeF(FPointSizeF);
	//  pixelSize = DPI * pointSize / 72
	FAppl->setFont(font);
#endif
}


//! Base color #039702
//!
//! \brief StyleEyecu::saveStyle
//! \return
//!
QString StyleEyecu::saveStyle()
{
    return QString().append(checkBox()).append(radioBottom()).append(slider()).append(scrollBar())
            .append(spinBox()).append(doubleSpinBox()).append(treeWidget()).append(treeView())
			.append(comboBox());//.append(groupBox());
}

//!
//! \brief StyleEyecu::spinBox
//! \return
//!
QString StyleEyecu::spinBox()
{
    QString style=QString(""
		"QSpinBox {padding-right: %1px; padding-left: %1px; border-image: url(:spinframe.png) 3; border-width: 3;}"
		"QSpinBox::up-button {subcontrol-origin: border; subcontrol-position: right; width: %2px; height: %2px;"
							 "border-image: url(:spinup.png) 1; border-width: 2px;}"
        "QSpinBox::up-button:hover {border-image: url(:spinup_hover.png) 1;}"
        "QSpinBox::up-button:pressed {border-image: url(:spinup_pressed.png) 1;}"
		"QSpinBox::up-arrow {border-image: url(:spinup_arrow.png) 1;}"
		"QSpinBox::up-arrow:disabled, QSpinBox::up-arrow:off {border-image: url(:spinup_arrow_disabled.png) 1;}"
		"QSpinBox::down-button {subcontrol-origin: border; subcontrol-position: left; width: %3px; height: %3px;"
							  "border-image: url(:spindown.png) 1; border-width: 2px;}"
        "QSpinBox::down-button:hover {border-image: url(:spindown_hover.png) 1;}"
        "QSpinBox::down-button:pressed {border-image: url(:spindown_pressed.png) 1;}"
		"QSpinBox::down-arrow {border-image: url(:spindown_arrow.png) 1;}"
		"QSpinBox::down-arrow:disabled,QSpinBox::down-arrow:off {border-image: url(:spindown_arrow_disabled.png) 1;}")
		.arg(FSize/4).arg(FSize).arg(FSize);
	return style;
}
//!
//! \brief StyleEyecu::doubleSpinBox
//! \return
//!
QString StyleEyecu::doubleSpinBox()
{
	QString style=QString(""
		"QDoubleSpinBox {padding-right: %1px; padding-left: %1px; border-image: url(:spinframe.png) 3; border-width: 3;}"
		"QDoubleSpinBox::up-button {subcontrol-origin: border; subcontrol-position: right; width: %2px; height: %2px;"
							 "border-image: url(:spinup.png) 1; border-width: 2px;}"
		"QDoubleSpinBox::up-button:hover {border-image: url(:spinup_hover.png) 1;}"
		"QDoubleSpinBox::up-button:pressed {border-image: url(:spinup_pressed.png) 1;}"
		"QDoubleSpinBox::up-arrow {border-image: url(:spinup_arrow.png) 1;}"
		"QDoubleSpinBox::up-arrow:disabled, QDoubleSpinBox::up-arrow:off {border-image: url(:spinup_arrow_disabled.png) 1;}"
		"QDoubleSpinBox::down-button {subcontrol-origin: border; subcontrol-position: left; width: %3px; height: %3px;"
							  "border-image: url(:spindown.png) 1; border-width: 2px;}"
		"QDoubleSpinBox::down-button:hover {border-image: url(:spindown_hover.png) 1;}"
		"QDoubleSpinBox::down-button:pressed {border-image: url(:spindown_pressed.png) 1;}"
		"QDoubleSpinBox::down-arrow {border-image: url(:spindown_arrow.png) 1;}"
		"QDoubleSpinBox::down-arrow:disabled,QDoubleSpinBox::down-arrow:off {border-image: url(:spindown_arrow_disabled.png) 1;}")
		.arg(FSize/4).arg(FSize).arg(FSize);
	return style;
}

/*! Base size = 8px */
//!
//! \brief StyleEyecu::checkBox
//! \return
//!
QString StyleEyecu::checkBox()
{
    QString style=QString(""
        "QCheckBox::indicator {width: %1px; height: %1px; spacing: %2px; background-color: white;}"
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

//!
//! \brief StyleEyecu::comboBox
//! \return
//!
QString StyleEyecu::comboBox()
{
	QString style=QString(""
        "QComboBox::down-arrow {border-image: url(:cmb-down-arrow.png);}"
        "QComboBox::drop-down {border-left: 1px solid grey;}"
    );
	return style;
}

//!
//! \brief StyleEyecu::radioBottom
//! \return
//!
QString StyleEyecu::radioBottom()
{
    QString style=QString("QRadioButton::indicator {width: %1px; height: %1px; spacing: %2px;}")
            .arg(FSize).arg(FSize/8);
    return style;
}

//!
//! \brief StyleEyecu::slider
//! \return
//!
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
//        "QSlider::sub-page:horizontal {background: #039302;}"   //befor 039702

//!
//! \brief StyleEyecu::treeWidget
//! \return
//!
QString StyleEyecu::treeWidget()
{
	QString style=QString(""
        "QTreeWidget::branch:has-siblings:!adjoins-item {border-image: url(:vline.png) 1;}"
        "QTreeWidget::branch:has-siblings:adjoins-item {border-image: url(:branch-more.png) 1;}"
        "QTreeWidget::branch:!has-children:!has-siblings:adjoins-item {border-image: url(:branch-end.png) 1;}"
		"QTreeWidget::branch:has-children:!has-siblings:closed,"
		"QTreeWidget::branch:closed:has-children:has-siblings {border-image: url(:branch-closed.png) 1;}"
		"QTreeWidget::branch:open:has-children:!has-siblings,"
		"QTreeWidget::branch:open:has-children:has-siblings {border-image: url(:branch-open.png) 1;}");
	return style;
}

//!
//! \brief StyleEyecu::treeView
//! \return
//!
QString StyleEyecu::treeView()
{
	QString style=QString(""
        "QTreeView::branch:has-siblings:!adjoins-item {border-image: url(:vline.png) 1;}"
        "QTreeView::branch:has-siblings:adjoins-item {border-image: url(:branch-more.png) 1;}"
        "QTreeView::branch:!has-children:!has-siblings:adjoins-item {border-image: url(:branch-end.png) 1;}"
		"QTreeView::branch:has-children:!has-siblings:closed,"
		"QTreeView::branch:closed:has-children:has-siblings {border-image: url(:branch-closed.png) 1;}"
		"QTreeView::branch:open:has-children:!has-siblings,"
		"QTreeView::branch:open:has-children:has-siblings {border-image: url(:branch-open.png) 1;}");
	return style;
}// image: none;

//!
//! \brief StyleEyecu::progressBar
//! \return
//!
QString StyleEyecu::progressBar()
{
    //A margin can be included to obtain more visible chunks.
	QString style=QString("QProgressBar::chunk {background-color: #CD96CD; width: 10px; margin: 0.5px;}");
	return style;
}

/*! Base size = 10 px*/
//!
//! \brief StyleEyecu::scrollBar
//! \return
//!
QString StyleEyecu::scrollBar()
{
    QString style=QString(""
        "QScrollBar:vertical {border: %2px solid #%4; background: #%5; width: %1px; margin-top: %1px; margin-bottom: %1px;}"
        "QScrollBar::handle:vertical {background: #%6; min-height: %3px;}"
        "QScrollBar::add-line:vertical {height: %1px; subcontrol-position: bottom; subcontrol-origin: margin;}"
        "QScrollBar::sub-line:vertical {height: %1px; subcontrol-position: top; subcontrol-origin: margin;}"
		"QScrollBar::up-arrow:vertical {border-image: url(:arrow_up.png) 1;}"
		"QScrollBar::down-arrow:vertical {border-image: url(:arrow_down.png) 1;}"
        "QScrollBar:horizontal {border: %2px solid #%4; background: #%5; height: %1px; margin-right: %1px; margin-left: %1px;}"
        "QScrollBar::handle:horizontal {background: #%6; min-width: %3px;}"
        "QScrollBar::add-line:horizontal {width: %1px; subcontrol-position: right; subcontrol-origin: margin;}"
        "QScrollBar::sub-line:horizontal {width: %1px; subcontrol-position: left; subcontrol-origin: margin;}"
		"QScrollBar::left-arrow:horizontal {border-image: url(:arrow_left.png) 1;}"
		"QScrollBar::right-arrow:horizontal {border-image: url(:arrow_right.png) 1;}"
		).arg(5*FScale).arg(FScale/2).arg(FScale).arg("919086").arg("039702").arg("F6F4E9");
	return style;
}

///!
//! \brief StyleEyecu::groupBox
//! \return
//!
QString StyleEyecu::groupBox()
{
		QString style=QString(""
			"QGroupBox::indicator {width: %1px; height: %1px;}").arg(FSize);
		return style;
}


/* 039702  //old - 919086  C2C0B5  F6F4E9
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
