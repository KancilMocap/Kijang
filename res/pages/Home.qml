import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    implicitWidth: 100
    implicitHeight: 100

    RowLayout {
        width: parent.width
        height: parent.height
        ColumnLayout {
            width: parent.width * 0.4
            id: mainPanel
            Layout.alignment: Qt.AlignTop

            Text {
                text: qsTr("Normal")
                color: "white"
            }
            Text {
                text: qsTr("Connected clients: 0")
                color: "white"
            }
            Text {
                text: qsTr("Video devices: 0")
                color: "white"
            }
            Text {
                text: qsTr("Audio devices: 0")
                color: "white"
            }
            Text {
                text: qsTr("Motion capture devices: 0")
                color: "white"
            }
            Text {
                text: qsTr("Current users: 0")
                color: "white"
            }
            Button {
                text: qsTr("Clear Logs")
            }
            Button {
                text: qsTr("Open Logs File")
            }
            Button {
                text: qsTr("About")
            }
        }

        ColumnLayout {
            width: parent.width * 0.6
            height: parent.height
            Layout.alignment: Qt.AlignTop

            ScrollView {
                Text {
                    objectName: "logsUI"
                    text: kijangLogsUI == null ? "" : kijangLogsUI.logString
                    color: "white"
                }
            }
        }
    }

}
