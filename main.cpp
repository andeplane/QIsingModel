#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QDebug>
#include <QElapsedTimer>
#include <iostream>
#include "isingmodelsystem.h"
#include "figure.h"
#include "linegraph.h"
using namespace std;
int main(int argc, char *argv[])
{
// #define BENCHMARK
//    Random2 rnd;
//    cout << "a = [" << rnd.nextUInt(0, 5);
//    for(int i=0; i<10000; i++) {
//        cout << ", " << rnd.nextUInt(0, 5);
//    }
//    cout << "];";
//    return 0;
#ifdef BENCHMARK
    IsingModelSystem system;
    system.initialize(512);

    QElapsedTimer t;
    t.start();
    for(int i=0; i<100; i++) {
        // qDebug() << i;
        system.tick();
    }
    qDebug() << "Finished after " << t.elapsed() << "ms.";
#else
    qmlRegisterType<IsingModelSystem>("IsingModel", 1, 0, "IsingModelSystem");
    qmlRegisterType<Figure>("QMLPlot", 1, 0, "Figure");
    qmlRegisterType<LineGraph>("QMLPlot", 1, 0, "LineGraph");
    qmlRegisterType<LineGraphDataSource>("QMLPlot", 1, 0, "LineGraphDataSource");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
#endif

    return 0;
}

