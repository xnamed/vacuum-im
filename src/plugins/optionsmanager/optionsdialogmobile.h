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

QT_BEGIN_NAMESPACE

class OptionsDialogMobile
{
public:
	~OptionsDialogMobile();
    QVBoxLayout *verticalLayout;
	QTreeView *trvNodes;
	QScrollArea *scaScroll;
    QWidget *scrollAreaWidgetContents;
	QDialogButtonBox *dbbButtons;
	QVBoxLayout *vBox;

	void setupUi(QDialog *OptionsDialogClass);
    void retranslateUi(QDialog *OptionsDialogClass);
};

namespace Ui {
    class OptionsDialogClass: public OptionsDialogMobile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OPTIONSDIALOGMOBILE_H
