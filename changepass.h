#ifndef CHANGEPASS_H
#define CHANGEPASS_H

#include <QDialog>
#include <QKeyEvent>

#include "mainwindow.h"
#include "ui_changepass.h"


namespace Ui {
class changePass;
}

class changePass : public QDialog
{
    Q_OBJECT

public:
    explicit changePass(QWidget *parent = 0);
    ~changePass();

private:
    Ui::changePass *ui;
 //   MainWindow *wdwCP = new MainWindow(this);
    QString newpass;
    QString newpassRepeat;

public slots:


private slots:

bool eventFilter(QObject *target, QEvent *event)
    {

    if (target == ui->lineEditNewPass)
      {
        if (event->type() == QEvent::KeyPress)
        {
          QKeyEvent *keyEvent = (QKeyEvent *)event;

          if ((keyEvent->key() == Qt::Key_Return)||(keyEvent->key() == Qt::Key_Enter)||(keyEvent->key() == Qt::Key_Tab))
          {
            if(ui->lineEditNewPass->text()!="")
            {
              ui->wrongPasslabel->clear();
              newpass = ui->lineEditNewPass->text();
              focusNextChild();
            }
            else{
                ui->wrongPasslabel->setText("Введите пароль!");
            }
            return true;
          }
        }
      }
//------------------------------------------------------------------------
      if (target == ui->lineEditRepeatPass)
      {
        if (event->type() == QEvent::KeyPress)
        {
          QKeyEvent *keyEvent = (QKeyEvent *)event;
          if ((keyEvent->key() == Qt::Key_Return)||(keyEvent->key() == Qt::Key_Enter))
          {
              newpassRepeat = ui->lineEditRepeatPass->text();
              if(newpass == newpassRepeat)
              {
                  ui->pushButtonOk->setDisabled(false);
                  ui->wrongPasslabel->clear();
                  focusNextChild();
              }
              else
              {
                  ui->wrongPasslabel->setHidden(false);
                  ui->lineEditNewPass->clear();
                  ui->lineEditRepeatPass->clear();
                  ui->wrongPasslabel->setStyleSheet("color:red;font:24");
                  ui->wrongPasslabel->setText("Пароли не совпадают");
                  focusPreviousChild();
              }
            return true;
          }
        }
      }
      return QDialog::eventFilter(target, event);
}

void on_pushButtonOk_clicked();

void on_pushButton_cancel_clicked();
void on_lineEditNewPass_textChanged();
};

#endif // CHANGEPASS_H
