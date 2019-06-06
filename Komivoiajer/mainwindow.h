#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QMainWindow>
#include <QModelIndex>
#include <iostream>

using namespace std;

struct coords{
    int x;
    int y;
    int value;
    int ocenka;
};

struct puti{
    int pred;
    int sled;
    int value;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_create_clicked();

    void on_generate_clicked();

    void on_next_clicked();

private:
    int find_min(int pos, bool line);
    int find_min(int pos, bool line, int pos_without);
    void find_min_line_rec();
    void find_min_column_rec();
    void find_max_0();
    void delete_line_column(int x, int y, int &size);
    bool find_cicle(int a, int b, bool check);
    QList <int> *list;
    QStandardItemModel *model;
    QStandardItemModel *model_2;
    QStandardItem *item_2;
    QStandardItem *item_3;
    QStandardItem *item;
    QString *ss;
    puti *puti_mas;
    int size;
    int stat_size;
    int global_per = 0;
    int count_of_list = 0;
    coords **elements;
    coords **stat_elements;

    QString vspomogat;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
