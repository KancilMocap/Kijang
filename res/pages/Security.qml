import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs

ColumnLayout {
    width: parent.width
    height: parent.height

    RowLayout {
        id: securityButtonRow
        Layout.alignment: Qt.AlignTop
        Button {
            text: qsTr("Change Access") // TODO: Complete
        }
        Button {
            text: qsTr("Change Password") // TODO: Complete
        }
    }

    ScrollView {
        height: parent.height - securityButtonRow.height
        Layout.alignment: Qt.AlignTop
        TableView {
            id: securityTable
            objectName: "securityTable" // Kijang itself has a block that can be changed access, so a table will always be needed
        }
    }
}
