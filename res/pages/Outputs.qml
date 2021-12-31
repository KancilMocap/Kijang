import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    width: parent.width
    height: parent.height

    RowLayout {
        ColumnLayout {
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
                        text: qsTr("Read port (TCP)")
                        color: "white"
                    }
                    SpinBox {
                        id: commPortValue
                        from: 1
                        to: 65535
                        value: kijangNetworkManager == null ? 4318 : kijangNetworkManager.readServerPort
                        onValueChanged: kijangNetworkManager.readServerPort = value
                        editable: true
                    }
                    Button {
                        text: qsTr("Autodetect")
                        onClicked: kijangNetworkManager.readAutoDetect();
                    }
                }
                Text {
                    id: commWarning
                    color: "red"
                    visible: outputEthernet.checked && (kijangNetworkManager == null ? false : kijangNetworkManager.readServerError)
                    text: kijangNetworkManager == null ? "" : kijangNetworkManager.readServerErrorString
                }

                RowLayout {
                    Label {
                        text: qsTr("Write port (TCP)")
                        color: "white"
                    }
                    SpinBox {
                        id: writePortValue
                        from: 1
                        to: 65535
                        value: kijangNetworkManager == null ? 4319 : kijangNetworkManager.writeServerPort
                        onValueChanged: kijangNetworkManager.writeServerPort = value
                        editable: true
                    }
                    Button {
                        text: qsTr("Autodetect")
                        onClicked: kijangNetworkManager.writeAutoDetect();
                    }
                }
                Text {
                    id: writeWarning
                    color: "red"
                    visible: outputEthernet.checked && (kijangNetworkManager == null ? false : kijangNetworkManager.writeServerError)
                    text: kijangNetworkManager == null ? "" : kijangNetworkManager.writeServerErrorString
                }

                RowLayout {
                    Label {
                        text: qsTr("Status update port (TCP)")
                        color: "white"
                    }
                    SpinBox {
                        id: statusPortValue
                        from: 1
                        to: 65535
                        value: kijangNetworkManager == null ? 4320 : kijangNetworkManager.statusServerPort
                        onValueChanged: kijangNetworkManager.statusServerPort = value
                        editable: true
                    }
                    Button {
                        text: qsTr("Autodetect")
                        onClicked: kijangNetworkManager.statusAutodetect();
                    }
                }
                Text {
                    id: statusWarning
                    color: "red"
                    visible: outputEthernet.checked && (kijangNetworkManager == null ? false : kijangNetworkManager.statusServerError)
                    text: kijangNetworkManager == null ? "" : kijangNetworkManager.statusServerErrorString
                }

                RowLayout {
                    Label {
                        text: qsTr("Input devices")
                        color: "white"
                    }
                    SpinBox {
                        id: inputDeviceValue
                        from: 0
                        to: 10000
                        editable: true
                    }
                    Text {
                        id: inputDeviceNotif
                        color: "white"
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

        }
    }
}
