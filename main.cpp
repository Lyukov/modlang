#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

bool debug = false;

class String
{
    char* body;
    int len;
    int capacity;

   public:
    explicit String( int cap = 20 ) : capacity( cap )
    {
        len = 0;
        body = new char[capacity];
        body[0] = '\0';
    }
    String( const char* c_str );
    String( const String& str );
    ~String() { delete[] body; }
    void clear()
    {
        len = 0;
        body[0] = '\0';
    }
    int length() const { return len; }
    bool empty() const { return len == 0; }
    void push_back( const char c );
    String operator+( const String& str ) const;
    char& operator[]( const int i );

    bool operator==( const String& str ) const;
    bool operator!=( const String& str ) const;
    bool operator<( const String& str ) const;
    bool operator>( const String& str ) const;
    bool operator>=( const String& str ) const;
    bool operator<=( const String& str ) const;

    String& operator=( const String& str );
    friend ostream& operator<<( ostream& strm, const String& s );
    friend istream& operator>>( istream& strm, String& s );
};

String::String( const char* c_str )
{
    if( !c_str )
    {
        len = 0;
        body = new char[capacity = 20];
        body[0] = '\0';
        return;
    }
    len = strlen( c_str );
    capacity = len + 1;
    body = new char[capacity];
    strcpy( body, c_str );
}

String::String( const String& str )
{
    capacity = str.capacity;
    len = str.len;
    body = new char[capacity];
    strcpy( body, str.body );
}

void String::push_back( const char c )
{
    if( len + 1 == capacity )
    {
        char* old = body;
        body = new char[capacity *= 2];
        strcpy( body, old );
        delete[] old;
    }
    body[len] = c;
    body[++len] = '\0';
}

String String::operator+( const String& str ) const
{
    String new_s( capacity + str.capacity - 1 );
    new_s.len = len + str.len;
    strcpy( new_s.body, body );
    strcpy( new_s.body + len, str.body );
    return new_s;
}

char& String::operator[]( const int i )
{
    if( i < 0 || i >= capacity )
    {
        throw "String: out of range";
    }
    return body[i];
}

bool String::operator==( const String& str ) const { return strcmp( body, str.body ) == 0; }
bool String::operator!=( const String& str ) const { return strcmp( body, str.body ) != 0; }
bool String::operator<( const String& str ) const { return strcmp( body, str.body ) == -1; }
bool String::operator>( const String& str ) const { return strcmp( body, str.body ) == 1; }
bool String::operator>=( const String& str ) const { return strcmp( body, str.body ) >= 0; }
bool String::operator<=( const String& str ) const { return strcmp( body, str.body ) <= 0; }
String& String::operator=( const String& str )
{
    capacity = str.capacity;
    len = str.len;
    delete[] body;
    body = new char[capacity];
    strcpy( body, str.body );
    return *this;
}

ostream& operator<<( ostream& strm, const String& s )
{
    strm << s.body;
    return strm;
}
istream& operator>>( istream& strm, String& s )
{
    char buf[256];
    strm >> buf;
    s = String( buf );
    return strm;
}

enum type_of_lex
{
    LEX_NULL,       // 0
    LEX_AND,        // 1
    LEX_BOOL,       // 2
    LEX_BREAK,      // 3
    LEX_CASE,       // 4
    LEX_CONTINUE,   // 5
    LEX_DO,         // 6
    LEX_ELSE,       // 7
    LEX_END,        // 8
    LEX_IF,         // 9
    LEX_FALSE,      // 10
    LEX_FOR,        // 11
    LEX_GOTO,       // 12
    LEX_INT,        // 13
    LEX_NOT,        // 14
    LEX_OF,         // 15
    LEX_OR,         // 16
    LEX_PROGRAM,    // 17
    LEX_READ,       // 18
    LEX_REAL,       // 19
    LEX_STEP,       // 20
    LEX_STRING,     // 21
    LEX_TRUE,       // 22
    LEX_UNTIL,      // 23
    LEX_WHILE,      // 24
    LEX_WRITE,      // 25
    LEX_WRITELN,    // 26
    LEX_FIN,        // 27
    LEX_SEMICOLON,  // 28
    LEX_COMMA,      // 29
    LEX_COLON,      // 30
    LEX_ASSIGN,     // 31
    LEX_LBRACK,     // 32
    LEX_RBRACK,     // 33
    LEX_LPAREN,     // 34
    LEX_RPAREN,     // 35
    LEX_EQ,         // 36
    LEX_LSS,        // 37
    LEX_GTR,        // 38
    LEX_PLUS,       // 39
    LEX_MINUS,      // 40
    LEX_TIMES,      // 41
    LEX_SLASH,      // 42
    LEX_LEQ,        // 43
    LEX_NEQ,        // 44
    LEX_GEQ,        // 45
    LEX_PERC,       // 46
    LEX_NUM,        // 47
    LEX_UNMINUS,    // 48
    LEX_UNPLUS,     // 49
    LEX_CONSTSTR,   // 50
    LEX_CONSTREAL,  // 51
    LEX_ID,         // 52
    POLIZ_LABEL,    // 53
    POLIZ_ADDRESS,  // 54
    POLIZ_GO,       // 55
    POLIZ_FGO,      // 56
    POLIZ_TGO,      // 57
    POLIZ_PUSH,     // 58
    POLIZ_POP,      // 59
    POLIZ_TOP       // 60
};

union u_val {
   private:
    int i;
    double r;
    bool b;

   public:
    u_val() {}
    u_val( const int _i ) { i = _i; }
    u_val( const double _r ) { r = _r; }
    u_val( const bool _b ) { b = _b; }
    operator int() const { return i; }
    operator double() const { return r; }
    operator bool() const { return b; }
};

class Lex
{
    type_of_lex t_lex;
    u_val v_lex;

