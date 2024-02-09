#include <iostream>
#include <cstdarg>

#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>

// -------------------------------------
// Print functions

void squirrel_print_error(HSQUIRRELVM vm, const SQChar* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    vfprintf(stdout, fmt, vl);
    va_end(vl);
}

void squirrel_print(HSQUIRRELVM vm, const SQChar* fmt, ...)
{
    va_list vl;
    va_start(vl, fmt);
    vfprintf(stderr, fmt, vl);
    va_end(vl);
}

// -------------------------------------
// Squirrel functions

// An example function call with 2 parameters and a return value.
void squirrel_triangle_area(HSQUIRRELVM vm, float base, float height)
{
    SQInteger top = sq_gettop(vm);
    sq_pushroottable(vm);

    sq_pushstring(vm, _SC("triangle_area"), -1);
    if (SQ_SUCCEEDED(sq_get(vm, -2)))
    {
        // We pass the arguments in, first the root table.
        // Then we pass the arguments with the same sequence
        // from what we defined in Squirrel.
        sq_pushroottable(vm);
        sq_pushfloat(vm, base);
        sq_pushfloat(vm, height);
        // Pass SQTrue as the third argument so that the returned value from
        // the Squirrel function is pushed to the top of the stack.
        sq_call(vm, 3, SQTrue, SQTrue);

        // Retrieve the returned value from the top of the stack.
        float ret = 0.0;
        if (SQ_SUCCEEDED(sq_getfloat(vm, -1, &ret))) {
            std::cout << "C++: Got value from triangle_area back: " << ret << '\n';
        }
    }

    sq_settop(vm, top);
}

// An example function call with no arguments and no return value.
void squirrel_log(HSQUIRRELVM vm)
{
    // Save the top of the stack to latter reset its state.
	SQInteger top = sq_gettop(vm);
    sq_pushroottable(vm);

    // Here we push the name of the function we want to call, then get its reference
    // from the root table (that's why we use sq_get with -2 index).
    sq_pushstring(vm, _SC("log"), -1);
    if (SQ_SUCCEEDED(sq_get(vm, -2)))
    {
        // Now we can pass in the arguments, in this case, the Squirrel
        // function doesn't have any beside the root table.
        // root table always accounts as one argument.
        sq_pushroottable(vm);
        sq_call(vm, 1, SQFalse, SQTrue);
    }

    // We restore the stack back to when we entered this function.
    // Basically we are manipulating the stack ourselves just like in assembly.
    sq_settop(vm, top);
}

// -------------------------------------
// Native functions

// Native function with 2 arguments and a return value.
SQInteger calculate_triangle_area(HSQUIRRELVM vm)
{
    float base, height;

    SQInteger arg_number = sq_gettop(vm);

    // Here index 3 and 2 are our float parameters height and base.
    if(sq_gettype(vm, arg_number) == OT_FLOAT && sq_gettype(vm, arg_number - 1) == OT_FLOAT) {
        sq_getfloat(vm, arg_number, &height);
        sq_getfloat(vm, arg_number - 1, &base);

        float area = 0.5f * base * height;

        // Push the value to the stack so that Squirrel can access it and return 1
        // to indicate that we are indeed returning a value.
        sq_pushfloat(vm, area);
        return 1;
    }

    // If the number of arguments are wrong, indicate that we return no value.
    return 0;
}

// Native function with 1 argument and no return value.
SQInteger log_cpp_arg(HSQUIRRELVM vm)
{
    std::cout << "C++: Called native function 'log_cpp_arg' from Squirrel!\n";

    // We expect only one argument from the Squirrel script, a string.
    SQInteger arg_number = sq_gettop(vm);

    // Our parameters are at the top of the stack, at index 1 is the root table which
    // we don't care about, index 2 is our first and only parameter.
    if (sq_gettype(vm, arg_number) == OT_STRING) {
        const char *log_value;
        sq_getstring(vm, arg_number, &log_value);
        std::cout << "C++: Log value from Squirrel: " << log_value << '\n';
    }

    return 0;
}

// Native function with no arguments and no return value.
SQInteger log_cpp(HSQUIRRELVM vm)
{
    std::cout << "C++: Called native function 'log_cpp' from Squirrel!\n";
    return 0;
}

// Utility function
void register_native_function(HSQUIRRELVM vm, const char *function_name, SQFUNCTION fn)
{
    sq_pushroottable(vm);
    sq_pushstring(vm, function_name, -1);
    sq_newclosure(vm, fn, 0);
    sq_newslot(vm, -3, SQFalse);
    sq_pop(vm, 1);
}

int main()
{
    // Initialize the VM with an initial size.
    HSQUIRRELVM vm = sq_open(1024);

    // This sqstd function is a utility to print errors from the compiler/runtime.
    sqstd_seterrorhandlers(vm);

    // Squirrel uses callbacks so that it can print to stdout, we provide both
    // a normal print function and an error function. Differently from above, this is meant
    // to be used by Squirrel print functions.
    sq_setprintfunc(vm, squirrel_print, squirrel_print_error);
    sq_pushroottable(vm);

    // Here we are defining a native functions which are C/C++ function that can
    // be called from inside Squirrel. Look at the conventions of a native function
    // at: http://squirrel-lang.org/squirreldoc/reference/embedding/creating_a_c_function.html
    register_native_function(vm, "log_cpp", &log_cpp);
    register_native_function(vm, "log_cpp_arg", &log_cpp_arg);
    register_native_function(vm, "calculate_triangle_area", &calculate_triangle_area);

    // sqstd_dofile is a utility function from the Squirrel standard library that helps us compile
    // our .nut files into bytecode. If you want to do it by hand, you can follow the example from the
    // Squirrel reference: http://squirrel-lang.org/squirreldoc/reference/embedding/compiling_a_script.html
    if (SQ_SUCCEEDED(sqstd_dofile(vm, _SC("example.nut"), SQFalse, SQTrue)))
    {
        squirrel_log(vm);
        squirrel_triangle_area(vm, 10, 5);
    }

    // Cleanup
    sq_pop(vm, 1); // pops the root table
    sq_close(vm);

    return 0;
}