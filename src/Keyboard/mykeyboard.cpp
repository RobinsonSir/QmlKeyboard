#include <QDebug>
#include <QKeyEvent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QCoreApplication>
#include <QGuiApplication>

#include <iostream>
#include <fstream>

#include "mykeyboard.h"

#define INPUT_INTERVAL     1000 //ms
#define CHINESE_DB_PATH    "./chinese.db"
#define CHINESE_SHOW_NUM   12

MyKeyboard::MyKeyboard(QObject *parent) :
    QObject(parent)
{
    m_charIndex = 0;
    m_chineseIndex = 0;
    m_lastChineseIndex = 0;
    m_curInputLanguage = "英";
    m_lastLanguage = "英";
    m_lastKey.clear();
    m_fullKeyboardFlag = false;
    m_lastfullFlag = false;

    QStringList list;

    //create number  keyboard model
    setNumKeyModel(list);

    //create english keyboard model
    setEnglishKeyModel(list);

    //create english keyboard model
    setChineseKeyModel(list);

    //create full keyboard
    setFullKeyboardModel();

    //read chinese db file
    readDBFile(CHINESE_DB_PATH);    

    m_timer.setSingleShot(true);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(resetCharIndex()));
}

MyKeyboard::~MyKeyboard()
{

}

void MyKeyboard::setNumKeyModel(QStringList list)
{
    list.append("");

    for(int i=0; i<10; i++)
        m_numKeyModel.append(QString::number(i));

    m_numKeyModel.append(".");
    m_numKeyModel.append("-");
    m_numKeyModel.append("空格");
    m_numKeyModel.append("<");
    m_numKeyModel.append("返回");
    m_numKeyModel.append("关闭");

}

void MyKeyboard::setEnglishKeyModel(QStringList list)
{
    list.append("no use");
    m_englishKeyHash.insert("英", NULL);
    m_englishKeyHash.insert("小写", NULL);
    m_englishKeyHash.insert("123", NULL);
    m_englishKeyHash.insert("关闭", NULL);
    m_englishKeyHash.insert("空格", NULL);

    m_englishKeyModel.append("英");


    char index = 'a';
    QString tmpStr;

    for(int i=1; i<9; i++)
    {
        tmpStr.clear();

        for(int j=1; j<4; j++)
        {
            tmpStr.append(index);
            index++;
        }

        m_englishKeyHash.insert(tmpStr, tmpStr.data());
        m_englishKeyModel.append(tmpStr);
    }

    tmpStr = "yz";
    m_englishKeyHash.insert(tmpStr, tmpStr.data());
    m_englishKeyModel.append(tmpStr);
    m_englishKeyModel.append("空格");
    m_englishKeyModel.append("<");
    m_englishKeyModel.append("小写");  
    m_englishKeyModel.append("123");   
    m_englishKeyModel.append("简");
    m_englishKeyModel.append("关闭");

}

void MyKeyboard::englishCaseSwitch(QString type)
{
    char index;
    QString tmpStr, lastWord, caseNow;;

    if(type == "小写")
    {
        index = 'A';
        lastWord = "YZ";
        caseNow = "大写";
    }
    else
    {
        index = 'a';
        lastWord = "yz";
        caseNow = "小写";
    }

    m_englishKeyHash.clear();
    m_englishKeyModel.clear();

    m_englishKeyHash.insert("英", NULL);
    m_englishKeyHash.insert(caseNow, NULL);
    m_englishKeyHash.insert("123", NULL);
    m_englishKeyHash.insert("关闭", NULL);
    m_englishKeyHash.insert("空格", NULL);

    m_englishKeyModel.append("英");

    for(int i=1; i<9; i++)
    {
        tmpStr.clear();

        for(int j=1; j<4; j++)
        {
            tmpStr.append(index);
            index++;
        }

        m_englishKeyHash.insert(tmpStr, tmpStr.data());
        m_englishKeyModel.append(tmpStr);
    }

    m_englishKeyHash.insert(lastWord, lastWord.data());
    m_englishKeyModel.append(lastWord);
    m_englishKeyModel.append("空格");
    m_englishKeyModel.append("<");
    m_englishKeyModel.append(caseNow);
    m_englishKeyModel.append("123");
    m_englishKeyModel.append("简");
    m_englishKeyModel.append("关闭");
}

