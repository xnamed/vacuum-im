#include <QPainter>
#include <QImageReader>
#include <QImageWriter>
#include <QColorDialog>
#include <QTextList>
#include <QTextImageFormat>
#include <QFontDialog>
#include <QBuffer>
#include <XmlTextDocumentParser>

#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <definitions/xhtmlicons.h>
#include <definitions/shortcuts.h>
#include <definitions/actiongroups.h>
#include <definitions/optionvalues.h>

#include "utils/action.h"

#include "edithtml.h"
#include "xhtmlim.h"
#include "insertimage.h"
#include "addlink.h"
#include "settooltip.h"

#define ADR_DECORATION_TYPE Action::DR_Parametr1
#define ADR_CAPITALIZATION_TYPE Action::DR_Parametr1
#define ADR_ALIGN_TYPE      Action::DR_Parametr1
#define ADR_LIST_TYPE       Action::DR_Parametr1
#define ADR_FORMATTING_TYPE Action::DR_Parametr1
#define ADR_SPECIAL_SYMBOL  Action::DR_Parametr1

EditHtml::EditHtml(IMessageEditWidget *AEditWidget, bool AEnableFormatAutoReset, IBitsOfBinary *BOB , QNetworkAccessManager *ANetworkAccessManager, XhtmlIm *AXhtmlIm, QWidget *parent) :
	QToolBar(parent),
	FToolBarChanger(new ToolBarChanger(this)),
	FEditWidget(AEditWidget),
	FTextEdit(AEditWidget->textEdit()),
	FIconStorage(NULL),
	FBitsOfBinary(BOB),
	FXhtmlIm(AXhtmlIm),
	FNetworkAccessManager(ANetworkAccessManager),
	FCurrentList(NULL),
	FCurrentListItemCount(0),
	FCurrentListItemNumber(0),
	FCurrentPosition(0),
	FCurrentCharacterCount(0),
	FCurrentIndent(0),
	FTextChangedProcessing(false),
	FNewListItemCreated(false)
{
#ifdef EYECU_MOBILE
    setIconSize(QSize(16*IconStorage::scale(),16*IconStorage::scale()));
#else
	setIconSize(QSize(16,16));
#endif
	FIconStorage = IconStorage::staticStorage(RSR_STORAGE_HTML);
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

	setupFontActions(AEnableFormatAutoReset);
	setupTextActions();

	connect(FTextEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)), SLOT(onCurrentCharFormatChanged(QTextCharFormat)));
	connect(FTextEdit, SIGNAL(textChanged()), SLOT(onTextChanged()));
	connect(FTextEdit, SIGNAL(cursorPositionChanged()), SLOT(onCursorPositionChanged()));
	connect(FXhtmlIm, SIGNAL(specialCharacterInserted(QChar)), SLOT(onSpecialCharacterInserted(QChar)));

	onCurrentCharFormatChanged(FTextEdit->currentCharFormat());
	updateCurrentBlock(FTextEdit->textCursor());
}

