#include <QDebug>

#include "autostatustoolbox.h"

enum RulesTableRows {
	RTC_ENABLED,
	RTC_TIME,
	RTC_SHOW,
	RTC_TEXT,
	RTC_PRIORTXT,
	RTC_PRIORITY,
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
    FAutoStatus     = AAutoStatus;
    FStatusChanger  = AStatusChanger;

	QVBoxLayout *verticalLayout;
	verticalLayout = new QVBoxLayout(this);
    verticalLayout->setMargin(0);
	//---
	toolBox = new QToolBox(this);
	toolBox->setFrameStyle(QFrame::Panel | QFrame::Raised);
	toolBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	//------
	QList<AutoStatusRuleExt> newRules = setAutoRules();
	QList<QUuid> rules = FAutoStatus->rules();
	for(int i=0;i<rules.count();i++)
	{
		QUuid ruleId = rules.at(i);
		IAutoStatusRule rule=FAutoStatus->ruleValue(ruleId);
		bool en=FAutoStatus->isRuleEnabled(ruleId);
		switch (rule.show)
		{
			case 0: newRules[3].rule=rule; newRules[3].uidId=ruleId; newRules[3].enable=en; break;	// Offline
			case 3: newRules[1].rule=rule; newRules[1].uidId=ruleId; newRules[1].enable=en;break;	// Away,
			case 4: newRules[0].rule=rule; newRules[0].uidId=ruleId; newRules[0].enable=en;break;	// DoNotDisturb,
			case 5: newRules[2].rule=rule; newRules[2].uidId=ruleId; newRules[2].enable=en; break;	// ExtendedAway,
			default:newRules[1].rule=rule; newRules[1].uidId=ruleId; newRules[1].enable=en; break;	//
		}
	}
	//! table to be sorted by time ---

	for(int i=0;i<newRules.count();i++)		// out table
		getStatusTable(newRules.at(i),i);
	//------

	verticalLayout->addWidget(toolBox);
	toolBox->setCurrentIndex(0);
	//---
	dbbButtonBox = new QDialogButtonBox(this);
	dbbButtonBox->addButton(QDialogButtonBox::Apply);//  :Ok
	dbbButtonBox->addButton(QDialogButtonBox::Help);
//	pbtHelp = dbbButtonBox->addButton(tr("Add"),QDialogButtonBox::ActionRole);//tlbAwayHelp

	QHBoxLayout *hblButtons = new QHBoxLayout;
//    hblButtons->addWidget(pbtAdd);
    hblButtons->addStretch();
	hblButtons->addWidget(dbbButtonBox);
	verticalLayout->addLayout(hblButtons);

	onRuledItemSelectionChanged(0);

	connect(toolBox,SIGNAL(currentChanged(int)),SLOT(onRuledItemSelectionChanged(int)));
	connect(dbbButtonBox,SIGNAL(clicked(QAbstractButton *)),SLOT(onDialogButtonBoxClicked(QAbstractButton *)));
}

AutoStatusToolBox::~AutoStatusToolBox()
{

}

