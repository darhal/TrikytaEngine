/*#include <lua.hpp>

struct crypt_data {
	FILE* fh;
	char key[SIZEOFKEY];
	char buff[SIZEOFCHUNK];
	char *p;
	int n;
};

const char * crypt_reader(lua_State *L, void *ud, size_t *sz) {
	int n;
	char *p;
	struct crypt_data *data = "" > if (!data->p || data->n <= 0) {
		data->n = fread(data->buff, SIZEOFCHUNK, sizeof(char), fh);
		data->p = data->buff;
	}
	p = data->p;
	n = data->n;
	data->p = NULL;
	data->n = 0;
	//decrypt using p, n, data->key <-- your code 
	*sz = n;
	return p;
}

void load_crypted(lua_State*L, FILE* fh) {
	struct crypt_data my_data;
	my_data.fh = fh;
	my_data.n = fread(my_data.buff, SIZEOFCHUNK, sizeof(char), fh);
	memcpy(my_data.key, my_data.buff, SIZEOFKEY * sizeof(char));
	p = &(my_data.buff[SIZEOFKEY]);
	my_data.n -= SIZEOFKEY;
	lua_load(L, crypt_reader, &my_data, "name of file");
}*/