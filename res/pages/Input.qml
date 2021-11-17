import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

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
