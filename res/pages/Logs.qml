import QtQuick
import QtQuick.Controls

ScrollView {
    implicitWidth: 100
    implicitHeight: 100

    Text {
        font.bold: true
        text: qsTr("Logs")
    }

    Text {
        objectName: "logsUI"
    }
}
