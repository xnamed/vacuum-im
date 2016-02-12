#include <QtGui>
#include <QDir>
#include <QDomDocument>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLine>
#include <QMessageBox>
#include <QLineEdit>

#include <interfaces/igateways.h>
#include <definitions/optionvalues.h>
#include <definitions/menuicons.h>
#include <definitions/wizardicons.h>
#include <definitions/serviceicons.h>
#include <definitions/resources.h>
#include <definitions/rosterindexroles.h>
#include <definitions/rosterindexkinds.h>
#include <utils/options.h>
#include <utils/pluginhelper.h>
#include <utils/qt4qt5compat.h>

#include "wizardpages.h"

#define TRANSPORT_LIST	"transports.xml"

#define FIELD_TYPE_HIDDEN       "hidden"
#define FIELD_TYPE_FIXED		"fixed"
#define FIELD_TYPE_BOOLEAN		"boolean"
#define FIELD_TYPE_TEXTSINGLE	"text-single"
#define FIELD_TYPE_TEXTPRIVATE	"text-private"
#define FIELD_TYPE_LISTSINGLE	"list-single"
#define FIELD_TYPE_JIDSINGLE	"jid-single"
#define FIELD_TYPE_TEXTMULTI	"text-multi"
#define FIELD_TYPE_JIDMULTI     "jid-multi"
#define FIELD_TYPE_LISTMULTI	"list-multi"

#define VALIDATE_TYPE_DATETIME  "xs:dateTime"
#define VALIDATE_TYPE_DATE      "xs:date"
#define VALIDATE_TYPE_TIME      "xs:time"

#define WF_TRANSPORT_FROM		"transportFrom"
#define WF_TRANSPORT_TO			"transportTo"
#define WF_NETWORK				"network"
#define WF_AUTO_SUBSCRIBE		"autoSubscribe"

TransportWizard::TransportWizard(const Jid &AStreamJid, const Jid &ATransportJid, QWidget *Parent) :
	QWizard(Parent),
	FStreamJid(AStreamJid)
{
	setAttribute(Qt::WA_DeleteOnClose);
	IRegistration *registration = PluginHelper::pluginInstance<IRegistration>();
	IServiceDiscovery *serviceDiscovery = PluginHelper::pluginInstance<IServiceDiscovery>();

	NetworksPage *networksPage = new NetworksPage(this);
	setPage(Page_Networks, networksPage);
	GatewayPage *Gateway = new GatewayPage(FStreamJid, serviceDiscovery, !ATransportJid.isEmpty(), this);
	setPage(Page_Gateway, Gateway);
	ProcessPage *Process = new ProcessPage(FStreamJid, registration, Gateway, this);
	setPage(Page_Process,Process);
	ResultPage *Result = new ResultPage(FStreamJid, registration, Process, this);
	setPage(Page_Result, Result);
	setPage(Page_Conclusion, new ConclusionPage(networksPage));

	if (ATransportJid.isEmpty())
	{
		setPage(Page_Intro, new IntroPage);
		TransportsPage *transportsPage = new TransportsPage(FStreamJid, serviceDiscovery, this);
		setPage(Page_Transports, transportsPage);
		setStartId(Page_Intro);
	}
	else
	{
		QString network;
		setField(WF_TRANSPORT_FROM, ATransportJid.full());
		if (serviceDiscovery->hasDiscoInfo(AStreamJid, ATransportJid))
		{
			IDiscoInfo discoInfo = serviceDiscovery->discoInfo(AStreamJid, ATransportJid);
			for (QList<IDiscoIdentity>::ConstIterator it = discoInfo.identity.constBegin(); it!=discoInfo.identity.constEnd(); ++it)
				if ((*it).category=="gateway")
				{
					network = (*it).type;
					break;
				}
		}

		if (network.isEmpty())
			setStartId(Page_Networks);
		else
		{
			setField(WF_NETWORK, network);
			setStartId(Page_Gateway);
		}
	}
	setOptions(NoBackButtonOnLastPage|NoBackButtonOnStartPage|NoCancelButton);

#ifndef Q_OS_MAC
	setWizardStyle(ModernStyle);
#endif

	QString fileName = IconStorage::staticStorage(RSR_STORAGE_WIZARDS)->fileFullName(WZI_WIZARD);
	setPixmap(QWizard::LogoPixmap, QPixmap(fileName));//LogoPixmap WatermarkPixmap

	fileName = IconStorage::staticStorage(RSR_STORAGE_WIZARDS)->fileFullName(WZI_BANNER);
	setPixmap(QWizard::BannerPixmap, QPixmap(fileName));

	setWindowTitle(tr("Legacy network connection Wizard"));
	setWindowIcon(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(MNI_MAINWINDOW_LOGO16));
}

void TransportWizard::onFinished(int AStatus)
{
	Q_UNUSED(AStatus)
	if (field(WF_AUTO_SUBSCRIBE).toBool())
		Options::node(OPV_ROSTER_AUTOSUBSCRIBE).setValue(FAutoSubscribe);
}

//!------------------------------
IntroPage::IntroPage(QWidget *parent): QWizardPage(parent)
{
	QString style="style='color:blue;'";
	setTitle(QString("<span %1>%2</span>").arg(style).arg(tr("Legacy network connection")));
	setSubTitle(QString("<span %1>%2</span>").arg(style).arg(tr("This Wizard will help you to connect to a legacy network via transport or change transport to another one")));

	QString fileName = IconStorage::staticStorage(RSR_STORAGE_WIZARDS)->fileFullName(WZI_TRANSPORT);
	setPixmap(QWizard::WatermarkPixmap, QPixmap(fileName));

	QVBoxLayout *layout = new QVBoxLayout;
	QLabel *lblText = new QLabel(QString("<span %1>%2</span>").arg(style).arg(tr("What do you want to do?")));
	layout->addWidget(lblText);
	lblText->setWordWrap(true);
	layout->addWidget(FClbConnectLegacyNetwork	= new QCommandLinkButton(tr("&Connect to a legacy network")));
	layout->addWidget(FClbChangeTransport		= new QCommandLinkButton(tr("&Change transport")));
	setLayout(layout);

	connect(FClbConnectLegacyNetwork, SIGNAL(clicked()), SLOT(onClicked()));
	connect(FClbChangeTransport, SIGNAL(clicked()), SLOT(onClicked()));
}

void IntroPage::onClicked()
{
	if (sender() == FClbChangeTransport)
		FNextId = TransportWizard::Page_Transports;
	else
		FNextId = TransportWizard::Page_Networks;
	wizard()->next();
}

//!------------------------------
TransportsPage::TransportsPage(const Jid &AStreamJid, const IServiceDiscovery *AServiceDiscovery, QWidget *parent):
	QWizardPage(parent), FServiceDiscovery(AServiceDiscovery), FStreamJid(AStreamJid)
{
	QString style="style='color:blue;'";
	setTitle(QString("<span %2>%1</span>").arg(tr("Transport selection")).arg(style));
	setSubTitle(QString("<span %2>%1</span>").arg(tr("Choose a transport you want to change")).arg(style));

	QLabel *lblTransportsList = new QLabel(QString("<b>%1:</b>").arg(tr("Please select a transport from the list")));
	FTransportsList = new SelectableTreeWidget();
	QStringList headers;
	headers << tr("Transport") << tr("Name") << tr("Type");
	FTransportsList->setHeaderLabels(headers);

	lblTransportsList->setBuddy(FTransportsList);
	registerField(WF_TRANSPORT_FROM"*", FTransportsList, "value", SIGNAL(valueChanged(QString)));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(lblTransportsList);
	layout->addWidget(FTransportsList);
	setLayout(layout);

	connect(FTransportsList, SIGNAL(valueChanged(QString)), SLOT(onTransportSelected(QString)));
	connect(FTransportsList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), parent, SLOT(next()));
}

int TransportsPage::nextId() const
{
	Jid jid(field(WF_TRANSPORT_FROM).toString());
	IDiscoInfo discoInfo = FServiceDiscovery->discoInfo(FStreamJid, jid);
	QString name, type;
	for (QList<IDiscoIdentity>::ConstIterator iti = discoInfo.identity.constBegin(); iti!=discoInfo.identity.constEnd(); ++iti)
		if ((*iti).category=="gateway")
		{
			name = (*iti).name;
			type = (*iti).type;
			break;
		}
	return type.isEmpty()?TransportWizard::Page_Networks:TransportWizard::Page_Gateway;
}

void TransportsPage::initializePage()
{
	IGateways *gateways = PluginHelper::pluginInstance<IGateways>();
	QList<Jid> agents = gateways->streamServices(FStreamJid);
	FTransportsList->clear();
	for (QList<Jid>::ConstIterator it = agents.constBegin(); it != agents.constEnd(); ++it)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(FTransportsList);

		IDiscoInfo discoInfo = FServiceDiscovery->discoInfo(FStreamJid, *it);
		QString name, type;
		for (QList<IDiscoIdentity>::ConstIterator iti = discoInfo.identity.constBegin(); iti!=discoInfo.identity.constEnd(); ++iti)
			if ((*iti).category=="gateway")
			{
				name = (*iti).name;
				type = (*iti).type;
				break;
			}
		item->setIcon(0, FServiceDiscovery->identityIcon(discoInfo.identity));
		item->setText(0, (*it).full());
		item->setData(0, SelectableTreeWidget::ValueRole, (*it).full());
		item->setText(1, name);
		item->setText(2, type);
		FTransportsList->addTopLevelItem(item);
	}
}

void TransportsPage::onTransportSelected(const QString &ATransportJid)
{
	IDiscoInfo discoInfo = FServiceDiscovery->discoInfo(FStreamJid, ATransportJid);
	QString type;
	for (QList<IDiscoIdentity>::ConstIterator iti = discoInfo.identity.constBegin(); iti!=discoInfo.identity.constEnd(); ++iti)
		if ((*iti).category=="gateway")
		{
			type = (*iti).type;
			break;
		}
	setField(WF_NETWORK, type);
}

//!------------------------------
NetworksPage::NetworksPage(QWidget *parent): QWizardPage(parent), FIconStorage(IconStorage::staticStorage(RSR_STORAGE_SERVICEICONS))
{
	QString style="style='color:blue;'";
	setTitle(QString("<span %2>%1</span>").arg(tr("Network selection")).arg(style));
	setSubTitle(QString("<span %2>%1</span>").arg(tr("Choose a legacy network you want to connect")).arg(style));

	QLabel *lblNetworksList = new QLabel(QString("<b>%1:</b>").arg(tr("Please select a network from the list")));
	FNetworksList = new SelectableTreeWidget();
	lblNetworksList->setBuddy(FNetworksList);
	registerField(WF_NETWORK"*", FNetworksList, "value", SIGNAL(valueChanged(QString)));

	FNetworksList->sortItems(0, Qt::AscendingOrder);

	QStringList headers;
	headers << tr("Netwok") << tr("Comment");
	FNetworksList->setHeaderLabels(headers);

	FNetworkNames.insert("aim", tr("AIM"));
	FNetworkNames.insert("gadu-gadu", tr("Gadu-Gadu"));
	FNetworkNames.insert("icq", tr("ICQ"));
	FNetworkNames.insert("irc", tr("IRC"));
	FNetworkNames.insert("mrim", tr("Mail.Ru Agent"));
	FNetworkNames.insert("qq", tr("QQ"));
	FNetworkNames.insert("renren", tr("Renren"));
	FNetworkNames.insert("skype", tr("Skype"));
	FNetworkNames.insert("sametime", tr("IBM Sametime"));
	FNetworkNames.insert("sms", tr("SMS"));
	FNetworkNames.insert("twitter", tr("Twitter"));
	FNetworkNames.insert("x-tlen", tr("Tlen.pl"));
	FNetworkNames.insert("vk", tr("vKontakte"));
	FNetworkNames.insert("yahoo", tr("Yahoo!"));

	FNetworkDescriptions.insert("aim", tr("AOL Instant Messenger"));
	FNetworkDescriptions.insert("gadu-gadu", tr("Gadu-Gadu - Polish instant messenger"));
	FNetworkDescriptions.insert("icq", tr("\"I seek You\" instant messenger, popular in exUSSR and Germany"));
	FNetworkDescriptions.insert("irc", tr("Internet Relay Chat"));
	FNetworkDescriptions.insert("mrim", tr("Instant messenger from Mail.ru portal"));
	FNetworkDescriptions.insert("qq", tr("Tencent QQ - Chinese instant messenger"));
	FNetworkDescriptions.insert("renren", tr("Chinese social network with an interface similar to Facebook"));
	FNetworkDescriptions.insert("skype", tr("IP-telephony software with voice, video and text communication"));
	FNetworkDescriptions.insert("sametime", tr("Real-time communication services from IBM (formerly IBM Lotus Sametime)"));
	FNetworkDescriptions.insert("sms", tr("Sending Short Messages (SMS) to mobile phones"));
	FNetworkDescriptions.insert("twitter", tr("An online social networking service that enables users to send and read short 140-character messages called \"tweets\""));
	FNetworkDescriptions.insert("x-tlen", tr("An adware licensed Polish instant messaging service, fully compatible with Gadu-Gadu"));
	FNetworkDescriptions.insert("vk", tr("Russian social network with an interface similar to Facebook"));
	FNetworkDescriptions.insert("yahoo", tr("Instant messenger from Yahoo! portal"));

	FNetworksList->setColumnWidth(0,160);
	FNetworksList->setColumnWidth(1,440);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(lblNetworksList);
	layout->addWidget(FNetworksList);
	setLayout(layout);

	loadNetworksList();

	connect(FNetworksList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), parent, SLOT(next()));
}

void NetworksPage::loadNetworksList()
{
	for(QHash<QString,QString>::ConstIterator it=FNetworkNames.constBegin(); it!=FNetworkNames.constEnd(); ++it)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(FNetworksList);
		QIcon icon=FIconStorage->getIcon(it.key());
		item->setText(0, *it);
		item->setData(0, SelectableTreeWidget::ValueRole, it.key());
		if(icon.isNull())
			icon=FIconStorage->getIcon(SRI_GATEWAY);
		item->setIcon(0, icon);
		item->setText(1, FNetworkDescriptions.value(it.key()));
	}
}

int NetworksPage::nextId() const
{
	return TransportWizard::Page_Gateway;
}

//!------------------------------
GatewayPage::GatewayPage(const Jid &AStreamJid, IServiceDiscovery *AServiceDiscovery, bool ATransport, QWidget *parent):
	QWizardPage(parent), FServiceDiscovery(AServiceDiscovery), FStreamJid(AStreamJid)
{
	QString style="style='color:blue;'";
	setTitle(QString("<span %2>%1</span>").arg(tr("Gateway selection")).arg(style));
	setSubTitle(QString("<span %2>%1</span>").arg(tr("Choose a gateway you want to use")).arg(style));

	FlblGatewaysList = new QLabel(QString("<b>%1:</b>").arg(tr("Gateways &list")));
	FTransportList = new SelectableTreeWidget();
	FlblGatewaysList->setBuddy(FTransportList);

	if (ATransport)
	{
		QLineEdit *ledTransport = new QLineEdit(this);	// Just dummy editor
		ledTransport->setVisible(false);
		registerField(WF_TRANSPORT_FROM, ledTransport);
	}

	registerField(WF_TRANSPORT_TO, FTransportList, "value", SIGNAL(valueChanged(QString)));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(FlblGatewaysList);
	layout->addWidget(FTransportList);
	setLayout(layout);

	FIconStorageWizards = IconStorage::staticStorage(RSR_STORAGE_WIZARDS);
	FIconStorageMenu = IconStorage::staticStorage(RSR_STORAGE_MENUICONS);

	connect(FServiceDiscovery->instance(), SIGNAL(discoItemsReceived(IDiscoItems)), SLOT(onDiscoItemsReceived(IDiscoItems)));
	connect(FServiceDiscovery->instance(), SIGNAL(discoInfoReceived(IDiscoInfo)), SLOT(onDiscoInfoReceived(IDiscoInfo)));
	connect(FTransportList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), parent, SLOT(next()));
}

void GatewayPage::initializePage()
{
	if (FNetwork != field(WF_NETWORK).toString())
	{
		FNetwork = field(WF_NETWORK).toString();
		FDiscoItems = IDiscoItems();
		loadTransportList();
	}
	FlblGatewaysList->setText(QString("<span><b>%2 %3</b></span>").arg(QString(tr("List of Gateways for"))).arg(FNetwork));
	if (FServiceDiscovery && FDiscoItems.streamJid.isEmpty())
		FServiceDiscovery->requestDiscoItems(FStreamJid, FStreamJid.domain());
}

bool GatewayPage::validatePage()
{
	switch (FTransportList->currentItem()->data(0, Qt::UserRole).toInt())
	{
		case Unavailable:
			if (QMessageBox::warning(wizard(), tr("Warning!"),
											   tr("The transport you selected is unavailable now. "
												  "An attempt to register at it will probably fail. "
												  "Press \"Ok\" to proceed anyway or \"Cancel\" to select another transport."), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Cancel)
				return false;
			break;

		case Unknown:
			if (QMessageBox::warning(wizard(), tr("Warning!"),
											   tr("The transport you selected is untested yet. "
												  "An attempt to register at it will possibly fail. "
												  "Press \"Ok\" to proceed anyway or \"Cancel\" to select another transport."), QMessageBox::Ok, QMessageBox::Cancel)==QMessageBox::Cancel)
				return false;
			break;

		case Available:;
	}
	return true;
}

//!--to make the file download session once-
void GatewayPage::loadTransportList()
{
	FTransportList->clear();
	QDir dir(FIconStorageWizards->resourcesDirs().first());
	dir.cd(FIconStorageWizards->storage());
	dir.cd(FIconStorageWizards->subStorage());
	QFile file(dir.absoluteFilePath(TRANSPORT_LIST));	
	if(file.open(QFile::ReadOnly))
	{
		QDomDocument doc;
		if(doc.setContent(file.readAll(), true))
		{
			FIconStorageServices = IconStorage::staticStorage(RSR_STORAGE_SERVICEICONS);
			FTransportList->sortItems(0, Qt::AscendingOrder);
			QStringList headers;
			headers << tr("Gateway") << tr("Software");

			FTransportList->setHeaderLabels(headers);
			FTransportList->sortItems(0, Qt::AscendingOrder);
			FTransportList->setColumnWidth(0,180);
			FTransportList->setColumnWidth(1,180);

			FExceptFields.clear();
			for (QDomElement gateway = doc.documentElement().firstChildElement("gateway");
				 !gateway.isNull();
				 gateway = gateway.nextSiblingElement("gateway"))
			{
				if(gateway.attribute("name")== FNetwork)
					for(QDomElement server=gateway.firstChildElement("server"); !server.isNull(); server = server.nextSiblingElement("server"))
					{
						QString jid = server.firstChildElement("jid").text();
						if (jid != field(WF_TRANSPORT_FROM).toString())
						{
							QTreeWidgetItem *item = new QTreeWidgetItem(FTransportList);
							item->setText(0, jid);
							item->setData(0, SelectableTreeWidget::ValueRole, jid);
							FPendingItemsListed.append(jid);
							if (FServiceDiscovery->hasDiscoInfo(FStreamJid, jid))
								setItemStatus(item, FServiceDiscovery->findIdentity(FServiceDiscovery->discoInfo(FStreamJid, jid).identity, "gateway", FNetwork)==-1?Unavailable:Available);
							else
							{
								FServiceDiscovery->requestDiscoInfo(FStreamJid, jid);
								setItemStatus(item, Unknown);
							}
							item->setText(1, server.firstChildElement("software").text());
							//!---------------
							if(!server.firstChildElement("exception").isNull())
							{
								QHash<QString, QHash<QString, QString> > except;
								QDomElement exception = server.firstChildElement("exception");
								for(QDomElement field=exception.firstChildElement("field"); !field.isNull(); field = field.nextSiblingElement("field"))
								{
									QHash<QString, QString> fieldHash;
									QDomNamedNodeMap attributes = field.attributes();
									QString var;
									for (int i=0; i<attributes.count(); ++i)
									{
										QDomAttr attribute = attributes.item(i).toAttr();
										if (attribute.name() == "var")
											var = attribute.value();
										else
											fieldHash.insert(attribute.name(), attribute.value());
									}
									if (!var.isEmpty())
										except.insert(var, fieldHash);
								}
								if (exception.hasAttribute("class"))
								{
									QHash<QString, QString> classHash;
									classHash.insert("class", exception.attribute("class"));
									except.insert("class", classHash);
								}

								FExceptFields.insert(server.firstChildElement("jid").text(), except);
							}
						}
					}
			}
		}
	}
}

void GatewayPage::appendLocalTransport(const IDiscoInfo &ADiscoInfo)
{
	int identity = FServiceDiscovery->findIdentity(ADiscoInfo.identity, "gateway", FNetwork);
	if (identity!=-1 && (ADiscoInfo.contactJid.full() != field(WF_TRANSPORT_FROM).toString()))
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(FTransportList);
		item->setText(0, ADiscoInfo.contactJid.full());
		item->setData(0, SelectableTreeWidget::ValueRole, ADiscoInfo.contactJid.full());
		item->setText(1, ADiscoInfo.identity.at(identity).name);
		setItemStatus(item, Available);
	}
}

void GatewayPage::onDiscoInfoReceived(const IDiscoInfo &ADiscoInfo)
{
	if (FPendingItems.contains(ADiscoInfo.contactJid))
	{
		appendLocalTransport(ADiscoInfo);
		FPendingItems.removeOne(ADiscoInfo.contactJid);
	}
	else if (FPendingItemsListed.contains(ADiscoInfo.contactJid))
	{
		QList<QTreeWidgetItem *> found = FTransportList->findItems(ADiscoInfo.contactJid.full(), Qt::MatchExactly, 0);
		if (!found.isEmpty())
			setItemStatus(found.at(0), ADiscoInfo.error.isNull()?Available:Unavailable);
		FPendingItemsListed.removeOne(ADiscoInfo.contactJid);
	}
}

void GatewayPage::onDiscoItemsReceived(const IDiscoItems &ADiscoItems)
{
	if (ADiscoItems.streamJid == FStreamJid && ADiscoItems.contactJid == FStreamJid.domain())
	{
		FDiscoItems = ADiscoItems;
		for (QList<IDiscoItem>::ConstIterator it=FDiscoItems.items.constBegin(); it!=FDiscoItems.items.constEnd(); it++)
			if (FTransportList->findItems((*it).itemJid.full(), Qt::MatchExactly).isEmpty())
			{
				if (FServiceDiscovery->hasDiscoInfo(FStreamJid, (*it).itemJid))
					appendLocalTransport(FServiceDiscovery->discoInfo(FStreamJid, (*it).itemJid));
				else
				{
					FPendingItems.append((*it).itemJid);
					FServiceDiscovery->requestDiscoInfo(FStreamJid, (*it).itemJid, (*it).node);
				}
			}
	}
}

void GatewayPage::setItemStatus(QTreeWidgetItem *AItem, GatewayPage::TransportStatus AStatus)
{
	AItem->setData(0, Qt::UserRole, AStatus);
	switch (AStatus)
	{
		case Available:
			AItem->setIcon(0, FIconStorageWizards->getIcon(WZI_YES));
			break;
		case Unavailable:
			AItem->setIcon(0, FIconStorageWizards->getIcon(WZI_NO));
			break;
		case Unknown:
			AItem->setIcon(0, FIconStorageServices->getIcon(SRI_SERVICE_WAIT));
			break;
	}
}

//!------------------------------
ProcessPage::ProcessPage(Jid &AStreamJid, IRegistration *ARegistration, GatewayPage *AGatewayPage, QWidget *parent):
	QWizardPage(parent),
	FStreamJid(AStreamJid),
	FRegistration(ARegistration),
	FGatewayPage(AGatewayPage),
	FLayout(NULL),
	FFieldWidth(300)
{
	QString style="style='color:blue;'";
	setTitle(QString("<span %2>%1</span>").arg(tr("The registration page")).arg(style));
	setSubTitle(QString("<span %2>%1</span>").arg(tr("Fill in the fields")).arg(style));

	FScrollArea=new QScrollArea;
	//scrollArea->setBackgroundRole(QPalette::Dark);
	FScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	FScrollArea->setWidgetResizable(true);
	FScrollArea->resize(460,300);

	FInstrLabel=new QLabel;
	FInstrLabel->setWordWrap(true);
	FInstrLabel->setAlignment(Qt::AlignHCenter);

	FErrorLabel=new QLabel;
	FErrorLabel->setWordWrap(true);
	FErrorLabel->setAlignment(Qt::AlignHCenter);

	FAutoRegCheckBox= new QCheckBox(tr("Automatically accept subscription requests"));
	registerField(WF_AUTO_SUBSCRIBE, FAutoRegCheckBox);
	FAutoRegCheckBox->setChecked(true);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->insertWidget(0, FInstrLabel);
	mainLayout->insertWidget(1, FScrollArea);
	mainLayout->insertWidget(2, FAutoRegCheckBox);
	mainLayout->insertWidget(3, FErrorLabel);
	setLayout(mainLayout);

	localTextLabel();

	connect(FRegistration->instance(),SIGNAL(registerFields(const QString &, const IRegisterFields &)),
		SLOT(onRegisterFields(const QString &, const IRegisterFields &)));
	connect(FRegistration->instance(),SIGNAL(registerError(const QString &, const XmppError &)),
		SLOT(onRegisterError(const QString &, const XmppError &)));
	connect(this, SIGNAL(oldFieldsReceived()), SLOT(onOldFieldsReceived()));
}

void ProcessPage::createGateway()
{
	if (field(WF_AUTO_SUBSCRIBE).toBool())
	{
		qobject_cast<TransportWizard*>(wizard())->setAutoSubscribe(Options::node(OPV_ROSTER_AUTOSUBSCRIBE).value().toBool());
		Options::node(OPV_ROSTER_AUTOSUBSCRIBE).setValue(true);
	}

	FServiceTo = Jid::fromUserInput(field(WF_TRANSPORT_TO).toString().trimmed());

	if (field(WF_TRANSPORT_FROM).isNull())
	{
		FRequestIdTo = FRegistration!=NULL ? FRegistration->sendRegisterRequest(FStreamJid, FServiceTo) : QString::null;
		if (!FRequestIdTo.isEmpty())
		{
			QString style="style='color:brown;'";
			FInstrLabel->setText(QString("<span %1>%2</span>").arg(style).arg(tr("Waiting for host response ...")));
		}
		else
		{
			QString style="style='color:red;'";
			FInstrLabel->setText(QString("<span %1>%2</span>").arg(style).arg(tr("Error: Can't send request to host.")));
		}
	}
	else
	{
		FServiceFrom = Jid::fromUserInput(field(WF_TRANSPORT_FROM).toString().trimmed());
		FRequestIdTo = FRegistration->sendRegisterRequest(FStreamJid, FServiceTo);
		FNewFieldsReceived = false;
		FRequestIdFrom = FRegistration->sendRegisterRequest(FStreamJid, FServiceFrom);
		FOldFields = IRegisterFields();
		FOldFieldsReceived = false;
	}
}

void ProcessPage::onRegisterFields(const QString &AId, const IRegisterFields &AFields)
{
	if (AId == FRequestIdTo)	// Registration fields
	{
		FSubmit.serviceJid = FServiceTo;
		FSubmit.fieldMask = AFields.fieldMask;
		FSubmit.key = AFields.key;

		FTmpFields.clear();
		FExceptFields = FGatewayPage->exceptFields();

		QString	instructions;

		bool disable = !field(WF_TRANSPORT_FROM).isNull();

		if(AFields.form.type.isEmpty())
		{
			FHasForm=false;			
			if (!AFields.instructions.isEmpty())
				instructions=AFields.instructions;

			if(AFields.fieldMask & IRegisterFields::Username)
			{
				QLabel		*label = new QLabel(tr("User Name"));
				QLineEdit	*lineEdit = new QLineEdit;
				QHBoxLayout *layout = new QHBoxLayout;
				FLayout->addLayout(layout);
				if (disable)
					lineEdit->setEnabled(false);
				lineEdit->setObjectName("username");
				registerField("username*", lineEdit);
				layout->addWidget(label);//,Qt::AlignLeft
				layout->addWidget(lineEdit);
				connect(lineEdit, SIGNAL(textChanged(QString)), SLOT(onTextChanged(QString)));
				if(!AFields.username.isNull())
					lineEdit->setText(AFields.username);
			}
			if(AFields.fieldMask & IRegisterFields::Password)
			{
				QLabel		*label = new QLabel(tr("Password"));
				QLineEdit	*lineEdit = new QLineEdit;
				QHBoxLayout *layout = new QHBoxLayout;
				FLayout->addLayout(layout);
				if (disable)
					lineEdit->setEnabled(false);
				lineEdit->setEchoMode(QLineEdit::Password);
				lineEdit->setObjectName("password");
				registerField("password*", lineEdit);
				layout->addWidget(label);//,Qt::AlignLeft
				layout->addWidget(lineEdit);
				connect(lineEdit, SIGNAL(textChanged(QString)), SLOT(onTextChanged(QString)));
				if(!AFields.password.isNull())
					lineEdit->setText(AFields.password);
			}
			if(AFields.fieldMask & IRegisterFields::Email)
			{
				QLabel		*label = new QLabel(tr("e-mail"));
				QLineEdit	*lineEdit = new QLineEdit;
				QHBoxLayout *layout = new QHBoxLayout;
				FLayout->addLayout(layout);
				if (disable)
					lineEdit->setEnabled(false);
				lineEdit->setObjectName("email");
				layout->addWidget(label);
				layout->addWidget(lineEdit);
				connect(lineEdit, SIGNAL(textChanged(QString)), SLOT(onTextChanged(QString)));
				if(!AFields.email.isNull())
					lineEdit->setText(AFields.email);
			}
			if(AFields.fieldMask & IRegisterFields::Redirect)
			{
				QLabel		*label = new QLabel(tr("Web registration required"));
				QCommandLinkButton	*commandLinkButton = new QCommandLinkButton(tr("Open registration page"), AFields.redirect.toString());
				QHBoxLayout *layout = new QHBoxLayout;
				FLayout->addLayout(layout);
				layout->addWidget(label);
				layout->addWidget(commandLinkButton);
				connect(commandLinkButton, SIGNAL(clicked(bool)), SLOT(onOpenPage()));
			}
		}
		else //! form.type = "form"
		{
			FHasForm=true;
			if (!AFields.form.instructions.isEmpty())   //! many instr...
				instructions=AFields.form.instructions[0];
			QList<IDataField> fields= AFields.form.fields;
			for(QList<IDataField>::ConstIterator it=fields.constBegin(); it!=fields.constEnd(); it++)
			{
				if((*it).type==FIELD_TYPE_HIDDEN)    //! similar to an <INPUT> tag
				{
					if((*it).var=="FORM_TYPE")
						FTmpFields.insert((*it).var, (*it).value.toString());
				}
				else
					addWidget(*it, FServiceTo, FLayout, disable);
			}
		}
		FLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
		FInstrLabel->setText(QString("%1").arg(instructions));
		FNewFieldsReceived = true;
		emit completeChanged();
		if (FOldFieldsReceived)
			QTimer::singleShot(0, this, SLOT(onOldFieldsReceived()));
	}
	else if (AId == FRequestIdFrom)	// Old fields
	{
		FOldFields = AFields;
		FOldFieldsReceived = true;
		if (FNewFieldsReceived)
			emit oldFieldsReceived();
	}
}

void ProcessPage::onRegisterError(const QString &AId, const XmppError &AError)
{
	if (AId == FRequestIdTo)
	{
		//! "Remote server not found"
		QString style="style='color:red;'";
		FErrorLabel->setText(QString("<h2>%1:<span %2><br/>%3</span></h2>")
							 .arg(tr("Requested operation failed")).arg(style).arg(AError.errorMessage()));
		FInstrLabel->clear();
		FAutoRegCheckBox->setVisible(false);
	}
	else if (AId == FRequestIdFrom)
	{
		if (FNewFieldsReceived)
			emit oldFieldsReceived();
	}
}

void ProcessPage::onOldFieldsReceived()
{
	if (FOldFields.form.type.isEmpty())
	{
		if (!FOldFields.username.isEmpty())
		{
			QLineEdit *lineEdit = findChild<QLineEdit*>("username");
			if (lineEdit)
				lineEdit->setText(FOldFields.username);
		}
		if (!FOldFields.password.isEmpty())
		{
			QLineEdit *lineEdit = findChild<QLineEdit*>("password");
			if (lineEdit)
				lineEdit->setText(FOldFields.password);
		}
		if (!FOldFields.email.isEmpty())
		{
			QLineEdit *lineEdit = findChild<QLineEdit*>("email");
			if (lineEdit)
				lineEdit->setText(FOldFields.email);
		}
	}
	else
	{
		for (QList<IDataField>::ConstIterator it=FOldFields.form.fields.constBegin(); it!=FOldFields.form.fields.constEnd(); ++it)
		{
			if((*it).type==FIELD_TYPE_BOOLEAN && (*it).value.type()==QVariant::Bool)				
			{
				QCheckBox *checkBox = findChild<QCheckBox*>((*it).var);
				if (checkBox)
					checkBox->setChecked((*it).value.toBool());
			}
			else if(((*it).type==FIELD_TYPE_TEXTSINGLE || (*it).type==FIELD_TYPE_TEXTPRIVATE) && (*it).value.type()==QVariant::String)
			{
				QLineEdit *lineEdit = findChild<QLineEdit*>((*it).var);
				if (lineEdit)
					lineEdit->setText((*it).value.toString());
			}
			else if(((*it).type==FIELD_TYPE_LISTSINGLE || (*it).type==FIELD_TYPE_JIDSINGLE) && (*it).value.type()==QVariant::String)
			{
				QComboBox *comboBox = findChild<QComboBox*>((*it).var);
				if (comboBox)
					comboBox->setEditText((*it).value.toString());
			}
			else if(((*it).type==FIELD_TYPE_TEXTMULTI || (*it).type==FIELD_TYPE_JIDMULTI) && (*it).value.type()==QVariant::String)
			{
				QTextEdit *textEdit = findChild<QTextEdit*>((*it).var);
				if (textEdit)
					textEdit->setText((*it).value.toString());
			}
			else if((*it).type==FIELD_TYPE_LISTMULTI && (*it).value.type()==QVariant::StringList)
			{
				QListWidget *listWidget = findChild<QListWidget*>((*it).var);
				if (listWidget)
				{
					QStringList values = (*it).value.toStringList();
					for (int i=0; i<listWidget->count(); i++)
					{
						QListWidgetItem *item = listWidget->item(i);
						item->setCheckState(values.contains(item->data(Qt::UserRole).toString()) ? Qt::Checked  : Qt::Unchecked);
					}
				}
			}
		}
	}

	// Enable all widgets
	int layoutCount = FLayout->count();
	for (int i = 0; i<layoutCount; ++i)
	{
		QLayout *layout = FLayout->itemAt(i)->layout();
		if (layout)
		{
			int widgetCount = layout->count();
			for (int j = 0; j<widgetCount; ++j)
			{
				QWidget *widget = layout->itemAt(j)->widget();
				if (widget && !widget->isEnabled())
					widget->setEnabled(true);
			}
		}
	}
}