   public:
    Lex( type_of_lex t = LEX_NULL ) : t_lex( t ), v_lex( 0 ) {}
    template <class T>
    Lex( type_of_lex t, T v ) : t_lex( t ), v_lex( v )
    {
    }
    type_of_lex get_type() { return t_lex; }
    u_val get_value() { return v_lex; }
    friend ostream& operator<<( ostream& s, Lex l )
    {
        s << '(' << l.t_lex << ',';
        switch( l.t_lex )
        {
        case LEX_CONSTREAL:
        case LEX_REAL:
            s << static_cast<double>( l.v_lex );
            break;
        case LEX_BOOL:
            s << static_cast<bool>( l.v_lex );
            break;
        default:
            s << static_cast<int>( l.v_lex );
        }
        s << ");";
        return s;
    }
};

class Ident
{
    String name;
    bool declare;
    type_of_lex type;
    bool assign;
    u_val value;

   public:
    Ident()
    {
        declare = false;
        assign = false;
    }
    String& get_name() { return name; }
    void put_name( String& s ) { name = s; }
    bool get_declare() { return declare; }
    void put_declare() { declare = true; }
    type_of_lex get_type() { return type; }
    void put_type( type_of_lex t ) { type = t; }
    bool get_assign() { return assign; }
    void put_assign() { assign = true; }
    u_val get_value() { return value; }
    void put_value( u_val v ) { value = v; }
};

class tabl_ident
{
    Ident* p;
    int size;
    int top;

   public:
    tabl_ident( int max_size )
    {
        p = new Ident[size = max_size];
        top = 1;
    }
    ~tabl_ident() { delete[] p; }
    Ident& operator[]( int k ) { return p[k]; }
    int put( String buf )
    {
        for( int j = 1; j < top; ++j )
            if( buf == p[j].get_name() )
                return j;
        p[top].put_name( buf );
        return top++;
    }
};

class tabl_string
{
    String* p;
    int size;
    int top;

   public:
    tabl_string( int max_size = 100 )
    {
        p = new String[size = max_size];
        top = 1;
    }
    ~tabl_string() { delete[] p; }
    String& operator[]( int k ) { return p[k]; }
    int put( const String& buf )
    {
        p[top] = buf;
        return top++;
    }
};

class Scanner
{
    enum state
    {
        H,
        WORD,
        NUM,
        SLASH,
        COM,
        ENDCOM,
        STR,
        EQLSGR,
        NEQ,
        DOT,
        REAL
    };
    static const String TW[];
    static type_of_lex words[];
    static const String TD[];
    static type_of_lex dlms[];
    state CS;
    FILE* fp;
    char c;
    String buf;
    int cur_line;

    int look( String& buf, const String* list )
    {
        int i = 0;
        while( !list[i].empty() )
        {
            if( buf == list[i] )
                return i;
            ++i;
        }
        return 0;
    }
    void gc()
    {
        c = fgetc( fp );
        if( c == '\n' )
        {
            ++cur_line;
        }
    }

   public:
    Lex get_lex();
    int get_cur_line() { return cur_line; }
    Scanner( const char* program )
    {
        fp = fopen( program, "r" );
        cur_line = 1;
        CS = H;
        gc();
    }
};

const String Scanner::TW[] = {"0",      "and",  "bool",  "break",   "case",  "continue", "do",
                              "else",   "end",  "if",    "false",   "for",   "goto",     "int",
                              "not",    "of",   "or",    "program", "read",  "real",     "step",
                              "string", "true", "until", "while",   "write", "writeln",  NULL};

