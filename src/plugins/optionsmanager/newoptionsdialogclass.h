#ifndef UI_NEWOPTIONSDIALOGCLASS_H
#define UI_NEWOPTIONSDIALOGCLASS_H

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

class Ui_NewOptionsDialogClass
{
public:
	~Ui_NewOptionsDialogClass();
    QVBoxLayout *verticalLayout;
	QTreeView *trvNodes;
	QScrollArea *scaScroll;
    QWidget *scrollAreaWidgetContents;
	QDialogButtonBox *dbbButtons;

	void setupUi(QDialog *OptionsDialogClass);
    void retranslateUi(QDialog *OptionsDialogClass);
};

namespace Ui {
    class OptionsDialogClass: public Ui_NewOptionsDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWOPTIONSDIALOGCLASS_H
