#ifndef SETTING_H_
#define SETTING_H_

#include <QDialog>
#include <QVariant>
#include <QSettings>
#include <QPushButton>
#include <QColorDialog>
#include <QFontDatabase>
#include <QCoreApplication>

typedef struct options {
    QColor colorBg;
    int projectionType;
    int lineType;
    QColor colorLine;
    double widthLine;
    int vertexType;
    QColor colorVertex;
    double vertexSize;
} options;

namespace Ui {
class setting;
}

class setting : public QDialog {
    Q_OBJECT

public:
    explicit setting(QWidget *parent = nullptr, options settings = {});
    ~setting();

signals:
    void changeSettings(options);

private slots:
    void setColor();
    void saveConfiguration();
    void setDefaultSetting();

private:
    Ui::setting *ui;
    options currentSetting;
    void updateValues();
};
#endif  // SETTING_H_
