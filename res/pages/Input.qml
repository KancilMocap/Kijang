import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: parent.width
    height: parent.height

    RowLayout {
        width: parent.width
        height: parent.height

        ScrollView {
            ColumnLayout {
                Text {
                    text: qsTr("Audio Inputs")
                }
                ColumnLayout {
                    objectName: "audioInputList"
                }
            }
        }
        Rectangle {
            width: 2
            height: parent.height
            color: "black"
        }
        ScrollView {
            ColumnLayout {
                Text {
                    text: qsTr("Video Inputs")
                }
                ColumnLayout {
                    objectName: "videoInputList"
                }
            }
        }
        Rectangle {
            width: 2
            height: parent.height
            color: "black"
        }
        ScrollView {
            ColumnLayout {
                Text {
                    text: qsTr("Motion Inputs")
                }
                ColumnLayout {
                    objectName: "motionInputList"
                }
            }
        }
    }
}