void MyKeyboard::setChineseKeyModel(QStringList list)
{
    list.append("no use");

    m_chineseKeyModel.append("中");

    char index = 'a';
    QString tmpStr;

    for(int i=1; i<9; i++)
    {
        tmpStr.clear();

        for(int j=1; j<4; j++)
        {
            tmpStr.append(index);
            index++;
        }
        m_pinyinKeyHash.insert(tmpStr, tmpStr.data());
        m_chineseKeyModel.append(tmpStr);
    }

    tmpStr = "yz";

    m_pinyinKeyHash.insert(tmpStr, tmpStr.data());
    m_chineseKeyModel.append(tmpStr);
    m_chineseKeyModel.append("空格");
    m_chineseKeyModel.append("<");
    m_chineseKeyModel.append("小写");
    m_chineseKeyModel.append("123");
    m_chineseKeyModel.append("简");
    m_chineseKeyModel.append("关闭");

}

void MyKeyboard::setFullKeyboardModel(QString caseMode)
{
    if(caseMode == "小写" && m_curInputLanguage=="英")
    {
       caseMode = "大写";
    }
    else //if(caseMode == "大写")
       caseMode = "小写";

    m_fullKeyboardModel.clear();
    char index;

    if(m_curInputLanguage=="中" || (m_curInputLanguage=="英" && caseMode=="小写"))
        index = 'a';
    else
        index = 'A';

    for(int i=1; i<=14; i++)
    {
        m_fullKeyboardModel.append(QString(index));
        index++;
    }
    m_fullKeyboardModel.append("全");
    m_fullKeyboardModel.append("<");

    m_fullKeyboardModel.append(m_curInputLanguage);

    for(int i=1; i<=12; i++)
    {
        m_fullKeyboardModel.append(QString(index));
        index++;
    }
    m_fullKeyboardModel.append(caseMode);
    m_fullKeyboardModel.append("123");
    m_fullKeyboardModel.append("关闭");
}

QString MyKeyboard::simulateKeyPressEvent(QString key, QString str)
{
    if(key == "英")
    {
        //switch to chinese keyboard
        m_curInputLanguage = "中";

        if(m_fullKeyboardFlag)
            setFullKeyboardModel();

        emit keyModelChanged();

        return "";
    }

    if(key == "中")
    {
        //switch to english keyboard
        m_curInputLanguage = "英";

        if(m_fullKeyboardFlag)
            setFullKeyboardModel("大写");
        emit keyModelChanged();

        return "";
    }

    if(key == "大写" || key == "小写")
    {
        if(key=="小写" && m_curInputLanguage=="中")
            m_curInputLanguage = "英";

        if(m_fullKeyboardFlag)
            setFullKeyboardModel(key);
        else
            englishCaseSwitch(key);

        emit keyModelChanged();

        return "";
    }

    if(key == "123")
    {
        //switch to number keyboard
        m_lastLanguage = m_curInputLanguage;
        m_lastfullFlag = m_fullKeyboardFlag;
        m_fullKeyboardFlag = false;
        m_curInputLanguage = "数";
        emit keyModelChanged();
        return "";
    }

    if(key == "关闭")
    {
        //close keyboard
        emit closeKeyboard();
        return "";
    }

    if(key == "全")
    {
        //switch to simple keyboard
        m_fullKeyboardFlag = false;
        emit keyModelChanged();
        return "";
    }

    if(key == "简")
    {
        //switch to full keyboard
        m_fullKeyboardFlag = true;
        setFullKeyboardModel();
        emit keyModelChanged();
        return "";
    }

    if(m_curInputLanguage == "中")
    {
        //
        if(m_fullKeyboardFlag)
            str = fullKeyboardPress(key, str);
        else
            str = chineseKeyPress(key, str);
        return str;
    }

    if(m_curInputLanguage == "英")
    {
        //
        if(m_fullKeyboardFlag)
            str = fullKeyboardPress(key, str);
        else
            englishKeyPress(key);
        return "";
    }

    if(m_curInputLanguage == "数")
    {
        //
        numberKeyPress(key);
        return "";
    }

    return "";
    //number keyboard
    //special char keyboard
}

void MyKeyboard::numberKeyPress(QString key)
{
    if(key == "返回")
    {
        m_fullKeyboardFlag = m_lastfullFlag;
        m_curInputLanguage = m_lastLanguage;
        emit keyModelChanged();
        return;
    }

    int keyCode = key.at(0).unicode();

    if(key == "<")
        keyCode = Qt::Key_Backspace;

    if(key == "空格")
    {
        keyCode = Qt::Key_Space;
        key = " ";
    }

    QKeyEvent pressEvent = QKeyEvent(QKeyEvent::KeyPress, keyCode, Qt::NoModifier, key);
    QKeyEvent releaseEvent = QKeyEvent(QKeyEvent::KeyRelease, keyCode, Qt::NoModifier, key);

    QQuickItem *receiver = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

    if(receiver)
    {
        receiver->window()->sendEvent(receiver, &pressEvent);
        receiver->window()->sendEvent(receiver, &releaseEvent);
    }
}

