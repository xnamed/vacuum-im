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


#define		FLAG			"STR_FLAG"
#define     SDR_VALUE		"SDR_VALUE"

AutoStatusToolBox::AutoStatusToolBox(IAutoStatus *AAutoStatus, IStatusChanger *AStatusChanger, QWidget *AParent) :
	QWidget(AParent)
{
	FAutoStatus = AAutoStatus;
	FStatusChanger = AStatusChanger;

	QVBoxLayout *verticalLayout;
	verticalLayout = new QVBoxLayout(this);
	verticalLayout->setContentsMargins(2, 2, 4, 2);
	//!---
	toolBox = new QToolBox(this);
	toolBox->setFrameStyle(QFrame::Panel | QFrame::Raised);
	toolBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

qDebug()<<"AutoStatusToolBox/FAutoStatus->rules()"<<FAutoStatus->rules().size();

	foreach(const QUuid &ruleId, FAutoStatus->rules())
		appendTableCol(ruleId, FAutoStatus->ruleValue(ruleId));

	verticalLayout->addWidget(toolBox);
	toolBox->setCurrentIndex(0);
	//---
	dbbButtonBox = new QDialogButtonBox(this);
	dbbButtonBox->addButton(QDialogButtonBox::Ok);
	dbbButtonBox->addButton(QDialogButtonBox::Cancel);
	pbtAdd = dbbButtonBox->addButton(tr("Add"),QDialogButtonBox::ActionRole);
	pbtDelete = dbbButtonBox->addButton(tr("Delete"),QDialogButtonBox::ActionRole);

	QHBoxLayout *hblButtons = new QHBoxLayout;
	//hblButtons->addWidget(pbtAdd);
	//hblButtons->addWidget(pbtDelete);
	//hblButtons->addStretch();
	hblButtons->addWidget(dbbButtonBox);
	verticalLayout->addLayout(hblButtons);

	onRuledItemSelectionChanged(0);

	connect(toolBox,SIGNAL(currentChanged(int)),SLOT(onRuledItemSelectionChanged(int)));
	connect(dbbButtonBox,SIGNAL(clicked(QAbstractButton *)),SLOT(onDialogButtonBoxClicked(QAbstractButton *)));
}

AutoStatusToolBox::~AutoStatusToolBox()
{

}

