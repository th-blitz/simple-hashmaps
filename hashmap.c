#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

// (c) th.blitz from discord. 

// Name of the HashTable : SimpleTables. 

struct SimpleTable {
	uint8_t** bucket; // (key:value pair bucket)
	uint64_t size; // (current hashmap size)
	uint64_t max_size; // (hashmap max size)
	uint64_t bucket_size; // (bucket size)
	uint64_t max_collisions; // (max collisions recorded at any certain point)
	uint64_t pseudo_random_number; // (pseudo random number)
	uint64_t table_options; // (table options, 64 options as 1s and 0s) 
};

struct SmpleTableFunctions {
	struct SimpleTable (*malloc)(struct SimpleTable hashmap, uint64_t table_size);
};

struct SimpleTable get_a_SimpleTable() {

	struct SimpleTable hashmap;
	hashmap.bucket = 0;
	hashmap.size = 0;
	hashmap.max_size = 0;
	hashmap.bucket_size = 0;
	hashmap.max_collisions = 0;
	hashmap.pseudo_random_number = 0;
	hashmap.table_options = 0;
	
	return hashmap;
}



uint64_t nearest_prime(uint64_t num) {
	
	uint64_t primes_size = (num + 2) / 8;
	num += 1;		
	if (((num + 1) % 8) != 0) {
		primes_size += 1;
	}

	uint8_t* primes = (uint8_t*)malloc( primes_size * sizeof(uint8_t));
	
	if (primes == 0) {
		printf("- Error : Maximum allocation size reached while calling the malloc(allocation size) function \n");
		return 0;	
	}

	for (uint64_t i = 0; i < primes_size; i++) {	
		primes[i] = 0xff;
	}

	uint8_t mask[8] = { 
		0x01, 
		0x02,
		0x04,
		0x08,
		0x10,
		0x20,
		0x40,
		0x80
	};


		
	uint8_t not_of_mask[8];	
	for (uint8_t i = 0; i < 8; i++) {
		not_of_mask[i] = ~mask[i];
	}
	
	for (uint64_t i = 2; i * i <= num; i++) {
		if ((primes[i/8] & mask[i % 8]) > 0) {
			for (uint64_t j = i * i; j <= num; j += i) {
				primes[j/8] &= (not_of_mask[j % 8]);
			}
		}
	}

	uint64_t prime = 0;
	for (uint64_t i = 0; i <= num; i++) {
		uint64_t j = num - i - 1;
		if ((primes[j/8] & mask[j % 8]) > 0) {
			prime = j;
			break;
		}
	}

//	FILE *file_pointer;

//	file_pointer = fopen("primes_cache", "w");
//	fwrite(primes, sizeof(uint8_t), primes_size, file_pointer);
//	fclose(file_pointer);

	free(primes);
	return prime;
}

struct SimpleTable malloc_a_simpletable(struct SimpleTable hashmap, uint64_t table_size) {
	
	uint64_t bucket_size = (table_size * 3) / 2;

	uint64_t nearest_prime_number = nearest_prime(bucket_size);
	
	hashmap.bucket = (uint64_t**)malloc(nearest_prime_number * sizeof(uint64_t*));
	hashmap.slot_info = (uint8_t*)malloc(nearest_prime_number * sizeof(uint8_t));
	
	for (uint64_t i = 0; i < nearest_prime_number; i++) {
		hashmap.bucket[i] = (uint64_t*)malloc(2 * sizeof(uint64_t));
		hashmap.slot_info[i] = 0x00;
		hashmap.bucket[i][0] = 0;
		hashmap.bucket[i][1] = 0;
	}

	hashmap.bucket_size = nearest_prime_number;
	hashmap.max_size = table_size;
	return hashmap;	
}

struct SimpleTable free_a_simpletable(struct SimpleTable hashmap) {
	
	for (uint64_t i = 0; i < hashmap.bucket_size; i++) {
		free(hashmap.bucket[i]);
	}

	free(hashmap.bucket);
	free(hashmap.slot_info);

	hashmap.bucket_size = 0;
	hashmap.max_size = 0;
	hashmap.size = 0;
	hashmap.max_collisions = 0;
	
	return hashmap;
}

// http://www.cse.yorku.ca/~oz/hash.html ( hash functions like djb2 and sdbm )
uint64_t hash_sdbm_for_simpletables(uint64_t key) {

	uint64_t hash = 793702983892; // some random prime number. 

	for (uint64_t i = 0; i < 64; i++) {
		hash = (key & (1 << i)) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

uint64_t probe_bucket_slot_for_simpletables(uint64_t slot, uint64_t number_of_collisions, uint64_t rand) {
	return slot + (number_of_collisions * number_of_collisions) + rand;
}



int main() {
//	uint64_t prime = nearest_prime(100000000000);
//	printf("%llu\n", prime);

	struct SimpleTable hashmap = get_a_SimpleTable();
	
	hashmap = malloc_a_simpletable(hashmap, 13892);

	printf("%lu\n", hashmap.bucket_size);	

	free_a_simpletable(hashmap);
	return 0;
}