const String Scanner::TD[] = {"0", "@", ";", ",", ":", "=", "{",  "}",  "(",  ")", "==",
                              "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "%", NULL};

tabl_ident TID( 100 );
tabl_string TSTR( 100 );

type_of_lex Scanner::words[] = {
    LEX_NULL,   LEX_AND,  LEX_BOOL,  LEX_BREAK,   LEX_CASE,  LEX_CONTINUE, LEX_DO,
    LEX_ELSE,   LEX_END,  LEX_IF,    LEX_FALSE,   LEX_FOR,   LEX_GOTO,     LEX_INT,
    LEX_NOT,    LEX_OF,   LEX_OR,    LEX_PROGRAM, LEX_READ,  LEX_REAL,     LEX_STEP,
    LEX_STRING, LEX_TRUE, LEX_UNTIL, LEX_WHILE,   LEX_WRITE, LEX_WRITELN,  LEX_NULL};
type_of_lex Scanner::dlms[] = {
    LEX_NULL,   LEX_FIN,    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LBRACK, LEX_RBRACK,
    LEX_LPAREN, LEX_RPAREN, LEX_EQ,        LEX_LSS,   LEX_GTR,   LEX_PLUS,   LEX_MINUS,  LEX_TIMES,
    LEX_SLASH,  LEX_LEQ,    LEX_NEQ,       LEX_GEQ,   LEX_PERC,  LEX_NULL};

Lex Scanner::get_lex()
{
    int d, j;
    double r, e;
    CS = H;
    do
    {
        switch( CS )
        {
        case H:
            if( c == ' ' || c == '\n' || c == '\r' || c == '\t' )
            {
                gc();
            }
            else if( isalpha( c ) || c == '_' )
            {
                buf.clear();
                buf.push_back( c );
                gc();
                CS = WORD;
            }
            else if( isdigit( c ) )
            {
                d = c - '0';
                gc();
                CS = NUM;
            }
            else if( c == '*' || c == ',' || c == ';' || c == '%' || c == '{' || c == '}' ||
                     c == '+' || c == '-' || c == '(' || c == ')' || c == ':' )
            {
                buf.clear();
                buf.push_back( c );
                gc();
                j = look( buf, TD );
                return Lex( dlms[j], j );
            }
            else if( c == '/' )
            {
                gc();
                CS = SLASH;
            }
            else if( c == '"' )
            {
                buf.clear();
                gc();
                CS = STR;
            }
            else if( c == '=' || c == '<' || c == '>' )
            {
                buf.clear();
                buf.push_back( c );
                gc();
                CS = EQLSGR;
            }
            else if( c == '!' )
            {
                gc();
                CS = NEQ;
            }
            else if( c == '.' )
            {
                gc();
                CS = DOT;
            }
            else if( c == EOF )
            {
                return Lex( LEX_FIN );
            }
            else
            {
                throw "Lexic analyze: unexpected symbol";
            }
            break;
        case DOT:
            if( isdigit( c ) )
            {
                e = 0.1;
                r = ( c - '0' ) * e;
                gc();
                CS = REAL;
            }
            else
            {
                throw "Lexic analyze: unexpected symbol after '.'";
            }
            break;
        case REAL:
            if( isdigit( c ) )
            {
                e *= 0.1;
                r += ( c - '0' ) * e;
                gc();
            }
            else
            {
                return Lex( LEX_CONSTREAL, r );
            }
            break;
        case WORD:
            if( isalpha( c ) || isdigit( c ) || c == '_' )
            {
                buf.push_back( c );
                gc();
            }
            else if( j = look( buf, TW ) )
                return Lex( words[j], j );
            else
            {
                j = TID.put( buf );
                return Lex( LEX_ID, j );
            }
            break;
        case NUM:
            if( isdigit( c ) )
            {
                d = d * 10 + ( c - '0' );
                gc();
            }
            else if( c == '.' )
            {
                gc();
                r = (double)d;
                e = 1.0;
                CS = REAL;
            }
            else
            {
                return Lex( LEX_NUM, d );
            }
            break;
        case SLASH:
            if( c == '*' )
            {
                gc();
                CS = COM;
            }
            else
            {
                return Lex( LEX_SLASH );
            }
            break;
        case STR:
            if( c == EOF )
            {
                throw "Lexic analyze: Error (EOF in constant string)";
            }
            else if( c == '"' )
            {
                gc();
                return Lex( LEX_CONSTSTR, TSTR.put( buf ) );
            }
            else
            {
                buf.push_back( c );
                gc();
            }
            break;
        case EQLSGR:
            if( c == '=' )
            {
                buf.push_back( c );
                gc();
                j = look( buf, TD );
                return Lex( dlms[j], j );
            }
            else
            {
                j = look( buf, TD );
                return Lex( dlms[j], j );
            }
            break;
        case NEQ:
            if( c == '=' )
            {
                gc();
                return Lex( LEX_NEQ );
            }
            else
            {
                throw "Lexic analyze: Error";
            }
            break;
        case COM:
            if( c == '*' )
            {
                gc();
                CS = ENDCOM;
            }
            else if( c == EOF )
            {
                throw "Lexic analyze: Error (EOF in comment)";
            }
            else
            {
                gc();
            }
            break;
        case ENDCOM:
            if( c == '/' )
            {
                gc();
                CS = H;
            }
            else if( c == '*' )
            {
                gc();
            }
            else if( c == EOF )
            {
                throw "Lexic analyze: Error (EOF in comment)";
            }
            else
            {
                gc();
                CS = COM;
            }
            break;
        }
    } while( true );
}

template <class T>
class Stack
{
    T* s;
    int _top;
    int capacity;

   public:
    Stack()
    {
        _top = 0;
        s = new T[capacity = 20];
    }
    void reset() { _top = 0; }
    void push( T i );
    T pop();
    T top();
    bool is_empty() { return _top == 0; }
    bool is_full() { return _top == capacity; }
    void print()
    {
        for( int i = 0; i < _top; ++i )
        {
            cout << s[i];
        }
        cout << endl;
    }
};

template <class T>
void Stack<T>::push( T a )
{
    if( is_full() )
    {
        T* old = s;
        s = new T[capacity *= 2];
        for( int i = 0; i < _top; ++i )
        {
            s[i] = old[i];
        }
        delete[] old;
    }
    s[_top] = a;
    ++_top;
}

template <class T>
T Stack<T>::pop()
{
    if( !is_empty() )
    {
        --_top;
        return s[_top];
    }
    else
    {
        throw "Stack is empty";
    }
}

template <class T>
T Stack<T>::top()
{
    if( !is_empty() )
    {
        return s[_top - 1];
    }
    else
    {
        throw "Stack is empty";
    }
}

class Poliz
{
    Lex* p;
    int size;
    int free;

   public:
    Poliz( int max_size )
    {
        p = new Lex[size = max_size];
        free = 0;
    }
    ~Poliz() { delete[] p; }
    void put_lex( Lex l ) { p[free++] = l; }
    void put_lex( Lex l, int place ) { p[place] = l; }
    void blank() { ++free; }
    int get_free() { return free; }
    Lex& operator[]( int index )
    {
        if( index > size )
        {
            throw "POLIZ: out of array";
        }
        else if( index > free )
        {
            throw "POLIZ: indefinite element of array";
        }
        else
        {
            return p[index];
        }
    }
    void print()
    {
        for( int i = 0; i < free; ++i )
        {
            cout << i << ": " << p[i] << "\t";
            if( !( ( i + 1 ) % 5 ) )
            {
                cout << endl;
            }
        }
    }
};

class Parser
{
    Lex curr_lex;
    type_of_lex c_type;
    u_val c_val;
    Scanner scan;
    type_of_lex var_type;
    Stack<int> st_break;
    Stack<int> st_continue;
    Stack<int> st;
    int len_expr;

    void PROG();
    void DESCS();
    void DESC();
    void VARS();
    void VAR();
    void TYPE();
    void OPS();
    void OP();
    void VAR_LIST();
    void VAR_CASE();
    type_of_lex CONST();
    void CONDITION();
    type_of_lex EXPR();
    type_of_lex A0();
    type_of_lex A();
    type_of_lex B();
    type_of_lex C();
    type_of_lex D();
    type_of_lex E();

    void check_lex( type_of_lex t1 );
    void check_lex( type_of_lex t1, type_of_lex t2 );
    void check_id();
    void check_type( type_of_lex t1, type_of_lex t2 );
    void check_type( type_of_lex t1, type_of_lex t2, type_of_lex t3 );
    void check_assign_type( type_of_lex type1, type_of_lex type2 );
    void push_to_prog( Stack<int>& st, int go_addr );

    void gl()
    {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }

   public:
    Poliz prog;
    Parser( const char* program ) : scan( program ), prog( 1000 ) {}
    int get_cur_line() { return scan.get_cur_line(); }
    void analyze();
};

void Parser::check_lex( type_of_lex t1 )
{
    if( c_type == t1 )
    {
        gl();
    }
    else
    {
        throw curr_lex;
    }
}
void Parser::check_lex( type_of_lex t1, type_of_lex t2 )
{
    if( c_type == t1 )
    {
        gl();
    }
    else
    {
        throw curr_lex;
    }
    if( c_type == t2 )
    {
        gl();
    }
    else
    {
        throw curr_lex;
    }
}

void Parser::check_id()
{
    if( !TID[c_val].get_declare() )
    {
        throw "Undeclared variable";
    }
}

void Parser::check_type( type_of_lex t1, type_of_lex t2 )
{
    if( t1 != t2 )
    {
        throw "Unexpected type";
    }
}
void Parser::check_type( type_of_lex t1, type_of_lex t2, type_of_lex t3 )
{
    if( t1 != t2 && t1 != t3 )
    {
        throw "Unexpected type";
    }
}

void Parser::check_assign_type( type_of_lex type1, type_of_lex type2 )
{
    if( type1 == LEX_INT || type1 == LEX_REAL )
    {
        check_type( type2, LEX_REAL, LEX_INT );
    }
    else
    {
        check_type( type1, type2 );
    }
}

void Parser::push_to_prog( Stack<int>& st, int go_addr )
{
    int addr;
    while( ( addr = st.pop() ) != -1 )
    {
        prog.put_lex( Lex( POLIZ_LABEL, go_addr ), addr );
    }
}

void Parser::analyze()
{
    gl();
    PROG();
    cout << "Parsing is correctly completed" << endl;
}

void Parser::PROG()
{
    check_lex( LEX_PROGRAM, LEX_LBRACK );
    DESCS();
    OPS();
    check_lex( LEX_RBRACK );
    if( c_type != LEX_FIN )
    {
        throw curr_lex;
    }
}

void Parser::DESCS()
{
    while( c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL || c_type == LEX_REAL )
    {
        DESC();
        check_lex( LEX_SEMICOLON );
    }
}

void Parser::DESC()
{
    TYPE();
    VARS();
}

void Parser::VARS()
{
    VAR();
    while( c_type == LEX_COMMA )
    {
        gl();
        VAR();
    }
}

void Parser::TYPE()
{
    if( c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL || c_type == LEX_REAL )
    {
        var_type = c_type;
        gl();
    }
    else
    {
        throw curr_lex;
    }
}

void Parser::VAR()
{
    if( c_type == LEX_ID )
    {
        if( TID[c_val].get_declare() )
        {
            throw "Second declaration";
        }
        else
        {
            TID[c_val].put_declare();
            TID[c_val].put_type( var_type );
        }
        prog.put_lex( Lex( POLIZ_ADDRESS, c_val ) );
        gl();
    }
    else
    {
        throw curr_lex;
    }
    if( c_type == LEX_ASSIGN )
    {
        gl();
        type_of_lex const_type = CONST();
        if( const_type != var_type )
        {
            throw "Unmatched type";
        }
        prog.put_lex( Lex( LEX_ASSIGN ) );
    }
}

type_of_lex Parser::CONST()
{
    type_of_lex const_type;
    if( c_type == LEX_MINUS || c_type == LEX_PLUS )
    {
        type_of_lex sign = ( c_type == LEX_MINUS ) ? LEX_UNMINUS : LEX_UNPLUS;
        gl();
        switch( c_type )
        {
        case LEX_NUM:
            const_type = LEX_INT;
            break;
        case LEX_CONSTREAL:
            const_type = LEX_REAL;
            break;
        default:
            throw curr_lex;
        }
        prog.put_lex( Lex( const_type, c_val ) );
        prog.put_lex( Lex( sign ) );
    }
    else
    {
        u_val const_value = c_val;
        switch( c_type )
        {
        case LEX_NUM:
            const_type = LEX_INT;
            break;
        case LEX_CONSTREAL:
            const_type = LEX_REAL;
            break;
        case LEX_CONSTSTR:
            const_type = LEX_STRING;
            break;
        case LEX_TRUE:
            const_type = LEX_BOOL;
            const_value = true;
            break;
        case LEX_FALSE:
            const_type = LEX_BOOL;
            const_value = false;
            break;
        default:
            throw curr_lex;
        }
        prog.put_lex( Lex( const_type, const_value ) );
    }
    gl();
    return const_type;
}

void Parser::OPS()
{
    while( c_type == LEX_IF || c_type == LEX_WHILE || c_type == LEX_DO || c_type == LEX_FOR ||
           c_type == LEX_CASE || c_type == LEX_READ || c_type == LEX_WRITE ||
           c_type == LEX_WRITELN || c_type == LEX_CONTINUE || c_type == LEX_BREAK ||
           c_type == LEX_GOTO || c_type == LEX_ID || c_type == LEX_LBRACK )
    {
        OP();
    }
}

void Parser::OP()
{
    switch( c_type )
    {
    case( LEX_IF ):
    {
        gl();
        check_lex( LEX_LPAREN );
        CONDITION();
        check_lex( LEX_RPAREN );
        int addr1 = prog.get_free();
        prog.blank();
        prog.put_lex( Lex( POLIZ_FGO ) );
        OP();
        if( c_type == LEX_ELSE )
        {
            gl();
            int addr2 = prog.get_free();
            prog.blank();
            prog.put_lex( Lex( POLIZ_GO ) );
            prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr1 );
            OP();
            prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr2 );
        }
        else
        {
            prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr1 );
        }
    }
    break;
    case LEX_WHILE:
    {
        gl();
        int addr_cond = prog.get_free();
        check_lex( LEX_LPAREN );
        CONDITION();
        check_lex( LEX_RPAREN );
        int addr1 = prog.get_free();
        prog.blank();
        prog.put_lex( Lex( POLIZ_FGO ) );
        st_break.push( -1 );
        st_continue.push( -1 );
        OP();
        prog.put_lex( Lex( POLIZ_LABEL, addr_cond ) );
        prog.put_lex( Lex( POLIZ_GO ) );
        prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr1 );
        push_to_prog( st_break, prog.get_free() );
        push_to_prog( st_continue, addr_cond );
    }
    break;
    case LEX_FOR:
    {
        gl();
        if( c_type == LEX_ID )
        {
            check_id();
            if( TID[c_val].get_type() != LEX_INT )
            {
                throw "Unexpected type (int expected)";
            }
            Lex ID = curr_lex;
            gl();
            prog.put_lex( Lex( POLIZ_ADDRESS, ID.get_value() ) );
            check_lex( LEX_ASSIGN );
            if( EXPR() != LEX_INT )
            {
                throw "Unexpected type (int expected)";
            }
            prog.put_lex( Lex( LEX_ASSIGN ) );
            int addr1 = prog.get_free();
            prog.blank();
            prog.put_lex( Lex( POLIZ_GO ) );
            check_lex( LEX_STEP );
            int addr_step = prog.get_free();
            prog.put_lex( Lex( POLIZ_ADDRESS, ID.get_value() ) );
            prog.put_lex( ID );
            if( EXPR() != LEX_INT )
            {
                throw "Unexpected type (int expected)";
            }
            prog.put_lex( Lex( LEX_PLUS ) );
            prog.put_lex( Lex( LEX_ASSIGN ) );
            check_lex( LEX_UNTIL );
            prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr1 );
            prog.put_lex( ID );
            if( EXPR() != LEX_INT )
            {
                throw "Unexpected type (int expected)";
            }
            prog.put_lex( Lex( LEX_EQ ) );
            int addr2 = prog.get_free();
            prog.blank();
            prog.put_lex( Lex( POLIZ_FGO ) );
            int addr3 = prog.get_free();
            prog.blank();
            prog.put_lex( Lex( POLIZ_GO ) );
            check_lex( LEX_DO );
            prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr2 );
            st_break.push( -1 );
            st_continue.push( -1 );
            OP();
            prog.put_lex( Lex( POLIZ_LABEL, addr_step ) );
            prog.put_lex( Lex( POLIZ_GO ) );
            prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr3 );
            push_to_prog( st_break, prog.get_free() );
            push_to_prog( st_continue, addr_step );
        }
        else if( c_type == LEX_LPAREN )
        {
            gl();
            if( c_type != LEX_SEMICOLON )
            {
                EXPR();
            }
            check_lex( LEX_SEMICOLON );
            int addr_cond = prog.get_free();
            if( c_type != LEX_SEMICOLON )
            {
                CONDITION();
            }
            else
            {
                prog.put_lex( Lex( LEX_TRUE ) );
            }
            check_lex( LEX_SEMICOLON );
            int addr1 = prog.get_free();
            prog.blank();
            prog.put_lex( Lex( POLIZ_FGO ) );
            int addr2 = prog.get_free();
            prog.blank();
            prog.put_lex( Lex( POLIZ_GO ) );
            int addr_change = prog.get_free();
            if( c_type != LEX_RPAREN )
            {
                EXPR();
            }
            check_lex( LEX_RPAREN );
            prog.put_lex( Lex( POLIZ_LABEL, addr_cond ) );
            prog.put_lex( Lex( POLIZ_GO ) );
            prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr2 );
            st_break.push( -1 );
            st_continue.push( -1 );
            OP();
            prog.put_lex( Lex( POLIZ_LABEL, addr_change ) );
            prog.put_lex( Lex( POLIZ_GO ) );
            prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr1 );
            push_to_prog( st_break, prog.get_free() );
            push_to_prog( st_continue, addr_change );
        }
        else
        {
            throw curr_lex;
        }
    }
    break;
    case LEX_DO:
    {
        gl();
        int addr_op = prog.get_free();
        st_break.push( -1 );
        st_continue.push( -1 );
        OP();
        check_lex( LEX_WHILE, LEX_LPAREN );
        int addr_cond = prog.get_free();
        CONDITION();
        prog.put_lex( Lex( LEX_NOT ) );
        prog.put_lex( Lex( POLIZ_LABEL, addr_op ) );
        prog.put_lex( Lex( POLIZ_FGO ) );
        push_to_prog( st_break, prog.get_free() );
        push_to_prog( st_continue, addr_cond );
        check_lex( LEX_RPAREN, LEX_SEMICOLON );
    }
    break;
    case LEX_CASE:
    {
        gl();
        check_lex( LEX_LPAREN );
        if( EXPR() != LEX_INT )
        {
            throw "Unexpected type (int expected)";
        }
        prog.put_lex( Lex( POLIZ_PUSH ) );
        check_lex( LEX_RPAREN, LEX_OF );
        st_break.push( -1 );
        VAR_LIST();
        push_to_prog( st_break, prog.get_free() );
        prog.put_lex( Lex( POLIZ_POP ) );
        check_lex( LEX_END, LEX_SEMICOLON );
    }
    break;
    case LEX_READ:
    {
        gl();
        check_lex( LEX_LPAREN );
        if( c_type == LEX_ID )
        {
            check_id();
            if( TID[c_val].get_type() == LEX_BOOL )
            {
                throw "Bool is not allowed in read()";
            }
            prog.put_lex( Lex( POLIZ_ADDRESS, c_val ) );
            prog.put_lex( Lex( LEX_READ ) );
            gl();
        }
        else
        {
            throw curr_lex;
        }
        check_lex( LEX_RPAREN, LEX_SEMICOLON );
    }
    break;
    case LEX_WRITE:
    case LEX_WRITELN:
    {
        type_of_lex write_type = c_type;
        gl();
        check_lex( LEX_LPAREN );
        int argc = 0;
        if( c_type != LEX_RPAREN )
        {
            EXPR();
            ++argc;
        }
        while( c_type == LEX_COMMA )
        {
            gl();
            EXPR();
            ++argc;
        }
        prog.put_lex( Lex( LEX_INT, argc ) );
        prog.put_lex( Lex( write_type ) );
        check_lex( LEX_RPAREN, LEX_SEMICOLON );
    }
    break;
    case LEX_CONTINUE:
    {
        gl();
        st_continue.push( prog.get_free() );
        prog.blank();
        prog.put_lex( Lex( POLIZ_GO ) );
        check_lex( LEX_SEMICOLON );
    }
    break;
    case LEX_BREAK:
    {
        gl();
        st_break.push( prog.get_free() );
        prog.blank();
        prog.put_lex( Lex( POLIZ_GO ) );
        check_lex( LEX_SEMICOLON );
    }
    break;
    case LEX_LBRACK:
    {
        gl();
        OPS();
        check_lex( LEX_RBRACK );
    }
    break;
    case LEX_ID:
    {
        check_id();
        int ID_num = c_val;
        gl();
        if( c_type == LEX_ASSIGN )
        {
            gl();
            prog.put_lex( Lex( POLIZ_ADDRESS, ID_num ) );
            type_of_lex type = EXPR();
            check_assign_type( TID[ID_num].get_type(), type );
            prog.put_lex( Lex( LEX_ASSIGN ) );
            check_lex( LEX_SEMICOLON );
        }
        else if( c_type == LEX_COLON )
        {
            gl();
            if( TID[ID_num].get_declare() )
            {
                throw "Declared variable can't be a label";
            }
            TID[ID_num].put_declare();
            TID[ID_num].put_type( POLIZ_LABEL );
            TID[ID_num].put_value( prog.get_free() );
            OP();
        }
        else
        {
            throw curr_lex;
        }
    }
    break;
    case LEX_GOTO:
    {
        gl();
        if( c_type == LEX_ID )
        {
            check_id();
            if( TID[c_val].get_type() != POLIZ_LABEL )
            {
                throw "Unexpexted ID (label expected)";
            }
            prog.put_lex( Lex( POLIZ_LABEL, TID[c_val].get_value() ) );
            prog.put_lex( Lex( POLIZ_GO ) );
            gl();
        }
        else
        {
            throw curr_lex;
        }
        check_lex( LEX_SEMICOLON );
    }
    break;
    default:
        throw curr_lex;
    }
}

