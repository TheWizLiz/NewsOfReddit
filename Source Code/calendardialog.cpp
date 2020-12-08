#include <QCalendarWidget>
#include <QDate>
#include <QApplication>
#include <QMessageBox>
#include <chrono>

#include "calendardialog.h"
#include "ui_calendardialog.h"

Calendar::Calendar(QWidget *parent) :

    QDialog(parent)
{
    ui.setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    ui.calendarWidget->setNavigationBarVisible(false);
    ui.calendarWidget->setDateRange(QDate(2008, 01, 25), QDate(2016, 10, 22));
    ui.calendarWidget->setSelectedDate(QDate(2008, 01, 25));
    ui.DateLabel->setText(QDate(2008, 01, 25).toString("dd MMMM yyyy"));
    ui.calendarWidget->setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
    ui.calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

    QObject::connect(ui.yearBackButton,SIGNAL (clicked()),this,SLOT (on_yearBackButton_clicked()));

    QObject::connect(ui.yearFrontButton,SIGNAL (clicked()),this,SLOT (on_yearFrontButton_clicked()));
    QObject::connect(ui.MonthBackButton,SIGNAL (clicked()),this,SLOT (on_MonthBackButton_clicked()));
    QObject::connect(ui.MonthFrontButton,SIGNAL (clicked()),this,SLOT (on_MonthFrontButton_clicked()));

    QObject::connect(ui.OkButtonTable,SIGNAL (clicked()),this,SLOT (on_OkButtonTable_clicked()));
    QObject::connect(ui.OkButtonTree,SIGNAL (clicked()),this,SLOT (on_OkButtonTree_clicked()));
    QObject::connect(ui.mostLikedDaysTable,SIGNAL (clicked()),this,SLOT (on_mostLikedDaysTable_clicked()));
    QObject::connect(ui.leastLikedDaysTable,SIGNAL (clicked()),this,SLOT (on_leastLikedDaysTable_clicked()));
    QObject::connect(ui.mostLikedDaysTree,SIGNAL (clicked()),this,SLOT (on_mostLikedDaysTree_clicked()));
    QObject::connect(ui.leastLikedDaysTree,SIGNAL (clicked()),this,SLOT (on_leastLikedDaysTree_clicked()));


    model = new QStringListModel(this);
}


//Rata Die function for calculating days in a date, taken from online
int Calendar::rdn(int y, int m, int d) {
    if (m < 3)
        y--, m += 12;
    return 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5 + d - 306;
}

vector<string> Calendar::split(string in) {
    stringstream stream(in);
    string data;
    vector<string> result;

    while (getline(stream, data, ',')) {
            result.push_back(data.substr(1, data.length()-2));
        }
    return result;
}

