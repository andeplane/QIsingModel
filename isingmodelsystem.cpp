#include "isingmodelsystem.h"
#include <QImage>
#include <QColor>
#include <QPainter>
#include <cmath>
IsingModelSystem::IsingModelSystem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    initialize(m_size);
    reset();
}

IsingModelSystem::~IsingModelSystem()
{
    m_lattice.clear();
}

void IsingModelSystem::copyBoundary()
{
    for(unsigned int i=0; i<m_size;i++) {
        m_lattice[0][i+1] = m_lattice[m_size][i+1];
        m_lattice[m_size+1][i+1] = m_lattice[1][i+1];

        m_lattice[i+1][0] = m_lattice[i+1][m_size]; // y=0 row
        m_lattice[i+1][m_size+1] =m_lattice[i+1][1]; // y=n-1 row
    }
}

double IsingModelSystem::calculateEnergy()
{
    double E = 0;
    for(unsigned int i=1;i<=m_size;i++) {
        for(unsigned int j=1;j<=m_size;j++) {
            E += m_lattice[i][j]*(m_lattice[i+1][j] + m_lattice[i][j+1]);
        }
    }
    E = -m_J*E;
    return E;
}

int IsingModelSystem::calculateMagnetization()
{
    int M = 0;
    for(unsigned int i=1; i<=m_size; i++) {
        for(unsigned int j=1; j<=m_size; j++) {
            M += m_lattice[i][j];
        }
    }
    return M;
}

void IsingModelSystem::randomize() {
    for(unsigned int i=1; i<=m_size; i++) {
        for(unsigned int j=1; j<=m_size; j++) {
            short number = 2*round(m_random.nextDouble()) - 1; // -1 or 1
            m_lattice[i][j] = number;
        }
    }

    copyBoundary();
    setEnergy(calculateEnergy());
    setMagnetization(calculateMagnetization());
}

double IsingModelSystem::J() const
{
    return m_J;
}

double IsingModelSystem::energy() const
{
    return m_energy;
}

int IsingModelSystem::magnetization() const
{
    return m_magnetization;
}

double IsingModelSystem::temperature() const
{
    return m_temperature;
}

long IsingModelSystem::acceptedConfigurations() const
{
    return m_acceptedConfigurations;
}

double IsingModelSystem::energyVariance() const
{
    double stepsMin1 = std::max(m_steps, 1l);
    return 1.0/stepsMin1*(m_energySquaredSum - m_energySum*m_energySum/stepsMin1);
}

double IsingModelSystem::magnetizationVariance() const
{
    double stepsMin1 = std::max(m_steps, 1l);
    return 1.0/stepsMin1*(m_magnetizationSquaredSum - (m_magnetizationSum*m_magnetizationSum)/stepsMin1);
}

long IsingModelSystem::steps() const
{
    return m_steps;
}

unsigned int IsingModelSystem::size() const
{
    return m_size;
}

void IsingModelSystem::setJ(double J)
{
    if (m_J == J)
        return;

    m_J = J;
    emit JChanged(J);
}

void IsingModelSystem::setEnergy(double energy)
{
    if (m_energy == energy)
        return;

    m_energy = energy;
    emit energyChanged(energy);
    emit energyPerSpinChanged(energyPerSpin());
}

void IsingModelSystem::setMagnetization(int magnetization)
{
    if (m_magnetization == magnetization)
        return;

    m_magnetization = magnetization;
    emit magnetizationChanged(magnetization);
    emit magnetizationPerSpinChanged(magnetizationPerSpin());
}

void IsingModelSystem::setTemperature(double temperature)
{
    if (m_temperature == temperature)
        return;

    m_temperature = temperature;
    updateDeltaEnergyTable();
    emit temperatureChanged(temperature);
}

void IsingModelSystem::setAcceptedConfigurations(long acceptedConfigurations)
{
    if (m_acceptedConfigurations == acceptedConfigurations)
        return;

    m_acceptedConfigurations = acceptedConfigurations;
    emit acceptedConfigurationsChanged(acceptedConfigurations);
}

void IsingModelSystem::setSteps(long steps)
{
    if (m_steps == steps)
        return;

    m_steps = steps;
    emit stepsChanged(steps);
}