void Parser::VAR_LIST()
{
    do
    {
        VAR_CASE();
    } while( c_type == LEX_NUM );
}

void Parser::VAR_CASE()
{
    type_of_lex const_type;
    st.push( -1 );
    prog.put_lex( Lex( POLIZ_TOP ) );
    const_type = CONST();
    if( const_type != LEX_INT )
    {
        throw "Unexpected type (int expected)";
    }
    prog.put_lex( Lex( LEX_NEQ ) );
    st_continue.push( prog.get_free() );
    prog.blank();
    prog.put_lex( Lex( POLIZ_FGO ) );
    while( c_type == LEX_COMMA )
    {
        gl();
        prog.put_lex( Lex( POLIZ_TOP ) );
        const_type = CONST();
        if( const_type != LEX_INT )
        {
            throw "Unexpected type (int expected)";
        }
        prog.put_lex( Lex( LEX_NEQ ) );
        st_continue.push( prog.get_free() );
        prog.blank();
        prog.put_lex( Lex( POLIZ_FGO ) );
    }
    int addr_not_match = prog.get_free();
    prog.blank();
    prog.put_lex( Lex( POLIZ_GO ) );
    push_to_prog( st, prog.get_free() );
    check_lex( LEX_COLON );
    OP();
    prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr_not_match );
}