void MyKeyboard::englishKeyPress(QString key)
{
    int keyCode;
    QString sendKey;
    sendKey.clear();

    //delete last char
    if(m_lastKey == key && m_timer.isActive())
    {
        keyCode = Qt::Key_Backspace;

        QKeyEvent pressEvent = QKeyEvent(QKeyEvent::KeyPress, keyCode, Qt::NoModifier, sendKey);
        QKeyEvent releaseEvent = QKeyEvent(QKeyEvent::KeyRelease, keyCode, Qt::NoModifier, sendKey);

        QQuickItem *receiver = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

        if(receiver)
        {
            receiver->window()->sendEvent(receiver, &pressEvent);
            receiver->window()->sendEvent(receiver, &releaseEvent);
        }
    }

    if(key == "<")
        keyCode = Qt::Key_Backspace;
    else if(key == "空格")
    {
        keyCode = Qt::Key_Space;
        sendKey = " ";
    }
    else
    {
        if(m_lastKey.isEmpty())
        {
            m_lastKey = key;
        }

        if(m_lastKey != key)
        {
            m_lastKey = key;
            m_charIndex = 0;
        }

        m_timer.start(INPUT_INTERVAL);

        QChar *charPointer = m_englishKeyHash[key];
        sendKey.append(charPointer[m_charIndex]);
        keyCode = sendKey.at(0).unicode();

        m_charIndex++;

        if((key == "yz" || key == "YZ") && (m_charIndex == 2))
        {
            m_charIndex = 0;
            //m_timer.stop();
        }

        if( m_charIndex == 3)
        {
            m_charIndex = 0;
            //m_timer.stop();
        }

    }

    QKeyEvent pressEvent = QKeyEvent(QKeyEvent::KeyPress, keyCode, Qt::NoModifier, sendKey);
    QKeyEvent releaseEvent = QKeyEvent(QKeyEvent::KeyRelease, keyCode, Qt::NoModifier, sendKey);

    QQuickItem *receiver = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

    if(receiver)
    {
        receiver->window()->sendEvent(receiver, &pressEvent);
        receiver->window()->sendEvent(receiver, &releaseEvent);
    }
}

QString MyKeyboard::chineseKeyPress(QString key, QString str)
{
    int keyCode;
    QString sendKey;
    sendKey.clear();
    m_chineseSelector.clear();

    if(key == "空格")
    {
        keyCode = Qt::Key_Space;
        key = " ";

        QKeyEvent pressEvent = QKeyEvent(QKeyEvent::KeyPress, keyCode, Qt::NoModifier, key);
        QKeyEvent releaseEvent = QKeyEvent(QKeyEvent::KeyRelease, keyCode, Qt::NoModifier, key);

        QQuickItem *receiver = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

        if(receiver)
        {
            receiver->window()->sendEvent(receiver, &pressEvent);
            receiver->window()->sendEvent(receiver, &releaseEvent);
        }
        return "";
    }

    if(key == "<" || m_lastKey == key)
    {
        if(str.isEmpty())
        {
            keyCode = Qt::Key_Backspace;

            QKeyEvent pressEvent = QKeyEvent(QKeyEvent::KeyPress, keyCode, Qt::NoModifier, key);
            QKeyEvent releaseEvent = QKeyEvent(QKeyEvent::KeyRelease, keyCode, Qt::NoModifier, key);

            QQuickItem *receiver = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

            if(receiver)
            {
                receiver->window()->sendEvent(receiver, &pressEvent);
                receiver->window()->sendEvent(receiver, &releaseEvent);
            }
        }
        else
        {
            if(str.length() == 1)
                str.clear();
            else
                str = str.left(str.length()-1);
        }
    }

    if( key != "<")
    {
        if(m_lastKey.isEmpty())
        {
            m_lastKey = key;
        }

        if(m_lastKey != key)
        {
            m_lastKey = key;
            m_charIndex = 0;
        }

        m_timer.start(INPUT_INTERVAL);

        //QChar *charPointer = m_englishKeyHash[key];
        QChar *charPointer = m_pinyinKeyHash[key];
        sendKey.append(charPointer[m_charIndex]);
        keyCode = sendKey.at(0).unicode();

        m_charIndex++;

        if(key == "yz" && m_charIndex == 2)
        {
            m_charIndex = 0;
            //m_timer.stop();
        }

        if( m_charIndex == 3)
        {
            m_charIndex = 0;
            //m_timer.stop();
        }

        str.append(sendKey.at(0));
    }


    QString chineseStr = m_chineseKeyHash[str];
    //qDebug() << "pinyin=" << str << " chinese=" << chineseStr;
    int n = chineseStr.lastIndexOf(" ");
    chineseStr = chineseStr.left(n);
    //qDebug() << "chinese=" << chineseStr;
    m_chineseSelector = chineseStr.split(" ");
    //qDebug() << "last char =" << m_chineseSelector.at(m_chineseSelector.size()-1);
    m_chineseIndex = 0;
    m_lastChineseIndex = 0;

    return str;
}

