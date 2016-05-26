#ifndef POSITOPTIONS_H
#define POSITOPTIONS_H

#include <QWidget>
#include <QTreeWidget>
#include <QUuid>

#include <interfaces/ipositioning.h>
#include <interfaces/ioptionsmanager.h>
#include <definitions/optionvalues.h>
#include <utils/options.h>

#ifdef EYECU_MOBILE
#include "ui_positioningoptions2.h"
#else
#include "ui_positioningoptions.h"
#endif

namespace Ui {
    class PositioningOptions;
}

class PositioningOptions : public QWidget,
						   public IOptionsDialogWidget
{
    Q_OBJECT
	Q_INTERFACES(IOptionsDialogWidget)

public:
	PositioningOptions(IPositioning *AManager, QHash<QUuid, IPositioningMethod *> ASenderList, IOptionsManager *AOptionsManager, QWidget *parent = 0);
    ~PositioningOptions();
    virtual QWidget* instance() { return this; }

public slots:
    void apply();
    void reset();

signals:
	void modified();
	void childApply();
	void childReset();

protected:
    void changeEvent(QEvent *e);

protected slots:
	void onOptionsClicked();
	void onMethodSelected(int AIndex);

private:	
    Ui::PositioningOptions *ui;
	IOptionsManager *FOptionsManager;
};

#endif // PPOSITOPTIONS_H