void EditHtml::setupFontActions(bool AEnableReset)
{
	//  *** Font options ***
	// Font
	FCmbFont = new QFontComboBox(this);
	connect(FCmbFont, SIGNAL(currentFontChanged(QFont)), SLOT(onCurrentFontFamilyChanged(QFont)));
	FToolBarChanger->insertWidget(FCmbFont, AG_XHTMLIM_FONT);

	FCmbFontSize = new QComboBox(this);
	FCmbFontSize->setEditable(true);
	FCmbFontSize->addItem(QString());
	QList<int> standardSizes=QFontDatabase::standardSizes();
	for(QList<int>::const_iterator it=standardSizes.begin(); it!=standardSizes.end(); it++ )
		FCmbFontSize->addItem(QString::number(*it));
	FCmbFontSize->setCurrentIndex(FCmbFontSize->findText(QString::number(QApplication::font().pointSize())));
	connect(FCmbFontSize, SIGNAL(currentIndexChanged(QString)), SLOT(onCurrentFontSizeChanged(QString)));
	FToolBarChanger->insertWidget(FCmbFontSize, AG_XHTMLIM_FONT);

	//  Bold
	FActionTextBold=new Action(this);
	FActionTextBold->setIcon(QIcon::fromTheme("format-text-bold",FIconStorage->getIcon(XHI_TEXT_BOLD)));
	FActionTextBold->setText(tr("Bold"));
//	FActionTextBold->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_BOLD);
	FActionTextBold->setPriority(QAction::LowPriority);
	FActionTextBold->setData(ADR_DECORATION_TYPE, DT_BOLD);
	FActionTextBold->setCheckable(true);
	connect(FActionTextBold, SIGNAL(triggered(bool)), SLOT(onSelectDecoration(bool)));
	FToolBarChanger->insertAction(FActionTextBold, AG_XHTMLIM_FONT);

	//  Italic
	FActionTextItalic=new Action(this);
	FActionTextItalic->setIcon(QIcon::fromTheme("format-text-italic",FIconStorage->getIcon(XHI_TEXT_ITALIC)));
	FActionTextItalic->setText(tr("Italic"));
	FActionTextItalic->setPriority(QAction::LowPriority);
//	FActionTextItalic->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_ITALIC);
	FActionTextItalic->setData(ADR_DECORATION_TYPE, DT_ITALIC);
	connect(FActionTextItalic, SIGNAL(triggered(bool)), SLOT(onSelectDecoration(bool)));
	FActionTextItalic->setCheckable(true);
	FToolBarChanger->insertAction(FActionTextItalic, AG_XHTMLIM_FONT);

	//  Underline
	FActionTextUnderline=new Action(this);
	FActionTextUnderline->setIcon(QIcon::fromTheme("format-text-underline",FIconStorage->getIcon(XHI_TEXT_UNDERLINE)));
	FActionTextUnderline->setText(tr("Underline"));
//	FActionTextUnderline->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_UNDERLINE);
	FActionTextUnderline->setPriority(QAction::LowPriority);
	FActionTextUnderline->setData(ADR_DECORATION_TYPE, DT_UNDERLINE);
	connect(FActionTextUnderline, SIGNAL(triggered(bool)), SLOT(onSelectDecoration(bool)));
	FActionTextUnderline->setCheckable(true);
	FToolBarChanger->insertAction(FActionTextUnderline, AG_XHTMLIM_FONT);

	//  Overline
	FActionTextOverline=new Action(this);
	FActionTextOverline->setIcon(QIcon::fromTheme("format-text-overline", FIconStorage->getIcon(XHI_TEXT_OVERLINE)));
	FActionTextOverline->setText(tr("Overline"));
//	FActionTextOverline->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_OVERLINE);
	FActionTextOverline->setPriority(QAction::LowPriority);
	FActionTextOverline->setData(ADR_DECORATION_TYPE, DT_OVERLINE);
	connect(FActionTextOverline, SIGNAL(triggered(bool)), SLOT(onSelectDecoration(bool)));
	FActionTextOverline->setCheckable(true);
	FToolBarChanger->insertAction(FActionTextOverline, AG_XHTMLIM_FONT);

	//  Striketrough
	FActionTextStrikeout=new Action(this);
	FActionTextStrikeout->setIcon(QIcon::fromTheme("format-text-strikeout",FIconStorage->getIcon(XHI_TEXT_STRIKEOUT)));
	FActionTextStrikeout->setText(tr("Strikeout"));
//	FActionTextStrikeout->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_STRIKEOUT);
	FActionTextStrikeout->setPriority(QAction::LowPriority);
	FActionTextStrikeout->setData(ADR_DECORATION_TYPE, DT_STRIKEOUT);
	connect(FActionTextStrikeout, SIGNAL(triggered(bool)), SLOT(onSelectDecoration(bool)));
	FActionTextStrikeout->setCheckable(true);
	FToolBarChanger->insertAction(FActionTextStrikeout, AG_XHTMLIM_FONT);

	// *** Capitalization submenu ***
	FMenuCapitalization = new Menu(this);
	FMenuCapitalization->setTitle(tr("Capitalization"));
	FMenuCapitalization->setIcon(QIcon::fromTheme("format-text-capitalization", FIconStorage->getIcon(XHI_CAPS_MIXED)));
	QActionGroup *group=new QActionGroup(FMenuCapitalization);

	//  Mixed case
	FActionMixed = new Action(group);
	FActionMixed->setIcon(QIcon::fromTheme("format-text-capitalization-mixedcase",FIconStorage->getIcon(XHI_CAPS_MIXED)));
	FActionMixed->setText(tr("Mixed case"));
	FActionMixed->setData(ADR_CAPITALIZATION_TYPE, QFont::MixedCase);
	FActionMixed->setCheckable(true);
	connect(FActionMixed, SIGNAL(triggered()), SLOT(onSelectCapitalization()));
	FMenuCapitalization->addAction(FActionMixed, AG_XHTMLIM_FONT);

	//  Small caps
	FActionSmallCaps = new Action(group);
	FActionSmallCaps->setIcon(QIcon::fromTheme("format-text-capitalization-smallcaps",FIconStorage->getIcon(XHI_CAPS_SMALLCAPS)));
	FActionSmallCaps->setText(tr("Small caps"));
	FActionSmallCaps->setData(ADR_CAPITALIZATION_TYPE, QFont::SmallCaps);
	FActionSmallCaps->setCheckable(true);
	connect(FActionSmallCaps, SIGNAL(triggered()), SLOT(onSelectCapitalization()));
	FMenuCapitalization->addAction(FActionSmallCaps, AG_XHTMLIM_FONT);

	//  All uppercase
	FActionAllUppercase = new Action(group);
	FActionAllUppercase->setIcon(QIcon::fromTheme("format-text-capitalization-alluppercase",FIconStorage->getIcon(XHI_CAPS_ALLUPPER)));
	FActionAllUppercase->setText(tr("All uppercase"));
	FActionAllUppercase->setData(ADR_CAPITALIZATION_TYPE, QFont::AllUppercase);
	FActionAllUppercase->setCheckable(true);
	connect(FActionAllUppercase, SIGNAL(triggered()), SLOT(onSelectCapitalization()));
	FMenuCapitalization->addAction(FActionAllUppercase, AG_XHTMLIM_FONT);

	//  All lowercase
	FActionAllLowercase = new Action(group);
	FActionAllLowercase->setIcon(QIcon::fromTheme("format-text-capitalization-alllowercase",FIconStorage->getIcon(XHI_CAPS_ALLLOWER)));
	FActionAllLowercase->setText(tr("All lowercase"));
	FActionAllLowercase->setData(ADR_CAPITALIZATION_TYPE, QFont::AllLowercase);
	FActionAllLowercase->setCheckable(true);
	connect(FActionAllLowercase, SIGNAL(triggered()), SLOT(onSelectCapitalization()));
	FMenuCapitalization->addAction(FActionAllLowercase, AG_XHTMLIM_FONT);

	//  All lowercase
	FActionCapitalize = new Action(group);
	FActionCapitalize->setIcon(QIcon::fromTheme("format-text-capitalization-capitalize",FIconStorage->getIcon(XHI_CAPS_CAPITALIZE)));
	FActionCapitalize->setText(tr("Capitalize"));
	FActionCapitalize->setData(ADR_CAPITALIZATION_TYPE, QFont::Capitalize);
	FActionCapitalize->setCheckable(true);
	connect(FActionCapitalize, SIGNAL(triggered()), SLOT(onSelectCapitalization()));
	FMenuCapitalization->addAction(FActionCapitalize, AG_XHTMLIM_FONT);

	FToolBarChanger->insertAction(FMenuCapitalization->menuAction(), AG_XHTMLIM_FONT)->setPopupMode(QToolButton::InstantPopup);;

	//  Code
	FActionTextCode=new Action(this);
	FActionTextCode->setIcon(QIcon::fromTheme("format-text-code", FIconStorage->getIcon(XHI_CODE)));
	FActionTextCode->setText(tr("Code"));
//	FActionTextCode->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_CODE);
	FActionTextCode->setPriority(QAction::LowPriority);
	connect(FActionTextCode, SIGNAL(triggered(bool)), SLOT(onTextCode(bool)));
	FActionTextCode->setCheckable(true);
	FToolBarChanger->insertAction(FActionTextCode, AG_XHTMLIM_FONT);

	// Color
	FColorToolButton = new ColorToolButton(this);
	FColorToolButton->setToolTip(tr("Color"));
	connect(FColorToolButton, SIGNAL(click(bool)), SLOT(onColorClicked(bool)));
	FToolBarChanger->insertWidget(FColorToolButton, AG_XHTMLIM_FONT);

	//  *** Insert ***
	//  Link
	FActionInsertLink=new Action(this);
	FActionInsertLink->setIcon(QIcon::fromTheme("insert-link",IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(MNI_LINK)));
	FActionInsertLink->setText(tr("Insert link"));
//	FActionInsertLink->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_INSERTLINK);
	FActionInsertLink->setPriority(QAction::LowPriority);
	FActionInsertLink->setCheckable(false);
	connect(FActionInsertLink, SIGNAL(triggered()), SLOT(onInsertLink()));
	FToolBarChanger->insertAction(FActionInsertLink, AG_XHTMLIM_INSERT);

	//  Image
	FActionInsertImage=new Action(this);
	FActionInsertImage->setIcon(QIcon::fromTheme("insert-image",FIconStorage->getIcon(XHI_INSERT_IMAGE)));
	FActionInsertImage->setText(tr("Insert image"));
//	FActionInsertImage->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_INSERTIMAGE);
	FActionInsertImage->setPriority(QAction::LowPriority);
	connect(FActionInsertImage, SIGNAL(triggered()), SLOT(onInsertImage()));
	FActionInsertImage->setCheckable(false);
	FToolBarChanger->insertAction(FActionInsertImage, AG_XHTMLIM_INSERT);

	//  Tool tip
	FActionSetTitle=new Action(this);
	FActionSetTitle->setIcon(QIcon::fromTheme("set-tooltip", FIconStorage->getIcon(XHI_SET_TOOLTIP)));
	FActionSetTitle->setText(tr("Set tool tip"));
//	FActionSetTitle->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_SETTOOLTIP);
	FActionSetTitle->setPriority(QAction::LowPriority);
	connect(FActionSetTitle, SIGNAL(triggered()), SLOT(onSetToolTip()));
	FActionSetTitle->setCheckable(true);
	FToolBarChanger->insertAction(FActionSetTitle, AG_XHTMLIM_INSERT);

	//-----
	FActionRemoveFormat=new Action(this);
	FActionRemoveFormat->setIcon(QIcon::fromTheme("format-text-clear", FIconStorage->getIcon(XHI_REMOVEFORMAT)));
	FActionRemoveFormat->setText(tr("Remove format"));
//	FActionRemoveFormat->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_FORMATREMOVE);
	FActionRemoveFormat->setPriority(QAction::LowPriority);
	connect(FActionRemoveFormat, SIGNAL(triggered()), this, SLOT(onRemoveFormat()));
//	connect(FActionRemoveFormat, SIGNAL(triggered()), FXhtmlIm, SLOT(onRemoveFormat()));
	FActionRemoveFormat->setCheckable(false);
	FToolBarChanger->insertAction(FActionRemoveFormat, AG_XHTMLIM_SPECIAL);
	//------------------------

	if (AEnableReset)
	{
		FActionAutoRemoveFormat = new Action(this);
		FActionAutoRemoveFormat->setIcon(QIcon::fromTheme("format-rich-text", FIconStorage->getIcon(XHI_NORICHTEXT)));
		FActionAutoRemoveFormat->setText(tr("Reset formatting on message send"));
//		FActionAutoRemoveFormat->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_FORMATAUTORESET);
		FActionAutoRemoveFormat->setPriority(QAction::LowPriority);
		connect(FActionAutoRemoveFormat, SIGNAL(toggled(bool)), FXhtmlIm, SLOT(onResetFormat(bool)));
		FActionAutoRemoveFormat->setCheckable(true);
		FActionAutoRemoveFormat->setChecked(Options::node(OPV_XHTML_FORMATAUTORESET).value().toBool());
		FToolBarChanger->insertAction(FActionAutoRemoveFormat, AG_XHTMLIM_SPECIAL);
		connect(Options::instance(), SIGNAL(optionsChanged(OptionsNode)), SLOT(onOptionsChanged(OptionsNode)));
	}
//-----
}

