#ifndef ABSTRACTWIDGETGENERATOR_H
#define ABSTRACTWIDGETGENERATOR_H

class AbstractWidgetGenerator
{
public:
    virtual QWidget * generate( QString className ) const = 0;
};

#endif // ABSTRACTWIDGETGENERATOR_H
