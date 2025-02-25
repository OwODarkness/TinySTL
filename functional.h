#ifndef TINYSTL_FUNCTIONAL_H
#define TINYSTL_FUNCTIONAL_H

#include <memory>
namespace tinystl{
    template<typename T>
    class function;

    template<typename ReturnType, typename... VARS>
    class function<ReturnType(VARS...)>{
    public:
        function(ReturnType (*func)(VARS... )):
        callable(std::make_unique<callable_impl<ReturnType (*)(VARS...)>>(func))
        {
        }

        ReturnType operator()(VARS ... vars)
        {
            return callable->call(vars...);
        }

    private:

        struct callable_interface{
            virtual ReturnType call(VARS...) = 0;
            virtual ~callable_interface() = default;
        };

        template<typename Callable>
        struct callable_impl: public callable_interface{
            callable_impl( Callable callable):
            callable_(std::move(callable))
            {
            
            }
            ReturnType call(VARS... vars)
            {
                return callable_(vars...);
            }
            Callable callable_;
        };

        std::unique_ptr<callable_interface> callable;
    }; 

    template<typename T>
    class function_v2;

    template<typename ReturnType, typename...VARS>
    class function_v2<ReturnType(VARS...)>{
    public:
        function_v2(ReturnType (*func)(VARS...)):
        func_(func)
        {

        }

        ReturnType operator()(VARS... vars)
        {
           return func_(vars...);
        }

    private:
        ReturnType (*func_)(VARS...);
    };
};

#endif // TINYSTL_FUNCTIONAL_H#