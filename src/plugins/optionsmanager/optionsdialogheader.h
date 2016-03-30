#ifndef OPTIONSHEADER_H
#define OPTIONSHEADER_H

#include <QLabel>
#include <definitions/resources.h>
#include <interfaces/ioptionsmanager.h>
#include <utils/iconstorage.h>

class OptionsDialogHeader : 
	public QLabel,
	public IOptionsDialogWidget
{
	Q_OBJECT;
	Q_INTERFACES(IOptionsDialogWidget);
public:
	OptionsDialogHeader(const QString &ACaption, QWidget *AParent);
	virtual QWidget *instance() { return this; }
public slots:
	virtual void apply();
	virtual void reset();
signals:
	void modified();
	void childApply();
	void childReset();
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
protected:
    void mousePressEvent(QMouseEvent *AEvent){QWidget::mousePressEvent(AEvent);}
    void mouseMoveEvent(QMouseEvent *AEvent){QWidget::mouseMoveEvent(AEvent);}
    void mouseReleaseEvent(QMouseEvent *AEvent){QWidget::mouseReleaseEvent(AEvent);}
#endif
// *** >>> eyeCU >>> ***
};

#endif // OPTIONSHEADER_H
