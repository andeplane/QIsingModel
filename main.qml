import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.0
import IsingModel 1.0
import QMLPlot 1.0

Window {
    id: windowRoot
    visible: true
    width: 1280
    height: 600
    SplitView {
        anchors.fill: parent
        Layout.alignment: Qt.AlignTop
        Layout.fillHeight: true

        IsingModelSystem {
            id: ising
            height: windowRoot.height
            Layout.minimumWidth: windowRoot.height
            size: 512
            temperature: 2.1
            NumberAnimation on temperature {
                from: 1.0; to: 3.0; duration: 100000;
            }
        }
        Column {
            Layout.fillHeight: true
            Layout.minimumWidth: 200
            Layout.fillWidth: true
            Figure {
                id: energyFigure
                width: parent.width
                height: parent.height*0.5
                yMin: -Math.abs(2*ising.J)
                yMax: Math.abs(2*ising.J)
                title: "Energy"
                xLabel: "Steps"
                yLabel: "<E>"
                LineGraph {
                    id: energyPlot
                    width: 2
                    dataSource: LineGraphDataSource {
                        saveMemory: false
                    }
                }
            }

            Figure {
                id: magnetizationFigure
                width: parent.width
                height: parent.height*0.5
                title: "Magnetization"
                yMin: -1
                yMax: 1
                xLabel: "Steps"
                yLabel: "<M>"
                LineGraph {
                    id: magnetizationPlot
                    width: 2
                    dataSource: LineGraphDataSource {
                        saveMemory: false
                    }
                }
            }
        }
        Column {
            id: controlColumn
            anchors.top: parent.top
            Layout.minimumWidth: 150
            Layout.maximumWidth: 150
            spacing: 5
            anchors.topMargin: 5
            Row {
                Label {
                    width: 80
                    text: "Up color: "
                }
                Rectangle {
                    id: upRectangle
                    border.color: "black"
                    height: 20
                    width: 20
                    color: ising.spinUpColor
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.visible = true
                            colorDialog.selectedSpin = 1
                            colorDialog.color = ising.spinUpColor
                        }
                    }
                }
            }

            Row {
                Label {
                    width: 80
                    text: "Down color: "
                }
                Rectangle {
                    id: downRectangle
                    border.color: "black"
                    height: 20
                    width: 20
                    color: ising.spinDownColor
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.visible = true
                            colorDialog.selectedSpin = 0
                            colorDialog.color = ising.spinDownColor
                        }
                    }
                }
            }

            Row {
                spacing: 10
                Label {
                    text: "T="+ising.temperature.toFixed(1)
                }

                Slider {
                    id: temperatureSlider
                    minimumValue: 0.0
                    maximumValue: 10.0
                    value: ising.temperature
                    onValueChanged: {
                        ising.temperature = value
                    }
                }
            }
        }
    }

    ColorDialog {
        id: colorDialog
        property int selectedSpin: 0
        onColorChanged: {
            if(selectedSpin == 1) {
                ising.spinUpColor = color
            } else {
                ising.spinDownColor = color
            }
        }
        onAccepted: {
            selectedSpin = -1
        }
        onRejected: {
            selectedSpin = -1
        }
    }

    Timer {
        running: true
        repeat: true
        interval: 1
        onTriggered: {
            ising.tick()
            energyPlot.addPoint(ising.steps, ising.energyPerSpin)
            energyFigure.xMax = parseFloat(ising.steps)
            magnetizationPlot.addPoint(ising.steps, ising.magnetizationPerSpin)
            magnetizationFigure.xMax = parseFloat(ising.steps)
        }
    }
}

