#include "mainwindow.h"

#include <QButtonGroup>
#include <QMessageBox>
#include <QPixmap>
#include <QVector>

#include "ui_mainwindow.h"

s21::MainWindow::MainWindow(s21::Controller *c, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller(c) {
  ui->setupUi(this);

  connect(ui->buttonGroup_Digits, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(digits_show(QAbstractButton *)));
  connect(ui->buttonGroup_PlusMinus, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(digits_show(QAbstractButton *)));
  connect(ui->buttonGroup_Binary, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(digits_show(QAbstractButton *)));
  connect(ui->buttonGroup_Unary, SIGNAL(buttonClicked(QAbstractButton *)), this,
          SLOT(digits_show(QAbstractButton *)));
  lscob_counter = 0;
  rscob_counter = 0;
  ui->resultShow->setText("");
  ui->History->setText("");
  ui->buttonGroup_Binary->blockSignals(true);
  ui->buttonGroup_Unary->blockSignals(false);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->Button_Lscob->blockSignals(false);
  ui->Button_Rscob->blockSignals(true);
  ui->resultShow->setText("");
  ui->History->setText("");
  ui->centralwidget->setGeometry(30, 30, 820, 600);
  QPixmap pix(":/resources/img/SmartCalc2.png");
  QPixmap pix2(":/resources/img/SmartCalc3.png");
  QPixmap pix_ad(":/resources/img/Ad.png");
  int w = ui->image->width();
  int h = ui->image->height();
  int w2 = ui->Image->width();
  int h2 = ui->Image->height();
  int w_ad = ui->image_ad->width();
  int h_ad = ui->image_ad->height();
  ui->image->setPixmap(pix.scaled(w, h));
  ui->Image->setPixmap(pix2.scaled(w2, h2));
  ui->image_ad->setPixmap(pix_ad.scaled(w_ad, h_ad));
  on_pushButton_Deposit_clear_clicked();
  on_pushButton_Call_clicked();
}

s21::MainWindow::~MainWindow() { delete ui; }

void s21::MainWindow::initialize_rules() {
  ui->buttonGroup_Digits->blockSignals(false);
  ui->buttonGroup_Binary->blockSignals(true);
  ui->buttonGroup_Unary->blockSignals(false);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->Button_Lscob->blockSignals(false);
  ui->Button_Rscob->blockSignals(true);
  ui->pushButton_X->blockSignals(false);
  ui->pushButton_dot->blockSignals(false);
  ui->pushButton_eq->blockSignals(false);
  ui->pushButton_Graph->blockSignals(false);
  ui->pushButton_E->blockSignals(true);
  lscob_counter = 0;
  rscob_counter = 0;
}

void s21::MainWindow::eq_rules() {
  ui->buttonGroup_Digits->blockSignals(true);
  ui->buttonGroup_Binary->blockSignals(true);
  ui->buttonGroup_Unary->blockSignals(true);
  ui->buttonGroup_PlusMinus->blockSignals(true);
  ui->Button_Lscob->blockSignals(true);
  ui->Button_Rscob->blockSignals(true);
  ui->pushButton_X->blockSignals(true);
  ui->pushButton_dot->blockSignals(true);
  ui->pushButton_eq->blockSignals(true);
  ui->pushButton_Graph->blockSignals(true);
  ui->pushButton_E->blockSignals(true);
}

void s21::MainWindow::digit_rules() {
  ui->buttonGroup_Digits->blockSignals(false);
  ui->buttonGroup_Binary->blockSignals(false);
  ui->buttonGroup_Unary->blockSignals(true);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->Button_Lscob->blockSignals(true);
  ui->pushButton_Graph->blockSignals(false);
  ui->Button_Rscob->blockSignals(false);
  ui->pushButton_X->blockSignals(true);
  ui->pushButton_dot->blockSignals(false);
  ui->pushButton_E->blockSignals(false);
}

void s21::MainWindow::binary_rules() {
  ui->buttonGroup_Digits->blockSignals(false);
  ui->buttonGroup_Binary->blockSignals(true);
  ui->buttonGroup_Unary->blockSignals(false);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->Button_Lscob->blockSignals(false);
  ui->Button_Rscob->blockSignals(true);
  ui->pushButton_Graph->blockSignals(true);
  ui->pushButton_X->blockSignals(false);
  ui->pushButton_dot->blockSignals(false);
  ui->pushButton_E->blockSignals(true);
}

void s21::MainWindow::unary_rules() {
  ui->buttonGroup_Digits->blockSignals(false);
  ui->buttonGroup_Binary->blockSignals(true);
  ui->buttonGroup_Unary->blockSignals(false);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->pushButton_Graph->blockSignals(true);
  ui->Button_Lscob->blockSignals(false);
  ui->Button_Rscob->blockSignals(true);
  ui->pushButton_X->blockSignals(false);
  ui->pushButton_dot->blockSignals(true);
  ui->pushButton_E->blockSignals(true);
}

void s21::MainWindow::plusminus_rules() {
  ui->buttonGroup_Digits->blockSignals(false);
  ui->buttonGroup_Binary->blockSignals(true);
  ui->buttonGroup_Unary->blockSignals(false);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->Button_Lscob->blockSignals(false);
  ui->pushButton_Graph->blockSignals(true);
  ui->Button_Rscob->blockSignals(true);
  ui->pushButton_X->blockSignals(false);
  ui->pushButton_dot->blockSignals(false);
  ui->pushButton_E->blockSignals(true);
}

void s21::MainWindow::lscob_rules() {
  ui->buttonGroup_Digits->blockSignals(false);
  ui->buttonGroup_Binary->blockSignals(true);
  ui->buttonGroup_Unary->blockSignals(false);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->Button_Lscob->blockSignals(false);
  ui->Button_Rscob->blockSignals(true);
  ui->pushButton_X->blockSignals(false);
  ui->pushButton_dot->blockSignals(false);
  ui->pushButton_E->blockSignals(true);
}

void s21::MainWindow::rscob_rules() {
  ui->buttonGroup_Digits->blockSignals(true);
  ui->buttonGroup_Binary->blockSignals(false);
  ui->buttonGroup_Unary->blockSignals(false);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->Button_Lscob->blockSignals(true);
  if (lscob_counter > rscob_counter)
    ui->Button_Rscob->blockSignals(false);
  else
    ui->Button_Rscob->blockSignals(true);
  ui->pushButton_X->blockSignals(true);
  ui->pushButton_dot->blockSignals(true);
  ui->pushButton_E->blockSignals(true);
}

void s21::MainWindow::x_rules() {
  ui->buttonGroup_Digits->blockSignals(true);
  ui->buttonGroup_Binary->blockSignals(false);
  ui->buttonGroup_Unary->blockSignals(true);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->pushButton_Graph->blockSignals(false);
  ui->Button_Lscob->blockSignals(true);
  ui->Button_Rscob->blockSignals(false);
  ui->pushButton_X->blockSignals(true);
  ui->pushButton_dot->blockSignals(true);
  ui->pushButton_E->blockSignals(false);
}

void s21::MainWindow::dot_rules() {
  ui->buttonGroup_Digits->blockSignals(false);
  ui->buttonGroup_Binary->blockSignals(false);
  ui->buttonGroup_Unary->blockSignals(true);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->Button_Lscob->blockSignals(true);
  ui->Button_Rscob->blockSignals(false);
  ui->pushButton_X->blockSignals(true);
  ui->pushButton_dot->blockSignals(true);
  ui->pushButton_E->blockSignals(true);
}

void s21::MainWindow::digits_show(QAbstractButton *button) {
  QString button_show = button->text();
  QString input_string = ui->resultShow->text();

  if (button->group() == ui->buttonGroup_Digits) {
    digit_rules();
    if (input_point.length() == 0) input_point.push_back("1");
    if (input_point.length() >= 1 && input_point.back() != '1')
      input_point.push_back("1");
  } else if (button->group() == ui->buttonGroup_Binary) {
    binary_rules();
    input_point.push_back("2");
  } else if (button->group() == ui->buttonGroup_Unary) {
    unary_rules();
    button_show.push_back("(");
    lscob_counter++;
    input_point.push_back("3");
  } else if (button->group() == ui->buttonGroup_PlusMinus) {
    plusminus_rules();
    QString::Iterator it = input_point.end();
    QChar first = *(--it);
    QChar second = *(--it);
    if ((first == '4') && (second == '4')) {
      button_show.clear();
      ui->buttonGroup_PlusMinus->blockSignals(true);
    } else
      input_point.push_back("4");
  } else if (button == ui->Button_Lscob) {
    lscob_rules();
    input_point.push_back("5");
  } else if (button == ui->Button_Rscob) {
    rscob_rules();
    input_point.push_back("6");
  } else if (button == ui->pushButton_X) {
    x_rules();
    input_point.push_back("7");
  } else if (button == ui->pushButton_dot) {
    dot_rules();
    QString::Iterator it = input_point.end();
    if (*(--it) == '1' && *(--it) == '8') {
      button_show.clear();
    } else
      input_point.push_back("8");
  }
  ui->resultShow->setText(input_string + button_show);
}

void s21::MainWindow::on_Button_Lscob_clicked() {
  QPushButton *button = (QPushButton *)sender();
  lscob_counter++;
  digits_show(button);
}

void s21::MainWindow::on_Button_Rscob_clicked() {
  QPushButton *button = (QPushButton *)sender();
  rscob_counter++;
  digits_show(button);
}

void s21::MainWindow::on_pushButton_X_clicked() {
  QPushButton *button = (QPushButton *)sender();
  digits_show(button);
}

void s21::MainWindow::on_pushButton_dot_clicked() {
  QPushButton *button = (QPushButton *)sender();
  dot_rules();
  digits_show(button);
}

void s21::MainWindow::on_pushButton_E_clicked() {
  QPushButton *button = (QPushButton *)sender();
  digits_show(button);
}

void s21::MainWindow::on_pushButton_Call_clicked() {
  ui->resultShow->setText(0);
  ui->History->setText(0);
  input_point.clear();
  lscob_counter = 0;
  rscob_counter = 0;
  ui->buttonGroup_Binary->blockSignals(true);
  ui->buttonGroup_Unary->blockSignals(false);
  ui->buttonGroup_PlusMinus->blockSignals(false);
  ui->buttonGroup_Digits->blockSignals(false);
  ui->pushButton_eq->blockSignals(false);
  ui->Button_Lscob->blockSignals(false);
  ui->Button_Rscob->blockSignals(true);
  ui->pushButton_X->blockSignals(false);
}

void s21::MainWindow::on_pushButton_eq_clicked() {
  eq_rules();
  ui->History->setText(ui->resultShow->text());
  std::string temp_input = ui->History->text().toLocal8Bit().constData();
  double x = ui->DoubleSpinBox_X_Value->value();
  std::string output = controller->calculate(temp_input, x);
  if (output != "Error!") {
    double optimization = std::stod(output.c_str());
    if (optimization < 10000000000.00) {
      ui->resultShow->setText(QString::fromStdString(output));
    } else {
      ui->resultShow->setText(QString::number(optimization, 'g', 2));
    }
  } else {
    ui->resultShow->setText(QString::fromStdString(output));
  }
}

void s21::MainWindow::on_pushButton_Graph_clicked() {
  ui->History->setText(ui->resultShow->text());

  bool result = controller->graph(
      ui->History->text(), ui->DoubleSpinBox_X_Value->value(),
      ui->DoubleSpinBox_X_Max->value(), ui->DoubleSpinBox_Y_Max->value());

  if (result) {
    controller->setDefaultGraph();
    ui->resultShow->setText("");
  } else {
    QMessageBox::warning(this, "Warn!", "Please check your input arguments");
  }
  ui->buttonGroup_Unary->blockSignals(false);
  ui->pushButton_X->blockSignals(false);
}

void s21::MainWindow::on_pushButton_CreditCalc_clicked() {
  double amount = ui->DoubleSpinBox_Credit_Amount->value();
  int term = ui->SpinBox_Credit_Term->value();
  double interest_rate = ui->DoubleSpinBox_Interest_Rate->value();
  std::string type =
      ui->ComboBox_Credit_Type->currentText().toLocal8Bit().constData();

  s21::CreditCalc::credit_output_t output =
      controller->CreditCalculate(amount, term, interest_rate, type);

  output_padding_credit(output);
}

void s21::MainWindow::on_pushButton_Deposit_calculate_clicked() {
  s21::DepositCalc::deposit_input_t input;
  input_padding_deposit(input);
  s21::DepositCalc::deposit_output_t output =
      controller->DepositCalculate(input);
  if (output.negate_check_)
    output_padding_deposit(output);
  else
    QMessageBox::warning(
        this, "Warning!",
        "Negative deposite, please check your withdraw amount");
}

void s21::MainWindow::output_padding_credit(
    s21::CreditCalc::credit_output_t &output) {
  if (ui->ComboBox_Credit_Type->currentText() == "Annuity") {
    ui->label_Total_pay->setText(QString::number(output.total_amount_, 'f', 2));
    ui->label_Excess_pay->setText(
        QString::number(output.overpayment_amount_, 'f', 2));
    ui->label_Month_pay->setText(
        QString::number(output.monthly_payment_, 'f', 2));
  } else {
    ui->label_Total_pay->setText(QString::number(output.total_amount_, 'f', 2));
    ui->label_Excess_pay->setText(
        QString::number(output.overpayment_amount_, 'f', 2));
    if (output.first_payment_ < 1000000.00) {
      ui->label_Month_pay->setText(
          QString::number(output.first_payment_, 'f', 2));
    } else {
      ui->label_Month_pay->setText(
          QString::number(output.first_payment_, 'g', 3));
    }
    ui->label_Month_pay->setText(ui->label_Month_pay->text() + " ... ");
    if (output.last_payment_ < 1000000.00) {
      ui->label_Month_pay->setText(
          ui->label_Month_pay->text() +
          QString::number(output.last_payment_, 'f', 2));
    } else {
      ui->label_Month_pay->setText(
          ui->label_Month_pay->text() +
          QString::number(output.last_payment_, 'g', 3));
    }
  }
}

void s21::MainWindow::input_padding_deposit(
    s21::DepositCalc::deposit_input_t &input) {
  input.deposit_amount_ = ui->DoubleSpinBox_Deposit_Amount->value();
  input.term_ = ui->SpinBox_Deposit_Term->value();
  input.interest_rate_ = ui->DoubleSpinBox_Deposit_Interest_rate->value();
  input.income_tax_rate_ = ui->DoubleSpinBox_Deposit_Income_tax->value();
  input.income_tax_ceiling_ = ui->DoubleSpinBox_Deposit_Tax_exemption->value();
  input.refinance_rate_ = ui->DoubleSpinBox_Deposit_Refinance_rate->value();
  input.reinvest_schema_ = (s21::DepositCalc::investment_schema_t)(
      ui->ComboBox_Deposit_Interest_compaunding->currentIndex());
  input.topup_schema_ = (s21::DepositCalc::investment_schema_t)(
      ui->ComboBox_Deposit_Topup_schema->currentIndex());
  input.withdraw_schema_ = (s21::DepositCalc::investment_schema_t)(
      ui->ComboBox_Deposit_Withdraw_schema->currentIndex());
  input.topup_amount_ = ui->DoubleSpinBox_Deposit_Topup_amount->value();
  input.draw_amount_ = ui->DoubleSpinBox_Deposit_Withdraw_amount->value();
}

void s21::MainWindow::output_padding_deposit(
    s21::DepositCalc::deposit_output_t &output) {
  if (output.final_deposit_amount_ < 1000000000.00) {
    ui->label_Deposit_total_amount->setText(
        QString::number(output.final_deposit_amount_, 'f', 2));
    ui->label_Deposit_tax_amount->setText(
        QString::number(output.tax_payment_, 'f', 2));
    ui->label_Deposit_total_interest->setText(
        QString::number(output.final_interest_amount_, 'f', 2));
    ui->label_Deposit_net_amount->setText(
        QString::number(output.net_amount_, 'f', 2));
    ui->label_Deposit_total_compaunding->setText(
        QString::number(output.total_compound_amount_, 'f', 2));
    ui->label_Deposit_total_topup->setText(
        QString::number(output.total_topup_amount_, 'f', 2));
    ui->label_Deposit_total_draw->setText(
        QString::number(output.total_draw_amount_, 'f', 2));
    ui->label_Deposit_net_amount->setText(
        QString::number(output.net_amount_, 'f', 2));
  } else {
    ui->label_Deposit_total_amount->setText(
        QString::number(output.final_deposit_amount_, 'g', 3));
    ui->label_Deposit_tax_amount->setText(
        QString::number(output.tax_payment_, 'g', 3));
    ui->label_Deposit_total_interest->setText(
        QString::number(output.final_interest_amount_, 'g', 3));
    ui->label_Deposit_net_amount->setText(
        QString::number(output.net_amount_, 'g', 3));
    ui->label_Deposit_total_compaunding->setText(
        QString::number(output.total_compound_amount_, 'g', 3));
    ui->label_Deposit_total_topup->setText(
        QString::number(output.total_topup_amount_, 'g', 3));
    ui->label_Deposit_total_draw->setText(
        QString::number(output.total_draw_amount_, 'g', 3));
    ui->label_Deposit_net_amount->setText(
        QString::number(output.net_amount_, 'g', 3));
  }
}

void s21::MainWindow::on_pushButton_Deposit_clear_clicked() {
  ui->DoubleSpinBox_Deposit_Amount->setValue(100000.00);
  ui->SpinBox_Deposit_Term->setValue(12);
  ui->DoubleSpinBox_Deposit_Interest_rate->setValue(10);
  ui->DoubleSpinBox_Deposit_Income_tax->setValue(13);
  ui->DoubleSpinBox_Deposit_Tax_exemption->setValue(100000.00);
  ui->DoubleSpinBox_Deposit_Refinance_rate->setValue(8);
  ui->DoubleSpinBox_Deposit_Topup_amount->setValue(0.00);
  ui->DoubleSpinBox_Deposit_Withdraw_amount->setValue(0.00);
  ui->ComboBox_Deposit_Interest_compaunding->setCurrentIndex(0);
  ui->ComboBox_Deposit_Topup_schema->setCurrentIndex(0);
  ui->ComboBox_Deposit_Withdraw_schema->setCurrentIndex(0);
  ui->label_Deposit_net_amount->setText(QString::number(0.00, 'f', 2));
  ui->label_Deposit_tax_amount->setText(QString::number(0.00, 'f', 2));
  ui->label_Deposit_total_amount->setText(QString::number(0.00, 'f', 2));
  ui->label_Deposit_total_compaunding->setText(QString::number(0.00, 'f', 2));
  ui->label_Deposit_total_draw->setText(QString::number(0.00, 'f', 2));
  ui->label_Deposit_total_interest->setText(QString::number(0.00, 'f', 2));
  ui->label_Deposit_total_topup->setText(QString::number(0.00, 'f', 2));
}

void s21::MainWindow::on_pushButton_CreditClear_clicked() {
  ui->DoubleSpinBox_Credit_Amount->setValue(0.00);
  ui->SpinBox_Credit_Term->setValue(0);
  ui->DoubleSpinBox_Interest_Rate->setValue(0.00);
  ui->ComboBox_Credit_Type->setCurrentIndex(0);
  ui->label_Month_pay->setText(QString::number(0.00, 'f', 2));
  ui->label_Excess_pay->setText(QString::number(0.00, 'f', 2));
  ui->label_Total_pay->setText(QString::number(0.00, 'f', 2));
  ui->DoubleSpinBox_X_Value->setValue(0.00);
  ui->DoubleSpinBox_X_Max->setValue(0.00);
  ui->DoubleSpinBox_Y_Max->setValue(0.00);
}