QString Calendar::loadTable() {

    //cout << "Loading Table" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    string tempString = qApp->applicationDirPath().toStdString() + "/coolerDataset.csv";
    ifstream database(tempString);
    string line, nextLine;

    getline(database, line);
    getline(database, nextLine);
    int date, upvotes, downvotes;
    string title;
    string temp;

    while(database.good()) {
        bool valid = false;
        line = nextLine;
        while (!valid && database.good()) {
            getline(database, nextLine);
            if (nextLine[0] != '"' || nextLine[1] != '1')
                line += nextLine;
            else
                valid = true;
        }
            vector<string> splitline = split(line);

            stringstream stream(splitline[1]);
            getline(stream, temp, '-');
            int year = stoi(temp);
            getline(stream, temp, '-');
            int month = stoi(temp);
            getline(stream, temp);
            int day = stoi(temp);
            date = rdn(year, month, day) - rdn(2008, 1, 25);

            upvotes = stoi(splitline[2]);
            downvotes = stoi(splitline[3]);
            title = splitline[4];

            //cout << date << " " << upvotes << " " << downvotes << " " << title << endl;
            ht.InsertNode(date, upvotes, downvotes, title);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    QString searchLabel = "Hash Table Loaded in: ";
    QString time = QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    QString timeLabel = " [µs]";
    searchLabel.append(time);
    searchLabel.append(timeLabel);
    return searchLabel;

}

void Calendar::loadTree(QString currentLabel) {

    //cout << "Loading Tree" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    string tempString = qApp->applicationDirPath().toStdString() + "/coolerDataset.csv";
    ifstream database(tempString);
    string line, nextLine;

    getline(database, line);
    getline(database, nextLine);
    int date, upvotes, downvotes;
    string title;
    string temp;
    bool first = true;

    while (database.good()) {
            bool valid = false;
            line = nextLine;
            while (!valid && database.good()) {
                getline(database, nextLine);
                if (nextLine[0] != '"' || nextLine[1] != '1')
                    line += nextLine;
                else
                    valid = true;
            }
                vector<string> splitline = split(line);

                stringstream stream(splitline[1]);
                getline(stream, temp, '-');
                int year = stoi(temp);
                getline(stream, temp, '-');
                int month = stoi(temp);
                getline(stream, temp);
                int day = stoi(temp);
                date = rdn(year, month, day) - rdn(2008, 1, 25);


                upvotes = stoi(splitline[2]);
                downvotes = stoi(splitline[3]);
                title = splitline[4];


                if (first) {
                    bst.root = new tree::node(tree::entry(date, upvotes, downvotes, title));
                    first = false;
                    //cout << bst.root->date;
                }
                else {
                    bst.insert(bst.root, tree::entry(date, upvotes, downvotes, title));
                }
                //cout << date << " " << upvotes << " " << downvotes << " " << title << endl;
                //bst.insert(bst.root, tree::entry(date, upvotes, downvotes, title));
        }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    QString searchLabel = " and Binary Search Tree Loaded in: ";
    QString time = QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    QString timeLabel = " [µs]";
    currentLabel.append(searchLabel);
    currentLabel.append(time);
    currentLabel.append(timeLabel);
    ui.searchLabel->setText(currentLabel);
}

void Calendar::populateDate(const QStringList &l) {
    model->setStringList(l);
    ui.dayList->setModel(model);
}

QDate Calendar::currentDate() const {
    return ui.calendarWidget->selectedDate();
}

void Calendar::setCurrentDate(const QDate &d) {
    QDate date = d;
    if (!date.isValid()) {
        date = QDate(2008, 01, 25);
    }
    ui.calendarWidget->setSelectedDate(date);
    ui.calendarWidget->showSelectedDate();
    on_calendar_clicked(date);
}

void Calendar::on_calendar_clicked(const QDate &date) {
    ui.DateLabel->setText(date.toString("dd MMMM yyyy"));
}

QDate Calendar::getdate () {
    ui.DateLabel->setText(ui.calendarWidget->selectedDate().toString("dd MMMM yyyy"));
    return ui.calendarWidget->selectedDate();
}

void Calendar::on_yearBackButton_clicked() {
    QDate date = currentDate();
    date = date.addYears(-1);
    setCurrentDate(date);
}

void Calendar::on_yearFrontButton_clicked() {
    QDate date = currentDate();
    date = date.addYears(+1);
    setCurrentDate(date);
}

void Calendar::on_MonthFrontButton_clicked() {
    QDate date = currentDate();
    date = date.addMonths(+1);
    setCurrentDate(date);
}

void Calendar::on_MonthBackButton_clicked() {
    QDate date = currentDate();
    date = date.addMonths(-1);
    setCurrentDate(date);
}

void Calendar::on_OkButtonTable_clicked() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //cout << "Ok button clicked" << endl;
    QDate temp = getdate();
    int daysTillEvent = rdn(temp.year(), temp.month(), temp.day()) - rdn(2008, 1, 25);
    //cout << daysTillEvent << endl;
    QStringList List;
    vector<Post*> posts = ht.Search(daysTillEvent);

    if (posts.size() != 0) {
        string tempString;
        for (unsigned long i = 0; i < posts.size(); i++) {

            tempString = "";
            tempString.append(posts[(int) i]->getTitle());
            tempString.append(" | ");
            tempString.append(to_string(posts[(int) i]->getUpvotes()));
            tempString.append(" upvotes");
            //cout << tempString << endl;
            QString tempQString = QString::fromStdString(tempString);
            List.push_back(tempQString);
        }
    } else {
        List << "No news reported";
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    QString searchLabel = "Searched in: ";
    QString time = QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    QString timeLabel = " [µs]";
    searchLabel.append(time);
    searchLabel.append(timeLabel);
    ui.searchLabel->setText(searchLabel);
    populateDate(List);
}

void Calendar::on_OkButtonTree_clicked() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    QDate temp = getdate();
    int daysTillEvent = rdn(temp.year(), temp.month(), temp.day()) - rdn(2008, 1, 25);
    QStringList List;
    vector<tree::entry> entries = bst.find(bst.root, daysTillEvent);
    if (entries.size() != 0) {
        string tempString;
        for (unsigned long i = 0; i < entries.size(); i++) {

            tempString = "";
            tempString.append(entries[(int) i].title);
            tempString.append(" | ");
            tempString.append(to_string(entries[(int) i].upvotes));
            tempString.append(" upvotes");
            //cout << tempString << endl;
            QString tempQString = QString::fromStdString(tempString);
            List.push_back(tempQString);
        }
    } else {
        List << "No news reported";
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    QString searchLabel = "Searched in: ";
    QString time = QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    QString timeLabel = " [µs]";
    searchLabel.append(time);
    searchLabel.append(timeLabel);
    ui.searchLabel->setText(searchLabel);
    populateDate(List);
}

void Calendar::on_mostLikedDaysTable_clicked() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    QStringList List;
    int date = ht.FindLikedDay(1);
    QDate mostLikedDay = QDate(2008, 01, 25).addDays(date);
    ui.calendarWidget->setSelectedDate(mostLikedDay);
    ui.DateLabel->setText(mostLikedDay.toString("dd MMMM yyyy"));
    vector<Post*> posts = ht.Search(date);

    string tempString;
    for (unsigned long i = 0; i < posts.size(); i++) {

        tempString = "";
        tempString.append(posts[(int) i]->getTitle());
        tempString.append(" | ");
        tempString.append(to_string(posts[(int) i]->getUpvotes()));
        tempString.append(" upvotes");
        //cout << tempString << endl;
        QString tempQString = QString::fromStdString(tempString);
        List.push_back(tempQString);
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    QString searchLabel = "Searched the Hash Table in: ";
    QString time = QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    QString timeLabel = " [µs]";
    searchLabel.append(time);
    searchLabel.append(timeLabel);
    ui.searchLabel->setText(searchLabel);
    populateDate(List);
}

void Calendar::on_leastLikedDaysTable_clicked() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    QStringList List;
    int date = ht.FindLikedDay(-1);
    QDate leastLikedDay = QDate(2008, 01, 25).addDays(date);
    ui.calendarWidget->setSelectedDate(leastLikedDay);
    ui.DateLabel->setText(leastLikedDay.toString("dd MMMM yyyy"));
    vector<Post*> posts = ht.Search(date);

    string tempString;
    for (unsigned long i = 0; i < posts.size(); i++) {

        tempString = "";
        tempString.append(posts[(int) i]->getTitle());
        tempString.append(" | ");
        tempString.append(to_string(posts[(int) i]->getUpvotes()));
        tempString.append(" upvotes");
        //cout << tempString << endl;
        QString tempQString = QString::fromStdString(tempString);
        List.push_back(tempQString);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    QString searchLabel = "Searched the Hash Table in: ";
    QString time = QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    QString timeLabel = " [µs]";
    searchLabel.append(time);
    searchLabel.append(timeLabel);
    ui.searchLabel->setText(searchLabel);
    populateDate(List);
}

void Calendar::on_mostLikedDaysTree_clicked() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    QStringList List;
    int date = bst.FindLikedDay(1);
    QDate mostLikedDay = QDate(2008, 01, 25).addDays(date);
    ui.calendarWidget->setSelectedDate(mostLikedDay);
    ui.DateLabel->setText(mostLikedDay.toString("dd MMMM yyyy"));
    vector<tree::entry> entries = bst.find(bst.root, date);
    string tempString;
    for (unsigned long i = 0; i < entries.size(); i++) {

        tempString = "";
        tempString.append(entries[(int) i].title);
        tempString.append(" | ");
        tempString.append(to_string(entries[(int) i].upvotes));
        tempString.append(" upvotes");
        //cout << tempString << endl;
        QString tempQString = QString::fromStdString(tempString);
        List.push_back(tempQString);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    QString searchLabel = "Searched the Binary Search Tree in: ";
    QString time = QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    QString timeLabel = " [µs]";
    searchLabel.append(time);
    searchLabel.append(timeLabel);
    ui.searchLabel->setText(searchLabel);
    populateDate(List);
}

void Calendar::on_leastLikedDaysTree_clicked() {
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    QStringList List;
    int date = bst.FindLikedDay(-1);
    QDate leastLikedDay = QDate(2008, 01, 25).addDays(date);
    ui.calendarWidget->setSelectedDate(leastLikedDay);
    ui.DateLabel->setText(leastLikedDay.toString("dd MMMM yyyy"));
    vector<tree::entry> entries = bst.find(bst.root, date);
    string tempString;
    for (unsigned long i = 0; i < entries.size(); i++) {

        tempString = "";
        tempString.append(entries[(int) i].title);
        tempString.append(" | ");
        tempString.append(to_string(entries[(int) i].upvotes));
        tempString.append(" upvotes");
        //cout << tempString << endl;
        QString tempQString = QString::fromStdString(tempString);
        List.push_back(tempQString);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    QString searchLabel = "Searched the Binary Search Tree in: ";
    QString time = QString::number(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count());
    QString timeLabel = " [µs]";
    searchLabel.append(time);
    searchLabel.append(timeLabel);
    ui.searchLabel->setText(searchLabel);
    populateDate(List);
}
