/*
 * YTS-stream.c
 *
 * Copyright (C) 2026 Reini Urban
 *
 * Single file yaml-test-suite test in plain C to check any deviations from libyaml.
 * - parse a yaml-test-suite/XXXX/in.yaml
 * - emit the node events in a special event stream format,
 *   and compare to yaml-test-suite/XXXX/test.event
 * - and emit it, comparing to yaml-test-suite/XXXX/out.yaml
 *
 * YTS-full is equivalent to:
 *    for f in `find yaml-test-suite -name in.yaml`; do
 *      ./yts-stream `dirname $f`
 *    done
 */

#define _GNU_SOURCE
#include "syck.h"
#include "YTS-lib.h"
#include <stdio.h>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

static int is_dir(FILE *f) {
    struct stat st;
    if (fstat(fileno(f), &st) == -1)
        return 0;

    return S_ISDIR(st.st_mode);
}

extern const struct test_node end_node;

/* parses the in.yaml, builds an event stream, and emits it to a cs string.
 */
static void test_yaml_and_stream(CuString *cs, const char *yaml, CuString *ev) {
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

    emitter->headless = 1;

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

    //puts("\n--- # Parsed Stream");
    CuStringAppend(ev, "+STR\n+DOC\n");
    emit_stream(ev, ystream);
    CuStringAppend(ev, "-DOC\n-STR\n");

    S_FREE(ystream);
    if (parser->syms != NULL)
        st_foreach(parser->syms, syck_free_copies, 0);
    syck_free_parser(parser);
    syck_free_emitter(emitter);
}

static int compare_cs(FILE *fh, CuString *cs) {
    CuString *file_cs = CuStringNew();
    CuString *s = CuStringNew();
    char buf[256];
    while (fgets(buf, sizeof(buf), fh)) {
        CuStringAppend(file_cs, buf);
    }
    if (strcmp(file_cs->buffer, cs->buffer) == 0)
        return 0;
    fprintf(stderr, "expected <\n%s> but was <\n%s>\n", file_cs->buffer, cs->buffer);
    return 1;
}

/*
   - emit and compare to out.yaml if parallel file exists
   - compare stream to test.event if parallel file exists.
*/
int main(int argc, char **argv) {
   char *yaml;
   struct test_node *stream;
   CuString *cs, *ev;
   FILE *fh, *outfh = NULL, *testfh = NULL;
   size_t fsize, nread;
   int retval = 0;

   if (argc != 2) {
       fprintf(stderr, "yaml-file argument missing\n");
       exit(1);
   }
   fh = fopen(argv[1], "r");
   if (!fh || is_dir(fh)) {
       // check if dir with in.yaml
       struct dirent **files;
       int n = scandir(argv[1], &files, NULL, alphasort);
       if (n) {
           int found = 0;
           int more = 0;
           for (int i=0; i<n; i++) {
               char a = files[i]->d_name[0];
               char fn[128];
               if (a == '.')
                   continue;
               if (!strcmp(files[i]->d_name, "in.yaml")) {
                   strncpy(fn, argv[1], sizeof(fn)-1);
                   strncat(fn, "/", sizeof(fn)-strlen(fn)-1);
                   strncat(fn, files[i]->d_name, sizeof(fn)-strlen(fn)-1);
                   fh = fopen(fn, "r");
                   found = 1;
               }
               else if (!strcmp(files[i]->d_name, "out.yaml")) {
                   strncpy(fn, argv[1], sizeof(fn)-1);
                   strncat(fn, "/", sizeof(fn)-strlen(fn)-1);
                   strncat(fn, files[i]->d_name, sizeof(fn)-strlen(fn)-1);
                   outfh = fopen(fn, "r");
                   more++;
               }
               else if (!strcmp(files[i]->d_name, "test.event")) {
                   strncpy(fn, argv[1], sizeof(fn)-1);
                   strncat(fn, "/", sizeof(fn)-strlen(fn)-1);
                   strncat(fn, files[i]->d_name, sizeof(fn)-strlen(fn)-1);
                   testfh = fopen(fn, "r");
                   more++;
               }
           }
           if (!found) {
               fprintf(stderr, "No in.yaml in %s\n", argv[1]);
               return 1;
           }
           if (more) {
               fprintf(stderr, "Also found %d more files in %s\n", more, argv[1]);
           }
       } else {
           fprintf(stderr, "Could not open file: %s\n", argv[1]);
           return 1;
       }
   }
   fseek(fh, 0, SEEK_END);
   fsize = ftell(fh);
   yaml = S_ALLOC_N(char, fsize + 1);
   fseek(fh, 0, SEEK_SET);
   yaml[fsize] = '\0';
   nread = fread(yaml, 1, fsize, fh);
   if (nread != fsize) {
       fprintf(stderr, "Unexpected file size %s: %zu != %zu read", argv[1], fsize, nread);
       exit(1);
   }

   puts("--- # YAML \n");
   puts(yaml);
   puts("\n...\n");

   cs = CuStringNew();
   ev = CuStringNew();
   test_yaml_and_stream(cs, yaml, ev);

   if (outfh) {
       //CuStringAppend(cs, "---\n");
       if (!compare_cs(outfh, cs))
           printf("OK out.yaml matches\n");
       else {
           printf("FAIL out.yaml does not match\n");
           retval++;
       }
       fclose(outfh);
   }
   if (testfh) {
       if (!compare_cs(testfh, ev))
           printf("OK test.event matches\n");
       else {
           printf("FAIL test.event does not match\n");
           retval++;
       }
       fclose(testfh);
   }

   CuStringFree(cs);
   CuStringFree(ev);
   S_FREE(yaml);
   return retval;
}