void AutoStatusToolBox::getStatusTable(const AutoStatusRuleExt ARuleExt,int ARow)
{
	QUuid ARuleId		  = ARuleExt.uidId;
	IAutoStatusRule ARule = ARuleExt.rule;
	bool Enable			  = ARuleExt.enable;
	if(!ARuleId.isNull())
		Enable = FAutoStatus->isRuleEnabled(ARuleId);
	else
		Enable = false;

	QWidget *page = new QWidget(toolBox);
	page->setProperty(SDR_VALUE,ARule.show);

	int size=IconStorage::iconSize();
	QFormLayout *formLayout = new QFormLayout(page);
	formLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
	formLayout->setVerticalSpacing(size/4);

	QCheckBox *chBoxEnabled = new QCheckBox(page);
	chBoxEnabled->setCheckState(Enable ? Qt::Checked : Qt::Unchecked);
	if(!ARuleId.isNull())
		chBoxEnabled->setProperty(SDR_VALUE,ARuleId.toString());
	else
		chBoxEnabled->setProperty(SDR_VALUE,"");

	bool flag=(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
	chBoxEnabled->setProperty(FLAG,flag);

	QSpinBox *spinBoxTime = getSpinBoxTime(page);
	spinBoxTime->setValue(ARule.time/60);
	spinBoxTime->setProperty(SDR_VALUE,ARule.time/60);

	QSpinBox *spinBox = getSpinBox(page);
	spinBox->setValue(ARule.priority);
	spinBox->setProperty(SDR_VALUE,ARule.priority);

	QTextEdit *textEdit = new QTextEdit(page);
	textEdit->setText(ARule.text);
	textEdit->setProperty(SDR_VALUE,ARule.text);

	QLabel *lblEn;
	if(ARule.show)
		lblEn= new QLabel(tr("Change status after"),page);
	else
		lblEn= new QLabel(tr("Automatically disconnect after"),page);
	lblEn->setWordWrap(true);

	formLayout->setWidget(RTC_ENABLED, QFormLayout::FieldRole,lblEn);
	formLayout->setWidget(RTC_ENABLED, QFormLayout::LabelRole,chBoxEnabled);
	formLayout->setWidget(RTC_TIME, QFormLayout::FieldRole,spinBoxTime);
	QLabel *lbShow= new QLabel(tr("of inactivity with message:"),page);
	lbShow->setWordWrap(true);
	formLayout->setWidget(RTC_SHOW, QFormLayout::FieldRole,lbShow);
	formLayout->setWidget(RTC_TEXT, QFormLayout::FieldRole,textEdit);
	formLayout->setWidget(RTC_PRIORTXT, QFormLayout::FieldRole,new QLabel(tr("Priority"),page));
	formLayout->setWidget(RTC_PRIORITY, QFormLayout::FieldRole,spinBox);

	QIcon icon=FStatusChanger->iconByShow(ARule.show);
	QString statusName=FStatusChanger->nameByShow(ARule.show);
	toolBox->insertItem(ARow,page,icon,statusName);

}

void AutoStatusToolBox::onRuledItemSelectionChanged(int AIndex)
{
    toolBox->setCurrentIndex(AIndex);
}

void AutoStatusToolBox::onDialogButtonBoxClicked(QAbstractButton *AButton)
{
	if (dbbButtonBox->buttonRole(AButton) == QDialogButtonBox::HelpRole)
	{
		emit helpButtonClicked();
	}
	else if (dbbButtonBox->buttonRole(AButton) == QDialogButtonBox::ApplyRole)//  AcceptRole
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
			if(allSpinBox.count()>0)
				rule.time = allSpinBox.at(0)->value()*60;
			if(allComboBox.count()>0){
				rule.show = getCurrentStatus(allComboBox.at(0)->currentIndex());
			}else{
				rule.show = wd->property(SDR_VALUE).toInt();//page->setProperty(SDR_VALUE,ARule.show);
			}
			if(allTextEdit.count()>0)
				rule.text = allTextEdit.at(0)->toPlainText();
			if(allSpinBox.count()>1)
				rule.priority = allSpinBox.at(1)->value();

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
				ruleId = FAutoStatus->insertRule(rule);
				allCheckBox.at(0)->setProperty(SDR_VALUE,ruleId.toString());
			}
			bool st=allCheckBox.at(0)->checkState()==Qt::Checked;
			FAutoStatus->setRuleEnabled(ruleId,st);

            toolBox->setItemIcon(row,FStatusChanger->iconByShow(rule.show));
            toolBox->setItemText(row,FStatusChanger->nameByShow(rule.show));
		}

		foreach(const QUuid &ruleId, oldRules)
			FAutoStatus->removeRule(ruleId);
		toolBox->setCurrentIndex(0);

		emit m_accepted(); //accept();
	}
}

QList<AutoStatusRuleExt> AutoStatusToolBox::setAutoRules()
{
	QList<AutoStatusRuleExt> rulesList;

	AutoStatusRuleExt rule;
	rule.rule.time		= 10*60;	//sec
	rule.rule.show		= IPresence::DoNotDisturb;
	rule.rule.priority	= 30;
	rule.rule.text		= tr("Auto status due to inactivity for more than #(m) minutes");
	rule.uidId			= QUuid();
	rule.enable			= false;
	rulesList.append(rule);

	rule.rule.time		= 20*60;
	rule.rule.show		= IPresence::Away;
	rule.rule.priority	= 20;
	rule.rule.text		= tr("Auto status");
	rule.uidId			= QUuid();
	rule.enable			= false;
	rulesList.append(rule);

	rule.rule.time		= 30*60;
	rule.rule.show		= IPresence::ExtendedAway;
	rule.rule.priority	= 15;
	rule.rule.text		= tr("Auto status");
	rule.uidId			= QUuid();
	rule.enable			= false;
	rulesList.append(rule);

	rule.rule.time		= 100*60;
	rule.rule.show		= IPresence::Offline;
	rule.rule.priority	= 10;
	rule.rule.text		= tr("Disabled due to inactivity more # (m) minutes");
	rule.uidId			= QUuid();
	rule.enable			= false;
	rulesList.append(rule);

	return rulesList;
}

QComboBox *AutoStatusToolBox::getComboBox(QWidget *page)
{
	QComboBox *comboBox = new QComboBox(page);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Away),FStatusChanger->nameByShow(IPresence::Away),IPresence::Away);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::DoNotDisturb),FStatusChanger->nameByShow(IPresence::DoNotDisturb),IPresence::DoNotDisturb);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::ExtendedAway),FStatusChanger->nameByShow(IPresence::ExtendedAway),IPresence::ExtendedAway);
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

int AutoStatusToolBox::getCurrentStatus(int AIndex)
{
	switch (AIndex)
	{
		case 0:  return IPresence::Away;
		case 1:  return IPresence::DoNotDisturb;
		case 2:  return IPresence::ExtendedAway;
		case 3:  return IPresence::Offline;
		default: return IPresence::Offline;
	}
	return 0;
}
