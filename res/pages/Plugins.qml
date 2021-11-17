import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs

RowLayout {
    width: parent.width
    height: parent.height

    ScrollView {
        width: parent.width * 0.8
        TableView {
            id: pluginTable
            objectName: "pluginTable"
        }
    }

    ColumnLayout {
        width: parent.width * 0.2
        Button {
            text: qsTr("Import from file")
            onClicked: kijangPluginManager.openImportDialog()
        }

        Button {
            text: qsTr("Enable")
        }

        Button {
            text: qsTr("Disable")
        }

        Button {
            text: qsTr("Delete")
        }

        Button {
            text: qsTr("Check for updates")
        }
    }
}
