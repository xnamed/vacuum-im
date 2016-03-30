#include "optionsdialogheader.h"
#include <utils/qt4qt5compat.h>

#include <QTextDocument>

OptionsDialogHeader::OptionsDialogHeader(const QString &ACaption, QWidget *AParent) : QLabel(AParent)
{
	setTextFormat(Qt::RichText);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
    setWordWrap(true);
    setText(QString("<h4>%1</h4>").arg(HTML_ESCAPE(ACaption)));
#else
    setText(QString("<h2>%1</h2>").arg(HTML_ESCAPE(ACaption)));
#endif
// *** >>> eyeCU >>> ***
}

void OptionsDialogHeader::apply()
{
	emit childApply();
}

void OptionsDialogHeader::reset()
{
	emit childReset();
}