void ProcessPage::onOpenPage()
{
	QCommandLinkButton *linkButton = qobject_cast<QCommandLinkButton *>(sender());
	if (linkButton)
		QDesktopServices::openUrl(QUrl(linkButton->description()));
}

void ProcessPage::initializePage()
{
	FScrollArea->setWidget(new QWidget);
	FScrollArea->widget()->setLayout(FLayout = new QVBoxLayout);
	createGateway();
}

//! bool required; QString var; QString type; QString desc; QVariant value;
//! IDataMedia media;IDataValidate validate; QList<IDataOption> options;
void ProcessPage::addWidget(const IDataField &AField, const Jid &ATransportJid, QVBoxLayout *ALayout, bool ADisable)
{
	QString  var	= AField.var;
	QVariant value	= AField.value;
	QLabel  *lblLabel = NULL;

	// Process exceptions
	if (FExceptFields.contains(ATransportJid.full()))
	{
		QHash<QString, QHash<QString, QString> > fields = FExceptFields.value(ATransportJid.full());
		if (fields.contains(var))
		{
			QHash<QString, QString> field = fields.value(var);
			if (field.contains("value"))
				FTmpFields.insert(var, field.value("value"));
			else
				FTmpFields.insert(var, value);

			if (fields.value("class").value("class")=="vk4xmpp")
			{
				if (var == "link" || var == "password")
				{
					QVBoxLayout *layout = new QVBoxLayout();
					ALayout->addLayout(layout);
					lblLabel = new QLabel();
					lblLabel->setWordWrap(true);						
					layout->addWidget(lblLabel);
					if (var == "link")
					{
						lblLabel->setText(tr("Press the button to open vKontakte logon page in the system browser. "
											 "Proceed logon (if not logged on already). "
											 "After you see a blank page with a single text message on it, "
											 "copy contents of browser's address line and close web browser."));
						QCommandLinkButton *linkButton = new QCommandLinkButton("Open registration page", AField.value.toString());
						layout->addWidget(lblLabel, 0, 0);
						layout->addWidget(linkButton, 1, 0);
						connect(linkButton, SIGNAL(clicked(bool)), SLOT(onOpenPage()));
					}
					else if (var == "password")
					{
						lblLabel->setText(tr("Insert copied browser's address line here"));
						QLineEdit *textEdit=new QLineEdit;
						textEdit->setEchoMode(QLineEdit::Password);
						textEdit->setObjectName(var);
						textEdit->setEnabled(false);
						if (ADisable)
							textEdit->setEnabled(false);
						layout->addWidget(textEdit);
						registerField(AField.var+"*", textEdit);
						connect(textEdit,SIGNAL(textChanged(QString)),SLOT(onTextChanged(QString)));
					}
				}
			}
			return;
		}
	}

	QHBoxLayout *layout = new QHBoxLayout();
	ALayout->addLayout(layout);

	QString  descr	= !AField.desc.isEmpty() ? QString("<span>%1</span>").arg(HTML_ESCAPE(AField.desc)) : QString::null;
	QString  label	= !AField.label.isEmpty() ? AField.label : AField.desc;
	if(AField.type==FIELD_TYPE_BOOLEAN)
	{
		QCheckBox *checkBox=new QCheckBox;
		checkBox->setObjectName(var);
		checkBox->setText(label);
		checkBox->setToolTip(descr);
		if (ADisable)
			checkBox->setEnabled(false);
		checkBox->setChecked(value.toBool());
		FTmpFields.insert(var, value);
		connect(checkBox,SIGNAL(clicked(bool)),SLOT(onCheckBoxClicked(bool)));
		layout->addWidget(checkBox);

	}
	else
	{
		lblLabel = new QLabel(getLocalText(var));
		if (lblLabel->text().isEmpty())
			lblLabel->setText(label);
		lblLabel->setWordWrap(true);
		layout->addWidget(lblLabel);

		if(AField.type==FIELD_TYPE_FIXED )
		{
			QLabel *lbl=new QLabel(value.toString());
			lbl->setWordWrap(true);
			lbl->setTextFormat(Qt::AutoText);
			lbl->setToolTip(descr);
			lbl->setFixedWidth(FFieldWidth);
			layout->addWidget(lbl);
		}
		else if(AField.type==FIELD_TYPE_TEXTSINGLE)
		{
			QLineEdit *lineEdit=new QLineEdit;
			lineEdit->setObjectName(var);
			lineEdit->setToolTip(descr);
			lineEdit->setFixedWidth(FFieldWidth);
			if (ADisable)
				lineEdit->setEnabled(false);
			if (AField.required)
				registerField(AField.var+"*", lineEdit);
			FTmpFields.insert(var,value);
			connect(lineEdit,SIGNAL(textChanged(QString)),SLOT(onTextChanged(QString)));
			layout->addWidget(lineEdit);
			if(!value.isNull())
				lineEdit->setText(value.toString());
		}
		else if(AField.type==FIELD_TYPE_TEXTPRIVATE)
		{
			QLineEdit *lineEdit=new QLineEdit;
			lineEdit->setEchoMode(QLineEdit::Password);
			lineEdit->setObjectName(var);
			lineEdit->setToolTip(descr);
			lineEdit->setFixedWidth(FFieldWidth);
			if (ADisable)
				lineEdit->setEnabled(false);
			if (AField.required)
				registerField(AField.var+"*", lineEdit);				
			FTmpFields.insert(var,value);
			connect(lineEdit,SIGNAL(textChanged(QString)),SLOT(onTextChanged(QString)));
			layout->addWidget(lineEdit);
			if(!value.isNull())
				lineEdit->setText(value.toString());
		}
		else if(AField.type==FIELD_TYPE_LISTSINGLE || AField.type==FIELD_TYPE_JIDSINGLE)
		{
			QComboBox *comBox=new QComboBox;
			comBox->setObjectName(var);
			comBox->setToolTip(descr);
			for(QList<IDataOption>::ConstIterator it=AField.options.constBegin(); it!=AField.options.constEnd(); it++)
				comBox->addItem((*it).label, (*it).value);			
			comBox->setFixedWidth(comBox->sizeHint().width());
			if (ADisable)
				comBox->setEnabled(false);
			if (AField.required)
				registerField(AField.var+"*", comBox);
			FTmpFields.insert(var,value);
			connect(comBox,SIGNAL(currentIndexChanged(QString)),SLOT(onComBoxChanged(QString)));
			layout->addWidget(comBox);
			if(!value.isNull())
				comBox->setEditText(value.toString());
		}
		else if(AField.type==FIELD_TYPE_TEXTMULTI)
		{
			QTextEdit *textEdit=new QTextEdit;
			textEdit->setObjectName(var);
			textEdit->setAcceptRichText(false);
			textEdit->setToolTip(descr);
			textEdit->setFixedWidth(FFieldWidth);
			if (ADisable)
				textEdit->setEnabled(false);
			FTmpFields.insert(var,value);
			if (AField.required)
				registerField(AField.var+"*", textEdit);
			connect(textEdit,SIGNAL(textChanged()),SLOT(onMultiTextChanged()));
			layout->addWidget(textEdit);
			if(!value.isNull())
			{
				QStringList list = value.toStringList();
				for(QStringList::ConstIterator it = list.constBegin(); it!=list.constEnd(); it++)
					textEdit->append(*it);
			}
		}
		else if(AField.type==FIELD_TYPE_JIDMULTI){
			QTextEdit *textEdit=new QTextEdit;
			textEdit->setObjectName(var);
			textEdit->setAcceptRichText(false);
			textEdit->setToolTip(descr);
			textEdit->setFixedWidth(FFieldWidth);
			if (ADisable)
				textEdit->setEnabled(false);
			FTmpFields.insert(var,value);
			if (AField.required)
				registerField(AField.var+"*", textEdit);
			connect(textEdit,SIGNAL(textChanged()),SLOT(onMultiTextChanged()));
			layout->addWidget(textEdit);
			if(!value.isNull())
			{
				QStringList list = value.toStringList();
				for(QStringList::ConstIterator it = list.constBegin(); it!=list.constEnd(); it++)
					textEdit->append(Jid(*it).uFull());
			}
		}
		else if(AField.type==FIELD_TYPE_LISTMULTI)
		{
			QListWidget *listWidget=new QListWidget;
			listWidget->setObjectName(var);
			listWidget->setToolTip(descr);
			listWidget->setFixedWidth(FFieldWidth);
			for(QList<IDataOption>::ConstIterator it=AField.options.constBegin(); it!=AField.options.constEnd(); it++)
			{
				QListWidgetItem *item = new QListWidgetItem((*it).label);
				item->setData(Qt::UserRole, (*it).value);
//				item->setFlags(!true ? Qt::ItemIsEnabled|Qt::ItemIsUserCheckable : Qt::ItemIsEnabled);
				item->setFlags(Qt::ItemIsEnabled);
				listWidget->addItem(item);
			}
			listWidget->setWrapping(true);
			if (ADisable)
				listWidget->setEnabled(false);
			if (AField.required)
				registerField(AField.var+"*", listWidget);
			connect(listWidget,SIGNAL(itemSelectionChanged()),SLOT(onListMultiSelectionChanged(QString)));
			layout->addWidget(listWidget);
			if(!value.isNull())
			{
				QStringList values = value.toStringList();
				for (int i=0; i<listWidget->count(); i++)
				{
					QListWidgetItem *item = listWidget->item(i);
					item->setCheckState(values.contains(item->data(Qt::UserRole).toString()) ? Qt::Checked  : Qt::Unchecked);
				}
			}
		}
		else
			layout->deleteLater();
	}
}
//! FField.validate.method == DATAVALIDATE_METHOD_OPEN
//! FField.validate.type == DATAVALIDATE_TYPE_DATE
//! FField.validate.type == DATAVALIDATE_TYPE_TIME