void Parser::CONDITION()
{
    if( EXPR() != LEX_BOOL )
    {
        throw "The condition must be of type bool";
    }
}

type_of_lex Parser::EXPR()
{
    type_of_lex type;
    len_expr = 0;
    type = A0();
    if( c_type == LEX_ASSIGN )
    {
        if( len_expr == 1 )
        {
            gl();
            check_assign_type( type, EXPR() );
            prog.put_lex( Lex( LEX_ASSIGN ) );
        }
        else
        {
            throw "Incorrect assign";
        }
    }
    return type;
}

type_of_lex Parser::A0()
{
    type_of_lex type;
    type = A();
    if( c_type == LEX_OR )
    {
        st.push( -1 );
        while( c_type == LEX_OR )
        {
            gl();
            st.push( prog.get_free() );
            prog.blank();
            prog.put_lex( Lex( POLIZ_TGO ) );
            check_type( type, LEX_BOOL );
            check_type( A(), LEX_BOOL );
            prog.put_lex( Lex( LEX_OR ) );
        }
        int addr = prog.get_free();
        prog.blank();
        prog.put_lex( Lex( POLIZ_GO ) );
        push_to_prog( st, prog.get_free() );
        prog.put_lex( Lex( LEX_BOOL, true ) );
        prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr );
    }
    return type;
}