void EditHtml::setupTextActions()
{
	// *** Special characters ***
	Menu *special = new Menu(this);
	special->setTitle(tr("Insert special symbol"));
	special->menuAction()->setIcon(RSR_STORAGE_HTML, XHI_INSERT_NBSP);
	special->menuAction()->setData(ADR_SPECIAL_SYMBOL, QChar::Nbsp);
	connect(special->menuAction(), SIGNAL(triggered()), SLOT(onInsertSpecial()));
	QActionGroup *group=new QActionGroup(special);

	Action *action = new Action(group);
	action->setText(tr("Non-breaking space"));
	action->setIcon(RSR_STORAGE_HTML, XHI_INSERT_NBSP);
	action->setData(ADR_SPECIAL_SYMBOL, QChar::Nbsp);
//	action->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_INSERTNBSP);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertSpecial()));
	special->addAction(action);

	action = new Action(group);
	action->setText(tr("New line"));
	action->setIcon(RSR_STORAGE_HTML, XHI_INSERT_NEWLINE);
	action->setData(ADR_SPECIAL_SYMBOL, QChar::LineSeparator);
//	action->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_INSERTNEWLINE);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertSpecial()));
	special->addAction(action);
	FToolBarChanger->insertAction(special->menuAction(), AG_XHTMLIM_INSERT);

	// *** Indentation ***
	FActionIndentLess= new Action(this);
	FActionIndentLess->setIcon(QIcon::fromTheme("format-indent-less", FIconStorage->getIcon(XHI_OUTDENT)));
	FActionIndentLess->setText(tr("Decrease indent"));
//	FActionIndentLess->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_INDENTDECREASE);
	FActionIndentLess->setPriority(QAction::LowPriority);
	FActionIndentLess->setCheckable(false);
	connect(FActionIndentLess, SIGNAL(triggered()), this, SLOT(onIndentChange()));
	FToolBarChanger->insertAction(FActionIndentLess, AG_XHTMLIM_INDENT);

	FActionIndentMore=new Action(this);
	FActionIndentMore->setIcon(QIcon::fromTheme("format-indent-more",FIconStorage->getIcon(XHI_INDENT)));
	FActionIndentMore->setText(tr("Increase indent"));
//	FActionIndentMore->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_INDENTINCREASE);
	FActionIndentMore->setPriority(QAction::LowPriority);
	FActionIndentMore->setCheckable(false);
	connect(FActionIndentMore, SIGNAL(triggered()), this, SLOT(onIndentChange()));
	FToolBarChanger->insertAction(FActionIndentMore, AG_XHTMLIM_INDENT);

	// *** Alignment ***
	FMenuAlign = new Menu(this);
	FMenuAlign->setTitle(tr("Text align"));
	FMenuAlign->menuAction()->setCheckable(true);
	connect(FMenuAlign->menuAction(), SIGNAL(triggered()), SLOT(onTextAlign()));

	group=new QActionGroup(FMenuAlign);
	action = new Action(group);
	action->setText(tr("Left"));
	action->setIcon(RSR_STORAGE_HTML, XHI_ALIGN_LEFT);
	action->setData(ADR_ALIGN_TYPE, int(Qt::AlignLeft|Qt::AlignAbsolute));
	action->setCheckable(true);
//	action->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_ALIGNLEFT);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onTextAlign()));
	FMenuAlign->addAction(action, AG_XHTMLIM_ALIGN);
	FActionLastAlign = action;

	action = new Action(group);
	action->setText(tr("Center"));
	action->setIcon(RSR_STORAGE_HTML,XHI_ALIGN_CENTER);
	action->setData(ADR_ALIGN_TYPE, Qt::AlignHCenter);
	action->setCheckable(true);
//	action->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_ALIGNCENTER);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onTextAlign()));
	FMenuAlign->addAction(action, AG_XHTMLIM_ALIGN);

	action = new Action(group);
	action->setText(tr("Right"));
	action->setIcon(RSR_STORAGE_HTML,XHI_ALIGN_RIGHT);
	action->setData(ADR_ALIGN_TYPE, int(Qt::AlignRight|Qt::AlignAbsolute));
	action->setCheckable(true);
//	action->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_ALIGNRIGHT);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onTextAlign()));
	FMenuAlign->addAction(action, AG_XHTMLIM_ALIGN);

	action = new Action(group);
	action->setText(tr("Justify"));
	action->setIcon(RSR_STORAGE_HTML,XHI_ALIGN_JUSTIFY);
	action->setData(ADR_ALIGN_TYPE, Qt::AlignJustify);
	action->setCheckable(true);
//	action->setShortcutId(SCT_MESSAGEWINDOWS_XHTMLIM_ALIGNJUSTIFY);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onTextAlign()));
	FMenuAlign->addAction(action, AG_XHTMLIM_ALIGN);

	FToolBarChanger->insertAction(FMenuAlign->menuAction(), AG_XHTMLIM_PARAGRAPH);

	// *** Text list ***
	FMenuList = new Menu(this);
	FMenuList->setTitle(tr("List"));
	FMenuList->setIcon(RSR_STORAGE_HTML, XHI_LIST_BULLET_DISC);
	FMenuList->menuAction()->setEnabled(true);
	FMenuList->menuAction()->setData(ADR_LIST_TYPE, QTextListFormat::ListDisc);
	connect(FMenuList->menuAction(), SIGNAL(triggered()), SLOT(onInsertList()));

	group=new QActionGroup(FMenuList);
	action = new Action(group);
	action->setText(tr("Disc"));
	action->setIcon(RSR_STORAGE_HTML, XHI_LIST_BULLET_DISC);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListDisc);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_LIST);
	FActionLastList = action;

	action = new Action(group);
	action->setText(tr("Circle"));
	action->setIcon(RSR_STORAGE_HTML,XHI_LIST_BULLET_CIRCLE);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListCircle);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_LIST);

	action = new Action(group);
	action->setText(tr("Square"));
	action->setIcon(RSR_STORAGE_HTML,XHI_LIST_BULLET_SQUARE);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListSquare);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_LIST);

	action = new Action(group);
	action->setText(tr("Decimal"));
	action->setIcon(RSR_STORAGE_HTML,XHI_LIST_ORDER_DECIMAL);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListDecimal);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_LIST);

	action = new Action(group);
	action->setText(tr("Alpha lower"));
	action->setIcon(RSR_STORAGE_HTML,XHI_LIST_ORDER_ALPHA_LOW);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListLowerAlpha);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_LIST);

	action = new Action(group);
	action->setText(tr("Alpha upper"));
	action->setIcon(RSR_STORAGE_HTML,XHI_LIST_ORDER_ALPHA_UP);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListUpperAlpha);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_LIST);

	action = new Action(group);
	action->setText(tr("Roman lower"));
	action->setIcon(RSR_STORAGE_HTML,XHI_LIST_ORDER_ROMAN_LOW);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListLowerRoman);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_LIST);

	action = new Action(group);
	action->setText(tr("Roman upper"));
	action->setIcon(RSR_STORAGE_HTML,XHI_LIST_ORDER_ROMAN_UP);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListUpperRoman);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_LIST);

	action = new Action(group);
	action->setText(tr("Definition list"));
	action->setIcon(RSR_STORAGE_HTML, XHI_LIST_DEFINITION);
	action->setData(ADR_LIST_TYPE, QTextListFormat::ListStyleUndefined);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onInsertList()));
	FMenuList->addAction(action, AG_XHTMLIM_DEFLIST);
	FToolBarChanger->insertAction(FMenuList->menuAction(), AG_XHTMLIM_PARAGRAPH);

	// Special formatting
	FMenuFormat = new Menu(this);
	FMenuFormat->setTitle(tr("Formatting type"));
	FMenuFormat->menuAction()->setCheckable(true);
	connect(FMenuFormat->menuAction(), SIGNAL(triggered()), SLOT(onSetFormat()));

	group=new QActionGroup(FMenuFormat);
	action = new Action(group);
	action->setText(tr("Preformatted text"));
	action->setIcon(RSR_STORAGE_HTML, XHI_PREFORMAT);
	action->setData(ADR_FORMATTING_TYPE, FMT_PREFORMAT);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onSetFormat()));
	FMenuFormat->addAction(action, AG_XHTMLIM_FORMAT);
	FActionLastFormat = action;

	action = new Action(group);
	action->setText(tr("Heading 1"));
	action->setIcon(RSR_STORAGE_HTML, XHI_HEADING1);
	action->setData(ADR_FORMATTING_TYPE, FMT_HEADING1);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onSetFormat()));
	FMenuFormat->addAction(action, AG_XHTMLIM_FORMATHEADING);

	action = new Action(group);
	action->setText(tr("Heading 2"));
	action->setIcon(RSR_STORAGE_HTML, XHI_HEADING2);
	action->setData(ADR_FORMATTING_TYPE, FMT_HEADING2);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onSetFormat()));
	FMenuFormat->addAction(action, AG_XHTMLIM_FORMATHEADING);

	action = new Action(group);
	action->setText(tr("Heading 3"));
	action->setIcon(RSR_STORAGE_HTML, XHI_HEADING3);
	action->setData(ADR_FORMATTING_TYPE, FMT_HEADING3);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onSetFormat()));
	FMenuFormat->addAction(action, AG_XHTMLIM_FORMATHEADING);

	action = new Action(group);
	action->setText(tr("Heading 4"));
	action->setIcon(RSR_STORAGE_HTML, XHI_HEADING4);
	action->setData(ADR_FORMATTING_TYPE, FMT_HEADING4);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onSetFormat()));
	FMenuFormat->addAction(action, AG_XHTMLIM_FORMATHEADING);

	action = new Action(group);
	action->setText(tr("Heading 5"));
	action->setIcon(RSR_STORAGE_HTML, XHI_HEADING5);
	action->setData(ADR_FORMATTING_TYPE, FMT_HEADING5);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onSetFormat()));
	FMenuFormat->addAction(action, AG_XHTMLIM_FORMATHEADING);

	action = new Action(group);
	action->setText(tr("Heading 6"));
	action->setIcon(RSR_STORAGE_HTML, XHI_HEADING6);
	action->setData(ADR_FORMATTING_TYPE, FMT_HEADING6);
	action->setCheckable(true);
	action->setPriority(QAction::LowPriority);
	action->setActionGroup(group);
	connect(action, SIGNAL(triggered()), SLOT(onSetFormat()));
	FMenuFormat->addAction(action, AG_XHTMLIM_FORMATHEADING);
	FToolBarChanger->insertAction(FMenuFormat->menuAction(), AG_XHTMLIM_PARAGRAPH);
}

