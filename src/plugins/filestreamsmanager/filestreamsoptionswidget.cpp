#include "filestreamsoptionswidget.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <definitions/optionvalues.h>
#include <utils/options.h>

FileStreamsOptionsWidget::FileStreamsOptionsWidget(IFileStreamsManager *AFileManager, QWidget *AParent) : QWidget(AParent)
{
	ui.setupUi(this);
	FFileManager = AFileManager;

	connect(ui.tlbDirectory, SIGNAL(clicked()), SLOT(onDirectoryButtonClicked()));
#ifdef EYECU_MOBILE
    connect(ui.lneDirectory,SIGNAL(textChanged()),SIGNAL(modified()));
#else
    connect(ui.lneDirectory,SIGNAL(textChanged(const QString &)),SIGNAL(modified()));
#endif
	reset();
}

void FileStreamsOptionsWidget::apply()
{
#ifdef EYECU_MOBILE
    Options::node(OPV_FILESTREAMS_DEFAULTDIR).setValue(ui.lneDirectory->toPlainText());
#else
    Options::node(OPV_FILESTREAMS_DEFAULTDIR).setValue(ui.lneDirectory->text());
#endif
	emit childApply();
}

void FileStreamsOptionsWidget::reset()
{
	ui.lneDirectory->setText(Options::node(OPV_FILESTREAMS_DEFAULTDIR).value().toString());
	emit childReset();
}

void FileStreamsOptionsWidget::onDirectoryButtonClicked()
{
#ifdef EYECU_MOBILE
    QString dir = QFileDialog::getExistingDirectory(this,tr("Select default directory"), ui.lneDirectory->toPlainText());
#else
    QString dir = QFileDialog::getExistingDirectory(this,tr("Select default directory"), ui.lneDirectory->text());
#endif
    if (!dir.isEmpty())
		ui.lneDirectory->setText(dir);
}