type_of_lex Parser::A()
{
    type_of_lex type;
    type = B();
    if( c_type == LEX_AND )
    {
        st.push( -1 );
        while( c_type == LEX_AND )
        {
            gl();
            st.push( prog.get_free() );
            prog.blank();
            prog.put_lex( Lex( POLIZ_FGO ) );
            check_type( type, LEX_BOOL );
            check_type( B(), LEX_BOOL );
        }
        int addr = prog.get_free();
        prog.blank();
        prog.put_lex( Lex( POLIZ_GO ) );
        push_to_prog( st, prog.get_free() );
        prog.put_lex( Lex( LEX_BOOL, false ) );
        prog.put_lex( Lex( POLIZ_LABEL, prog.get_free() ), addr );
    }
    return type;
}

type_of_lex Parser::B()
{
    type_of_lex type;
    type = C();
    if( type == LEX_STRING )
    {
        if( c_type == LEX_EQ || c_type == LEX_NEQ || c_type == LEX_LSS || c_type == LEX_GTR )
        {
            Lex op = curr_lex;
            gl();
            check_type( C(), LEX_STRING );
            type = LEX_BOOL;
            prog.put_lex( op );
        }
        return type;
    }
    if( c_type == LEX_EQ || c_type == LEX_NEQ || c_type == LEX_LSS || c_type == LEX_GTR ||
        c_type == LEX_LEQ || c_type == LEX_GEQ )
    {
        Lex op = curr_lex;
        gl();
        check_type( C(), LEX_INT, LEX_REAL );
        type = LEX_BOOL;
        prog.put_lex( op );
    }
    return type;
}

