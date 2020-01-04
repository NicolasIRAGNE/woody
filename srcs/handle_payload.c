/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_payload.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: niragne <niragne@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/04 15:23:55 by niragne           #+#    #+#             */
/*   Updated: 2020/01/04 15:26:58 by niragne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"
#include <stdio.h>

int    get_payload(struct s_woody* wrapper)
{
	if (fill_file(DEFAULT_PAYLOAD, wrapper->file_to_inject))
	{
		perror(wrapper->prog_name);
		return (1);
	}
	Elf64_Shdr* p_txt_sec_ptr = find_section(wrapper->file_to_inject->ptr, ".text");
	wrapper->payload.size = p_txt_sec_ptr->sh_size;
	wrapper->payload.data = wrapper->file_to_inject->ptr + p_txt_sec_ptr->sh_offset;
	int i = wrapper->payload.size;
	replace_pattern(0x1111111111111111L, wrapper->payload.data, wrapper->payload.size, wrapper->old_entry - wrapper->new_entry);	
}

int		inject_payload(struct s_woody* wrapper)
{
	int offset;
	int available_size;
	Elf64_Phdr* t_txt_seg_ptr = find_codecave(wrapper->file_to_pack->ptr, wrapper->file_to_pack->st.st_size, &offset, &available_size);
	if ((void*)t_txt_seg_ptr < wrapper->file_to_pack->ptr)
		return (1);
	
	printf("SIZE = %x\n", t_txt_seg_ptr->p_filesz);
	
	// rc4_crypt(wrapper->file_to_pack->ptr + t_txt_seg_ptr->p_offset, t_txt_seg_ptr->p_filesz, wrapper->key);
	printf("%llx %llx\n", t_txt_seg_ptr->p_offset, t_txt_seg_ptr->p_filesz);
	rc4_crypt(wrapper->file_to_pack->ptr + t_txt_seg_ptr->p_offset, t_txt_seg_ptr->p_filesz, wrapper->key);
	wrapper->file_to_pack->header_union.elf64->e_entry = (Elf64_Addr)(offset);
	wrapper->new_entry = (void*)(uint64_t)offset;
	get_payload(wrapper);

	if(wrapper->payload.size > available_size)
	{
		printf("[!] Payload too big to inject.\n");
		return (1);
	}
	replace_pattern(0x2222222222222222L, wrapper->payload.data, wrapper->payload.size, (uint64_t)(t_txt_seg_ptr->p_offset - (uint64_t)wrapper->new_entry));	
	replace_pattern(0x3333333333333333L, wrapper->payload.data, wrapper->payload.size, (uint64_t)(t_txt_seg_ptr->p_filesz));	
	replace_pattern(0x4444444444444444L, wrapper->payload.data, wrapper->payload.size, *(uint64_t*)(wrapper->key));	

	printf("%llx\n", (uint64_t)(t_txt_seg_ptr->p_offset - (uint64_t)wrapper->new_entry));
	printf("%llx\n", (uint64_t)((uint64_t)(t_txt_seg_ptr->p_filesz)));
	printf("%llx\n", (uint64_t)(*(uint64_t*)(wrapper->key)));

	t_txt_seg_ptr->p_flags |= PF_W;


	memmove(wrapper->file_to_pack->ptr + offset, wrapper->payload.data, wrapper->payload.size);
	if (write(wrapper->new_file->fd, wrapper->file_to_pack->ptr, wrapper->file_to_pack->st.st_size) == 0)
		perror("MERDE");

	return (0);
}