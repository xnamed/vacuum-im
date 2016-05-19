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

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
void FileStreamsOptionsWidget::onDirectoryButtonClicked()
{
    FSelDirName.clear();
	QFileDialog dialog(this,tr("Select default directory"),ui.lneDirectory->toPlainText());
	dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setViewMode(QFileDialog::List);
    connect(&dialog,SIGNAL(fileSelected(QString)),this,SLOT(onFileSelected(QString)));
	dialog.showMaximized();
    if (dialog.exec()!=QDialog::Accepted)
        FSelDirName.clear();
    if (!FSelDirName.isEmpty())
        ui.lneDirectory->setText(FSelDirName);
}
#else
// *** >>> eyeCU >>> ***
void FileStreamsOptionsWidget::onDirectoryButtonClicked()
{
	QString dir = QFileDialog::getExistingDirectory(this,tr("Select default directory"), ui.lneDirectory->text());
	if (!dir.isEmpty())
		ui.lneDirectory->setText(dir);
}
#endif
