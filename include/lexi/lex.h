// lexi - lex.h
//
// Token and lexer representation.

#ifndef LEXI_LEX_H
#define LEXI_LEX_H

#include <stddef.h>

struct token {
    size_t len;
    char  *lexeme;

    unsigned int lineno;
    unsigned int column;
};

struct lexer {
    size_t sourcecap;
    char  *sourcebuf;
    char  *sourceend;

    char *tokstart;
    char *tokcurrn;

    unsigned int lineno;
    unsigned int column;
};

// Initialize a lexer.
void lexInit(struct lexer *lex);

// Free a lexer.
void lexFree(struct lexer *lex);

// Fill the lexer with `len` bytes of `source`.
void lexFill(struct lexer *lex, const char *source, size_t len);

// Get the length of the lexer buffer.
size_t lexBufLen(struct lexer *lex);

// Get the next token. Returns 0 if lexer is at the end of the source buffer.
int lexNext(struct lexer *lex, struct token *out);

// Print the current state of the lexer.
void lexPrint(struct lexer *lex);

#endif /* LEXI_LEX_H */
