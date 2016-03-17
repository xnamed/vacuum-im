#include "newoptionsdialogclass.h"

void Ui_NewOptionsDialogClass::setupUi(QDialog *OptionsDialogClass)
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

    scaScroll = new QScrollArea;
	scaScroll->setObjectName(QStringLiteral("scaScroll"));
	scaScroll->setWidgetResizable(true);

    scrollAreaWidgetContents = new QWidget();
	scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
	scaScroll->setWidget(scrollAreaWidgetContents);

	dbbButtons = new QDialogButtonBox(OptionsDialogClass);
	dbbButtons->setObjectName(QStringLiteral("dbbButtons"));
	dbbButtons->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::Reset);
	verticalLayout->addWidget(dbbButtons);

	QWidget::setTabOrder(trvNodes, dbbButtons);
	retranslateUi(OptionsDialogClass);
	QMetaObject::connectSlotsByName(OptionsDialogClass);
} // setupUi For Android

void Ui_NewOptionsDialogClass::retranslateUi(QDialog *OptionsDialogClass)
{
	Q_UNUSED(OptionsDialogClass);
} // retranslateUi

Ui_NewOptionsDialogClass::~Ui_NewOptionsDialogClass()
{
    delete scaScroll;
}
