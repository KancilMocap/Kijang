import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Universal 2.12

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Kijang")
    Universal.theme: Universal.Dark

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
            ToolButton {
                text: qsTr("Plugins")
                onClicked: m_container.source = "res/pages/Plugins.qml"
            }

            ToolButton {
                text: qsTr("Logs")
                onClicked: m_container.source = "res/pages/Logs.qml"
            }
        }
    }

    Loader {
        id: m_container
        objectName: "m_container"
        source: "res/pages/Home.qml"
        width: parent.width
        height: parent.height
    }
}
