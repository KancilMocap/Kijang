import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Item {
    width: parent.width
    height: parent.height

    ColumnLayout {
        Text {
            text: qsTr("Processing")
            color: "white"
        }

        Text {
            text: qsTr("Outputs")
            color: "white"
        }

        RowLayout {
            CheckBox {
                id: outputAudio
                text: qsTr("Output audio")
                enabled: false
            }
            CheckBox {
                id: outputVideo
                text: qsTr("Output video")
                enabled: false
            }
            CheckBox {
                id: outputOriginalMotion
                text: qsTr("Output original motion")
                enabled: false
            }
        }

        Text {
            text: qsTr("Units")
            color: "white"
        }
    }
}
