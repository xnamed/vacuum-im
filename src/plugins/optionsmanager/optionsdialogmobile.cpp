#include "optionsdialogmobile.h"

void OptionsDialogMobile::setupUi(QDialog *OptionsDialogClass)
{
	if (OptionsDialogClass->objectName().isEmpty())
		OptionsDialogClass->setObjectName(QStringLiteral("OptionsDialogClass"));

	verticalLayout = new QVBoxLayout(OptionsDialogClass);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(5, 5, 5, 5);
	verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

	trvNodes = new QTreeView;
	trvNodes->setObjectName(QStringLiteral("trvNodes"));
	trvNodes->setEditTriggers(QAbstractItemView::NoEditTriggers);
	trvNodes->setTextElideMode(Qt::ElideNone);
	trvNodes->setIndentation(12);
	trvNodes->setSortingEnabled(true);
	trvNodes->header()->setVisible(false);
	verticalLayout->addWidget(trvNodes);

//------------
    QVBoxLayout *vBox=new QVBoxLayout;
    scaScroll = new QScrollArea;
	scaScroll->setObjectName(QStringLiteral("scaScroll"));
	scaScroll->setWidgetResizable(true);

    scrollAreaWidgetContents = new QWidget();
	scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
	scaScroll->setWidget(scrollAreaWidgetContents);
    vBox->addWidget(scaScroll);

    vSpaser=new QSpacerItem(60, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vBox->addItem(vSpaser);
//------------
	dbbButtons = new QDialogButtonBox(OptionsDialogClass);
	dbbButtons->setObjectName(QStringLiteral("dbbButtons"));
	dbbButtons->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::Reset);
	verticalLayout->addWidget(dbbButtons);

	QWidget::setTabOrder(trvNodes, dbbButtons);
	retranslateUi(OptionsDialogClass);
	QMetaObject::connectSlotsByName(OptionsDialogClass);
} // setupUi For Android

void OptionsDialogMobile::retranslateUi(QDialog *OptionsDialogClass)
{
	Q_UNUSED(OptionsDialogClass);
} // retranslateUi

OptionsDialogMobile::~OptionsDialogMobile()
{
	delete scaScroll;
}
