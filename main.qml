import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Universal 2.12

ApplicationWindow {
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
                text: qsTr("Video")
                onClicked: m_container.source = "res/pages/Video.qml"
            }
            ToolButton {
                text: qsTr("Audio")
                onClicked: m_container.source = "res/pages/Audio.qml"
            }
            ToolButton {
                text: qsTr("Motion")
                onClicked: m_container.source = "res/pages/Motion.qml"
            }
            ToolButton {
                text: qsTr("Outputs")
                onClicked: m_container.source = "res/pages/Outputs.qml"
            }
            ToolButton {
                text: qsTr("Security")
                onClicked: m_container.source = "res/pages/Security.qml"
            }
            ToolButton {
                text: qsTr("Plugins")
                onClicked: m_container.source = "res/pages/Plugins.qml"
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
        width: parent.width
        height: parent.height
    }
}
