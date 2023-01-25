#include "setting.h"
#include "ui_setting.h"

setting::setting(QWidget *parent, options settings) : QDialog(parent), ui(new Ui::setting) {
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/fonts/res/fonts/roboto-regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont roboto(family);
    roboto.setPointSize(10);
    qApp->setFont(roboto);

    // Initializing settings
    currentSetting = settings;
    updateValues();

    connect(ui->colorBg_tb, &QAbstractButton::clicked, this, &setting::setColor);
    connect(ui->colorLine_tb, &QAbstractButton::clicked, this, &setting::setColor);
    connect(ui->colorVertex_tb, &QAbstractButton::clicked, this, &setting::setColor);

    connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), &QAbstractButton::clicked,
            this, &setting::setDefaultSetting);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &setting::saveConfiguration);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &setting::updateValues);
}

void setting::setColor() {
    QToolButton* button = (QToolButton *)sender();
    QString labelName = button->objectName().split("_")[0] + "_label";
    QLabel *ColorLabel = button->parent()->findChild<QLabel *>(labelName);

    const QColor color = QColorDialog::getColor(QColor(ColorLabel->text()), this, "Выбор цвета");

    if (color.isValid()) {
        ColorLabel->setText(color.name());
        ColorLabel->setPalette(QPalette(color));
    }
}

setting::~setting() {
    delete ui;
}

void setting::saveConfiguration() {
    // save settings
    options newSetting;
    // Background color
    QColor colorBg = QColor(ui->colorBg_label->text());
    newSetting.colorBg = colorBg.isValid() ? colorBg : currentSetting.colorBg;
    // Projection type
    newSetting.projectionType = ui->projectionType_cb->currentIndex();
    // Line type
    newSetting.lineType = ui->lineType_cb->currentIndex();
    // Line color
    QColor colorLine = QColor(ui->colorLine_label->text());
    newSetting.colorLine = colorLine.isValid() ? colorLine : currentSetting.colorLine;
    // Width line
    newSetting.widthLine = ui->widthLine_dsb->value();
    // Vertex type
    newSetting.vertexType = ui->vertexType_cb->currentIndex();
    // Vertex color
    QColor colorVertex = QColor(ui->colorVertex_label->text());
    newSetting.colorVertex = colorVertex.isValid() ? colorVertex : currentSetting.colorVertex;
    // Vertex size
    newSetting.vertexSize = ui->vertexSize_dsb->value();

    if (newSetting.colorBg != currentSetting.colorBg ||
            newSetting.projectionType != currentSetting.projectionType ||
            newSetting.lineType != currentSetting.lineType ||
            newSetting.colorLine != currentSetting.colorLine ||
            newSetting.widthLine != currentSetting.widthLine ||
            newSetting.vertexType != currentSetting.vertexType ||
            newSetting.colorVertex != currentSetting.colorVertex ||
            newSetting.vertexSize != currentSetting.vertexSize) {
        currentSetting = newSetting;
        emit this->changeSettings(newSetting);
    }
}

void setting::setDefaultSetting() {
    // Background color
    QColor colorBg = QColor(0, 0, 0);
    ui->colorBg_label->setText(colorBg.name());
    ui->colorBg_label->setPalette(QPalette(colorBg));
    // Projection type
    ui->projectionType_cb->setCurrentIndex(0);
    // Line type
    ui->lineType_cb->setCurrentIndex(0);
    // Line color
    QColor colorLine = QColor(255, 255, 255);
    ui->colorLine_label->setText(colorLine.name());
    ui->colorLine_label->setPalette(QPalette(colorLine));
    // Width line
    ui->widthLine_dsb->setValue(1.0);
    // Vertex type
    ui->vertexType_cb->setCurrentIndex(0);
    // Vertex color
    QColor colorVertex = QColor(255, 0, 0);
    ui->colorVertex_label->setText(colorVertex.name());
    ui->colorVertex_label->setPalette(QPalette(colorVertex));
    // Vertex size
    ui->vertexSize_dsb->setValue(1.0);
}

void setting::updateValues() {
    // Background color
    ui->colorBg_label->setText(currentSetting.colorBg.name());
    ui->colorBg_label->setPalette(QPalette(currentSetting.colorBg));
    // Projection type
    ui->projectionType_cb->setCurrentIndex(currentSetting.projectionType);
    // Line type
    ui->lineType_cb->setCurrentIndex(currentSetting.lineType);
    // Line color
    ui->colorLine_label->setText(currentSetting.colorLine.name());
    ui->colorLine_label->setPalette(QPalette(currentSetting.colorLine));
    // Width line
    ui->widthLine_dsb->setValue(currentSetting.widthLine);
    // Vertex type
    ui->vertexType_cb->setCurrentIndex(currentSetting.vertexType);
    // Vertex color
    ui->colorVertex_label->setText(currentSetting.colorVertex.name());
    ui->colorVertex_label->setPalette(QPalette(currentSetting.colorVertex));
    // Vertex size
    ui->vertexSize_dsb->setValue(currentSetting.vertexSize);
}