// ---- SLOTS -----

void EditHtml::onOptionsChanged(const OptionsNode &ANode)
{
	if (ANode.path() == OPV_XHTML_FORMATAUTORESET)
		FActionAutoRemoveFormat->setChecked(ANode.value().toBool());
}

void EditHtml::onColorClicked(bool ABackground)
{
	if(ABackground)
		selectBackgroundColor();
	else
		selectForegroundColor();
}

//void EditHtml::onResetFormat(bool AStatus)
//{
//	Options::node(OPV_XHTML_FORMATAUTORESET).setValue(AStatus);
//}

void EditHtml::onInsertImage()
{
	QUrl        imageUrl;
	QByteArray  imageData;
	QPixmap		pixmap;
	QTextCursor cursor = FTextEdit->textCursor();
	QTextCharFormat charFmtCurrent=cursor.charFormat();
	QSize       size;
	QString     alt;

	bool supportBoB=FBitsOfBinary && FBitsOfBinary->isSupported(FEditWidget->messageWindow()->streamJid(),FEditWidget->messageWindow()->contactJid());

	if (!cursor.hasSelection())
		if (charFmtCurrent.isImageFormat())
		{
			QTextImageFormat imageFormat=charFmtCurrent.toImageFormat();
			cursor.select(QTextCursor::WordUnderCursor);
			imageUrl = imageFormat.name();
			QVariant imageResource = FTextEdit->document()->resource(QTextDocument::ImageResource, imageUrl);
			if (imageResource.type()==QVariant::ByteArray)
				imageData=imageResource.toByteArray();
			else if (imageResource.type()==QVariant::Pixmap)
				pixmap = imageResource.value<QPixmap>();
			else if (imageResource.type()==QVariant::Image)
				pixmap = QPixmap::fromImage(imageResource.value<QImage>());
			size.setWidth(imageFormat.width());
			size.setHeight(imageFormat.height());
			alt=imageFormat.property(XmlTextDocumentParser::ImageAlternativeText).toString();
		}

	InsertImage *inserImage = new InsertImage(FXhtmlIm, FNetworkAccessManager, imageData, pixmap, imageUrl, size, alt);

	inserImage->setWindowIcon(FIconStorage->getIcon(XHI_INSERT_IMAGE));
	if(!supportBoB)
		inserImage->ui->pbBrowse->hide();
	if(inserImage->exec() == QDialog::Accepted)
	{
		if(!inserImage->getUrl().isEmpty())
		{
			QTextImageFormat imageFormat;
			QString          alt=inserImage->getAlternativeText();
			if (!alt.isEmpty())
				imageFormat.setProperty(XmlTextDocumentParser::ImageAlternativeText, alt);
			if (!inserImage->physResize())
			{
				if(inserImage->newHeight()!=inserImage->originalHeight())
					imageFormat.setHeight(inserImage->newHeight());
				if(inserImage->newWidth()!=inserImage->originalWidth())
					imageFormat.setWidth(inserImage->newWidth());
			}
			cursor.beginEditBlock();
			if(inserImage->isRemote())
			{
				QUrl url=inserImage->getUrl();
				imageFormat.setName(url.toString());
				cursor.document()->addResource(QTextDocument::ImageResource, url, inserImage->getImageData());
				cursor.insertImage(imageFormat);
			}
			else
				if(supportBoB)
				{
					QByteArray imageData=inserImage->getImageData().toByteArray();
					if (!imageData.isNull())
					{
						QString contentId=FBitsOfBinary->contentIdentifier(imageData);
						QString uri=QString("cid:").append(contentId);
						imageFormat.setName(uri);
						imageFormat.setProperty(XhtmlIm::PMaxAge, inserImage->getMaxAge());
						imageFormat.setProperty(XhtmlIm::PMimeType, inserImage->getFileType());
						imageFormat.setProperty(XhtmlIm::PEmbed, inserImage->embed());
						cursor.document()->addResource(QTextDocument::ImageResource, QUrl(uri), imageData);
						cursor.insertImage(imageFormat);
					}
				}
			cursor.endEditBlock();
		}
	}
	inserImage->deleteLater();
}