void ProcessPage::onCheckBoxClicked(bool AState)
{
	QCheckBox *obj= qobject_cast<QCheckBox *>(sender());
	if(obj)
		FTmpFields.insert(obj->objectName(),AState);
}
void ProcessPage::onTextChanged(QString AText)
{
	QLineEdit *obj= qobject_cast<QLineEdit *>(sender());
	if(obj)
		FTmpFields.insert(obj->objectName(), AText);
}
void ProcessPage::onComBoxChanged(QString AText)
{
	QComboBox *obj= qobject_cast<QComboBox *>(sender());
	if(obj)
		FTmpFields.insert(obj->objectName(), AText);
}
void ProcessPage::onListMultiSelectionChanged()
{	
	QListWidget *obj= qobject_cast<QListWidget *>(sender());
	if(obj)
	{
		QStringList selectedItems;
		QList<QListWidgetItem*> items = obj->selectedItems();
		for(QList<QListWidgetItem*>::ConstIterator it=items.constBegin(); it!=items.constEnd(); it++)
			if ((*it)->isSelected())
				selectedItems.append((*it)->data(Qt::UserRole).toString());
		FTmpFields.insert(obj->objectName(), selectedItems);
	}
}
void ProcessPage::onMultiTextChanged()
{
	QTextEdit *obj= qobject_cast<QTextEdit *>(sender());
	if(!obj->document()->isEmpty())
		FTmpFields.insert(obj->objectName(),obj->toPlainText());
}

IRegisterSubmit ProcessPage::getSubmit()
{
	if(!FHasForm)
	{
		FSubmit.username   = FTmpFields.value("username").toString();
		FSubmit.password   = FTmpFields.value("password").toString();
		FSubmit.email      = FTmpFields.value("email").toString();
		FSubmit.form       = IDataForm();
	}
	else
	{
		QList<IDataField> newFields;
		QHashIterator<QString, QVariant> i(FTmpFields);
		while (i.hasNext())
		{			
			IDataField field;
			i.next();
			field.var = i.key();
			field.value = i.value();
			newFields.append(field);
		}
		FForm.fields = newFields;
		FForm.type = "submit";
		if (!FForm.instructions.isEmpty())
			FForm.instructions.clear();
		if (!FForm.title.isEmpty())
			FForm.title.clear();
		FSubmit.form = FForm;
	}
	return FSubmit;
}

QString ProcessPage::getLocalText(QString AKey)
{
	if(FLocalText.value(AKey).isEmpty())
		return AKey;
	else
		return FLocalText.value(AKey);
}

int ProcessPage::nextId() const
{
	return TransportWizard::Page_Result;
}

