/*
   eyeCU widget
*/

#ifndef MGROUPBOX_H
#define MGROUPBOX_H

#include <QGroupBox>

class MGroupBox : public QGroupBox
{
public:
    MGroupBox(QWidget *AParent =0){Q_UNUSED(AParent)}
protected:
    void mousePressEvent(QMouseEvent *AEvent){QWidget::mousePressEvent(AEvent);}
    void mouseMoveEvent(QMouseEvent *AEvent){QWidget::mouseMoveEvent(AEvent);}
    void mouseReleaseEvent(QMouseEvent *AEvent){QWidget::mouseReleaseEvent(AEvent);}
};

#endif // MGROUPBOX_H