void EditHtml::onSetToolTip()
{
	QTextCursor cursor = FTextEdit->textCursor();
	QTextCharFormat charFormat=cursor.charFormat();
	if (!charFormat.hasProperty(QTextFormat::TextToolTip) &&
		!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);

	Action *action=qobject_cast<Action *>(sender());
	int toolTipType = charFormat.intProperty(XmlTextDocumentParser::ToolTipType);

	SetToolTip *setToolTip = new SetToolTip(toolTipType, charFormat.toolTip(), action->parentWidget()->parentWidget());
	if(setToolTip->exec() == QDialog::Accepted)
	{
		cursor.beginEditBlock();
		if (setToolTip->toolTipText().isEmpty())	// Remove tooltip
		{
			if (cursor.hasSelection())
			{
				charFormat.setProperty(QTextFormat::TextToolTip, QVariant());
				charFormat.setProperty(XmlTextDocumentParser::ToolTipType, XmlTextDocumentParser::None);
				if (charFormat.underlineStyle()==QTextCharFormat::DotLine && charFormat.underlineColor()==Qt::red)
				{
					charFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);
					charFormat.setUnderlineColor(QColor());
				}
				cursor.mergeCharFormat(charFormat);
			}
			else
			{
				charFormat.clearProperty(QTextFormat::TextToolTip);
				charFormat.clearProperty(XmlTextDocumentParser::ToolTipType);
				if (charFormat.underlineStyle()==QTextCharFormat::DotLine && charFormat.underlineColor()==Qt::red)
				{
					charFormat.clearProperty(QTextFormat::TextUnderlineStyle);
					charFormat.clearProperty(QTextFormat::TextUnderlineColor);
				}
				cursor.setCharFormat(charFormat);
			}
		}
		else
		{
			QTextCharFormat format;
			format.setProperty(QTextFormat::TextToolTip, setToolTip->toolTipText());
			if (setToolTip->type()!=SetToolTip::None)
			{
				format.setUnderlineStyle(QTextCharFormat::DotLine);
				format.setUnderlineColor(Qt::red);
			}
			else
				if (charFormat.underlineStyle()==QTextCharFormat::DotLine &&
					charFormat.underlineColor()==Qt::red)
				{
					format.setUnderlineStyle(QTextCharFormat::NoUnderline);
					format.setUnderlineColor(QColor());
				}
			format.setProperty(XmlTextDocumentParser::ToolTipType, setToolTip->type());
			cursor.mergeCharFormat(format);
		}
		cursor.endEditBlock();
	}
	setToolTip->deleteLater();
}

void EditHtml::onInsertLink()
{
	QTextCursor cursor = FTextEdit->textCursor();

	QTextCharFormat charFmtCurrent=cursor.charFormat();

	if (!cursor.hasSelection())
	{
		if (charFmtCurrent.isAnchor())
		{
			QTextBlock block=cursor.block();
			for (QTextBlock::iterator it = block.begin(); !(it.atEnd()); ++it)
			{
				QTextFragment currentFragment = it.fragment();
				if (currentFragment.isValid())
				{
					if (currentFragment.contains(cursor.position()))
					{
						cursor.setPosition(currentFragment.position());
						cursor.setPosition(currentFragment.position()+currentFragment.length(), QTextCursor::KeepAnchor);
						break;
					}
				}
			}
		}
		else
			cursor.select(QTextCursor::WordUnderCursor);
	}

	bool needsToBeInserted=(cursor.selection().isEmpty());

	Action *action=qobject_cast<Action *>(sender());

	AddLink *addLink = new AddLink(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(MNI_LINK),
								   QUrl::fromEncoded(charFmtCurrent.anchorHref().toLatin1()), cursor.selectedText(), action->parentWidget()->parentWidget());
	switch (addLink->exec())
	{
		case AddLink::Add:
		{
			QTextCharFormat charFormat=charFmtCurrent;
			charFormat.setAnchor(true);
			charFormat.setAnchorHref(addLink->url().toEncoded());
			charFormat.setFontUnderline(true);
			charFormat.setForeground(QBrush(Qt::blue));
			cursor.beginEditBlock();
			if (needsToBeInserted)
			{
				cursor.insertText(addLink->description(), charFormat);
				cursor.insertText(" ", charFmtCurrent);
			}
			else
				cursor.mergeCharFormat(charFormat);
			cursor.endEditBlock();
			break;
		}

		case AddLink::Remove:
		{
			QTextCharFormat charFormat;
			cursor.beginEditBlock();
			if (cursor.hasSelection())
			{
				charFormat.setAnchor(false);
				charFormat.setAnchorHref(QString());
				charFormat.setAnchorName(QString());
				cursor.mergeCharFormat(charFormat);
			}
			else
			{
				charFormat = charFmtCurrent;
				charFormat.clearProperty(QTextFormat::AnchorHref);
				charFormat.clearProperty(QTextFormat::AnchorName);
				charFormat.clearProperty(QTextFormat::IsAnchor);
				cursor.setCharFormat(charFormat);
			}
			cursor.endEditBlock();
			break;
		}
	}
	addLink->deleteLater();
}

void EditHtml::onSelectDecoration(bool ASelected)
{
	Action *action = qobject_cast<Action *>(sender());
	QTextCharFormat charFormat;
	switch (action->data(ADR_DECORATION_TYPE).toInt())
	{
		case DT_BOLD:
			charFormat.setFontWeight(ASelected?QFont::Bold:QFont::Normal);
			break;
		case DT_ITALIC:
			charFormat.setFontItalic(ASelected);
			break;
		case DT_OVERLINE:
			charFormat.setFontOverline(ASelected);
			break;
		case DT_UNDERLINE:
			charFormat.setFontUnderline(ASelected);
			break;
		case DT_STRIKEOUT:
			charFormat.setFontStrikeOut(ASelected);
			break;
	}
	mergeFormatOnWordOrSelection(charFormat);
}

void EditHtml::onSelectCapitalization()
{
	Action *action = qobject_cast<Action *>(sender());
	QTextCharFormat charFormat;
	charFormat.setFontCapitalization((QFont::Capitalization)action->data(ADR_CAPITALIZATION_TYPE).toInt());
	mergeFormatOnWordOrSelection(charFormat);
}

//void EditHtml::onTextBold()
//{
//	QTextCharFormat fmt;
//	fmt.setFontWeight(FActionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
//	mergeFormatOnWordOrSelection(fmt);
//}

//void EditHtml::onTextItalic()
//{
//	QTextCharFormat fmt;
//	fmt.setFontItalic(FActionTextItalic->isChecked());
//	mergeFormatOnWordOrSelection(fmt);
//}

//void EditHtml::onTextUnderline()
//{
//	QTextCharFormat fmt;
//	fmt.setProperty(QTextFormat::TextUnderlineStyle, FActionTextUnderline->isChecked()?QTextCharFormat::SingleUnderline
//																					  :QTextCharFormat::NoUnderline);
//	mergeFormatOnWordOrSelection(fmt);
//}

//void EditHtml::onTextOverline()
//{
//	QTextCharFormat fmt;
//	fmt.setFontOverline(FActionTextOverline->isChecked());
//	mergeFormatOnWordOrSelection(fmt);
//}

//void EditHtml::onTextStrikeout()
//{
//	QTextCharFormat fmt;
//	fmt.setFontStrikeOut(FActionTextStrikeout->isChecked());
//	mergeFormatOnWordOrSelection(fmt);
//}

void EditHtml::onTextCode(bool AChecked)
{
	QTextCursor cursor = FTextEdit->textCursor();
	cursor.beginEditBlock();
	if (AChecked)
	{
		QTextCharFormat charFormat;
		charFormat.setFontFamily("Courier New,courier");
		mergeFormatOnWordOrSelection(charFormat);
	}
	else
	{
		if (!cursor.hasSelection())
			cursor.select(QTextCursor::WordUnderCursor);
		QTextCharFormat charFormat = cursor.charFormat();
		charFormat.clearProperty(QTextFormat::FontFamily);
		cursor.setCharFormat(charFormat);
	}
	cursor.endEditBlock();
}

void EditHtml::onCurrentFontFamilyChanged(const QFont &AFont)
{
	QTextCharFormat fmt;
	fmt.setFontFamily(AFont.family());
	mergeFormatOnWordOrSelection(fmt);
}

