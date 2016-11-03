#ifndef UNIVERSAL_FUNCTIONS_H
#define UNIVERSAL_FUNCTIONS_H

#include <QString>

QString randomColorGenerator()
{
    QString color("#");

    int ran;
    char cran;
    for(int i = 0; i < 3; ++i)
    {
        ran = rand()%15;
        cran = (ran <= 9) ? '0' + ran : 'a' + ran - 10;
        color.append(cran);
        color.append(cran);
    }
    return color;
}

#endif // UNIVERSAL_FUNCTIONS_H
