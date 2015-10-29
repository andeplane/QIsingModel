#ifndef ISINGMODELSYSTEM_H
#define ISINGMODELSYSTEM_H
#include <QVector>
#include <QObject>
#include <QQuickPaintedItem>
#include "random.h"
#include "random2.h"

class IsingModelSystem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(double J READ J WRITE setJ NOTIFY JChanged)
    Q_PROPERTY(double energy READ energy WRITE setEnergy NOTIFY energyChanged)
    Q_PROPERTY(double energyPerSpin READ energyPerSpin NOTIFY energyPerSpinChanged)
    Q_PROPERTY(double magnetizationPerSpin READ magnetizationPerSpin NOTIFY magnetizationPerSpinChanged)
    Q_PROPERTY(double temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(int magnetization READ magnetization WRITE setMagnetization NOTIFY magnetizationChanged)
    Q_PROPERTY(long acceptedConfigurations READ acceptedConfigurations WRITE setAcceptedConfigurations NOTIFY acceptedConfigurationsChanged)
    Q_PROPERTY(long steps READ steps WRITE setSteps NOTIFY stepsChanged)
    Q_PROPERTY(unsigned int size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(double energyVariance READ energyVariance NOTIFY energyVarianceChanged)
    Q_PROPERTY(double magnetizationVariance READ magnetizationVariance NOTIFY magnetizationVarianceChanged)
    Q_PROPERTY(QColor spinUpColor READ spinUpColor WRITE setSpinUpColor NOTIFY spinUpColorChanged)
    Q_PROPERTY(QColor spinDownColor READ spinDownColor WRITE setSpinDownColor NOTIFY spinDownColorChanged)
public:
    explicit IsingModelSystem(QQuickItem *parent = 0);
    ~IsingModelSystem();
    void randomize();
    double J() const;
    double energy() const;
    int magnetization() const;
    double temperature() const;
    long acceptedConfigurations() const;
    long steps() const;
    unsigned int size() const;
    double energyVariance() const;
    double magnetizationVariance() const;
    Q_INVOKABLE void initialize(unsigned int size, long seed = -1);
    Q_INVOKABLE void tick();

    virtual void paint(QPainter *painter);
    QColor spinUpColor() const;
    QColor spinDownColor() const;
    double energyPerSpin() const;
    double magnetizationPerSpin() const;

public slots:
    void reset();
    void setJ(double J);
    void setEnergy(double energy);
    void setMagnetization(int magnetization);
    void setTemperature(double temperature);
    void setAcceptedConfigurations(long acceptedConfigurations);
    void setSteps(long steps);
    void setSize(unsigned int size);
    void setSpinUpColor(QColor spinUpColor);
    void setSpinDownColor(QColor spinDownColor);

signals:
    void JChanged(double J);
    void energyChanged(double energy);
    void magnetizationChanged(int magnetization);
    void temperatureChanged(double temperature);
    void acceptedConfigurationsChanged(long acceptedConfigurations);
    void stepsChanged(long steps);
    void sizeChanged(unsigned int size);
    void energyVarianceChanged(double energyVariance);
    void magnetizationVarianceChanged(double magnetizationVariance);
    void spinUpColorChanged(QColor spinUpColor);
    void spinDownColorChanged(QColor spinDownColor);
    void energyPerSpinChanged(double energyPerSpin);
    void magnetizationPerSpinChanged(double magnetizationPerSpin);

private:
    QColor m_spinUpColor = "white";
    QColor m_spinDownColor;
    Random m_random;
    Random2 m_random2;
    unsigned int m_size = 10;
    long m_steps = 0;
    double m_J = 1;
    double m_temperature = 2;
    double m_energySum = 0;
    double m_energySquaredSum = 0;
    long m_magnetizationSum = 0;
    long m_magnetizationSquaredSum = 0;
    double m_energyVariance = 0;
    double m_energy = 0;
    int m_magnetization = 0;
    long m_acceptedConfigurations = 0;
    QVector<double> m_deltaEnergyTable;
    void updateDeltaEnergyTable();
    QVector<QVector<short> > m_lattice;
    void copyBoundary();
    double calculateEnergy();
    int calculateMagnetization();
};

#endif // ISINGMODELSYSTEM_H
