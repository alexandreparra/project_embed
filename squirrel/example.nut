// In this script will be mixing Squirrel functions and native functions.

function log()
{
	::print("Squirrel: Called function 'log' at file: "+__FILE__+"\n");
    log_cpp();
    log_cpp_arg("called from a .nut file!");
}

function triangle_area(b, h)
{
    ::print("Squirrel: Called function 'triangle_area' with args, base="+ b +", height="+ h +"\n");
    local result = calculate_triangle_area(b, h);
    return result;
}
