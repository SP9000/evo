--simple library for basic 3d shapes
--
--The general form of library functions is:
--  function Name(model, arg1, arg2, ...)
--   where model is the model being created (returned by ModelStart()),
--   and arg1, arg2, etc. are any arguments that determine the output.
--

--radius: radius of the circle
--precision: number of vertices
function Circle(model, radius, precision)
    x2 = radius * cos(0.0)
    y2 = radius * sin(0.0)

    for i = 0, precision-1, 1 do
        --set x1 and y1 to the previous vertex coordinates
        x1 = x2
        y2 = y2
        --set x2 and y2 to the next vertex coordinates 
        x2 = radius * cos((i / precision) * PI);
        y2 = radius * sin((i / precision) * PI);
        --set x3 and y3 to the origin (0,0)
        x3 = 0.0
        y3 = 0.0
        ModelTriangle2D(model, x1,y1, x2,y2, x3,y3)
    end
end
