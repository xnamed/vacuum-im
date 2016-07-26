#ifndef OPTIONSDIALOGMOBILE_H
#define OPTIONSDIALOGMOBILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPainter>
#include <QLabel>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE

class OptionsScrollArea;
class OptionsTreeView;
class LineOnWidget;
class QPaintEvent;

class OptionsDialogMobile
{
public:
    ~OptionsDialogMobile();
    QVBoxLayout *verticalLayout;
//!    QTreeView *trvNodes;         // Old variant ---
    OptionsTreeView *trvNodes;      // New variant ---
	OptionsScrollArea *scaScroll;
	QDialogButtonBox *dbbButtons;

	void setupUi(QDialog *OptionsDialogClass);
    void retranslateUi(QDialog *OptionsDialogClass);

};
namespace Ui {
	class OptionsDialogClass: public OptionsDialogMobile {};
} // namespace Ui


//!
//! \brief The OptionsTreeView class
//!
class OptionsTreeView : public QTreeView
{
public:
    OptionsTreeView();
    ~OptionsTreeView();
protected:
    void mousePressEvent(QMouseEvent *AEvent);
    void mouseMoveEvent(QMouseEvent *AEvent);
    void mouseReleaseEvent(QMouseEvent *AEvent);
private:
    QPoint FPressedPos;
    int	   FScrollBarValue;
};

//!
//! \brief The OptionsScrollArea class
//!
class OptionsScrollArea : public QScrollArea
{
public:
	OptionsScrollArea();
    ~OptionsScrollArea();
protected:
	void mousePressEvent(QMouseEvent *AEvent);
    void mouseMoveEvent(QMouseEvent *AEvent);
    void mouseReleaseEvent(QMouseEvent *AEvent);
private:
	QPoint FPressedPos;
	int	   FScrollBarValue;
};


//!
//! \brief The LineOnWidget class
//!
class LineOnWidget : public QLabel
{
	Q_OBJECT
public:
	explicit LineOnWidget(QLabel *parent = 0);
	void setColor(QColor AColor){FColorLine=AColor;}
    void setWidthLine(int AWidth){FcurWidth=AWidth;}
protected:
	void paintEvent(QPaintEvent *AEvent);
private:
    QColor  FColorLine;
    int     FcurWidth;
};

QT_END_NAMESPACE

#endif // OPTIONSDIALOGMOBILE_H