type_of_lex Parser::C()
{
    type_of_lex type, type1;
    type = D();
    if( type == LEX_STRING )
    {
        while( c_type == LEX_PLUS )
        {
            gl();
            check_type( D(), LEX_STRING );
            prog.put_lex( Lex( LEX_PLUS ) );
        }
        return type;
    }
    while( c_type == LEX_PLUS || c_type == LEX_MINUS )
    {
        Lex op = curr_lex;
        gl();
        type1 = D();
        check_type( type1, LEX_INT, LEX_REAL );
        type = ( type == LEX_REAL || type1 == LEX_REAL ) ? LEX_REAL : LEX_INT;
        prog.put_lex( op );
    }
    return type;
}

type_of_lex Parser::D()
{
    type_of_lex type, type1;
    type = E();
    while( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_PERC )
    {
        Lex op = curr_lex;
        gl();
        type1 = E();
        if( op.get_type() == LEX_PERC )
        {
            check_type( type, LEX_INT );
            check_type( type1, LEX_INT );
        }
        else
        {
            check_type( type, LEX_INT, LEX_REAL );
            check_type( type1, LEX_INT, LEX_REAL );
            type = ( type == LEX_REAL || type1 == LEX_REAL ) ? LEX_REAL : LEX_INT;
        }
        prog.put_lex( op );
    }
    return type;
}

type_of_lex Parser::E()
{
    type_of_lex type;
    switch( c_type )
    {
    case LEX_LPAREN:
        gl();
        type = EXPR();
        check_lex( LEX_RPAREN );
        len_expr += 2;
        break;
    case LEX_NOT:
        type = LEX_BOOL;
        gl();
        check_type( E(), LEX_BOOL );
        prog.put_lex( Lex( LEX_NOT ) );
        ++len_expr;
        break;
    case LEX_MINUS:
        gl();
        type = EXPR();
        check_type( type, LEX_INT, LEX_REAL );
        prog.put_lex( Lex( LEX_UNMINUS ) );
        ++len_expr;
        break;
    case LEX_PLUS:
        gl();
        type = EXPR();
        check_type( type, LEX_INT, LEX_REAL );
        prog.put_lex( Lex( LEX_UNPLUS ) );
        ++len_expr;
        break;
    case LEX_ID:
    {
        check_id();
        type = TID[c_val].get_type();
        Lex ID = curr_lex;
        gl();
        if( c_type == LEX_ASSIGN )
        {
            prog.put_lex( Lex( POLIZ_ADDRESS, ID.get_value() ) );
        }
        else
        {
            prog.put_lex( ID );
        }
        ++len_expr;
    }
    break;
    default:
        type = CONST();  // If there is error, CONST () will detect it
        len_expr += 2;
    }
    return type;
}

class Executer
{
    Lex pc_el;

    template <class T>
    bool compare( const T& v1, const T& v2, type_of_lex relation );
    template <class T>
    u_val calculate( const T& v1, const T& v2, type_of_lex operation );
    double to_double( Lex l );

   public:
    void execute( Poliz& prog );
};

template <class T>
bool Executer::compare( const T& v1, const T& v2, type_of_lex relation )
{
    switch( relation )
    {
    case LEX_EQ:
        return v1 == v2;
    case LEX_LEQ:
        return v1 <= v2;
    case LEX_NEQ:
        return v1 != v2;
    case LEX_GEQ:
        return v1 >= v2;
    case LEX_LSS:
        return v1 < v2;
    case LEX_GTR:
        return v1 > v2;
    default:
        throw "Error in Executer::compare";
    }
}

template <class T>
u_val Executer::calculate( const T& v1, const T& v2, type_of_lex operation )
{
    switch( operation )
    {
    case LEX_PLUS:
        return v1 + v2;
    case LEX_MINUS:
        return v1 - v2;
    case LEX_TIMES:
        return v1 * v2;
    case LEX_SLASH:
        return v1 / v2;
    default:
        throw "Error in Executer::calculate";
    }
}

double Executer::to_double( Lex l )
{
    if( l.get_type() == LEX_REAL )
    {
        return double( l.get_value() );
    }
    else
    {
        return double( int( l.get_value() ) );
    }
}

