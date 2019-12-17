/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc4.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 15:32:02 by niragne           #+#    #+#             */
/*   Updated: 2019/12/17 14:57:38 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

uint64_t	generate_key(void)
{
	long k = rand();
	k = (k << 32) | rand();
	return (k);
}

void swap(unsigned char *a, unsigned char *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void	rc4_crypt(char* mem, size_t size, uint8_t key[8])
{
	int i = 0;
	int j = 0;
	int k = 0; 
	uint8_t perm[256];

	while (i < sizeof(perm))
	{
		perm[i] = i;
		i++;
	}
	
	i = 0;
	
	while (i < sizeof(perm))
	{
		//j = (j + perm[i] + key[i % 8]) % sizeof(perm);
		j = (j + perm[i]) % sizeof(perm);
		printf("SWAP %x %x\n", i, j);
		printf("PERM %x %x\n", perm[i], perm[j]);
		swap(&perm[i], &perm[j]);
		i++;
	}
	int banane = 0;
	while (banane < 256)
		printf("%x ", perm[banane++]);
	printf("\n");
	i = 0;
	j = 0;
	while (k < size)
	{
		i = (i + 1) % sizeof(perm);
		j = (j + perm[i]) % sizeof(perm);
		swap(&perm[i], &perm[j]);
		int rnd = perm[(perm[i] + perm[j]) % sizeof(perm)];
		printf("rnd = %x i = %x j = %x xor = %x\n", rnd, i, j, rnd ^ mem[k]);
		mem[k] = rnd ^ mem[k];
		k++;
	}
}