QString MyKeyboard::fullKeyboardPress(QString key, QString str)
{
    int keyCode;
    m_chineseSelector.clear();

    if(m_curInputLanguage == "中")
    {
        if(key == "<")
        {
            if(str.isEmpty())
            {
                keyCode = Qt::Key_Backspace;

                QKeyEvent pressEvent = QKeyEvent(QKeyEvent::KeyPress, keyCode, Qt::NoModifier, key);
                QKeyEvent releaseEvent = QKeyEvent(QKeyEvent::KeyRelease, keyCode, Qt::NoModifier, key);

                QQuickItem *receiver = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

                if(receiver)
                {
                    receiver->window()->sendEvent(receiver, &pressEvent);
                    receiver->window()->sendEvent(receiver, &releaseEvent);
                }
            }
            else
            {
                if(str.length() == 1)
                    str.clear();
                else
                    str = str.left(str.length()-1);
            }
        }
        else
        {
            str.append(key.at(0));
        }

        QString chineseStr = m_chineseKeyHash[str];
        //qDebug() << "pinyin=" << str << " chinese=" << chineseStr;
        int n = chineseStr.lastIndexOf(" ");
        chineseStr = chineseStr.left(n);
        //qDebug() << "chinese=" << chineseStr;
        m_chineseSelector = chineseStr.split(" ");
        //qDebug() << "last char =" << m_chineseSelector.at(m_chineseSelector.size()-1);
        m_chineseIndex = 0;
        m_lastChineseIndex = 0;

        return str;
    }
    else // if(m_curInputLanguage == "英")
    {
        if(key == "<")
            keyCode = Qt::Key_Backspace;
        else
        {
            keyCode = key.at(0).unicode();
        }

        QKeyEvent pressEvent = QKeyEvent(QKeyEvent::KeyPress, keyCode, Qt::NoModifier, key);
        QKeyEvent releaseEvent = QKeyEvent(QKeyEvent::KeyRelease, keyCode, Qt::NoModifier, key);

        QQuickItem *receiver = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

        if(receiver)
        {
            receiver->window()->sendEvent(receiver, &pressEvent);
            receiver->window()->sendEvent(receiver, &releaseEvent);
        }

        return "";
    }

}

void MyKeyboard::chineseSelector(QString key)
{
    int keyCode = key.at(0).unicode();

    QKeyEvent pressEvent = QKeyEvent(QKeyEvent::KeyPress, keyCode, Qt::NoModifier, key);
    QKeyEvent releaseEvent = QKeyEvent(QKeyEvent::KeyRelease, keyCode, Qt::NoModifier, key);

    QQuickItem *receiver = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

    if(receiver)
    {
        receiver->window()->sendEvent(receiver, &pressEvent);
        receiver->window()->sendEvent(receiver, &releaseEvent);
    }
    m_chineseSelector.clear();
}

void MyKeyboard::resetCharIndex()
{
    m_charIndex = 0;
    m_lastKey = "next";
}


bool MyKeyboard::isInputType()
{
    QQuickItem *focus = qobject_cast<QQuickItem *>(QGuiApplication::focusObject());

    if(focus)
    {
        /*
        if(focus->inherits("QQuickTextInput"))
        {

            return true;
        }
        else
        {

            return false;
        }
        */
        if(focus->inherits("QQuickTextInput"))
        {
            QString objectName(focus->parentItem()->metaObject()->className());
            if(objectName.contains("SpinBox"))
            {
                m_lastLanguage = "英";
                m_curInputLanguage = "数";
                m_lastfullFlag = m_fullKeyboardFlag;
                m_fullKeyboardFlag = false;
                emit keyModelChanged();
            }
            else{
                m_curInputLanguage = "中";

                if(m_fullKeyboardFlag)
                    setFullKeyboardModel();

                emit keyModelChanged();
            }

            return true;
        }

            return false;

    }
    else
        return false;
}