void Executer::execute( Poliz& prog )
{
    Stack<Lex> args;
    Stack<Lex> st_case;
    Stack<Lex> st_write;
    int op_id, j, index = 0, size = prog.get_free();
    Lex op1, op2;
    u_val res;

    while( index < size )
    {
        pc_el = prog[index];

        switch( pc_el.get_type() )
        {
        case LEX_BOOL:
        case LEX_INT:
        case LEX_REAL:
        case LEX_STRING:
        case POLIZ_ADDRESS:
        case POLIZ_LABEL:
            args.push( pc_el );
            break;
        case LEX_ID:
            op_id = pc_el.get_value();
            args.push( Lex( TID[op_id].get_type(), TID[op_id].get_value() ) );
            break;
        case LEX_NOT:
            op1 = args.pop();
            res = !bool( op1.get_value() );
            args.push( Lex( LEX_BOOL, res ) );
            break;
        case LEX_AND:
            op2 = args.pop();
            op1 = args.pop();
            res = bool( op1.get_value() ) && bool( op2.get_value() );
            args.push( Lex( LEX_BOOL, res ) );
            break;
        case LEX_OR:
            op2 = args.pop();
            op1 = args.pop();
            res = bool( op1.get_value() ) || bool( op2.get_value() );
            args.push( Lex( LEX_BOOL, res ) );
            break;
        case LEX_ASSIGN:
            op2 = args.pop();
            op_id = args.pop().get_value();
            switch( TID[op_id].get_type() )
            {
            case LEX_INT:
                if( op2.get_type() == LEX_INT )
                {
                    res = int( op2.get_value() );
                }
                else
                {
                    res = int( double( op2.get_value() ) );
                }
                break;
            case LEX_REAL:
                if( op2.get_type() == LEX_REAL )
                {
                    res = op2.get_value();
                }
                else
                {
                    res = double( int( op2.get_value() ) );
                }
                break;
            case LEX_BOOL:
                res = op2.get_value();
                break;
            case LEX_STRING:
                res = TSTR.put( TSTR[op2.get_value()] );
                break;
            default:
                cout << "Type of variable: " << TID[op_id].get_type() << endl;
                throw index;
            }
            TID[op_id].put_value( res );
            args.push( Lex( TID[op_id].get_type(), res ) );
            break;
        case LEX_UNMINUS:
            op1 = args.pop();
            if( op1.get_type() == LEX_INT )
            {
                res = -int( op1.get_value() );
            }
            else
            {
                res = -double( op1.get_value() );
            }
            args.push( Lex( op1.get_type(), res ) );
            break;
        case LEX_UNPLUS:
            break;
        case LEX_EQ:
        case LEX_LEQ:
        case LEX_NEQ:
        case LEX_GEQ:
        case LEX_LSS:
        case LEX_GTR:
        {
            op2 = args.pop();
            op1 = args.pop();
            type_of_lex t1 = op1.get_type(), t2 = op2.get_type();
            type_of_lex relation = pc_el.get_type();

            if( t1 == LEX_STRING )
            {
                res = compare<String>( TSTR[op1.get_value()], TSTR[op2.get_value()], relation );
            }
            else if( t1 == LEX_INT && t2 == LEX_INT )
            {
                res = compare<int>( int( op1.get_value() ), int( op2.get_value() ), relation );
            }
            else
            {
                res = compare<double>( to_double( op1 ), to_double( op2 ), relation );
            }
            args.push( Lex( LEX_BOOL, res ) );
        }
        break;
        case LEX_PLUS:
        case LEX_MINUS:
        case LEX_TIMES:
        case LEX_SLASH:
        {
            op2 = args.pop();
            op1 = args.pop();
            type_of_lex t1 = op1.get_type(), t2 = op2.get_type();
            type_of_lex operation = pc_el.get_type();
            type_of_lex type;

            if( t1 == LEX_STRING )
            {
                res = TSTR.put( TSTR[op1.get_value()] + TSTR[op2.get_value()] );
                type = LEX_STRING;
            }
            else if( t1 == LEX_INT && t2 == LEX_INT )
            {
                res = calculate<int>( int( op1.get_value() ), int( op2.get_value() ), operation );
                type = LEX_INT;
            }
            else
            {
                res = calculate<double>( to_double( op1 ), to_double( op2 ), operation );
                type = LEX_REAL;
            }
            args.push( Lex( type, res ) );
        }
        break;
        case LEX_PERC:
            op2 = args.pop();
            op1 = args.pop();
            res = int( op1.get_value() ) % int( op2.get_value() );
            args.push( Lex( LEX_INT, res ) );
            break;
        case LEX_READ:
        {
            op1 = args.pop();
            op_id = op1.get_value();
            switch( TID[op_id].get_type() )
            {
            case LEX_INT:
            {
                int d;
                cin >> d;
                TID[op_id].put_value( d );
            }
            break;
            case LEX_REAL:
            {
                double r;
                cin >> r;
                TID[op_id].put_value( r );
            }
            break;
            case LEX_STRING:
            {
                String s;
                cin >> s;
                res = TSTR.put( s );
                TID[op_id].put_value( res );
            }
            break;
            default:
                throw index;
            }
        }
        break;
        case LEX_WRITE:
        case LEX_WRITELN:
        {
            int argc = args.pop().get_value();
            for( int i = 0; i < argc; ++i )
            {
                st_write.push( args.pop() );
            }
            for( int i = 0; i < argc; ++i )
            {
                op1 = st_write.pop();
                res = op1.get_value();
                switch( op1.get_type() )
                {
                case LEX_INT:
                    cout << int( res );
                    break;
                case LEX_REAL:
                    cout << double( res );
                    break;
                case LEX_BOOL:
                    cout << bool( res );
                    break;
                case LEX_STRING:
                    cout << TSTR[res];
                    break;
                default:
                    throw index;
                }
            }
            if( pc_el.get_type() == LEX_WRITELN )
            {
                cout << endl;
            }
        }
        break;
        case POLIZ_GO:
            index = int( args.pop().get_value() ) - 1;
            break;
        case POLIZ_FGO:
            j = int( args.pop().get_value() );
            if( !bool( args.pop().get_value() ) )
            {
                index = j - 1;
            }
            break;
        case POLIZ_TGO:
            j = int( args.pop().get_value() );
            if( bool( args.pop().get_value() ) )
            {
                index = j - 1;
            }
            break;
        case POLIZ_PUSH:
            st_case.push( args.pop() );
            break;
        case POLIZ_POP:
            st_case.pop();
            break;
        case POLIZ_TOP:
            args.push( st_case.top() );
            break;
        default:
            throw index;
        }
        if( debug )
        {
            args.print();
        }
        ++index;
    }
    cout << "\nExecuting succesfully completed\n";
}

int main( int argc, char** argv )
{
    if( argc > 3 )
    {
        debug = true;
    }
    Parser synt( argv[1] );
    Executer exec;
    try
    {
        synt.analyze();
        if( argc > 2 )
        {
            synt.prog.print();
        }
        cout << endl;
        exec.execute( synt.prog );
    }
    catch( const char* str )
    {
        cout << synt.get_cur_line() << ": " << str << endl;
    }
    catch( Lex curr_lex )
    {
        cout << synt.get_cur_line() << ": Syntax error: " << curr_lex << endl;
    }
    catch( int el )
    {
        cout << "Error in POLIZ in element " << el << endl;
    }
    return 0;
}
