#include "syck.h"
#include "gram.h"

int sycklex_yaml_utf8( YYSTYPE *, SyckParser * );
int sycklex_yaml_utf16( YYSTYPE *, SyckParser * );
int sycklex_yaml_utf32( YYSTYPE *, SyckParser * );
int sycklex_bytecode_utf8( YYSTYPE *sycklval, SyckParser *parser );
