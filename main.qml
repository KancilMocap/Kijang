import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Kijang")

    header: ToolBar {
        width: parent.width

        RowLayout {
            ToolButton {
                text: qsTr("Home")
                onClicked: m_container.source = "res/pages/Home.qml"
            }
            ToolButton {
                text: qsTr("Inputs")
                onClicked: m_container.source = "res/pages/Input.qml"
            }
            ToolButton {
                text: qsTr("Settings")
                onClicked: m_container.source = "res/pages/Settings.qml"
            }
        }
    }

    Loader {
        id: m_container
        objectName: "m_container"
        source: "res/pages/Home.qml"
    }
}
