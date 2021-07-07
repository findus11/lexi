#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "lexi/lex.h"

int main(void) {
    char buf[79];
    size_t len;
    struct lexer lex;
    struct token tok;

    lexInit(&lex);

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        len = strlen(buf);
        lexFill(&lex, buf, len);

        //fprintf(stderr, "\nFILL BUF\n");
        //lexPrint(&lex);

        while (lexNext(&lex, &tok)) {
            fprintf(
                stdout,
                "(%zu %u:%u) %.*s\n",
                tok.len,
                tok.lineno,
                tok.column,
                (int) tok.len,
                tok.lexeme
            );
        }
    }

    // Append some whitespace to let the lexer consume the final token (if any).
    lexFill(&lex, "\n ", 2);
    while (lexNext(&lex, &tok))
        fprintf(
            stdout,
            "(%zu %u:%u) %.*s\n",
            tok.len,
            tok.lineno,
            tok.column,
            (int) tok.len,
            tok.lexeme
        );

    lexFree(&lex);

    return 0;
}
