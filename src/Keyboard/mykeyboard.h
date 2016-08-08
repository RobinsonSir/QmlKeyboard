#ifndef MYKEYBOARD_H
#define MYKEYBOARD_H

#include <QChar>
#include <QHash>
#include <QTimer>
#include <QObject>
#include <QStringList>


class MyKeyboard : public QObject
{
    Q_OBJECT
public:
    explicit MyKeyboard(QObject *parent = 0);
    ~MyKeyboard();

    Q_PROPERTY(QStringList numKeyModel READ getNumKeyModel WRITE setNumKeyModel NOTIFY numKeyModelChanged )
    Q_PROPERTY(QStringList englishKeyModel READ getEnglishKeyModel WRITE setEnglishKeyModel NOTIFY englishKeyModelChanged)
    Q_PROPERTY(QStringList chineseKeyModel READ getChineseKeyModel WRITE setChineseKeyModel NOTIFY chineseKeyModelChanged)

    Q_PROPERTY(QStringList keyModel READ getKeyModel WRITE setKeyModel NOTIFY keyModelChanged)

    Q_INVOKABLE bool isChineseInput();
    Q_INVOKABLE bool isNumberInput();

    Q_INVOKABLE QStringList getChineseSelectorModel();
    Q_INVOKABLE void chineseSelector(QString key);
    Q_INVOKABLE QStringList turnPageChinese(QString key);

    Q_INVOKABLE bool isInputType();
    Q_INVOKABLE QStringList getKeyModel();
    Q_INVOKABLE QString simulateKeyPressEvent(QString key, QString str);

    Q_INVOKABLE int getKeysNumber();

    Q_INVOKABLE QStringList getFullKeyboardModel();

    Q_INVOKABLE inline bool getFullKeyboardFlag()
    {
        return m_fullKeyboardFlag;
    }

    Q_INVOKABLE inline void setFullKeyboardFlag(bool f)
    {
        m_fullKeyboardFlag = f;
    }

    inline QStringList getNumKeyModel()
    {
        return m_numKeyModel;
    }

    inline QStringList getEnglishKeyModel()
    {
        return m_englishKeyModel;
    } 

    inline QStringList getChineseKeyModel()
    {
        return m_chineseKeyModel;
    }

    inline void setKeyModel(QStringList lan)
    {
        m_curInputLanguage = lan.at(0);
    }

    void setNumKeyModel(QStringList list);
    void setEnglishKeyModel(QStringList list);
    void setChineseKeyModel(QStringList list);
    void setFullKeyboardModel(QString caseMode="小写");

    void englishCaseSwitch(QString type);

signals:
    void chineseKeyModelChanged();
    void englishKeyModelChanged();
    void numKeyModelChanged();
    void keyModelChanged();
    void closeKeyboard();

private slots:
    void resetCharIndex();

private:
    void englishKeyPress(QString key);
    void numberKeyPress(QString key);
    QString chineseKeyPress(QString key, QString str);

    QString fullKeyboardPress(QString key, QString str);

    void readDBFile(const QString &name);

    QHash<QString, QChar*> m_englishKeyHash;
    QHash<QString, QString> m_chineseKeyHash;
    QHash<QString, QChar*> m_pinyinKeyHash;

    QStringList m_chineseKeyModel;
    QStringList m_englishKeyModel;
    QStringList m_numKeyModel;

    QStringList m_fullKeyboardModel;

    bool m_fullKeyboardFlag;
    bool m_lastfullFlag;

    QStringList m_chineseSelector;

    QTimer m_timer;
    int m_charIndex;
    QString m_lastKey;
    int m_lastChineseIndex;
    int m_chineseIndex;

    QString m_curInputLanguage;
    QString m_lastLanguage;
};

#endif // MYKEYBOARD_H
