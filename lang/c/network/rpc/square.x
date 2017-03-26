struct square_in {
    long    arg1;
};

struct square_out {
    long    res1;
};

typedef int (*call_back)(long input);

program SQUARE_PROG {
    version SQUARE_VERS {
        square_out SQUAREPROC(square_in, call_back) = 1;
    } = 1;
} = 0x31230000;
