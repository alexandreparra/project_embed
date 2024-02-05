function log()
{
	::print("Called squirrel function 'log' at file: "+__FILE__+"\n");
}

function triangle_area(b, h)
{
    ::print("Called triangle_area(), base="+b+", height="+h+"\n");
    return 0.5 * b * h;
}