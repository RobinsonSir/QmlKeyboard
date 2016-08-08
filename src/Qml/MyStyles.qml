import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    property Component textFieldStyle: tFieldStyle
    property Component tabViewStyle: tViewStyle
    property Component toolButtonStyle: toolBtnStyle
    property Component iconButtonStyle: iconBtnStyle

    property Component switchStyle: swStyle

    //Tool Button Style
    Component {
        id: toolBtnStyle
        ButtonStyle {
            background: Rectangle {
                implicitHeight: 36
                implicitWidth: 80
                color: (control.pressed || control.checked || !enabled) ? "gray" : "black"
                border.color: ((control.hovered || control.checked) && enabled) ? "white" : "gray"
                radius: 5
            }

            label: Item {
                Text {
                    text: control.text
                    color: enabled ? "white" : "lightgray"
                    anchors.centerIn: parent
                }
            }
        }
    }

    // Only icon
    Component {
        id: iconBtnStyle
        ButtonStyle {
            background: Rectangle {
                implicitHeight: 32
                implicitWidth: 32
                color: "transparent"
            }
            label: Item {
                anchors.fill: parent
                Image {
                    source: control.iconSource
                    anchors.fill: parent
                }
            }
        }
    }

    //Text Field Style
    Component {
        id: tFieldStyle
        TextFieldStyle {
            textColor: "#e9e9e9"
            background: Rectangle {
                implicitHeight: 30
                implicitWidth: 200
                color: "#121212"
                radius: 4
                anchors.fill: parent
            }
        }
    }

    //Switch Style
    Component {
        id: swStyle
        SwitchStyle {
            groove: Rectangle {
                implicitHeight: 40
                implicitWidth: 152
                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: parent.width/2 - 2
                    height: 20
                    anchors.margins: 2
                    color: control.checked ? "#f1592a" : "#111"
                    Behavior on color {ColorAnimation {}}
                    Text {
                        font.pixelSize: 23
                        color: "white"
                        anchors.centerIn: parent
                        text: "ON"
                    }
                }
                Item {
                    width: parent.width/2
                    height: parent.height
                    anchors.right: parent.right
                    Text {
                        font.pixelSize: 23
                        color: "white"
                        anchors.centerIn: parent
                        text: "OFF"
                    }
                }
                color: "#111"
                border.color: "#333"
                border.width: 2
            }
            handle: Rectangle {
                width: parent.parent.width/2
                height: parent.parent.height
                color: "#222"
                border.color: "#333"
                border.width: 2
            }
        }
    }

    Component {
        id: tViewStyle
        TabViewStyle {
            tabsAlignment: Qt.AlignVCenter
            tabOverlap: 0
            frame: Item { }
            tab: Item {
                visible: styleData.enabled
                implicitWidth: 100//control.width/control.count
                implicitHeight: 50
                BorderImage {
                    anchors.fill: parent
                    border.bottom: 8
                    border.top: 8
                    source: styleData.selected ? "../images/tab_selected.png" : "../images/tabs_standard.png"
                    Column {
                        anchors.centerIn: parent
                        spacing: 1
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            color: styleData.selected ? "#f1592a" : "white"
                            text: styleData.title.toUpperCase()
                            font.pixelSize: 18
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: myTabView.getTab(index).subTitle 
                            color: styleData.selected ? "#ad4c2d" : "white"
                            font.pixelSize: 10
                        }
                    }
                    Rectangle {
                        visible: index > 0
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        //anchors.right: parent.right
                        anchors.margins: 10
                        //anchors.rightMargin: 0 
                        width:1
                        color: "#4a4a4a"
                    }
                }
            }
        }
    }
}
