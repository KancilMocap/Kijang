import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: parent.width
    height: parent.height

    TabBar {
        id: bar
        width: parent.width

        TabButton {
            text: qsTr("Inputs")
        }

        TabButton {
            text: qsTr("Outputs")
        }

        TabButton {
            text: qsTr("Users")
        }
    }

    StackLayout {
        width: parent.width
        height: parent.height - bar.height
        currentIndex: bar.currentIndex
        anchors.top: bar.bottom
        anchors.leftMargin: 5
        anchors.topMargin: 5
        ColumnLayout {
            id: inputTab

            Button {
                text: qsTr("Recalibrate all devices")
            }
            Button {
                text: qsTr("Manage devices")
            }
        }
        ColumnLayout {
            id: outputTab

            RowLayout {
                CheckBox {
                    id: outputAudio
                    text: qsTr("Output audio")
                }
                CheckBox {
                    id: outputVideo
                    text: qsTr("Output video")
                }
                CheckBox {
                    id: outputOriginalMotion
                    text: qsTr("Output original motion")
                }
            }

            CheckBox {
                id: outputEthernet
                text: qsTr("Output via ethernet")
            }

            ColumnLayout {
                visible: outputEthernet.checked

                RowLayout {
                    Label {
                        text: qsTr("Communications port (TCP)")
                    }
                    SpinBox {
                        id: commPortValue
                        from: 1
                        to: 65535
                        editable: true
                    }
                    Button {
                        text: qsTr("Autodetect")
                    }
                    Text {
                        id: commWarning
                        color: "red"
                        visible: false
                    }
                }

                RowLayout {
                    Label {
                        text: qsTr("Status update port (TCP)")
                    }
                    SpinBox {
                        id: statusPortValue
                        from: 1
                        to: 65535
                        editable: true
                    }
                    Button {
                        text: qsTr("Autodetect")
                    }
                    Text {
                        id: statusWarning
                        color: "red"
                        visible: false
                    }
                }

                RowLayout {
                    Label {
                        text: qsTr("Device input / output ports (UDP)")
                    }
                    SpinBox {
                        id: io1Value
                        from: 1
                        to: 65535
                        editable: true
                    }
                    Label {
                        text: qsTr(" to ")
                    }
                    SpinBox {
                        id: io2Value
                        from: 1
                        to: 65535
                        editable: true
                    }
                    Button {
                        text: qsTr("Autodetect")
                    }
                    Text {
                        id: ioNotif
                        visible: false
                    }
                }

                RowLayout {
                    Label {
                        text: qsTr("Input devices")
                    }
                    SpinBox {
                        id: inputDeviceValue
                        from: 0
                        to: 10000
                        editable: true
                    }
                    Text {
                        id: inputDeviceNotif
                    }
                }
            }

            CheckBox {
                id: outputSerial
                text: qsTr("Output via serial")
            }
            Button {
                text: qsTr("Manage serial devices")
                visible: outputSerial.checked
            }

        }
        ColumnLayout {
            id: usersTab

            CheckBox {
                id: allowMultiple
                text: qsTr("Allow multiple users")
                enabled: false
            }
            Button {
                text: qsTr("Manage users")
                visible: allowMultiple.checked
            }
        }
    }
}
