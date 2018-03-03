program {
    int i, j, m = 40, n = 138;
    real x,y;
    real a = -2., b = 1.3, c = -1., d = 1.;
    real h1, h2, c1, c2, x1, y1;
    h1 = (b-a)/n;
    h2 = (d-c)/m;
    for (c2 = c; c2 < d; c2 = c2 + h2) {
        for (c1 = a; c1 < b; c1 = c1 + h1) {
            x = 0.;
            y = 0.;
            for (i = 0; i < 100; i=i+1) {
                x1 = x*x - y*y + c1;
                y1 = 2.*x*y + c2;
                x = x1;
                y = y1;
            }
            if (x*x + y*y < 4.) {
                write("#");
            } else {
                write(" ");
            }
        }
        writeln();
    }
}