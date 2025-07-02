#include "splash.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    splash s;
    s.show();
    return a.exec();
	//end
}
