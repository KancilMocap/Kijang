import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

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
            text: kijangLogsUI == null ? "" : kijangLogsUI.logString
            color: "white"
        }
    }
}
