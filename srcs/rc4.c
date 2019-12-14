/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc4.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/14 15:32:02 by niragne           #+#    #+#             */
/*   Updated: 2019/12/14 16:02:08 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

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
		j = (j + perm[i] + key[i % 8]) % sizeof(perm);
		swap(&perm[i], &perm[j]);
		i++;
	}
	i = 0;
	j = 0;
	while (k < size)
	{
		i = (i + 1) % sizeof(perm);
		j = (j + perm[i]) % sizeof(perm);
		swap(&perm[i], &perm[j]);
		int rnd = perm[(perm[i] + perm[j]) % sizeof(perm)];
		mem[k] = rnd ^ mem[k];
		k++;
	}
}