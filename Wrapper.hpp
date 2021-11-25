#pragma once
#include <vector>
#include <exception>
#include <functional>

/**
 * \brief Type to represent class method pointer, that returns int
 */
template<typename Object, typename ...FuncArgs>
using Method = int(Object::* )(FuncArgs...);

/**
 * \brief Data type of argument with name and integer value
 * \see Wrapper, Engine
 */
struct Argument
{
    std::string name;
    int val;
};

/**
 * \brief Interface class for template based wrapper implimentations
 * \see Wrapper, Engine, TemplateWrapper
 */
class WrapperImpl
{
protected:
    WrapperImpl() {};
public:
    /**
     * \brief Execute wraped function with given argument list
     * \param[in] argList - list of arguments in any order
     * \return int - evaluated function call result
     */
    virtual int execute(const std::vector<Argument>& argList) = 0;
    virtual ~WrapperImpl() {};
};

/**
 * \brief Template based wrapper implimentation class
 * \see Wrapper, Engine, WrapperImpl
 */
template<typename Object, typename ...FuncArgs>
class TemplateWrapper : public WrapperImpl
{
private:
    /**
     * \brief List of method arguments in correct order. 
     * 'val' filds of arguments are used for temporary storage of values in correct order on execute
     */
    std::vector<Argument> argList;

    /**
     * \brief Object to execute method from
     */
    Object* object;

    /**
     * \brief Method to execute
     */
    Method<Object, FuncArgs...> method;

    /**
     * \brief Temporary store value in correct order, using 'argList'
     * \param[in] arg - argument with new value
     */
    void addValToArgList(const Argument& arg)
    {
        for (auto& argument : argList)
        {
            if (argument.name == arg.name)
            {
                argument.val = arg.val;
                return;
            }
        }
        throw std::exception("Unknown command argument.");
    }

    /**
     * \brief Checks if arguments list has duplicate arguments
     * \param[in] argList - list of arguments in any order
     */
    static bool HasDuplicatelArguments(const std::vector<Argument>& argList)
    {
        for (int i = 0; i < sizeof...(FuncArgs) - 1; i++)
        {
            for (int j = i + 1; j < sizeof...(FuncArgs); j++)
                if (argList[i].name == argList[j].name)
                    return true;
        }
        return false;
    }

    /**
     * \brief Calls method from object, using variaduc templates
     * \param[in] sec - index sequence object to get variadic sequence of 'argList' indexes
     * \return int - evaluated method call result
     */
    template<size_t ...Indexes>
    int callMethodFromVariadic(std::index_sequence<Indexes...> sec)
    {
        return (object->*method)(argList[Indexes].val...);
    }


    /**
     * \brief TemplateWrapper constructor. Assumes passed parameters are correct.
     * \param[in] object - object to execute method from
     * \param[in] method - method to execute
     * \param[in] argList - list of arguments in correct order
     */
    TemplateWrapper(Object* object, Method<Object, FuncArgs...> method, const std::vector<Argument>& argList) : argList(argList)
    {
        _ASSERT(this->argList.size() == sizeof...(FuncArgs));
        this->object = object;
        this->method = method;
    }
public:

    /**
     * \brief Safe TemplateWrapper construction. Checks if parameters are correct.
     * \param[in] object - object to execute method from
     * \param[in] method - method to execute
     * \param[in] argList - list of arguments in correct order
     * \return TemplateWrapper* - pointer to new TemplateWrapper object, or nullptr if some parameters are incorrect
     */
    static TemplateWrapper* get(Object* object, Method<Object, FuncArgs...> method, const std::vector<Argument>& argList)
    {
        if (object == nullptr || method == nullptr)
            return nullptr;
        if (argList.size() != sizeof...(FuncArgs))
            return nullptr;
        if (HasDuplicatelArguments(argList))
            return nullptr;
        return new TemplateWrapper(object, method, argList);
    }

    /**
     * \brief Execute wraped function with given argument list
     * \param[in] argList - list of arguments in any order
     * \return int - evaluated method call result
     */
    int execute(const std::vector<Argument>& argList) override
    {
        if (argList.size() != sizeof...(FuncArgs))
            throw std::exception("Command call contains incorrect arguments count.");
        if (HasDuplicatelArguments(argList))
            throw std::exception("Command call contains multiple argument with same name.");
        for (auto& arg : argList)
        {
            addValToArgList(arg);
        }
        return callMethodFromVariadic(std::make_index_sequence<sizeof...(FuncArgs)>{});
    }
};

class Wrapper
{
private:
    /**
     * \brief Pointer to implementation of this wrapper
     */
    WrapperImpl* wrapperPImpl;
public:
    Wrapper()
    {
        wrapperPImpl = nullptr;
    }

    /**
     * \brief Wrapper construction. Checks if parameters are correct. If can't construct, 'isValid()' will return false
     * \param[in] object - object to execute method from
     * \param[in] method - method to execute
     * \param[in] argList - list of arguments in correct order
     */
    template<typename Object, typename ...FuncArgs>
    Wrapper(Object* object, Method<Object, FuncArgs...> method, const std::vector<Argument>& argList)
    {
        wrapperPImpl = TemplateWrapper<Object, FuncArgs...>::get(object, method, argList);
    }

    /**
     * \brief Is construction of this object was successful
     */
    bool isValid()
    {
        return wrapperPImpl != nullptr;
    }

    /**
     * \brief Execute wraped function with given argument list
     * \param[in] argList - list of arguments in any order
     * \return int - evaluated method call result
     */
    int execute(const std::vector<Argument>& argList)
    {
        return wrapperPImpl->execute(argList);
    }

    ~Wrapper()
    {
        delete wrapperPImpl;
    }
};