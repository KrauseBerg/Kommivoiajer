#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->generate->setDisabled(true);
    ui->next->setDisabled(true);
}

MainWindow::~MainWindow(){
    delete ui;
    delete [] model;
    delete [] model_2;
    delete [] item;
    delete [] item_2;
    delete [] item_3;
    delete [] ss;
    delete [] puti_mas;
}

bool MainWindow::find_cicle(int a, int b, bool check){
    int first = -1, second = -1;
    for (int i = 0 ; i < stat_size; i++) {
        if (list[i].isEmpty())
            continue;
        for (int j = 0; j < list[i].size(); j++) {
            if(stat_elements[a][b].x == list[i][j])
                first = i;
            if(stat_elements[a][b].y == list[i][j])
                second = i;
        }
    }
    if (!check){                                                                     //Распределение
        if (first != second && first != -1 && second != -1){
            list[first] += list[second];
            list[second].clear();
            return false;
        }
        if (first == -1 && second == -1){
            list[count_of_list] << stat_elements[a][b].x << stat_elements[a][b].y;
            count_of_list ++;
            return false;
        }
        if (first != -1 || second != -1){
            if (first == second)
                return true;
            else {
                int first_count = 0, second_count = 0;
                if (first != -1){
                    for(int i = 0 ; i < list[first].size(); i++)
                        if(stat_elements[a][b].x == list[first][i])
                            first_count ++;
                    if (first_count == 1){
                        list[first] << stat_elements[a][b].y;
                        return false;
                    }
                    else
                        return true;
                }
                if (second != -1){
                    for(int i = 0 ; i < list[second].size(); i++)
                        if(stat_elements[a][b].y == list[second][i])
                            second_count ++;
                    if (second_count == 1){
                        list[second] << stat_elements[a][b].x;
                        return false;
                    }
                    else
                        return true;
                }
            }
        }
        return 1;
    }
    else {                                                                           //Проверка на вхождение
        if (first == second && first != -1 && second != -1)
            return true;
        else
            return false;
    }
}

int MainWindow::find_min(int pos, bool line){
    int min = 0xFFFF;
    for (int j = 0; j < size; j++) {
        if (line)
            min = elements[pos][j].value < min ?
                        elements[pos][j].value :min;
        else
            min = elements[j][pos].value < min ?
                        elements[j][pos].value :min;
    }
    return min;
}

int MainWindow::find_min(int pos, bool line, int pos_without){
    int min = 0xFFFF;
    for (int j = 0; j < size; j++) {
        if (j == pos_without)
            continue;
        if (line)
            min = elements[pos][j].value < min ?
                        elements[pos][j].value :min;
        else
            min = elements[j][pos].value < min ?
                        elements[j][pos].value :min;
    }
    return min;
}

void MainWindow::find_min_line_rec(){
    int min = 0;
    for (int i = 0; i < size; i++) {
        min = find_min(i,true);
        for (int j = 0; j < size; j++)
            if (elements[i][j].value != 0xFFFF)
                elements[i][j].value -= min;
    }
}

void MainWindow::find_min_column_rec(){
    int min = 0;
    for (int i = 0; i < size; i++) {
        min = find_min(i,false);
        for (int j = 0; j < size; j++)
            if (elements[j][i].value != 0xFFFF)
                elements[j][i].value -= min;
    }
}

void MainWindow::delete_line_column(int x, int y, int &size){
    size -=1;
    int new_i = 0, new_j;
    coords **matrica = new coords*[size];
    for (int i = 0; i < size; i++) {
        if (i == x)
            new_i ++;
        matrica[i] = new coords[size];
        new_j = 0;
        for (int j = 0; j < size; j++) {
            if (j == y)
                new_j ++;
            matrica[i][j] = elements[new_i][new_j];
            new_j ++;
        }
        new_i ++;
    }
    elements = new coords*[size];
    for (int i = 0; i < size; i++)
        elements[i] = new coords[size];
    elements = matrica;
}

