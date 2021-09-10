import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ScrollView {
    width: parent.width
    height: parent.height

    ColumnLayout {
        Text {
            font.bold: true
            text: qsTr("Logs")
            color: "white"
        }

        Text {
            objectName: "logsUI"
            text: kijangLogsUI.logString
            color: "white"
        }
    }
}
