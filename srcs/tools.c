/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 15:25:51 by niragne           #+#    #+#             */
/*   Updated: 2020/01/04 15:26:17 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

void	replace_pattern(uint64_t pattern, char* mem, size_t size, uint64_t replace)
{
	int i = 0;
	while (i < size)
	{
		uint64_t *tmp;
		tmp = (uint64_t *)(mem + i);
		if (*tmp == pattern)
		{
			*tmp = replace;
		}
		i += 1;
	}
}
