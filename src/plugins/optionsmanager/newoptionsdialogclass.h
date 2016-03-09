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
#ifdef Q_OS_ANDROID
	~Ui_NewOptionsDialogClass();
#endif
	QVBoxLayout *verticalLayout;
	QSplitter *sprSplitter;
	QTreeView *trvNodes;
	QScrollArea *scaScroll;
	QWidget *scrollAreaWidgetContents;
	QDialogButtonBox *dbbButtons;

	void setupUi(QDialog *OptionsDialogClass);
	void retranslateUi(QDialog *OptionsDialogClass);
};

namespace Ui {
	class OptionsDialogClass2: public Ui_NewOptionsDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWOPTIONSDIALOGCLASS_H
