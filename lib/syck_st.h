/* This is a public domain general purpose hash table package written by Peter Moore @ UCB. */

/* @(#) st.h 5.1 89/12/14 */

#ifndef ST_INCLUDED
#define ST_INCLUDED

typedef struct st_table st_table;
typedef st_data_t st_index_t;

struct st_hash_type {
    int (*compare)(const void * _x, const void * _y);
    st_index_t (*hash)(const void * _n);
};

struct st_table {
    struct st_hash_type *type;
    st_index_t num_bins;
    st_index_t num_entries;
    struct st_table_entry **bins;
};

enum st_retval {ST_CONTINUE, ST_STOP, ST_DELETE /*, ST_CHECK, ST_REPLACE*/};

#define st_is_member(table,key) st_lookup(table,key,(st_data_t*)0)

st_table *st_init_table(const struct st_hash_type *type);
st_table *st_init_table_with_size(const struct st_hash_type *type, st_index_t size);
st_table *st_init_numtable(void);
st_table *st_init_numtable_with_size(st_index_t size);
st_table *st_init_strtable(void);
st_table *st_init_strtable_with_size(st_index_t size);
void st_free_table(st_table *table);
int st_lookup(st_table *table, st_data_t key, /*@out@*/ st_data_t *value);
int st_insert(st_table *table, st_data_t key, st_data_t value);
void st_add_direct(st_table *table, st_data_t key, st_data_t value);
st_table *st_copy(st_table *old_table);
int st_delete(st_table *table, st_data_t *key, /*@out@*/ st_data_t *value);
int st_delete_safe(st_table *table, st_data_t *key,
		   /*@out@*/ st_data_t *value, st_data_t never);
void st_foreach(st_table *table,
		enum st_retval (*func)(const char *key, void *record, void *arg),
		void *arg);
void st_cleanup_safe(st_table *table, char *never);

#define ST_NUMCMP	((int (*)()) 0)
#define ST_NUMHASH	((int (*)()) -2)

#endif /* ST_INCLUDED */
