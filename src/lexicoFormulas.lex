%option noyywrap
%option   outfile="scanner.c" header-file="scanner.h"

%top{
  #include <iostream>
  #include "grammarFormulas.tab.c"

  #define SAVE_TOKEN yylval.String = new std::string(yytext, yyleng)
  #define TOKEN(t) (yylval.token = t)

  using namespace std;
  extern "C" int yywrap() { }
}

DIGIT [0-9]
VARIAVEL [A-Z]
LITERAL [a-tx-z]
MAXPT "v"
MINPT "u"
OR "or"
AND "and"
NOT "not"

%%

[ \t\n]+          /* eat up whitespace */
"." { 
	return TOKEN(POINT);	
        }

{LITERAL} { 
           SAVE_TOKEN ;
            return LITERAL;
          }

{MAXPT} { 
            return TOKEN(MAXPT);
			
        }

{MINPT} { 
            return TOKEN(MINPT);
        }

{OR} { 
	return TOKEN(OR);
        }

{AND} { 
            return TOKEN(AND);
        }

{NOT} { 
	return TOKEN(NOT);		
        }

"AX" { 
        return TOKEN(AX);
        }

"EX"	{ 
	return TOKEN(EX);		
        }

{VARIAVEL} { 
            SAVE_TOKEN ;
            return VARIAVEL;
        }

"(" { 
	return TOKEN(LPAREN);		
        }

")" { 
	return TOKEN(RPAREN);				
        }

"," { 
	return TOKEN(VIRG);				
        }

"+" { 
	return TOKEN(PLUS);				
        }
"-" { 
	return TOKEN(MINUS);				
        }

":" { 
	return TOKEN(DBPOINT);				
        }

[0-9]+[a-zA-Z0-9]+ {SAVE_TOKEN ;
                        return ESTADO;}

.           printf( "Unrecognized character: %s\n", yytext );

%%
