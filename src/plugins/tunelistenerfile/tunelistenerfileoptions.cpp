#include <QFileDialog>
#include <definitions/optionvalues.h>
#include "tunelistenerfileoptions.h"
#include "tunelistenerfile.h"
#include "ui_tunelistenerfileoptions.h"

TuneListenerFileOptions::TuneListenerFileOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TuneListenerFileOptions)
{
    ui->setupUi(this);
    connect(ui->pbBrowse, SIGNAL(clicked()), SLOT(onBrowseClicked()));
    connect(ui->ledFilePath, SIGNAL(textChanged(QString)), SIGNAL(modified()));
    connect(ui->cmbFileFormat, SIGNAL(currentIndexChanged(int)), SIGNAL(modified()));
    reset();
}

TuneListenerFileOptions::~TuneListenerFileOptions()
{
    delete ui;
}

void TuneListenerFileOptions::apply()
{
    Options::node(OPV_TUNE_LISTENER_FILE_NAME).setValue(ui->ledFilePath->text());
    Options::node(OPV_TUNE_LISTENER_FILE_FORMAT).setValue(ui->cmbFileFormat->currentIndex());
    emit childApply();
}

void TuneListenerFileOptions::reset()
{
    ui->ledFilePath->setText(Options::node(OPV_TUNE_LISTENER_FILE_NAME).value().toString());
    ui->cmbFileFormat->setCurrentIndex(Options::node(OPV_TUNE_LISTENER_FILE_FORMAT).value().toInt());
    emit childReset();
}

#ifdef EYECU_MOBILE
void TuneListenerFileOptions::onBrowseClicked()
{
	QString format=ui->cmbFileFormat->currentIndex()==TuneListenerFile::Plain?tr("Plain text (*.txt *.log)"):tr("XML (*.xml)");
	QString fileName;
	QFileDialog dialog(this, tr("Please, select file to read"), ui->ledFilePath->text(), format);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.showMaximized();
	if (dialog.exec())
	{
		QStringList lst=dialog.selectedFiles();
		fileName=lst[0];
	}
	else
		fileName.clear();
//	QString fileName = dialog.getOpenFileName(this, tr("Please, select file to read"), ui->ledFilePath->text(), format);
	if (!fileName.isEmpty())
		ui->ledFilePath->setText(fileName);
}
#else
void TuneListenerFileOptions::onBrowseClicked()
{
	QString format=ui->cmbFileFormat->currentIndex()==TuneListenerFile::Plain?tr("Plain text (*.txt *.log)")
																			 :tr("XML (*.xml)");
	QString fileName = QFileDialog::getOpenFileName(this, tr("Please, select file to read"), ui->ledFilePath->text(), format);
	if (!fileName.isEmpty())
		ui->ledFilePath->setText(fileName);
}
#endif
