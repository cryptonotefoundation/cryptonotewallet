// Copyright (c) 2014-2017, The Monero Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "hash-ops.h"

#if !defined(__FreeBSD__) && !defined(__OpenBSD__) && !defined(__DragonFly__)
 #include <alloca.h>
#else
 #include <stdlib.h>
#endif

/*** 
* Round to power of two, for count>=3 and for count being not too large (as reasonable for tree hash calculations)
*/
size_t tree_hash_cnt(size_t count) {
	// This algo has some bad history but all we are doing is 1 << floor(log2(count))
	// There are _many_ ways to do log2, for some reason the one selected was the most obscure one,
	// and fixing it made it even more obscure.
	//
	// Iterative method implemented below aims for clarity over speed, if performance is needed
	// then my advice is to use the BSR instruction on x86
	//
	// All the paranoid asserts have been removed since it is trivial to mathematically prove that
	// the return will always be a power of 2.
	// Problem space has been defined as 3 <= count <= 2^28. Of course quarter of a billion transactions
	// is not a sane upper limit for a block, so there will be tighter limits in other parts of the code

	assert( count >= 3 ); // cases for 0,1,2 are handled elsewhere
	assert( count <= 0x10000000 ); // sanity limit to 2^28, MSB=1 will cause an inf loop

	size_t pow = 2;
	while(pow < count) pow <<= 1;
	return pow >> 1;
}

void tree_hash(const char (*hashes)[HASH_SIZE], size_t count, char *root_hash) {
// The Monero blockchain block at height 202612 http://monerochain.info/block/bbd604d2ba11ba27935e006ed39c9bfdd99b76bf4a50654bc1e1e61217962698
// contained 514 transactions, that triggered bad calculation of variable "cnt" in the original version of this function
// as from CryptoNote code.
//
// This bug applies to all CN altcoins.
//
// Mathematical bug here was first published on 14:45:34 (GMT+2) 2014-09-04 by Rafal Freeman <rfree>
// https://github.com/rfree2monero/bitmonero/commit/b417abfb7a297d09f1bbb6de29030f8de9952ac8
// and soon also applied to CryptoNote (15:10 GMT+2), and BoolBerry used not fully correct work around:
// the work around of sizeof(size_t)*8 or <<3 as used before in 2 coins and in BBL later was blocking
// exploitation on normal platforms, how ever we strongly recommend the following fix because it removes
// mistake in mathematical formula.

  assert(count > 0);
  if (count == 1) {
    memcpy(root_hash, hashes, HASH_SIZE);
  } else if (count == 2) {
    cn_fast_hash(hashes, 2 * HASH_SIZE, root_hash);
  } else {
    size_t i, j;

    size_t cnt = tree_hash_cnt( count );

    char (*ints)[HASH_SIZE];
    size_t ints_size = cnt * HASH_SIZE;
    ints = alloca(ints_size); 	memset( ints , 0 , ints_size);  // allocate, and zero out as extra protection for using uninitialized mem

    memcpy(ints, hashes, (2 * cnt - count) * HASH_SIZE);

    for (i = 2 * cnt - count, j = 2 * cnt - count; j < cnt; i += 2, ++j) {
      cn_fast_hash(hashes[i], 64, ints[j]);
    }
    assert(i == count);

    while (cnt > 2) {
      cnt >>= 1;
      for (i = 0, j = 0; j < cnt; i += 2, ++j) {
        cn_fast_hash(ints[i], 64, ints[j]);
      }
    }

    cn_fast_hash(ints[0], 64, root_hash);
  }
}

size_t tree_depth(size_t count)
{
	size_t i;
	size_t depth = 0;
	assert(count > 0);
	for (i = sizeof(size_t) << 2; i > 0; i >>= 1)
	{
		if (count >> i > 0)
		{
			count >>= i;
			depth += i;
		}
	}
	return depth;
}

void tree_branch(const char(*hashes)[HASH_SIZE], size_t count, char(*branch)[HASH_SIZE])
{
	size_t i, j;
	size_t cnt = 1;
	size_t depth = 0;
	char(*ints)[HASH_SIZE];
	assert(count > 0);
	for (i = sizeof(size_t) << 2; i > 0; i >>= 1)
	{
		if (cnt << i <= count)
		{
			cnt <<= i;
			depth += i;
		}
	}
	assert(cnt == 1ULL << depth);
	assert(depth == tree_depth(count));
	ints = alloca((cnt - 1) * HASH_SIZE);
	memcpy(ints, hashes + 1, (2 * cnt - count - 1) * HASH_SIZE);
	for (i = 2 * cnt - count, j = 2 * cnt - count - 1; j < cnt - 1; i += 2, ++j)
	{
		cn_fast_hash(hashes[i], 2 * HASH_SIZE, ints[j]);
	}
	assert(i == count);
	while (depth > 0)
	{
		assert(cnt == 1ULL << depth);
		cnt >>= 1;
		--depth;
		memcpy(branch[depth], ints[0], HASH_SIZE);
		for (i = 1, j = 0; j < cnt - 1; i += 2, ++j)
		{
			cn_fast_hash(ints[i], 2 * HASH_SIZE, ints[j]);
		}
	}
}

void tree_hash_from_branch(const char(*branch)[HASH_SIZE], size_t depth, const char* leaf, const void* path, char* root_hash)
{
	if (depth == 0)
	{
		memcpy(root_hash, leaf, HASH_SIZE);
	}
	else
	{
		char buffer[2][HASH_SIZE];
		int from_leaf = 1;
		char *leaf_path, *branch_path;
		while (depth > 0)
		{
			--depth;
			if (path && (((const char*)path)[depth >> 3] & (1 << (depth & 7))) != 0)
			{
				leaf_path = buffer[1];
				branch_path = buffer[0];
			}
			else
			{
				leaf_path = buffer[0];
				branch_path = buffer[1];
			}
			if (from_leaf)
			{
				memcpy(leaf_path, leaf, HASH_SIZE);
				from_leaf = 0;
			}
			else
			{
				cn_fast_hash(buffer, 2 * HASH_SIZE, leaf_path);
			}
			memcpy(branch_path, branch[depth], HASH_SIZE);
		}
		cn_fast_hash(buffer, 2 * HASH_SIZE, root_hash);
	}
}