bool MyKeyboard::isChineseInput()
{
    if(m_curInputLanguage == "中")
        return true;
    else
        return false;
}

bool MyKeyboard::isNumberInput()
{
    if(m_curInputLanguage == "数")
        return true;
    else
        return false;
}

QStringList MyKeyboard::getKeyModel()
{
    if(m_fullKeyboardFlag)
        return m_fullKeyboardModel;
    else if(m_curInputLanguage == "英")
        return m_englishKeyModel;
    else if(m_curInputLanguage == "数")
        return m_numKeyModel;
    else //if(m_curInputLanguage == "中")     
        return m_chineseKeyModel;
}

QStringList MyKeyboard::getChineseSelectorModel()
{
    m_lastChineseIndex = m_chineseIndex;
    QStringList showChineseList;
    showChineseList.clear();
    for(int i=0; i<m_chineseSelector.size(); i++)
    {
        if(i == CHINESE_SHOW_NUM )
            break;
        showChineseList.append(m_chineseSelector.at(i));
        m_chineseIndex++;
    }

    //qDebug() << "lastIndex=" << m_lastChineseIndex << " nowIndex=" << m_chineseIndex;
    //qDebug() << "chinese size=" << m_chineseSelector.size();
    if(m_chineseSelector.size() - m_chineseIndex > 0)
        showChineseList.append(">>");

    return showChineseList;
}

QStringList MyKeyboard::turnPageChinese(QString key)
{
    if(key == ">>")
    {
        m_lastChineseIndex = m_chineseIndex;
    }
    else if(key == "<<")
    {
        m_chineseIndex = m_lastChineseIndex - CHINESE_SHOW_NUM;
        m_lastChineseIndex = m_chineseIndex;
    }

    QStringList showChineseList;
    showChineseList.clear();
    for(int i=m_chineseIndex; i<m_chineseSelector.size(); i++)
    {
        if((i - m_lastChineseIndex) == CHINESE_SHOW_NUM )
            break;
        showChineseList.append(m_chineseSelector.at(i));
        m_chineseIndex++;
    }

    //qDebug() << "lastIndex=" << m_lastChineseIndex << " nowIndex=" << m_chineseIndex;
    if(m_lastChineseIndex > 0)
        showChineseList.append("<<");
    if(m_chineseSelector.size() - m_chineseIndex > 0 )
        showChineseList.append(">>");

    return showChineseList;

}

void MyKeyboard::readDBFile(const QString &name)
{
    std::ifstream in(name.toStdString().c_str());
    if(!in)
    {
        qDebug() << "Can't find chinese db file ";
        return;
    }

    std::string line;
    getline(in, line, '\n');

    while(!in.eof())
    {
        std::string strLine(line);

        std::string::size_type index = 0;
        std::string::size_type pos = 0;
        if((index = strLine.find(' ',  pos)) !=  std::string::npos)
        {
            QString key = QString::fromStdString(strLine.substr(pos, index));
            QString val = QString::fromStdString(strLine.substr(index + 1));

            m_chineseKeyHash.insert(key, val);
        }
        getline(in, line, '\n');
    }

    return;
}

QStringList MyKeyboard::getFullKeyboardModel()
{
    return m_fullKeyboardModel;
}

int MyKeyboard::getKeysNumber()
{
/*
    if(m_fullKeyboardFlag)
        return m_fullKeyboardModel.size()/2;
    else if(m_curInputLanguage == "中")
        return m_chineseKeyModel.size();
    else if(m_curInputLanguage == "英")
        return m_englishKeyModel.size();
    else if(m_curInputLanguage == "数")
        return m_numKeyModel.size();
*/
    int maxNum = m_fullKeyboardModel.size()/2;
    if( m_chineseKeyModel.size() > maxNum)
        maxNum = m_chineseKeyModel.size();
    if( m_englishKeyModel.size() > maxNum)
        maxNum = m_englishKeyModel.size();
    if( m_curInputLanguage.size() > maxNum)
        maxNum = m_curInputLanguage.size();

    //qDebug() << "maxNum = " <<maxNum;

    return maxNum;
}
