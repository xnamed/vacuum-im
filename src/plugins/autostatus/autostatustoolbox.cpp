#include <QDebug>

#include "autostatustoolbox.h"

enum RulesTableRows {
	RTC_ENABLED,
	RTC_TIME,
	RTC_SHOW,
	RTC_PRIORITY,
	RTC_TEXT,
	RTC__COUNT
};

enum RulesTableRoles {
	SDR_VALUE = Qt::UserRole+1
};

#define     STR_PROPERTY        "SDR_VALUE"

AutoStatusToolBox::AutoStatusToolBox(IAutoStatus *AAutoStatus, IStatusChanger *AStatusChanger, QWidget *AParent) :
	QWidget(AParent)
{
	FAutoStatus = AAutoStatus;
	FStatusChanger = AStatusChanger;


	setupUi();
}

AutoStatusToolBox::~AutoStatusToolBox()
{

}

void AutoStatusToolBox::setupUi()
{
	QVBoxLayout *verticalLayout;
	verticalLayout = new QVBoxLayout(this);
	verticalLayout->setContentsMargins(2, 2, 4, 2);

	toolBox = new QToolBox(this);
	toolBox->setFrameStyle(QFrame::Panel | QFrame::Raised);
	//!---
	foreach(const QUuid &ruleId, FAutoStatus->rules())
		appendTableRow(ruleId, FAutoStatus->ruleValue(ruleId));
	//!---
	verticalLayout->addWidget(toolBox);
	toolBox->setCurrentIndex(0);
	//------------------
	QHBoxLayout *horLayoutPbutt= new QHBoxLayout();
	pbtAdd = new QPushButton(tr("Add"),this);
	horLayoutPbutt->addWidget(pbtAdd);
	pbtDelete = new QPushButton(tr("Delete"),this);
	horLayoutPbutt->addWidget(pbtDelete);
	QSpacerItem *horizontalSpacer = new QSpacerItem(20,20,QSizePolicy::Expanding, QSizePolicy::Minimum);
	horLayoutPbutt->addItem(horizontalSpacer);

	dbbButtonBox = new QDialogButtonBox(this);
	dbbButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
	connect(dbbButtonBox,SIGNAL(clicked(QAbstractButton *)),SLOT(onDialogButtonBoxClicked(QAbstractButton *)));
	horLayoutPbutt->addWidget(dbbButtonBox);
	verticalLayout->addLayout(horLayoutPbutt);
}

void AutoStatusToolBox::appendTableRow(const QUuid &ARuleId, const IAutoStatusRule &ARule)
{
	{
		QWidget *page = new QWidget(toolBox);
		QFormLayout *formLayout = new QFormLayout(page);
		formLayout->setContentsMargins(2, 2, 2, 2);

		QLabel *label = new QLabel(tr("Enabled"),page);
		formLayout->setWidget(RTC_ENABLED, QFormLayout::FieldRole,label);
		QCheckBox *chBoxEnabled = new QCheckBox(page);
		chBoxEnabled->setCheckState(FAutoStatus->isRuleEnabled(ARuleId) ? Qt::Checked : Qt::Unchecked);
		chBoxEnabled->setProperty(STR_PROPERTY,ARuleId.toString());
		formLayout->setWidget(RTC_ENABLED, QFormLayout::LabelRole,chBoxEnabled);

		QLabel *lblTime = new QLabel(tr("Time"),page);
		formLayout->setWidget(RTC_TIME, QFormLayout::LabelRole,lblTime);
		QSpinBox *spinBoxTime = new QSpinBox(page);
		spinBoxTime->setSuffix(tr(" minutes"));
		spinBoxTime->setMinimum(0);
		spinBoxTime->setMaximum(24*60);
		spinBoxTime->setValue(ARule.time/60);
		spinBoxTime->setProperty(STR_PROPERTY,ARule.time/60);
		formLayout->setWidget(RTC_TIME, QFormLayout::FieldRole,spinBoxTime);

		QLabel *lblStatus = new QLabel(tr("Status"),page);
		formLayout->setWidget(RTC_SHOW, QFormLayout::LabelRole,lblStatus);
		QComboBox *show = getComboBox();
		show->setCurrentText(FStatusChanger->nameByShow(ARule.show));
		formLayout->setWidget(RTC_SHOW, QFormLayout::FieldRole,show);

		QLabel *lblPriority = new QLabel(tr("Priority"),page);
		formLayout->setWidget(RTC_PRIORITY, QFormLayout::LabelRole,lblPriority);
		QSpinBox *spinBox = getSpinBox();
		spinBox->setValue(ARule.priority);
		spinBox->setProperty(STR_PROPERTY,ARule.priority);
		formLayout->setWidget(RTC_PRIORITY, QFormLayout::FieldRole,spinBox);

		QLabel *lblText = new QLabel(tr("Text"),page);
		formLayout->setWidget(RTC_TEXT, QFormLayout::LabelRole,lblText);
		QTextEdit *textEdit = new QTextEdit(page);
		formLayout->setWidget(RTC_TEXT, QFormLayout::FieldRole,textEdit);
		textEdit->setText(ARule.text);
		textEdit->setProperty(STR_PROPERTY,ARule.text);

		QString statusName=FStatusChanger->nameByShow(ARule.show);
		QIcon icon=FStatusChanger->iconByShow(ARule.show);
		toolBox->addItem(page,icon,statusName);

	}
}

void AutoStatusToolBox::onRuledItemSelectionChanged()
{

}

void AutoStatusToolBox::onDialogButtonBoxClicked(QAbstractButton *AButton)
{

}

QComboBox *AutoStatusToolBox::getComboBox()
{
	QComboBox *comboBox = new QComboBox;
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Away),FStatusChanger->nameByShow(IPresence::Away),IPresence::Away);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::DoNotDisturb),FStatusChanger->nameByShow(IPresence::DoNotDisturb),IPresence::DoNotDisturb);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::ExtendedAway),FStatusChanger->nameByShow(IPresence::ExtendedAway),IPresence::ExtendedAway);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Invisible),FStatusChanger->nameByShow(IPresence::Invisible),IPresence::Invisible);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Offline),FStatusChanger->nameByShow(IPresence::Offline),IPresence::Offline);
	comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	comboBox->setEditable(false);
	return comboBox;
}

QSpinBox *AutoStatusToolBox::getSpinBox()
{
	QSpinBox *spinBox = new QSpinBox;
	spinBox->setAlignment(Qt::AlignHCenter);
	spinBox->setMinimum(-128);
	spinBox->setMaximum(128);
//	spinBox->setSizePolicy(WDTSIZEPOLICY);
	return spinBox;
}

QTimeEdit *AutoStatusToolBox::getTimeEdit()
{
	QTimeEdit *timeEdit = new QTimeEdit;//AParent
	timeEdit->setDisplayFormat("HH:mm:ss");
	return timeEdit;
}

int AutoStatusToolBox::getCurrentStatus(int AIndex)
{
	switch (AIndex)
	{
		case 0:  return IPresence::Away;
		case 1:  return IPresence::DoNotDisturb;
		case 2:  return IPresence::ExtendedAway;
		case 3:  return IPresence::Invisible;
		case 4:  return IPresence::Offline;
		default: return IPresence::Offline;
	}
	return 0;
}