//!------------------------------
ResultPage::ResultPage(Jid &AStreamJid, IRegistration *ARegistration, ProcessPage *AProcess, QWidget *parent):
	QWizardPage(parent),
	FStreamJid(AStreamJid),
	FRegistration(ARegistration),
	FProcess(AProcess),
	FWizardGo(false)
{
	QString style="style='color:blue;'";
	setTitle(QString("<span %2>%1</span>").arg(tr("Result Page")).arg(style));
	setSubTitle(QString("<span %2>%1</span>").arg(tr("Result Page")).arg(style));

	FErrorLabel =new QLabel;
	FErrorLabel->setWordWrap(true);
	FErrorLabel->setAlignment(Qt::AlignHCenter);

	FLayout = new QVBoxLayout;
	setLayout(FLayout);

	connect(FRegistration->instance(),SIGNAL(registerError(const QString &, const XmppError &)),
		SLOT(onRegisterError(const QString &, const XmppError &)));
	connect(FRegistration->instance(),SIGNAL(registerSuccess(const QString &)),
		SLOT(onRegisterSuccessful(const QString &)));
}

void ResultPage::initializePage()
{
	IGateways *gateways = PluginHelper::pluginInstance<IGateways>();
	FSubmit = FProcess->getSubmit();
	if (field(WF_TRANSPORT_FROM).isNull() || gateways->changeService(FStreamJid, field(WF_TRANSPORT_FROM).toString(), field(WF_TRANSPORT_TO).toString(), true, true))
		FRequestId = FRegistration->sendRequestSubmit(FStreamJid,FSubmit);
	else
	{
		FLayout->addWidget(FErrorLabel);
		FWizardGo=true;
		FErrorLabel->setText(QString("<h2>%1</h2>").arg(tr("Cannot change service")));
		FNextId = -1;
		emit completeChanged();
	}
}

void ResultPage::onRegisterError(const QString &AId, const XmppError &AError)
{
	if (FRequestId == AId)
	{
		FLayout->addWidget(FErrorLabel);
		QString style="style='color:red;'";
		FErrorLabel->setText(QString("<h2>%1 <span %2><br/>%3</span></h2>")
				 .arg(tr("Requested operation failed:")).arg(style).arg(AError.errorMessage()));
		if(AError.errorString()=="Undefined error condition")
			setButtonText(QWizard::BackButton,tr("Retry"));
	}
}

void ResultPage::onRegisterSuccessful(const QString &AId)
{
	if (FRequestId == AId)
	{
		FNextId = TransportWizard::Page_Conclusion;
		wizard()->next();
	}
}

bool ResultPage::isComplete() const
{
	return FWizardGo;
}

//!------------------------------
ConclusionPage::ConclusionPage(NetworksPage *ANetworkPage, QWidget *parent):
	QWizardPage(parent),
	FNetworkPage(ANetworkPage)
{
	QString style="style='color:blue;'";
	setTitle(QString("<span %1>%2</span>").arg(style).arg(tr("Done!")));
	setSubTitle(QString("<span %1>%2</span>").arg(style).arg(tr("Transport Wizard completed successfuly")));

	QString fileName = IconStorage::staticStorage(RSR_STORAGE_WIZARDS)->fileFullName(WZI_TRANSPORT_END);
	setPixmap(QWizard::WatermarkPixmap, QPixmap(fileName));
	FLblTitle = new QLabel(QString("<center><h2 style='color:green;'>%2</h2></center>").arg(tr("Congratulations!")));
	FLblTitle->setWordWrap(true);

	FLblText1 = new QLabel;
	FLblText1->setWordWrap(true);

	FLblText2 = new QLabel(QString("<b>%1: </b>%2").arg(tr("Attention"))
						   .arg(tr("Some transports may report about successful registreation, even if wrong credentials were provided. "
								   "They may inform you about authentication error later, with a message.\n"
								   "If you receive such message, please rerun the Wizard to register at transport with correct credentials.")));
	FLblText2->setWordWrap(true);

	FLblText3 = new QLabel(tr("Press \"Finish\" button to close Wizard.", "\"Finish\" should match the text of an appropriate Qt Wizard button"));
	FLblText3->setWordWrap(true);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(FLblTitle);
	layout->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Fixed));
	layout->addWidget(FLblText1);
	layout->addWidget(FLblText2);
	layout->addWidget(FLblText3);
	setLayout(layout);
}

int ConclusionPage::nextId() const
{
	return -1;
}

void ConclusionPage::initializePage()
{
	FLblText1->setText(QString("<span style='align:justify'>%1</span>")
					  .arg(field(WF_TRANSPORT_FROM).isNull()?
							tr("You successfuly connected to %1 via %2.").arg(FNetworkPage->networkName(field(WF_NETWORK).toString()))
																		 .arg(field(WF_TRANSPORT_TO).toString()):
							tr("You successfuly changed %1 transport from %2 to %3.").arg(FNetworkPage->networkName(field(WF_NETWORK).toString()))
																			.arg(field(WF_TRANSPORT_FROM).toString())
																			.arg(field(WF_TRANSPORT_TO).toString())
						   ));
}

void ProcessPage::localTextLabel()
{
	FLocalText.insert("address",tr("Street"));
	FLocalText.insert("action",tr("Select Action"));
	FLocalText.insert("birthyear",tr("Birth Year"));
	FLocalText.insert("born",tr("Born"));
	FLocalText.insert("city",tr("City"));
	FLocalText.insert("connections_params",tr("Connections Parameters"));
	FLocalText.insert("email",tr("Email Address"));
	FLocalText.insert("encoding",tr("Encoding"));
	FLocalText.insert("first",tr("Given Name"));
	FLocalText.insert("firstname",tr("First Name"));
	FLocalText.insert("familyname",tr("Family Name"));
	FLocalText.insert("familycity",tr("Family City"));
	FLocalText.insert("friends_only",tr("Friends Only"));
	FLocalText.insert("invisible",tr("Invisible"));
	FLocalText.insert("gender",tr("Gender"));
	FLocalText.insert("lastname",tr("Last Name"));
	FLocalText.insert("last",tr("Family Name"));
	FLocalText.insert("language",tr("Language"));
	FLocalText.insert("link",tr("Link"));
	FLocalText.insert("locale",tr("Language"));
	FLocalText.insert("nick",tr("Nick Name"));
	FLocalText.insert("name",tr("Full Name"));
	FLocalText.insert("Password",tr("Password"));
	FLocalText.insert("password",tr("Password"));
	FLocalText.insert("phone",tr("Phone Number"));
	FLocalText.insert("state",tr("Region"));
	FLocalText.insert("uin",tr("UIN"));
	FLocalText.insert("unregister",tr("Unregister"));
	FLocalText.insert("url",tr("Your Web Page"));
	FLocalText.insert("username",tr("User Name"));
	FLocalText.insert("use_password",tr("Access-token"));
	FLocalText.insert("userlist",tr("User List"));
	FLocalText.insert("zip",tr("Zip Code"));
}
