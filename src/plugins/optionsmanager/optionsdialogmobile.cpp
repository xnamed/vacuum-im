#include <QDebug>
#include <QPainter>
#include "optionsdialogmobile.h"
#include <definitions/optionvalues.h>
#include <utils/iconstorage.h>		// *** <<< eyeCU <<< ***
#include <utils/options.h>			// *** <<< eyeCU <<< ***

OptionsDialogMobile::~OptionsDialogMobile(){delete scaScroll;}

void OptionsDialogMobile::setupUi(QDialog *OptionsDialogClass)
{
	if (OptionsDialogClass->objectName().isEmpty())
		OptionsDialogClass->setObjectName(QStringLiteral("OptionsDialogClass"));

	verticalLayout = new QVBoxLayout(OptionsDialogClass);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(5, 5, 5, 5);
	verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

//    trvNodes = new QTreeView;
    trvNodes = new OptionsTreeView;       //! -- New Variant with mouse ---
	trvNodes->setObjectName(QStringLiteral("trvNodes"));
	trvNodes->setEditTriggers(QAbstractItemView::NoEditTriggers);
	trvNodes->setTextElideMode(Qt::ElideNone);
    trvNodes->setIndentation(12);
    trvNodes->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	trvNodes->setSortingEnabled(true);
    trvNodes->header()->setVisible(false);
	verticalLayout->addWidget(trvNodes);

	dbbButtons = new QDialogButtonBox(OptionsDialogClass);
    dbbButtons->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	dbbButtons->setObjectName(QStringLiteral("dbbButtons"));
//	dbbButtons->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::Reset);
    dbbButtons->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Reset);

    verticalLayout->addWidget(dbbButtons);

//! ---for scaScroll ---- OWO_OPTION_SCRLBAR
	scaScroll = new OptionsScrollArea;
	scaScroll->setObjectName(QStringLiteral("scaScroll"));

    QWidget *scrollAreaWidgetContents = new QWidget();
	scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
    scaScroll->setWidget(scrollAreaWidgetContents);

	QWidget::setTabOrder(trvNodes, dbbButtons);
	retranslateUi(OptionsDialogClass);
	QMetaObject::connectSlotsByName(OptionsDialogClass);
} // setupUi For Android

void OptionsDialogMobile::retranslateUi(QDialog *OptionsDialogClass)
{
	Q_UNUSED(OptionsDialogClass);
} //! retranslateUi


//------------
//!
//! \brief OptionsScrollArea::OptionsScrollArea
//!
OptionsScrollArea::OptionsScrollArea()
{
	setWidgetResizable(true);
	bool statusPolicy=Options::node(OPV_OPTION_SCRLBAR).value().toBool();
	setVerticalScrollBarPolicy(statusPolicy ? Qt::ScrollBarAsNeeded :Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(statusPolicy ? Qt::ScrollBarAsNeeded :Qt::ScrollBarAlwaysOff);
    FPressedPos = QPoint();
}

OptionsScrollArea::~OptionsScrollArea(){}

void OptionsScrollArea::mousePressEvent(QMouseEvent *AEvent)
{
	FPressedPos=AEvent->pos();
    FScrollBarValue=verticalScrollBar()->value();  //scroll->value();
	QScrollArea::mousePressEvent(AEvent);
}

void OptionsScrollArea::mouseMoveEvent(QMouseEvent *AEvent)
{
	if (!FPressedPos.isNull()){
        verticalScrollBar()->setValue(FScrollBarValue+FPressedPos.ry()-AEvent->pos().ry());
        QScrollArea::mouseMoveEvent(AEvent);
	}
    else {
        QScrollArea::mouseMoveEvent(AEvent);
	}
}

void OptionsScrollArea::mouseReleaseEvent(QMouseEvent *AEvent)
{
    FPressedPos = QPoint();
    QScrollArea::mouseReleaseEvent(AEvent);
} //! OptionsScrollArea


//!
//! \brief OptionsTreeView::OptionsTreeView
//!
OptionsTreeView::OptionsTreeView()
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    FPressedPos = QPoint();
}

OptionsTreeView::~OptionsTreeView()
{

}

void OptionsTreeView::mousePressEvent(QMouseEvent *AEvent)
{
    FPressedPos=AEvent->pos();
    FScrollBarValue=verticalScrollBar()->value();  //scroll->value();
    QTreeView::mousePressEvent(AEvent);
}

void OptionsTreeView::mouseMoveEvent(QMouseEvent *AEvent)
{
    if (!FPressedPos.isNull()){
        verticalScrollBar()->setValue(FScrollBarValue+FPressedPos.ry()-AEvent->pos().ry());
        QTreeView::mouseMoveEvent(AEvent);
    }
    else {
        QTreeView::mouseMoveEvent(AEvent);
    }
}

void OptionsTreeView::mouseReleaseEvent(QMouseEvent *AEvent)
{
    FPressedPos = QPoint();
    QTreeView::mouseReleaseEvent(AEvent);
} //! OptionsTreeView


//!
//! \brief LineOnWidget::LineOnWidget
//! \param parent
//!
LineOnWidget::LineOnWidget(QLabel *parent) :
FColorLine(Qt::darkGreen),
FcurWidth(1)
{
	Q_UNUSED (parent)
}

void LineOnWidget::paintEvent(QPaintEvent *AEvent){
	QWidget::paintEvent(AEvent);
    setFixedHeight(FcurWidth);
	QPainter p(this);
    p.setPen(QPen(FColorLine,FcurWidth,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
	p.drawLine(0,0,width(),0);
} //! LineOnWidget
