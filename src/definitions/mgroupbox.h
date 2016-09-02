#ifndef MGROUPBOX_H
#define MGROUPBOX_H

// *** <<< eyeCU >>> ***

#include <QGroupBox>

class MGroupBox : public QGroupBox
{
public:
    MGroupBox(QWidget *AWidget=0){Q_UNUSED(AWidget)}
protected:
    void mousePressEvent(QMouseEvent *AEvent){QWidget::mousePressEvent(AEvent);}
    void mouseMoveEvent(QMouseEvent *AEvent){QWidget::mouseMoveEvent(AEvent);}
    void mouseReleaseEvent(QMouseEvent *AEvent){QWidget::mouseReleaseEvent(AEvent);}
};

#endif // MGROUPBOX_H
