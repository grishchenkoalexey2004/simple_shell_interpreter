
int main() {
    typedef enum { Start, Word, Greater, Greater2, Newline, Stop } vertex;
    vertex V = Start;
    c = getchar();
    null_list();
    while (1)
        switch (V) {
        case Start:
            if (c == ' ' || c == '\t')
                c = getchar();
            else if (c == EOF) {
                termlist();
                printlist();
                clearlist();
                V = Stop;
            } else if (c == '\n') {
                termlist();
                printlist();
                V = Newline;
                c = getchar();
            } else {
                nullbuf();
                addsym();
                V = (c == '>') ? Greater : Word;
                c = getchar();
            }
            break;
        case Word:
            if (symset(c)) {
                addsym();
                c = getchar();
            } else {
                V = Start;
                addword();
            }
            break;
        case Greater:
            if (c == '>') {
                addsym();
                c = getchar();
                V = Greater2;
            } else {
                V = Start;
                addword();
            }
            break;
        case Greater2:
            V = Start;
            addword();
            break;
        case Newline:
            clearlist();
            V = Start;
            break;
        case Stop:
            exit(0);
            break;
        }
}
