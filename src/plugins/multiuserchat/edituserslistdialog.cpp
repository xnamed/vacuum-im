#include "edituserslistdialog.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QInputDialog>
#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <utils/iconstorage.h>
#include <utils/logger.h>
#include "utils/qt4qt5compat.h"

#define TIDR_ITEMJID    Qt::UserRole+1
#define JID_COLUMN_NUM 0

EditUsersListDialog::EditUsersListDialog(const QString &AAffiliation, const QList<IMultiUserListItem> &AList, QWidget *AParent) : QDialog(AParent)
{
	REPORT_VIEW;
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose,true);
	FAffiliation = AAffiliation;

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
    QPixmap pixmap;
    if (AAffiliation == MUC_AFFIL_OUTCAST)
        pixmap=IconStorage::getStoragePixmap(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->fileFullName(MNI_MUC_EDIT_BAN_LIST));
    else if (AAffiliation == MUC_AFFIL_MEMBER)
        pixmap=IconStorage::getStoragePixmap(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->fileFullName(MNI_MUC_EDIT_MEMBERS_LIST));
    else if (AAffiliation == MUC_AFFIL_ADMIN)
        pixmap=IconStorage::getStoragePixmap(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->fileFullName(MNI_MUC_EDIT_ADMINS_LIST));
    else if (AAffiliation == MUC_AFFIL_OWNER)
        pixmap=IconStorage::getStoragePixmap(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->fileFullName(MNI_MUC_EDIT_OWNERS_LIST));
    ui.lblIcon->setPixmap(pixmap);
#else
// *** >>> eyeCU >>> ***
	if (AAffiliation == MUC_AFFIL_OUTCAST)
		IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->insertAutoIcon(this,MNI_MUC_EDIT_BAN_LIST,0,0,"windowIcon");
	else if (AAffiliation == MUC_AFFIL_MEMBER)
		IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->insertAutoIcon(this,MNI_MUC_EDIT_MEMBERS_LIST,0,0,"windowIcon");
	else if (AAffiliation == MUC_AFFIL_ADMIN)
		IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->insertAutoIcon(this,MNI_MUC_EDIT_ADMINS_LIST,0,0,"windowIcon");
	else if (AAffiliation == MUC_AFFIL_OWNER)
		IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->insertAutoIcon(this,MNI_MUC_EDIT_OWNERS_LIST,0,0,"windowIcon");
#endif

	int row = 0;
	ui.tbwTable->setRowCount(AList.count());
	foreach(const IMultiUserListItem &listItem, AList)
	{
		Jid itemJid = listItem.jid;
		QTableWidgetItem *jidItem = new QTableWidgetItem();
		jidItem->setText(itemJid.uFull());
		jidItem->setData(TIDR_ITEMJID, itemJid.full());
		jidItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		ui.tbwTable->setItem(row,0,jidItem);
		if (FAffiliation == MUC_AFFIL_OUTCAST)
		{
			QTableWidgetItem *reasonItem = new QTableWidgetItem(listItem.notes);
			reasonItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			ui.tbwTable->setItem(jidItem->row(),1,reasonItem);
		}
		row++;
		FCurrentItems.insert(itemJid,jidItem);
	}
	ui.tbwTable->horizontalHeader()->setHighlightSections(false);

	if (AAffiliation == MUC_AFFIL_OUTCAST)
	{
		ui.tbwTable->horizontalHeader()->SETRESIZEMODE(0,QHeaderView::ResizeToContents);
		ui.tbwTable->horizontalHeader()->SETRESIZEMODE(1,QHeaderView::Stretch);
	}
	else
	{
		ui.tbwTable->hideColumn(1);
		ui.tbwTable->horizontalHeader()->SETRESIZEMODE(0,QHeaderView::Stretch);
	}
	// default sort to JID column
	ui.tbwTable->setSortingEnabled(true);
	ui.tbwTable->sortByColumn(JID_COLUMN_NUM, Qt::AscendingOrder);

	connect(ui.pbtAdd,SIGNAL(clicked()),SLOT(onAddClicked()));
	connect(ui.pbtDelete,SIGNAL(clicked()),SLOT(onDeleteClicked()));
}

EditUsersListDialog::~EditUsersListDialog()
{

}

QString EditUsersListDialog::affiliation() const
{
	return FAffiliation;
}

QList<IMultiUserListItem> EditUsersListDialog::deltaList() const
{
	QList<IMultiUserListItem> result;
	foreach(QTableWidgetItem *jidItem, FAddedItems)
	{
		IMultiUserListItem listItem;
		listItem.jid = jidItem->data(TIDR_ITEMJID).toString();
		listItem.affiliation = FAffiliation;
		if (FAffiliation == MUC_AFFIL_OUTCAST)
			listItem.notes = ui.tbwTable->item(jidItem->row(),1)->text();
		result.append(listItem);
	}
	foreach(const Jid &userJid, FDeletedItems)
	{
		IMultiUserListItem listItem;
		listItem.jid = userJid.full();
		listItem.affiliation = MUC_AFFIL_NONE;
		result.append(listItem);
	}
	return result;
}

void EditUsersListDialog::setTitle(const QString &ATitle)
{
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
    ui.lblTitle->setText(ATitle);
#else
// *** >>> eyeCU >>> ***
	setWindowTitle(ATitle);
#endif
}

void EditUsersListDialog::onAddClicked()
{
	Jid userJid = Jid::fromUserInput(QInputDialog::getText(this,tr("Add new item"),tr("Enter new item JID:")));
	if (userJid.isValid() && !FCurrentItems.contains(userJid))
	{
		int row = ui.tbwTable->rowCount();
		ui.tbwTable->setRowCount(row+1);
		QTableWidgetItem *jidItem = new QTableWidgetItem();
		jidItem->setText(userJid.uFull());
		jidItem->setData(TIDR_ITEMJID,userJid.full());
		jidItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		ui.tbwTable->setItem(row,0,jidItem);
		if (FAffiliation == MUC_AFFIL_OUTCAST)
		{
			QTableWidgetItem *reasonItem = new QTableWidgetItem;
			reasonItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
			ui.tbwTable->setItem(jidItem->row(),1,reasonItem);
			ui.tbwTable->horizontalHeader()->resizeSection(0,QHeaderView::ResizeToContents);
		}
		ui.tbwTable->setCurrentItem(jidItem);
		FDeletedItems.removeAll(userJid);
		FAddedItems.insert(userJid,jidItem);
		FCurrentItems.insert(userJid,jidItem);
	}
	else if (!userJid.isEmpty())
	{
		QMessageBox::warning(this,tr("Wrong item JID"),tr("Entered item JID is not valid or already exists."));
	}
}

void EditUsersListDialog::onDeleteClicked()
{
	QTableWidgetItem *tableItem = ui.tbwTable->currentItem();
	if (tableItem)
	{
		Jid userJid = ui.tbwTable->item(tableItem->row(),0)->data(TIDR_ITEMJID).toString();
		if (!FAddedItems.contains(userJid))
			FDeletedItems.append(userJid);
		else
			FAddedItems.remove(userJid);
		FCurrentItems.remove(userJid);
		ui.tbwTable->removeRow(tableItem->row());
	}
}
