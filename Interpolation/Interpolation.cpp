#include "Interpolation.h"


Linear::Linear(QWidget *parent)
    : QWidget(parent)
{

}

Linear::Linear(const QVector<double>& vec, QWidget* parent)
    : QWidget(parent), m_Data(vec)
{

}

void Linear::paintEvent(QPaintEvent* e)
{
    if (m_Data.size() < 2)
    {
        qWarning("Not enough data to draw");
        return;
    }

    QPainter painter(this);
    painter.translate(QPoint(0, height() / 2)); //center graph
    painter.setPen(QPen(QBrush("#000000"), 1));

    int dataSize = m_Data.size() - 1;

    int64_t gridStep = 40;
    int interpolationStep = 2;
    int64_t x = 0;
    double prevY;

    for (int i = 0; i < dataSize; ++i)
    {
        prevY = -m_Data[i];
        for (int j = interpolationStep; j <= gridStep; j += interpolationStep)
        {
            int64_t curX = x + j;

            double curY = m_Data[i] + ((m_Data[i + 1] - m_Data[i]) / (gridStep)) * (curX - x);
            curY *= -1;

            painter.drawLine(curX - interpolationStep, prevY, curX, curY);
            prevY = curY;

        }
        x += gridStep;
    }
}



Quadratic::Quadratic(QWidget* parent)
    : QWidget(parent)
{

}

Quadratic::Quadratic(const QVector<double>& vec, QWidget* parent)
    : QWidget(parent), m_Data(vec)
{

}

void Quadratic::paintEvent(QPaintEvent* e)
{
    if (m_Data.size() < 2)
    {
        qWarning("Not enough data to draw");
        return;
    }

    QPainter painter(this);
    painter.translate(QPoint(0, height() / 2)); //center graph
    painter.setPen(QPen(QBrush("#000000"), 1));

    //quadratic interpolation:

    int dataSize = m_Data.size() - 1;

    int64_t gridStep = 40;
    int interpolationStep = 2;
    int64_t x = 0;
    double prevY;


    for (int i = 0; i < dataSize; i+=2)
    {
        prevY = -m_Data[i];

        if (i + 2 < m_Data.size())
        {
            for (int j = interpolationStep; j <= 2*gridStep; j += interpolationStep)
            {
                int64_t curX = x + j;
                int64_t x1 = x;
                int64_t x2 = x+gridStep;
                int64_t x3 = x+2*gridStep;

                double curY = m_Data[i] * ((curX - x2) * (curX - x3)) / ((x1 - x2) * (x1 - x3)) +
                          m_Data[i + 1] * ((curX - x1) * (curX - x3)) / ((x2 - x1) * (x2 - x3)) +
                          m_Data[i + 2] * ((curX - x1) * (curX - x2)) / ((x3 - x1) * (x3 - x2));

                curY *= -1;

                painter.drawLine(curX - interpolationStep, prevY, curX, curY);
                prevY = curY;

            }
        }

        else
        {
            for (int j = interpolationStep; j <= gridStep; j += interpolationStep)
            {
                int64_t curX = x + j;

                double curY = m_Data[i] + ((m_Data[i + 1] - m_Data[i]) / (x + gridStep - x)) * (curX - x);
                curY *= -1;

                painter.drawLine(curX - interpolationStep, prevY, curX, curY);
                prevY = curY;

            }
        }

        x += 2*gridStep;
    }

}



Cubic::Cubic(QWidget* parent)
    : QWidget(parent)
{

}

Cubic::Cubic(const QVector<double>& vec, QWidget* parent)
    : QWidget(parent), m_Data(vec)
{

}

void Cubic::paintEvent(QPaintEvent* e)
{
    if (m_Data.size() < 2)
    {
        qWarning("Not enough data to draw");
        return;
    }

    int n = m_Data.size() - 1;
    int gridStep = 40;
    int interpolationStep = 2;

    // Find factors

    QVector<double> a = m_Data;
    QVector<double> x(m_Data.size());

    for (int i = 1; i < x.size(); ++i)
    {
        x[i] = x[i - 1] + gridStep;
    }

    QVector<double> b(n);
    QVector<double> d(n);
    QVector<double> h(n);

    for (int i = 0; i < n; ++i)
    {
        h[i] = x[i + 1] - x[i];
    }

    QVector<double> alpha(n);

    for (int i = 1; i < n; ++i)
    {
        alpha[i] = (3 / h[i]) * (a[i + 1] - a[i]) - (3 / h[i - 1]) * (a[i] - a[i - 1]);
    }

    QVector<double> c(n + 1);
    QVector<double> l(n + 1);
    QVector<double> mu(n + 1);
    QVector<double> z(n + 1);

    l[0] = 1; mu[0] = 0; z[0] = 0;

    for (int i = 1; i < n; ++i)
    {
        l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i] * z[i - 1]) / l[i];
    }

    l[n] = 1; z[n] = 0; c[n] = 0;

    for (int i = n - 1; i >= 0; --i)
    {
        c[i] = z[i] - mu[i] * c[i + 1];
        b[i] = (a[i + 1] - a[i]) / h[i] - (h[i] * (c[i + 1] + 2 * c[i]) / 3.0);
        d[i] = (c[i + 1] - c[i]) / (3.0 * h[i]);
    }

    //draw spline
    QPainter painter(this);
    painter.translate(QPoint(0, height() / 2)); //center graph
    painter.setPen(QPen(QBrush("#000000"), 1));


    for (int i = 0; i < n; ++i)
    {
        double prevY = -m_Data[i];
        for (int j = interpolationStep; j <= gridStep; j += interpolationStep)
        {
            int64_t curX = x[i] + j;

            double curY = d[i] * pow((curX - x[i]), 3) + c[i] * pow((curX - x[i]), 2) + b[i] * (curX - x[i]) + a[i];
            curY *= -1;

            painter.drawLine(curX - interpolationStep, prevY, curX, curY);
            prevY = curY;

        }
    }


}