void IsingModelSystem::setSize(unsigned int size)
{
    if (m_size == size)
        return;

    m_size = size;
    emit sizeChanged(size);
    initialize(m_size);
    reset();
}

void IsingModelSystem::setSpinUpColor(QColor spinUpColor)
{
    if (m_spinUpColor == spinUpColor)
        return;

    m_spinUpColor = spinUpColor;
    emit spinUpColorChanged(spinUpColor);
}

void IsingModelSystem::setSpinDownColor(QColor spinDownColor)
{
    if (m_spinDownColor == spinDownColor)
        return;

    m_spinDownColor = spinDownColor;
    emit spinDownColorChanged(spinDownColor);
}

void IsingModelSystem::updateDeltaEnergyTable()
{
    m_deltaEnergyTable.resize(5);
    double epsilon = 1e-20;
    for(int i=0;i<5;i++){
        m_deltaEnergyTable[i] = exp(-(i*4 - 8)/(m_temperature+epsilon));
    }
}

void IsingModelSystem::initialize(unsigned int size, long seed)
{
    m_random.seed = seed;
    m_size = size;
    m_lattice.resize(m_size+2);
    for(auto &vec : m_lattice) {
        vec.resize(m_size+2);
    }
    updateDeltaEnergyTable();
    randomize();
}

void IsingModelSystem::tick()
{
    //Turn as many spins as we have
    int numberOfSpins = m_size*m_size;
    int currentEnergy = m_energy / m_J;
    long currentMagnetization = m_magnetization;
    for(int k = 0;k<numberOfSpins;k++) {
        int i = m_random2.nextUInt(1,m_size);
        int j = m_random2.nextUInt(1,m_size);

        int deltaE = 2*m_lattice[i][j]*(m_lattice[i][j+1] + m_lattice[i][j-1] + m_lattice[i-1][j] + m_lattice[i+1][j]);
        if(m_deltaEnergyTable[(deltaE + 8)/4] >= m_random2.nextDouble()) {
            m_lattice[i][j] *= -1; // Flip this spin
            // Update potential boundary conditions
            m_lattice[i][m_size+1] = m_lattice[i][1];
            m_lattice[m_size+1][j] = m_lattice[1][j];
            m_lattice[0][j] = m_lattice[m_size][j];
            m_lattice[i][0] = m_lattice[i][m_size];

            currentEnergy += deltaE;
            currentMagnetization += 2*m_lattice[i][j];
            m_acceptedConfigurations++;
        }
    }
    setSteps(m_steps+1);
    setEnergy(currentEnergy*m_J);
    setMagnetization(currentMagnetization);
    emit acceptedConfigurationsChanged(m_acceptedConfigurations);
    update();
}

void IsingModelSystem::reset()
{
    m_steps = 0;
    m_energySum = 0;
    m_energySquaredSum = 0;
    m_magnetizationSum = 0;
    m_magnetizationSquaredSum = 0;
    m_energy = 0;
    m_magnetization = 0;
    m_acceptedConfigurations = 0;
    emit stepsChanged(m_steps);
    emit energyChanged(m_energy);
    emit magnetizationChanged(m_magnetization);
    emit acceptedConfigurationsChanged(m_acceptedConfigurations);
}

void IsingModelSystem::paint(QPainter *painter)
{
    QImage image(m_size, m_size, QImage::Format_RGBA8888);
    for(unsigned int i = 1; i <= m_size; i++) {
        for(unsigned int j = 1; j<= m_size; j++) {
            short value = m_lattice[i][j];
            if(value==1) {
                image.setPixel(j-1, i-1, m_spinUpColor.rgba());
            } else {
                image.setPixel(j-1, i-1, m_spinDownColor.rgba());
            }
        }
    }

    painter->drawImage(boundingRect(), image);
}

QColor IsingModelSystem::spinUpColor() const
{
    return m_spinUpColor;
}

QColor IsingModelSystem::spinDownColor() const
{
    return m_spinDownColor;
}

double IsingModelSystem::energyPerSpin() const
{
    return m_energy / (m_size*m_size);
}

double IsingModelSystem::magnetizationPerSpin() const
{
    return double(m_magnetization) / (m_size*m_size);
}
