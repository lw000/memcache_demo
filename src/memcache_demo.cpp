//============================================================================
// Name        : memcache_demo.cpp
// Author      : lw
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

#include <libmemcached/memcached.h>

int main() {
	memcached_st *memc;
	memcached_return rc;
	memcached_server_st *servers;
	char value[8191];
	//connect multi server
	memc = memcached_create(NULL);
	// memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_SORT_HOSTS, 1);
	memcached_behavior_set(memc, MEMCACHED_BEHAVIOR_DISTRIBUTION,
			MEMCACHED_DISTRIBUTION_CONSISTENT);
	servers = memcached_server_list_append(NULL, "localhost", 11211, &rc);
	//  servers = memcached_server_list_append(servers, "localhost", 21201, &rc);
	servers = memcached_server_list_append(servers, "localhost", 11211, &rc);
	//   servers = memcached_server_list_append(servers, "localhost",11216,&rc);
	rc = memcached_server_push(memc, servers);
	//    int count = memcached_server_list_count(servers);
	int count2 = memcached_server_count(memc);

	fprintf(stdout, "server count is%d/n", memc->number_of_hosts);
	memcached_server_free(servers);

	//Save multi data
	size_t i;
	const char *keys[] = { "key1", "key2", "key3", "key4" };
	const size_t key_length[] = { 4, 4, 4, 4 };
	char *values[] = { "This is c first value", "This is c second value",
			"This is c third value", " this is c forth value" };
	size_t val_length[] = { 21, 22, 21, 21 };
	for (i = 0; i < 4; i++) {
		rc = memcached_set(memc, keys[i], key_length[i], values[i],
				val_length[i], (time_t) 180, (uint32_t) 0);
		if (rc == MEMCACHED_SUCCESS) {
			printf("Save key:%s data:\"%s\"success./n", keys[i], values[i]);
		}
	}

	//Fetch multi data
	char return_key[MEMCACHED_MAX_KEY];
	size_t return_key_length;
	char *return_value;
	size_t return_value_length;
	uint32_t flags;
	rc = memcached_mget(memc, keys, key_length, 4);
	while ((return_value = memcached_fetch(memc, return_key, &return_key_length,
			&return_value_length, &flags, &rc))) {
		if (rc == MEMCACHED_SUCCESS) {
			printf("Fetch key:%s data:%s/n", return_key, return_value);
		}
	}

	//Delete multi data
	for (i = 0; i < 4; i++) {
		rc = memcached_set(memc, keys[i], key_length[i], values[i],
				val_length[i], (time_t) 180, (uint32_t) 0);
		rc = memcached_delete(memc, keys[i], key_length[i], (time_t) 0);
		if (rc == MEMCACHED_SUCCESS) {
			printf("Delete %s %s success/n", keys[i], values[i]);
		}
	}
	//free
	memcached_free(memc);
	return 0;
}
