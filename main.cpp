#include "mainwindow.h"
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDate>
#include <QFile>
#include <QTextStream>

// Функция для вычисления индекса массы тела, входные параметры: вес (кг), рост (см)
double calculateBMI(double weight, double height) {
    if (height == 0) return 0;
    return weight / ((height / 100) * (height / 100));
}

// Функция для расчёта основного обмена веществ в ккал, входные параметры: вес (кг), рост (см), возраст (лет), уровень активности, пол
double calculateBasicMetabolism(double weight,double height,int age,double activity, QString gender)
{
    if (gender=="female")
    {
        return 655+(9.6*weight)+(1.8*height)-(4.7*age*activity);
    }
    if (gender=="male")
    {
        return 66+(13.7*weight)+(5*height)-(6.76*age*activity);
    }
    else { return 0;}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    double BMItoFile=0;
    double BMRtoFile=0;
    QDate present=QDate::currentDate();
    QString strpresent=present.toString("yyyy-MM-dd");
    QWidget window;
    window.setWindowTitle("Calculator of body");
    QLabel resultLabel("Your BMI will be shown here");
    QLabel resultLabel2("Your BMR will be shown here");
    QLabel current_date("The date of the measurement will be shown here");
    QPushButton button("Calculate the BMI");
    QPushButton button2("Calculate the BMR");
    QPushButton button3("Save the results to a file");

    // Кнопка, реализующая вычисление индекса массы тела
    QObject::connect(&button,&QPushButton::clicked,[&]()
    {
        bool ok;
        double weight=QInputDialog::getDouble(&window,"Enter the weight","Enter your weight in kg:",0.0,40,500,1,&ok);
        if (!ok) return;

        double height=QInputDialog::getDouble(&window,"Enter the height","Enter your height in sm:",0.0,140,230,1,&ok);
        if (!ok) return;
        double BMI=calculateBMI(weight,height);
        resultLabel.setText("Your BMI: "+QString::number(BMI,'f',2));
        QString resume;
        if (BMI<18.5) {
            resume="Underweight";
        }
        else if (18.5<=BMI && BMI<24.9)
        {
            resume="Normal weight";
        }
        else if (24.9<=BMI && BMI<29.9)
        {
            resume="Overweight";
        }
        else {
            resume="Fatness";
        }
        QMessageBox::information(&window,"Result of BMI","Your BMI: "+QString::number(BMI,'f',2)+'\n'+resume);
        current_date.setText("Date of measurement: "+strpresent);
        BMItoFile=BMI;

    });

    // Кнопка, реализующая расчёт основного обмена веществ
    QObject::connect(&button2,&QPushButton::clicked,[&]()
                     {
                         bool ok2;
                         double weight=QInputDialog::getDouble(&window,"Enter the weight","Enter your weight in kg:",0.0,0,500,1,&ok2);
                         if (!ok2) return;

                         double height=QInputDialog::getDouble(&window,"Enter the height","Enter your height in sm:",0.0,0,230,1,&ok2);
                         if (!ok2) return;
                         int age=QInputDialog::getInt(&window,"Enter the age","Enter your age in years:",0,18,100,1,&ok2);
                         if (!ok2) return;

                         double activity;
                         QStringList activities;
                         activities<<"little activity"<<"low activity (once or twice a week)"<<"moderate activity (three times a week)"<<"high activity (five times a week)"<<"very high activity (every day)";
                         QString type_of_activity=QInputDialog::getItem(nullptr,"Choose the activity","Choose your activity:",activities,0,false,&ok2);
                         if (type_of_activity=="little activity")
                             {activity=1.2;}
                        else if (type_of_activity=="low activity (once or twice a week)")
                             {activity=1.375;}
                        else if (type_of_activity=="moderate activity (three times a week)")
                             {activity=1.55;}
                        else if (type_of_activity=="high activity (five times a week)")
                         {activity=1.725;}
                        else if (type_of_activity=="very high activity (every day)")
                         {activity=1.9;}

                         QStringList genders;
                         genders<<"female"<<"male";
                         QString gender=QInputDialog::getItem(nullptr,"Choose the gender","Choose your gender:",genders,0,false,&ok2);
                         double BMR=calculateBasicMetabolism(weight,height,age,activity,gender);

                         resultLabel2.setText("Your BMR: "+QString::number(BMR,'f',2));

                         QMessageBox::information(&window,"Result of BMR","Your BMR: "+QString::number(BMR,'f',2));
                         current_date.setText("Date of measurement: "+strpresent);
                         BMRtoFile=BMR;

                     });

    // Кнопка, реализующая запись данных в текстовый файл
    QObject::connect(&button3,&QPushButton::clicked,[&]()
                     {
                        bool ok3;
                        QString name=QInputDialog::getText(&window,"Enter the name","Enter your name:",QLineEdit::Normal,"",&ok3);
                        if (!ok3 || name.isEmpty()) return;
                        QString surname=QInputDialog::getText(&window,"Enter the surname","Enter your surname:",QLineEdit::Normal,"",&ok3);
                        if (!ok3 || surname.isEmpty()) return;
                        QString patronymic=QInputDialog::getText(&window,"Enter the patronymic","Enter your patronymic:",QLineEdit::Normal,"",&ok3);
                        if (!ok3 || patronymic.isEmpty()) return;
                         QString filename="Measurement results from "+strpresent+".txt";
                         QFile file(filename);
                         if ((file.open(QIODevice::WriteOnly)) && (BMItoFile!=0 || BMRtoFile!=0))
                         { QTextStream out(&file);
                             out<<surname<<" "<<name<<" "<<patronymic<<Qt::endl;
                             out<<"BMI: "<<BMItoFile<<Qt::endl;
                             out<<"BMR: "<<BMRtoFile<<Qt::endl;
                             QMessageBox::information(&window,"The result of writing to a file","The data has been successfully written to the file.");
                            }
                        else {QMessageBox::information(&window,"The result of writing to a file","Error writing data to a file. There is not enough data about BMI or BMR.");}

                     });

    QVBoxLayout layout;
    layout.addWidget(&resultLabel);
    layout.addWidget(&resultLabel2);
    layout.addWidget(&current_date);
    layout.addWidget(&button);
    layout.addWidget(&button2);
    layout.addWidget(&button3);
    window.setLayout(&layout);
    window.show();

    return a.exec();
}
