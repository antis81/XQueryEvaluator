#ifndef ABSTRACTWIDGETGENERATOR_H
#define ABSTRACTWIDGETGENERATOR_H

//#include <QtGui/QWidget>
class QWidget;


/**
@brief
Abstract widget generator to dynamically register widget types.
*/
class AbstractWidgetGenerator
{
public:
    explicit AbstractWidgetGenerator() {}
    virtual ~AbstractWidgetGenerator() {}

    virtual QWidget * newInstance() const = 0;
};

/**
@brief
Generator class template to create a new widget instance.
*/
template<typename T>
class WidgetGeneratorTemplate : public AbstractWidgetGenerator
{
public:
    QWidget * newInstance() const { return new T(); }
};


#endif // ABSTRACTWIDGETGENERATOR_H
