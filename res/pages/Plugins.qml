import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
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
            onClicked: selectPlugin.open()
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

    FileDialog {
        id: selectPlugin
        title: qsTr("Select a file")
        currentFolder: shortcuts.home
    }
}
