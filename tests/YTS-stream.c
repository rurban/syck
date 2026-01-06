#include "syck.h"
#include "YTS-lib.h"
#include <stdio.h>

extern const struct test_node end_node;

static void emit_stream(const char *yaml) {
    CuString *cs = CuStringNew();
    SyckEmitter *emitter = syck_new_emitter();
    struct test_node *ystream = S_ALLOC_N(struct test_node, 1);
    int doc_ct = 0;
    int i = 0;
  
    /* Set up parser */
    SyckParser *parser = syck_new_parser();
    syck_parser_str_auto(parser, yaml, NULL);
    syck_parser_handler(parser, syck_copy_handler);
    syck_parser_error_handler(parser, NULL);
    syck_parser_implicit_typing(parser, 1);
    syck_parser_taguri_expansion(parser, 1);

    /* Parse all streams */
    while (1) {
        struct test_node *ydoc;
        SYMID oid = syck_parse(parser);
        int res;

        if (parser->eof == 1)
            break;

        /* Add document to stream */
        res = syck_lookup_sym(parser, oid, (char **)&ydoc);
        if (0 == res)
            break;

        ystream[doc_ct] = ydoc[0];
        doc_ct++;
        S_REALLOC_N(ystream, struct test_node, doc_ct + 1);
    }
    ystream[doc_ct] = end_node;
    
    build_symbol_table(emitter, ystream);

    /* Build the stream */
    syck_output_handler(emitter, test_output_handler);
    syck_emitter_handler(emitter, test_emitter_handler);
    emitter->bonus = cs;
    while (ystream[i].type != T_END) {
        syck_emit(emitter, (st_data_t)&ystream[i]);
        syck_emitter_flush(emitter, 0);
        i++;
    }
    // print it
    puts(cs->buffer);

    S_FREE(ystream);
    if (parser->syms != NULL)
        st_foreach(parser->syms, syck_free_copies, 0);
    syck_free_parser(parser);
    
    CuStringFree(cs);
    syck_free_emitter(emitter);
}

int main(int argc, char **argv) {
   char *yaml;
   struct test_node *stream;
   if (argc == 2) {
       FILE *fh = fopen(argv[1], "r");
       size_t fsize;
       if (!fh) {
           fprintf(stderr, "Could not open file: %s\n", argv[1]);
           return 1;
       }
       fseek(fh, 0, SEEK_END);
       fsize = ftell(fh);
       yaml = S_ALLOC_N(char, fsize + 1);
       fseek(fh, 0, SEEK_SET);
       yaml[fsize] = '\0';
       fread(yaml, 1, fsize, fh);
   }
   else
       yaml = (char*)"test: 1\nand: \"with new\\nline\\n\"\nalso: &3 three\nmore: *3";
   puts("--- # YAML \n");
   puts(yaml);
   puts("\n...\n");

   emit_stream(yaml);

   S_FREE(yaml);
   return 0;
}
