import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Layouts 1.1

import QtQml 2.2

import mykeyboard 1.0

import "Qml"

ApplicationWindow {
    id: mainWindow
    visible: true

    width: 1000
    height: 400

    //visibility: "FullScreen"

    //keyboard
    MyKeyboard{
        id:myKeyboard
    }

    //Load styles
    MyStyles {
        id: myStyles
    }

    Rectangle{
        id: background
        width: mainWindow.width
        height: 400
        anchors {horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter}
        //color: "#3f4649"
        color: "#e0e0e0"

        RowLayout{
            anchors {horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter}
            Text{
                width: 100
                height: 100
                text: "请输入："
            }

            TextField{
                width: 100
                height: 100

            }

            SpinBox{

            }
        }
    }

    Rectangle{
        id: keyboard
        visible: false
        width: mainWindow.width - 10
        height: 50
        radius: 8
        anchors {
            horizontalCenter: parent.horizontalCenter;
            bottom: parent.bottom;
            bottomMargin: 5
        }

        color: "#00000000"

        Keyboard{
            id: key
            //anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenter: parent.verticalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 25
        }

    }

    onActiveFocusItemChanged: {
        console.log("isInputType = " + myKeyboard.isInputType());

        if(myKeyboard.isInputType())
        {
            keyboard.visible = true;
            /*if(keyboard.x == -1200)
            {
                keyboard.x = keyboard.x + 1200;
                //keyboard.y = keyboard.y + 50;
            }
            */
        }
        else
        {
            /*if(keyboard.x == 0)
            {
                keyboard.x = keyboard.x - 1200;
                //keyboard.y = keyboard.y - 50;
            }
            */
            keyboard.visible = false;
        }
    }

    Connections{
        target: myKeyboard
        onCloseKeyboard: keyboard.visible = false
    }

}