void MainWindow::find_max_0(){
    int max_value = 0, max_i = 0, max_j = 0, curr_i = 0, curr_j = 0, curr_value=0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if(elements[i][j].value == 0){
                elements[i][j].ocenka = find_min(i,true,j) + find_min(j,false,i);    //Вычисляем оценку
                if (elements[i][j].ocenka >= max_value){
                    if( !find_cicle(elements[i][j].x, elements[i][j].y, true) ||
                            global_per == stat_size - 1) {
                        max_value = elements[i][j].ocenka;                               //Берём максимальный из всех оценок
                        max_i = elements[i][j].x; max_j = elements[i][j].y;              //Берём её координаты согласно исходной
                        curr_i = i; curr_j = j;                                          //Берём координаты согласно текущей матрицы
                        curr_value = stat_elements[max_i][max_j].value;                  //Берём путь дуги (из исходной матрицы)
                    }
                }
            }


    find_cicle(max_i, max_j, false);
    ss[global_per]="("+QString::number(max_i + 1)+", "+QString::number(max_j + 1)+")";
    puti_mas[global_per].pred = max_i + 1;
    puti_mas[global_per].sled = max_j + 1;
    puti_mas[global_per].value = curr_value;
    global_per ++;

    delete_line_column(curr_i, curr_j, size);                                        //Удаляем строку и колонну
    model_2 = new QStandardItemModel;

    for(int i = 0; i < size; i++){
        for (int j = 0; j < size; j++) {
            if(elements[i][j].x == max_j and elements[i][j].y == max_i)
                elements[i][j].value = 0xFFFF;                                       //Обратный путь - бесконечность
            if(find_cicle(elements[i][j].x, elements[i][j].y, true) && global_per != stat_size - 1){
                elements[i][j].value = 0xFFFF;
            }
        }
    }
}

void MainWindow::on_create_clicked(){
    ui->next->setDisabled(true);
    model = new QStandardItemModel;
    model_2 = new QStandardItemModel;
    size = ui->size->text().toInt();
    ss = new QString[size];
    list = new QList <int> [size];
    puti_mas = new puti[size];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            item = new QStandardItem("0");
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i, j, item);
        }
    }
    ui->initial->setModel(model);
    ui->generate->setDisabled(false);
}

void MainWindow::on_generate_clicked(){
    ui->next->setDisabled(false);
    int elem;
    srand(static_cast<unsigned int>(time(nullptr))); //Для рандомизации от времени
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if(i == j)
                item = new QStandardItem(QString("∞"));
            else {
                elem = rand()%10 + 1; //Случайно (от 1 до 10)
                item = new QStandardItem(QString::number(elem));
            }
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i, j, item);
        }
    }
    ui->initial->setModel(model);
}

void MainWindow::on_next_clicked(){
    if (global_per == 0){
        int elem;
        elements = new coords*[size];
        stat_elements = new coords*[size];
        //        stat_size = size;
        for(int i = 0; i < size; i++){
            elements[i] = new coords[size];
            stat_elements[i] = new coords[size];
            for (int j = 0; j < size; j++){
                elem = ui->initial->model()->itemData(ui->initial->
                                                      model()->index(i,j,QModelIndex())).value(0).toInt();
                if (elem == 0)
                    elem = 0xFFFF;
                elements[i][j].value = elem;
                stat_elements[i][j].value = elem;
                elements[i][j].x = i;
                stat_elements[i][j].x = i;
                elements[i][j].y = j;
                stat_elements[i][j].y = j;
            }
        }
    }

    stat_size = size;
    for(int i = 0 ; i < stat_size; i++){
        find_min_line_rec();
        find_min_column_rec();
        find_max_0();
    }

    int next = 1;
    int value = 0;
    for (int i = 0 ; i < stat_size; i++){
        for (int j = 0; j < stat_size; j++) {
            if (puti_mas[j].pred == next){
                next = puti_mas[j].sled;
                item_2 = new QStandardItem(ss[j]);
                model_2->setItem(0, i, item_2);
                value += puti_mas[j].value;
                break;
            }
        }
    }
    ui->lenght->setText(QString::number(value));
    ui->short_track->setModel(model_2);
}
