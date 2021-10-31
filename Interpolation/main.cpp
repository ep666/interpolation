#include "Interpolation.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QFile table("data.txt");

    if (!table.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open file for reading");
        return 1;
    }

    QTextStream in(&table);
    QVector<double> data;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        data.push_back(line.toDouble());
    }

    table.close();

    QApplication a(argc, argv);

    Linear w1(data);

    w1.resize(800, 700);
    w1.setWindowTitle("Line Interpolation");
    w1.show();

    Quadratic w2(data);

    w2.resize(800, 700);
    w2.setWindowTitle("Quadratic Interpolation");
    w2.show();

    Cubic w3(data);

    w3.resize(800, 700);
    w3.setWindowTitle("Cubic Interpolation");
    w3.show();

    return a.exec();
}
