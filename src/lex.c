#include "lexi/lex.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexi/err.h"

/* init --------------------------------------------------------------------- */

// Initialize a lexer.
void lexInit(struct lexer *lex) {
    lex->sourcecap = 0;
    lex->sourcebuf = NULL;
    lex->sourceend = NULL;

    lex->tokstart = NULL;
    lex->tokcurrn = NULL;

    lex->lineno = 1;
    lex->column = 0;
}

// Free a lexer.
void lexFree(struct lexer *lex) {
    free(lex->sourcebuf);
    lexInit(lex);
}


/* fill --------------------------------------------------------------------- */

// Fill the lexer with `len` bytes of `source`.
void lexFill(struct lexer *lex, const char *source, size_t len) {
    // Here, we want to move the remaining content of the source buffer to the
    // start of the buffer, allocate more space if necessary, then append the
    // provided source after the remaining source. This way, we don't use memory
    // storing the entirety of the source.

    // If we reallocate, what will be the new length of the remaining source?
    size_t remaining_len = (size_t) (lex->tokcurrn - lex->tokstart);

    // Move the remaining content to the start of the buffer
    if (remaining_len > 0) {
        memmove(lex->sourcebuf, lex->tokstart, remaining_len);
    }

    if (lex->tokstart != NULL) {
        lex->tokcurrn = lex->sourcebuf + (lex->tokcurrn - lex->tokstart);
        lex->tokstart = lex->sourcebuf;
    }

    // Reallocate to fit provided source
    if (remaining_len + len > lex->sourcecap) {
        // Get offset of start and current pointer from source start.
        // `tokcurrn` should always be equal to or ahead of `tokstart`.
        size_t curroff = (size_t) (lex->tokcurrn - lex->tokstart);
        size_t startoff;

        if (lex->tokstart != NULL) 
            startoff = (size_t) (lex->tokstart - lex->sourcebuf);
        else
            startoff = 0;

        lex->sourcecap = remaining_len + len;
        lex->sourcebuf = realloc(lex->sourcebuf, remaining_len + len);
        if (lex->sourcebuf == NULL)
            die("lexFill: realloc failure");

        lex->tokstart = lex->sourcebuf + startoff;
        lex->tokcurrn = lex->tokstart + curroff;
    }

    // Append provided source
    memcpy(lex->sourcebuf + remaining_len, source, len);
    lex->sourceend = lex->sourcebuf + remaining_len + len;
}

// Get the length of the lexer buffer.
size_t lexBufLen(struct lexer *lex) {
    return (size_t) (lex->sourceend - lex->sourcebuf);
}


/* debug -------------------------------------------------------------------- */

// Print the current state of the lexer.
void lexPrint(struct lexer *lex) {
    size_t buflen = lexBufLen(lex);
    fprintf(
        stderr,
        "[buf len: %zu] %.*s\n",
        buflen,
        (int) buflen,
        lex->sourcebuf
    );

    size_t toklen = (size_t) (lex->tokcurrn - lex->tokstart);
    size_t startoff = (size_t) (lex->tokstart - lex->sourcebuf);
    size_t currnoff = (size_t) (lex->tokcurrn - lex->sourcebuf);
    fprintf(
        stderr,
        "[tok len: %zu start: %zu currn: %zu] %.*s\n",
        toklen,
        startoff,
        currnoff,
        (int) toklen,
        lex->tokstart
    );
}


/* lexing ------------------------------------------------------------------- */

// Returns non-zero if the end pointer of the current token has reached the end
// of the buffer.
static int isAtEnd(struct lexer *lex) {
    return lex->tokcurrn >= lex->sourceend;
}

// Consume and return one character.
static char advance(struct lexer *lex) {
    char res = *lex->tokcurrn;

    if (res == '\n') {
        lex->lineno++;
        lex->column = 0;
    } else {
        lex->column++;
    }

    lex->tokcurrn++;
    return res;
}

// Peek at the next character without advancing the lexer.
static char peek(struct lexer *lex) {
    return *lex->tokcurrn;
}

// Emit the current token.
static void emitToken(
        struct lexer *lex,
        struct token *out,
        unsigned int lineno,
        unsigned int column) {
    out->lineno = lineno;
    out->column = column;

    out->len = (size_t) (lex->tokcurrn - lex->tokstart);
    out->lexeme = lex->tokstart;

    lex->tokstart = lex->tokcurrn;
}

// Skip whitespace.
static void skipWhitespace(struct lexer *lex) {
    int skipped = 0;

    while (!isAtEnd(lex) && isspace(peek(lex))) {
        skipped = 1;
        (void) advance(lex);
    }

    if (skipped)
        lex->tokstart = lex->tokcurrn;
}

// Get the next token. Returns 0 if lexer is at the end of the source buffer.
int lexNext(struct lexer *lex, struct token *out) {
    // Only skip whitespace if we are not currently inside a token
    if (lex->tokstart == lex->tokcurrn)
        skipWhitespace(lex);

    if (isAtEnd(lex))
        return 0;

    unsigned int lineno = lex->lineno;
    unsigned int column = lex->column;

    while (!isAtEnd(lex) && !isspace(peek(lex)))
        (void) advance(lex);

    if (isAtEnd(lex))
        return 0;

    emitToken(lex, out, lineno, column);
    return 1;
}

