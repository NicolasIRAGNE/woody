/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_section.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 14:54:42 by niragne           #+#    #+#             */
/*   Updated: 2019/10/28 14:58:17 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <stdio.h>

Elf64_Shdr* find_section(void *ptr_elf, char *query)
{
	/*
		Set up the ptr to the elf header, section table.
		Also, get the total numbers of sections and declare var for the section name.
	*/
	Elf64_Ehdr *ehdr 	= (Elf64_Ehdr *) ptr_elf;
	Elf64_Shdr *elf_sec = (Elf64_Shdr *)(ptr_elf + ehdr->e_shoff);
	int total_sec = ehdr->e_shnum;
	char *sname;

	/*
		Create a list that would fit all the section strings.
			
	*/
	Elf64_Shdr *sec_strtab 			 = &elf_sec[ehdr->e_shstrndx];
	char *const sec_strtab_ptr = ptr_elf + sec_strtab->sh_offset; 

	ft_printf("[+] Searching for %s section.\n", query);

	/*

	*/
	for(int i = 0; i < total_sec; i++)
	{
		sname = sec_strtab_ptr + elf_sec[i].sh_name;
		if(!ft_strcmp(sname, query))
		{
			ft_printf("[+] %s section found.\n", query);
			return &elf_sec[i];	
		} 
	}

	return NULL;
}