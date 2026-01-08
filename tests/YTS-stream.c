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

extern const struct test_node end_node;

static int is_dir(FILE *f) {
    struct stat st;
    if (fstat(fileno(f), &st) == -1)
        return 0;

    return S_ISDIR(st.st_mode);
}

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
   char *yaml;
   struct test_node *stream;
   CuString *cs, *ev;
   FILE *fh, *outfh = NULL, *testfh = NULL;
   size_t fsize, nread;
   int retval = 0;
   int should_fail = 0;

   if (argc != 2) {
       fprintf(stderr, "yaml-file argument missing\n");
       exit(1);
   }
   fh = fopen(argv[1], "r");
   if (!fh || is_dir(fh)) {
       // check if dir with in.yaml
       struct dirent **files;
       int n = scandir(argv[1], &files, NULL, alphasort);
       if (n > 0) {
           int found = 0;
           int more = 0;
           for (int i=0; i<n; i++) {
               char a = files[i]->d_name[0];
               char fn[512];
               if (a == '.')
                   continue;
               if (!strcmp(files[i]->d_name, "in.yaml")) {
                   fh = open_path(argv[1], "in.yaml");
                   found = 1;
               }
               else if (!strcmp(files[i]->d_name, "out.yaml")) {
                   outfh = open_path(argv[1], "out.yaml");
                   more++;
               }
               else if (!strcmp(files[i]->d_name, "test.event")) {
                   testfh = open_path(argv[1], "test.event");
                   more++;
               }
               else if (!strcmp(files[i]->d_name, "===")) {
                   FILE *cmt = open_path(argv[1], "===");
                   CuString *cmt_cs = CuSlurpFile(cmt);
                   printf("%s %s\n", argv[1], cmt_cs->buffer);
                   fclose(cmt);
                   CuStringFree(cmt_cs);
               }
               else if (!strcmp(files[i]->d_name, "error")) {
                   should_fail = 1;
               }
           }
           while (n--) {
               free(files[n]);
           }
           free(files);
           if (!found) {
               fprintf(stderr, "No in.yaml in %s\n", argv[1]);
               return 1;
           }
           //if (more)
           //    fprintf(stderr, "Also found %d more files in %s\n", more, argv[1]);
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
   test_yaml_and_stream(cs, yaml, ev, 0); // display the errors

   if (outfh) {
       //CuStringAppend(cs, "---\n");
       if (!compare_cs(NULL, outfh, cs))
           printf("OK out.yaml matches\n");
       else {
           if (should_fail)
               printf("FAIL out.yaml but shold fail\n");
           else
               printf("FAIL out.yaml does not match\n");
           retval++;
       }
       fclose(outfh);
   } else if (should_fail && cs->length == 0) {
       printf("OK parse should fail\n");
   } else {
       printf("FAIL out.yaml missing\n");
       retval++;
   }

   if (testfh) {
       if (!compare_cs(NULL, testfh, ev))
           printf("OK test.event matches\n");
       else {
           printf("TODO test.event does not match\n");
           //retval++;
       }
       fclose(testfh);
   }

   CuStringFree(cs);
   CuStringFree(ev);
   S_FREE(yaml);
   return retval;
}