void AutoStatusToolBox::appendTableCol(const QUuid &ARuleId, const IAutoStatusRule &ARule)
{
	QWidget *page = new QWidget(toolBox);
	QFormLayout *formLayout = new QFormLayout(page);
	formLayout->setContentsMargins(2, 2, 2, 2);

	QCheckBox *chBoxEnabled = new QCheckBox(page);
	chBoxEnabled->setCheckState(FAutoStatus->isRuleEnabled(ARuleId) ? Qt::Checked : Qt::Unchecked);
	chBoxEnabled->setProperty(SDR_VALUE,ARuleId.toString());
	bool flag=(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
	chBoxEnabled->setProperty(FLAG,flag);

	QSpinBox *spinBoxTime = getSpinBoxTime(page);
	spinBoxTime->setValue(ARule.time/60);
	spinBoxTime->setProperty(SDR_VALUE,ARule.time/60);

	QComboBox *show = getComboBox(page);
	show->setCurrentText(FStatusChanger->nameByShow(ARule.show));
	show->setProperty(SDR_VALUE,ARule.show);

	QSpinBox *spinBox = getSpinBox(page);
	spinBox->setValue(ARule.priority);
	spinBox->setProperty(SDR_VALUE,ARule.priority);

	QTextEdit *textEdit = new QTextEdit(page);
	textEdit->setText(ARule.text);
	textEdit->setProperty(SDR_VALUE,ARule.text);

	formLayout->setWidget(RTC_ENABLED, QFormLayout::FieldRole,new QLabel(tr("Enabled"),page));
	formLayout->setWidget(RTC_ENABLED, QFormLayout::LabelRole,chBoxEnabled);
	formLayout->setWidget(RTC_TIME, QFormLayout::LabelRole,new QLabel(tr("Time"),page));
	formLayout->setWidget(RTC_TIME, QFormLayout::FieldRole,spinBoxTime);
	formLayout->setWidget(RTC_SHOW, QFormLayout::LabelRole,new QLabel(tr("Status"),page));
	formLayout->setWidget(RTC_SHOW, QFormLayout::FieldRole,show);
	formLayout->setWidget(RTC_PRIORITY, QFormLayout::LabelRole,new QLabel(tr("Priority"),page));
	formLayout->setWidget(RTC_PRIORITY, QFormLayout::FieldRole,spinBox);
	formLayout->setWidget(RTC_TEXT, QFormLayout::LabelRole,new QLabel(tr("Text"),page));
	formLayout->setWidget(RTC_TEXT, QFormLayout::FieldRole,textEdit);

	QString statusName=FStatusChanger->nameByShow(ARule.show);
	QIcon icon=FStatusChanger->iconByShow(ARule.show);
	toolBox->addItem(page,icon,statusName);
qDebug()<<"appendTableCol/toolBox->count()"<<toolBox->count();
}

void AutoStatusToolBox::appendTableCol(const QUuid &ARuleId, const IAutoStatusRule &ARule, bool AEn)
{
	QWidget *page = new QWidget(toolBox);
	QFormLayout *formLayout = new QFormLayout(page);
	formLayout->setContentsMargins(2, 2, 2, 2);

	QCheckBox *chBoxEnabled = new QCheckBox(page);
	chBoxEnabled->setCheckState(AEn ? Qt::Checked : Qt::Unchecked);
	chBoxEnabled->setProperty(SDR_VALUE,ARuleId.toString());
	bool flag=(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
	chBoxEnabled->setProperty(FLAG,flag);

	QSpinBox *spinBoxTime = getSpinBoxTime(page);
	spinBoxTime->setValue(ARule.time/60);
	spinBoxTime->setProperty(SDR_VALUE,ARule.time/60);

	QComboBox *show = getComboBox(page);
	show->setCurrentText(FStatusChanger->nameByShow(ARule.show));
	show->setProperty(SDR_VALUE,ARule.show);

	QSpinBox *spinBox = getSpinBox(page);
	spinBox->setValue(ARule.priority);
	spinBox->setProperty(SDR_VALUE,ARule.priority);

	QTextEdit *textEdit = new QTextEdit(page);
	textEdit->setText(ARule.text);
	textEdit->setProperty(SDR_VALUE,ARule.text);

	formLayout->setWidget(RTC_ENABLED, QFormLayout::FieldRole,new QLabel(tr("Enabled"),page));
	formLayout->setWidget(RTC_ENABLED, QFormLayout::LabelRole,chBoxEnabled);
	formLayout->setWidget(RTC_TIME, QFormLayout::LabelRole,new QLabel(tr("Time"),page));
	formLayout->setWidget(RTC_TIME, QFormLayout::FieldRole,spinBoxTime);
	formLayout->setWidget(RTC_SHOW, QFormLayout::LabelRole,new QLabel(tr("Status"),page));
	formLayout->setWidget(RTC_SHOW, QFormLayout::FieldRole,show);
	formLayout->setWidget(RTC_PRIORITY, QFormLayout::LabelRole,new QLabel(tr("Priority"),page));
	formLayout->setWidget(RTC_PRIORITY, QFormLayout::FieldRole,spinBox);
	formLayout->setWidget(RTC_TEXT, QFormLayout::LabelRole,new QLabel(tr("Text"),page));
	formLayout->setWidget(RTC_TEXT, QFormLayout::FieldRole,textEdit);

	QString statusName=FStatusChanger->nameByShow(ARule.show);
	QIcon icon=FStatusChanger->iconByShow(ARule.show);
	toolBox->addItem(page,icon,statusName);
qDebug()<<"appendTableCol-2/toolBox->count()"<<toolBox->count();
}

void AutoStatusToolBox::onRuledItemSelectionChanged(int)
{
	bool st=toolBox->count()>1 ? true : false;
	pbtDelete->setEnabled(st);
}

void AutoStatusToolBox::onDialogButtonBoxClicked(QAbstractButton *AButton)
{
	if (AButton == pbtAdd)
	{
		IAutoStatusRule rule;
		if (toolBox->count() > 0)
		{
			QWidget *wd=toolBox->currentWidget();
			QList<QSpinBox *> allSpinBox = wd->findChildren<QSpinBox *>();
			if(allSpinBox.count() >0)
				rule.time=(allSpinBox.at(0)->property(SDR_VALUE).toInt()+5)*60;
		}
		else
			rule.time = 10*60;
		rule.priority = 20;
		rule.show = IPresence::Away;
		rule.text = tr("Auto status");
		appendTableCol(QUuid(),rule,RTC_ENABLED);
	}
	else if (AButton == pbtDelete)
	{
		int row=toolBox->currentIndex();
		if(row>0)
			toolBox->removeItem(row);
		toolBox->setCurrentIndex(0);
	}
	else if (dbbButtonBox->buttonRole(AButton) == QDialogButtonBox::AcceptRole)
	{
		QList<QUuid> oldRules = FAutoStatus->rules();
		for (int row = 0; row<toolBox->count(); row++)
		{
			toolBox->setCurrentIndex(row);
			QWidget *wd=toolBox->currentWidget();
			QList<QCheckBox *> allCheckBox = wd->findChildren<QCheckBox *>();
			QList<QSpinBox *> allSpinBox   = wd->findChildren<QSpinBox *>();
			QList<QComboBox *> allComboBox = wd->findChildren<QComboBox *>();
			QList<QTextEdit *> allTextEdit = wd->findChildren<QTextEdit *>();

			IAutoStatusRule rule;
			rule.time	  = allSpinBox.at(0)->value()*60;
			rule.show	  = getCurrentStatus(allComboBox.at(0)->currentIndex());
			rule.text	  = allTextEdit.at(0)->toPlainText();
			rule.priority = allSpinBox.at(1)->value();

qDebug()<<"onDialogButtonBoxClicked/checkState"<<row<<allCheckBox.at(0)->checkState()<<"Time"<<rule.time<<rule.priority<<"status"<<rule.show;

			//QUuid ruleId = tbwRules->item(row,RTC_ENABLED)->data(SDR_VALUE).toString();
			QUuid ruleId = allCheckBox.at(0)->property(SDR_VALUE).toString();
			if (!ruleId.isNull())
			{
				IAutoStatusRule oldRule = FAutoStatus->ruleValue(ruleId);
				if (oldRule.time!=rule.time || oldRule.show!=rule.show || oldRule.text!=rule.text || oldRule.priority!=rule.priority)
					FAutoStatus->updateRule(ruleId,rule);
				oldRules.removeAll(ruleId);
			}
			else
			{
//tbwRules->item(row,RTC_ENABLED)->setData(SDR_VALUE,ruleId.toString());
				ruleId = FAutoStatus->insertRule(rule);
				allCheckBox.at(0)->setProperty(SDR_VALUE,ruleId.toString());
			}
			bool st=allCheckBox.at(0)->checkState()==Qt::Checked;
			FAutoStatus->setRuleEnabled(ruleId,st);
//			FAutoStatus->setRuleEnabled(ruleId,tbwRules->item(row,RTC_ENABLED)->checkState()==Qt::Checked);
		}
		toolBox->setCurrentIndex(0);
		foreach(const QUuid &ruleId, oldRules)
			FAutoStatus->removeRule(ruleId);

		emit m_accepted(); //accept();
	}
	else if (dbbButtonBox->buttonRole(AButton) == QDialogButtonBox::RejectRole)
	{
		//reject();
	}
}

QComboBox *AutoStatusToolBox::getComboBox(QWidget *page)
{
	QComboBox *comboBox = new QComboBox(page);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Away),FStatusChanger->nameByShow(IPresence::Away),IPresence::Away);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::DoNotDisturb),FStatusChanger->nameByShow(IPresence::DoNotDisturb),IPresence::DoNotDisturb);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::ExtendedAway),FStatusChanger->nameByShow(IPresence::ExtendedAway),IPresence::ExtendedAway);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Invisible),FStatusChanger->nameByShow(IPresence::Invisible),IPresence::Invisible);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Offline),FStatusChanger->nameByShow(IPresence::Offline),IPresence::Offline);
	comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	comboBox->setEditable(false);
	return comboBox;
}

QSpinBox *AutoStatusToolBox::getSpinBox(QWidget *page)
{
	QSpinBox *spinBox = new QSpinBox(page);
	spinBox->setAlignment(Qt::AlignHCenter);
	spinBox->setMinimum(-128);
	spinBox->setMaximum(128);
	return spinBox;
}

QSpinBox *AutoStatusToolBox::getSpinBoxTime(QWidget *page)
{
	QSpinBox *spinBox = new QSpinBox(page);
	spinBox->setAlignment(Qt::AlignHCenter);
	spinBox->setSuffix(tr(" minutes"));
	spinBox->setMinimum(0);
	spinBox->setMaximum(24*60);
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
