#ifndef NAMESDIALOG_H
#define NAMESDIALOG_H

#include <QDialog>
class QLineEdit;
class FantasyButton;
class FantasyLabel;

class NamesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NamesDialog(QWidget *parent = nullptr);
    QString getUsername1() const;
    QString getUsername2() const;

signals:
    void namesSubmitted(const QString& name1, const QString& name2);

private:
    QLineEdit *username1LineEdit;
    QLineEdit *username2LineEdit;
};

#endif // NAMESDIALOG_H
