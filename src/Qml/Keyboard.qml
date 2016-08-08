import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import QtQml 2.2

Item{
    id:keyboard

    width:980
    height:35
    focus:false

    GridView{
        id: listModel
        anchors.fill: parent
        //orientation: ListView.Horizontal
        //spacing: 5

        cellHeight: 35
        cellWidth: 940/myKeyboard.getKeysNumber()
        model: myKeyboard.getKeyModel()
        delegate: Button {
            //width: myKeyboard.isNumberInput() ? 900/15 : 900/13
            width: 880/myKeyboard.getKeysNumber()
            height:35
            style: myStyles.toolButtonStyle
            //checked: (myKeyboard.isChineseInput() && modelData=="小写") ? true:false
            text: modelData
            onClicked:{
                var str = pinyin.text;             

                if(!myKeyboard.isChineseInput())
                    myKeyboard.simulateKeyPressEvent(modelData, str);
                else{
                    //if(modelData!="小写"){
                        str = myKeyboard.simulateKeyPressEvent(modelData, str);
                        pinyin.text = str;
                    //}
                }

            }
        }

        Component.onCompleted: {
            myKeyboard.setFullKeyboardFlag(true);
        }

    }

    RowLayout {
        id: chineseRow
        visible: false
        anchors.bottom: listModel.top
        anchors.bottomMargin: 2
        width: parent.width
        height: parent.height

        Rectangle {
            id: pinyinRec
            width: 80
            height: 35
            visible: true
            color: "black"
            border.color: "gray"
            radius: 5
            anchors.left: parent.left
            anchors.leftMargin: 0
            Text {
                id: pinyin
                text: ""
                color: "white"
                anchors.centerIn: parent
                //horizontalAlignment: Text.AlignHCenter
                //verticalAlignment: Text.AlignVCenter
                onTextChanged: chineseSelector.model = myKeyboard.getChineseSelectorModel()
            }
        }

        ListView{
            id: chineseSelector
            width: 870
            height: 35
            anchors.left: pinyinRec.right
            anchors.leftMargin: 5
            orientation: ListView.Horizontal
            spacing: 5
            model: myKeyboard.getChineseSelectorModel()
            delegate: Button {
                width: 800/14
                height:35
                style: myStyles.toolButtonStyle
                text: modelData
                onClicked:{

                    if( modelData != "<<" && modelData != ">>"){
                        myKeyboard.chineseSelector(modelData);
                        pinyin.text = "";
                    }
                    else
                        chineseSelector.model = myKeyboard.turnPageChinese(modelData);
                }
            }

        }

    }


    Connections{
        target: myKeyboard
        onKeyModelChanged: {

            if(true){
                listModel.anchors.bottomMargin = -15;
                chineseRow.anchors.bottomMargin = -15;
                keyboard.height = 70;
            }
            else{
                listModel.anchors.bottomMargin = 0;
                chineseRow.anchors.bottomMargin = 0;
                keyboard.height = 35;
            }

            if(myKeyboard.isChineseInput())
                chineseRow.visible = true;
            else
                chineseRow.visible = false;

            listModel.model = myKeyboard.getKeyModel();
            //console.log("KeysNum = "+myKeyboard.getKeysNumber());
        }
    }

    //Behavior on x { SmoothedAnimation { velocity: 1300 } }
    //Behavior on y { SmoothedAnimation { velocity: 10} }
}

