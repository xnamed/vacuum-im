#ifndef OPTIONSWIDGET_H
#define OPTIONSWIDGET_H

#include <QLabel>
#include <QSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QCheckBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QFontComboBox>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <interfaces/ioptionsmanager.h>

class OptionsDialogWidget :
	public QWidget,
	public IOptionsDialogWidget
{
	Q_OBJECT;
	Q_INTERFACES(IOptionsDialogWidget);
public:
	OptionsDialogWidget(const OptionsNode &ANode, const QString &ACaption, QWidget *AParent);
	OptionsDialogWidget(const OptionsNode &ANode, const QString &ACaption, QWidget *AControl, QWidget *AParent);
	virtual ~OptionsDialogWidget();
    virtual QWidget* instance() { return this; }
public slots:
	virtual void apply();
	virtual void reset();
signals:
	void modified();
	void childApply();
	void childReset();
protected:
	bool eventFilter(QObject *AWatched, QEvent *AEvent);
protected:
	void insertEditor(const QString &ACaption, QWidget *AEditor, QHBoxLayout *ALayout);
    void insertEditor2(const QString &ACaption, QWidget *AEditor, QHBoxLayout *ALayout);  // *** <<< eyeCU >>> ***
	void rigisterEditor(const OptionsNode &ANode, const QString &ACaption, QWidget *AEditor);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
protected:
//    void mousePressEvent(QMouseEvent *AEvent){QWidget::mousePressEvent(AEvent);}
//    void mouseMoveEvent(QMouseEvent *AEvent){QWidget::mouseMoveEvent(AEvent);}
//    void mouseReleaseEvent(QMouseEvent *AEvent){QWidget::mouseReleaseEvent(AEvent);}
#endif
// *** >>> eyeCU >>> ***
private:
	QLabel *FCaption;
	QCheckBox *FCheckBox;
	QLineEdit *FLineEdit;
    QTextEdit *FTextEdit;               // *** <<< eyeCU <<< ***
	QComboBox *FComboBox;
	QFontComboBox *FFontComboBox;       // inherits QComboBox
	QSpinBox *FSpinBox;
	QTimeEdit *FTimeEdit;               // inherits QDateTimeEdit
	QDateEdit *FDateEdit;               // inherits QDateTimeEdit
	QDateTimeEdit *FDateTimeEdit;       // inherits QSpinBox
	QDoubleSpinBox *FDoubleSpinBox;     // inherits QSpinBox
private:
	QVariant FValue;
	OptionsNode FNode;
};

#endif // OPTIONSWIDGET_H
