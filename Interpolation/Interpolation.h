#pragma once

#include <QtWidgets>
#include <QVector>
#include <math.h>

class Linear : public QWidget
{
    QVector<double> m_Data;

public:
    Linear(QWidget *parent = Q_NULLPTR);
    Linear(const QVector<double>& vec, QWidget* parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent* event);
};

class Quadratic : public QWidget
{
    QVector<double> m_Data;

public:
    Quadratic(QWidget* parent = Q_NULLPTR);
    Quadratic(const QVector<double>& vec, QWidget* parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent* event);
};

class Cubic : public QWidget
{
    QVector<double> m_Data;

public:
    Cubic(QWidget* parent = Q_NULLPTR);
    Cubic(const QVector<double>& vec, QWidget* parent = Q_NULLPTR);

protected:
    void paintEvent(QPaintEvent* event);
};
