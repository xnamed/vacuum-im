#include "optionsdialogheader.h"
#include <utils/qt4qt5compat.h>

#include <QTextDocument>

OptionsDialogHeader::OptionsDialogHeader(const QString &ACaption, QWidget *AParent) : QLabel(AParent)
{
	setTextFormat(Qt::RichText);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
	QString FHeaderStyle  =QString("background-color:#039702; color:white;");
	setStyleSheet(FHeaderStyle);
    setWordWrap(true);

	QFont font=this->font();
	font.setBold(true);
	font.setPointSizeF(font.pointSizeF()*1.1);
	this->setFont(font);
	setText(QString("&nbsp;%1").arg(HTML_ESCAPE(ACaption)));

    setObjectName(QStringLiteral("optionsDialogHeader"));

	setFixedHeight(1.5*IconStorage::iconSize());
//	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
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
