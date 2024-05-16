#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_rb_inventario_clicked();

    void on_rb_entrada_clicked();

    void on_radioButton_3_clicked();

    void on_btn_ingresar_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
