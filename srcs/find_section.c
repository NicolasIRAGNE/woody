/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_section.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 14:54:42 by niragne           #+#    #+#             */
/*   Updated: 2019/12/17 09:56:05 by niragne          ###   ########.fr       */
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

Elf64_Phdr* find_codecave (void *d, int fsize, int *p, int *len)
{
  Elf64_Ehdr* elf_hdr = (Elf64_Ehdr *) d;
  Elf64_Phdr* elf_seg, *text_seg;
  int         n_seg = elf_hdr->e_phnum;
  int         i = 0;
  int         text_end = 0;
  int			gap=fsize;

  elf_seg = (Elf64_Phdr *) ((unsigned char*) elf_hdr 
			    + (unsigned int) elf_hdr->e_phoff);

	printf("nseg = %d\n", n_seg);
  	while (i < n_seg)
  	{
		printf("i = %d\n", i);
		if (elf_seg->p_type == PT_LOAD && elf_seg->p_flags & PF_X)
		{
		  printf ("+ Found .text segment (#%d)\n", i);
		  text_seg = elf_seg;
		  text_end = elf_seg->p_offset + elf_seg->p_filesz;
		}
  		else if (elf_seg->p_type == PT_LOAD && (elf_seg->p_offset - text_end) < gap && text_end != 0) 
		{
	    	printf ("   * Found LOAD segment (#%d) close to .text (offset: 0x%x)\n", i, (unsigned int)elf_seg->p_offset);
			gap = elf_seg->p_offset - text_end;
	    }
		elf_seg = (Elf64_Phdr *) ((unsigned char*) elf_seg + (unsigned int) elf_hdr->e_phentsize);
		i++;
	}

  *p = text_end;
  *len = gap;

  printf ("+ .text segment gap at offset 0x%x(%d bytes available)\n", text_end, gap);

  return text_seg;
}