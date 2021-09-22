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
                    enabled: false
                }
                CheckBox {
                    id: outputVideo
                    text: qsTr("Output video")
                    enabled: false
                }
                CheckBox {
                    id: outputOriginalMotion
                    text: qsTr("Output original motion")
                    enabled: false
                }
            }

            CheckBox {
                // https://stackoverflow.com/a/25149119
                id: outputEthernet
                text: qsTr("Output via ethernet")
                onClicked: kijangApp.ethernetEnabled = checked
                Component.onCompleted: checked = kijangApp.ethernetEnabled
                onStateChanged: outputEthernet.checked = kijangApp.ethernetEnabled
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
                        value: kijangNetworkManager.commServerPort
                        onValueChanged: kijangNetworkManager.commServerPort = value
                        editable: true
                    }
                    Button {
                        text: qsTr("Autodetect")
                    }
                    Text {
                        id: commWarning
                        color: "red"
                        visible: kijangNetworkManager.commServerError
                        text: kijangNetworkManager.commServerErrorString
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
                        value: kijangNetworkManager.statusServerPort
                        onValueChanged: kijangNetworkManager.statusServerPort = value
                        editable: true
                    }
                    Button {
                        text: qsTr("Autodetect")
                        onClicked: kijangNetworkManager.statusAutodetect();
                    }
                    Text {
                        id: statusWarning
                        color: "red"
                        visible: kijangNetworkManager.statusServerError
                        text: kijangNetworkManager.statusServerErrorString
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
                        onClicked: kijangNetworkManager.commAutoDetect();
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
                enabled: false
                onClicked: kijangApp.serialEnabled = checked
                Component.onCompleted: checked = kijangApp.serialEnabled
                onStateChanged: outputSerial.checked = kijangApp.serialEnabled
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
