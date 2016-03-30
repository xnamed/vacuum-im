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

QT_BEGIN_NAMESPACE

class OptionsScrollArea;

class OptionsDialogMobile
{
public:
    ~OptionsDialogMobile();
    QVBoxLayout *verticalLayout;
	QTreeView *trvNodes;
	OptionsScrollArea *scaScroll;
    QWidget *scrollAreaWidgetContents;
	QDialogButtonBox *dbbButtons;

	void setupUi(QDialog *OptionsDialogClass);
    void retranslateUi(QDialog *OptionsDialogClass);

};
namespace Ui {
	class OptionsDialogClass: public OptionsDialogMobile {};
} // namespace Ui


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


QT_END_NAMESPACE

#endif // OPTIONSDIALOGMOBILE_H
