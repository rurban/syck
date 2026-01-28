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
#include <libgen.h>
#include <unistd.h>

extern const struct test_node end_node;
extern int syckdebug;

static FILE *open_path(const char *path, const char *fname) {
    char fn[512];
    strncpy(fn, path, sizeof(fn)-1);
    strncat(fn, "/", sizeof(fn)-strlen(fn)-1);
    strncat(fn, fname, sizeof(fn)-strlen(fn)-1);
    return fopen(fn, "r");
}

/*
   - emit and compare to out.yaml if parallel file exists
   - compare stream to test.event if parallel file exists.
*/
int main(int argc, char **argv) {
   struct test_node *stream;
   char fn[512];
   char *yaml;
   CuString *cs, *ev;
   FILE *fh = NULL, *outfh = NULL, *testfh = NULL;
   struct dirent **files;
   size_t fsize, nread;
   int retval = 0;
   int n, should_fail = 0;

   if (argc > 2 && strEQc(argv[1], "--debug")) {
       argc--;
       syckdebug = 1;
       strcpy(fn, argv[2]);
   }
   if (argc != 2) {
       fprintf(stderr, "yaml-file argument missing\n");
       exit(1);
   }
   if (!syckdebug)
       strcpy(fn, argv[1]);
   if (file_exists(fn)) {
       fh = fopen(fn,"r");
       strcpy(fn, dirname(argv[1]));
   } else if (!dir_exists(fn)) {
       fprintf(stderr, "Could not open file or directory: %s\n", fn);
       exit(1);
   }
   // check if dir with in.yaml
   n = scandir(fn, &files, NULL, alphasort);
   if (n > 0) {
       FILE *fh1;
       int found = 0;
       int more = 0;
       fh1 = open_path(fn, "error");
       if (fh1) {
           should_fail = 1;
           fclose(fh1);
       }
       for (int i=0; i<n; i++) {
           char a = files[i]->d_name[0];
           if (a == '.')
               continue;
           if (strEQc(files[i]->d_name, "in.yaml")) {
               if (fh)
                   fclose(fh);
               fh = open_path(fn, "in.yaml");
               found = 1;
           }
           else if (strEQc(files[i]->d_name, "out.yaml")) {
               outfh = open_path(fn, "out.yaml");
               more++;
           }
           else if (strEQc(files[i]->d_name, "test.event")) {
               testfh = open_path(fn, "test.event");
               more++;
           }
           else if (!should_fail && strEQc(files[i]->d_name, "error")) {
               should_fail = 1;
           }
           else if (strEQc(files[i]->d_name, "===")) {
               FILE *cmt = open_path(fn, "===");
               cs = CuSlurpFile(cmt);
               // skip the ending \n
               if (cs->length)
                   cs->buffer[cs->length-1] = '\0';
               if (should_fail)
                   printf("%s \"%s\". Does not parse.\n", fn, cs->buffer);
               else
                   printf("%s \"%s\"\n", fn, cs->buffer);
               fclose(cmt);
               CuStringFree(cs);
           }
       }
       while (n--) {
           free(files[n]);
       }
       free(files);
       if (!found && !fh) {
           fprintf(stderr, "No in.yaml in %s\n", fn);
           return 1;
       }
       //if (more)
       //    fprintf(stderr, "Also found %d more files in %s\n", more, fn);
   } else {
       fprintf(stderr, "Could not open dir: %s\n", fn);
       exit(1);
   }
   if (!fh) {
       perror("fopen");
       exit(1);
   }
   fseek(fh, 0, SEEK_END);
   fsize = ftell(fh);
   yaml = S_ALLOC_N(char, fsize + 1);
   fseek(fh, 0, SEEK_SET);
   yaml[fsize] = '\0';
   nread = fread(yaml, 1, fsize, fh);
   if (nread != fsize) {
       fprintf(stderr, "Unexpected file size %s: %zu != %zu read",
               fn, fsize, nread);
       exit(1);
   }

   puts("<\n--- # YAML");
   puts(yaml);
   puts("...\n");

   cs = CuStringNew();
   ev = CuStringNew();
   test_yaml_and_stream(cs, yaml, ev, should_fail); // display the errors

   if (outfh) {
       //CuStringAppend(cs, "---\n");
       if (!compare_cs(NULL, outfh, cs))
           printf("OK out.yaml matches\n");
       else {
           if (should_fail)
               printf("FAIL out.yaml but should fail\n");
           else
               printf("FAIL out.yaml does not match\n");
           retval++;
       }
       fclose(outfh);
   } else if (should_fail && cs->length == 0) {
       printf("OK parse should fail\n");
   } else {
       if (should_fail) {
           printf("FAIL should not parse\n");
           retval++;
       }
       else { // have no out.yaml to compare against
           puts(">\n--- # YAML");
           puts(cs->buffer);
           puts("...\n");
       }
   }

   if (testfh) {
       if (!should_fail) { // we have no event stream on parser errors
           if (!compare_cs(NULL, testfh, ev))
               printf("OK test.event matches\n");
           else {
               printf("TODO test.event does not match\n");
               //retval++;
           }
       }
       fclose(testfh);
   } else {
       puts(ev->buffer);
   }

   CuStringFree(cs);
   CuStringFree(ev);
   S_FREE(yaml);
   return retval;
}
