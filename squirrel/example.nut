function log()
{
	::print("Squirrel: Called squirrel function 'log' at file: "+__FILE__+"\n");
}

function triangle_area(b, h)
{
    ::print("Squirrel: Called triangle_area(), base="+b+", height="+h+"\n");
    return 0.5 * b * h;
}