void EditHtml::onCurrentFontSizeChanged(const QString &ASize)
{
	QTextCharFormat charFormat;
	qreal pointSize = ASize.toFloat();
	if (ASize.toFloat() > 0)
	{
		charFormat.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(charFormat);
	}
	else // Remove FontPointSize property
	{
		QTextCursor cursor = FTextEdit->textCursor();
		if (!cursor.hasSelection())
			cursor.select(QTextCursor::WordUnderCursor);
		QTextCharFormat charFormat = cursor.charFormat();
		charFormat.clearProperty(QTextFormat::FontPointSize);
		cursor.setCharFormat(charFormat);
	}
}

void EditHtml::onInsertList()
{
	Action *ac = qobject_cast<Action *>(sender());
	if (ac)
	{
		QTextListFormat::Style style = (QTextListFormat::Style)ac->data(ADR_LIST_TYPE).toInt();// = QTextListFormat::ListStyleUndefined;
		FMenuList->setIcon(ac->icon());
		FMenuList->menuAction()->setData(ADR_LIST_TYPE, style);
		QTextCursor cursor = FTextEdit->textCursor();
		if (style >=QTextListFormat::ListUpperRoman && style <=QTextListFormat::ListStyleUndefined)
		{
			cursor.beginEditBlock();
			QTextListFormat listFormat;
			QTextList       *list=cursor.currentList();
			if (list)   //  Have list at current position
			{
				listFormat = list->format();
				int indent=listFormat.indent();
				if (listFormat.style()==QTextListFormat::ListStyleUndefined)
					indent++;
				listFormat.setStyle(style);
				if (style==QTextListFormat::ListStyleUndefined)
					indent--;
				listFormat.setIndent(indent);
				if (list->itemNumber(cursor.block())==0 && cursor.atBlockStart())    // First block in the list
					list->setFormat(listFormat);               //  Just update list format
				else                                        // Not first block
				{
					indent++;
					listFormat.setIndent(indent);  //  Create a sublist
					if (!cursor.atBlockStart() || cursor.block().previous().textList()!=list)
						cursor.insertBlock();
					cursor.createList(listFormat);
				}
			}
			else        // No list at current position
			{
				int indent=style==QTextListFormat::ListStyleUndefined?0:1;
				listFormat.setIndent(indent);
				listFormat.setStyle(style);    // Just set its style
				cursor.createList(listFormat); // and create a root list
			}
			cursor.endEditBlock();
		}
		else
		{
			QTextBlockFormat blockFormat;
			blockFormat.setObjectIndex(-1);
			cursor.mergeBlockFormat(blockFormat);
		}
	}
}

//int EditHtml::checkBlockFormat(const QTextCursor &ACursor)
//{
//	QTextCharFormat  charFormat = ACursor.blockCharFormat();
//	QTextBlockFormat format = ACursor.blockFormat();
//	int header=XmlTextDocumentParser::header(charFormat);
//	if (header)
//		return header;
//	else if (format.boolProperty(QTextFormat::BlockNonBreakableLines) && charFormat.boolProperty(QTextFormat::FontFixedPitch))
//		return FMT_PREFORMAT;
//	return FMT_NORMAL;
//}

//void EditHtml::clearBlockProperties(const QTextBlock &ATextBlock, const QSet<QTextFormat::Property> &AProperties)
//{
//	QTextCursor cursor(ATextBlock);
//	for (QTextBlock::iterator it=ATextBlock.begin(); it!=ATextBlock.end(); it++)
//	{
//		QTextFragment fragment=it.fragment();
//		// Select fragment
//		cursor.setPosition(fragment.position());
//		cursor.setPosition(fragment.position()+fragment.length(), QTextCursor::KeepAnchor);
//		QTextCharFormat charFormat=fragment.charFormat();
//		for (QSet<QTextFormat::Property>::const_iterator it=AProperties.begin(); it!=AProperties.end(); it++)
//			charFormat.clearProperty(*it);
//		cursor.setCharFormat(charFormat);
//	}
//}

QTextFragment EditHtml::getTextFragment(const QTextCursor &ACursor)
{
	QTextBlock block=ACursor.block();
	for (QTextBlock::iterator it=block.begin(); it!=block.end(); it++)
		if (it.fragment().contains(ACursor.position()))
			return it.fragment();
	return QTextFragment();
}

void EditHtml::selectTextFragment(QTextCursor &ACursor)
{
	QTextFragment fragment=getTextFragment(ACursor);
	if (fragment.isValid())
	{
		ACursor.setPosition(fragment.position());
		ACursor.setPosition(fragment.position()+fragment.length(), QTextCursor::KeepAnchor);
	}
}

void EditHtml::onSetFormat()
{
	Action *action = qobject_cast<Action *>(sender());
	if (action!=FMenuFormat->menuAction())
		FActionLastFormat=action;
	FXhtmlIm->setFormat(FTextEdit, action->data(ADR_FORMATTING_TYPE).toInt());
/*
	int formatType = action->data(ADR_FORMATTING_TYPE).toInt();
	QTextCursor cursor = FXhtmlIm->getCursor(FTextEdit, true, false);	
	int currentFormatType=XhtmlIm::checkBlockFormat(cursor);

	cursor.beginEditBlock();	
	QTextCharFormat blockCharFormat;
	QTextBlockFormat blockFormat;

	if (currentFormatType!=formatType)
	{
		if (formatType==FMT_PREFORMAT)
		{
			blockCharFormat.setFontFixedPitch(true);
			blockFormat.setProperty(QTextFormat::BlockNonBreakableLines, true);
		}
		else
		{
			blockCharFormat.setProperty(QTextFormat::FontSizeAdjustment, 4-formatType);
			blockCharFormat.setFontWeight(QFont::Bold);
		}

		int first, last;
		if (cursor.position()<cursor.anchor())
		{
			first=cursor.position();
			last=cursor.anchor();
		}
		else
		{
			first=cursor.anchor();
			last=cursor.position();
		}
		cursor.setPosition(first);
		cursor.movePosition(QTextCursor::StartOfBlock);
		QTextBlock block;
		for (block=cursor.block(); !block.contains(last); block=block.next());

		cursor.setPosition(block.position(), QTextCursor::KeepAnchor);
		cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
		cursor.mergeCharFormat(blockCharFormat);
	}
	else
	{
		QSet<QTextFormat::Property> properties;
		properties.insert(QTextFormat::FontSizeAdjustment);
		properties.insert(QTextFormat::FontWeight);
		XhtmlIm::clearBlockProperties(cursor.block(), properties);
	}
	cursor.setBlockCharFormat(blockCharFormat);
	cursor.setBlockFormat(blockFormat);
	cursor.endEditBlock();
	updateCurrentBlock(cursor);
*/
}

void EditHtml::onInsertSpecial()
{
	Action *action = qobject_cast<Action *>(sender());
	QChar specialSybmol = (QChar)(action->data(ADR_SPECIAL_SYMBOL).toInt());
	Menu *special = qobject_cast<Menu *>(action->parentWidget());
	special->menuAction()->setData(ADR_SPECIAL_SYMBOL, specialSybmol);
	special->menuAction()->setIcon(action->icon());
	QTextCursor cursor = FTextEdit->textCursor();
	cursor.beginEditBlock();
	cursor.insertText(specialSybmol);
	cursor.endEditBlock();
}

void EditHtml::onIndentChange()
{
	QAction *action=qobject_cast<QAction *>(sender());
	QTextCursor cursor = FTextEdit->textCursor();
	bool increase=action==FActionIndentMore;
	QTextBlockFormat blockFmt = cursor.blockFormat();
	if (cursor.currentList())
	{
		if (cursor.currentList()->format().style()==QTextListFormat::ListStyleUndefined)
			blockFmt.setIndent(increase);
	}
	else
	{
		qreal indentWidth=FTextEdit->document()->indentWidth();
		qreal indent=blockFmt.textIndent();
		if (increase)
			blockFmt.setTextIndent(indent+indentWidth);
		else
			if (indent>0)
				blockFmt.setTextIndent(indent-indentWidth);
	}
	cursor.setBlockFormat(blockFmt);
}

