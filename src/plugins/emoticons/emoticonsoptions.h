#ifndef EMOTICONSOPTIONS_H
#define EMOTICONSOPTIONS_H

#include <QWidget>
#include <interfaces/iemoticons.h>
#include <interfaces/ioptionsmanager.h>

#ifdef EYECU_MOBILE
#include "ui_emoticonsoptions2.h"
#else
#include "ui_emoticonsoptions.h"
#endif

class EmoticonsOptions :
	public QWidget,
	public IOptionsDialogWidget
{
	Q_OBJECT;
	Q_INTERFACES(IOptionsDialogWidget);
public:
	EmoticonsOptions(IEmoticons *AEmoticons, QWidget *AParent);
	~EmoticonsOptions();
	virtual QWidget* instance() { return this; }
public slots:
	virtual void apply();
	virtual void reset();
signals:
	void modified();
	void childApply();
	void childReset();
protected slots:
	void onUpButtonClicked();
	void onDownButtonClicked();
private:
	Ui::EmoticonsOptionsClass ui;
private:
	IEmoticons *FEmoticons;
};

#endif // EMOTICONSOPTIONS_H
