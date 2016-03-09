#include "newoptionsdialogclass.h"

#ifdef Q_OS_ANDROID
void Ui_NewOptionsDialogClass::setupUi(QDialog *OptionsDialogClass)
{
	if (OptionsDialogClass->objectName().isEmpty())
		OptionsDialogClass->setObjectName(QStringLiteral("OptionsDialogClass"));
	OptionsDialogClass->resize(360,400);
//------------
	verticalLayout = new QVBoxLayout(OptionsDialogClass);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(5, 5, 5, 5);
	verticalLayout->setObjectName(QStringLiteral("verticalLayout"));

	sprSplitter = new QSplitter(OptionsDialogClass);
	sprSplitter->setObjectName(QStringLiteral("sprSplitter"));

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
	scaScroll->resize(540,400);
	scaScroll->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Maximum);

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
} // setupUi

Ui_NewOptionsDialogClass::~Ui_NewOptionsDialogClass()
{
	delete scaScroll;
}

#else
void Ui_NewOptionsDialogClass::setupUi(QDialog *OptionsDialogClass)
{
	if (OptionsDialogClass->objectName().isEmpty())
		OptionsDialogClass->setObjectName(QStringLiteral("OptionsDialogClass"));
	OptionsDialogClass->resize(508, 498);
	verticalLayout = new QVBoxLayout(OptionsDialogClass);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(5, 5, 5, 5);
	verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
	sprSplitter = new QSplitter(OptionsDialogClass);
	sprSplitter->setObjectName(QStringLiteral("sprSplitter"));
	sprSplitter->setOrientation(Qt::Horizontal);
	trvNodes = new QTreeView(sprSplitter);
	trvNodes->setObjectName(QStringLiteral("trvNodes"));
	trvNodes->setEditTriggers(QAbstractItemView::NoEditTriggers);
	trvNodes->setTextElideMode(Qt::ElideNone);
	trvNodes->setIndentation(12);
	trvNodes->setSortingEnabled(true);
	sprSplitter->addWidget(trvNodes);
	trvNodes->header()->setVisible(false);
	scaScroll = new QScrollArea(sprSplitter);
	scaScroll->setObjectName(QStringLiteral("scaScroll"));
	scaScroll->setWidgetResizable(true);
	scrollAreaWidgetContents = new QWidget();
	scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
	scrollAreaWidgetContents->setGeometry(QRect(0, 0, 70, 453));
	scaScroll->setWidget(scrollAreaWidgetContents);
	sprSplitter->addWidget(scaScroll);

	verticalLayout->addWidget(sprSplitter);

	dbbButtons = new QDialogButtonBox(OptionsDialogClass);
	dbbButtons->setObjectName(QStringLiteral("dbbButtons"));
	dbbButtons->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok|QDialogButtonBox::Reset);

	verticalLayout->addWidget(dbbButtons);

	QWidget::setTabOrder(trvNodes, dbbButtons);

	retranslateUi(OptionsDialogClass);

	QMetaObject::connectSlotsByName(OptionsDialogClass);

} // setupUi
#endif

void Ui_NewOptionsDialogClass::retranslateUi(QDialog *OptionsDialogClass)
{
	Q_UNUSED(OptionsDialogClass);
} // retranslateUi

