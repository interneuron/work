#include "changepass.h"
#include "ui_changepass.h"
#include "passbox.h"



changePass::changePass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changePass)
{
    ui->setupUi(this);
   // setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    ui->wrongPasslabel->clear();
    ui->pushButtonOk->setDefault(true);
    ui->pushButtonOk->setDisabled(true);
    ui->pushButton_cancel->setDefault(true);
    ui->lineEditNewPass->installEventFilter(this);
    ui->lineEditRepeatPass->installEventFilter(this);

   // connect(ui->lineEditNewPass,SIGNAL(returnPressed()),SLOT(showRepeatLineEdit()));
}

changePass::~changePass()
{
    delete ui;
    qDebug()<< "changePass - was Deleted";
}

void changePass::on_pushButtonOk_clicked()
{
    QString str = "Пароль успешно изменен";
    PassBox *pb = new PassBox(this);
    pb->setNewPassword(newpassRepeat);
    pb->close();  
    changePass::close();
    qDebug()<< "OK";
}


void changePass::on_pushButton_cancel_clicked()
{
   changePass::close();
}

void changePass::on_lineEditNewPass_textChanged()
{

}
