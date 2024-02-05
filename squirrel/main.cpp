#include <iostream>
#include <cstdarg>

#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>

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

void squirrel_triangle_area(HSQUIRRELVM vm, float base, float height)
{
    SQInteger top = sq_gettop(vm);
    sq_pushroottable(vm);

    sq_pushstring(vm, _SC("triangle_area"), -1);
    if (SQ_SUCCEEDED(sq_get(vm, -2)))
    {
        sq_pushroottable(vm);
        sq_pushfloat(vm, base);
        sq_pushfloat(vm, height);
        sq_call(vm, 3, SQFalse, SQTrue);

        float ret = 0.0;
        if (sq_getfloat(vm, -1, &ret) == SQ_OK) {
            std::cout << "Got value from triangle_area back " << ret << '\n';
        }
    }

    sq_settop(vm, top);
}

void squirrel_log(HSQUIRRELVM vm)
{
	SQInteger top = sq_gettop(vm);
    sq_pushroottable(vm);

    // Here we push the name of the function we want to call, then correctly getting its reference,
    // we can call it passing the appropriate amount of parameters.
    sq_pushstring(vm, _SC("log"), -1);
    if (SQ_SUCCEEDED(sq_get(vm, -2)))
    {
        // root table always accounts as one parameter.
        sq_pushroottable(vm);
        sq_call(vm, 1, SQFalse, SQTrue);
    }

    // We restore the stack back to when we entered this function.
    // Basically we are manipulating the stack ourselves just like in assembly.
    sq_settop(vm, top);
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
    if (SQ_SUCCEEDED(sqstd_dofile(vm, _SC("example.nut"), SQFalse, SQTrue))) // also prints syntax errors if any
    {
        squirrel_log(vm);
        squirrel_triangle_area(vm, 10, 5);
    }

    sq_pop(vm, 1); // pops the root table
    sq_close(vm);

    return 0;
}