void EditHtml::onCursorPositionChanged()
{
	QTextCursor cursor=qobject_cast<QTextEdit *>(sender())->textCursor();
	FCurrentPosition = cursor.position();
	updateCurrentList(cursor);
	updateCurrentBlock(cursor);
}

void EditHtml::onTextChanged()
{
	if (!FTextChangedProcessing)
	{
		FTextChangedProcessing=true;
		QTextEdit *editor=qobject_cast<QTextEdit *>(sender());
		QTextCursor cursor=editor->textCursor();
		QTextBlock block=cursor.block();
		const QTextDocument *doc=block.document();

		if (cursor.position()==FCurrentPosition &&      // Position unchanged
			doc->characterCount()==FCurrentCharacterCount &&    // Text length unchanged
			FCurrentList && !block.textList())          // List changed
		{
			QTextBlockFormat format=cursor.blockFormat();
			format.setIndent(0);
			cursor.setBlockFormat(format);
			QTextList     *parentList=NULL;
			if (FCurrentIndent>1)   // A sublist
			{                       // should be moved into parent list
				for (int i=block.blockNumber()-1; i>=0; i--)
				{
					QTextList  *list=doc->findBlockByNumber(i).textList();
					if (!list)
						break;
					int indent=list->format().indent();
					if (list->format().style()==QTextListFormat::ListStyleUndefined)
						indent++;
					if (indent==FCurrentIndent-1)
					{
						parentList=list;
						break;
					}
				}

				if (parentList)
					parentList->add(block);
			}

			QSet<QTextList *>lists;
			for (QTextBlock b=block.next(); b!=doc->end(); b=b.next())
			{
				QTextList *list=b.textList();
				if (list==FCurrentList)
					if (parentList)
						parentList->add(b);
					else
					{
						FCurrentList->remove(b);
						QTextCursor cursor(b);
						QTextBlockFormat format=cursor.blockFormat();
						format.setIndent(0);
						cursor.setBlockFormat(format);
					}
				else if (list)
					if (list->format().indent()>FCurrentIndent)
						lists.insert(list);
					else
						break;
				else
					break;
			}
			for (QSet<QTextList *>::iterator it=lists.begin(); it!=lists.end(); it++)
			{
				QTextListFormat format=(*it)->format();
				format.setIndent(format.indent()-1);
				(*it)->setFormat(format);
			}
		}
		else if (FNewListItemCreated &&
				FCurrentList->format().style() == QTextListFormat::ListStyleUndefined)
		{
				QTextBlockFormat format=FCurrentList->item(FCurrentListItemNumber).blockFormat();
				if (format.indent())
					format.setIndent(0);
				else
					format.setIndent(1);
				cursor.setBlockFormat(format);
		} else
		{
			if (Options::node(OPV_XHTML_TABINDENT).value().toBool() &&
				doc->characterCount() == FCurrentCharacterCount+1 &&
#if QT_VERSION >= 0x040700
				cursor.positionInBlock()
#else
				(cursor.position() - block.position())
#endif
					== 1 &&
				block.text()[0]=='\t')
				{
					cursor.beginEditBlock();
					cursor.deletePreviousChar();
					cursor.endEditBlock();
					FActionIndentMore->trigger();
				}
		}
		updateCurrentList(cursor);
		updateCurrentBlock(cursor);
		FCurrentCharacterCount=doc->characterCount();
		FTextChangedProcessing=false;
	}
}

void EditHtml::onTextAlign()
{
	Action *action = qobject_cast<Action *>(sender());
	if (action)
	{
		Qt::AlignmentFlag align = Qt::AlignmentFlag(action->data(ADR_ALIGN_TYPE).toInt());
		QTextBlockFormat format=FTextEdit->textCursor().blockFormat();
		if (format.hasProperty(QTextFormat::BlockAlignment) &&
			format.alignment()==align)
		{
			format.clearProperty(QTextFormat::BlockAlignment);
			FTextEdit->textCursor().setBlockFormat(format);
		}
		else
		{
			FMenuAlign->setIcon(action->icon());
			FMenuAlign->menuAction()->setData(ADR_ALIGN_TYPE, align);
			FTextEdit->setAlignment(align);
			FActionLastAlign = action;
		}
		updateCurrentBlock(FTextEdit->textCursor());
	}
}

void EditHtml::onRemoveFormat()
{
	FXhtmlIm->clearFormatOnSelection(FXhtmlIm->getCursor(FTextEdit, true, true), FTextEdit);
}

void EditHtml::onCurrentCharFormatChanged(const QTextCharFormat &ACharFormat)
{
	fontChanged(ACharFormat);
	QBrush brush=ACharFormat.foreground();
	if (brush.style())
		FColorToolButton->setForegroundColor(brush.color());
	else
		FColorToolButton->setForegroundColor(FTextEdit->palette().foreground().color());
	brush=ACharFormat.background();
	if (brush.style())
		FColorToolButton->setBackgroundColor(brush.color());
	else
		FColorToolButton->setBackgroundColor(FTextEdit->palette().background().color());
	FActionSetTitle->setChecked(ACharFormat.hasProperty(QTextFormat::TextToolTip));

	switch (ACharFormat.fontCapitalization())
	{
		case QFont::MixedCase:
			FMenuCapitalization->setIcon(FActionMixed->icon());
			if (ACharFormat.hasProperty(QTextFormat::FontCapitalization))
				FActionMixed->setChecked(true);
			break;
		case QFont::SmallCaps:
			FMenuCapitalization->setIcon(FActionSmallCaps->icon());
			if (ACharFormat.hasProperty(QTextFormat::FontCapitalization))
				FActionSmallCaps->setChecked(true);
			break;
		case QFont::AllUppercase:
			FMenuCapitalization->setIcon(FActionAllUppercase->icon());
			if (ACharFormat.hasProperty(QTextFormat::FontCapitalization))
				FActionAllUppercase->setChecked(true);
			break;
		case QFont::AllLowercase:
			FMenuCapitalization->setIcon(FActionAllLowercase->icon());
			if (ACharFormat.hasProperty(QTextFormat::FontCapitalization))
				FActionAllLowercase->setChecked(true);
			break;
		case QFont::Capitalize:
			FMenuCapitalization->setIcon(FActionCapitalize->icon());
			if (ACharFormat.hasProperty(QTextFormat::FontCapitalization))
				FActionCapitalize->setChecked(true);
			break;
	}
}

void EditHtml::onSpecialCharacterInserted(QChar ASpecialCharacter)
{
	QList<QAction *> actions = FToolBarChanger->groupItems(AG_XHTMLIM_INSERT);
	if (!actions.isEmpty())
	{
		Action *action = FToolBarChanger->handleAction(actions.first());
		switch (ASpecialCharacter.unicode())
		{
			case QChar::Nbsp:
				action->setIcon(RSR_STORAGE_HTML, XHI_INSERT_NBSP);
				action->setData(ADR_SPECIAL_SYMBOL, QChar::Nbsp);
				break;
			case QChar::LineSeparator:
				action->setIcon(RSR_STORAGE_HTML, XHI_INSERT_NEWLINE);
				action->setData(ADR_SPECIAL_SYMBOL, QChar::LineSeparator);
				break;
		}
	}
}

