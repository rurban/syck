/* This is a public domain general purpose hash table package written by Peter Moore @ UCB. */

/* @(#) st.h 5.1 89/12/14 */

#ifndef ST_INCLUDED
#define ST_INCLUDED

#ifdef IN_OLD_EXT_RUBY
// rename all st_ names to use our syck_st.h, not ruby's new API
#undef st_is_member
#undef st_init_table_with_size
#define st_retval        	syck_st_retval
#define ST_CONTINUE       	syck_ST_CONTINUE
#define ST_STOP	       	        syck_ST_STOP
#define ST_DELETE       	syck_ST_DELETE
#define st_is_member		syck_st_is_member
#define st_table_entry        	syck_st_table_entry
#define st_table         	syck_st_table
#define st_hash_type         	syck_st_hash_type
#define st_init_table_with_size syck_st_init_table_with_size
#define st_init_table           syck_st_init_table
#define st_init_numtable 	syck_st_init_numtable
#define st_init_numtable_with_size syck_st_init_numtable_with_size
#define st_init_strtable 	syck_st_init_strtable
#define st_init_strtable_with_size syck_st_init_strtable_with_size
#define st_free_table         	syck_st_free_table
#define st_lookup         	syck_st_lookup
#define st_insert         	syck_st_insert
#define st_add_direct         	syck_st_add_direct
#define st_copy         	syck_st_copy
#define st_delete         	syck_st_delete
#define st_delete_safe         	syck_st_delete_safe
#define st_foreach         	syck_st_foreach
#define st_cleanup_safe         syck_st_cleanup_safe
#endif

typedef struct st_table st_table;

struct st_hash_type {
    int (*compare)(const void * _x, const void * _y);
/*@relnull@*/
    int (*hash)(const void * _n);
};

struct st_table {
/*@relnull@*/
    struct st_hash_type *type;
    int num_bins;
    int num_entries;
/*@relnull@*/
    struct st_table_entry **bins;
};

#ifndef IN_OLD_EXT_RUBY
enum st_retval {ST_CONTINUE, ST_STOP, ST_DELETE};
#else
enum syck_st_retval {syck_ST_CONTINUE, syck_ST_STOP, syck_ST_DELETE};
#endif

#define st_is_member(table,key) st_lookup(table,key,(char **)0)

/*@null@*/
st_table *st_init_table(struct st_hash_type *type)
	/*@*/;
/*@null@*/
st_table *st_init_table_with_size(struct st_hash_type *type, int size)
	/*@*/;
/*@null@*/
st_table *st_init_numtable(void)
	/*@*/;
/*@null@*/
st_table *st_init_numtable_with_size(int size)
	/*@*/;
/*@null@*/
st_table *st_init_strtable(void)
	/*@*/;
/*@null@*/
st_table *st_init_strtable_with_size(int size)
	/*@*/;
void st_free_table(/*@only@*/ st_table *table)
	/*@modifies table @*/;
int st_lookup(st_table *table, const char *key, /*@out@*/ const void **value)
	/*@modifies *value @*/;
int st_insert(st_table *table, const char *key, const void *value)
	/*@modifies table @*/;
void st_add_direct(st_table *table, const char *key, const void *value)
	/*@modifies table @*/;
/*@null@*/
st_table *st_copy(st_table *old_table)
	/*@modifies old_table @*/;
int st_delete(st_table *table, const void **key, /*@out@*/ const void **value)
	/*@modifies table, *key, *value @*/;
int st_delete_safe(st_table *table, const void **key, /*@out@*/ const void **value,
		char *never)
	/*@modifies table, *key, *value @*/;
void st_foreach(st_table *table,
		enum st_retval (*func) (const char *key, void *record, void *arg),
		/*@null@*/ void *arg)
	/*@modifies table @*/;
void st_cleanup_safe(st_table *table, char *never)
	/*@modifies table @*/;

#define ST_NUMCMP	((int (*)()) 0)
#define ST_NUMHASH	((int (*)()) -2)

#define st_numcmp	ST_NUMCMP
#define st_numhash	ST_NUMHASH

#endif /* ST_INCLUDED */
