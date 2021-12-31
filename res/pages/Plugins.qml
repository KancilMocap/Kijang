import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs

ColumnLayout {
    width: parent.width
    height: parent.height

    RowLayout {
        id: pluginButtonRow
        Layout.alignment: Qt.AlignTop
        Button {
            text: qsTr("Import from URL") // TODO: Complete
        }

        Button {
            text: qsTr("Import from file")
            onClicked: kijangPluginManager.openImportDialog()
        }

        Button {
            text: qsTr("Enable") // TODO: Complete
        }

        Button {
            text: qsTr("Disable") // TODO: Complete
        }

        Button {
            text: qsTr("Delete") // TODO: Complete
        }

        Button {
            text: qsTr("View Log File") // TODO: Complete
        }
    }

    ScrollView {
        height: parent.height - pluginButtonRow.height
        Layout.alignment: Qt.AlignTop
        TableView {
            id: pluginTable
            visible: kijangPluginManager.hasPlugins
            objectName: "pluginTable"
        }
        Text {
            visible: !kijangPluginManager.hasPlugins
            text: qsTr("No plugins enabled")
            color: "white"
        }
    }
}