void EditHtml::fontChanged(const QTextCharFormat &ACharFormat)
{
	FCmbFont->blockSignals(true);
	FCmbFont->setCurrentFont(ACharFormat.hasProperty(QTextFormat::FontFamily)?ACharFormat.font():FTextEdit->font());
	FCmbFont->blockSignals(false);

	FCmbFontSize->blockSignals(true);
	FCmbFontSize->setCurrentIndex(FCmbFontSize->findText(QString::number(ACharFormat.hasProperty(QTextFormat::FontPointSize)?ACharFormat.fontPointSize():FTextEdit->fontPointSize())));
	FCmbFontSize->blockSignals(false);
/*
	else if (ACharFormat.hasProperty(QTextFormat::FontStyleHint))
	{
		int index;
		switch (ACharFormat.intProperty(QTextFormat::FontStyleHint))
		{
			case QFont::Serif: index=FNT_SERIF; break;
			case QFont::SansSerif: index=FNT_SANSSERIF; break;
			case QFont::Cursive: index=FNT_CURSIVE; break;
			case QFont::Monospace: index=FNT_MONOSPACED; break;
			case QFont::Fantasy: index=FNT_FANTASY; break;
		}
		FCmbFont->setCurrentIndex(index);
	}
	else
		FCmbFont->setCurrentIndex(FNT_SEPARATOR);   // No font
*/
//    if (ACharFormat.hasProperty(QTextFormat::FontPointSize))
//		FCmbFontSize->setEditText(QString::number(ACharFormat.doubleProperty(QTextFormat::FontPointSize)));
	if (ACharFormat.hasProperty(QTextFormat::FontWeight))
		FActionTextBold->setChecked(ACharFormat.fontWeight()>=QFont::Bold);
	else
		FActionTextBold->setChecked(false);
	if (ACharFormat.hasProperty(QTextFormat::FontItalic))
		FActionTextItalic->setChecked(ACharFormat.boolProperty(QTextFormat::FontItalic));
	else
		FActionTextItalic->setChecked(false);
	if (ACharFormat.hasProperty(QTextFormat::FontUnderline))
		FActionTextUnderline->setChecked(ACharFormat.boolProperty(QTextFormat::FontUnderline));
	else if (ACharFormat.hasProperty(QTextFormat::TextUnderlineStyle))
		FActionTextUnderline->setChecked(ACharFormat.intProperty(QTextFormat::TextUnderlineStyle)==QTextCharFormat::SingleUnderline);
	else
		FActionTextUnderline->setChecked(false);
	if (ACharFormat.hasProperty(QTextFormat::FontOverline))
		FActionTextOverline->setChecked(ACharFormat.boolProperty(QTextFormat::FontOverline));
	else
		FActionTextOverline->setChecked(false);
	if (ACharFormat.hasProperty(QTextFormat::FontStrikeOut))
		FActionTextStrikeout->setChecked(ACharFormat.boolProperty(QTextFormat::FontStrikeOut));
	else
		FActionTextStrikeout->setChecked(false);
	if (ACharFormat.hasProperty(QTextFormat::FontFamily))
		FActionTextCode->setChecked(ACharFormat.stringProperty(QTextFormat::FontFamily)=="Courier New,courier");
	else
		FActionTextCode->setChecked(false);
}

void EditHtml::selectForegroundColor()
{
	QColor col = QColorDialog::getColor(FTextEdit->textColor(), this);
	if (!col.isValid())
		return;
	QTextCharFormat fmt;
	fmt.setForeground(col);
	mergeFormatOnWordOrSelection(fmt);
	FColorToolButton->setForegroundColor(col);
}

void EditHtml::selectBackgroundColor()
{
	QColor color = QColorDialog::getColor(FTextEdit->textBackgroundColor(), this);
	if (!color.isValid())
		return;
	QTextCharFormat fmt;
	fmt.setBackground(color);
	mergeFormatOnWordOrSelection(fmt);
	FColorToolButton->setBackgroundColor(color);
}

void EditHtml::updateCurrentList(const QTextCursor &ACursor)
{
	FNewListItemCreated = FCurrentList &&                                              // It's a list and
					   ACursor.currentList() == FCurrentList &&                      // The list is unchanged and
					   FCurrentList->count() == FCurrentListItemCount+1 &&          // Number of items in the list increased by 1 and
					   FCurrentList->itemNumber(ACursor.block()) == FCurrentListItemNumber+1; // Number of current item increased by 1

	if ((FCurrentList = ACursor.currentList()))
	{
		FCurrentIndent = FCurrentList->format().indent();
		if (ACursor.currentList()->format().style()==QTextListFormat::ListStyleUndefined)
			FCurrentIndent++;
		FCurrentListItemNumber = FCurrentList->itemNumber(ACursor.block());
		FCurrentListItemCount = FCurrentList->count();
	}
	else
	{
		FCurrentIndent = 0;
		FCurrentListItemNumber = 0;
		FCurrentListItemCount = 0;
	}
}

void EditHtml::updateCurrentBlock(const QTextCursor &ACursor)
{
	if (ACursor.currentList())
		FMenuFormat->setEnabled(false);
	else
	{
		FMenuFormat->setEnabled(true);
		QTextBlockFormat blockFormat=ACursor.blockFormat();

		int format=XhtmlIm::checkBlockFormat(ACursor);
		if (format)
		{
			FMenuList->setEnabled(false);
			QList<Action *>actions=FMenuFormat->actions();
			Action *action=NULL;
			for (QList<Action *>::iterator it=actions.begin(); it!=actions.end(); it++)
				if ((*it)->data(ADR_FORMATTING_TYPE).toInt()==format)
				{
					action=*it;
					break;
				}
			action->setChecked(true);
			FMenuFormat->setIcon(action->icon());
			FMenuFormat->menuAction()->setData(ADR_FORMATTING_TYPE, format);
			FMenuFormat->menuAction()->setChecked(true);
		}
		else
		{
			FMenuList->setEnabled(true);
			FMenuFormat->menuAction()->setData(ADR_FORMATTING_TYPE, FActionLastFormat->data(ADR_FORMATTING_TYPE));
			FMenuFormat->setIcon(FActionLastFormat->icon());
			FMenuFormat->menuAction()->setChecked(false);
		}

		if (blockFormat.hasProperty(QTextFormat::BlockAlignment))
		{
			Qt::Alignment align = blockFormat.alignment();
			QList<Action *>actions=FMenuAlign->actions();
			Action *action=NULL;
			for (QList<Action *>::iterator it=actions.begin(); it!=actions.end(); it++)
				if (((*it)->data(ADR_ALIGN_TYPE).toInt()&0x0F)==(align&0x0F))
				{
					action=*it;
					break;
				}
			action->setChecked(true);
			FMenuAlign->setIcon(action->icon());
			FMenuAlign->menuAction()->setData(ADR_ALIGN_TYPE, (int)align);
			FMenuAlign->menuAction()->setChecked(true);
		}
		else
		{
			FMenuAlign->menuAction()->setData(ADR_ALIGN_TYPE, FActionLastAlign->data(ADR_ALIGN_TYPE));
			FMenuAlign->setIcon(FActionLastAlign->icon());
			FMenuAlign->menuAction()->setChecked(false);
		}
	}
}

void EditHtml::selectFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, FEditWidget->instance()->parentWidget());
	if (ok)
	{
		QTextCharFormat fmt;
		fmt.setFont(font);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void EditHtml::mergeFormatOnWordOrSelection(const QTextCharFormat &AFormat)
{
	QTextCursor cursor = FTextEdit->textCursor();
	if (!cursor.hasSelection())
		cursor.select(QTextCursor::WordUnderCursor);
	cursor.mergeCharFormat(AFormat);
	FTextEdit->mergeCurrentCharFormat(AFormat);
}

void EditHtml::clearFormatOnWordOrSelection(bool AWholeDocument)
{
	QTextCharFormat emptyCharFormat;
	QTextCursor cursor = FTextEdit->textCursor();
	cursor.beginEditBlock();
	if (AWholeDocument)
	{
		cursor.select(QTextCursor::Document);
		cursor.setBlockCharFormat(emptyCharFormat);
	}
	else if (!cursor.hasSelection())
	{
		cursor.select(QTextCursor::BlockUnderCursor);
		cursor.setBlockCharFormat(emptyCharFormat);
	}
	cursor.setCharFormat(emptyCharFormat);
	cursor.endEditBlock();
	FTextEdit->setCurrentCharFormat(emptyCharFormat);
}
