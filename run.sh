obj_folder="bin"

c_files="src/main.c src/grammar/lex.yy.c src/grammar/grammar.tab.c "
output_file="ss"

lexer="src/grammar/lexer.l"
grammar="src/grammar/grammar.y"

lexer_out="src/grammar/lex.yy.c"
grammar_out="src/grammar/grammar.tab.c"

bison "$grammar" -o "$grammar_out" -H
flex -o "$lexer_out" "$lexer" 

gcc $c_files -o "$output_file"

./$output_file